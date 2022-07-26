#pragma once

#include "hotstuff/xdr/types.h"

#include <stdexcept>

#include <sodium.h>
#include <xdrpp/marshal.h>

namespace hotstuff
{

//! Hash an xdr (serializable) type.
//! Must call sodium_init() prior to usage.
template<typename xdr_type>
Hash hash_xdr(const xdr_type& value) {
	Hash hash_buf;
	auto serialized = xdr::xdr_to_opaque(value);

	if (crypto_generichash(
		hash_buf.data(), hash_buf.size(), 
		serialized.data(), serialized.size(), 
		NULL, 0) != 0) 
	{
		throw std::runtime_error("error in crypto_generichash");
	}
	return hash_buf;
}

} /* hotstuff */
