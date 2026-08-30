#pragma once
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware
{
  struct AluResult
  {
    const Bits16 result;
    const bool zr;
    const bool ng;
  };
  AluResult Alu16( const Bits16 &a, const Bits16 &b, bool zx, bool nx, bool zy, bool ny, bool f, bool no);
}
