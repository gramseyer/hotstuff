#pragma once

/** \file hotstuff_vm_bridge.h
 *
 * The "VM Bridge" is the main interaction layer
 * between hotstuff consensus and the replicated state machine.
 *
 */
#include "hotstuff/hotstuff_debug_macros.h"

#include "hotstuff/vm/speculative_exec_gadget.h"
#include "hotstuff/vm/vm_control_interface.h"
#include "hotstuff/lmdb.h"

#include <cstdint>
#include <memory>

namespace hotstuff
{

class LogAccessWrapper;
class VMBase;

/**
 * Bridge between hotstuff consensus and the replicated
 * state machine.
 *
 * This is where speculative application of blocks is tracked.
 * The "speculative exec gadget" tracks which blocks are applied
 * speculatively or which are committed,
 * so that when apply_block is called,
 * this bridge knows whether the VM has speculatively applied the
 * block already, or whether to revert the VM to committed state.
 *
 * Important invariant: the value of vm_block_id should be in 1-1 correspondence
 * from block contents (block semantic meaning).  One might use vm_block_id =
 * sha256(block_type). The id is used in disambiguating blocks during the
 * speculation. This class needs to know, for example, how to differentiate
 * command A (applied, but only speculatively, at hotstuff height X) from
 * command B (also applied at hotstuff height X, and committed, i.e. by being
 * proposed by another replica.
 *
 * For every block, hotstuff calls apply_block and then notify_vm_of_commitment.
 * (Yes, this is redundant, and may change in the future.  These two methods
 * could be easily combined).
 * In theory, one could make these two processes separate (and the initial plan
 * was to do so) but that separation makes speculation tracking much more
 * complicated and error-prone.
 */

struct HotstuffConfigs;

class HotstuffVMBridge
{

    using vm_block_id = VMBlockID;
    using vm_block_type = VMBlock;

    SpeculativeExecGadget<vm_block_id> speculation_map;

    std::shared_ptr<VMBase> vm;
    VMControlInterface vm_interface;

    bool initialized;

    //! clear the speculation gadget.
    void revert_to_last_commitment();
    //! map block_type to block_id, considering the fact
    //! that the input block might be null
    //! (in which case, this returns a default, null_id value).
    static vm_block_id get_block_id(std::unique_ptr<vm_block_type> const& blk);

    void init_guard() const;

public:
    HotstuffVMBridge(std::shared_ptr<VMBase> vm, HotstuffConfigs const& configs);

    //! Initialize state machine and speculation gadget to clean slate.
    void init_clean();

    //! Initialize the state machine and the speculation gadget from disk
    void init_from_disk(LogAccessWrapper const& decided_block_index,
                        uint64_t decided_hotstuff_height);
    //! Generate an empty proposal.
    //! Useful when switching leaders (i.e. leader can generate a string of
    //! empty proposals easily, which serves as a leader election process).
    xdr::opaque_vec<> make_empty_proposal(uint64_t proposal_height);

    //! Get proposal from state machine.
    //! Implicitly, getting a proposal tells the vm interface that it can
    //! speculatively apply the proposal (if it does not, it should store enough
    //! information so as to be able to apply it later when the proposal
    //! commits).
    xdr::opaque_vec<> get_and_apply_next_proposal(uint64_t proposal_height);

    //! Apply a block to the state machine.
    //! Note that this block must be decided by consensus
    //! (or else the block might be falsely thought to be committed
    //! upon a crash recovery).
    //! Logs the block within the speculation gadget, and
    //! rewinds the state machine if the applied block
    //! does not match what was speculatively executed.
    //! Always expects notify_vm_of_commitment to be called immediately
    //! afterwards.
    void apply_block(block_ptr_t blk, HotstuffLMDB::txn& txn);
    //! Notify the vm that a block has committed.
    //! Should always be called immediately after apply_block.
    void notify_vm_of_commitment(block_ptr_t blk);
    //! Notify the VM that it should be ready to generate proposals.
    void put_vm_in_proposer_mode();
    //! checks whether the vm has any proposals to give.
    //! main use case is managing an orderly system shutdown.
    bool proposal_buffer_is_empty() const;

    //! For orderly shutdown -- tell the vm to stop making new proposals.
    void stop_proposals();
};

} // namespace hotstuff
