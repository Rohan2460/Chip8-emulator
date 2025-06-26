#include "cpu.h"
#include "gui.h"

int main()
{
    CPU cpu;
    cpu.reset();
    // cpu.loadROM("/files/Projects/chip8/roms/test_opcode.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/3-corax+.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/6-keypad.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/8-scrolling.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/4-flags.ch8");
    // cpu.loadROM("/files/Projects/chip8/roms/5-quirks.ch8");
    cpu.printReg();
    uint16_t* video = cpu.getVideo();

    GUI gui;
    gui.init("Chip8", video);
    cpu.setKeys(gui.getKeys());
    bool quit {false};
    Uint32 frameStart = SDL_GetTicks();

    while (!quit)
    {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 dt = currentTime - frameStart;

        if (dt >= (1000 / 500)) 
        {
            frameStart = currentTime;
            quit = gui.events();

            cpu.cycle();
            gui.update(&cpu.videoUpdated);
        }
    }

    gui.quit();
    return 0;
}
