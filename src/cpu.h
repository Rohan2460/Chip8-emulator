#pragma once
#include <cstdint>

class CPU
{
	using byte_t = uint8_t; 
	
	byte_t   memory[4096];
	uint16_t stack[16];

	struct Registers
	{
		byte_t   VX[16]; // V0 - VF
		byte_t   SP; 
		uint16_t PC; 
		uint16_t I; // address register
	} reg;

public:
	static bool video[64][32];

	CPU();
	void reset();
	void printReg();
	void cycle();
	void dispatch(uint16_t& opcode);
	void CPU::push(uint16_t item);
};
