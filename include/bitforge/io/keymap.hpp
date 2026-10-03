#pragma once
// Flexible SDL3 -> Hack keyboard mapping.
// Hack codes: printable ASCII as-is (space=32 ...), plus:
// 128 enter, 129 backspace, 130 left, 131 up, 132 right, 133 down,
// 134 home, 135 end, 136 pgup, 137 pgdn, 138 insert, 139 delete,
// 140 esc, 141-152 F1-F12. 0 = no key.
// Change behavior by editing the table, not the event loop.
#include <SDL3/SDL.h>
#include <unordered_map>

namespace bitforge::io
{

  struct HackKeymap
  {
    std::unordered_map<SDL_Keycode, int> table;

    HackKeymap()
    {
      // Control keys
      table[SDLK_RETURN] = 128;
      table[SDLK_BACKSPACE] = 129;
      table[SDLK_LEFT] = 130;
      table[SDLK_UP] = 131;
      table[SDLK_RIGHT] = 132;
      table[SDLK_DOWN] = 133;
      table[SDLK_HOME] = 134;
      table[SDLK_END] = 135;
      table[SDLK_PAGEUP] = 136;
      table[SDLK_PAGEDOWN] = 137;
      table[SDLK_INSERT] = 138;
      table[SDLK_DELETE] = 139;
      table[SDLK_ESCAPE] = 140;
      table[SDLK_F1] = 141;
      table[SDLK_F2] = 142;
      table[SDLK_F3] = 143;
      table[SDLK_F4] = 144;
      table[SDLK_F5] = 145;
      table[SDLK_F6] = 146;
      table[SDLK_F7] = 147;
      table[SDLK_F8] = 148;
      table[SDLK_F9] = 149;
      table[SDLK_F10] = 150;
      table[SDLK_F11] = 151;
      table[SDLK_F12] = 152;
      // Printable ASCII 32-126: map directly (letters normalized below).
      for (int c = 32; c <= 126; ++c)
        table.emplace(static_cast<SDL_Keycode>(c), c);
    }

    // Custom override: changing the table changes input behavior everywhere.
    void set(SDL_Keycode sdl, int hack) { table[sdl] = hack; }
    void unbind(SDL_Keycode sdl) { table.erase(sdl); }

    // SDL gives 'a' (97) for the A key; Hack expects 65 ('A').
    // Normalize a-z to A-Z, pass everything else through the table.
    int toHack(SDL_Keycode k) const
    {
      if (k >= SDLK_A && k <= SDLK_Z)
        return static_cast<int>(k - (SDLK_A - 65));
      if (k >= 'a' && k <= 'z')
        return static_cast<int>(k - 32);
      auto it = table.find(k);
      return it == table.end() ? 0 : it->second;
    }
  };

} // namespace bitforge::io
