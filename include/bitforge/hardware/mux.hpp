#pragma once

#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware
{

  //=================================
  // 1-Bit Multiplexers
  //=================================

  bool Mux(bool a, bool b, bool sel);

  //=================================
  // 16-Bit Multiplexers
  //=================================

  Bits16 Mux16(const Bits16 &a, const Bits16 &b, bool sel);

  Bits16 Mux4Way16(const Bits16 &a16,
                   const Bits16 &b16,
                   const Bits16 &c16,
                   const Bits16 &d16,
                   const Bits2 &sel2);

  Bits16
  Mux8Way16(const Bits16 &a16, const Bits16 &b16,
            const Bits16 &c16, const Bits16 &d16,
            const Bits16 &e16, const Bits16 &f16,
            const Bits16 &g16, const Bits16 &h16,
            const Bits3 &sel3);
  //=================================
  // 1-Bit Demultiplexers
  //=================================
  std::pair<bool, bool> Dmux(bool in, bool sel);
  //=================================
  // 4-Bit Demultiplexers
  //=================================
  Bits4 Dmux4Way(bool in, const Bits2 &sel2);
  //=================================
  // 8-Bit Demultiplexers
  //=================================
  Bits8 Dmux8Way(bool in, const Bits3 &sel3);

} // namespace bitforge::hardware
