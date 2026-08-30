#include "bitforge/hardware/gates.hpp"
namespace bitforge::hardware
{
  bool Nand(bool a, bool b)
  {
    if (a == true && b == true)
      return false;
    return true;
  }

  bool Not(bool a) { return Nand(a, a); }

  bool And(bool a, bool b)
  {
    bool nand = Nand(a, b);
    return Not(nand);
  }

  bool Or(bool a, bool b)
  {
    bool not_a = Not(a);
    bool not_b = Not(b);
    bool not_a_and_not_b = And(not_a, not_b);
    return Not(not_a_and_not_b);
  }

  bool Xor(bool a, bool b)
  {
    bool not_a = Not(a);
    bool not_b = Not(b);
    bool a_and_not_b = And(a, not_b);
    bool b_and_not_a = And(not_a, b);

    return Or(a_and_not_b, b_and_not_a);
  }

  Bits16 Nand16(const Bits16 &a16, const Bits16 &b16)
  {
    Bits16 out16;
    for (int i = 0; i < 16; i++)
      out16[i] = Nand(a16[i], b16[i]);
    return out16;
  }

  Bits16 Not16(const Bits16 &a16)
  {
    Bits16 out16;
    for (int i = 0; i < 16; i++)
      out16[i] = Not(a16[i]);
    return out16;
  }

  Bits16 And16(const Bits16 &a16, const Bits16 &b16)
  {
    auto nand_ab = Nand16(a16, b16);
    return Not16(nand_ab);
  }

  Bits16 Or16(const Bits16 &a16, const Bits16 &b16)
  {
    auto not_a = Not16(a16);
    auto not_b = Not16(b16);
    auto not_a_and_not_b = And16(not_a, not_b);
    return Not16(not_a_and_not_b);
  }

  Bits16 Xor16(const Bits16 &a16, const Bits16 &b16)
  {
    auto not_a = Not16(a16);
    auto not_b = Not16(b16);
    auto a_and_not_b = And16(a16, not_b);
    auto b_and_not_a = And16(not_a, b16);
    return Or16(a_and_not_b, b_and_not_a);
  }

  bool Or8Way(const Bits8 &in8)
  {
    bool out = false;
    for (int i = 0; i < 8; i++)
      out = Or(out, in8[i]);
    return out;
  }

} // namespace bitforge::hardware
