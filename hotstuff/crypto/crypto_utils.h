#pragma once

#include <cstdint>
#include <utility>

#include "hotstuff/xdr/types.h"

namespace hotstuff
{

void 
__attribute__((constructor)) 
initialize_crypto();

std::pair<PublicKey, SecretKey>
deterministic_keypair_from_uint64(uint64_t val);

} /* hotstuff */
