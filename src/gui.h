# pragma once

#include "cpu.h"
#include <SDL3/SDL.h>
#include <string_view>

inline constexpr int SCREEN_WIDTH { 640 };
inline constexpr int SCREEN_HEIGHT { 320 };

inline constexpr int WINDOW_WIDTH { SCREEN_WIDTH + 150 };
inline constexpr int WINDOW_HEIGHT { SCREEN_HEIGHT + 60};

inline constexpr int pixelWidth { WINDOW_WIDTH / 64 };
inline constexpr int pixelHeight { WINDOW_HEIGHT / 32 };

template <typename T>
const char* getString(const char* label, T val, bool asHex = true);
byte_t waitAndGetKey();

class GUI
{
    static SDL_Window* window ;
    static SDL_Renderer* renderer;
    static SDL_Texture* texture;
    uint16_t* pixels;
    const CpuData* cpuData;
    const SDL_FRect screenRenderArea = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; 

public:
    // GUI();
    static bool keys[16];
    int init(const char* title, uint16_t* video, const CpuData* data);
    void update(bool* videoUpdated);
    void renderDebugInfo();
    bool events();
    void quit();
    bool* getKeys();
};
