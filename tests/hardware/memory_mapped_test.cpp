#include "bitforge/hardware/memory_mapped.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace bitforge::hardware;

namespace {
Bits15 A15(int n) {
  Bits15 a{};
  for (int i = 0; i < 15; i++)
    a[i] = (n >> i) & 1;
  return a;
}
Bits16 V16(int v) {
  Bits16 b{};
  v &= 0xFFFF;
  for (int i = 0; i < 16; i++)
    b[i] = (v >> i) & 1;
  return b;
}
int ToInt(Bits16 b) {
  int v = 0;
  for (int i = 0; i < 16; i++)
    if (b[i])
      v |= (1 << i);
  return v;
}
int R(Memory &m, int addr) {
  Bits16 z{};
  return ToInt(m.evaluate(z, A15(addr), false));
}
void W(Memory &m, int addr, int value) {
  m.evaluate(V16(value), A15(addr), true);
}
} // namespace

TEST_CASE("Memory routes RAM 0-16383", "[mmap]") {
  Memory m;
  W(m, 0, 0xAAAA);
  W(m, 16383, 0x5555);
  REQUIRE(R(m, 0) == 0xAAAA);
  REQUIRE(R(m, 16383) == 0x5555);
}

TEST_CASE("Memory RAM words are independent", "[mmap]") {
  Memory m;
  W(m, 10, 0x1234);
  W(m, 8192, 0x5678);
  REQUIRE(R(m, 10) == 0x1234);
  REQUIRE(R(m, 8192) == 0x5678);
}

TEST_CASE("Memory routes Screen incl. second chip and boundary", "[mmap]") {
  Memory m;
  W(m, 16384, 0x1234); // screen offset 0
  W(m, 20480, 0x00AA); // screen offset 4096 (second RAM4K)
  W(m, 24575, 0xFFFF); // last screen word
  REQUIRE(R(m, 16384) == 0x1234);
  REQUIRE(R(m, 20480) == 0x00AA);
  REQUIRE(R(m, 24575) == 0xFFFF);
}

TEST_CASE("Memory RAM and Screen are isolated", "[mmap]") {
  Memory m;
  W(m, 5, 0x00FF);
  W(m, 16384, 0x1234);
  REQUIRE(R(m, 5) == 0x00FF);
  REQUIRE(R(m, 16384) == 0x1234);
}

TEST_CASE("Memory SetPixel formula round-trips", "[mmap]") {
  Memory m;
  // (row 1, col 5) -> addr 16416, bit 5 ; (row 0, col 20) -> addr 16385, bit 4
  int addr = 16384 + 1 * 32 + 5 / 16;
  REQUIRE(addr == 16416);
  W(m, addr, R(m, addr) | (1 << (5 % 16)));
  REQUIRE(R(m, 16416) == (1 << 5));
  W(m, 16385, R(m, 16385) | (1 << 4));
  REQUIRE(R(m, 16385) == (1 << 4));
  // clear again without touching neighbours
  W(m, 16385, R(m, 16385) & ~(1 << 4));
  REQUIRE(R(m, 16385) == 0);
  REQUIRE(R(m, 16416) == (1 << 5));
}

TEST_CASE("Memory keyboard is host-set, CPU-write ignored", "[mmap]") {
  Memory m;
  m.setKeyboard(V16(65));
  REQUIRE(R(m, 24576) == 65);
  W(m, 24576, 99); // CPU write must not stick
  REQUIRE(R(m, 24576) == 65);
  m.clearKeyboard();
  REQUIRE(R(m, 24576) == 0);
}

TEST_CASE("Memory invalid addresses read 0 and ignore writes", "[mmap]") {
  Memory m;
  W(m, 5, 0x00FF);
  W(m, 16384, 0x1234);
  REQUIRE(R(m, 24577) == 0);
  REQUIRE(R(m, 32767) == 0);
  W(m, 30000, 0x4321);
  REQUIRE(R(m, 5) == 0x00FF);
  REQUIRE(R(m, 16384) == 0x1234);
}

TEST_CASE("Memory readScreen shadow matches hierarchy writes", "[mmap]") {
  Memory m;
  W(m, 16384, 0x00FF);
  W(m, 20480, 0x0F0F);
  REQUIRE(ToInt(m.readScreen(0)) == 0x00FF);
  REQUIRE(ToInt(m.readScreen(4096)) == 0x0F0F);
  REQUIRE(ToInt(m.readScreen(1)) == 0);
}
