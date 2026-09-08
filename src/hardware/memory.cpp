#include "bitforge/hardware/memory.hpp"
#include "bitforge/hardware/gates.hpp"

SR_LATCH::SR_LATCH()
{
  Q = false;
  Qn = true;
}

void SR_LATCH::evaluate(bool S, bool R)
{
  Q = bitforge::hardware::Nand(S, Qn);
  Qn = bitforge::hardware::Nand(R, Q);
}

bool SR_LATCH::getQ() { return Q; }
bool SR_LATCH::getQn() { return Qn; }

DFF::DFF()
{

  Q = false;

  Qn = false;
}
void DFF::evaluate(bool D, bool clock)
{
  bool R = bitforge::hardware::Not(D);

  bool S = D;
}
