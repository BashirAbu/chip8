#ifndef ROMDIRWIDGET_H
#define ROMDIRWIDGET_H

#include <QWidget>

namespace Ui
{
    class RomDirWidget;
}

class RomDirWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit RomDirWidget(class Config *config, QWidget *parent = nullptr);
    ~RomDirWidget();

    bool eventFilter(QObject *watched, QEvent *event) override;
  signals:
    void HideMe();
    void ChangeRomsDir();

  private:
    Ui::RomDirWidget *ui;
    class Config *config = nullptr;
};

#endif // ROMDIRWIDGET_H
