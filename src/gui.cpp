#include "gui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

SDL_Window* GUI::window = nullptr;
SDL_Renderer* GUI::renderer = nullptr;
SDL_Texture* GUI::texture = nullptr;


int GUI::init(const char* title, uint16_t* video)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 2;
    }

    if (!SDL_CreateWindowAndRenderer(title, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return 2;
    }

    pixels = video;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
    return 0;
}

void GUI::update(bool* videoUpdated)
{
    if (!*videoUpdated)
        return;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // pixels[2047] = 0; // debug

    SDL_FRect area = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // scale
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint16_t));
    SDL_RenderTexture(renderer, texture, NULL, &area);

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    // SDL_RenderDebugText(renderer, SCREEN_WIDTH, 10, "Test");

    SDL_RenderPresent(renderer);
}

bool GUI::events()
{
    bool quit {false};
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            quit = true;
            break;
        }
        else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
        {
            bool e = !(event.type & 0x00F); // 1 for key down and 0 for key up
            switch (event.key.key)
            {
            case SDLK_0:
                key[0] = e;
                break;
            
            case SDLK_1:
                key[1] = e;
                break;
            
            case SDLK_2:
                key[2] = e;
                break;

            case SDLK_3:
                key[3] = e;
                break;

            case SDLK_4:
                key[4] = e;
                break;

            case SDLK_5:
                key[5] = e;
                break;

            case SDLK_6:
                key[6] = e;
                break;

            case SDLK_7:
                key[7] = e;
                break;

            case SDLK_8:
                key[8] = e;
                break;
            
            case SDLK_9:
                key[9] = e;
                break;

            case SDLK_A:
                key[10] = e;
                break;

            case SDLK_B:
                key[11] = e;
                break;

            case SDLK_C:
                key[12] = e;
                break;
            
            case SDLK_D:
                key[13] = e;
                break;

            case SDLK_E:
                key[14] = e;
                break;

            case SDLK_F:
                key[15] = e;
                break;

            default:
                break;
            }
        }
    }
    return quit;
}

void GUI::quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

template <typename T>
const char* getHex(const char* label, T val)
{
    static char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%s: 0x%X", label, val);
    return buffer;
}