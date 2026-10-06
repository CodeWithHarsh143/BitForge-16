#include "bitforge/hardware/rom.hpp"
#include "bitforge/hardware/types.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <string>
#include <utility>

namespace {
void trim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));

  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}
} // namespace

using namespace std;

namespace bitforge::hardware {

ROM32K::ROM32K() { programSize = 0; }

pair<bool, uint16_t> ROM32K::parseLine(const string &line) {

  if (line.size() != 16)
    return {false, 0};
  uint16_t v = 0;
  for (int i = 0; i < 16; i++) {
    if (line[i] != '1' && line[i] != '0')
      return {false, 0};
    // .hack is MSB-first: leftmost char is bit15 (value 32768).
    v = static_cast<uint16_t>((v << 1) | static_cast<uint16_t>(line[i] - '0'));
  }
  return {true, v};
}
bool ROM32K::loadStream(istream &file) {
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
bool ROM32K::loadFile(const string &path) {
  ifstream file(path);

  if (!file)
    return false;

  return ROM32K::loadStream(file);
}

uint16_t ROM32K::readRaw(int addr) const {
  return data[addr & 0x7FFF]; // 15-bit mask, like hardware address lines
}
Bits16 ROM32K::read(int addr) const {
  uint16_t v = readRaw(addr); // masked read; OOB wraps, never garbage
  Bits16 value{};

  for (int i = 0; i < 16; i++)
    value[i] = (v >> i) & 1;

  return value;
}

} // namespace bitforge::hardware
