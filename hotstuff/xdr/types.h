// -*- C++ -*-
// Automatically generated from hotstuff/xdr/types.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_HOTSTUFF_XDR_TYPES_H_INCLUDED__
#define __XDR_HOTSTUFF_XDR_TYPES_H_INCLUDED__ 1

#include <xdrpp/types.h>

namespace hotstuff {

using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;
using uint128 = xdr::opaque_array<16>;
using uint256 = xdr::opaque_array<32>;
using Signature = xdr::opaque_array<64>;
using PublicKey = xdr::opaque_array<32>;
using Hash = xdr::opaque_array<32>;
using SecretKey = xdr::opaque_array<64>;
using ReplicaID = uint32;

Constexpr const std::uint32_t MAX_REPLICAS = 32;
Constexpr const std::uint32_t UNKNOWN_REPLICA = 32;

}

#endif // !__XDR_HOTSTUFF_XDR_TYPES_H_INCLUDED__
