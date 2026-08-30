#include <catch2/catch_test_macros.hpp>
#include <bitforge/hardware/gates.hpp>

using namespace bitforge::hardware;

TEST_CASE("Nand gate truth table", "[gates]")
{
  REQUIRE(Nand(true, true) == false);
  REQUIRE(Nand(true, false) == true);
  REQUIRE(Nand(false, true) == true);
  REQUIRE(Nand(false, false) == true);
}

TEST_CASE("Not gate truth table", "[gates]")
{
  REQUIRE(Not(true) == false);
  REQUIRE(Not(false) == true);
}

TEST_CASE("AND gate truth table", "[gates]")
{
  REQUIRE(And(true, true) == true);
  REQUIRE(And(true, false) == false);
  REQUIRE(And(false, true) == false);
  REQUIRE(And(false, false) == false);
}

TEST_CASE("OR gate truth table", "[gates]")
{
  REQUIRE(Or(true, true) == true);
  REQUIRE(Or(true, false) == true);
  REQUIRE(Or(false, true) == true);
  REQUIRE(Or(false, false) == false);
}

TEST_CASE("XOR gate truth table", "[gates]")
{
  REQUIRE(Xor(true, true) == false);
  REQUIRE(Xor(true, false) == true);
  REQUIRE(Xor(false, true) == true);
  REQUIRE(Xor(false, false) == false);
}

TEST_CASE("Not16 inverts every bit", "[gates]")
{
  Bits16 a{};
  a[0] = a[5] = a[15] = true;
  auto out = Not16(a);
  REQUIRE(out[0] == false);
  REQUIRE(out[5] == false);
  REQUIRE(out[15] == false);
  REQUIRE(out[1] == true);
  REQUIRE(out[14] == true);
}

TEST_CASE("Nand16 is bitwise Nand", "[gates]")
{
  Bits16 a{};
  Bits16 b{};
  a[0] = a[1] = true;
  b[1] = b[2] = true;
  auto out = Nand16(a, b);
  REQUIRE(out[1] == false); // Nand(true,true)
  REQUIRE(out[0] == true);  // Nand(true,false)
  REQUIRE(out[2] == true);  // Nand(false,true)
  REQUIRE(out[3] == true);  // Nand(false,false)
}

TEST_CASE("And16 is bitwise And", "[gates]")
{
  Bits16 a{};
  Bits16 b{};
  a[0] = a[1] = true;
  b[1] = b[2] = true;
  auto out = And16(a, b);
  REQUIRE(out[0] == false);
  REQUIRE(out[1] == true);
  REQUIRE(out[2] == false);
  REQUIRE(out[3] == false);
}

TEST_CASE("Or16 is bitwise Or", "[gates]")
{
  Bits16 a{};
  Bits16 b{};
  a[0] = true;
  b[1] = true;
  auto out = Or16(a, b);
  REQUIRE(out[0] == true);
  REQUIRE(out[1] == true);
  REQUIRE(out[2] == false);
}

TEST_CASE("Xor16 is bitwise Xor", "[gates]")
{
  Bits16 a{};
  Bits16 b{};
  a[0] = a[1] = true;
  b[1] = b[2] = true;
  auto out = Xor16(a, b);
  REQUIRE(out[0] == true);  // 1 xor 0
  REQUIRE(out[1] == false); // 1 xor 1
  REQUIRE(out[2] == true);  // 0 xor 1
  REQUIRE(out[3] == false); // 0 xor 0
}

TEST_CASE("Or8Way returns true if any bit set", "[gates]")
{
  Bits8 all_false{};
  REQUIRE(Or8Way(all_false) == false);

  Bits8 with_bit{};
  with_bit[4] = true;
  REQUIRE(Or8Way(with_bit) == true);

  Bits8 all_true{};
  for (int i = 0; i < 8; ++i)
    all_true[i] = true;
  REQUIRE(Or8Way(all_true) == true);
}
