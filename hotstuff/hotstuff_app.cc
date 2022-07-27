#include "hotstuff/hotstuff_app.h"
#include "hotstuff/hotstuff.h"

#include "hotstuff/config/replica_config.h"
#include "hotstuff/vm/vm_base.h"


namespace hotstuff
{

std::unique_ptr<Hotstuff>
make_speculative_hotstuff_instance(const ReplicaConfig& config, ReplicaID self_id, SecretKey sk, std::shared_ptr<VMBase> vm)
{
	return std::make_unique<HotstuffApp>(config, self_id, sk, vm);
}

} /* hotstuff */
