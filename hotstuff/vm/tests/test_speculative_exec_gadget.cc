#include <catch2/catch_test_macros.hpp>

#include "hotstuff/vm/speculative_exec_gadget.h"
#include <cstdint>

namespace hotstuff
{

TEST_CASE("sequential success", "[vm]")
{
	SpeculativeExecGadget<uint64_t> gadget;

	gadget.add_height_pair(1, {0});
	gadget.add_height_pair(2, {1});
	gadget.add_height_pair(3, {2});

	REQUIRE(gadget.on_commit_hotstuff(1) == 0u);
	REQUIRE(gadget.on_commit_hotstuff(2) == 1u);
	REQUIRE(gadget.on_commit_hotstuff(3) == 2u);

	gadget.add_height_pair(4, {3});
	REQUIRE(gadget.on_commit_hotstuff(4) == 3u);
}

TEST_CASE("get lowest", "[vm]")
{
	SpeculativeExecGadget<uint64_t> gadget;

	gadget.add_height_pair(1, 0);
	gadget.add_height_pair(2, 1);
	gadget.add_height_pair(3, 2);

	REQUIRE(gadget.on_commit_hotstuff(1) == 0u);
	REQUIRE(gadget.on_commit_hotstuff(2) == 1u);

	gadget.add_height_pair(4, 3);

	REQUIRE(gadget.get_lowest_speculative_hotstuff_height().second == 2u);
}

TEST_CASE("test hotstuff height gap", "[vm]")
{
	SpeculativeExecGadget<uint64_t> gadget;
	gadget.add_height_pair(5, 0);
	gadget.add_height_pair(10, 1);

	REQUIRE_THROWS(gadget.get_lowest_speculative_hotstuff_height());

	REQUIRE_THROWS(gadget.on_commit_hotstuff(5));
}

TEST_CASE("test revert gadget", "[vm]")
{
	SpeculativeExecGadget<uint64_t> gadget;
	gadget.add_height_pair(1, 100);
	gadget.add_height_pair(2, 101);
	gadget.on_commit_hotstuff(1);

	gadget.clear();

	gadget.add_height_pair(2, 102);
	REQUIRE(gadget.on_commit_hotstuff(2) == 102u);
}

} /* hotstuff */

