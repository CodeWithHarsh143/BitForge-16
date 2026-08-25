#pragma once

namespace bitforge::hardware {

struct HalfAdderResult {
  bool sum;
  bool carry;
};
HalfAdderResult half_adder(bool a, bool b);
} // namespace bitforge::hardware
