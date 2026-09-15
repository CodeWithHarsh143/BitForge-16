#include "bitforge/hardware/ram.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

namespace {
Bits3 Address3(int n) {
  Bits3 address;
  address[0] = n & 1;
  address[1] = n & 2;
  address[2] = n & 4;
  return address;
}

Bits6 Address6(int n) {
  Bits6 address;
  address[0] = n & 1;
  address[1] = n & 2;
  address[2] = n & 4;
  address[3] = n & 8;
  address[4] = n & 16;
  address[5] = n & 32;
  return address;
}

Bits16 MakeBit(int pos) {
  Bits16 in{};
  in[pos] = true;
  return in;
}

Bits16 Read8(RAM8 &ram, const Bits3 &address) {
  Bits16 in{};
  return ram.evaluate(in, address, false);
}

Bits16 Read64(RAM64 &ram, const Bits6 &address) {
  Bits16 in{};
  return ram.evaluate(in, address, false);
}
} // namespace

TEST_CASE("RAM8 resets all registers to zero", "[ram]") {
  RAM8 ram;
  for (int i = 0; i < 8; i++) {
    Bits16 out = Read8(ram, Address3(i));
    REQUIRE(out == Bits16{});
  }
}

TEST_CASE("RAM8 writes and reads back the same address", "[ram]") {
  RAM8 ram;
  Bits16 in = MakeBit(3);

  ram.evaluate(in, Address3(4), true);
  Bits16 out = Read8(ram, Address3(4));

  REQUIRE(out[3] == true);
  REQUIRE(out[0] == false);
}

TEST_CASE("RAM8 stores data at the addressed register", "[ram]") {
  RAM8 ram;
  Bits16 in = MakeBit(7);

  ram.evaluate(in, Address3(2), true);
  Bits16 out = Read8(ram, Address3(2));

  REQUIRE(out[7] == true);
}

TEST_CASE("RAM8 registers are independent", "[ram]") {
  RAM8 ram;
  Bits16 reg0_in = MakeBit(0);
  Bits16 reg1_in = MakeBit(15);

  ram.evaluate(reg0_in, Address3(0), true);
  ram.evaluate(reg1_in, Address3(1), true);

  Bits16 out0 = Read8(ram, Address3(0));
  Bits16 out1 = Read8(ram, Address3(1));

  REQUIRE(out0[0] == true);
  REQUIRE(out0[15] == false);
  REQUIRE(out1[15] == true);
  REQUIRE(out1[0] == false);
}

TEST_CASE("RAM8 holds value when load is false", "[ram]") {
  RAM8 ram;
  Bits16 in = MakeBit(9);

  ram.evaluate(in, Address3(5), true);

  Bits16 out = Read8(ram, Address3(5));
  REQUIRE(out[9] == true);
}

TEST_CASE("RAM8 can address all eight registers", "[ram]") {
  RAM8 ram;
  for (int i = 0; i < 8; i++) {
    Bits16 v = MakeBit(i);
    ram.evaluate(v, Address3(i), true);
  }
  for (int i = 0; i < 8; i++) {
    Bits16 out = Read8(ram, Address3(i));
    REQUIRE(out[i] == true);
    for (int j = 0; j < 8; j++) {
      if (j != i)
        REQUIRE(out[j] == false);
    }
  }
}

TEST_CASE("RAM8 ignores writes to other addresses while load is true", "[ram]") {
  RAM8 ram;
  Bits16 in = MakeBit(11);

  ram.evaluate(in, Address3(3), true);
  Bits16 zero{};
  ram.evaluate(zero, Address3(0), true);

  Bits16 out = Read8(ram, Address3(3));
  REQUIRE(out[11] == true);
}

TEST_CASE("RAM8 overwrites existing data when writing", "[ram]") {
  RAM8 ram;
  Bits16 first = MakeBit(5);
  Bits16 second = MakeBit(13);

  ram.evaluate(first, Address3(6), true);
  ram.evaluate(second, Address3(6), true);

  Bits16 out = Read8(ram, Address3(6));
  REQUIRE(out[13] == true);
  REQUIRE(out[5] == false);
}

TEST_CASE("RAM64 resets all registers to zero", "[ram]") {
  RAM64 ram;
  for (int i = 0; i < 64; i++) {
    Bits16 out = Read64(ram, Address6(i));
    REQUIRE(out == Bits16{});
  }
}

TEST_CASE("RAM64 writes and reads back the same address", "[ram]") {
  RAM64 ram;
  Bits16 in = MakeBit(3);

  ram.evaluate(in, Address6(4), true);
  Bits16 out = Read64(ram, Address6(4));

  REQUIRE(out[3] == true);
  REQUIRE(out[0] == false);
}

TEST_CASE("RAM64 stores data at the addressed register", "[ram]") {
  RAM64 ram;
  Bits16 in = MakeBit(7);

  ram.evaluate(in, Address6(2), true);
  Bits16 out = Read64(ram, Address6(2));

  REQUIRE(out[7] == true);
}

TEST_CASE("RAM64 registers are independent", "[ram]") {
  RAM64 ram;
  Bits16 reg0_in = MakeBit(0);
  Bits16 reg1_in = MakeBit(15);

  ram.evaluate(reg0_in, Address6(0), true);
  ram.evaluate(reg1_in, Address6(1), true);

  Bits16 out0 = Read64(ram, Address6(0));
  Bits16 out1 = Read64(ram, Address6(1));

  REQUIRE(out0[0] == true);
  REQUIRE(out0[15] == false);
  REQUIRE(out1[15] == true);
  REQUIRE(out1[0] == false);
}

TEST_CASE("RAM64 holds value when load is false", "[ram]") {
  RAM64 ram;
  Bits16 in = MakeBit(9);

  ram.evaluate(in, Address6(5), true);

  Bits16 out = Read64(ram, Address6(5));
  REQUIRE(out[9] == true);
}

TEST_CASE("RAM64 can address all 64 registers", "[ram]") {
  RAM64 ram;
  for (int i = 0; i < 64; i++) {
    Bits16 v = MakeBit(i % 16);
    ram.evaluate(v, Address6(i), true);
  }
  for (int i = 0; i < 64; i++) {
    Bits16 out = Read64(ram, Address6(i));
    REQUIRE(out[i % 16] == true);
    for (int j = 0; j < 16; j++) {
      if (j != (i % 16))
        REQUIRE(out[j] == false);
    }
  }
}

TEST_CASE("RAM64 ignores writes to other addresses while load is true", "[ram]") {
  RAM64 ram;
  Bits16 in = MakeBit(11);

  ram.evaluate(in, Address6(3), true);
  Bits16 zero{};
  ram.evaluate(zero, Address6(0), true);

  Bits16 out = Read64(ram, Address6(3));
  REQUIRE(out[11] == true);
}

TEST_CASE("RAM64 overwrites existing data when writing", "[ram]") {
  RAM64 ram;
  Bits16 first = MakeBit(5);
  Bits16 second = MakeBit(13);

  ram.evaluate(first, Address6(6), true);
  ram.evaluate(second, Address6(6), true);

  Bits16 out = Read64(ram, Address6(6));
  REQUIRE(out[13] == true);
  REQUIRE(out[5] == false);
}

TEST_CASE("RAM64 high address bits select a different word than low bits alone", "[ram]") {
  RAM64 ram;
  ram.evaluate(MakeBit(5), Address6(2), true);

  Bits16 out_hi = Read64(ram, Address6(10));
  REQUIRE(out_hi == Bits16{});

  Bits16 out_low = Read64(ram, Address6(2));
  REQUIRE(out_low[5] == true);
}

TEST_CASE("RAM64 words in different chips are independent", "[ram]") {
  RAM64 ram;
  Bits16 low = MakeBit(12);
  Bits16 high = MakeBit(3);

  ram.evaluate(low, Address6(10), true);
  ram.evaluate(high, Address6(42), true);

  Bits16 out0 = Read64(ram, Address6(10));
  Bits16 out1 = Read64(ram, Address6(42));

  REQUIRE(out0[12] == true);
  REQUIRE(out0[3] == false);
  REQUIRE(out1[3] == true);
  REQUIRE(out1[12] == false);
}