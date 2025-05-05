#pragma once
#include <filesystem>
#include <stack>
#include <stdint.h>
/*
  Address space:
  interpreter:
  0x000 t0 0x1FF reserved. font date is here
  game:
  0x200 -> onwards


*/

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define MEMORY_SIZE 1024 * 4
#define REGISTER_COUNT 16

class Chip8
{
  public:
    Chip8(std::filesystem::path romPath = "");
    ~Chip8();

    void Tick();

    void Fetch();
    void DecodeAndExecute();

    void I_00E0();
    void I_1NNN();
    void I_6XNN();
    void I_7XNN();
    void I_ANNN();
    void I_DXYN();

    uint8_t font_data[16 * 5] = {0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
                                 0x20, 0x60, 0x20, 0x20, 0x70,  // 1
                                 0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
                                 0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
                                 0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
                                 0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
                                 0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
                                 0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
                                 0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
                                 0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
                                 0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
                                 0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
                                 0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
                                 0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
                                 0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
                                 0xF0, 0x80, 0xF0, 0x80, 0x80}; // F

    uint8_t memory[MEMORY_SIZE];
    uint8_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    uint8_t registers[REGISTER_COUNT];
    uint16_t programCounter = 0;
    uint16_t indexRegister = 0;

    uint16_t instruction = 0;

    std::stack<uint16_t> stack;

    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;
};
