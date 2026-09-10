#include "bitforge/hardware/memory.hpp"
#include "bitforge/hardware/gates.hpp"
// SR Latch
namespace bitforge::hardware {
SR_LATCH::SR_LATCH() {
    Q = false;
    Qn = false;
}

void SR_LATCH::evaluate(bool S, bool R) {
  if (S && !R) {
    Q = true;
    Qn = false;
  } else if (!S && R) {
    Q = false;
    Qn = true;
  } else if (S && R) {
    Q = false;
    Qn = false;
  }
}

bool SR_LATCH::getQ() { return Q; }
bool SR_LATCH::getQn() { return Qn; }
// JK Flip Flop
JKFF::JKFF() {
    master.evaluate(true, false);
}
void JKFF::evaluate(bool J, bool K, bool clock) {
  Q = master.getQ();
  Qn = master.getQn();
  master.evaluate(And(Qn, And(J, clock)), And(Q, And(K, clock)));
  bool Q_m = master.getQ();
  bool Qn_m = master.getQn();
  slave.evaluate(And(Q_m, Not(clock)), And(Qn_m, Not(clock)));
  Q = slave.getQ();
  Qn = slave.getQn();
}
bool JKFF::getQ() { return Q; }
bool JKFF::getQn() { return Qn; }
// D Flip Flop
DFF::DFF() {}
void DFF::evaluate(bool D, bool clock) {
  bool J = D;
  bool K = Not(D);
  jk.evaluate(J, K, clock);
  Q = jk.getQ();
  Qn = jk.getQn();
}
bool DFF::getQ() { return Q; }
bool DFF::getQn() { return Qn; }
} // namespace bitforge::hardware
