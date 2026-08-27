#pragma once

namespace bitforge::hardware
{

  struct HalfAdderResult
  {
    const bool sum;
    const bool carry;
  };
  struct FullAdderResult
  {
    const bool sum;
    const bool carry;
  };
  HalfAdderResult half_adder(bool a, bool b);
  FullAdderResult full_adder(bool a, bool b, bool c);
} // namespace bitforge::hardware
