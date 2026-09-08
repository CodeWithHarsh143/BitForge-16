#pragma once

namespace bitforge::hardware
{
  class SR_LATCH
  {
  private:
    bool Q;
    bool Qn;

  public:
    SR_LATCH();
    void evaluate(bool S, bool R);
    bool getQ();
    bool getQn();
  };
  class JKFF
  {
  private:
    SR_LATCH master;
    SR_LATCH slave;
    bool Q;
    bool Qn;

  public:
    JKFF();
    void evaluate(bool J, bool K, bool clock);
    bool getQ();
    bool getQn();
  };
  class DFF
  {
  private:
    bool Q;
    bool Qn;
    JKFF jk;

  public:
    DFF();
    void evaluate(bool D, bool clock);
    bool getQ();
    bool getQn();
  };
}
