#include "chip_8.h"
#include "chip_8_widget.h"
#include <QPaintEvent>
#include <QPainter>
#include <cstring>
#include <qiterator.h>
#include <qlogging.h>
#include <qobject.h>
#include <qtimer.h>
#include <qwidget.h>
Chip8Widget::Chip8Widget(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    qImage =
        new QImage({DISPLAY_WIDTH, DISPLAY_HEIGHT}, QImage::Format_Grayscale8);

    chip8 = new Chip8("roms/6-keypad.ch8");
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, chip8, &Chip8::IncrementTimers);
    timer->start(int((1.0f / 60.0f) * 1000.0f));

    QObject::connect(chip8, &Chip8::RefreshDisplay, this,
                     &Chip8Widget::RefreshImage);
}

Chip8Widget::~Chip8Widget()
{
    delete qImage;
    delete chip8;
}

void Chip8Widget::Tick() { chip8->Tick(); }
void Chip8Widget::RefreshImage()
{
    memcpy(qImage->bits(), chip8->display,
           (size_t)DISPLAY_WIDTH * DISPLAY_HEIGHT);
    update();
}
void Chip8Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawImage(rect(), *qImage);
}
void Chip8Widget::keyPressEvent(QKeyEvent *event)
{
    if (chip8->qKeyToChip8KeyMap.contains(event->key()))
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
