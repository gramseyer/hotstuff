#pragma once

#include "hotstuff/config/replica_config.h"

#include "hotstuff/nonblocking_rpc_client.h"

#include "hotstuff/xdr/hotstuff.h"

#include <xdrpp/srpc.h>

#include <set>

namespace hotstuff {

class NetworkEventQueue;

/*!
	Manages one peer connection.

	Requests a set of blocks, then awaits the response, parses, and validates response (checks sigs).
*/ 
class BlockFetchWorker : public NonblockingRpcClient<xdr::srpc_client<FetchBlocksV1>> {

	using utils::AsyncWorker::mtx;
	using utils::AsyncWorker::cv;

	using client_t = xdr::srpc_client<FetchBlocksV1>;

	std::set<Hash> reqs;

	NetworkEventQueue& network_event_queue;

	bool exists_work_to_do() override final;

	void run();

	void readd_request(BlockFetchRequest const& req);

	ReplicaService get_service() const override final {
		return ReplicaService::BLOCK_FETCH_SERVICE;
	}

	xdr::xvector<Hash> extract_reqs();


public:

	BlockFetchWorker(const ReplicaInfo& info, NetworkEventQueue& network_event_queue);

	void add_request(Hash const& request);

};

} /* hotstuff */