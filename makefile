src := src/main.cpp src/cpu.cpp

all: chip8

chip8: $(src)
	g++ -std=c++17 -Wall -Wextra $(src) -o emu

clean: emu
	rm emu