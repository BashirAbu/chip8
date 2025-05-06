#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filesystem>

#include "chip_8_widget.h"
struct Rom
{
    uint32_t tickRate = 1; // ms
    uint32_t bgColor = 0x00000000;
    uint32_t fgColor = 0xffffffff;
};

struct Config
{
    std::filesystem::path romsDir;
    // keymap
};

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    Chip8Widget *chip8Widget;
    Config config = {};
};
#endif // MAINWINDOW_H
