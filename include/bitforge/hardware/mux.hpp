#pragma once

#include <array>
#include <utility>

namespace bitforge::hardware {

//=================================
// 1-Bit Multiplexers
//=================================

bool Mux(bool a, bool b, bool sel);

//=================================
// 16-Bit Multiplexers
//=================================

std::array<bool, 16> Mux16(

    const std::array<bool, 16> &a, const std::array<bool, 16> &b, bool sel);

std::array<bool, 16> Mux4Way16(const std::array<bool, 16> &a16,
                               const std::array<bool, 16> &b16,
                               const std::array<bool, 16> &c16,
                               const std::array<bool, 16> &d16,
                               const std::array<bool, 2> &sel2);

std::array<bool, 16>
Mux8Way16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16,
          const std::array<bool, 16> &c16, const std::array<bool, 16> &d16,
          const std::array<bool, 16> &e16, const std::array<bool, 16> &f16,
          const std::array<bool, 16> &g16, const std::array<bool, 16> &h16,
          const std::array<bool, 3> &sel3);
//=================================
// 1-Bit Demultiplexers
//=================================
std::pair<bool, bool> Dmux(bool in, bool sel);
//=================================
// 4-Bit Demultiplexers
//=================================
std::array<bool, 4> Dmux4Way(bool in, std::array<bool, 2> sel2);
//=================================
// 8-Bit Demultiplexers
//=================================
std::array<bool, 8> Dmux8Way(bool in, std::array<bool, 3> sel3);

} // namespace bitforge::hardware
