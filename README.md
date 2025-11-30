# CHIP-8 Emulator

A CHIP-8 emulator written in C++ using SDL3 for graphics and input. This project allows you to run classic CHIP-8 ROMs and experiment with emulator development.

## Features
- CHIP-8 instruction set emulation
- SDL3-based graphics and input
- Key mapping for CHIP-8 keypad
- Debug logging for opcode execution
- Configurable frame rate (approx 60Hz)

## Requirements
- C++17 or newer
- SDL3 (included in `external/SDL3`)
- CMake >= 3.20
- macOS (tested), should work on Linux/Windows with minor changes

## Building
1. Clone the repository and initialize submodules (if any):
   ```sh
   git clone <repo-url>
   cd chip-8-emulator
   ```
2. Build with CMake:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

## Running
Run the emulator with a CHIP-8 ROM:
```sh
./chip8 ../roms/TICTAC
```

## Key Mapping
| CHIP-8 Key | Keyboard |
|------------|----------|
| 1 2 3 C    | 1 2 3 4  |
| 4 5 6 D    | Q W E R  |
| 7 8 9 E    | A S D F  |
| A 0 B F    | Z X C V  |

## Directory Structure
- `src/` - Source code
- `include/` - Header files
- `external/SDL3/` - SDL3 library
- `roms/` - CHIP-8 ROM files
- `build/` - Build output
- `.vscode/` - VS Code configuration

## Debugging
See the README for instructions on setting up VS Code debugging with LLDB.

## License
MIT License
