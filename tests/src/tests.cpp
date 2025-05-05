#include "chip_8.h"
#include <cstdint>
#include <gtest/gtest.h>
TEST(Chip8, Fetch)
{
    Chip8 chip8;

    chip8.memory[chip8.programCounter] = 0xbb;
    chip8.memory[chip8.programCounter + 1] = 0xaa;
    chip8.Fetch();
    ASSERT_EQ(chip8.instruction, 0xbbaa);
    ASSERT_EQ(chip8.programCounter, 0x202);
}

TEST(Chip8, I_00E0)
{
    Chip8 chip8;
    chip8.Tick();
    for (int i = 0; i < 32 * 64; i++)
    {
        ASSERT_EQ(chip8.display[i], 0);
    }
}
TEST(Chip8, I_1NNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x1A;
    chip8.memory[chip8.programCounter + 1] = 0xBC;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0x0ABC);
}
TEST(Chip8, I_6XNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x6A;
    chip8.memory[chip8.programCounter + 1] = 0xBC;
    chip8.Tick();

    ASSERT_EQ(chip8.registers[0xA], 0xBC);
}
TEST(Chip8, I_7XNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x7A;
    chip8.memory[chip8.programCounter + 1] = 0xBC;
    uint16_t prevX = chip8.registers[0xA];
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 0xBC + prevX);
}
TEST(Chip8, I_ANNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0xAA;
    chip8.memory[chip8.programCounter + 1] = 0xBC;
    chip8.Tick();
    ASSERT_EQ(chip8.indexRegister, 0xABC);
}

TEST(Chip8, I_00EE)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x00;
    chip8.memory[chip8.programCounter + 1] = 0xEE;
    chip8.stack.push(0xabcd);
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0xabcd);
}
TEST(Chip8, I_2NNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x2a;
    chip8.memory[chip8.programCounter + 1] = 0xbc;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0x0abc);
    ASSERT_EQ(chip8.stack.top(), 0x0202);
}
TEST(Chip8, I_3XNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x3A;
    chip8.registers[0xA] = 0xBC;
    chip8.memory[chip8.programCounter + 1] = 0xBC;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0x0204);
}
TEST(Chip8, I_4XNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x4A;
    chip8.registers[0xA] = 0xBC;
    chip8.memory[chip8.programCounter + 1] = 0xBC;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0x0202);
}
TEST(Chip8, I_5XY0)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x5B;
    chip8.memory[chip8.programCounter + 1] = 0xC0;
    chip8.registers[0xB] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0x0202);
}
TEST(Chip8, I_9XY0)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x9B;
    chip8.memory[chip8.programCounter + 1] = 0xC0;
    chip8.registers[0xB] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0x0204);
}
TEST(Chip8, I_8XY0)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC0;
    chip8.registers[0xA] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 12);
}
TEST(Chip8, I_8XY1)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC1;
    chip8.registers[0xA] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 12 | 10);
}
TEST(Chip8, I_8XY2)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC2;
    chip8.registers[0xA] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 12 & 10);
}
TEST(Chip8, I_8XY3)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC3;
    chip8.registers[0xA] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 12 ^ 10);
}
TEST(Chip8, I_8XY4)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC4;
    chip8.registers[0xA] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 12 + 10);
}
TEST(Chip8, I_8XY5)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC5;
    chip8.registers[0xA] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    uint8_t v = 10 - 12;
    ASSERT_EQ(chip8.registers[0xA], v);
    ASSERT_EQ(chip8.registers[0xf], 0);
}
TEST(Chip8, I_8XY7)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC7;
    chip8.registers[0xA] = 10;
    chip8.registers[0xC] = 12;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 12u - 10u);
    ASSERT_EQ(chip8.registers[0xf], 1);
}
TEST(Chip8, I_8XY6)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xC6;
    chip8.registers[0xA] = 0b00000010;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 0b00000001);
    ASSERT_EQ(chip8.registers[0xF], 0);
}
TEST(Chip8, I_8XYE)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0x8A;
    chip8.memory[chip8.programCounter + 1] = 0xCE;
    chip8.registers[0xA] = 0b00000011;
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 0b00000110);
    ASSERT_EQ(chip8.registers[0xF], 1);
}
TEST(Chip8, I_BNNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0xBA;
    chip8.memory[chip8.programCounter + 1] = 0xBC;
    chip8.registers[0] = 10;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, chip8.registers[0] + 0xABC);
}
