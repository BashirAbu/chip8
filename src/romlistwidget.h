#ifndef ROMLISTWIDGET_H
#define ROMLISTWIDGET_H

#include "mainwindow.h"
#include <QWidget>
#include <qlistwidget.h>
#include <qtmetamacros.h>
namespace Ui
{
    class RomListWidget;
}

class RomListItem : public QWidget
{
    Q_OBJECT
  public:
    RomListItem(const Rom &rom, QWidget *parent = nullptr);
    ~RomListItem();
  signals:
    void PlayRom(const Rom &rom);

  protected:
    void mouseDoubleClickEvent(class QMouseEvent *event) override;
    const Rom &rom;
    class QLabel *romTitle = nullptr;
    class QHBoxLayout *hlayout = nullptr;
};

class RomListWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit RomListWidget(const Config &config, std::vector<Rom> &roms,
                           QWidget *parent = nullptr);
    ~RomListWidget();
    QListWidget *GetListWidget();

  private:
    std::vector<Rom> &roms;
    Ui::RomListWidget *ui;
};

#endif // ROMLISTWIDGET_H
