#include <catch2/catch_test_macros.hpp>
#include <bitforge/hardware/alu.hpp>

using namespace bitforge::hardware;

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

// Interpret a raw 16-bit pattern as a signed two's-complement value.
static int from_bits16_signed(const Bits16 &b)
{
  int value = 0;
  for (int i = 0; i < 16; ++i)
    value = (value << 1) | (b[i] ? 1 : 0);
  if (value & 0x8000)
    value -= 0x10000;
  return value;
}

// Interpret a raw 16-bit pattern as an unsigned value.
static unsigned int from_bits16_unsigned(const Bits16 &b)
{
  unsigned int value = 0;
  for (int i = 0; i < 16; ++i)
    value = (value << 1) | (b[i] ? 1u : 0u);
  return value;
}

TEST_CASE("ALU returns 0 (zx, zy set)", "[alu]")
{
  Bits16 x = to_bits16(0x1234u);
  Bits16 y = to_bits16(0xABCDu);
  auto r = Alu16(x, y, true, false, true, false, true, false);
  REQUIRE(from_bits16_unsigned(r.result) == 0u);
  REQUIRE(r.zr == true);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU returns 1 (zx, nx, zy, ny, no set)", "[alu]")
{
  Bits16 x = to_bits16(0x1234u);
  Bits16 y = to_bits16(0xABCDu);
  auto r = Alu16(x, y, true, true, true, true, true, true);
  REQUIRE(from_bits16_unsigned(r.result) == 1u);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU returns -1 (all ones)", "[alu]")
{
  Bits16 x = to_bits16(0x1234u);
  Bits16 y = to_bits16(0xABCDu);
  auto r = Alu16(x, y, true, true, true, false, true, false);
  REQUIRE(from_bits16_unsigned(r.result) == 0xFFFFu);
  REQUIRE(from_bits16_signed(r.result) == -1);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == true);
}

TEST_CASE("ALU passes x through", "[alu]")
{
  Bits16 x = to_bits16(0x1A2Bu);
  Bits16 y = to_bits16(0x7777u);
  auto r = Alu16(x, y, false, false, true, true, false, false);
  REQUIRE(from_bits16_unsigned(r.result) == 0x1A2Bu);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU passes y through", "[alu]")
{
  Bits16 x = to_bits16(0x1111u);
  Bits16 y = to_bits16(0x3456u);
  auto r = Alu16(x, y, true, true, false, false, false, false);
  REQUIRE(from_bits16_unsigned(r.result) == 0x3456u);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes NOT x", "[alu]")
{
  Bits16 x = to_bits16(0x0F0Fu);
  Bits16 y = to_bits16(0x7777u);
  auto r = Alu16(x, y, false, false, true, true, false, true);
  REQUIRE(from_bits16_unsigned(r.result) == (~0x0F0Fu & 0xFFFFu));
  REQUIRE(r.ng == true);
}

TEST_CASE("ALU computes NOT y", "[alu]")
{
  Bits16 x = to_bits16(0x1111u);
  Bits16 y = to_bits16(0x0FF0u);
  auto r = Alu16(x, y, true, true, false, false, false, true);
  REQUIRE(from_bits16_unsigned(r.result) == (~0x0FF0u & 0xFFFFu));
  REQUIRE(r.ng == true);
}

TEST_CASE("ALU computes -x", "[alu]")
{
  Bits16 x = to_bits16(18u);
  Bits16 y = to_bits16(0x7777u);
  auto r = Alu16(x, y, false, false, true, true, true, true);
  REQUIRE(from_bits16_signed(r.result) == -18);
  REQUIRE(r.ng == true);
}

TEST_CASE("ALU computes -y", "[alu]")
{
  Bits16 x = to_bits16(0x1111u);
  Bits16 y = to_bits16(42u);
  auto r = Alu16(x, y, true, true, false, false, true, true);
  REQUIRE(from_bits16_signed(r.result) == -42);
  REQUIRE(r.ng == true);
}

TEST_CASE("ALU computes x + 1", "[alu]")
{
  Bits16 x = to_bits16(54u);
  Bits16 y = to_bits16(0x7777u);
  auto r = Alu16(x, y, false, true, true, true, true, true);
  REQUIRE(from_bits16_unsigned(r.result) == 55u);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes y + 1", "[alu]")
{
  Bits16 x = to_bits16(0x1111u);
  Bits16 y = to_bits16(99u);
  auto r = Alu16(x, y, true, true, false, true, true, true);
  REQUIRE(from_bits16_unsigned(r.result) == 100u);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes x - 1", "[alu]")
{
  Bits16 x = to_bits16(100u);
  Bits16 y = to_bits16(0x7777u);
  auto r = Alu16(x, y, false, false, true, true, true, false);
  REQUIRE(from_bits16_unsigned(r.result) == 99u);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes y - 1", "[alu]")
{
  Bits16 x = to_bits16(0x1111u);
  Bits16 y = to_bits16(100u);
  auto r = Alu16(x, y, true, true, false, false, true, false);
  REQUIRE(from_bits16_unsigned(r.result) == 99u);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes x + y", "[alu]")
{
  Bits16 x = to_bits16(12345u);
  Bits16 y = to_bits16(6789u);
  auto r = Alu16(x, y, false, false, false, false, true, false);
  REQUIRE(from_bits16_unsigned(r.result) == 12345u + 6789u);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes x - y", "[alu]")
{
  Bits16 x = to_bits16(100u);
  Bits16 y = to_bits16(30u);
  auto r = Alu16(x, y, false, true, false, false, true, true);
  REQUIRE(from_bits16_signed(r.result) == 70);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes y - x", "[alu]")
{
  Bits16 x = to_bits16(30u);
  Bits16 y = to_bits16(100u);
  auto r = Alu16(x, y, false, false, false, true, true, true);
  REQUIRE(from_bits16_signed(r.result) == 70);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes x AND y", "[alu]")
{
  Bits16 x = to_bits16(0x00FFu);
  Bits16 y = to_bits16(0x0F0Fu);
  auto r = Alu16(x, y, false, false, false, false, false, false);
  REQUIRE(from_bits16_unsigned(r.result) == 0x000Fu);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU computes x OR y", "[alu]")
{
  Bits16 x = to_bits16(0x00FFu);
  Bits16 y = to_bits16(0x0F00u);
  auto r = Alu16(x, y, false, true, false, true, false, true);
  REQUIRE(from_bits16_unsigned(r.result) == 0x0FFFu);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU zero result sets zr flag", "[alu]")
{
  Bits16 x = to_bits16(50u);
  Bits16 y = to_bits16(50u);
  auto r = Alu16(x, y, false, true, false, false, true, true); // x - y == 0
  REQUIRE(from_bits16_unsigned(r.result) == 0u);
  REQUIRE(r.zr == true);
  REQUIRE(r.ng == false);
}

TEST_CASE("ALU negative result sets ng flag", "[alu]")
{
  Bits16 x = to_bits16(10u);
  Bits16 y = to_bits16(50u);
  auto r = Alu16(x, y, false, true, false, false, true, true); // x - y < 0
  REQUIRE(from_bits16_signed(r.result) == -40);
  REQUIRE(r.zr == false);
  REQUIRE(r.ng == true);
}

TEST_CASE("ALU flags for all-ones result", "[alu]")
{
  Bits16 x = to_bits16(0x1234u);
  Bits16 y = to_bits16(0xABCDu);
  auto r = Alu16(x, y, true, false, true, false, true, false); // 0
  REQUIRE(r.zr == true);

  auto r1 = Alu16(x, y, true, true, true, false, true, false); // -1
  REQUIRE(r1.zr == false);
  REQUIRE(r1.ng == true);
}
