#pragma once
#include <cstdint>

using byte_t = uint8_t; 

inline constexpr uint16_t ROM_START_ADDR { 0X200 };
inline constexpr uint16_t FONT_START_ADDR { 0 }; 
inline constexpr int FONTSET_SIZE { 80 }; 

inline const byte_t fontset[FONTSET_SIZE] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };


class CPU
{

    uint16_t video[64 * 32];
    byte_t memory[4096];
    uint16_t stack[16];

    struct Registers
    {
        byte_t   VX[16]; // V0 - VF
        byte_t   SP; 
        uint16_t PC; 
        uint16_t I; // address register
        byte_t 	 DT; // delay
        byte_t 	 ST; // sound

    } reg;
    
    void dispatch(uint16_t& opcode);
    void push(uint16_t item);
    void loadFonts();
    bool* keys;

public:
    CPU();
    // void init(char const* rom_path, bool* keys);
    void reset();
    void printReg();
    void cycle();
    void loadROM(char const* filename);
    void setKeys(bool* keys);
    uint16_t* getVideo();
    static bool videoUpdated;
};
