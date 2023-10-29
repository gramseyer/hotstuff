#include "hotstuff/manage_data_dirs.h"

#include "utils/hs_save_load_xdr.h"

#include <utils/mkdir.h>

#include <filesystem>
#include <system_error>

#include "hotstuff/config/replica_config.h"

namespace hotstuff {

using utils::mkdir_safe;

std::string hotstuff_index_lmdb_dir(const ReplicaInfo& info) {

	return info.root_data_folder + std::string("hotstuff_block_index/");
}

std::string hotstuff_block_data_dir(const ReplicaInfo& info) {
	return info.root_data_folder + std::string("hotstuff_block_data/");
}

void make_hotstuff_dirs(const ReplicaInfo& info) {	
	mkdir_safe(info.root_data_folder.c_str());
	auto path = hotstuff_block_data_dir(info);
	mkdir_safe(path.c_str());
	path = hotstuff_index_lmdb_dir(info);
	mkdir_safe(path.c_str());
}

void clear_hotstuff_dirs(const ReplicaInfo& info) {
	auto path = hotstuff_block_data_dir(info);
	std::error_code ec;
	std::filesystem::remove_all({path}, ec);
	if (ec) {
		throw std::runtime_error("failed to clear hotstuff block dir");
	}

	path = hotstuff_index_lmdb_dir(info);
	std::filesystem::remove_all({path}, ec);
	if (ec) {
		throw std::runtime_error("failed to clear hotstuff index lmdb dir");
	}
}

void clear_all_data_dirs(const ReplicaInfo& info) {
	clear_hotstuff_dirs(info);
}

void make_all_data_dirs(const ReplicaInfo& info) {
	make_hotstuff_dirs(info);
}

} /* speedex */
