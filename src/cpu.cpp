#include "cpu.h"
#include <cstring>
#include <iostream>
#include <random>

CPU::CPU()
{
	reset();
}

void CPU::reset()
{
	reg.SP = 0;
	reg.PC = 0x200;
	reg.I = 0;

	for(int i {0}; i < 16; i++)
	{
		reg.VX[i] = 0;
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

	for(int i {0}; i < 16; i++)
	{
		cout << "V" << i << ": " << std::hex << +reg.VX[i] << " | ";
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
	int index = static_cast<int>((opcode >> 8) & 0x000F);
	int x = index;
	int y = static_cast<int>((opcode >> 4) & 0x000F);

	// msb nibble
	switch (static_cast<byte_t>(opcode >> 12))
	{

	case 0:
	{
		switch (opcode & 0x000F)
		{
		// CLS
		case 0:
			memset(video, 0, sizeof(video));
			break;

		case 0xE:
			reg.PC = stack[reg.SP];
			reg.SP--;
			break;
		
		default:
			break;
		}
		break;
	}
	// jump
	case 1:
		reg.PC = opcode & 0x0FFF;
		break;
	
	// call
	case 2:
		reg.SP++;
		push(reg.PC);
		reg.PC = opcode & 0x0FFF;
		break;

	case 3:
		if (reg.VX[index] == opcode & 0x00FF)
			reg.PC += 2;
		break;

	case 4:
		if (reg.VX[index] != opcode & 0x00FF)
			reg.PC += 2;
		break;

	case 5:
		if (reg.VX[x] == reg.VX[y])
			reg.PC += 2;
		break;

	case 6:
		reg.VX[index] = opcode & 0x00FF;
		break;
	
	// add
	case 7:
		reg.VX[index] += opcode & 0x00FF;
		break;

	case 8:
	{
		// lsb nibble
		switch (opcode & 0x000F)
		{
		// or
		case 1:
			reg.VX[x] |= reg.VX[y];
			break;
		
		// and
		case 2:
			reg.VX[x] &= reg.VX[y];
			break;

		// xor
		case 3:
			reg.VX[x] ^= reg.VX[y];
			break;

		// add 
		case 4:
			uint16_t res = reg.VX[x] + reg.VX[y];
			reg.VX[x] = res;
			if (res > 0xFF)
				reg.VX[0xF] = 1;
			else
				reg.VX[0xF] = 0;
			break;

		// sub
		case 5:
			uint16_t res = reg.VX[x] - reg.VX[y];
			if (reg.VX[x] > reg.VX[y])
				reg.VX[0xF] = 1;
			else
				reg.VX[0xF] = 0;

			reg.VX[x] = res;
			break;

		// shr
		case 6:
			if (reg.VX[x] & 0x01) 	// lsb
				reg.VX[0xF] = 1;
			else
				reg.VX[0xF] = 0;

			reg.VX[x] = reg.VX[x] >> 1;
			break;
		
		// subn
		case 7:
			uint16_t res = reg.VX[y] - reg.VX[x];
			if (reg.VX[x] < reg.VX[y])
				reg.VX[0xF] = 1;
			else
				reg.VX[0xF] = 0;

			reg.VX[x] = res;
			break;

		// shl
		case 0xE:
			if (reg.VX[x] & 0x80) 	// msb, mask 0x80 = 0b10000000
				reg.VX[0xF] = 1;
			else
				reg.VX[0xF] = 0;

			reg.VX[x] = reg.VX[x] << 1;
			break;

			
		default:
			break;
		}
		break;
	}
	// 9xy0
	case 9:
		if(reg.VX[x] != reg.VX[y])
			reg.PC += 2;
		break;
	
	// load
	case 0xA:
		reg.I = opcode & 0x0FFF;
		break;
		
	// jump
	case 0xB:
		reg.PC = (opcode & 0x0FFF) + reg.VX[0];
		break;
	
	// RND
	case 0xC:
		std::mt19937 mt{};
		std::uniform_int_distribution randRange{ 0, 255 };
		reg.VX[index] = static_cast<byte_t>(randRange(mt) & (opcode & 0x00FF));
		break;

	// Display Dxyn
	case 0xD: 
	{
		int spriteLength = opcode & 0X000F;
		byte_t sprite[spriteLength];
		byte_t addr = reg.I;
		reg.VX[0xF] = 0;

		// load sprite
		for (int i = 0; i < spriteLength; i++)
		{
			sprite[i] = memory[addr];
			addr += 8;
		}

		for (int i = reg.VX[x]; i < reg.VX[x] + 8; i++)
		{
			int index = 0;
			for (int j = reg.VX[y]; j < reg.VX[y] + spriteLength; j++)
			{
				if (video[j % 32][i % 64] == 1 and sprite[index + (8 * i)])
					reg.VX[0xF] = 1;
				
				video[j % 32][i % 64] ^= sprite[j + (8 * i)];
				index++;
			}
		}
		
		break;
	}
		
	default:
		break;
	}
}


void CPU::push(uint16_t item)
{
	stack[reg.SP] = item;
	// reg.SP++;
}

