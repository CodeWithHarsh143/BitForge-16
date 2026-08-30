#include <catch2/catch_test_macros.hpp>
#include <bitforge/hardware/arithmetic.hpp>

using namespace bitforge::hardware;

TEST_CASE("Half adder truth table", "[arithmetic]")
{
  REQUIRE(half_adder(false, false).sum == false);
  REQUIRE(half_adder(false, false).carry == false);

  REQUIRE(half_adder(true, false).sum == true);
  REQUIRE(half_adder(true, false).carry == false);

  REQUIRE(half_adder(false, true).sum == true);
  REQUIRE(half_adder(false, true).carry == false);

  REQUIRE(half_adder(true, true).sum == false);
  REQUIRE(half_adder(true, true).carry == true);
}

TEST_CASE("Full adder truth table", "[arithmetic]")
{
  // a b c -> sum, carry
  REQUIRE(full_adder(false, false, false).sum == false);
  REQUIRE(full_adder(false, false, false).carry == false);

  REQUIRE(full_adder(true, false, false).sum == true);
  REQUIRE(full_adder(true, false, false).carry == false);

  REQUIRE(full_adder(false, true, false).sum == true);
  REQUIRE(full_adder(false, true, false).carry == false);

  REQUIRE(full_adder(false, false, true).sum == true);
  REQUIRE(full_adder(false, false, true).carry == false);

  REQUIRE(full_adder(true, true, false).sum == false);
  REQUIRE(full_adder(true, true, false).carry == true);

  REQUIRE(full_adder(true, false, true).sum == false);
  REQUIRE(full_adder(true, false, true).carry == true);

  REQUIRE(full_adder(false, true, true).sum == false);
  REQUIRE(full_adder(false, true, true).carry == true);

  REQUIRE(full_adder(true, true, true).sum == true);
  REQUIRE(full_adder(true, true, true).carry == true);
}

TEST_CASE("1-bit adder-subtractor adds", "[arithmetic]")
{
  // sep=false -> a + b + c
  auto r = adder_subtractor(true, true, false, false);
  REQUIRE(r.result == false);
  REQUIRE(r.carry == true);

  auto r2 = adder_subtractor(true, false, false, false);
  REQUIRE(r2.result == true);
  REQUIRE(r2.carry == false);
}

TEST_CASE("1-bit adder-subtractor with sep complements b", "[arithmetic]")
{
  // result = a + (b XOR sep) + c
  // sep=true, a=1, b=1, c=0 -> 1 + 0 + 0 = 1
  auto r = adder_subtractor(true, true, false, true);
  REQUIRE(r.result == true);
  REQUIRE(r.carry == false);

  // sep=true, a=1, b=0, c=0 -> 1 + 1 + 0 = 2 -> sum 0, carry 1
  auto r2 = adder_subtractor(true, false, false, true);
  REQUIRE(r2.result == false);
  REQUIRE(r2.carry == true);
}

static Bits16 to_bits16(unsigned int value)
{
  Bits16 b{};
  for (int i = 15; i >= 0 && value; --i)
  {
    b[i] = value & 1;
    value >>= 1;
  }
  return b;
}

static unsigned int from_bits16(const Bits16 &b)
{
  unsigned int value = 0;
  for (int i = 0; i < 16; ++i)
    value = (value << 1) | (b[i] ? 1u : 0u);
  return value;
}

TEST_CASE("16-bit adder adds two numbers", "[arithmetic]")
{
  Bits16 a = to_bits16(12345);
  Bits16 b = to_bits16(6789);
  auto res = adder_subtractor16(a, b, false);
  REQUIRE(from_bits16(res) == 12345u + 6789u);
}

TEST_CASE("16-bit adder with carry-in result", "[arithmetic]")
{
  Bits16 a = to_bits16(65535u); // all ones
  Bits16 b = to_bits16(1u);
  auto res = adder_subtractor16(a, b, false);
  REQUIRE(from_bits16(res) == 0u); // wraps
}

TEST_CASE("16-bit subtractor subtracts two numbers", "[arithmetic]")
{
  Bits16 a = to_bits16(42u);
  Bits16 b = to_bits16(10u);
  auto res = adder_subtractor16(a, b, true);
  REQUIRE(from_bits16(res) == 32u);
}

TEST_CASE("16-bit subtractor of equal numbers is zero", "[arithmetic]")
{
  Bits16 a = to_bits16(5000u);
  Bits16 b = to_bits16(5000u);
  auto res = adder_subtractor16(a, b, true);
  REQUIRE(from_bits16(res) == 0u);
}
