#pragma once
#include <stdint.h>

/*
  Address space:
  interpreter:
  0x000 t0 0x1FF reserved. font date is here
  game:
  0x200 -> onwards


*/

class Chip8
{
  public:
    Chip8();
    ~Chip8();

  private:
    uint8_t memory[4 * 1024];
    uint8_t display[64 * 32];

    uint16_t programCounter = 0;
    uint16_t indexRegister = 0;
};
