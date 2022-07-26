#pragma once

/** \file hotstuff_vm_bridge.h
 * 
 * The "VM Bridge" is the main interaction layer
 * between hotstuff consensus and the replicated state machine.
 * 
 */
#include "hotstuff/hotstuff_debug_macros.h"

#include "hotstuff/vm/speculative_exec_gadget.h"
#include "hotstuff/vm/vm_control_interface.h"

namespace hotstuff {

/** 
 * Bridge between hotstuff consensus and the replicated
 * state machine.  
 * 
 * This is where speculative application of blocks is tracked.
 * The "speculative exec gadget" tracks which blocks are applied
 * speculatively or which are committed,
 * so that when apply_block is called, 
 * this bridge knows whether the VM has speculatively applied the
 * block already, or whether to revert the VM to committed state.
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
class HotstuffVMBridge {

	using vm_block_id = typename VMType::block_id;
	using vm_block_type = typename VMType::block_type;

	SpeculativeExecGadget<vm_block_id> speculation_map;

	VMControlInterface<VMType> vm_interface;

	bool initialized;

	//! clear the speculation gadget.
	void revert_to_last_commitment() {
		VM_BRIDGE_INFO("revert to last commitment: clearing speculation map");
		speculation_map.clear();
	}

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

	HotstuffVMBridge(std::shared_ptr<VMType> vm)
		: speculation_map()
		, vm_interface(vm)
		, initialized(false)
		{}

	//! Initialize state machine and speculation gadget to clean slate.
	void init_clean() {
		vm_interface.init_clean();
		initialized = true;
	}

	//! Initialize the state machine and the speculation gadget from disk
	void init_from_disk(HotstuffLMDB const& decided_block_index, uint64_t decided_hotstuff_height) {
		vm_interface.init_from_disk(decided_block_index);
		speculation_map.init_from_disk(decided_hotstuff_height);
		initialized = true;
	}

	//! Generate an empty proposal.
	//! Useful when switching leaders (i.e. leader can generate a string of 
	//! empty proposals easily, which serves as a leader election process).
	xdr::opaque_vec<> 
	make_empty_proposal(uint64_t proposal_height) {
		init_guard();
		auto lock = speculation_map.lock();
		VM_BRIDGE_INFO("made empty proposal at height %lu", proposal_height);
		speculation_map.add_height_pair(proposal_height, VMType::empty_block_id());
		return xdr::opaque_vec<>();
	}
	
	//! Get proposal from state machine.
	//! Implicitly, getting a proposal tells the vm interface that it can speculatively
	//! apply the proposal (if it does not, it should store enough information so 
	//! as to be able to apply it later when the proposal commits).
	xdr::opaque_vec<> 
	get_and_apply_next_proposal(uint64_t proposal_height) {
		init_guard();
		VM_BRIDGE_INFO("start get_and_apply_next_proposal for height %lu", proposal_height);
		auto lock = speculation_map.lock();
		auto proposal = vm_interface.get_proposal();
		if (proposal == nullptr) {
			VM_BRIDGE_INFO("try make nonempty, got empty proposal at height %lu", proposal_height);
			speculation_map.add_height_pair(proposal_height, VMType::empty_block_id());
			return xdr::opaque_vec<>();
		}
		VM_BRIDGE_INFO("made nonempty proposal at height %lu", proposal_height);
		speculation_map.add_height_pair(proposal_height, VMType::nonempty_block_id(*proposal));
		return xdr::xdr_to_opaque(*proposal);
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

		auto lock = speculation_map.lock();
		
		auto blk_value = blk -> template try_vm_parse<vm_block_type>();
		auto blk_id = get_block_id(blk_value);

		txn.add_decided_block(blk, blk_id);

		if (!speculation_map.empty()) {

			auto const& [lowest_speculative_exec_hs_height, speculative_block_id] = speculation_map.get_lowest_speculative_hotstuff_height();

			if (blk_id == speculative_block_id) {
				return;
			}

			VM_BRIDGE_INFO("rewinding vm");

			revert_to_last_commitment();
			vm_interface.finish_work_and_force_rewind();
		}

		VM_BRIDGE_INFO("adding height entry for %lu", blk -> get_height());
		speculation_map.add_height_pair(blk -> get_height(), blk_id);

		VM_BRIDGE_INFO("submitting height %lu for exec", blk -> get_height());
		vm_interface.submit_block_for_exec(std::move(blk_value));
		VM_BRIDGE_INFO("done submit for exec %lu", blk -> get_height());
	}

	//! Notify the vm that a block has committed.
	//! Should always be called immediately after apply_block.
	void notify_vm_of_commitment(block_ptr_t blk) {
		init_guard();

		VM_BRIDGE_INFO("consuming height entry for %lu", blk -> get_height());
		auto committed_block_id = speculation_map.on_commit_hotstuff(blk->get_height());

		vm_interface.log_commitment(committed_block_id);
	}

	//! Notify the VM that it should be ready to generate proposals.
	void put_vm_in_proposer_mode() {
		init_guard();
		
		vm_interface.set_proposer();
	}

	//! checks whether the vm has any proposals to give.
	//! main use case is managing an orderly system shutdown.
	bool proposal_buffer_is_empty() const {
		return vm_interface.proposal_buffer_is_empty();
	}

	//! For orderly shutdown -- tell the vm to stop making new proposals.
	void stop_proposals() {
		vm_interface.stop_proposals();
	}

};

} /* hotstuff */
