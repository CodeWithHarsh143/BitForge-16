#include "bitforge/hardware/alu.hpp"
#include "bitforge/hardware/arithmetic.hpp"
#include "bitforge/hardware/gates.hpp"
#include "bitforge/hardware/mux.hpp"
namespace bitforge::hardware
{
  AluResult Alu16(const Bits16 &x16, const Bits16 &y16, bool zx, bool nx, bool zy, bool ny, bool f, bool no)
  {
    const bool zero_x = Not(zx);
    const bool zero_y = Not(zy);
    Bits16 x_prepped, y_prepped;
    for (int i = 15; i >= 0; i--)
    {
      x_prepped[i] = And(x16[i], zero_x);
      x_prepped[i] = Xor(x_prepped[i], nx);
      y_prepped[i] = And(y16[i], zero_y);
      y_prepped[i] = Xor(y_prepped[i], ny);
    }
    Bits16 x_and_y = And16(x_prepped, y_prepped);
    Bits16 x_plus_y = adder_subtractor16(x_prepped, y_prepped, false);
    Bits16 result = Mux16(x_and_y, x_plus_y, f);
    for (int i = 15; i >= 0; i--)
    {
      result[i] = Xor(result[i], no);
    }
    const bool ng = result[0];
    Bits8 high_half, low_half;
    std::copy(result.begin(), result.begin() + 8, high_half.begin());
    std::copy(result.begin() + 8, result.begin() + 16, low_half.begin());
    const bool high_half_any = Or8Way(high_half);
    const bool low_half_any = Or8Way(low_half);
    const bool zr = Not(Or(high_half_any, low_half_any));
    return {result, zr, ng};
  }
}
