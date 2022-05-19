#include "hotstuff/config/replica_config.h"

#include "hotstuff/crypto/crypto_utils.h"

#include "hotstuff/hotstuff_debug_macros.h"

#include <optional>

namespace hotstuff {

xdr::unique_sock
ReplicaInfo::tcp_connect(ReplicaService service) const
{
	return xdr::tcp_connect(hostname.c_str(), get_service_name(service));
}

const char* 
ReplicaInfo::get_service_name(ReplicaService service) const
{
	switch (service)
	{
		case BLOCK_FETCH_SERVICE:
			return block_fetch_port.c_str();
		case PROTOCOL_SERVICE:
			return protocol_port.c_str();
	}
	throw std::runtime_error("invalid service");
}


ReplicaInfo::ReplicaInfo(
    ReplicaID id, 
    PublicKey pk, 
    std::string hostname,
    std::string block_fetch_port,
    std::string protocol_port)
	: hostname(hostname)
	, block_fetch_port(block_fetch_port)
	, protocol_port(protocol_port)
	, id(id)
	, pk(pk)
	{}

ReplicaConfig::ReplicaConfig()
	: replica_map()
	, nreplicas(0)
	, nmajority(0) {}


void
ReplicaConfig::add_replica(const ReplicaInfo &info) {
	auto rid = info.id;
	auto it = replica_map.find(rid);
	if (it != replica_map.end()) {
		throw std::runtime_error("can't add replicaid twice!");
	}
    replica_map.insert(std::make_pair(rid, info));
    nreplicas++;
}

void
ReplicaConfig::finish_init() {
	size_t nfaulty = nreplicas / 3;
	HOTSTUFF_INFO("nfaulty = %lu", nfaulty);
	if (nfaulty == 0) {
		HOTSTUFF_INFO("num faulty tolerated is 0!  Is this ok?");
	}
	nmajority = nreplicas - nfaulty;
}

const ReplicaInfo&
ReplicaConfig::get_info(ReplicaID rid) const {
    auto it = replica_map.find(rid);
    if (it == replica_map.end())
        throw std::runtime_error(std::string("rid") + std::to_string(rid) + "not found");
    return it->second;
}

const PublicKey&
ReplicaConfig::get_publickey(ReplicaID rid) const {
    return get_info(rid).pk;
}

} /* hotstuff */
