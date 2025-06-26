#include "cpu.h"
#include "gui.h"
#include <cstring>
#include <iostream>
#include <random>
#include <fstream>

bool CPU::videoUpdated{true};

void CPU::init(bool* keys, bool debug = false)
{
    reset();
    loadFonts();
    setKeys(keys);

    if (debug)
        memset(video, 0xFFFF, sizeof(video)); // White screen
}

void CPU::reset()
{
    reg.SP = 0;
    reg.PC = 0x200;
    reg.I = 0;

    memset(stack, 0, sizeof(stack));
    memset(reg.VX, 0, sizeof(reg.VX));
    memset(video, 0, sizeof(video));

    // Debug
    // memory[reg.PC] = 0xF0;
    // memory[reg.PC + 1] = 0x0A;
}

void CPU::loadFonts()
{
    for (int i = 0; i < FONTSET_SIZE; i++)
    {
        memory[FONT_START_ADDR + i] = fontset[i];
    }
}

void CPU::loadROM(char const *filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::streampos size = file.tellg();
        char *buffer = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i < size; ++i)
        {
            memory[0x200 + i] = buffer[i];
        }

        delete[] buffer;
    }
}

void CPU::setKeys(bool *keyPtr)
{
    keys = keyPtr;
}

void CPU::handleTimers()
{
    if (reg.DT > 0)
        reg.DT--;
    if (reg.ST > 0)
        reg.ST--;
}

void CPU::printReg()
{
    using std::cout;
    cout << "SP: " << std::hex << +reg.SP << "\n";
    cout << "PC: " << std::hex << +reg.PC << "\n";
    cout << "I: " << std::hex << +reg.I << "\n";

    for (int i{0}; i < 16; i++)
    {
        cout << "V" << i << ": " << std::hex << +reg.VX[i] << " | ";
    }
    cout << "\n";
}

void CPU::cycle()
{
    uint16_t opcode = (memory[reg.PC] << 8) | memory[reg.PC + 1];
    reg.PC += 2;
    dispatch(opcode);
    handleTimers();
}

// Instructions

void CPU::dispatch(uint16_t &opcode)
{
    const int index = static_cast<int>((opcode >> 8) & 0x000F);
    const int &x = index;
    const int y = static_cast<int>((opcode >> 4) & 0x000F);

    // msb nibble
    switch (static_cast<byte_t>(opcode >> 12))
    {

    case 0:
    {
        switch (opcode & 0x00FF)
        {
        // CLS
        case 0xE0:
            memset(video, 0, sizeof(video));
            videoUpdated = true;
            break;

        // RET
        case 0xEE:
            reg.PC = stack[reg.SP];
            reg.SP--;
            break;

        default:
            invalidOpcode(opcode);
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
        stack[reg.SP] = reg.PC;
        reg.PC = opcode & 0x0FFF;
        break;

    case 3:
        if (reg.VX[index] == (opcode & 0x00FF))
            reg.PC += 2;
        break;

    case 4:
        if (reg.VX[index] != (opcode & 0x00FF))
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
        const byte_t vx = reg.VX[x];
        const byte_t vy = reg.VX[y];

        // lsb nibble
        switch (opcode & 0x000F)
        {
        // load
        case 0:
            reg.VX[x] = reg.VX[y];
            break;

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
        {
            uint16_t res = reg.VX[x] + reg.VX[y];
            reg.VX[x] = res;

            if (res > 0xFF)
                reg.VX[0xF] = 1;
            else
                reg.VX[0xF] = 0;
            break;
        }

        // sub
        case 5:
        {
            reg.VX[x] = vx - vy;
            
            if (vx >= vy)
                reg.VX[0xF] = 1;
            else
                reg.VX[0xF] = 0;
            break;
        }

        // shr
        case 6:
            reg.VX[x] = vx >> 1;

            if (vx & 0x01) // lsb
                reg.VX[0xF] = 1;
            else
                reg.VX[0xF] = 0;
            break;

        // subn
        case 7:
        {
            reg.VX[x] = vy - vx;

            if (vx <= vy)
                reg.VX[0xF] = 1;
            else
                reg.VX[0xF] = 0;
            break;
        }

        // shl
        case 0xE:
            reg.VX[x] = vx << 1;

            if (vx & 0x80) // msb, mask 0x80 = 0b10000000
                reg.VX[0xF] = 1;
            else
                reg.VX[0xF] = 0;
            break;

        default:
            invalidOpcode(opcode);
            break;
        }
        break;
    }
    
    // 9xy0
    case 9:
        if (reg.VX[x] != reg.VX[y])
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
    {
        byte_t rnd = randGen();
        reg.VX[index] = rnd & (opcode & 0x00FF);
        break;
    }

    // Display Dxyn
    case 0xD:
    {
        videoUpdated = true;
        int spriteHeight = opcode & 0X000F;
        byte_t sprite[spriteHeight];
        uint16_t addr = reg.I;
        reg.VX[0xF] = 0;

        // load sprite
        for (int i = 0; i < spriteHeight; i++)
        {
            sprite[i] = memory[addr];
            addr += 1;
        }

        for (int i = 0; i < spriteHeight; i++)
        {
            byte_t spriteRow = sprite[i];
            int col = reg.VX[x];
            int row = reg.VX[y];

            for (int j = 0; j < 8; j++)
            {
                // multiply 64 by row index and add column index
                byte_t spritePixel = spriteRow & (0x80 >> j);
                uint16_t *screenPixel = &video[(64 * ((row + i) % 32)) + ((col + j) % 64)];

                if (spritePixel)
                {
                    // collision
                    if (*screenPixel == 0xFFFF)
                        reg.VX[0xF] = 1;

                    *screenPixel ^= 0xFFFF;
                }
            }
        }
        break;
    }

    case 0xE:
    {
        switch (opcode & 0x00FF)
        {
        case 0x9E:
        {
            if (keys[reg.VX[index]])
            {
                reg.PC += 2;
                // resetKey = reg.VX[index];
            }
            break;
        }

        case 0xA1:
        {
            if (!keys[reg.VX[index]])
            {
                reg.PC += 2;
            }
            break;
        }

        default:
            invalidOpcode(opcode);
            break;
        }
        break;
    }

    case 0xF:
    {
        switch (opcode & 0x00FF)
        {
        case 0x07:
            reg.VX[index] = reg.DT;
            break;

        case 0x0A:
        {
            byte_t key = waitAndGetKey();
            reg.VX[index] = key;
            break;
        }

        case 0x15:
            reg.DT = reg.VX[index];
            break;

        case 0x18:
            reg.ST = reg.VX[index];
            break;

        case 0x1E:
            reg.I += reg.VX[index];
            break;

        case 0x29:
            reg.I = (reg.VX[index] * 5) + FONT_START_ADDR;
            break;

        case 0x33:
        {
            byte_t val = static_cast<byte_t>(reg.VX[index]);
            memory[reg.I + 2] = val % 10;
            val /= 10;

            memory[reg.I + 1] = val % 10;
            val /= 10;

            memory[reg.I] = val % 10;
            val /= 10;
            break;
        }

        case 0x55:
        {
            for (int i = 0; i <= index; i++)
            {
                memory[reg.I + i] = reg.VX[i];
            }
            break;
        }

        case 0x65:
        {
            for (int i = 0; i <= index; i++)
            {
                reg.VX[i] = memory[reg.I + i];
            }
            break;
        }

        default:
            invalidOpcode(opcode);
            break;
        }
        break;
    }

    default:
        invalidOpcode(opcode);
        break;
    }
}

void CPU::invalidOpcode(uint16_t& opcode)
{
    std::cout << "Invalid opcode: " << std::hex << opcode << "\n";
}

uint16_t *CPU::getVideo()
{
    return video;
}

byte_t randGen()
{
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 255);

    return distr(gen);
}
