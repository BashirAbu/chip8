#include "chip_8.h"
#include "chip_8_widget.h"
#include "mainwindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <qapplication.h>
#include <qiterator.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpoint.h>
#include <qtimer.h>

Chip8Widget::Chip8Widget(const Rom *rom, QWidget *parent)
    : QWidget(parent), rom(rom)
{
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
}

void Chip8Widget::Play()
{
    qImage = new QImage({DISPLAY_WIDTH, DISPLAY_HEIGHT}, QImage::Format_RGB888);

    chip8 = new Chip8(rom->path);
    chip8Timer = new QTimer(this);
    QObject::connect(chip8Timer, &QTimer::timeout, chip8,
                     &Chip8::IncrementTimers);
    chip8Timer->start(16);

    QObject::connect(chip8, &Chip8::RefreshDisplay, this,
                     &Chip8Widget::RefreshImage);

    tickTimer = new QTimer(this);
    QObject::connect(tickTimer, &QTimer::timeout, this, &Chip8Widget::Tick);
    tickTimer->start((int)rom->tickRate);
}

void Chip8Widget::Pause()
{
    tickTimer->stop();
    chip8Timer->stop();
}
void Chip8Widget::Resume()
{
    tickTimer->start();
    chip8Timer->start();
}
Chip8Widget::~Chip8Widget()
{
    delete qImage;
    delete chip8;
}

void Chip8Widget::Tick()
{
    chip8->Tick();
    if (chip8->soundTimer != 0)
    {
        Beep();
    }
}

void Chip8Widget::Beep() {}
void Chip8Widget::RefreshImage()
{

    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < DISPLAY_WIDTH; x++)
        {
            QRgb rgb;
            if (chip8->display[(x + (y * DISPLAY_WIDTH))])
            {
                rgb = (int)rom->fgColor;
            }
            else
            {
                rgb = (int)rom->bgColor;
            }
            qImage->setPixelColor({x, y}, rgb);
        }
    }
    update();
}
void Chip8Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QSize scaled = qImage->size();
    scaled.scale(size(), Qt::KeepAspectRatio);
    QRect targetRect(QPoint(0, 0), scaled);
    targetRect.moveCenter(rect().center());
    p.drawImage(targetRect, *qImage);
}
void Chip8Widget::keyPressEvent(QKeyEvent *event)
{
    if (chip8->qKeyToChip8KeyMap.contains(event->key()) &&
        chip8->currentKey != event->key())
    {
        chip8->currentKey = event->key();
    }
}
void Chip8Widget::keyReleaseEvent(QKeyEvent *event)
{
    if (chip8->qKeyToChip8KeyMap.contains(event->key()))
    {
        chip8->currentKey = -1;
    }
}
