#include "bitforge/hardware/cpu.hpp"
#include "bitforge/hardware/alu.hpp"
#include "bitforge/hardware/mux.hpp"
#include "bitforge/hardware/pc.hpp"
#include "bitforge/hardware/rom.hpp"
#include "bitforge/hardware/types.hpp"
#include <istream>
#include <string>

using namespace std;

namespace bitforge::hardware {

CPU::CPU() {}

namespace {
// Local helpers: Bits16 <-> int, LSB-first (bit[0] = value 1).
int ToInt16(const Bits16 &b) {
  int v = 0;
  for (int i = 0; i < 16; i++)
    if (b[i])
      v |= (1 << i);
  return v;
}
// Memory addresses are 15-bit: drop bit15 of a 16-bit register value.
Bits15 Addr15(const Bits16 &b) {
  Bits15 a{};
  for (int i = 0; i < 15; i++)
    a[i] = b[i];
  return a;
}
} // namespace

Bits16 CPU::A_instruction(Bits16 &instr) {
  A.evaluate(instr, true);
  return pc.evaluate(instr, false, true, false);
}
void CPU::dest_ADM(Bits16 &result, bool load) {
  mem.evaluate(result, Addr15(A.getQ()), load);
  A.evaluate(result, load);
  D.evaluate(result, load);
}
void CPU::dest_A(Bits16 &result, bool load) { A.evaluate(result, load); }
void CPU::dest_D(Bits16 &result, bool load) { D.evaluate(result, load); }
void CPU::dest_M(Bits16 &result, bool load) {
  mem.evaluate(result, Addr15(A.getQ()), load);
}
void CPU::dest_AD(Bits16 &result, bool load) {
  A.evaluate(result, load);
  D.evaluate(result, load);
}
void CPU::dest_AM(Bits16 &result, bool load) {
  mem.evaluate(result, Addr15(A.getQ()), load);
  A.evaluate(result, load);
}
void CPU::dest_DM(Bits16 &result, bool load) {
  mem.evaluate(result, Addr15(A.getQ()), load);
  D.evaluate(result, load);
}
Bits16 CPU::C_instruction(Bits16 &instr) {
  // y-input: A register or Memory[A], chosen by the a-bit (instr[12]).
  // M is read BEFORE any dest write, so it sees the previous cycle's A.
  Bits16 y = instr[12] ? mem.evaluate(Bits16{}, Addr15(A.getQ()), false)
                       : A.getQ();
  AluResult ar = Alu16(D.getQ(), y, instr[11], instr[10], instr[9], instr[8],
                       instr[7], instr[6]);
  Bits16 alu_result = ar.result;
  Bits3 dest_sep = {instr[3], instr[4], instr[5]};
  Bits8 dest = Dmux8Way(true, dest_sep);
  // for dest (only the matching entry carries load=true; dest[0] = null)
  dest_M(alu_result, dest[1]);
  dest_D(alu_result, dest[2]);
  dest_DM(alu_result, dest[3]);
  dest_A(alu_result, dest[4]);
  dest_AM(alu_result, dest[5]);
  dest_AD(alu_result, dest[6]);
  dest_ADM(alu_result, dest[7]);
  // jump: instr[2]=JLT(out<0), instr[1]=JEQ(out==0), instr[0]=JGT(out>0).
  bool pos = !ar.zr && !ar.ng;
  bool shouldJump =
      (instr[2] && ar.ng) || (instr[1] && ar.zr) || (instr[0] && pos);
  if (shouldJump)
    return pc.evaluate(A.getQ(), true, false, false); // PC = A
  Bits16 dummy{};
  return pc.evaluate(dummy, false, true, false); // PC + 1
}
void CPU::evaluate() {
  Bits16 instr = rom.read(ToInt16(curr));
  // Exactly one path executes: A-type (bit15=0) or C-type (bit15=1).
  if (!instr[15])
    curr = A_instruction(instr);
  else
    curr = C_instruction(instr);
}

bool CPU::loadROM(const std::string &path) { return rom.loadFile(path); }
bool CPU::loadROMStream(std::istream &in) { return rom.loadStream(in); }
Bits16 CPU::getA() { return A.getQ(); }
Bits16 CPU::getD() { return D.getQ(); }
int CPU::getPC() { return ToInt16(curr); }

} // namespace bitforge::hardware
