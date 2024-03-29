#include "hotstuff/block_storage/block_fetch_server.h"

#include "hotstuff/block_storage/block_store.h"
#include "hotstuff/config/replica_config.h"

namespace hotstuff {

std::unique_ptr<BlockFetchResponse>
BlockFetchHandler::fetch(std::unique_ptr<BlockFetchRequest> req)
{
	std::unique_ptr<BlockFetchResponse> response = std::make_unique<BlockFetchResponse>();

	for (auto const& hash : req -> reqs)
	{
		auto resp_block = block_store.get_block(hash);
		if (resp_block) {
			if (!(resp_block -> is_flushed_from_memory())) {
				response->responses.push_back(resp_block->to_wire());
			}
		}
	}
	return response;
}

BlockFetchServer::BlockFetchServer(BlockStore& block_store, ReplicaInfo const& self_info)
	: handler(block_store)
	, ps()
	, fetch_listener(
		ps, xdr::tcp_listen(self_info.get_service_name(ReplicaService::BLOCK_FETCH_SERVICE), AF_INET), false, xdr::session_allocator<void>())
	{
		fetch_listener.register_service(handler);

		std::thread([this] {
			while(!start_shutdown)
			{
				ps.poll(1000);
			}
			std::lock_guard lock(mtx);
			ps_is_shutdown = true;
			cv.notify_all();
		}).detach();
	}

void
BlockFetchServer::await_pollset_shutdown()
{
	auto done_lambda = [this] () -> bool {
		return ps_is_shutdown;
	};

	std::unique_lock lock(mtx);
	if (!done_lambda()) {
		cv.wait(lock, done_lambda);
	}
}


} /* hotstuff */
