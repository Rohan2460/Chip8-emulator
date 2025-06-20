#pragma once
#include <cstdint>

inline constexpr uint16_t ROM_START_ADDR { 0X200 };
inline constexpr uint16_t FONT_START_ADDR { 0X200 }; 


class CPU
{
	using byte_t = uint8_t; 

	uint16_t video[64 * 32];
	byte_t memory[4096];
	uint16_t stack[16];

	struct Registers
	{
		byte_t   VX[16]; // V0 - VF
		byte_t   SP; 
		uint16_t PC; 
		uint16_t I; // address register
	} reg;
	
	void dispatch(uint16_t& opcode);
	void push(uint16_t item);
	void loadFonts();

public:
	CPU();
	void reset();
	void printReg();
	void cycle();
	void loadROM(char const* filename);
	uint16_t* getVideo();
};
