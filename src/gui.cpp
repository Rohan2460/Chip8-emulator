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

    if (!SDL_CreateWindowAndRenderer(title, WIDTH, HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return 2;
    }

    pixels = video;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
    return 0;
}

void GUI::update()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    

    

    // convert
    // for (int i = 0; i < (64 * 32); i++)
    // {
    //     if (video[i])
    //         pixels[i] = 0xFFFF;
    //     else
    //         pixels[i] = 0;
    // }

    // pixels[2047] = 0; // debug

    // SDL_FRect area = {0, 0, 320, 160}; // scale
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint16_t));
    SDL_RenderTexture(renderer, texture, NULL, NULL);

    

    // grid
    // SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
    // for(int i = 0; i < WIDTH; i += pixelWidth)
    //     SDL_RenderLine(renderer, i, 0, i, HEIGHT);
        
    // for(int i = 0; i < HEIGHT; i += pixelHeight)
    //     SDL_RenderLine(renderer, 0, i, WIDTH, i);


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, 20, 10, "Test");


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
    }

    return quit;
}

void GUI::quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
