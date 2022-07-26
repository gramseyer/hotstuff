#pragma once

#include "config.h"

#include "hotstuff/block.h"

#include "hotstuff/block_storage/io_utils.h"

#include "lmdb/lmdb_types.h"
#include "lmdb/lmdb_wrapper.h"

#include "hotstuff/xdr/types.h"

namespace hotstuff {

class QuorumCertificate;
class ReplicaInfo;

/**
 * Stored value is <key = decided hotstuff height, value = (block hash, block id)>
 * This isn't fully stop-restart fault tolerant - crashing + reloading might
 * allow a replica to double-vote, so nodes should wait to be leader until consensus is
 * achieved on a new block.
 * We store the highest seen QC so that we can propose new blocks and trace back a new QC
 * from an updated QC (without tracing back to genesis).
 */
class HotstuffLMDB : private lmdb::LMDBInstance {

	constexpr static auto DB_NAME = "hotstuff";
	const ReplicaInfo& info;

	void open_env();

	std::optional<std::pair<Hash, std::vector<uint8_t>>>
	get_decided_hash_id_pair_(uint64_t height) const;

public:

	void create_db() {
		LMDBInstance::create_db(DB_NAME);
	}

	void open_db() {
		LMDBInstance::open_db(DB_NAME);
	}

	using LMDBInstance::sync;

	HotstuffLMDB(const ReplicaInfo& info) : LMDBInstance(), info(info) { open_env(); }

	template<typename vm_block_id>
	std::optional<std::pair<Hash, vm_block_id>>
	get_decided_hash_id_pair(uint64_t height) const;

	class txn {
		friend class HotstuffLMDB;

		lmdb::dbenv::wtxn tx;
		MDB_dbi data_dbi, meta_dbi;

		void add_decided_block_(block_ptr_t blk, std::vector<uint8_t> const& serialized_vm_blk_id);

	public:
		txn(lmdb::dbenv::wtxn&& tx, MDB_dbi data_dbi, MDB_dbi meta_dbi);
		
		template<typename vm_block_id>
		void add_decided_block(block_ptr_t blk, vm_block_id const& id);

		void set_qc_on_top_block(QuorumCertificate const& qc);
	};

	txn open_txn();
	void commit(txn& tx);

	QuorumCertificate get_highest_qc() const;

	class cursor {

		lmdb::dbenv::txn rtx;
		lmdb::dbenv::cursor c;

		friend class HotstuffLMDB;
		cursor(HotstuffLMDB const& lmdb)
			: rtx(lmdb.rbegin())
			, c(rtx.cursor_open(lmdb.get_data_dbi()))
			{}

	public:
	
		struct iterator {
			lmdb::dbenv::cursor::iterator it;

			using kv_t = std::pair<uint64_t, Hash>;

			iterator(lmdb::dbenv::cursor& c) : it(c) {}
			constexpr iterator() : it(nullptr) {}

			kv_t operator*();

			template<typename vm_block_id>
			std::pair<Hash, vm_block_id>
			get_hs_hash_and_vm_data();

			iterator& operator++() { ++it; return *this; }

			friend bool operator==(const iterator &a, const iterator &b) {
				return a.it == b.it;   // only works for forward iteration.
			}
			friend bool operator!=(const iterator &a, const iterator &b) = default;
		};

		iterator begin() {
			c.get(MDB_FIRST);
			return iterator(c);
		}

		constexpr iterator end() {
			return iterator();
		}
	};

	cursor forward_cursor() const {
		return cursor{*this};
	}
	
	template<typename vm_block_type>
	vm_block_type
	load_vm_block(Hash const& hash) const;
};

namespace detail
{
static Hash
get_hash_from_lmdb_value(const std::vector<uint8_t>& value_bytes)
{
	Hash hash;
	if (value_bytes.size() < hash.size())
	{
		throw std::runtime_error("invalid bytes length from lmdb");
	}
	memcpy(hash.data(), value_bytes.data(), hash.size());
	//hash.insert(hash.end(), value_bytes.begin(), value_bytes.begin() + hash.size());
	static_assert(hash.size() == 32, "hash size incorrect");

	return hash;
}

} /* detail */

template<typename vm_block_id>
void 
HotstuffLMDB::txn::add_decided_block(block_ptr_t blk, vm_block_id const& id)
{
	add_decided_block_(blk, id.serialize());
}

template<typename vm_block_id>
std::optional<std::pair<Hash, vm_block_id>>
HotstuffLMDB::get_decided_hash_id_pair(uint64_t height) const {
	auto out = get_decided_hash_id_pair_(height);
	if (!out) {
		return out;
	}
	return {out -> first, vm_block_id(out -> second)};
}

template<typename vm_block_id>
std::pair<Hash, vm_block_id>
HotstuffLMDB::cursor::iterator::get_hs_hash_and_vm_data() {
	auto const& [_, v] = *it;

	auto value_bytes = v.bytes();

	Hash hash = detail::get_hash_from_lmdb_value(value_bytes);
	//hash_bytes.insert(hash_bytes.end(), value_bytes.begin(), value_bytes.begin() + hash.size());
	//xdr::xdr_from_opaque(hash_bytes, hash);

	std::vector<uint8_t> block_id_bytes;
	block_id_bytes.insert(block_id_bytes.end(), value_bytes.begin() + hash.size(), value_bytes.end());

	return {hash, vm_block_id(block_id_bytes)};
}

template<typename vm_block_type>
vm_block_type
HotstuffLMDB::load_vm_block(Hash const& hash) const
{
	auto unparsed_opt = load_block(hash, info);
	if (!unparsed_opt) {
		throw std::runtime_error("failed to load expected block");
	}
	vm_block_type out;

	xdr::xdr_from_opaque((*unparsed_opt).body, out);
	return out;
}

} /* hotstuff */
