#pragma once
#include "chip_8.h"
#include <QHBoxLayout>
#include <QImage>
#include <QtWidgets/QWidget>
#include <cstdint>
#include <qboxlayout.h>

class DisplayContainer : public QWidget
{
    Q_OBJECT
  public:
    DisplayContainer(uint8_t *displayBuffer, const class Rom *rom,
                     QWidget *parent = nullptr);
    ~DisplayContainer();

    void RefreshImage();

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    uint8_t *displayBuffer = nullptr;
    const Rom *rom = nullptr;
    QImage qImage;
};

class Chip8Widget : public QWidget
{
    Q_OBJECT
  public:
    Chip8Widget(const class Rom *rom, QWidget *parent = nullptr);
    ~Chip8Widget();
    void Play();
    void Pause();
    void Resume();
    void Beep();

  protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
  public slots:
    void Tick();
    void RefreshImage();

  public:
    const class Rom *rom = nullptr;
    class DebugWidget *debugWidget = nullptr;
    QHBoxLayout *hlayout = nullptr;
    DisplayContainer *displayContainer = nullptr;
    Chip8 *chip8;
    QTimer *chip8Timer;
    QTimer *tickTimer;
};
