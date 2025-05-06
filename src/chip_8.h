#pragma once
#include <QObject>
#include <cstdint>
#include <filesystem>
#include <qtmetamacros.h>
#include <stack>
#include <stdint.h>
#include <unordered_map>
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

class Chip8 : public QObject
{
    Q_OBJECT
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
    void I_0NNN();
    void I_00EE();
    void I_2NNN();
    void I_3XNN();
    void I_4XNN();
    void I_5XY0();
    void I_9XY0();
    void I_8XY0();
    void I_8XY1();
    void I_8XY2();
    void I_8XY3();
    void I_8XY4();
    void I_8XY5();
    void I_8XY6();
    void I_8XYE();
    void I_8XY7();
    void I_BNNN();
    void I_CXNN();
    void I_EX9E();
    void I_EXA1();
    void I_FX07();
    void I_FX15();
    void I_FX18();
    void I_FX1E();
    void I_FX0A();
    void I_FX29();
    void I_FX33();
    void I_FX55();
    void I_FX65();

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
    bool refreshDisplay = false;
    int currentKey = -1;

    std::unordered_map<int, int8_t> qKeyToChip8KeyMap;

    uint16_t programCounter = 0;
    uint16_t indexRegister = 0;

    uint16_t instruction = 0;

    std::stack<uint16_t> stack;

    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

  signals:
    void RefreshDisplay();
  public slots:
    void IncrementTimers();
};
