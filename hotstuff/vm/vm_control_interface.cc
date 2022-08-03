#include "hotstuff/vm/vm_control_interface.h"
#include "hotstuff/hotstuff_debug_macros.h"

#include "hotstuff/vm/vm_base.h"

namespace hotstuff
{

VMControlInterface::VMControlInterface(std::shared_ptr<VMBase> vm)
    : utils::AsyncWorker()
    , vm_instance(vm)
    , blocks_to_validate()
    , proposal_buffer()
    , additional_proposal_requests(0)
    , is_proposer(false)
    , highest_committed_id(std::nullopt)
{
    start_async_thread([this] { run(); });
}

VMControlInterface::~VMControlInterface()
{
    end_async_thread();
}

void
VMControlInterface::set_proposer()
{
    std::lock_guard lock(mtx);
    HOTSTUFF_INFO("VM INTERFACE: entering proposer mode");
    is_proposer = true;
}

VMControlInterface::proposal_buffer_t
VMControlInterface::get_proposal()
{
    std::unique_lock lock(mtx);
    HOTSTUFF_INFO("VM INTERFACE: start get_proposal");
    if (!is_proposer)
    {
        HOTSTUFF_INFO(
            "VM INTERFACE: not in proposer mode, returning empty proposal");
        return nullptr;
    }

    if (done_flag)
        return nullptr;

    if (proposal_buffer.empty())
    {
        if (additional_proposal_requests < PROPOSAL_BUFFER_TARGET)
        {
            additional_proposal_requests = PROPOSAL_BUFFER_TARGET;
        }
        if (PROPOSAL_BUFFER_TARGET == 0)
        {
            HOTSTUFF_INFO("VM INTERFACE: cannot get proposal from empty buffer "
                          "after proposals are stopped");
            return nullptr;
        }
        cv.notify_all();
        HOTSTUFF_INFO("VM INTERFACE: waiting on new proposal from vm");
        cv.wait(lock,
                [this] { return done_flag || (!proposal_buffer.empty()); });
    }

    if (done_flag)
        return nullptr;

    HOTSTUFF_INFO("VM INTERFACE: got new proposal from vm");

    auto out = std::move(proposal_buffer.front());
    proposal_buffer.erase(proposal_buffer.begin());

    additional_proposal_requests
        = (proposal_buffer.size() > PROPOSAL_BUFFER_TARGET)
              ? 0
              : PROPOSAL_BUFFER_TARGET - proposal_buffer.size();
    if (additional_proposal_requests > 0)
    {
        cv.notify_all();
    }
    return out;
}

void
VMControlInterface::clear_proposal_settings()
{
    is_proposer = false;
    proposal_buffer.clear();
    additional_proposal_requests = 0;
}

void
VMControlInterface::submit_block_for_exec(submission_t submission)
{
    std::lock_guard lock(mtx);

    clear_proposal_settings();

    blocks_to_validate.push_back(std::move(submission));

    cv.notify_all();
}

//! log_commitment should be called in order in which things are committed
//! Note that it is possible, albeit likely rare, that
//! some block_ids would get skipped
//! (if, for example, this method gets called twice in succession
//! before the run() thread wakes on the cv).
void
VMControlInterface::log_commitment(VMBlockID const& block_id)
{
    std::lock_guard lock(mtx);

    if (block_id)
    {
        highest_committed_id = block_id;
    }
    if (highest_committed_id)
    {
        cv.notify_all();
    }
}

void
VMControlInterface::run()
{
    HOTSTUFF_INFO("VM INTERFACE: start run()");
    while (true)
    {
        std::unique_lock lock(mtx);

        if ((!done_flag) && (!exists_work_to_do()))
        {
            cv.wait(lock,
                    [this]() { return done_flag || exists_work_to_do(); });
        }

        if (done_flag)
            return;

        while (!blocks_to_validate.empty())
        {
            auto block_to_validate = std::move(blocks_to_validate.front());
            blocks_to_validate.erase(blocks_to_validate.begin());

            // block_to_validate is nullptr iff parsing failed.
            // In this case, vm doesn't do anything.
            if (block_to_validate)
            {
                // exec_block is responsible for reverting any speculative state
                // left over from calls to propose()
                HOTSTUFF_INFO("VM INTERFACE: start exec_block");
                vm_instance->exec_block(*block_to_validate);
                HOTSTUFF_INFO("VM INTERFACE: end exec block");
            }
        }
        if (highest_committed_id)
        {
            vm_instance->log_commitment(*highest_committed_id);
            highest_committed_id = std::nullopt;
        }
        if (additional_proposal_requests > 0)
        {
            // get additional proposals
            if (!is_proposer)
            {
                throw std::runtime_error("vm thread woke up early");
            }

            // vm->propose() leaves vm in speculative future state.
            proposal_buffer.emplace_back(vm_instance->propose());
            additional_proposal_requests--;
        }
        cv.notify_all();
    }
}

//! wait for the vm to finish whatever it's doing.
//! Then rewinds the vm to its last committed state.
//! Note: this method does not acquire a lock for the whole
//! duration of its execution,
//! so proposals and other calls into the vm
//! should not be executed while this runs.
//! In this implementation, this property is guaranteed
//! by the fact that apply_block is not called
//! concurrently with notify_vm_of_commitment
//! (in the vm bridge)
//! and get_and_apply_next_proposal and apply_block
//! both acquire a lock on speculation_map.
//! TODO this arrangement is fragile, and would be easy
//! to accidentally break.
void
VMControlInterface::finish_work_and_force_rewind()
{
    wait_for_async_task();
    vm_instance->rewind_to_last_commit();
}

void
VMControlInterface::init_clean()
{
    vm_instance->init_clean();
}

void
VMControlInterface::init_from_disk(LogAccessWrapper const& decided_block_cache)
{
    vm_instance->init_from_disk(decided_block_cache);
}

} // namespace hotstuff
