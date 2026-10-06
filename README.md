# BitForge-16

A 16-bit Hack computer emulator written in C++, built bottom-up from NAND gates
following the **Nand2Tetris (Part 1: Hardware)** track. Every chip in the
machine — gates, ALU, registers, RAM hierarchy, CPU, and memory-mapped
screen/keyboard — is modelled in code, and the machine runs real Hack programs
with a live graphical display.

## System Overview

```
                        +------------------ Hack Computer ------------------+
                        |                                                   |
  .hack program ------> |  ROM32K (instruction memory, 32K words)           |
                        |      |                                            |
                        |      v                                            |
                        |     CPU                                           |
                        |   +--[A/D registers]--[ALU]--[PC]--+               |
                        |      |                             |               |
                        |      +---- Memory (data, 0-32767) <-+               |
                        |              |         |          |                  |
                        |         RAM 16K    Screen 8K   Keyboard 1 word       |
                        |         0-16383    16384-24575    24576             |
                        +---------------------------------------------------+
                                   |                    |
                        SDL3 window (512x256)    keyboard events
```

The CPU fetches one instruction per cycle from ROM, executes it through the
ALU/registers, and reaches the outside world only through memory-mapped I/O:
writing to screen addresses paints pixels, reading address 24576 gives the
pressed key.

## Hardware Approach vs Software Approach

The guiding rule: **hardware semantics where behavior matters, plain software
where it is only plumbing.**

**Modelled hardware-style (gate/register truth):**
- Gates and multiplexers — the atoms everything else is built from.
- Adders and ALU — real carry chains and `zr`/`ng` flag logic, so overflow and
  jump conditions behave like silicon.
- Registers, RAM hierarchy, Screen storage — `Register`/`RAM8`/`RAM64`/`RAM512`/
  `RAM4K`/`RAM16K` mirror the HDL chip tree with DMux load-routing and Mux
  output selection (selected-path evaluation keeps it fast without changing
  observable behavior).
- CPU datapath — A/D registers, ALU wiring (`x=D`, `y=A-or-M` via the `a`-bit),
  dest decoding, PC update.

**Deliberately software (no gates to learn there):**
- Instruction decode and control flow (`A` vs `C` branch, jump conditions) —
  plain C++ branches instead of control Muxes.
- ROM32K — a flat array plus `.hack` loader instead of a Mux tree; a ROM has no
  state and is filled from disk.
- Memory routing top level — range checks instead of wire select lines.
- Display, keyboard, assembler, file I/O — host-side concerns by nature.

## Circuits Used and Why

- **Mux / DMux (all widths)** — the universal selector: register selection,
  ALU operation choice, device routing, dest dispatch.
- **HalfAdder / FullAdder / 16-bit adder-subtractor** — arithmetic core of the
  ALU and the PC incrementer; ripple-carry matches the course design.
- **DFF → Bit → Register → RAM8 → … → RAM16K** — the storage ladder: one flip-
  flop per bit, eight registers per RAM8, eight-fold fan-out per level.
- **ALU (zx, nx, zy, ny, f, no + zr/ng)** — all Hack computations and the
  comparisons that drive jumps.
- **PC (load/inc/reset priority)** — sequential execution, jumps (`PC=A`) and
  reset.
- **Screen 8K = 2 x RAM4K** — contiguous split (low/high 4K halves) so the
  framebuffer stays linear for the display; plus an O(1) flat shadow so the
  60 FPS refresh never walks the gate hierarchy.
- **Keyboard = 1 Register** — a single word suffices because the machine
  reports exactly one pressed key; host-set, read-only from the CPU.

## Memory Map

```
0 - 16383      RAM 16K   (16384 words, general purpose)
16384 - 24575  Screen    (8192 words, 512x256 pixels, 16 px/word)
24576          Keyboard  (1 word, key code or 0)
24577 - 32767  Invalid   (reads 0, writes ignored)
```

Pixel formula: `address = 16384 + row*32 + col/16`, `bit = col % 16`
(bit 1 = black, bit 0 = white). Devices decode from address bits 14/13, exactly
as the HDL `DMux/Mux` select lines do.

## Display and Input (SDL3)

- Streaming `ARGB8888` texture (512x256) scaled with nearest-neighbour to a
  1536x768 window: each Hack pixel renders as a crisp 3x3 block with zero
  per-pixel draw calls (one buffer upload per frame).
- Full framebuffer refresh at ~60 FPS; CPU runs at full speed between frames.
- Keyboard: `SDL_PollEvent` → Hack codes (ASCII direct, `a-z` normalized to
  `A-Z`, Enter 128, Backspace 129, arrows 130-133, Esc 140, F1-F12 141-152)
  via a remappable table; press writes address 24576, release writes 0
  (last-press-wins for multiple keys); window title shows the live key code.

## Toolchain: Assembler + Simulator

```
temp.asm --[assembler]--> program.hack --[ROM32K loader]--> simulator window
```

- **Assembler**: translates Hack assembly (`.asm`: `@symbols`, `D=A`, jumps)
  into 16-bit `.hack` binaries (A-instructions `0 + value`, C-instructions
  `111 + comp + dest + jump`).
- **Simulator** (`apps/simulator/main.cpp`): loads a `.hack` file into ROM32K,
  steps the CPU, and renders `Screen` + `Keyboard` live in the SDL3 window.

## Build, Run, Test

Requires CMake 3.28+ and a C++17 compiler. SDL3 and Catch2 are fetched
automatically.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
ctest --test-dir build          # all hardware + display/keymap suites
./build/bitforge_simulator      # GUI: live screen + keyboard
```

Bit convention used throughout: LSB-first (`bit[0]` = value 1), enforced by
every test helper so gates, RAM, CPU and ROM all agree.

## Tech Stack

C++17, CMake, SDL3 (display/input), Catch2 (tests). No external HDL tooling:
the hardware lives in ordinary, debuggable code.
