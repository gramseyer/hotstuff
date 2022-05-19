/**
 * File derived from libhotstuff/include/crypto.h
 * Original copyright notice below.
 *
 * Copyright 2018 VMware
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <sodium.h>

#include "hotstuff/config/replica_config.h"

#include "hotstuff/xdr/types.h"
#include "hotstuff/xdr/hotstuff.h"

#include <map>

namespace hotstuff {

/**!
 * Certificate of one vote from one replica on one hash.
 * These are combined into a quorum cert.
 * Extends PartialCertificateWire, the serializable version of the structure (defined in xdr/hotstuff.x)
 */
struct PartialCertificate : public PartialCertificateWire {

    PartialCertificate(const Hash& _hash, const SecretKey& sk);

    PartialCertificate(PartialCertificateWire&& wire_cert);

    bool validate(const ReplicaInfo& info) const;
};

class QuorumCertificate {

    Hash obj_hash;
    std::map<ReplicaID, Signature> sigs;

public:

    QuorumCertificate(const Hash& obj_hash);
    QuorumCertificate(QuorumCertificateWire const& qc_wire);

    //! for building a QC from PCs.
    //! Does not check the signature - this should be done in advance.
    void add_partial_certificate(ReplicaID rid, const PartialCertificate& pc);

    //! Checks whether the QC has enough certificates for a quorum.
    //! assumes all certs inserted by add_partial_certificate are valid.
    //! (i.e. check certs before adding them with add_partial_certificate)
    bool has_quorum(const ReplicaConfig& config) const; 

    // for verifying qc from another node
    bool verify(const ReplicaConfig &config) const;

    const Hash& 
    get_obj_hash() const {
        return obj_hash;
    }

    //! Serialize to a representation that can be sent over the wire.
    QuorumCertificateWire serialize() const;
};

} /* hotstuff */
