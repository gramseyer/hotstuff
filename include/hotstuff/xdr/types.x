namespace hotstuff
{

typedef unsigned int uint32;
typedef int int32;

typedef unsigned hyper uint64;
typedef hyper int64;

typedef opaque uint128[16];

typedef opaque uint256[32];

typedef opaque Signature[64]; //ed25519 sig len is 512 bits
typedef opaque PublicKey[32]; //ed25519 key len is 256 bits
typedef opaque Hash[32]; // 256 bit hash, i.e. output of sha256
typedef opaque SecretKey[64]; //ed25519 secret key len is 64 bytes, at least on libsodium

typedef uint32 ReplicaID;
const MAX_REPLICAS = 32; // max length of bitmap.  More replicas => longer bitmap

//Valid replicas have IDs 0-31, inclusive
const UNKNOWN_REPLICA = 32;

} /* hotstuff */
