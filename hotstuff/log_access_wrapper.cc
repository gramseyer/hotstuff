#include "hotstuff/log_access_wrapper.h"

#include "hotstuff/lmdb.h"

namespace hotstuff
{

struct LogAccessWrapper::IteratorInternals
{
	std::optional<HotstuffLMDB::cursor> cursor;
	HotstuffLMDB::cursor::iterator it;

	IteratorInternals(HotstuffLMDB const& lmdb)
		: cursor(lmdb.forward_cursor())
		, it(cursor->begin())
		{}

	IteratorInternals()
		: cursor()
		, it(HotstuffLMDB::cursor::end())
		{}
};

std::optional<xdr::opaque_vec<>> 
LogAccessWrapper::load_block_unparsed(Hash const& hash) const
{
	return lmdb.load_block_unparsed(hash);
}

LogAccessWrapper::iterator::iterator(HotstuffLMDB const& c) 
	: it(new IteratorInternals(c))
	{}

LogAccessWrapper::iterator::iterator()
	: it(new IteratorInternals())
	{}

 
LogAccessWrapper::iterator::~iterator()
{
	if (!it)
	{
		// invalid iterator internals in iterator destruction
		std::terminate();
	}
	delete it;
}

typename LogAccessWrapper::iterator::kv_t 
LogAccessWrapper::iterator::operator*()
{
	return *((*it).it);
}


std::pair<Hash, VMBlockID>
LogAccessWrapper::iterator::get_hs_hash_and_vm_data()
{
	return it -> it.template get_hs_hash_and_vm_data<VMBlockID>();
}

LogAccessWrapper::iterator& 
LogAccessWrapper::iterator::operator++() { 
	++((*it).it); return *this; 
}

bool 
LogAccessWrapper::iterator::operator==(const LogAccessWrapper::iterator& other) const {
	return (*it).it == (*(other.it)).it;   // only works for forward iteration.
}


LogAccessWrapper::iterator 
LogAccessWrapper::begin() const {
	return iterator(lmdb);
}
 
LogAccessWrapper::iterator 
LogAccessWrapper::end() const {
	return iterator();
}

} /* hotstuff */
