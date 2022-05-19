#pragma once

#include "hotstuff/hotstuff_debug_macros.h"

#include <compare>
#include <cstdint>
#include <optional>
#include <vector>

#include <xdrpp/marshal.h>

namespace hotstuff {

class HotstuffLMDB;

struct CountingVMBlockID {
	std::optional<uint64_t> value;

	CountingVMBlockID() : value(std::nullopt) {}
	CountingVMBlockID(uint64_t value) : value(value) {}

	CountingVMBlockID(std::vector<uint8_t> const& bytes);

	bool operator==(const CountingVMBlockID&) const = default;

	std::vector<uint8_t>
	serialize() const {
		if (!value) {
			return {};
		}
		return xdr::xdr_to_opaque(*value);
	}

	operator bool() const {
		return value.has_value();
	}
};


class CountingVM {
	uint64_t state = 0;
	uint64_t last_committed_state = 0;

public:

	using block_type = uint64_t;
	using block_id = CountingVMBlockID;

	static block_id nonempty_block_id(block_type const& blk) {
		return CountingVMBlockID{blk};
	}
	static block_id empty_block_id() {
		return CountingVMBlockID{};
	}

	uint64_t get_last_committed_height() const
	{
		return last_committed_state;
	}
	uint64_t get_speculative_height() const
	{
		return state;
	}

	void init_clean() {
		state = 0;
		last_committed_state = 0;
	}

	void init_from_disk(HotstuffLMDB const& lmdb);

	std::unique_ptr<block_type> propose() {
		state++;
		HOTSTUFF_INFO("VM: proposing value %lu", state);
		return std::make_unique<block_type>(state);
	}

	// Main workflow for non-proposer is exec_block (called indirectly
	// by update) immediately followed by log_commitment
	// Proposer skips the exec_block call.
	void exec_block(const block_type& blk);

	void log_commitment(const block_id& id);

	void rewind_to_last_commit();
};

} /* hotstuff */
