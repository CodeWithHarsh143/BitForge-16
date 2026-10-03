#include "bitforge/hardware/ram.hpp"
#include "bitforge/hardware/mux.hpp"
#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware {
RAM8::RAM8() {}
Bits16 RAM8::evaluate(const Bits16 &inp, const Bits3 &address, bool load) {
  // Pruned: only the selected register is touched. Others would get
  // load=0 (state unchanged) and their outputs are discarded by the Mux,
  // so skipping them is observably identical, ~8x faster.
  int idx = (address[0] ? 1 : 0) | (address[1] ? 2 : 0) | (address[2] ? 4 : 0);
  if (load)
    registers[idx].evaluate(inp, true);
  return registers[idx].getQ();
}

RAM64::RAM64() {}
Bits16 RAM64::evaluate(const Bits16 &inp, const Bits6 &address, bool load) {
  const Bits3 ram_address{
      address[3],
      address[4],
      address[5],
  };
  int idx = (address[0] ? 1 : 0) | (address[1] ? 2 : 0) | (address[2] ? 4 : 0);
  return rams[idx].evaluate(inp, ram_address, load);
}

RAM512::RAM512() {}
Bits16 RAM512::evaluate(const Bits16 &inp, const Bits9 &address, bool load) {
  const Bits6 ram_address{
      address[3], address[4], address[5], address[6], address[7], address[8],
  };
  // Pruned: same observable behavior as Dmux-broadcast + Mux-select.
  int idx = (address[0] ? 1 : 0) | (address[1] ? 2 : 0) | (address[2] ? 4 : 0);
  return rams[idx].evaluate(inp, ram_address, load);
}

RAM4K::RAM4K() {}
Bits16 RAM4K::evaluate(const Bits16 &inp, const Bits12 &address, bool load) {
  const Bits9 ram_address{address[3], address[4],  address[5],
                          address[6], address[7],  address[8],
                          address[9], address[10], address[11]};
  // Pruned: same observable behavior as Dmux-broadcast + Mux-select.
  int idx = (address[0] ? 1 : 0) | (address[1] ? 2 : 0) | (address[2] ? 4 : 0);
  return rams[idx].evaluate(inp, ram_address, load);
}

RAM16K::RAM16K() {}
Bits16 RAM16K::evaluate(const Bits16 &inp, const Bits14 &address, bool load) {
  const Bits12 ram_address{address[2],  address[3],  address[4],
                           address[5],  address[6],  address[7],
                           address[8],  address[9],  address[10],
                           address[11], address[12], address[13]};
  // Pruned: same observable behavior as Dmux-broadcast + Mux-select.
  int idx = (address[0] ? 1 : 0) | (address[1] ? 2 : 0);
  return rams[idx].evaluate(inp, ram_address, load);
}

} // namespace bitforge::hardware