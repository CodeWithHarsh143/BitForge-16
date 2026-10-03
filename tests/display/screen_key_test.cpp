#include "bitforge/display/screen_view.hpp"
#include "bitforge/io/keymap.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <vector>

using bitforge::display::ConvertWordsToPixels;
using bitforge::display::kScreenH;
using bitforge::display::kScreenW;
using bitforge::display::kWords;

TEST_CASE("Pixels all-white when screen is zero", "[screen]") {
  std::vector<uint16_t> words(kWords, 0);
  std::vector<uint32_t> px(kScreenW * kScreenH, 0);
  ConvertWordsToPixels(words.data(), px.data());
  REQUIRE(px[0] == 0xFFFFFFFFu);
  REQUIRE(px[kScreenW * kScreenH - 1] == 0xFFFFFFFFu);
}

TEST_CASE("Pixels all-black when screen is full", "[screen]") {
  std::vector<uint16_t> words(kWords, 0xFFFF);
  std::vector<uint32_t> px(kScreenW * kScreenH, 0);
  ConvertWordsToPixels(words.data(), px.data());
  REQUIRE(px[0] == 0xFF000000u);
  REQUIRE(px[20] == 0xFF000000u);
  REQUIRE(px[kScreenW * kScreenH - 1] == 0xFF000000u);
}

TEST_CASE("Pixel (0,20) maps from word offset 1 bit 4", "[screen]") {
  std::vector<uint16_t> words(kWords, 0);
  words[1] = static_cast<uint16_t>(1 << 4); // address 16385
  std::vector<uint32_t> px(kScreenW * kScreenH, 0);
  ConvertWordsToPixels(words.data(), px.data());
  REQUIRE(px[20] == 0xFF000000u); // black dot
  REQUIRE(px[0] == 0xFFFFFFFFu);  // neighbours stay white
  REQUIRE(px[16] == 0xFFFFFFFFu);
  REQUIRE(px[21] == 0xFFFFFFFFu);
  REQUIRE(px[kScreenW] == 0xFFFFFFFFu); // row 1 untouched
}

TEST_CASE("Row start offset 32 lands on second row", "[screen]") {
  std::vector<uint16_t> words(kWords, 0);
  words[32] = 0x0001; // row 1, col 0
  std::vector<uint32_t> px(kScreenW * kScreenH, 0);
  ConvertWordsToPixels(words.data(), px.data());
  REQUIRE(px[kScreenW] == 0xFF000000u);
  REQUIRE(px[0] == 0xFFFFFFFFu);
}

TEST_CASE("Keymap Hack special codes", "[keymap]") {
  bitforge::io::HackKeymap km;
  REQUIRE(km.toHack(SDLK_RETURN) == 128);
  REQUIRE(km.toHack(SDLK_BACKSPACE) == 129);
  REQUIRE(km.toHack(SDLK_LEFT) == 130);
  REQUIRE(km.toHack(SDLK_UP) == 131);
  REQUIRE(km.toHack(SDLK_RIGHT) == 132);
  REQUIRE(km.toHack(SDLK_DOWN) == 133);
  REQUIRE(km.toHack(SDLK_ESCAPE) == 140);
  REQUIRE(km.toHack(SDLK_F1) == 141);
  REQUIRE(km.toHack(SDLK_F12) == 152);
}

TEST_CASE("Keymap letters and digits", "[keymap]") {
  bitforge::io::HackKeymap km;
  REQUIRE(km.toHack(SDLK_A) == 65);
  REQUIRE(km.toHack(static_cast<SDL_Keycode>('a')) == 65);
  REQUIRE(km.toHack(static_cast<SDL_Keycode>('0')) == 48);
  REQUIRE(km.toHack(static_cast<SDL_Keycode>(' ')) == 32);
}

TEST_CASE("Keymap unknown key gives 0, custom table overrides", "[keymap]") {
  bitforge::io::HackKeymap km;
  REQUIRE(km.toHack(SDLK_CAPSLOCK) == 0);
  km.set(SDLK_F1, 999);
  REQUIRE(km.toHack(SDLK_F1) == 999);
  km.unbind(SDLK_F1);
  REQUIRE(km.toHack(SDLK_F1) == 0);
}
