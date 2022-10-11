#pragma once

#include "hotstuff/config/replica_config.h"

#include "hotstuff/xdr/hotstuff.h"

#include <xdrpp/pollset.h>
#include <xdrpp/srpc.h>

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace hotstuff {

class NetworkEventQueue;

class HotstuffProtocolHandler {

	NetworkEventQueue& queue;

	const ReplicaConfig& config;

public:

	using rpc_interface_type = HotstuffProtocolV1;

	HotstuffProtocolHandler(NetworkEventQueue& queue, const ReplicaConfig& config)
		: queue(queue)
		, config(config)
		{}

	//rpc methods

	void vote(std::unique_ptr<VoteMessage> v);
	void propose(std::unique_ptr<ProposeMessage> p);
};

class HotstuffProtocolServer {

	HotstuffProtocolHandler handler;
	xdr::pollset ps;

	xdr::srpc_tcp_listener<> protocol_listener;

	bool ps_is_shutdown = false;
	std::atomic_flag start_shutdown = false;
	std::mutex mtx;
	std::condition_variable cv;

	void await_pollset_shutdown();

public:

	HotstuffProtocolServer(NetworkEventQueue& queue, const ReplicaConfig& config, const ReplicaID self_id);

	~HotstuffProtocolServer()
	{
		start_shutdown.test_and_set();
		await_pollset_shutdown();
	}
};


} /* hotstuff */