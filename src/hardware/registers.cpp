#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/mux.hpp"
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware {

Bit::Bit() {
  Q = false;
  dff.evaluate(false, true);
  dff.evaluate(false, false);
}
void Bit::evaluate(bool inp, bool prev, bool load) {

  bool selected = Mux(prev, inp, load);
  dff.evaluate(selected, true);
  dff.evaluate(selected, false);
  Q = dff.getQ();
}
bool Bit::getQ() { return Q; }
Register::Register() {
  for (int pos = 0; pos < 16; pos++) {
    Q[pos] = bit.getQ();
  }
}
void Register::evaluate(Bits16 &inp, bool load) {
  for (int pos = 0; pos < 16; pos++) {
    bit.evaluate(inp[pos], Q[pos], load);
    Q[pos] = bit.getQ();
  }
}
Bits16 Register::getQ() { return Q; }
} // namespace bitforge::hardware
