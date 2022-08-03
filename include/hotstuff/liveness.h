/**
 * Based on libhotstuff/include/liveness.h
 * Original copyright declaration follows
 *
 * Copyright 2018 VMware
 * Copyright 2018 Ted Yin
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

#include "hotstuff/hotstuff_app.h"

namespace hotstuff
{

/**!
 * Proposer with fixed leader rotation
 * that waits for a qc on the last proposed block
 * (not commitment)
 * before proposing the next.
 * Waiting is not _strictly_ required,
 * just facilitated via the wait_for_qc() method.
 * Does not implement leader rotation by
 * default, but will defer to new leader
 * by default if another replica proposes a block.
 *
 * Timeouts, leader failure detection not implemented.
 */
class PaceMakerWaitQC
{

    Hotstuff& hotstuff_app;

    Hash expected_hash;

    void rotate_proposers()
    {
        proposer = (proposer + 1) % hotstuff_app.get_config().nreplicas;
    }

    ReplicaID proposer;

public:
    PaceMakerWaitQC(std::unique_ptr<Hotstuff>& hotstuff_app)
        : hotstuff_app(*hotstuff_app)
        , expected_hash()
        , proposer(hotstuff_app->get_last_proposer()) // get genesis proposer
    {}

    void wait_for_qc()
    {
        auto res = hotstuff_app.wait_for_new_qc(expected_hash);

        if (!res)
        {
            proposer = hotstuff_app.get_last_proposer();
        }
    }

    bool should_propose() { return proposer == hotstuff_app.get_self_id(); }

    void set_self_as_proposer() { proposer = hotstuff_app.get_self_id(); }

    void do_propose()
    {
        proposer = hotstuff_app.get_self_id();
        expected_hash = hotstuff_app.do_propose();
        // TODO reset any timeout
    }

    void do_empty_propose()
    {
        proposer = hotstuff_app.get_self_id();
        expected_hash = hotstuff_app.do_empty_propose();
    }
};

} // namespace hotstuff
