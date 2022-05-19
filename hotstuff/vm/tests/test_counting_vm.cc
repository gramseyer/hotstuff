#include <catch2/catch_test_macros.hpp>

#include "hotstuff/vm/counting_vm.h"

#include "hotstuff/config/replica_config.h"
#include "hotstuff/crypto/crypto_utils.h"
#include "hotstuff/manage_data_dirs.h"

#include "hotstuff/liveness.h"

namespace hotstuff
{

TEST_CASE("single replica", "[vm]")
{
	clear_all_data_dirs();
	make_all_data_dirs();

	auto [pk, sk] = deterministic_keypair_from_uint64(1);
	ReplicaInfo r1(0, pk, "localhost", "9000", "9001");

	ReplicaConfig config;
	config.add_replica(r1);

	config.finish_init();

	auto vm = std::make_shared<CountingVM>();

	HotstuffApp app(config, r1.id, sk, vm);
	app.init_clean();

	PaceMakerWaitQC pmaker(app);
	pmaker.set_self_as_proposer();
	app.put_vm_in_proposer_mode();

	SECTION("5 proposes")
	{
		for (size_t i = 0; i < 5; i++)
		{
			pmaker.do_propose();
			pmaker.wait_for_qc();
		}

		REQUIRE(vm->get_last_committed_height() == 0);
		// this test case will break if we change how many proposals
		// are buffered within the vm bridge.
		// Right now 3 -- so vm runs 3 ahead of proposed value,
		// which is 2 ahead of last committed.
		REQUIRE(vm->get_speculative_height() == 5);
	}

	SECTION("8 proposes")
	{
		for (size_t i = 0; i < 8; i++)
		{
			pmaker.do_propose();
			pmaker.wait_for_qc();
		}

		REQUIRE(vm->get_last_committed_height() == 2);
		REQUIRE(vm->get_speculative_height() == 8);
	}

}


} /* hotstuff */
