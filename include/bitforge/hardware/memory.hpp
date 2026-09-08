#ifndef MEMORY_HPP
#define MEMORY_HPP
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
class DFF
{
private:
  SR_LATCH master;
  SR_LATCH slave;
  bool Q;
  bool Qn;

public:
  DFF();
  void evaluate(bool D, bool clock);
  bool getQ();
  bool getQn();
};
#endif
