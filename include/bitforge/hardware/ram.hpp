#pragma once
#include "bitforge/hardware/registers.hpp"
#include "bitforge/hardware/types.hpp"
namespace bitforge::hardware {
class RAM8 {
private:
  std::array<Register, 8> registers;

public:
  RAM8();
  Bits16 evaluate(const Bits16 &inp, const Bits3 &address, bool load);
};

class RAM64 {
private:
  std::array<RAM8, 8> rams;

public:
  RAM64();
  Bits16 evaluate(const Bits16 &inp, const Bits6 &address, bool load);
};
class RAM512 {
private:
  std::array<RAM64, 8> rams;

public:
  RAM512();
  Bits16 evaluate(const Bits16 &inp, const Bits9 &address, bool load);
};

class RAM4K {
private:
  std::array<RAM512, 8> rams;

public:
  RAM4K();
  Bits16 evaluate(const Bits16 &inp, const Bits12 &address, bool load);
};
class RAM16K {
private:
  std::array<RAM4K, 4> rams;

public:
  RAM16K();
  Bits16 evaluate(const Bits16 &inp, const Bits14 &address, bool load);
};
} // namespace bitforge::hardware
