#pragma once

#include <xdrpp/marshal.h>
#include <xdrpp/types.h>

#include <optional>
#include <vector>
#include <cstdint>

using xdr::operator==;

namespace hotstuff
{

/*
struct VMBlockID
{
	std::optional<std::vector<uint8_t>> value;

	VMBlockID() : value(std::nullopt) {}

	VMBlockID(std::vector<uint8_t> const& bytes)
		: value(bytes)
		{}


	bool operator==(const VMBlockID&) const = default;

	std::vector<uint8_t>
	serialize() const {
		if (!value) {
			return {};
		}
		return value;
	}

	operator bool() const {
		return value.has_value();
	}
}; */


template<typename block_id>
struct VMBlockID {
	std::optional<block_id> value;

	VMBlockID() : value(std::nullopt) {}
	VMBlockID(block_id const& value) : value(value) {}

	VMBlockID(std::vector<uint8_t> const& bytes)
		: value(std::nullopt)
	{
		try {
			value = std::make_optional<block_id>();
			xdr::xdr_from_opaque(bytes, *value);
		} catch(...)
		{
			value = std::nullopt;
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
