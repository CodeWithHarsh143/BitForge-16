#include "bitforge/hardware/arithmetic.hpp"
#include "bitforge/hardware/gates.hpp"

namespace bitforge::hardware {

HalfAdderResult half_adder(bool a, bool b) {

  return HalfAdderResult{.sum = Xor(a, b), .carry = And(a, b)};
}

} // namespace bitforge::hardware
