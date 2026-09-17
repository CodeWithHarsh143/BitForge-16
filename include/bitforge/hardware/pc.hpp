#pragma once
#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware {

class PC {
private:
  Register registers;

public:
  PC();
  Bits16 evaluate(const Bits16 &inp, bool load, bool inc, bool reset);
};
} // namespace bitforge::hardware
