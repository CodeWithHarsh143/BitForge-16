#pragma once

#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware
{

  //=================================
  // 1-Bit Gates
  //=================================

  bool Nand(bool a, bool b);
  bool Not(bool a);
  bool And(bool a, bool b);
  bool Or(bool a, bool b);
  bool Xor(bool a, bool b);

  //=================================
  // 16-Bit Gates
  //=================================

  Bits16 Nand16(const Bits16 &a16, const Bits16 &b16);
  Bits16 Not16(const Bits16 &a16);
  Bits16 And16(const Bits16 &a16, const Bits16 &b16);
  Bits16 Or16(const Bits16 &a16, const Bits16 &b16);
  Bits16 Xor16(const Bits16 &a16, const Bits16 &b16);

  //=================================
  // Additional Gates
  //=================================

  bool Or8Way(const Bits8 &in8);

} // namespace bitforge::hardware
