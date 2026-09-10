#include "bitforge/hardware/memory.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

TEST_CASE("SR Latch Truth Table", "[memory]") {
  SR_LATCH sr_latch = SR_LATCH();
  // Hold
  sr_latch.evaluate(false, false);
  REQUIRE(sr_latch.getQ() == false);
  REQUIRE(sr_latch.getQn() == false);

  // Set
  sr_latch.evaluate(true, false);
  REQUIRE(sr_latch.getQ() == true);
  REQUIRE(sr_latch.getQn() == false);

  // Reset
  sr_latch.evaluate(false, true);
  REQUIRE(sr_latch.getQ() == false);
  REQUIRE(sr_latch.getQn() == true);

  // Invalid
  sr_latch.evaluate(true, true);
  REQUIRE(sr_latch.getQ() == false);
  REQUIRE(sr_latch.getQn() == false);
}

TEST_CASE("JK Flip-Flop Truth Table", "[memory]") {
  JKFF jk = JKFF();

  // Set: J=1, K=0
  jk.evaluate(true, false, true);
  jk.evaluate(true, false, false);
  REQUIRE(jk.getQ() == true);
  REQUIRE(jk.getQn() == false);

  // Reset: J=0, K=1
  jk.evaluate(false, true, true);
  jk.evaluate(false, true, false);
  REQUIRE(jk.getQ() == false);
  REQUIRE(jk.getQn() == true);

  // Toggle: J=1, K=1 (Q=0 → Q=1)
  jk.evaluate(true, true, true);
  jk.evaluate(true, true, false);
  REQUIRE(jk.getQ() == true);
  REQUIRE(jk.getQn() == false);

  // Toggle: J=1, K=1 (Q=1 → Q=0)
  jk.evaluate(true, true, true);
  jk.evaluate(true, true, false);
  REQUIRE(jk.getQ() == false);
  REQUIRE(jk.getQn() == true);

  // Hold: J=0, K=0 (Q stays 0)
  jk.evaluate(false, false, true);
  jk.evaluate(false, false, false);
  REQUIRE(jk.getQ() == false);
  REQUIRE(jk.getQn() == true);
}

TEST_CASE("D Flip-Flop Truth Table", "[memory]") {
  DFF dff = DFF();

  // Set: D=1
  dff.evaluate(true, true);
  dff.evaluate(true, false);
  REQUIRE(dff.getQ() == true);
  REQUIRE(dff.getQn() == false);

  // Reset: D=0
  dff.evaluate(false, true);
  dff.evaluate(false, false);
  REQUIRE(dff.getQ() == false);
  REQUIRE(dff.getQn() == true);

  // Set: D=1 again
  dff.evaluate(true, true);
  dff.evaluate(true, false);
  REQUIRE(dff.getQ() == true);
  REQUIRE(dff.getQn() == false);
}
