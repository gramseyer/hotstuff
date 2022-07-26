#pragma once

#include "hotstuff/xdr/hotstuff.h"

#include <optional>

namespace hotstuff {

class ReplicaInfo;

std::string
block_filename(const Hash& header_hash, const ReplicaInfo& info);

void save_block(const HotstuffBlockWire& block, const ReplicaInfo& info);

std::optional<HotstuffBlockWire>
load_block(const Hash& req_header_hash, const ReplicaInfo& info);

} /* hotstuff */
