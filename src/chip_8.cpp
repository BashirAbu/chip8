#include "chip_8.h"
#include <QFile>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <qimage.h>
#include <qnamespace.h>
#include <qtmetamacros.h>
#include <time.h>
Chip8::Chip8(std::filesystem::path romPath)
{
    srand(time(nullptr));

    programCounter = 0x200;
    memset(memory, 0, (size_t)4 * 1024);
    memset(display, 0, (size_t)32 * 64);
    memset(registers, 0, 16);
    for (int i = 0x50; i < 0x50 + (16 * 5); i++)
    {
        memory[i] = font_data[i - 0x50];
    }

    qKeyToChip8KeyMap[-1] = -1;
    qKeyToChip8KeyMap[Qt::Key_1] = 1;
    qKeyToChip8KeyMap[Qt::Key_2] = 2;
    qKeyToChip8KeyMap[Qt::Key_3] = 3;
    qKeyToChip8KeyMap[Qt::Key_4] = 0xC;
    qKeyToChip8KeyMap[Qt::Key_Q] = 4;
    qKeyToChip8KeyMap[Qt::Key_W] = 5;
    qKeyToChip8KeyMap[Qt::Key_E] = 6;
    qKeyToChip8KeyMap[Qt::Key_R] = 0xD;
    qKeyToChip8KeyMap[Qt::Key_A] = 7;
    qKeyToChip8KeyMap[Qt::Key_S] = 8;
    qKeyToChip8KeyMap[Qt::Key_D] = 9;
    qKeyToChip8KeyMap[Qt::Key_F] = 0xE;
    qKeyToChip8KeyMap[Qt::Key_Z] = 0xA;
    qKeyToChip8KeyMap[Qt::Key_X] = 0;
    qKeyToChip8KeyMap[Qt::Key_C] = 0xB;
    qKeyToChip8KeyMap[Qt::Key_V] = 0xF;

    if (!romPath.empty())
    {

        int64_t romSize = (int64_t)std::filesystem::file_size(romPath);
        if (romSize > (MEMORY_SIZE - 0x200))
        {
            std::cerr << std::format("Rom '{}' size is too large",
                                     romPath.generic_string())
                      << std::endl;
            return;
        }
        QFile romFile(romPath);
        if (!romFile.open(QIODevice::ReadOnly))
        {
            std::cerr << "Failed to open rom" << std::endl;
            return;
        }

        QByteArray data = romFile.readAll();

        romFile.close();

        memcpy(&memory[0x200], data.data(), romSize);
    }
}
Chip8::~Chip8() {}

void Chip8::Tick()
{

    Fetch();
    DecodeAndExecute();
}
void Chip8::IncrementTimers()
{
    if (delayTimer)
    {
        delayTimer--;
    }
    if (soundTimer)
    {
        soundTimer--;
    }
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
        if (instruction == 0x00EE)
        {
            I_00EE();
        }
        else if (instruction == 0x00E0)
        {
            I_00E0();
        }
    }
    break;
    case 0x1:
    {
        I_1NNN();
    }
    break;
    case 0x2:
    {
        I_2NNN();
    }
    break;
    case 0x3:
    {
        I_3XNN();
    }
    break;
    case 0x4:
    {
        I_4XNN();
    }
    break;
    case 0x5:
    {
        I_5XY0();
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
    case 0x8:
    {
        uint8_t last4Bits = (instruction & 0x000f);
        switch (last4Bits)
        {
        case 0:
            I_8XY0();
            break;
        case 1:
            I_8XY1();
            break;
        case 2:
            I_8XY2();
            break;
        case 3:
            I_8XY3();
            break;
        case 4:
            I_8XY4();
            break;
        case 5:
            I_8XY5();
            break;
        case 6:
            I_8XY6();
            break;
        case 7:
            I_8XY7();
            break;
        case 0xE:
            I_8XYE();
            break;
        }
    }
    break;
    case 0x9:
    {
        I_9XY0();
    }
    break;
    case 0xA:
    {
        I_ANNN();
    }
    break;
    case 0xB:
    {
        I_BNNN();
    }
    break;
    case 0xC:
    {
        I_CXNN();
    }
    break;
    case 0xD:
    {
        I_DXYN();
    }
    break;
    case 0xE:
    {
        uint8_t lastByte = 0x00ff & instruction;

        if (lastByte == 0x9E)
        {
            I_EX9E();
        }
        else if (lastByte == 0xA1)
        {
            I_EXA1();
        }
    }
    case 0xF:
    {
        uint8_t lastByte = 0x00ff & instruction;

        switch (lastByte)
        {
        case 0x07:
            I_FX07();
            break;
        case 0x15:
            I_FX15();
            break;
        case 0x18:
            I_FX18();
            break;
        case 0x1E:
            I_FX1E();
            break;
        case 0x0A:
            I_FX0A();
            break;
        case 0x29:
            I_FX29();
            break;
        case 0x33:
            I_FX33();
            break;
        case 0x55:
            I_FX55();
            break;
        case 0x65:
            I_FX65();
            break;
        }
    }
    default:
    {
        std::cerr << "Unkown instruction" << std::endl;
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
    uint16_t xPos = registers[x];
    uint8_t y = (0x00f0 & instruction) >> 4;
    uint16_t yPos = registers[y];
    uint8_t n = 0x000f & instruction;

    registers[0xf] = 0;

    for (uint8_t row = 0; row < n; row++)
    {
        uint8_t spriteByte = memory[indexRegister + row];
        uint8_t currentY = (yPos + row) % DISPLAY_HEIGHT;

        for (int bit = 0; bit < 8; bit++)
        {
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

    emit RefreshDisplay();
}
void Chip8::I_0NNN() {}
void Chip8::I_00EE()
{
    programCounter = stack.top();
    stack.pop();
}
void Chip8::I_2NNN()
{
    uint16_t nnn = (0x0fff & instruction);
    stack.push(programCounter);
    programCounter = nnn;
}
void Chip8::I_3XNN()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    x = registers[x];
    uint8_t nn = (0x00ff & instruction);
    if (x == nn)
    {
        programCounter += 2;
    }
}
void Chip8::I_4XNN()
{

    uint8_t x = (0x0f00 & instruction) >> 8;
    x = registers[x];
    uint8_t nn = (0x00ff & instruction);
    if (x != nn)
    {
        programCounter += 2;
    }
}
void Chip8::I_5XY0()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    x = registers[x];
    uint8_t y = (0x00f0 & instruction) >> 4;
    y = registers[y];
    if (x == y)
    {
        programCounter += 2;
    }
}
void Chip8::I_9XY0()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    x = registers[x];
    uint8_t y = (0x00f0 & instruction) >> 4;
    y = registers[y];
    if (x != y)
    {
        programCounter += 2;
    }
}
void Chip8::I_8XY0()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t y = (0x00f0 & instruction) >> 4;
    registers[x] = registers[y];
}
void Chip8::I_8XY1()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t y = (0x00f0 & instruction) >> 4;
    registers[x] |= registers[y];
}
void Chip8::I_8XY2()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t y = (0x00f0 & instruction) >> 4;
    registers[x] &= registers[y];
}
void Chip8::I_8XY3()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t y = (0x00f0 & instruction) >> 4;
    registers[x] ^= registers[y];
}
void Chip8::I_8XY4()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t y = (0x00f0 & instruction) >> 4;
    registers[x] += registers[y];
}
void Chip8::I_8XY5()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t y = (0x00f0 & instruction) >> 4;

    if (registers[x] >= registers[y])
    {
        registers[0xf] = 1;
    }
    else
    {
        registers[0xf] = 0;
    }

    registers[x] -= registers[y];
}
void Chip8::I_8XY6()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    // uint8_t y = (0x00f0 & instruction) >> 4;
    uint8_t lsb = 0x0001 & registers[x];
    registers[0xf] = lsb;
    registers[x] >>= 1;
}
void Chip8::I_8XY7()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t y = (0x00f0 & instruction) >> 4;

    if (registers[x] <= registers[y])
    {
        registers[0xf] = 1;
    }
    else
    {
        registers[0xf] = 0;
    }

    registers[x] = registers[y] - registers[x];
}
void Chip8::I_8XYE()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    // uint8_t y = (0x00f0 & instruction) >> 4;
    uint8_t lsb = 0x0001 & registers[x];
    registers[0xf] = lsb;
    registers[x] <<= 1;
}
void Chip8::I_BNNN()
{

    uint16_t nnn = (0x0fff & instruction);
    programCounter = registers[0] + nnn;
}
void Chip8::I_CXNN()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint16_t nn = (0x00ff & instruction);

    registers[x] = (rand() % 0xff) & nn;
}
void Chip8::I_EX9E()
{
    uint8_t x = (0x0f00 & instruction) >> 8;

    if ((int8_t)registers[x] == qKeyToChip8KeyMap[currentKey])
    {
        programCounter += 2;
    }
}
void Chip8::I_EXA1()
{
    uint8_t x = (0x0f00 & instruction) >> 8;

    if ((int8_t)registers[x] != qKeyToChip8KeyMap[currentKey])
    {
        programCounter += 2;
    }
}
void Chip8::I_FX07()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    registers[x] = delayTimer;
}
void Chip8::I_FX15()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    delayTimer = registers[x];
}
void Chip8::I_FX18()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    soundTimer = registers[x];
}
void Chip8::I_FX1E()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    indexRegister += registers[x];
}
void Chip8::I_FX0A()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    if (qKeyToChip8KeyMap[currentKey] != -1)
    {
        registers[x] = qKeyToChip8KeyMap[currentKey];
    }
    else
    {
        programCounter -= 2;
    }
}
void Chip8::I_FX29()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t character = registers[x];
    indexRegister = 0x50 + (character * 5);
}
void Chip8::I_FX33()
{

    uint8_t x = (0x0f00 & instruction) >> 8;
    uint8_t value = registers[x];

    uint8_t digit1 = value % 10;
    uint8_t digit2 = (value / 10) % 10;
    uint8_t digit3 = value / 100;

    memory[indexRegister] = digit1;
    memory[indexRegister + 1] = digit2;
    memory[indexRegister + 2] = digit3;
}
void Chip8::I_FX55()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    for (int i = 0; i <= x; i++)
    {
        memory[indexRegister + i] = registers[i];
    }
}
void Chip8::I_FX65()
{
    uint8_t x = (0x0f00 & instruction) >> 8;
    for (int i = 0; i <= x; i++)
    {
        registers[i] = memory[indexRegister + i];
    }
}
