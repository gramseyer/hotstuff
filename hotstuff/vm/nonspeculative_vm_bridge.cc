#include "hotstuff/vm/nonspeculative_vm_bridge.h"

#include "hotstuff/vm/vm_base.h"
#include "hotstuff/vm/vm_block_id.h"

namespace hotstuff
{

//! Initialize state machine and speculation gadget to clean slate.
void

NonspeculativeVMBridge::init_clean()
{
    vm->init_clean();
    initialized = true;
}

//! Initialize the state machine and the speculation gadget from disk
void
NonspeculativeVMBridge::init_from_disk(
    LogAccessWrapper const& decided_block_index)
{
    vm->init_from_disk(decided_block_index);
    initialized = true;
}

NonspeculativeVMBridge::vm_block_id
NonspeculativeVMBridge::get_block_id(std::unique_ptr<vm_block_type> const& blk)
{
    if (blk)
    {
        return VMBase::nonempty_block_id(*blk);
    }
    return VMBase::empty_block_id();
}

xdr::opaque_vec<>
NonspeculativeVMBridge::get_next_proposal(uint64_t proposal_height)
{
    init_guard();
    VM_BRIDGE_INFO("start get_next_proposal for height %lu", proposal_height);
    std::lock_guard lock(mtx);

    if (proposal_buffer.size() == 0)
    {
        VM_BRIDGE_INFO("try make nonempty, got empty proposal at height %lu",
                       proposal_height);
        return xdr::opaque_vec<>();
    }

    auto out = std::move(proposal_buffer.back());
    proposal_buffer.pop_back();

    return out;
}

//! Apply a block to the state machine.
//! Note that this block must be decided by consensus
//! (or else the block might be falsely thought to be committed
//! upon a crash recovery).
//! Logs the block within the speculation gadget, and
//! rewinds the state machine if the applied block
//! does not match what was speculatively executed.
//! Always expects notify_vm_of_commitment to be called immediately
//! afterwards.
void
NonspeculativeVMBridge::apply_block(block_ptr_t blk, HotstuffLMDB::txn& txn)
{
    init_guard();

    auto blk_value = vm->try_parse(blk->get_wire_body());
    auto blk_id = get_block_id(blk_value);

    txn.add_decided_block(blk, blk_id);

    VM_BRIDGE_INFO("submitting height %lu for exec", blk->get_height());
    // vm_interface.submit_block_for_exec(std::move(blk_value));

    // TODO this call is blocking
    vm->exec_block(*blk_value);
    VM_BRIDGE_INFO("done submit for exec %lu", blk->get_height());
}

} // namespace hotstuff