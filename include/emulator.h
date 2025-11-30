#pragma once
#include "Chip8.h"
#include "chip8renderer.h"
#include <SDL3/SDL.h>
#include "event_logger.h"


/**
 * @class Emulator
 * @brief Main application class for the CHIP-8 emulator.
 *
 * Coordinates the CHIP-8 core, renderer, and event logging. Handles setup,
 * main emulation loop, and SDL event processing.
 */
class Emulator {

public:
    Emulator() = default;
    void run();
    void setup(int argc, char* argv[]);
private:
    Chip8 chip8;
    Chip8Renderer renderer;
    bool running = true;
    SDL_Event event;
};