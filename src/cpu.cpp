#include "cpu.h"
#include <cstring>
#include <iostream>

CPU::CPU()
{
	reset();
}

void CPU::reset()
{
	reg.SP = 0;
	reg.PC = 0x200;
	reg.I = 0;

	for(int i {0}; i < 16; ++i)
	{
		reg.Vx[i] = 0;
	}

	// Debug
	memory[reg.PC] = 0x13;
	memory[reg.PC + 1] = 0x33;

}

void CPU::printReg()
{
	using std::cout;
	cout << "SP: " << std::hex << +reg.SP << "\n";
	cout << "PC: " << std::hex << +reg.PC << "\n";
	cout << "I: "  << std::hex << +reg.I  << "\n";

	for(int i {0}; i < 16; ++i)
	{
		cout << "V" << i << ": " << std::hex << +reg.Vx[i] << " | ";
	}
	cout << "\n";

}

void CPU::cycle()
{
	// fetch
	uint16_t opcode = (memory[reg.PC] << 8) | memory[reg.PC + 1];
	// decode and execute
	dispatch(opcode);
}

// Instructions

void CPU::dispatch(uint16_t& opcode)
{
	switch (static_cast<uint8_t>(opcode >> 12))
	{

	// Jump
	case 0x1:
		reg.PC = opcode & 0x0FFF;
		std::cout << "Jump called \n";
		break;
	
	default:
		break;
	}
}