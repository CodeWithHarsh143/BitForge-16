#ifndef MEMORY_HPP
#define MEMORY_HPP
class SR_LATCH
{
private:
  bool Q;
  bool Qn;

public:
  SR_LATCH();
  bool evalute(bool S, bool R);
};
class DFF
{
private:
  bool Q;
  bool Qn;

private:
  DFF();
  bool evaluate(bool D, bool clock);
};
#endif
