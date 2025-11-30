#pragma once
#include "Chip8.h"
#include "chip8renderer.h"
#include <SDL3/SDL.h>

class Emulator {

public:
    void run();
    void setup(int argc, char* argv[]);
private:
    Chip8 chip8;
    Chip8Renderer renderer;
    bool running = true;
    SDL_Event event;
};