#include "bitforge/hardware/memory.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

TEST_CASE("SR Latch Truth Table", "[memory]") {
  SR_LATCH sr_latch = SR_LATCH();
  // Invalid State
  sr_latch.evaluate(false, false);
  REQUIRE(sr_latch.getQ() == true);
  REQUIRE(sr_latch.getQn() == true);

  // Set
  sr_latch.evaluate(1, 0);
  REQUIRE(sr_latch.getQ() == 1);
  REQUIRE(sr_latch.getQn() == 0);

  // Reset
  sr_latch.evaluate(0, 1);
  REQUIRE(sr_latch.getQ() == 0);
  REQUIRE(sr_latch.getQn() == 1);

  // Hold
  sr_latch.evaluate(1, 0);
  REQUIRE(sr_latch.getQ() == 1);
  REQUIRE(sr_latch.getQn() == 0);
}
