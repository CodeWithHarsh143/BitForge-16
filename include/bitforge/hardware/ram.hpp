#pragma once
#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware {
class RAM8 {
private:
  std::array<Register, 8> registers;

public:
  RAM8();
  Bits16 evaluate(const Bits16 &inp, const Bits3 &address, bool load);
};

class RAM64 {
private:
  std::array<RAM8, 8> rams;

public:
  RAM64();
  Bits16 evaluate(const Bits16 &inp, const Bits6 &address, bool load);
};

} // namespace bitforge::hardware
