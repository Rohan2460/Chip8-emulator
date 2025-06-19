#include "gui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

SDL_Window* GUI::window = nullptr;
SDL_Renderer* GUI::renderer = nullptr;


int GUI::init(const char* title)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 2;
    }

    if (!SDL_CreateWindowAndRenderer(title, WIDTH, HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return 2;
    }

    return 0;
}

void GUI::update()
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);

    for(int i = 0; i < WIDTH; i += pixelWidth)
        SDL_RenderLine(renderer, i, 0, i, HEIGHT);
        
    for(int i = 0; i < HEIGHT; i += pixelHeight)
        SDL_RenderLine(renderer, 0, i, WIDTH, i);    

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