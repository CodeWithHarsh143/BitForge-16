#include "bitforge/display/screen_view.hpp"
#include "bitforge/hardware/memory_mapped.hpp"
#include "bitforge/io/keymap.hpp"
#include <cstdlib>
#include <SDL3/SDL.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_set>
#include <vector>

using bitforge::display::ConvertWordsToPixels;
using bitforge::display::kScreenH;
using bitforge::display::kScreenW;
using bitforge::display::kWords;
using bitforge::hardware::Bits15;
using bitforge::hardware::Bits16;
using bitforge::hardware::Memory;

namespace
{
  // 16-bit masking: Hack words are 16-bit, larger ints wrap (& 0xFFFF).
  Bits16 ToBits16(int v)
  {
    Bits16 b{};
    v &= 0xFFFF;
    for (int i = 0; i < 16; i++)
      b[i] = (v >> i) & 1;
    return b;
  }
  int ToInt16(Bits16 b)
  {
    int v = 0;
    for (int i = 0; i < 16; i++)
      if (b[i])
        v |= (1 << i);
    return v;
  }
  Bits15 ToAddr15(int v)
  {
    Bits15 a{};
    for (int i = 0; i < 15; i++)
      a[i] = (v >> i) & 1;
    return a;
  }
  void MemWrite(Memory &m, int addr, int value)
  {
    if (addr < 0 || addr > 32767)
      return; // invalid: ignore (edge case 24577+ handled inside Memory too)
    m.evaluate(ToBits16(value), ToAddr15(addr), true);
  }
  int MemRead(Memory &m, int addr)
  {
    if (addr < 0 || addr > 32767)
      return 0;
    Bits16 z{};
    return ToInt16(m.evaluate(z, ToAddr15(addr), false));
  }

  // ---- Demo experiments proving write -> display (Topic 11) ----
  void DemoClear(Memory &m)
  {
    for (int i = 0; i < kWords; i++)
      MemWrite(m, 16384 + i, 0);
  }
  void DemoOnePixel(Memory &m)
  {
    DemoClear(m);
    // (row 0, col 20) -> address 16385, bit 4 => value 1<<4
    MemWrite(m, 16385, 1 << 4);
  }
  void DemoOneRow(Memory &m)
  {
    DemoClear(m);
    for (int i = 0; i < 32; i++)
      MemWrite(m, 16384 + i, 0xFFFF);
  }
  void DemoAllOn(Memory &m)
  {
    for (int i = 0; i < kWords; i++)
      MemWrite(m, 16384 + i, 0xFFFF);
  }
  // Single pixel: addr = 16384 + row*32 + col/16, bit = col%16.
  // Read-modify-write so the other 15 pixels in the word survive.
  void SetPixel(Memory &m, int row, int col, bool on)
  {
    if (row < 0 || row >= 256 || col < 0 || col >= 512)
      return;
    int addr = 16384 + row * 32 + col / 16;
    int bit = col % 16;
    int w = MemRead(m, addr);
    if (on)
      w |= (1 << bit);
    else
      w &= ~(1 << bit);
    MemWrite(m, addr, w);
  }
} // namespace

static int SmokeTest()
{
  // Headless: Memory routing + pixel convert + keymap, no window.
  Memory m;
  MemWrite(m, 5, 0x00FF);
  if (MemRead(m, 5) != 0x00FF)
    return 1;
  DemoOnePixel(m);
  if (MemRead(m, 16385) != (1 << 4))
    return 2;
  // Bit clear: same word, bit off
  int w = MemRead(m, 16385) & ~(1 << 4);
  MemWrite(m, 16385, w);
  if (MemRead(m, 16385) != 0)
    return 3;
  // Masking: 0x1FFFF must wrap to 0xFFFF
  MemWrite(m, 100, 0x1FFFF);
  if (MemRead(m, 100) != 0xFFFF)
    return 4;
  // Invalid address: read 0, write ignored
  if (MemRead(m, 30000) != 0)
    return 5;
  MemWrite(m, 30000, 0x1234);
  if (MemRead(m, 5) != 0x00FF)
    return 6;
  // Keyboard: host-set visible, CPU write ignored
  m.setKeyboard(ToBits16(65));
  if (MemRead(m, 24576) != 65)
    return 7;
  MemWrite(m, 24576, 99);
  if (MemRead(m, 24576) != 65)
    return 8;
  m.clearKeyboard();
  if (MemRead(m, 24576) != 0)
    return 9;
  // Pixel convert: one word on -> 16 black dots at row 0, x 16-31
  std::vector<uint16_t> words(kWords, 0);
  words[1] = static_cast<uint16_t>(1 << 4); // offset 1 == address 16385
  std::vector<uint32_t> px(kScreenW * kScreenH, 0);
  ConvertWordsToPixels(words.data(), px.data());
  if (px[20] != 0xFF000000u)
    return 10; // (0,20) must be black
  if (px[0] != 0xFFFFFFFFu)
    return 11; // (0,0) stays white
  // Keymap flexibility: custom override changes behavior
  bitforge::io::HackKeymap km;
  if (km.toHack(SDLK_RETURN) != 128 || km.toHack(SDLK_LEFT) != 130)
    return 12;
  km.set(SDLK_F1, 999);
  if (km.toHack(SDLK_F1) != 999)
    return 13;
  std::printf("SMOKE_OK\n");
  return 0;
}

int main(int argc, char **argv)
{
  for (int i = 1; i < argc; i++)
  {
    if (std::strcmp(argv[i], "--smoke") == 0)
      return SmokeTest();
  }

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::printf("SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  Memory mem;
  bitforge::display::ScreenView view;

  if (!view.init("BitForge-16  (Hack Screen 512x256)", 3))
  {
    std::printf("Window failed: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  std::printf("title: %s\n", SDL_GetWindowTitle(view.window()));
  bitforge::io::HackKeymap keymap;
  // Example custom mapping (Topic 15): uncomment to change behavior.
  // keymap.set(SDLK_W, 131); // W acts as Up-arrow (131)

  std::vector<uint16_t> words(kWords, 0);
  std::vector<uint32_t> pixels(kScreenW * kScreenH, 0xFFFFFFFFu);

  // CPU/display separation (Topic 10): CPU would tick full-speed here;
  // display refreshes only ~60 FPS below. Demo script cycles experiments.
  const char *phaseName[4] = {"clear", "one pixel (0,20)", "one row", "all on"};
  int phase = 0;
  Uint64 lastPhase = SDL_GetTicks();
  Uint64 lastFrame = SDL_GetTicks();
  DemoClear(mem);
  SetPixel(mem, 0, 20, true);    // initial pixel for phase 1
  SetPixel(mem, 100, 100, true); // initial pixel for phase 2
  SetPixel(mem, 200, 300, true); // initial pixel for phase 3
  SetPixel(mem, 255, 511, true); // initial pixel for phase 4
  MemWrite(mem, 100, 0x1FFFF);   // -> 0xFFFF via masking
  MemWrite(mem, 30000, 0x1234);  // invalid -> ignored
  MemWrite(mem, 24576, 77);      // keyboard CPU-write -> ignored

  bool running = true;
  std::unordered_set<SDL_Keycode> held;
  int curHack = 0;

  while (running)
  {
    // ---- Events: quit + key press/release -> address 24576 ----
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_EVENT_QUIT)
      {
        running = false; // window X / Cmd-Q: clean exit path
      }
      else if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat)
      {
        held.insert(e.key.key);
        curHack = keymap.toHack(e.key.key);
        std::printf("SDL key: %d -> Hack key: %d\n",
                    e.key.key, curHack);
        if (curHack)
          mem.setKeyboard(ToBits16(curHack));
        // Multiple keys: last press wins (single Hack register).
      }
      else if (e.type == SDL_EVENT_KEY_UP)
      {
        held.erase(e.key.key);
        if (held.empty())
        {
          curHack = 0;
          mem.clearKeyboard(); // release -> 0
        }
        else
        {
          curHack = keymap.toHack(*held.begin()); // one survivor stays
          mem.setKeyboard(ToBits16(curHack));
        }
      }
    }

    // ---- Demo script: switch experiment every 2s (live bit effect) ----
    Uint64 now = SDL_GetTicks();
    // if (now - lastPhase > 2000)
    // {
    //   lastPhase = now;
    //   phase = (phase + 1) % 4;
    //   if (phase == 0)
    //     DemoClear(mem);
    //   else if (phase == 1)
    //     DemoOnePixel(mem);
    //   else if (phase == 2)
    //     DemoOneRow(mem);
    //   else
    //     DemoAllOn(mem);
    // }

    // ---- 60 FPS display refresh (full-buffer read; see note below) ----
    if (now - lastFrame >= 16)
    {
      lastFrame = now;
      // Trade-off note: full 8192-word read each frame is ~8K RAM
      // evaluates (~60*8K = 500K/s) — trivial for CPU, simplest correct.
      // Dirty-flag (refresh only written words) is faster but adds
      // bookkeeping; unnecessary at this size.
      for (int i = 0; i < kWords; i++)
        words[i] = static_cast<uint16_t>(ToInt16(mem.readScreen(i)));
      ConvertWordsToPixels(words.data(), pixels.data());
      if (!view.present(pixels.data()))
      {
        std::printf("Present failed: %s\n", SDL_GetError());
        break;
      }
      char title[128];
      std::snprintf(title, sizeof(title),
                    "BitForge-16 [%s]  key=%d (addr 24576)", phaseName[phase],
                    MemRead(mem, 24576));
      SDL_SetWindowTitle(view.window(), title);
    }
    else
    {
      SDL_Delay(1); // yield; CPU work would go here at full speed
    }
  }

  view.shutdown();
  SDL_Quit();
  return 0;
}
