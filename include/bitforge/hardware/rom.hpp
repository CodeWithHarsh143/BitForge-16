#pragma once
#include "bitforge/hardware/types.hpp"
#include <cstdint>
#include <istream>
#include <string>
#include <utility>
#include <vector>

namespace bitforge::hardware {
class ROM32K {

private:
  std::vector<uint16_t> data = std::vector<uint16_t>(32768, 0);

  int programSize = 0;

public:
  ROM32K();
  std::pair<bool, uint16_t> parseLine(const std::string &line);
  bool loadStream(std::istream &file);
  bool loadFile(const std::string &path);
  uint16_t readRaw(int addr) const;
  Bits16 read(int addr) const;
};
} // namespace bitforge::hardware
