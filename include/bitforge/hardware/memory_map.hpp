#pragma once
#include "bitforge/hardware/ram.hpp"
#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware {
class Memory_Map {
private:
  RAM16K ram16k;
  std::array<RAM4K, 2> screen_map;
  Register keyboard_map;

public:
  Memory_Map();
  Bits16 evaluate(const Bits16 &inp, const Bits15 &address, bool load);
};
} // namespace bitforge::hardware
