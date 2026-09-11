#pragma once
#include "bitforge/hardware/memory.hpp"
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware {
class Bit {
private:
  DFF dff;
  bool Q;

public:
  Bit();
  void evaluate(bool inp, bool out, bool load);
  bool getQ();
};
class Register {

private:
  Bit bit;
  Bits16 Q;

public:
  Register();
  void evaluate(Bits16 &inp, bool load);
  Bits16 getQ();
};
} // namespace bitforge::hardware
