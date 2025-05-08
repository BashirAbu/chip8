#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "romdirwidget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <qcontainerfwd.h>
#include <qstackedwidget.h>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

#include <filesystem>

#include "chip_8_widget.h"
struct Rom
{
    std::filesystem::path path;
    uint32_t tickRate = 1; // ms
    uint32_t bgColor = 0x00000000;
    uint32_t fgColor = 0xffffffff;
    bool useDefValuse = true;
};

struct Config
{
    std::filesystem::path romsDir;
    Rom defRomConfig;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  public slots:
    void HideRomDirWidget();
    void WriteConfigFile();
    void ParseConfigFile();
    void ParseRomsInRomDir();
    void PlayRom(const Rom &rom);
    void PauseEmulation();
    void StopEmulation();
    void RestartEmulation();
    void ChangeRomsDir();
    void ConfigAct(Rom *rom = nullptr);

  protected:
    bool event(QEvent *e) override;

  private:
    Rom ParseRomConfigFile(std::filesystem::path configPath);
    void GenRomThumbnail(Rom *rom);
    void WriteRomConfigFile(Rom *rom);

    Ui::MainWindow *ui;
    Chip8Widget *chip8Widget = nullptr;
    Config config = {};
    std::vector<Rom> roms;
    QStackedWidget *centralStackWidget;
    class RomDirWidget *romDirWidget = nullptr;
    class RomListWidget *romListWidget = nullptr;

    class QAction *pauseAct = nullptr;
};

#endif // MAINWINDOW_H
