#pragma once
#include "bitforge/hardware/types.hpp"
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
namespace bitforge::hardware {
class ROM32K {

private:
  vector<uint16_t> data = vector<uint16_t>(32768, 0);

  int programSize;

public:
  ROM32K();
  pair<bool, uint16_t> parseLine(string &line);
  bool loadStream(ifstream &file);
  bool loadFile(string &path);
  uint16_t readRaw(int addr);
  Bits16 read(int addr);
};
} // namespace bitforge::hardware
