#include "bitforge/hardware/registers.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

TEST_CASE("Bit resets to false", "[registers]") {
  Bit bit;
  REQUIRE(bit.getQ() == false);
}

TEST_CASE("Bit loads input when load is true", "[registers]") {
  Bit bit;

  bit.evaluate(true, bit.getQ(), true); // load 1
  REQUIRE(bit.getQ() == true);

  bit.evaluate(false, bit.getQ(), true); // load 0
  REQUIRE(bit.getQ() == false);
}

TEST_CASE("Bit holds value when load is false", "[registers]") {
  Bit bit;

  bit.evaluate(true, bit.getQ(), true); // load 1
  REQUIRE(bit.getQ() == true);

  bit.evaluate(true, bit.getQ(), false); // hold even though input changed
  REQUIRE(bit.getQ() == true);

  bit.evaluate(false, bit.getQ(), false); // still holding
  REQUIRE(bit.getQ() == true);
}

TEST_CASE("Register loads 16-bit value", "[registers]") {
  Register reg;

  Bits16 in{};
  in[0] = true;
  in[5] = true;
  in[15] = true;

  reg.evaluate(in, true);
  Bits16 out = reg.getQ();

  REQUIRE(out[0] == true);
  REQUIRE(out[5] == true);
  REQUIRE(out[15] == true);
  REQUIRE(out[1] == false);
  REQUIRE(out[14] == false);
}

TEST_CASE("Register holds value when load is false", "[registers]") {
  Register reg;

  Bits16 in{};
  in[0] = true;

  reg.evaluate(in, true);
  REQUIRE(reg.getQ()[0] == true);

  Bits16 hold_in{};
  reg.evaluate(hold_in, false);
  REQUIRE(reg.getQ()[0] == true);
}

TEST_CASE("Register overwrites all bits when loading", "[registers]") {
  Register reg;

  Bits16 ones{};
  ones.fill(true);
  reg.evaluate(ones, true);
  REQUIRE(reg.getQ()[0] == true);
  REQUIRE(reg.getQ()[15] == true);

  Bits16 zeros{};
  reg.evaluate(zeros, true);
  REQUIRE(reg.getQ()[0] == false);
  REQUIRE(reg.getQ()[15] == false);
}