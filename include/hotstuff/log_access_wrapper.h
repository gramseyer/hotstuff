#pragma once

#include "hotstuff/vm/vm_block_id.h"
#include "hotstuff/vm/vm_base.h"
#include "hotstuff/xdr/types.h"

#include <memory>
#include <utility>

namespace hotstuff
{

class HotstuffLMDB; 

class LogAccessWrapper
{

	HotstuffLMDB const& lmdb;

	struct IteratorInternals;

	std::optional<xdr::opaque_vec<>> load_block_unparsed(Hash const& hash) const;

public:

	LogAccessWrapper(HotstuffLMDB const& lmdb)
		: lmdb(lmdb)
		{}

	class iterator
	{

		IteratorInternals* it;
		//std::unique_ptr<IteratorInternals> it;
		//std::unique_ptr<HotstuffLMDB::cursor> cursor;
		//std::unique_ptr<HotstuffLMDB::cursor::iterator> it;

	public:

		using kv_t = std::pair<uint64_t, Hash>;


		iterator(HotstuffLMDB const& c);
		iterator();

		kv_t operator*();

		std::pair<Hash, VMBlockID>
		get_hs_hash_and_vm_data();

		iterator& operator++();

		bool operator==(const iterator &other) const;
		//friend bool operator!=(const iterator &a, const iterator &b) = default;

		~iterator();
	};

	iterator begin() const;

	iterator end() const;

	template<typename vm_block_type>
	vm_block_type
	load_vm_block(Hash const& hash) const;

};

template<typename vm_block_type>
vm_block_type
LogAccessWrapper::load_vm_block(Hash const& hash) const
{
	auto unparsed_opt = load_block_unparsed(hash);
	if (!unparsed_opt) {
		throw std::runtime_error("failed to load expected block");
	}
	return vm_block_type((*unparsed_opt));
}



} /* hotstuff */
