#include "bitforge/hardware/ram.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

namespace {
Bits3 Address(int n) {
  Bits3 address;
  address[0] = n & 1;
  address[1] = n & 2;
  address[2] = n & 4;
  return address;
}

Bits16 MakeBit(int pos) {
  Bits16 in{};
  in[pos] = true;
  return in;
}

Bits16 Read(RAM &ram, const Bits3 &address) {
  Bits16 in{};
  return ram.Ram8(in, address, false);
}
} // namespace

TEST_CASE("RAM8 resets all registers to zero", "[ram]") {
  RAM ram;
  for (int i = 0; i < 8; i++) {
    Bits16 out = Read(ram, Address(i));
    REQUIRE(out == Bits16{});
  }
}

TEST_CASE("RAM8 writes and reads back the same address", "[ram]") {
  RAM ram;
  Bits16 in = MakeBit(3);

  ram.Ram8(in, Address(4), true);
  Bits16 out = Read(ram, Address(4));

  REQUIRE(out[3] == true);
  REQUIRE(out[0] == false);
}

TEST_CASE("RAM8 stores data at the addressed register", "[ram]") {
  RAM ram;
  Bits16 in = MakeBit(7);

  ram.Ram8(in, Address(2), true);
  Bits16 out = Read(ram, Address(2));

  REQUIRE(out[7] == true);
}

TEST_CASE("RAM8 registers are independent", "[ram]") {
  RAM ram;
  Bits16 reg0_in = MakeBit(0);
  Bits16 reg1_in = MakeBit(15);

  ram.Ram8(reg0_in, Address(0), true);
  ram.Ram8(reg1_in, Address(1), true);

  Bits16 out0 = Read(ram, Address(0));
  Bits16 out1 = Read(ram, Address(1));

  REQUIRE(out0[0] == true);
  REQUIRE(out0[15] == false);
  REQUIRE(out1[15] == true);
  REQUIRE(out1[0] == false);
}

TEST_CASE("RAM8 holds value when load is false", "[ram]") {
  RAM ram;
  Bits16 in = MakeBit(9);

  ram.Ram8(in, Address(5), true);

  Bits16 out = Read(ram, Address(5));
  REQUIRE(out[9] == true);
}

TEST_CASE("RAM8 can address all eight registers", "[ram]") {
  RAM ram;
  for (int i = 0; i < 8; i++) {
    Bits16 v = MakeBit(i);
    ram.Ram8(v, Address(i), true);
  }
  for (int i = 0; i < 8; i++) {
    Bits16 out = Read(ram, Address(i));
    REQUIRE(out[i] == true);
    for (int j = 0; j < 8; j++) {
      if (j != i)
        REQUIRE(out[j] == false);
    }
  }
}

TEST_CASE("RAM8 ignores writes to other addresses while load is true", "[ram]") {
  RAM ram;
  Bits16 in = MakeBit(11);

  ram.Ram8(in, Address(3), true);
  Bits16 zero{};
  ram.Ram8(zero, Address(0), true);

  Bits16 out = Read(ram, Address(3));
  REQUIRE(out[11] == true);
}

TEST_CASE("RAM8 overwrites existing data when writing", "[ram]") {
  RAM ram;
  Bits16 first = MakeBit(5);
  Bits16 second = MakeBit(13);

  ram.Ram8(first, Address(6), true);
  ram.Ram8(second, Address(6), true);

  Bits16 out = Read(ram, Address(6));
  REQUIRE(out[13] == true);
  REQUIRE(out[5] == false);
}
