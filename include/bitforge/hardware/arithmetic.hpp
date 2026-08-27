#pragma once
#include <array>
using Bits16 = std::array<bool, 16>;
namespace bitforge::hardware
{
  //
  // 1-Bit Adder/Subtractor
  //
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
  struct AdderSubtractorResult
  {
    const bool result;
    const bool carry;
  };
  struct AdderSubtractor16Result
  {
    Bits16 result;
  };
  HalfAdderResult half_adder(bool a, bool b);
  FullAdderResult full_adder(bool a, bool b, bool c);
  AdderSubtractorResult adder_subtractor(bool a, bool b, bool c, bool sep);
  AdderSubtractor16Result adder_subtractor16(const Bits16 &a, const Bits16 &b, bool sep);
} // namespace bitforge::hardware
