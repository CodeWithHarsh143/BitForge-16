#include "bitforge/hardware/gates.hpp"

namespace bitforge::hardware {

bool Nand(bool a, bool b) {
  if (a == true && b == true)
    return false;
  return true;
}

bool Not(bool a) { return Nand(a, a); }

bool And(bool a, bool b) {
  bool nand = Nand(a, b);
  return Not(nand);
}

bool Or(bool a, bool b) {
  bool not_a = Not(a);
  bool not_b = Not(b);
  bool not_a_And_not_b = And(not_a, not_b);
  return Not(not_a_And_not_b);
}

bool Xor(bool a, bool b) {
  bool not_a = Not(a);
  bool not_b = Not(b);
  bool a_And_not_b = And(a, not_b);
  bool b_And_not_a = And(not_a, b);

  return Or(a_And_not_b, b_And_not_a);
}

std::array<bool, 16> Nand16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16) {
  std::array<bool, 16> out16;
  for (int i = 0; i < 16; i++)
    out16[i] = Nand(a16[i], b16[i]);
  return out16;
}

std::array<bool, 16> Not16(const std::array<bool, 16> &a16) {
  std::array<bool, 16> out16;
  for (int i = 0; i < 16; i++)
    out16[i] = Not(a16[i]);
  return out16;
}

std::array<bool, 16> And16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16) {
  auto a16_Nand16_b16 = Nand16(a16, b16);
  return Not16(a16_Nand16_b16);
}

std::array<bool, 16> Or16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16) {
  auto not_a16 = Not16(a16);
  auto not_b16 = Not16(b16);
  auto not_a16_And16_not_b16 = And16(not_a16, not_b16);
  return Not16(not_a16_And16_not_b16);
}

std::array<bool, 16> Xor16(const std::array<bool, 16> &a16, const std::array<bool, 16> &b16) {
  auto not_a16 = Not16(a16);
  auto not_b16 = Not16(b16);
  auto a16_And16_not_b16 = And16(a16, not_b16);
  auto b16_And16_not_a16 = And16(not_a16, b16);
  return Or16(a16_And16_not_b16, b16_And16_not_a16);
}

bool Or8Way(const std::array<bool, 8> &in8) {
  bool out = false;
  for (int i = 0; i < 8; i++)
    out = Or(out, in8[i]);
  return out;
}

} // namespace bitforge::hardware
