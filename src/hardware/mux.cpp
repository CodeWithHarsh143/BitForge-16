#include "bitforge/hardware/mux.hpp"
#include "bitforge/hardware/gates.hpp"
namespace bitforge::hardware
{

  bool Mux(bool a, bool b, bool sel)
  {
    bool not_sel = Not(sel);
    bool a_And_not_sel = And(a, not_sel);
    bool b_And_sel = And(b, sel);
    return Or(a_And_not_sel, b_And_sel);
  }

  Bits16 Mux16(const Bits16 &a16,
               const Bits16 &b16, bool sel)
  {
    Bits16 out16;
    for (int i = 0; i < 16; i++)
      out16[i] = Mux(a16[i], b16[i], sel);
    return out16;
  }

  Bits16 Mux4Way16(const Bits16 &a16,
                   const Bits16 &b16,
                   const Bits16 &c16,
                   const Bits16 &d16,
                   const Bits2 &sel2)
  {
    auto a16_Mux_b16 = Mux16(a16, b16, sel2[0]);
    auto c16_Mux_d16 = Mux16(c16, d16, sel2[0]);
    return Mux16(a16_Mux_b16, c16_Mux_d16, sel2[1]);
  }

  Bits16
  Mux8Way16(const Bits16 &a16, const Bits16 &b16,
            const Bits16 &c16, const Bits16 &d16,
            const Bits16 &e16, const Bits16 &f16,
            const Bits16 &g16, const Bits16 &h16,
            const Bits3 &sel3)
  {
    auto a16_Mux16_e16 = Mux16(a16, e16, sel3[2]);
    auto b16_Mux16_f16 = Mux16(b16, f16, sel3[2]);
    auto c16_Mux16_g16 = Mux16(c16, g16, sel3[2]);
    auto d16_Mux16_h16 = Mux16(d16, h16, sel3[2]);
    Bits2 sel2 = {sel3[0], sel3[1]};
    return Mux4Way16(a16_Mux16_e16, b16_Mux16_f16, c16_Mux16_g16, d16_Mux16_h16,
                     sel2);
  }

  std::pair<bool, bool> Dmux(bool in, bool sel)
  {
    bool not_sel = Not(sel);
    bool a = And(in, not_sel);
    bool b = And(in, sel);
    return {a, b};
  }

  Bits4 Dmux4Way(bool in, const Bits2 &sel2)
  {
    auto [ab, cd] = Dmux(in, sel2[1]);
    auto [a, b] = Dmux(ab, sel2[0]);
    auto [c, d] = Dmux(cd, sel2[0]);
    return {a, b, c, d};
  }

  Bits8 Dmux8Way(bool in, const Bits3 &sel3)
  {
    auto [abcd, efgh] = Dmux(in, sel3[2]);
    auto [a, b, c, d] = Dmux4Way(abcd, {sel3[0], sel3[1]});
    auto [e, f, g, h] = Dmux4Way(efgh, {sel3[0], sel3[1]});

    return {a, b, c, d, e, f, g, h};
  }

} // namespace bitforge::hardware
