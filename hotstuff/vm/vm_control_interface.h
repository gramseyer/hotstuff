#pragma once

#include "hotstuff/vm/vm_base.h"
#include "utils/async_worker.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

namespace hotstuff
{

class LogAccessWrapper;

/**
 * Wraps the virtual machine in an asynchronous interface
 * that buffers requests in and out of the machine.
 *
 * Manages a proposal buffer and a buffer of decided blocks
 * that the virtual machine must handle.
 *
 */
class VMControlInterface : public utils::AsyncWorker
{
    std::shared_ptr<VMBase> vm_instance;

    using utils::AsyncWorker::cv;
    using utils::AsyncWorker::mtx;

    using proposal_buffer_t = std::unique_ptr<VMBlock>;
    using submission_t = std::unique_ptr<VMBlock>;

    size_t PROPOSAL_BUFFER_TARGET = 3;

    std::vector<submission_t> blocks_to_validate;

    std::vector<proposal_buffer_t> proposal_buffer;

    size_t additional_proposal_requests;

    bool is_proposer;

    std::optional<VMBlockID> highest_committed_id;

    void clear_proposal_settings();

    // vm runner side
    void run();

    bool exists_work_to_do() override final
    {
        return (blocks_to_validate.size() > 0)
               || ((additional_proposal_requests > 0) && is_proposer)
               || ((bool)highest_committed_id);
    }

public:
    VMControlInterface(std::shared_ptr<VMBase> vm);
    ~VMControlInterface();

    // from hotstuff side
    void set_proposer();
    proposal_buffer_t get_proposal();
    void submit_block_for_exec(submission_t submission);
    void log_commitment(VMBlockID const& block_id);
    void finish_work_and_force_rewind();

    // call before any usage
    void init_clean() { vm_instance->init_clean(); }
    void init_from_disk(LogAccessWrapper const& decided_block_cache)
    {
        vm_instance->init_from_disk(decided_block_cache);
    }

    // Called by external controls (i.e. from main logic component)
    bool proposal_buffer_is_empty() const
    {
        std::lock_guard lock(mtx);
        return proposal_buffer.empty() && (PROPOSAL_BUFFER_TARGET == 0)
               && (additional_proposal_requests == 0);
    }

    void stop_proposals()
    {
        std::lock_guard lock(mtx);
        PROPOSAL_BUFFER_TARGET = 0;
    }
};

} // namespace hotstuff
