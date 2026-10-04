#include "bitforge/hardware/rom.hpp"
#include "bitforge/hardware/types.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <string>
#include <sys/types.h>
void trim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));

  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}
using namespace std;

namespace bitforge::hardware {

ROM32K::ROM32K() { programSize = 0; }

pair<bool, uint16_t> ROM32K::parseLine(string &line) {

  if (line.size() != 16)
    return {false, 0};
  uint16_t v = 0;
  for (int i = 0; i < 16; i++) {
    if (line[i] != '1' && line[i] != '0')
      return {false, 0};
    if (line[i] == '1')
      v |= (1u << i);
  }
  return {true, v};
}
bool ROM32K::loadStream(ifstream &file) {
  vector<uint16_t> fresh = vector<uint16_t>(32768, 0);
  int curr = 0;
  string line;
  while (getline(file, line)) {
    trim(line);
    if (line.empty())
      continue;
    if (curr == 32768)
      return false;
    auto [ok, value] = ROM32K::parseLine(line);
    if (!ok)
      return false;
    fresh[curr++] = value;
  }
  data = move(fresh);
  programSize = curr;
  return true;
}
bool ROM32K::loadFile(string &path) {
  ifstream file(path);

  if (!file)
    return false;

  return ROM32K::loadStream(file);
}

uint16_t ROM32K::readRaw(int addr) {
  if (addr < 0 || addr >= 32768)
    return 0;
  return data[addr & 0x7FFF];
}
Bits16 ROM32K::read(int addr) {
  Bits16 value;
  if (addr < 0 || addr >= 32768)
    return value;
  uint16_t v = data[addr];

  for (int i = 0; i < 16; i++)
    value[i] = (v >> i) & 1;

  return value;
}

} // namespace bitforge::hardware
