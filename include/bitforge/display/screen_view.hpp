#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <vector>

namespace bitforge::display {

inline constexpr int kScreenW = 512;
inline constexpr int kScreenH = 256;
inline constexpr int kWords = 8192; // 512*256/16
inline constexpr int kWordsPerRow = 32;

// Pure logic, no SDL: 8192 words -> 512*256 ARGB pixels.
// bit 1 = black, bit 0 = white. Word bit b (0=LSB) -> x = wcol*16 + b.
// words[i] is the 16-bit value of screen offset i (already masked to 0xFFFF).
void ConvertWordsToPixels(const uint16_t *words, uint32_t *pixels);

// Forward-declared to avoid pulling hardware headers into SDL TU when testing.
class ScreenView {
public:
  ScreenView();
  ~ScreenView();
  ScreenView(const ScreenView &) = delete;
  ScreenView &operator=(const ScreenView &) = delete;

  // Opens window (scaled for visibility) + streaming ARGB8888 texture.
  bool init(const char *title, int scale = 2);
  // Uploads pixel buffer and presents. Returns false on SDL error.
  bool present(const uint32_t *pixels);
  void shutdown();

  SDL_Window *window() const { return window_; }
  SDL_Renderer *renderer() const { return renderer_; }

private:
  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
  SDL_Texture *texture_ = nullptr;
  int scale_ = 2;
};

} // namespace bitforge::display
