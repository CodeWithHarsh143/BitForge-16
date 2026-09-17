#include <bitforge/hardware/pc.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

namespace {
Bits16 B(uint16_t n) {
  Bits16 v;
  for (int i = 0; i < 16; i++)
    v[i] = (n >> i) & 1;
  return v;
}

uint16_t U(const Bits16 &v) {
  uint16_t n = 0;
  for (int i = 0; i < 16; i++)
    if (v[i]) n |= 1u << i;
  return n;
}
} // namespace

TEST_CASE("PC holds value when no control is set", "[pc]") {
  PC pc;
  pc.evaluate(B(10), true, false, false);
  REQUIRE(U(pc.evaluate(B(0), false, false, false)) == 10);
  REQUIRE(U(pc.evaluate(B(0), false, false, false)) == 10);
}

TEST_CASE("PC loads input", "[pc]") {
  PC pc;
  REQUIRE(U(pc.evaluate(B(42), true, false, false)) == 42);
}

TEST_CASE("PC increments", "[pc]") {
  PC pc;
  pc.evaluate(B(5), true, false, false);
  REQUIRE(U(pc.evaluate(B(0), false, true, false)) == 6);
  REQUIRE(U(pc.evaluate(B(0), false, true, false)) == 7);
}

TEST_CASE("PC load takes priority over increment", "[pc]") {
  PC pc;
  pc.evaluate(B(10), false, true, false);
  REQUIRE(U(pc.evaluate(B(20), true, true, false)) == 20);
}

TEST_CASE("PC resets to zero", "[pc]") {
  PC pc;
  pc.evaluate(B(100), true, false, false);
  REQUIRE(U(pc.evaluate(B(0), false, false, true)) == 0);
}

TEST_CASE("PC reset overrides load and increment", "[pc]") {
  PC pc;
  pc.evaluate(B(100), true, true, false);
  REQUIRE(U(pc.evaluate(B(200), true, true, true)) == 0);
}

TEST_CASE("PC resets repeatedly", "[pc]") {
  PC pc;
  for (int i = 0; i < 3; i++) {
    pc.evaluate(B(50), true, false, false);
    REQUIRE(U(pc.evaluate(B(0), false, false, true)) == 0);
  }
}

TEST_CASE("PC counter wraps around after 65535", "[pc]") {
  PC pc;
  pc.evaluate(B(0xFFFF), true, false, false);
  REQUIRE(U(pc.evaluate(B(0), false, true, false)) == 0);
}

TEST_CASE("PC default state is zero", "[pc]") {
  PC pc;
  REQUIRE(U(pc.evaluate(B(0), false, false, false)) == 0);
}