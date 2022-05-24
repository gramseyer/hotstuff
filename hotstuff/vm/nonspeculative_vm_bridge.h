#pragma once

/** \file nonspeculative_vm_bridge.h
 * 
 * The "VM Bridge" is the main interaction layer
 * between hotstuff consensus and the replicated state machine.
 * 
 * This version does not execute commands speculatively.
 * 
 * Not at all tested, your mileage may vary.
 * 
 */
#include "hotstuff/hotstuff_debug_macros.h"

namespace hotstuff {

/** 
 * Bridge between hotstuff consensus and the replicated
 * state machine.  
 * 
 * Important invariant: the value of vm_block_id should be in 1-1 correspondence from
 * block contents (block semantic meaning).  One might use vm_block_id = sha256(block_type).
 * The id is used in disambiguating blocks during the speculation.
 * This class needs to know, for example, how to differentiate command A 
 * (applied, but only speculatively, at hotstuff height X)
 * from command B (also applied at hotstuff height X, and committed, 
 * i.e. by being proposed by another replica.
 * 
 * For every block, hotstuff calls apply_block and then notify_vm_of_commitment.
 * (Yes, this is redundant, and may change in the future.  These two methods
 * could be easily combined).
 * In theory, one could make these two processes separate (and the initial plan was to do so)
 * but that separation makes speculation tracking much more complicated and error-prone.
 */
template<typename VMType>
class NonspeculativeVMBridge {

	using vm_block_id = typename VMType::block_id;
	using vm_block_type = typename VMType::block_type;

	//TODO: rewrite with a non-speculative version of control interface
	// cut out some of the extra code, essentially
	//VMControlInterface<VMType> vm_interface;
	std::shared_ptr<VMType> vm;
	
	bool initialized;

	std::mutex mtx;
	std::vector<xdr::opaque_vec<>> proposal_buffer;

	//! map block_type to block_id, considering the fact
	//! that the input block might be null
	//! (in which case, this returns a default, null_id value).
	vm_block_id get_block_id(std::unique_ptr<vm_block_type> const& blk) {
		if (blk) {
			return VMType::nonempty_block_id(*blk);
		}
		return VMType::empty_block_id();
	}

	void init_guard() const {
		if (!initialized) {
			throw std::runtime_error("uninitialized vm!");
		}
	}

public:

	NonspeculativeVMBridge(std::shared_ptr<VMType> vm)
		: vm(vm)
		, initialized(false)
		{}

	//! Initialize state machine and speculation gadget to clean slate.
	void init_clean() {
		//TODO init vm
		initialized = true;
	}

	//! Initialize the state machine and the speculation gadget from disk
	void init_from_disk(HotstuffLMDB const& decided_block_index, uint64_t decided_hotstuff_height) {
		//TODO init vm
		initialized = true;
	}

	//! Generate an empty proposal.
	//! Useful when switching leaders (i.e. leader can generate a string of 
	//! empty proposals easily, which serves as a leader election process).
	xdr::opaque_vec<> 
	make_empty_proposal(uint64_t proposal_height) {
		init_guard();
		VM_BRIDGE_INFO("made empty proposal at height %lu", proposal_height);
		return xdr::opaque_vec<>();
	}
	
	//! Get proposal from proposal buffer
	//! Note - name difference from regular vm bridge -- this'll need to
	//! change in call sites to use this version.
	xdr::opaque_vec<> 
	get_next_proposal(uint64_t proposal_height) {
		init_guard();
		VM_BRIDGE_INFO("start get_next_proposal for height %lu", proposal_height);
		std::lock_guard lock(mtx);

		if (proposal_buffer.size() == 0)
		{
			VM_BRIDGE_INFO("try make nonempty, got empty proposal at height %lu", proposal_height);
			return xdr::opaque_vec<>();
		}

		auto out = std::move(proposal_buffer.back());
		proposal_buffer.pop_back();

		return out;
	}

	//! Apply a block to the state machine.
	//! Note that this block must be decided by consensus
	//! (or else the block might be falsely thought to be committed
	//! upon a crash recovery).
	//! Logs the block within the speculation gadget, and
	//! rewinds the state machine if the applied block
	//! does not match what was speculatively executed.
	//! Always expects notify_vm_of_commitment to be called immediately
	//! afterwards.
	void apply_block(block_ptr_t blk, HotstuffLMDB::txn& txn) {

		init_guard();
		
		auto blk_value = blk -> template try_vm_parse<vm_block_type>();
		auto blk_id = get_block_id(blk_value);

		txn.add_decided_block(blk, blk_id);

		VM_BRIDGE_INFO("submitting height %lu for exec", blk -> get_height());
		//vm_interface.submit_block_for_exec(std::move(blk_value));
		
		//TODO this call is blocking
		vm -> exec_block(blk_value);
		VM_BRIDGE_INFO("done submit for exec %lu", blk -> get_height());
	}

	void notify_vm_of_commitment(block_ptr_t blk) {
		// no op
	}

	//! checks whether the vm has any proposals to give.
	//! main use case is managing an orderly system shutdown.
	bool proposal_buffer_is_empty() const {
		std::lock_guard lock(mtx);
		return proposal_buffer.size() == 0;
	}

	void add_proposal(xdr::opaque_vec<>&& proposal)
	{
		std::lock_guard lock(mtx);
		proposal_buffer.insert(proposal_buffer.begin(), std::move(proposal));
	}
};

} /* hotstuff */
