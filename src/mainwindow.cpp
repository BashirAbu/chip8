#include "./ui_mainwindow.h"
#include "chip_8_widget.h"
#include "configdialog.h"
#include "mainwindow.h"
#include "romdirwidget.h"
#include "romlistwidget.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>
#include <qaction.h>
#include <qimage.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <qstackedwidget.h>
#include <qvariant.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    centralStackWidget = new QStackedWidget(this);
    setCentralWidget(centralStackWidget);
    ParseConfigFile();
    romDirWidget = new RomDirWidget(&config, this);

    centralStackWidget->addWidget(romDirWidget);
    centralStackWidget->setCurrentWidget(romDirWidget);
    QObject::connect(romDirWidget, &RomDirWidget::HideMe, this,
                     &MainWindow::HideRomDirWidget);
    QObject::connect(romDirWidget, &RomDirWidget::ChangeRomsDir, this,
                     &MainWindow::ChangeRomsDir);
    if (!config.romsDir.empty())
    {
        HideRomDirWidget();
    }
    auto *fileMenu = ui->menubar->addMenu("File");

    QAction *changeCurrentRomsDirAction =
        new QAction("Change roms directory", this);
    QAction *configAction = new QAction("Config", this);

    fileMenu->addAction(changeCurrentRomsDirAction);
    fileMenu->addAction(configAction);

    connect(changeCurrentRomsDirAction, &QAction::triggered, this,
            &MainWindow::ChangeRomsDir);

    connect(configAction, &QAction::triggered, this, [&] { ConfigAct(); });
    auto *emulationMenu = ui->menubar->addMenu("Emulation");
    pauseAct = new QAction("Pause", this);
    QAction *stopAct = new QAction("Stop", this);
    QAction *restartAct = new QAction("Restart", this);
    emulationMenu->addAction(pauseAct);
    emulationMenu->addAction(stopAct);
    emulationMenu->addAction(restartAct);

    connect(pauseAct, &QAction::triggered, this, &MainWindow::PauseEmulation);
    connect(stopAct, &QAction::triggered, this, &MainWindow::StopEmulation);
    connect(restartAct, &QAction::triggered, this,
            &MainWindow::RestartEmulation);
}

MainWindow::~MainWindow() { delete ui; }
void MainWindow::ConfigAct(Rom *rom)
{
    if (rom)
    {

        ConfigDialog *dialog = new ConfigDialog(nullptr, rom, this);
        connect(dialog, &ConfigDialog::WriteRomFile, this,
                &MainWindow::WriteRomConfigFile);
        dialog->show();
    }
    else
    {
        ConfigDialog *dialog = new ConfigDialog(&config, rom, this);
        connect(dialog, &ConfigDialog::accepted, this,
                &MainWindow::WriteConfigFile);
        dialog->show();
    }
}
void MainWindow::ChangeRomsDir()
{
    config.romsDir =
        QFileDialog::getExistingDirectory(
            this, "Select rom directory", QDir::homePath(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)
            .toStdString();
    if (!config.romsDir.empty())
    {
        WriteConfigFile();
        HideRomDirWidget();
    }
}

void MainWindow::ParseRomsInRomDir()
{
    for (auto path : std::filesystem::directory_iterator(config.romsDir))
    {
        if (path.is_regular_file())
        {
            if (path.path().extension() == ".ch8" ||
                path.path().extension() == ".c8")
            {
                std::filesystem::path romConfigPath = std::format(
                    "config/{}/config.json", path.path().filename().string());
                roms.push_back(ParseRomConfigFile(romConfigPath));
            }
        }
    }
}
Rom MainWindow::ParseRomConfigFile(std::filesystem::path configPath)
{
    if (!std::filesystem::exists(configPath.parent_path()))
    {
        std::filesystem::create_directory(configPath.parent_path());
    }

    QFile configFile(configPath);

    if (!configFile.open(QIODevice::ReadWrite))
    {
        qFatal() << "Failed to open rom's config file: "
                 << configPath.generic_string();
    }

    QByteArray jsonData = configFile.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    Rom rom;

    if (!doc.isObject())
    {
        rom.path = config.romsDir / configPath.parent_path().filename();
        QJsonObject root = {
            {"path", rom.path.generic_string().c_str()},
            {"tick_rate", (int64_t)config.defRomConfig.tickRate},
            {"bg_color", (int64_t)config.defRomConfig.bgColor},
            {"fg_color", (int64_t)config.defRomConfig.fgColor},
            {"use_def_values", true}};
        doc.setObject(root);
        if (!doc.isObject())
        {
            qFatal() << "Failed to write json to doc";
        }
        configFile.close();
        configFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        configFile.write(doc.toJson(QJsonDocument::Indented));
    }
    else
    {
        QJsonObject obj = doc.object();
        if (obj.contains("path"))
        {
            rom.path = obj.value("path").toString().toStdString();
        }
        if (obj.contains("tick_rate"))
        {
            rom.tickRate = (uint32_t)obj.value("tick_rate").toInteger();
        }
        if (obj.contains("bg_color"))
        {
            rom.bgColor = (uint32_t)obj.value("bg_color").toInteger();
        }
        if (obj.contains("fg_color"))
        {
            rom.fgColor = (uint32_t)obj.value("fg_color").toInteger();
        }
        if (obj.contains("use_def_values"))
        {
            rom.useDefValuse = (uint32_t)obj.value("use_def_values").toBool();
        }
    }

    configFile.close();
    return rom;
}
void MainWindow::WriteRomConfigFile(Rom *rom)
{
    std::filesystem::path configPath =
        "config" / rom->path.filename() / "config.json";
    QFile configFile(configPath);

    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        ParseConfigFile();
    }

    QJsonObject root = {{"path", rom->path.generic_string().c_str()},
                        {"tick_rate", (int64_t)rom->tickRate},
                        {"bg_color", (int64_t)rom->bgColor},
                        {"fg_color", (int64_t)rom->fgColor},
                        {"use_def_values", rom->useDefValuse}};
    if (rom->useDefValuse)
    {
        root = {{"path", rom->path.generic_string().c_str()},
                {"tick_rate", (int64_t)config.defRomConfig.tickRate},
                {"bg_color", (int64_t)config.defRomConfig.bgColor},
                {"fg_color", (int64_t)config.defRomConfig.fgColor},
                {"use_def_values", true}};
    }
    configFile.write(QJsonDocument(root).toJson());
    configFile.close();
}
void MainWindow::HideRomDirWidget()
{
    romDirWidget->hide();
    ParseRomsInRomDir();

    if (!romListWidget)
    {
        romListWidget = new RomListWidget(config, roms, this);
        for (int i = 0; i < romListWidget->GetListWidget()->count(); i++)
        {
            auto *item = dynamic_cast<RomListItem *>(
                romListWidget->GetListWidget()->itemWidget(
                    romListWidget->GetListWidget()->item(i)));

            QObject::connect(item, &RomListItem::PlayRom, this,
                             &MainWindow::PlayRom);
        }
        connect(romListWidget, &RomListWidget::ConfigRomFile, this,
                &MainWindow::WriteRomConfigFile);
    }
    centralStackWidget->addWidget(romListWidget);
    centralStackWidget->setCurrentWidget(romListWidget);
}
void MainWindow::PlayRom(const Rom &rom)
{

    for (auto &r : roms)
    {
        if (r.useDefValuse)
        {
            auto temp = r.path;
            r = config.defRomConfig;
            r.path = temp;
        }
    }
    chip8Widget = new Chip8Widget(&rom, this);
    romListWidget->hide();
    centralStackWidget->addWidget(chip8Widget);
    centralStackWidget->setCurrentWidget(chip8Widget);
    chip8Widget->Play();
}
void MainWindow::PauseEmulation()
{
    if (chip8Widget && pauseAct->text() == "Pause")
    {
        chip8Widget->Pause();
        pauseAct->setText("Resume");
    }
    else if (chip8Widget && pauseAct->text() == "Resume")
    {
        chip8Widget->Resume();
        pauseAct->setText("Pause");
    }
}
void MainWindow::StopEmulation()
{
    if (chip8Widget)
    {
        pauseAct->setText("Pause");
        chip8Widget->hide();
        centralStackWidget->removeWidget(chip8Widget);
        romListWidget->setHidden(false);
        centralStackWidget->setCurrentWidget(romListWidget);
        delete chip8Widget;
        chip8Widget = nullptr;
    }
}
void MainWindow::RestartEmulation()
{
    if (chip8Widget)
    {
        pauseAct->setText("Pause");
        const Rom *currentRom = chip8Widget->rom;
        chip8Widget->hide();
        centralStackWidget->removeWidget(chip8Widget);
        delete chip8Widget;
        chip8Widget = new Chip8Widget(currentRom, this);
        centralStackWidget->addWidget(chip8Widget);
        centralStackWidget->setCurrentWidget(chip8Widget);
        chip8Widget->Play();
    }
}
void MainWindow::ParseConfigFile()
{
    bool exists = std::filesystem::exists("config/");
    if (!exists)
    {
        std::filesystem::create_directory("config/");
    }

    QFile configFile("config/config.json");

    if (!configFile.open(QIODevice::ReadWrite))
    {
        qFatal() << "Failed to open config.json";
    }

    QByteArray jsonData = configFile.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (!doc.isObject())
    {
        QJsonObject root = {
            {"rom_dir", ""},
            {"tick_rate", (int64_t)config.defRomConfig.tickRate},
            {"bg_color", (int64_t)config.defRomConfig.bgColor},
            {"fg_color", (int64_t)config.defRomConfig.fgColor},
        };
        doc.setObject(root);
        if (!doc.isObject())
        {
            qFatal() << "Failed to write json to doc";
        }
        configFile.close();
        configFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        configFile.write(doc.toJson(QJsonDocument::Indented));
    }
    else
    {
        QJsonObject obj = doc.object();
        if (obj.contains("rom_dir"))
        {
            config.romsDir = obj.value("rom_dir").toString().toStdString();
        }
        if (obj.contains("tick_rate"))
        {
            config.defRomConfig.tickRate =
                (uint32_t)obj.value("tick_rate").toInteger();
        }
        if (obj.contains("bg_color"))
        {
            config.defRomConfig.bgColor =
                (uint32_t)obj.value("bg_color").toInteger();
        }
        if (obj.contains("fg_color"))
        {
            config.defRomConfig.fgColor =
                (uint32_t)obj.value("fg_color").toInteger();
        }
    }

    configFile.close();
}
void MainWindow::WriteConfigFile()
{
    QFile configFile("config/config.json");

    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        ParseConfigFile();
    }

    QJsonObject obj = {
        {"rom_dir", config.romsDir.generic_string().c_str()},
        {"tick_rate", (int64_t)config.defRomConfig.tickRate},
        {"bg_color", (int64_t)config.defRomConfig.bgColor},
        {"fg_color", (int64_t)config.defRomConfig.fgColor},
    };

    configFile.write(QJsonDocument(obj).toJson());
    configFile.close();

    for (auto &r : roms)
    {
        if (r.useDefValuse)
        {
            auto temp = r.path;
            r = config.defRomConfig;
            r.path = temp;
        }
    }
}
