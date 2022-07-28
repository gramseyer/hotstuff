#pragma once

#include "hotstuff/xdr/types.h"
namespace hotstuff
{

class ReplicaConfig;
class VMBase;

class Hotstuff
{

public:

	virtual void init_clean() = 0;
	virtual void init_from_disk() = 0;
	virtual void put_vm_in_proposer_mode() = 0;

	virtual bool proposal_buffer_is_empty() const = 0;
	virtual void stop_proposals() = 0;

	virtual Hash do_propose() = 0;
	//! Propose an empty block.
	virtual Hash do_empty_propose() = 0;

	//! wait for new quorum cert.
	//! return true is the new QC is on the
	//! expected input hash.
	virtual bool wait_for_new_qc(Hash const& expected_next_qc) = 0;
	virtual void cancel_wait_for_new_qc() = 0;

	virtual 
	const ReplicaConfig& get_config() = 0;

	virtual ReplicaID 
	get_last_proposer() const = 0;

	virtual ReplicaID 
	get_self_id() const = 0;

	virtual ~Hotstuff() {
	}
};

std::unique_ptr<Hotstuff>
make_speculative_hotstuff_instance(const ReplicaConfig& config, ReplicaID self_id, SecretKey sk, std::shared_ptr<VMBase> vm);

} /* hotstuff */
