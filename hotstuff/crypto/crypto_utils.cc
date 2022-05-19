#include "crypto/crypto_utils.h"

#include <sodium.h>
#include <cstdio>
#include <stdexcept>

namespace hotstuff
{

void 
initialize_crypto()
{
	if (sodium_init() == -1) {
		throw std::runtime_error("failed to init sodium");
	}
}

} /* hotstuff */
