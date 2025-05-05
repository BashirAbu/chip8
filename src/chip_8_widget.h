#pragma once
#include "chip_8.h"
#include <QImage>
#include <QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qboxlayout.h>
class Chip8Widget : public QWidget
{
  public:
    Chip8Widget(QWidget *parent = nullptr);
    ~Chip8Widget();

    void Tick();

  protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
  public slots:
    void RefreshImage();

  private:
    QImage *qImage;
    Chip8 *chip8;
    QTimer *timer;
};
