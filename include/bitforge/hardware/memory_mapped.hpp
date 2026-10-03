#pragma once
#include "bitforge/hardware/ram.hpp"
#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/types.hpp"
#include <array>
#include <cstdint>

namespace bitforge::hardware {

// Screen = 8192 words (512x256, 16 px/word), built as 2 x RAM4K.
// Convention: Bits arrays are LSB-first: addr[0]=value 1, addr[12]=value 4096.
// offset 0-4095 -> chip0, 4096-8191 -> chip1 (contiguous, display-friendly).
class Screen8K {
private:
  RAM4K low;  // offset 0-4095
  RAM4K high; // offset 4096-8191

public:
  Screen8K();
  Bits16 evaluate(const Bits16 &inp, const Bits13 &address, bool load);
  // Direct read for display (no decode overhead). offset 0-8191.
  Bits16 read(const Bits13 &address);
};

class Memory {
private:
  RAM16K ram;      // 0-16383
  Screen8K screen; // 16384-24575 (hierarchy: CPU-truth)
  Register kbd;    // 24576 only (1 word)
  // Flat shadow of screen for 60 FPS display: same data as `screen`,
  // updated on every screen write. Display reads this O(1); CPU reads
  // still go through the hierarchy. Both start zero, stay in sync.
  std::array<uint16_t, 8192> screenFlat{};

  static bool isZero13(const Bits13 &a);
  static int toOffset13(const Bits13 &a);
  static uint16_t toU16(const Bits16 &b);
  static Bits16 toBits16(uint16_t v);

public:
  Memory();
  // Full 15-bit address space: 0-32767. load=true => CPU write.
  // RAM/Screen route by bit14/bit13. Keyboard is read-only from CPU
  // (CPU writes to 24576 are ignored). Invalid 24577+ : read 0, write ignored.
  Bits16 evaluate(const Bits16 &inp, const Bits15 &address, bool load);

  // Host -> keyboard (SDL key press/release calls this, not CPU).
  void setKeyboard(const Bits16 &code);
  void clearKeyboard();

  // Display helper: read screen word by flat offset 0-8191.
  Bits16 readScreen(int offset);
};

} // namespace bitforge::hardware
