#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/mux.hpp"

namespace bitforge::hardware {

Bit::Bit() {
  Q = false;
  dff.evaluate(false, true);
  dff.evaluate(false, false);
}
void Bit::evaluate(bool inp, bool load) {
  bool prev = dff.getQ();
  bool selected = Mux(prev, inp, load);
  dff.evaluate(selected, true);
  dff.evaluate(selected, false);
  Q = prev;
}
bool Bit::getQ() { return Q; }
} // namespace bitforge::hardware
