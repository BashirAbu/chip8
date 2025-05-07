#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui
{
    class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ConfigDialog(class Config *config, class Rom *rom,
                          QWidget *parent = nullptr);
    ~ConfigDialog();
    void SetStartValues(class Rom *startRom);
    void ConfigConfig();
    void ConfigRom();
  public slots:
    void accept() override;

  private:
    Ui::ConfigDialog *ui;
    uint32_t bgColor;
    uint32_t fgColor;
    uint32_t tickRate;
    class Config *config;
    class Rom *rom;
};

#endif // CONFIGDIALOG_H
