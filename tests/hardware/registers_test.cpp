#include "bitforge/hardware/registers.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

TEST_CASE("Bit resets to false", "[registers]") {
  Bit bit;
  REQUIRE(bit.getQ() == false);
}

TEST_CASE("Bit loads input with one-cycle delay (out(t)=in(t-1))", "[registers]") {
  Bit bit;

  bit.evaluate(false, false); // cycle 1: nothing loaded, out stays reset value
  REQUIRE(bit.getQ() == false);

  bit.evaluate(true, true);   // cycle 2: load 1, appears next cycle
  REQUIRE(bit.getQ() == false);

  bit.evaluate(true, false);  // cycle 3: no load
  REQUIRE(bit.getQ() == true); // out(3) = in(2) = 1
}

TEST_CASE("Bit holds value when load is false (out(t)=out(t-1))", "[registers]") {
  Bit bit;

  bit.evaluate(true, true);
  bit.evaluate(false, false);
  REQUIRE(bit.getQ() == true);

  bit.evaluate(false, false); // hold even though in changed
  REQUIRE(bit.getQ() == true);

  bit.evaluate(true, false);  // still holding
  REQUIRE(bit.getQ() == true);
}

TEST_CASE("Bit toggles between 0 and 1", "[registers]") {
  Bit bit;

  bit.evaluate(true, true);
  bit.evaluate(false, false);
  REQUIRE(bit.getQ() == true);

  bit.evaluate(false, true);
  bit.evaluate(false, false);
  REQUIRE(bit.getQ() == false);

  bit.evaluate(true, true);
  bit.evaluate(false, false);
  REQUIRE(bit.getQ() == true);
}