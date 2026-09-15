#include "bitforge/hardware/ram.hpp"
#include "bitforge/hardware/mux.hpp"
#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware {
RAM8::RAM8() {}
Bits16 RAM8::evaluate(const Bits16 &inp, const Bits3 &address, bool load) {
  Bits8 write_address = Dmux8Way(load, address);
  for (int pos = 0; pos < 8; pos++) {
    registers[pos].evaluate(inp, write_address[pos]);
  }
  return Mux8Way16(registers[0].getQ(), registers[1].getQ(),
                   registers[2].getQ(), registers[3].getQ(),
                   registers[4].getQ(), registers[5].getQ(),
                   registers[6].getQ(), registers[7].getQ(), address);
}

RAM64::RAM64() {}
Bits16 RAM64::evaluate(const Bits16 &inp, const Bits6 &address, bool load) {
  const Bits3 ram_selection{
      address[0],
      address[1],
      address[2],
  };
  const Bits3 ram_address{
      address[3],
      address[4],
      address[5],
  };
  Bits8 selected_ram = Dmux8Way(load, ram_selection);
  return Mux8Way16(rams[0].evaluate(inp, ram_address, selected_ram[0]),
                   rams[1].evaluate(inp, ram_address, selected_ram[1]),
                   rams[2].evaluate(inp, ram_address, selected_ram[2]),
                   rams[3].evaluate(inp, ram_address, selected_ram[3]),
                   rams[4].evaluate(inp, ram_address, selected_ram[4]),
                   rams[5].evaluate(inp, ram_address, selected_ram[5]),
                   rams[6].evaluate(inp, ram_address, selected_ram[6]),
                   rams[7].evaluate(inp, ram_address, selected_ram[7]),
                   ram_selection);
}

RAM512::RAM512() {}
Bits16 RAM512::evaluate(const Bits16 &inp, const Bits9 &address, bool load) {
  const Bits6 ram_address{
      address[3], address[4], address[5], address[6], address[7], address[8],
  };
  const Bits3 ram_selection{
      address[0],
      address[1],
      address[2],
  };
  Bits8 selected_ram = Dmux8Way(load, ram_selection);
  return Mux8Way16(rams[0].evaluate(inp, ram_address, selected_ram[0]),
                   rams[1].evaluate(inp, ram_address, selected_ram[1]),
                   rams[2].evaluate(inp, ram_address, selected_ram[2]),
                   rams[3].evaluate(inp, ram_address, selected_ram[3]),
                   rams[4].evaluate(inp, ram_address, selected_ram[4]),
                   rams[5].evaluate(inp, ram_address, selected_ram[5]),
                   rams[6].evaluate(inp, ram_address, selected_ram[6]),
                   rams[7].evaluate(inp, ram_address, selected_ram[7]),
                   ram_selection);
}

RAM4K::RAM4K() {}
Bits16 RAM4K::evaluate(const Bits16 &inp, const Bits12 &address, bool load) {
  const Bits9 ram_address{address[3], address[4],  address[5],
                          address[6], address[7],  address[8],
                          address[9], address[10], address[11]};
  const Bits3 ram_selection{
      address[0],
      address[1],
      address[2],
  };
  Bits8 selected_ram = Dmux8Way(load, ram_selection);
  return Mux8Way16(rams[0].evaluate(inp, ram_address, selected_ram[0]),
                   rams[1].evaluate(inp, ram_address, selected_ram[1]),
                   rams[2].evaluate(inp, ram_address, selected_ram[2]),
                   rams[3].evaluate(inp, ram_address, selected_ram[3]),
                   rams[4].evaluate(inp, ram_address, selected_ram[4]),
                   rams[5].evaluate(inp, ram_address, selected_ram[5]),
                   rams[6].evaluate(inp, ram_address, selected_ram[6]),
                   rams[7].evaluate(inp, ram_address, selected_ram[7]),
                   ram_selection);
}

RAM16K::RAM16K() {}
Bits16 RAM16K::evaluate(const Bits16 &inp, const Bits14 &address, bool load) {
  const Bits12 ram_address{address[2],  address[3],  address[4],
                           address[5],  address[6],  address[7],
                           address[8],  address[9],  address[10],
                           address[11], address[12], address[13]};
  const Bits2 ram_selection{
      address[0],
      address[1],
  };

  Bits4 selected_ram = Dmux4Way(load, ram_selection);
  return Mux4Way16(rams[0].evaluate(inp, ram_address, selected_ram[0]),
                   rams[1].evaluate(inp, ram_address, selected_ram[1]),
                   rams[2].evaluate(inp, ram_address, selected_ram[2]),
                   rams[3].evaluate(inp, ram_address, selected_ram[3]),
                   ram_selection);
}

} // namespace bitforge::hardware