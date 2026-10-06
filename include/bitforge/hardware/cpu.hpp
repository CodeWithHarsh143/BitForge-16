#pragma once
#include "bitforge/hardware/memory_mapped.hpp"
#include "bitforge/hardware/pc.hpp"
#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/rom.hpp"
#include "bitforge/hardware/types.hpp"
#include <istream>
#include <string>
namespace bitforge::hardware {

class CPU {
private:
  Register A;
  Register D;
  ROM32K rom;
  PC pc;
  Bits16 curr{};
  Memory mem;

public:
  CPU();
  void evaluate();
  bool loadROM(const std::string &path);
  bool loadROMStream(std::istream &in);
  Bits16 getA();
  Bits16 getD();
  int getPC(); // mirrors last PC output (curr)
  Bits16 A_instruction(Bits16 &instr);
  Bits16 C_instruction(Bits16 &instr);
  // dest
  void dest_ADM(Bits16 &result, bool load);
  void dest_A(Bits16 &result, bool load);
  void dest_D(Bits16 &result, bool load);
  void dest_M(Bits16 &result, bool load);
  void dest_AM(Bits16 &result, bool load);
  void dest_AD(Bits16 &result, bool load);
  void dest_DM(Bits16 &result, bool load);
};
} // namespace bitforge::hardware
