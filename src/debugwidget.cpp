#include "chip_8.h"
#include "debugwidget.h"
#include "ui_debugwidget.h"
#include <cstdint>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qtablewidget.h>
DebugWidget::DebugWidget(Chip8 *chip8, QWidget *parent)
    : QWidget(parent), ui(new Ui::DebugWidget), chip8(chip8)
{
    ui->setupUi(this);

    for (int row = 0; row < ui->registersTabel->rowCount(); row++)
    {
        for (int column = 0; column < ui->registersTabel->columnCount();
             column++)
        {
            auto *item = new QTableWidgetItem();
            ui->registersTabel->setItem(row, column, item);
        }
    }
    for (int row = 0; row < ui->stackTabel->rowCount(); row++)
    {
        for (int column = 0; column < ui->stackTabel->columnCount(); column++)
        {
            auto *item = new QTableWidgetItem();
            ui->stackTabel->setItem(row, column, item);
        }
    }
}
void DebugWidget::Update()
{
    auto *item = ui->registersTabel->item(0, 0);
    item->setData(Qt::DisplayRole,
                  std::format("0x{:02X}", chip8->programCounter).c_str());

    item = ui->registersTabel->item(1, 0);
    item->setData(Qt::DisplayRole,
                  std::format("0x{:02X}", chip8->indexRegister).c_str());

    item = ui->registersTabel->item(2, 0);
    item->setData(Qt::DisplayRole,
                  std::format("0x{:02X}", chip8->delayTimer).c_str());

    item = ui->registersTabel->item(3, 0);
    item->setData(Qt::DisplayRole,
                  std::format("0x{:02X}", chip8->soundTimer).c_str());
    for (int i = 0; i < 16; i++)
    {
        item = ui->registersTabel->item(i + 4, 0);
        item->setData(Qt::DisplayRole,
                      std::format("0x{:02X}", chip8->registers[i]).c_str());
    }

    for (int i = 0; i < 16; i++)
    {
        item = ui->stackTabel->item(i, 0);
        item->setData(Qt::DisplayRole, std::format("").c_str());
    }
    auto stackCopy = chip8->stack;
    int index = 0;
    while (!stackCopy.empty())
    {
        item = ui->stackTabel->item(index, 0);

        uint8_t value = stackCopy.top();
        stackCopy.pop();

        item->setData(Qt::DisplayRole, std::format("0x{:02X}", value).c_str());
        index++;
    }

    {
        uint16_t instruction = chip8->instruction;
        uint8_t instructNumber = (0xf000 & instruction) >> 12;
        std::string inst = "";
        switch (instructNumber)
        {
        case 0x0:
        {
            if (instruction == 0x00EE)
            {
                inst = "00EE";
            }
            else if (instruction == 0x00E0)
            {
                inst = "00E0";
            }
        }
        break;
        case 0x1:
        {
            inst = "1NNN";
        }
        break;
        case 0x2:
        {
            inst = "2NNN";
        }
        break;
        case 0x3:
        {
            inst = "3XNN";
        }
        break;
        case 0x4:
        {
            inst = "4XNN";
        }
        break;
        case 0x5:
        {
            inst = "5XY0";
        }
        break;
        case 0x6:
        {
            inst = "6XNN";
        }
        break;
        case 0x7:
        {
            inst = "7XNN";
        }
        break;
        case 0x8:
        {
            uint8_t last4Bits = (instruction & 0x000f);
            switch (last4Bits)
            {
            case 0:
                inst = "8XY0";
                break;
            case 1:
                inst = "8XY1";
                break;
            case 2:
                inst = "8XY2";
                break;
            case 3:
                inst = "8XY3";
                break;
            case 4:
                inst = "8XY4";
                break;
            case 5:
                inst = "8XY5";
                break;
            case 6:
                inst = "8XY6";
                break;
            case 7:
                inst = "8XY7";
                break;
            case 0xE:
                inst = "8XYE";
                break;
            }
        }
        break;
        case 0x9:
        {
            inst = "9XY0";
        }
        break;
        case 0xA:
        {
            inst = "ANNN";
        }
        break;
        case 0xB:
        {
            inst = "BNNN";
        }
        break;
        case 0xC:
        {
            inst = "CXNN";
        }
        break;
        case 0xD:
        {
            inst = "DXYN";
        }
        break;
        case 0xE:
        {
            uint8_t lastByte = 0x00ff & instruction;

            if (lastByte == 0x9E)
            {
                inst = "EX9E";
            }
            else if (lastByte == 0xA1)
            {
                inst = "EXA1";
            }
        }
        break;
        case 0xF:
        {
            uint8_t lastByte = 0x00ff & instruction;

            switch (lastByte)
            {
            case 0x07:
                inst = "FX07";
                break;
            case 0x15:
                inst = "FX15";
                break;
            case 0x18:
                inst = "FX18";
                break;
            case 0x1E:
                inst = "FX1E";
                break;
            case 0x0A:
                inst = "FX0A";
                break;
            case 0x29:
                inst = "FX29";
                break;
            case 0x33:
                inst = "FX33";
                break;
            case 0x55:
                inst = "FX55";
                break;
            case 0x65:
                inst = "FX65";
                break;
            }
        }
        break;
        default:
        {
            inst = "Unkown";
        }
        break;
        }

        ui->instruction->setText(std::format("Instruction: {}", inst).c_str());
    }
}
DebugWidget::~DebugWidget() { delete ui; }
