#include "chip_8.h"
#include <gtest/gtest.h>
TEST(Chip8, Fetch)
{
    Chip8 chip8;

    chip8.memory[chip8.programCounter] = 0xaa;
    chip8.memory[chip8.programCounter + 1] = 0xbb;
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
    chip8.memory[chip8.programCounter] = 0xBC;
    chip8.memory[chip8.programCounter + 1] = 0x1A;
    chip8.Tick();
    ASSERT_EQ(chip8.programCounter, 0x0ABC);
}
TEST(Chip8, I_6XNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0xBC;
    chip8.memory[chip8.programCounter + 1] = 0x6A;
    chip8.Tick();

    ASSERT_EQ(chip8.registers[0xA], 0xBC);
}
TEST(Chip8, I_7XNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0xBC;
    chip8.memory[chip8.programCounter + 1] = 0x7A;
    uint16_t prevX = chip8.registers[0xA];
    chip8.Tick();
    ASSERT_EQ(chip8.registers[0xA], 0xBC + prevX);
}
TEST(Chip8, I_ANNN)
{
    Chip8 chip8;
    chip8.memory[chip8.programCounter] = 0xBC;
    chip8.memory[chip8.programCounter + 1] = 0xAA;
    chip8.Tick();
    ASSERT_EQ(chip8.indexRegister, 0xABC);
}
