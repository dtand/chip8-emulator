#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <iostream>


/**
 * @class Chip8Renderer
 * @brief Handles rendering for the CHIP-8 emulator using SDL3.
 *
 * Manages the SDL window, renderer, and texture for displaying the CHIP-8
 * graphics buffer. Provides methods for initialization and frame rendering.
 */
class Chip8Renderer {
public:
    Chip8Renderer() = default;
    ~Chip8Renderer();

    int initialize();
    void render(const uint8_t* videoBuffer);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};