#pragma once

#include <xdrpp/marshal.h>
#include <xdrpp/types.h>

#include <optional>
#include <vector>
#include <cstdint>

using xdr::operator==;

namespace hotstuff
{

template<typename block_id>
struct VMBlockID {
	std::optional<block_id> value;

	VMBlockID() : value(std::nullopt) {}
	VMBlockID(block_id const& value) : value(value) {}

	VMBlockID(std::vector<uint8_t> const& bytes)
		: value(std::nullopt)
	{
		if (bytes.size() > 0) {
			value = std::make_optional<block_id>();
			xdr::xdr_from_opaque(bytes, *value);
		}
	}

	bool operator==(const VMBlockID&) const = default;

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

} /* hotstuff */
