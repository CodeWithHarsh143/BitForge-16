#pragma once
#include "bitforge/hardware/types.hpp"

namespace bitforge::hardware {
Bits16 Ram8(Bits16 &inp, Bits3 &address, bool load);
}
