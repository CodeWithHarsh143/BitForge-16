#include "bitforge/display/screen_view.hpp"

namespace bitforge::display {

namespace {
inline constexpr uint32_t kWhite = 0xFFFFFFFFu; // ARGB white
inline constexpr uint32_t kBlack = 0xFF000000u; // ARGB black
} // namespace

void ConvertWordsToPixels(const uint16_t *words, uint32_t *pixels) {
  // One pass over 8192 words; each word fans out to 16 pixels.
  // No per-pixel draw calls: caller uploads the whole buffer once.
  for (int off = 0; off < kWords; ++off) {
    const uint16_t w = words[off];
    const int row = off / kWordsPerRow;
    const int wcol = off % kWordsPerRow;
    const int baseX = wcol * 16;
    const int baseY = row * kScreenW;
    for (int b = 0; b < 16; ++b) {
      const bool on = (w >> b) & 1u;
      pixels[baseY + baseX + b] = on ? kBlack : kWhite;
    }
  }
}

ScreenView::ScreenView() = default;
ScreenView::~ScreenView() { shutdown(); }

bool ScreenView::init(const char *title, int scale) {
  scale_ = scale > 0 ? scale : 1;
  window_ = SDL_CreateWindow(title, kScreenW * scale_, kScreenH * scale_, 0);
  if (!window_)
    return false;
  renderer_ = SDL_CreateRenderer(window_, nullptr);
  if (!renderer_) {
    shutdown();
    return false;
  }
  texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING, kScreenW, kScreenH);
  if (!texture_) {
    shutdown();
    return false;
  }
  // Scale texture up to window with minimal filtering side-effects.
  SDL_SetTextureScaleMode(texture_, SDL_SCALEMODE_NEAREST);
  return true;
}

bool ScreenView::present(const uint32_t *pixels) {
  if (!renderer_ || !texture_)
    return false;
  const int pitch = kScreenW * static_cast<int>(sizeof(uint32_t));
  if (!SDL_UpdateTexture(texture_, nullptr, pixels, pitch))
    return false;
  SDL_RenderClear(renderer_);
  if (!SDL_RenderTexture(renderer_, texture_, nullptr, nullptr))
    return false;
  return SDL_RenderPresent(renderer_);
}

void ScreenView::shutdown(){
  if (texture_) {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
  }
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }
  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
}

} // namespace bitforge::display
