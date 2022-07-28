#pragma once

#include "hotstuff/config/replica_config.h"

#include "hotstuff/block.h"
#include "hotstuff/network_event.h"

#include "hotstuff/block_storage/block_fetch_worker.h"

#include "hotstuff/xdr/hotstuff.h"

#include <atomic>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace hotstuff {

class BlockStore;

// Not threadsafe in general.
// Worker threads can crawl this to read hash and see whether req was satisfied;
// two threads should not modify/read the network event list or requested_from concurrently.
class RequestContext {
	Hash request;
	std::atomic<bool> block_is_received;
	std::vector<NetEvent> dependent_network_events;

	ReplicaIDBitMap requested_from;

public:

	RequestContext(Hash const& request);

	void add_network_events(std::vector<NetEvent> events);

	bool is_received() const;

	void mark_received();

	std::vector<NetEvent> const& get_network_events() const {
		return dependent_network_events;
	}

	Hash const& get_requested_hash() const {
		return request;
	}

	bool was_requested_from(ReplicaID rid) const {
		return (requested_from >> rid) & 1;
	}
};

typedef std::shared_ptr<RequestContext> request_ctx_ptr;

class NetworkEventQueue;

class ReplicaFetchQueue {

	// CAS on an iterator (ptr to iterator) could eliminate this mtx.
	std::mutex mtx;

	const ReplicaInfo info;

	std::vector<request_ctx_ptr> outstanding_reqs;

	BlockFetchWorker worker;

	void do_gc();

	constexpr static size_t GC_FREQ = 10;

public:

	ReplicaFetchQueue(const ReplicaInfo& info, NetworkEventQueue& net_queue)
		: mtx()
		, info(info)
		, outstanding_reqs()
		, worker(info, net_queue)
		{}


	void add_request(request_ctx_ptr req);
};

class BlockFetchManager {

	std::unordered_map<ReplicaID, std::unique_ptr<ReplicaFetchQueue>> queues;
	std::map<Hash, request_ctx_ptr> outstanding_reqs;

	BlockStore& block_store;

	const ReplicaConfig& config;

	void add_replica(ReplicaInfo const& info, NetworkEventQueue& net_queue);

public:

	BlockFetchManager(BlockStore& block_store, const ReplicaConfig& config)
		: queues()
		, outstanding_reqs()
		, block_store(block_store)
		, config(config)
		{}

	void init_configs(NetworkEventQueue& net_queue)
	{
			auto reps = config.list_info();
			for (auto& replica : reps) {
				add_replica(replica, net_queue);
			}
	}

	// add_fetch_request and deliver_block are ONLY called by
	// the network event queue processor thread.  
	// Not threadsafe.
	void
	add_fetch_request(
		Hash const& requested_block, 
		ReplicaID request_target, 
		std::vector<NetEvent> const& dependent_events);

	//returns network events to execute
	std::vector<NetEvent> deliver_block(block_ptr_t blk);
};

} /* hotstuff */