#include "hotstuff/protocol/hotstuff_server.h"

#include "hotstuff/network_event.h"
#include "hotstuff/network_event_queue.h"

namespace hotstuff {

void
HotstuffProtocolHandler::vote(std::unique_ptr<VoteMessage> v)
{

	if (!config.is_valid_replica(v->voter))
		return;

	queue.validate_and_add_event(
		NetEvent(
			VoteNetEvent(std::move(v))));
}

void
HotstuffProtocolHandler::propose(std::unique_ptr<ProposeMessage> p)
{
	if (!config.is_valid_replica(p -> proposer))
		return;

	queue.validate_and_add_event(
		NetEvent(
			ProposalNetEvent(std::move(p))));
}

HotstuffProtocolServer::HotstuffProtocolServer(NetworkEventQueue& queue, const ReplicaConfig& config, const ReplicaID self_id)
	: handler(queue, config)
	, ps()
	, protocol_listener(
		ps, 
		xdr::tcp_listen(
			config.get_info(self_id).get_service_name(ReplicaService::PROTOCOL_SERVICE), 
			AF_INET), 
		false, 
		xdr::session_allocator<void>())
	{
		protocol_listener.register_service(handler);
		
		std::thread([this] {ps.run();}).detach();
	}

} /* hotstuff */