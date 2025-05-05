#include "chip_8.h"
#include <cstdint>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>

Chip8::Chip8(std::filesystem::path romPath)
{
    programCounter = 0x200;
    memset(memory, 0, (size_t)4 * 1024);
    memset(display, 0, (size_t)32 * 64);
    memset(registers, 0, 16);
    for (int i = 0x50; i < 0x50 + (16 * 5); i++)
    {
        memory[i] = font_data[i];
    }
    if (!romPath.empty())
    {

        int64_t romSize = (int64_t)std::filesystem::file_size(romPath);
        if (romSize > (MEMORY_SIZE - 0x200))
        {
            std::cerr << std::format("Rom '{}' size is too large",
                                     romPath.generic_string())
                      << std::endl;
        }
        try
        {
            std::ifstream rom(romPath, std::ios::binary);

            if (rom.is_open())
            {
                rom.read((char *)&(memory[0x200]), romSize);
            }
        }
        catch (std::exception &e)
        {
            std::cerr << "Failed to open rom. Error: " << e.what() << std::endl;
        }
    }
}
Chip8::~Chip8() {}

void Chip8::Tick()
{
    Fetch();
    DecodeAndExecute();
}
void Chip8::Fetch()
{
    instruction = memory[programCounter];
    instruction <<= 8;
    instruction |= memory[programCounter + 1];

    programCounter += 2;
}
void Chip8::DecodeAndExecute()
{
    uint8_t instructNumber = (0xf000 & instruction) >> 12;

    switch (instructNumber)
    {
    case 0x0:
    {
        I_00E0();
    }
    break;
    case 0x1:
    {
        I_1NNN();
    }
    break;
    case 0x6:
    {
        I_6XNN();
    }
    break;
    case 0x7:
    {
        I_7XNN();
    }
    break;
    case 0xA:
    {
        I_ANNN();
    }
    break;
    case 0xD:
    {
        I_DXYN();
    }
    break;
    }
}

void Chip8::I_00E0()
{
    memset(display, 0, size_t(DISPLAY_WIDTH * DISPLAY_HEIGHT));
}
void Chip8::I_1NNN()
{
    uint16_t nnn = 0x0fff & instruction;
    programCounter = nnn;
}
void Chip8::I_6XNN()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t nn = 0x00ff & instruction;
    registers[x] = nn;
}
void Chip8::I_7XNN()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t nn = 0x00ff & instruction;
    registers[x] += nn;
}
void Chip8::I_ANNN()
{
    uint16_t nnn = 0x0fff & instruction;
    indexRegister = nnn;
}
void Chip8::I_DXYN()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint16_t xPos = registers[x] % 64;
    uint8_t y = (0x00f0 & instruction) >> 4;
    uint16_t yPos = registers[y] % 32;
    uint8_t n = 0x000f & instruction;

    registers[0xf] = 0;

    for (uint8_t row = 0; row < n; row++)
    {
        uint8_t spriteByte = memory[indexRegister + row];
        uint8_t currentY = (yPos + row) % DISPLAY_HEIGHT;

        for (int bit = 0; bit < 8; bit++)
        { // Draw 8 pixels per row
            uint8_t currentX = (xPos + bit) % DISPLAY_WIDTH;
            uint8_t *pixel = &display[currentY * DISPLAY_WIDTH + currentX];

            uint8_t spriteBit = (spriteByte >> (7 - bit)) & 0x01;

            if (*pixel && spriteBit)
            {
                registers[0xF] = 1;
            }

            *pixel ^= spriteBit;
            *pixel = *pixel ? 255 : 0;
        }
    }
}
