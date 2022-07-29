#pragma once

#include <xdrpp/marshal.h>
#include <xdrpp/types.h>

#include <cstdint>
#include <optional>
#include <vector>

using xdr::operator==;

namespace hotstuff
{

struct VMBlockID
{
    std::optional<std::vector<uint8_t>> value;

    VMBlockID()
        : value(std::nullopt)
    {}

    VMBlockID(std::vector<uint8_t> const& bytes)
        : value(bytes)
    {}

    bool operator==(const VMBlockID&) const = default;

    std::vector<uint8_t> serialize() const
    {
        if (!value)
        {
            return {};
        }
        return *value;
    }

    operator bool() const { return value.has_value(); }
};

} // namespace hotstuff
