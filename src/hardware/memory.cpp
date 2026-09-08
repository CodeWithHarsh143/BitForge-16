#include "bitforge/hardware/memory.hpp"
#include "bitforge/hardware/gates.hpp"
namespace bitforge::hardware
{
  SR_LATCH::SR_LATCH()
  {
    Q = true;
    Qn = false;
  }

  void SR_LATCH::evaluate(bool S, bool R)
  {
    Q = Nand(S, Qn);
    Qn = Nand(R, Q);
  }

  bool SR_LATCH::getQ() { return Q; }
  bool SR_LATCH::getQn() { return Qn; }

  DFF::DFF()
  {

    Q = true;

    Qn = false;
  }
  void DFF::evaluate(bool D, bool clock)
  {
    bool J = D;
    bool K = Not(D);
    master.evaluate(And(Qn, And(J, clock)), And(Q, And(K, clock)));
    bool Q_m = master.getQ();
    bool Qn_m = master.getQn();
    slave.evaluate(And(Q_m, Not(clock)), And(Qn_m, Not(clock)));
    Q = slave.getQ();
    Qn = slave.getQn();
  }
}
