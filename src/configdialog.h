#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "mainwindow.h"
#include <QDialog>
namespace Ui
{
    class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ConfigDialog(Config *config, Rom *rom, QWidget *parent = nullptr);
    ~ConfigDialog();
    void SetStartValues(Rom *startRom);
    void ConfigConfig();
    void ConfigRom();
  signals:
    void WriteRomFile(Rom *rom);
  public slots:
    void accept() override;

  private:
    Ui::ConfigDialog *ui;
    uint32_t bgColor;
    uint32_t fgColor;
    uint32_t tickRate;
    Config *config;
    Rom *rom;
};

#endif // CONFIGDIALOG_H
