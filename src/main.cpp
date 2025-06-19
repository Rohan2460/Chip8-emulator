#include "cpu.h"
#include "gui.h"

int main()
{
    CPU cpu;
    cpu.reset();
    cpu.cycle();
    cpu.printReg();

    GUI gui;
    gui.init("Chip8");
    bool quit {false};

    while (!quit)
    {
        quit = gui.events();
        gui.update();
    }

    gui.quit();
    return 0;
}
