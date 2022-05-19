#include "hotstuff/manage_data_dirs.h"

#include "utils/save_load_xdr.h"

#include <filesystem>
#include <system_error>

#include "config.h"

namespace hotstuff {

using utils::mkdir_safe;

std::string hotstuff_index_lmdb_dir() {
	return std::string(ROOT_DB_DIRECTORY) + std::string(HOTSTUFF_INDEX);
}

std::string hotstuff_block_data_dir() {
	return std::string(ROOT_DB_DIRECTORY) + std::string(HOTSTUFF_BLOCKS);
}

void make_hotstuff_dirs() {
	mkdir_safe(ROOT_DB_DIRECTORY);
	auto path = hotstuff_block_data_dir();
	mkdir_safe(path.c_str());
	path = hotstuff_index_lmdb_dir();
	mkdir_safe(path.c_str());
}

void clear_hotstuff_dirs() {
	auto path = hotstuff_block_data_dir();
	std::error_code ec;
	std::filesystem::remove_all({path}, ec);
	if (ec) {
		throw std::runtime_error("failed to clear hotstuff block dir");
	}

	path = hotstuff_index_lmdb_dir();
	std::filesystem::remove_all({path}, ec);
	if (ec) {
		throw std::runtime_error("failed to clear hotstuff index lmdb dir");
	}
}

void clear_all_data_dirs() {
	clear_hotstuff_dirs();
}

void make_all_data_dirs() {
	make_hotstuff_dirs();
}

} /* speedex */
