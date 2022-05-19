#pragma once

#include "hotstuff/config/replica_config.h"

#include "hotstuff/xdr/hotstuff.h"

#include <xdrpp/pollset.h>
#include <xdrpp/srpc.h>

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

public:

	HotstuffProtocolServer(NetworkEventQueue& queue, const ReplicaConfig& config, const ReplicaID self_id);
};


} /* hotstuff */