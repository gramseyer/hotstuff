#pragma once

#include "hotstuff/config/replica_config.h"

#include "hotstuff/block.h"
#include "hotstuff/crypto/certs.h"

#include "hotstuff/xdr/hotstuff.h"

#include <variant>

namespace hotstuff {

class HotstuffCore;

/**
 * Vote event for state machine event queue.
 * Before putting object onto event queue, 
 * we must:
 * - ensure block has been added to block storage
 *   (so we know block height in updateQCHigh)
 * - ensure that the vote is valid (sig checks ok)
 * - ensure that vote is on block hash (perhaps implicit
 *   in the first check)
 */
class VoteEvent {

	ReplicaID rid;
	PartialCertificate cert;
	block_ptr_t block;

public:
	VoteEvent(PartialCertificate const& cert, block_ptr_t blk, ReplicaID rid);

	bool validate(ReplicaConfig const& config) const;
	void operator() (HotstuffCore& core) const;
};

class ProposalEvent {

	ReplicaID rid;
	// block must exist in BlockStorage cache (i.e. must have a height)
	block_ptr_t block;

public:

	ProposalEvent(block_ptr_t block, ReplicaID rid);

	bool validate(ReplicaConfig const& config) const;
	void operator() (HotstuffCore& core) const;
};

class Event {
	std::variant<VoteEvent, ProposalEvent> e;

public:

	Event(VoteEvent&& v)
		: e(std::move(v))
		{}
	Event(ProposalEvent&& p)
		: e(std::move(p))
		{}

	bool validate(ReplicaConfig const& config) const;
	void operator() (HotstuffCore& core) const;
};

} /* hotstuff */
