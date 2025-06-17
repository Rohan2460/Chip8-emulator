#pragma once

#include <cstdint>

class CPU
{
	uint8_t	 memory[4096];
	uint16_t stack[16];

	struct Registers
	{
		uint8_t  Vx[16]; // V0 - VF
		uint8_t  SP; 
		uint16_t PC; 
		uint16_t I; // address register
	} reg;

public:
	CPU();
	void reset();
	void printReg();
	void cycle();
	void dispatch(uint16_t& opcode);
};


// uint16_t fetch(uint16_t &address, uint8_t* memory);