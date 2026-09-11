#pragma once
#include "bitforge/hardware/memory.hpp"

namespace bitforge::hardware {
class Bit {
private:
  DFF dff;
  bool Q;

public:
  Bit();
  void evaluate(bool inp, bool load);
  bool getQ();
};
} // namespace bitforge::hardware
