#include "chip8renderer.h"

const int VIDEO_SCALE = 10;
const int VIDEO_WIDTH = 64;
const int VIDEO_HEIGHT = 32;

/**
 * @brief Initializes the SDL3 renderer, window, and texture for CHIP-8 display.
 *
 * Creates the SDL window, renderer, and texture. Returns 0 on success, 1 on failure.
 *
 * @return int 0 if successful, 1 if initialization fails.
 */
int Chip8Renderer::initialize(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("CHIP-8 Emulator",
                                VIDEO_WIDTH * VIDEO_SCALE,
                                VIDEO_HEIGHT * VIDEO_SCALE,
                                0);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                VIDEO_WIDTH,
                                VIDEO_HEIGHT);

    if (!texture) {
        std::cerr << "Failed to create SDL texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
}

/**
 * @brief Renders the CHIP-8 video buffer to the SDL window.
 *
 * Converts the CHIP-8 video buffer to pixel data and updates the SDL texture and window.
 *
 * @param videoBuffer Pointer to the CHIP-8 video buffer (size: 64x32).
 */
void Chip8Renderer::render(const uint8_t* videoBuffer){
    uint32_t pixels[VIDEO_WIDTH * VIDEO_HEIGHT];
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; ++i) {
        pixels[i] = videoBuffer[i] ? 0xFFFFFFFF : 0x000000FF; // White or Black
    }

    SDL_UpdateTexture(texture, nullptr, pixels, VIDEO_WIDTH * sizeof(uint32_t));
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

/**
 * @brief Destructor for Chip8Renderer.
 *
 * Cleans up SDL resources: texture, renderer, window, and quits SDL.
 */
Chip8Renderer::~Chip8Renderer(){
    if(texture) SDL_DestroyTexture(texture);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}