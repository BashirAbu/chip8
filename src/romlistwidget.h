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

class ThumbnailContainer : public QWidget
{
    Q_OBJECT
  public:
    ThumbnailContainer(QString path, QWidget *parent = nullptr);
    ~ThumbnailContainer();

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QImage thumbnail;
};

class RomListItem : public QWidget
{
    Q_OBJECT
  public:
    RomListItem(Rom &rom, QWidget *parent = nullptr);
    ~RomListItem();
  signals:
    void PlayRom(const Rom &rom);

  protected:
    void mouseDoubleClickEvent(class QMouseEvent *event) override;
    Rom &rom;
    class QLabel *romTitle = nullptr;
    ThumbnailContainer *thumbnailCnt = nullptr;
    class QHBoxLayout *hlayout = nullptr;
    friend RomListWidget;
};

class RomListWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit RomListWidget(const Config &config, std::vector<Rom> &roms,
                           QWidget *parent = nullptr);
    ~RomListWidget();
    QListWidget *GetListWidget();
  public slots:
    void ShowContextMenu(const QPoint &point);
    void EmitConfigRomFileSignal(Rom *rom);
  signals:
    void ConfigRomFile(Rom *rom);

  private:
    std::vector<Rom> &roms;
    Ui::RomListWidget *ui;
};

#endif // ROMLISTWIDGET_H
