#include <catch2/catch_test_macros.hpp>

#include "examples/vm/counting_vm.h"
#include "hotstuff/hotstuff_app.h"

#include "hotstuff/hotstuff_configs.h"

#include "hotstuff/config/replica_config.h"
#include "hotstuff/manage_data_dirs.h"

#include "test_utils/keygen.h"

#include "hotstuff/liveness.h"

namespace hotstuff
{

TEST_CASE("single replica", "[vm]")
{
	std::printf("start test\n");
	auto [pk, sk] = test::deterministic_keypair_from_uint64(1);
	ReplicaInfo r1(0, pk, "localhost", "9000", "9001", "test_data_folder/");

	clear_all_data_dirs(r1);
	make_all_data_dirs(r1);

	ReplicaConfig config;
	config.add_replica(r1);

	config.finish_init();

	auto vm = std::make_shared<CountingVM>();

	auto app = make_speculative_hotstuff_instance(config, r1.id, sk, vm, HotstuffConfigs());

	app->init_clean();

	PaceMakerWaitQC pmaker(app);
	pmaker.set_self_as_proposer();
	app->put_vm_in_proposer_mode();

	std::printf("done init\n");
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
	std::printf("start second section\n");

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
	clear_all_data_dirs(r1);
}


} /* hotstuff */
