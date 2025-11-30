#include "chip8.h"
#include "chip8renderer.h"
#include <iostream>
#include <chrono>
#include <thread>

static Chip8 chip8;
static Chip8Renderer renderer;

bool quit = false;
SDL_Event event;

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

int setup(int argc, char* argv[]){
    std::cout << "Chip-8 Emulator setup" << std::endl;

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
        return 1;
    }

    chip8.loadRom(argv[1]);

    int initResult = renderer.initialize();

    if (initResult != 0) {
        std::cerr << "Failed to initialize renderer." << std::endl;
        return initResult;
    }
    
    return 0;
}

int runEmulation(){
    while(!quit){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
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

int main(int argc, char* argv[]) {

    int setupResult = setup(argc, argv);
    if (setupResult != 0) {
        std::cout << "Setup failed with error code: " << setupResult << std::endl;
        return setupResult;
    }

    runEmulation();

    return 0;
}