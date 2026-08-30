#pragma once
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware {
  struct AluResult {
    Bits16 result;
    bool zr;
    bool ng;
  };
  AluResult Alu16()
}
