#pragma once

#include "hotstuff/xdr/types.h"
namespace hotstuff
{

class ReplicaConfig;
class VMBase;
class HotstuffConfigs;

/**!
 * Base class for replicated state machine.
 * Methods here are public methods that can be called
 * by external logic.
 *
 * Some of the methods are implemented by one but not the other
 * of the instantiations of this class.
 *
 * This class is threadsafe.
 */
class Hotstuff
{
    [[noreturn]] void unimplemented() const
    {
        throw std::runtime_error("unimplemented");
    }

public:
    virtual void init_clean() = 0;
    virtual void init_from_disk() = 0;
    virtual void put_vm_in_proposer_mode() { unimplemented(); }

    virtual bool proposal_buffer_is_empty() const = 0;
    virtual void stop_proposals() { unimplemented(); }

    virtual Hash do_propose() = 0;
    //! Propose an empty block.
    virtual Hash do_empty_propose() = 0;

    virtual void add_proposal(xdr::opaque_vec<>&& proposal) { unimplemented(); }

    //! wait for new quorum cert.
    //! return true is the new QC is on the
    //! expected input hash.
    virtual bool wait_for_new_qc(Hash const& expected_next_qc) = 0;
    virtual void cancel_wait_for_new_qc() = 0;

    virtual const ReplicaConfig& get_config() = 0;

    virtual ReplicaID get_last_proposer() const = 0;

    virtual ReplicaID get_self_id() const = 0;

    virtual ~Hotstuff() = default;
};

/**!
 * Create a hotstuff instance where the replicated state machine
 * speculatively executes proposed blocks as it generates them.
 * The VM must support rolling back speculatively executed
 * commands.
 *
 * In this version, the consensus protocol periodically asks the vm
 * for new commands (the VM can return an empty string).
 *
 * add_proposal() is therefore not implemented,
 * and will throw an error.
 *
 * put_vm_in_proposer_mode() tells the vm that it should get ready to start
 * proposing blocks. the vm should stop proposing speculatively creating
 * proposals when hotstuff comes to consensus on a command from a different
 * replica.
 */
std::unique_ptr<Hotstuff>
make_speculative_hotstuff_instance(std::shared_ptr<ReplicaConfig> config,
                                   ReplicaID self_id,
                                   SecretKey sk,
                                   std::shared_ptr<VMBase> vm,
                                   HotstuffConfigs const& configs);

/**!
 * Create a hotstuff instance where the replicated state machine
 * only executes commands that have gone through consensus.
 *
 * Commands will be drawn from the list of commands that are submitted
 * via add_proposal().  If none are submitted to add_proposal,
 * then hotstuff will propose empty commands.
 */
std::unique_ptr<Hotstuff>
make_nonspeculative_hotstuff_instance(std::shared_ptr<ReplicaConfig> config,
                                      ReplicaID self_id,
                                      SecretKey sk,
                                      std::shared_ptr<VMBase> vm,
                                      HotstuffConfigs const& configs);

} // namespace hotstuff
