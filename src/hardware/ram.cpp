#include "bitforge/hardware/ram.hpp"
#include "bitforge/hardware/mux.hpp"
#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware {
RAM::RAM() {}
Bits16 RAM::Ram8(Bits16 &inp, const Bits3 &address, bool load) {
  Bits8 write_address = Dmux8Way(load, address);
  for (int pos = 0; pos < 8; pos++) {
    registers8[pos].evaluate(inp, write_address[pos]);
  }
  return Mux8Way16(registers8[0].getQ(), registers8[1].getQ(),
                   registers8[2].getQ(), registers8[3].getQ(),
                   registers8[4].getQ(), registers8[5].getQ(),
                   registers8[6].getQ(), registers8[7].getQ(), address);
}
} // namespace bitforge::hardware
