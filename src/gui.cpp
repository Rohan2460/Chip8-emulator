#include "gui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

SDL_Window* GUI::window = nullptr;
SDL_Renderer* GUI::renderer = nullptr;
SDL_Texture* GUI::texture = nullptr;
bool GUI::keys[16] {false};


int GUI::init(const char* title, uint16_t* video, const CpuData* reg)
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
    cpuData = reg;
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
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint16_t));
    SDL_RenderTexture(renderer, texture, NULL, &screenRenderArea);
    
    renderDebugInfo();
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
            case SDLK_X:
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

            case SDLK_Q:
                keys[4] = e;
                break;

            case SDLK_W:
                keys[5] = e;
                break;

            case SDLK_E:
                keys[6] = e;
                break;

            case SDLK_A:
                keys[7] = e;
                break;

            case SDLK_S:
                keys[8] = e;
                break;
            
            case SDLK_D:
                keys[9] = e;
                break;

            case SDLK_Z:
                keys[10] = e;
                break;

            case SDLK_C:
                keys[11] = e;
                break;

            case SDLK_4:
                keys[12] = e;
                break;
            
            case SDLK_R:
                keys[13] = e;
                break;

            case SDLK_F:
                keys[14] = e;
                break;

            case SDLK_V:
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


void GUI::renderDebugInfo()
{
    const int xPos = SCREEN_WIDTH + 10;
    const int yPos = 10;

    const int stackYPos = SCREEN_HEIGHT - 10;
    const int opcodeYPos = SCREEN_WIDTH + 30;
    const CpuData* &cpu = cpuData;

    
    // pc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, xPos, yPos, getString("PC: ", cpu->reg->PC));

    // opcode
    uint16_t opcode = cpu->memory[cpu->reg->PC - 2];
    opcode = opcode << 8 | (cpu->memory[cpu->reg->PC]);
    SDL_RenderDebugText(renderer, xPos, yPos + 20, getString("Opcode: ", opcode));

    // stack
    SDL_RenderDebugText(renderer, xPos, stackYPos + 20, "Stack");
    for (int i = 0; i < 16; i++)
    {
        SDL_RenderLine(renderer, xPos, stackYPos - (i*15), xPos + 50, stackYPos - (i*15));
    }

    for (int i = 0; i < cpu->reg->SP; i++)
    {
        SDL_RenderDebugText(renderer, xPos, (stackYPos - (i * 15) - 10), getString(" ", cpu->stack[i]));
    }

    // vx

    for (int i = 0; i < 16; i++)
    {
        SDL_RenderDebugText(renderer, 10 + (i * 40), SCREEN_HEIGHT + 20, getString(" ", cpu->reg->VX[i]));
        SDL_RenderDebugText(renderer, 10 + (i * 40), SCREEN_HEIGHT + 30, getString(" V", i, false));
    }

}

template <typename T>
const char* getString(const char* label, T val, bool asHex)
{
    static char buffer[32];
    if (asHex)
        std::snprintf(buffer, sizeof(buffer), "%s0x%X", label, val);
    else
        std::snprintf(buffer, sizeof(buffer), "%s%d", label, val);

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
        else if (event.type == SDL_EVENT_QUIT)
        {
            SDL_PushEvent(&event);
            return 0;
        }
    }
}
