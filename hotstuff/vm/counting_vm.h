#pragma once

#include "hotstuff/hotstuff_debug_macros.h"
#include "hotstuff/vm/vm_block_id.h"

#include <compare>
#include <cstdint>
#include <optional>
#include <vector>

#include <xdrpp/marshal.h>

namespace hotstuff {

class HotstuffLMDB;

//! Demo class for vm types + block ids.
//! Important: block_id should be in bijection with
//! block_type (up to computational limits; e.g. block_id = sha256(block_type) is fine)

typedef VMBlockID<uint64_t> CountingVMBlockID;

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
