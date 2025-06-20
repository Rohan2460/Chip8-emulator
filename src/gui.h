# pragma once

#include <SDL3/SDL.h>
#include <string_view>

class GUI
{
    static SDL_Window* window ;
    static SDL_Renderer* renderer;
    static SDL_Texture* texture;
    static uint32_t pixels[64 * 32];

    static constexpr int WIDTH {640};
    static constexpr int HEIGHT {320};

    static constexpr int pixelWidth {WIDTH / 64};
    static constexpr int pixelHeight {HEIGHT / 32}; 

public:
    // GUI();
    int init(const char* title);
    void update(bool* video);
    bool events();
    void quit();
};