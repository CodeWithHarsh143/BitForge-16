#include "bitforge/hardware/memory_mapped.hpp"
#include "bitforge/hardware/mux.hpp"

namespace bitforge::hardware {

Screen8K::Screen8K() {}

Bits16 Screen8K::evaluate(const Bits16 &inp, const Bits13 &address, bool load) {
  // MSB (value 4096) selects chip, lower 12 bits address inside.
  bool sel = address[12];
  Bits12 inner{};
  for (int i = 0; i < 12; i++)
    inner[i] = address[i];

  auto [l0, l1] = Dmux(load, sel);
  Bits16 o0 = low.evaluate(inp, inner, l0);
  Bits16 o1 = high.evaluate(inp, inner, l1);
  return Mux16(o0, o1, sel);
}

Bits16 Screen8K::read(const Bits13 &address) {
  return evaluate(Bits16{}, address, false);
}

Memory::Memory() {}

bool Memory::isZero13(const Bits13 &a) {
  for (bool b : a)
    if (b)
      return false;
  return true;
}

Bits16 Memory::evaluate(const Bits16 &inp, const Bits15 &address, bool load) {
  bool bit14 = address[14]; // value 16384
  bool bit13 = address[13]; // value 8192

  // Level 1: RAM (bit14==0) vs rest (bit14==1)
  if (!bit14) {
    Bits14 ramAddr{};
    for (int i = 0; i < 14; i++)
      ramAddr[i] = address[i];
    return ram.evaluate(inp, ramAddr, load);
  }

  // bit14==1: Screen (bit13==0) vs keyboard/invalid (bit13==1)
  if (!bit13) {
    Bits13 scrAddr{};
    for (int i = 0; i < 13; i++)
      scrAddr[i] = address[i];
    return screen.evaluate(inp, scrAddr, load);
  }

  // bit14==1 && bit13==1: only 24576 is keyboard, rest invalid.
  Bits13 low13{};
  for (int i = 0; i < 13; i++)
    low13[i] = address[i];
  if (isZero13(low13)) {
    // Keyboard is read-only from CPU: ignore load, return key code.
    // Still need to keep RAM/Screen untouched (we branched, so untouched).
    return kbd.getQ();
  }

  // Invalid 24577-32767: read 0, write ignored.
  return Bits16{};
}

void Memory::setKeyboard(const Bits16 &code) { kbd.evaluate(code, true); }

void Memory::clearKeyboard() {
  Bits16 zero{};
  kbd.evaluate(zero, true);
}

Bits16 Memory::readScreen(int offset) {
  Bits13 a{};
  for (int i = 0; i < 13; i++)
    a[i] = (offset >> i) & 1;
  return screen.read(a);
}

} // namespace bitforge::hardware
