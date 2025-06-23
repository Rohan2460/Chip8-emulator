# pragma once

#include "cpu.h"
#include <SDL3/SDL.h>
#include <string_view>

inline constexpr int SCREEN_WIDTH { 640 };
inline constexpr int SCREEN_HEIGHT { 320 };

inline constexpr int WINDOW_WIDTH { SCREEN_WIDTH };
inline constexpr int WINDOW_HEIGHT { SCREEN_HEIGHT };

inline constexpr int pixelWidth { WINDOW_WIDTH / 64 };
inline constexpr int pixelHeight { WINDOW_HEIGHT / 32 };

template <typename T>
const char* getHex(const char* label, T val);

class GUI
{
    static SDL_Window* window ;
    static SDL_Renderer* renderer;
    static SDL_Texture* texture;
    uint16_t* pixels;

public:
    // GUI();
    bool key[16] {false};
    int init(const char* title, uint16_t* video);
    void update(bool* videoUpdated);
    bool events();
    void quit();
};