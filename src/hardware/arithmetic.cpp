#include "bitforge/hardware/arithmetic.hpp"
#include "bitforge/hardware/gates.hpp"

namespace bitforge::hardware
{

  HalfAdderResult half_adder(bool a, bool b)
  {
    const bool sum = Xor(a, b);
    const bool carry = And(a, b);
    return {sum, carry};
  }
  FullAdderResult full_adder(bool a, bool b, bool c)
  {
    const bool sum = Xor(Xor(a, b), c);
    const bool carry = Or(Or(And(a, b), And(a, c)), And(b, c));
    return {sum, carry};
  }
} // namespace bitforge::hardware
