
%#include "hotstuff/xdr/types.h"

namespace hotstuff {

typedef uint32 ReplicaIDBitMap;

struct PartialCertificateWire {
	Hash hash;
	Signature sig;
};

struct QuorumCertificateWire {
	Hash justify;
	ReplicaIDBitMap bmp;
	Signature sigs<MAX_REPLICAS>;
};

struct HotstuffBlockHeader {
	Hash parent_hash;
	QuorumCertificateWire qc; // qc.justify need not be the direct parent
	// body_hash included for convenience.  We can hash a "block" by just hashing the header.
	Hash body_hash; // hash of HotstuffBlockHeader::body
};

struct HotstuffBlockWire {
	HotstuffBlockHeader header;
	
	opaque body<>; // serialized vm block (for speedex, HashedBlockTransactionListPair)
};

struct BlockFetchRequest {
	Hash reqs<>;
};

struct BlockFetchResponse {
	HotstuffBlockWire responses<>;
};

program FetchBlocks {
	version FetchBlocksV1 {
		 BlockFetchResponse fetch(BlockFetchRequest) = 1;
	} = 1;
} = 0x11111114;

struct VoteMessage {
	PartialCertificateWire vote;
	ReplicaID voter;
};

struct ProposeMessage {
	HotstuffBlockWire proposal;
	ReplicaID proposer;
};

program HotstuffProtocol {
	version HotstuffProtocolV1 {
		void vote(VoteMessage) = 1;
		void propose(ProposeMessage) = 2;
	} = 1;
} = 0x11111115;

} /* hotstuff */
