#pragma once

#include "hotstuff/xdr/hotstuff.h"

#include <memory>

#include <xdrpp/pollset.h>
#include <xdrpp/srpc.h>

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace hotstuff {

class BlockStore;
class ReplicaInfo;

class BlockFetchHandler {

	BlockStore& block_store;

public:
	using rpc_interface_type = FetchBlocksV1;

	BlockFetchHandler(BlockStore& block_store)
		: block_store(block_store)
		{}

	//rpc methods
	std::unique_ptr<BlockFetchResponse> fetch(std::unique_ptr<BlockFetchRequest> req);
};

class BlockFetchServer {
	BlockFetchHandler handler;

	xdr::pollset ps;
	xdr::srpc_tcp_listener<> fetch_listener;

	bool ps_is_shutdown = false;
	std::atomic<bool> start_shutdown = false;
	std::mutex mtx;
	std::condition_variable cv;

	void await_pollset_shutdown();

public:

	BlockFetchServer(BlockStore& block_store, ReplicaInfo const& self_info);

	~BlockFetchServer()
	{
		start_shutdown = true;
		await_pollset_shutdown();
	}
};


} /* hotstuff */
