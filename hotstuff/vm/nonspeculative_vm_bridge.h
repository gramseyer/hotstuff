#pragma once

/** \file nonspeculative_vm_bridge.h
 *
 * The "VM Bridge" is the main interaction layer
 * between hotstuff consensus and the replicated state machine.
 *
 * This version does not execute commands speculatively.
 *
 * Not at all tested, your mileage may vary.
 *
 */
#include "hotstuff/hotstuff_debug_macros.h"

#include "hotstuff/lmdb.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

#include <xdrpp/types.h>

namespace hotstuff
{

class LogAccessWrapper;
class VMBase;
class VMBlock;
class VMBlockID;

/**
 * Bridge between hotstuff consensus and the replicated
 * state machine.
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
class NonspeculativeVMBridge
{

    using vm_block_id = VMBlockID;
    using vm_block_type = VMBlock;

    // TODO: rewrite with a non-speculative version of control interface
    //  cut out some of the extra code, essentially
    std::shared_ptr<VMBase> vm;

    bool initialized;

    mutable std::mutex mtx;
    std::vector<xdr::opaque_vec<>> proposal_buffer;

    //! map block_type to block_id, considering the fact
    //! that the input block might be null
    //! (in which case, this returns a default, null_id value).
    vm_block_id get_block_id(std::unique_ptr<vm_block_type> const& blk);

    void init_guard() const
    {
        if (!initialized)
        {
            throw std::runtime_error("uninitialized vm!");
        }
    }

public:
    NonspeculativeVMBridge(std::shared_ptr<VMBase> vm)
        : vm(vm)
        , initialized(false)
    {}

    //! Initialize state machine and speculation gadget to clean slate.
    void init_clean();

    //! Initialize the state machine and the speculation gadget from disk
    void init_from_disk(LogAccessWrapper const& decided_block_index);

    //! Generate an empty proposal.
    //! Useful when switching leaders (i.e. leader can generate a string of
    //! empty proposals easily, which serves as a leader election process).
    xdr::opaque_vec<> make_empty_proposal(uint64_t proposal_height)
    {
        init_guard();
        VM_BRIDGE_INFO("made empty proposal at height %lu", proposal_height);
        return xdr::opaque_vec<>();
    }

    //! Get proposal from proposal buffer
    //! Note - name difference from regular vm bridge -- this'll need to
    //! change in call sites to use this version.
    xdr::opaque_vec<> get_next_proposal(uint64_t proposal_height);

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

    void notify_vm_of_commitment(block_ptr_t blk)
    {
        // no op
    }

    //! checks whether the vm has any proposals to give.
    //! main use case is managing an orderly system shutdown.
    bool proposal_buffer_is_empty() const
    {
        std::lock_guard lock(mtx);
        return proposal_buffer.size() == 0;
    }

    void add_proposal(xdr::opaque_vec<>&& proposal)
    {
        std::lock_guard lock(mtx);
        proposal_buffer.insert(proposal_buffer.begin(), std::move(proposal));
    }
};

} // namespace hotstuff
