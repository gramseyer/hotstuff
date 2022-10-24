#pragma once

/**
 * Derived in part from libhotstuff/include/entity.h
 * Original copyright notice follows
 *
 * Copyright 2018 VMware
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hotstuff/xdr/types.h"

#include <xdrpp/socket.h>

#include <unordered_map>

namespace hotstuff
{

enum ReplicaService : int32_t
{
    BLOCK_FETCH_SERVICE = 1,
    PROTOCOL_SERVICE = 2
};

class ReplicaInfo
{

    std::string hostname;
    std::string block_fetch_port;
    std::string protocol_port;

public:
    std::string root_data_folder;

    ReplicaID id;
    PublicKey pk;

    ReplicaInfo(ReplicaID id,
                PublicKey pk,
                std::string hostname,
                std::string block_fetch_port,
                std::string protocol_port,
                std::string root_data_folder);

    xdr::unique_sock tcp_connect(ReplicaService service) const;
    xdr::unique_sock tcp_connect(const char* service) const;

    std::string const& get_hostname() const { return hostname; }

    const char* get_service_name(ReplicaService service) const;

    virtual ~ReplicaInfo() = default;
};

class ReplicaConfig
{

    std::unordered_map<ReplicaID, std::unique_ptr<ReplicaInfo>> replica_map;

public:
    size_t nreplicas;
    size_t nmajority;

    ReplicaConfig();

    void add_replica(std::unique_ptr<ReplicaInfo> info);
    void finish_init();

    const ReplicaInfo& get_info(ReplicaID rid) const;

    const PublicKey& get_publickey(ReplicaID rid) const;

    std::vector<const ReplicaInfo*> list_info() const;

    bool is_valid_replica(ReplicaID replica) const;
};

} // namespace hotstuff
