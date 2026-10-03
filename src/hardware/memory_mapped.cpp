#include "bitforge/hardware/memory_mapped.hpp"
#include "bitforge/hardware/mux.hpp"

namespace bitforge::hardware {

Screen8K::Screen8K() {}

Bits16 Screen8K::evaluate(const Bits16 &inp, const Bits13 &address, bool load) {
  // MSB (value 4096) selects chip, lower 12 bits address inside.
  // Pruned: only the selected chip is evaluated. Same result as
  // Dmux(load)->both chips + Mux(out), because load=0 leaves the
  // other chip untouched and Mux discards its output anyway.
  bool sel = address[12];
  Bits12 inner{};
  for (int i = 0; i < 12; i++)
    inner[i] = address[i];
  if (!sel)
    return low.evaluate(inp, inner, load);
  return high.evaluate(inp, inner, load);
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

int Memory::toOffset13(const Bits13 &a) {
  int v = 0;
  for (int i = 0; i < 13; i++)
    if (a[i])
      v |= (1 << i);
  return v;
}

uint16_t Memory::toU16(const Bits16 &b) {
  uint16_t v = 0;
  for (int i = 0; i < 16; i++)
    if (b[i])
      v |= static_cast<uint16_t>(1u << i);
  return v;
}

Bits16 Memory::toBits16(uint16_t v) {
  Bits16 b{};
  for (int i = 0; i < 16; i++)
    b[i] = (v >> i) & 1u;
  return b;
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
    Bits16 out = screen.evaluate(inp, scrAddr, load);
    if (load)
      screenFlat[toOffset13(scrAddr)] = toU16(inp); // keep shadow in sync
    return out;
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
  // O(1) flat shadow read for 60 FPS display. No hierarchy walk.
  // Masked like hardware: offset wraps into 0-8191.
  int idx = offset & 8191;
  if (offset < 0)
    idx = ((offset % 8192) + 8192) % 8192;
  return toBits16(screenFlat[idx]);
}

} // namespace bitforge::hardware
