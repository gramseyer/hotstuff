#pragma once

#include <string>

namespace hotstuff {

std::string hotstuff_index_lmdb_dir();
std::string hotstuff_block_data_dir();
void make_hotstuff_dirs();
void clear_hotstuff_dirs();

void clear_all_data_dirs();
void make_all_data_dirs();

} /* hotstuff */
