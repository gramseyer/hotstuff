#include "hotstuff/hotstuff_app.h"
#include "hotstuff/hotstuff.h"

#include "hotstuff/config/replica_config.h"
#include "hotstuff/vm/vm_base.h"


namespace hotstuff
{

std::unique_ptr<Hotstuff>
make_speculative_hotstuff_instance(const ReplicaConfig& config, ReplicaID self_id, SecretKey sk, std::shared_ptr<VMBase> vm, HotstuffConfigs const& configs)
{
	return std::make_unique<HotstuffApp>(config, self_id, sk, vm, configs);
}

std::unique_ptr<Hotstuff>
make_nonspeculative_hotstuff_instance(const ReplicaConfig& config, ReplicaID self_id, SecretKey sk, std::shared_ptr<VMBase> vm, HotstuffConfigs const& configs)
{
	return std::make_unique<NonspeculativeHotstuffApp>(config, self_id, sk, vm, configs);
}

} /* hotstuff */
