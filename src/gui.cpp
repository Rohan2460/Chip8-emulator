#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

constexpr int WIDTH {640};
constexpr int HEIGHT {320};

constexpr int pixelWidth {WIDTH / 64};
constexpr int pixelHeight {HEIGHT / 32};

void update()
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

int init(int argc, char *argv[])
{

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 2;
    }

    if (!SDL_CreateWindowAndRenderer("chip8 emulator", WIDTH, HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return 2;
    }

    bool quit {false};
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
                break;
            }
        }

        update();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
