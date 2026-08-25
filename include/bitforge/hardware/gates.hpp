#pragma once

#include <array>

namespace bitforge::hardware {

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

std::array<bool, 16> Nand16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16);
std::array<bool, 16> Not16(const std::array<bool, 16> &a16);
std::array<bool, 16> And16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16);
std::array<bool, 16> Or16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16);
std::array<bool, 16> Xor16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16);

//=================================
// Additional Gates
//=================================

bool Or8Way(const std::array<bool, 8> &in8);

} // namespace bitforge::hardware
