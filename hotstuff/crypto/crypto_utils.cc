#include "hotstuff/crypto/crypto_utils.h"

#include <sodium.h>
#include <array>
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

std::pair<PublicKey, SecretKey>
deterministic_keypair_from_uint64(uint64_t val)
{
	std::array<uint64_t, 4> seed_bytes; // 32 bytes
	seed_bytes.fill(0);
	seed_bytes[0] = val;

	SecretKey sk;
	PublicKey pk;

	if (crypto_sign_seed_keypair(pk.data(), sk.data(), reinterpret_cast<unsigned char*>(seed_bytes.data()))) {
		throw std::runtime_error("sig gen failed!");
	}

	return std::make_pair(pk, sk);
}

} /* hotstuff */
