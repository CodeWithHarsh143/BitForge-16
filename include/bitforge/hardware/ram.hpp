#pragma once
#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware {
class RAM {
private:
  std::array<Register, 8> registers8;

public:
  RAM();
  Bits16 Ram8(Bits16 &inp, const Bits3 &address, bool load);
};
} // namespace bitforge::hardware
