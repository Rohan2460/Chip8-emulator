#include "cpu.h"
#include "gui.h"

int main()
{
    CPU cpu;
    GUI gui;
    cpu.init(gui.getKeys(), false);

    // cpu.loadROM("/files/Projects/chip8/roms/test_opcode.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/3-corax+.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/6-keypad.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/8-scrolling.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/4-flags.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/5-quirks.ch8");
    cpu.loadROM("/files/Projects/chip8/roms/spacejam.ch8");

    cpu.printReg();
    uint16_t* video = cpu.getVideo();
    const CpuData* data = cpu.getRegisters();
    gui.init("Chip8", video, data);

    bool quit {false};
    Uint32 frameStart = SDL_GetTicks();
    
    const int cyclesPerFrame = 100;
    const int targetFPS = 30;
    const int frameDelay = 1000 / targetFPS;

    while (!quit)
    {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 dt = currentTime - frameStart;

        if (dt >= frameDelay) 
        {
            frameStart = currentTime;
            quit = gui.events();

            for (int i = {0}; i < cyclesPerFrame; i++)
            {
                cpu.cycle();
            }
            gui.update(&cpu.videoUpdated);
        }
    }

    gui.quit();
    return 0;
}
