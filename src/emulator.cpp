#include "emulator.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <SDL3/SDL.h>

const SDL_Scancode keymap[16] = {
    SDL_SCANCODE_X,    // 0
    SDL_SCANCODE_1,    // 1
    SDL_SCANCODE_2,    // 2
    SDL_SCANCODE_3,    // 3
    SDL_SCANCODE_Q,    // 4
    SDL_SCANCODE_W,    // 5
    SDL_SCANCODE_E,    // 6
    SDL_SCANCODE_A,    // 7
    SDL_SCANCODE_S,    // 8
    SDL_SCANCODE_D,    // 9
    SDL_SCANCODE_Z,    // A
    SDL_SCANCODE_C,    // B
    SDL_SCANCODE_4,    // C
    SDL_SCANCODE_R,    // D
    SDL_SCANCODE_F,    // E
    SDL_SCANCODE_V     // F
};

const int FRAME_DELAY_US = 16667; // Approx 60Hz

/**
 * @brief Sets up the CHIP-8 emulator environment.
 *
 * Initializes the emulator, loads the ROM, and sets up the renderer.
 * Exits the program if initialization fails or arguments are invalid.
 *
 * @param argc Argument count from main.
 * @param argv Argument vector from main.
 */
void Emulator::setup(int argc, char* argv[]) {
    std::cout << "Chip-8 Emulator setup" << std::endl;

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
        exit(1);
    }

    chip8.loadRom(argv[1]);

    if (renderer.initialize() != 0) {
        std::cerr << "Setup failed with error code: 1" << std::endl;
        exit(1);
    }
}

/**
 * @brief Runs the main emulation loop.
 *
 * Handles SDL events, processes key input, executes CHIP-8 cycles,
 * and triggers rendering when needed. Runs at approximately 60Hz.
 */
void Emulator::run() {
    while(running){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if(event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                bool pressed = (event.type == SDL_EVENT_KEY_DOWN);
                SDL_Scancode scancode = event.key.scancode;
                for (int i = 0; i < 16; ++i) {
                    if (scancode == keymap[i]) {
                        chip8.key[i] = pressed ? 1 : 0;
                    }
                }
            }
        }

        chip8.emulateCycle();

        if (chip8.drawFlag) {
            std::cout << "Rendering frame" << std::endl;
            renderer.render(chip8.gfx.data());
            chip8.drawFlag = false;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(FRAME_DELAY_US)); // Approx 60Hz
    }
}