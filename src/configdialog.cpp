#include "configdialog.h"
#include "mainwindow.h"
#include "ui_configdialog.h"
#include <QColorDialog>
#include <qcolordialog.h>
#include <qdialog.h>
#include <qvalidator.h>
ConfigDialog::ConfigDialog(class Config *config, class Rom *rom,
                           QWidget *parent)
    : QDialog(parent), ui(new Ui::ConfigDialog), config(config), rom(rom)
{
    ui->setupUi(this);
    ui->tickRateLE->setValidator(
        new QIntValidator(0, 2147483647, ui->tickRateLE));
    Rom *startRom = nullptr;
    if (config)
    {
        startRom = &config->defRomConfig;
    }
    else if (rom)
    {
        startRom = rom;
    }
    SetStartValues(startRom);

    connect(ui->bgColorPB, &QPushButton::clicked, this,

            [&]
            {
                QColor color = QColorDialog::getColor(bgColor, this,
                                                      "Pick background color");
                if (color.isValid())
                {
                    bgColor = color.rgb();
                    ui->bgColorPB->setStyleSheet(
                        QString("background:#%1;")
                            .arg(bgColor, 6, 16, QLatin1Char('0')));
                }
            });
    connect(ui->fgColorPB, &QPushButton::clicked, this,
            [&]
            {
                QColor color = QColorDialog::getColor(fgColor, this,
                                                      "Pick foreground color");
                if (color.isValid())
                {
                    fgColor = color.rgb();
                    ui->fgColorPB->setStyleSheet(
                        QString("background:#%1;")
                            .arg(fgColor, 6, 16, QLatin1Char('0')));
                }
            });
}

void ConfigDialog::SetStartValues(Rom *startRom)
{
    ui->tickRateLE->setText(std::format("{}", startRom->tickRate).c_str());
    tickRate = startRom->tickRate;
    ui->bgColorPB->setStyleSheet(
        QString("background:#%1;")
            .arg(startRom->bgColor, 6, 16, QLatin1Char('0')));
    bgColor = startRom->bgColor;
    ui->fgColorPB->setStyleSheet(
        QString("background:#%1;")
            .arg(startRom->fgColor, 6, 16, QLatin1Char('0')));
    fgColor = startRom->fgColor;
}

ConfigDialog::~ConfigDialog() { delete ui; }

void ConfigDialog::accept()
{
    tickRate = (uint32_t)ui->tickRateLE->text().toLongLong();
    if (config)
    {
        ConfigConfig();
        emit accepted();
    }
    else if (rom)
    {
        ConfigRom();
        emit WriteRomFile(rom);
    }
    this->destroy(true);
}

void ConfigDialog::ConfigConfig()
{
    config->defRomConfig.bgColor = bgColor;
    config->defRomConfig.fgColor = fgColor;
    config->defRomConfig.tickRate = tickRate;
}
void ConfigDialog::ConfigRom()
{
    rom->bgColor = bgColor;
    rom->fgColor = fgColor;
    rom->tickRate = tickRate;
    rom->useDefValuse = false;
}
