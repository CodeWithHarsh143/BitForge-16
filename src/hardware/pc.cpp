#include "bitforge/hardware/pc.hpp"
#include "bitforge/hardware/arithmetic.hpp"
#include "bitforge/hardware/mux.hpp"
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware {
PC::PC() {};
Bits16 PC::evaluate(const Bits16 &inp, bool load, bool inc, bool reset) {

  Bits16 current = registers.getQ();
  Bits16 increment = increment_adder(current);
  Bits16 next = Mux16(current, increment, inc);
  next = Mux16(next, inp, load);
  Bits16 resetBits{};
  next = Mux16(next, resetBits, reset);
  registers.evaluate(next, 1);
  return registers.getQ();
}
} // namespace bitforge::hardware
