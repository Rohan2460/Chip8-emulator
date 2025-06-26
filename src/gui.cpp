#include "gui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

SDL_Window* GUI::window = nullptr;
SDL_Renderer* GUI::renderer = nullptr;
SDL_Texture* GUI::texture = nullptr;
bool GUI::keys[16] {false};


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
    // if (!*videoUpdated)
    //     return;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint16_t));
    SDL_RenderTexture(renderer, texture, NULL, &screenRenderArea);

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, SCREEN_WIDTH + 10, 10, getHex("Test", 1515) );

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
                keys[0] = e;
                break;
            
            case SDLK_1:
                keys[1] = e;
                break;
            
            case SDLK_2:
                keys[2] = e;
                break;

            case SDLK_3:
                keys[3] = e;
                break;

            case SDLK_4:
                keys[4] = e;
                break;

            case SDLK_5:
                keys[5] = e;
                break;

            case SDLK_6:
                keys[6] = e;
                break;

            case SDLK_7:
                keys[7] = e;
                break;

            case SDLK_8:
                keys[8] = e;
                break;
            
            case SDLK_9:
                keys[9] = e;
                break;

            case SDLK_A:
                keys[10] = e;
                break;

            case SDLK_B:
                keys[11] = e;
                break;

            case SDLK_C:
                keys[12] = e;
                break;
            
            case SDLK_D:
                keys[13] = e;
                break;

            case SDLK_E:
                keys[14] = e;
                break;

            case SDLK_F:
                keys[15] = e;
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

bool* GUI::getKeys()
{
    return keys;
}

byte_t waitAndGetKey()
{
    SDL_Event event;
    SDL_Keycode key;
    byte_t result;
    bool exit { false };

    while(true)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_EVENT_KEY_DOWN)
        {
            key = event.key.key;
            if (key >= 0x61 && key <= 0x66)
                result = key - 87;
            else if (key >= 0x30 && key <= 0x39)
                result = key - 48;
        }
        else if (event.type == SDL_EVENT_KEY_UP)
        {
            return result;
        }
    }
}
