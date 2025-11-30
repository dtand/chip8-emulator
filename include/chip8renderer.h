#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <iostream>

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