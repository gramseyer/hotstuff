#pragma once

#include "hotstuff/xdr/types.h"

#include <cstdint>

namespace hotstuff
{

namespace test
{

std::pair<PublicKey, SecretKey>
deterministic_keypair_from_uint64(uint64_t val);

}
} // namespace hotstuff