#include "cpu.h"
#include "gui.h"

int main()
{
    CPU cpu;
    cpu.reset();
    cpu.loadROM("/files/Projects/chip8/roms/test2");
    cpu.printReg();
    uint16_t* video = cpu.getVideo();


    GUI gui;
    gui.init("Chip8", video);
    bool quit {false};

    Uint32 frameStart = SDL_GetTicks();
    

    while (!quit)
    {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 dt = currentTime - frameStart;

        if (dt >= (1000 / 30)) 
        {
            frameStart = currentTime;
            quit = gui.events();

            cpu.cycle();
            gui.update();

        }
    }

    gui.quit();
    return 0;
}
