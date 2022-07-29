#pragma once

#include <string>

namespace hotstuff
{

class ReplicaInfo;

std::string
hotstuff_index_lmdb_dir(const ReplicaInfo&);
std::string
hotstuff_block_data_dir(const ReplicaInfo&);
void
make_hotstuff_dirs(const ReplicaInfo&);
void
clear_hotstuff_dirs(const ReplicaInfo&);

void
clear_all_data_dirs(const ReplicaInfo&);
void
make_all_data_dirs(const ReplicaInfo&);

} // namespace hotstuff
