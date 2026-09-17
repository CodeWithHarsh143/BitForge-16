#include "bitforge/hardware/arithmetic.hpp"
#include "bitforge/hardware/gates.hpp"
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware {
//=====================================
// 1-Bit Adders/Subtractor
//=====================================
HalfAdderResult half_adder(bool a, bool b) {
  const bool sum = Xor(a, b);
  const bool carry = And(a, b);
  return {sum, carry};
}
FullAdderResult full_adder(bool a, bool b, bool c) {
  const bool sum = Xor(Xor(a, b), c);
  const bool carry = Or(Or(And(a, b), And(a, c)), And(b, c));
  return {sum, carry};
}
AdderSubtractorResult adder_subtractor(bool a, bool b, bool c, bool sep) {
  bool b_xor_sep = Xor(b, sep);
  auto [result, carry] = full_adder(a, b_xor_sep, c);
  return {result, carry};
}
//====================================
// 16-Bit Adder/Subtractor
//====================================
Bits16 adder_subtractor16(const Bits16 &a, const Bits16 &b, bool sep) {
  Bits16 result{};
  bool carry = sep;
  for (int i = 15; i >= 0; --i) {
    auto [res, car] = adder_subtractor(a[i], b[i], carry, sep);
    result[i] = res;
    carry = car;
  }
  return result;
}
Bits16 increment_adder(const Bits16 &a) {
  Bits16 out;
  bool carry = true;
  for (int pos = 0; pos < 16; pos++) {
    auto [s, c] = full_adder(a[pos], false, carry);
    carry = c;
    out[pos] = s;
  }
  return out;
}
} // namespace bitforge::hardware
