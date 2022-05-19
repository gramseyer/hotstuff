#pragma once

#include "hotstuff/xdr/hotstuff.h"

#include <optional>

namespace hotstuff {

std::string
block_filename(const HotstuffBlockWire& block);

std::string
block_filename(const Hash& header_hash);

void save_block(const HotstuffBlockWire& block);

std::optional<HotstuffBlockWire>
load_block(const Hash& req_header_hash);

void make_block_folder();

} /* hotstuff */
