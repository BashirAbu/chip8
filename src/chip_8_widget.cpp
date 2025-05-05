#include "chip_8.h"
#include "chip_8_widget.h"
#include <QPaintEvent>
#include <QPainter>
#include <cstring>
#include <qlogging.h>
Chip8Widget::Chip8Widget(QWidget *parent) : QWidget(parent)
{
    qImage =
        new QImage({DISPLAY_WIDTH, DISPLAY_HEIGHT}, QImage::Format_Grayscale8);

    chip8 = new Chip8("roms/IBM Logo.ch8");
}

Chip8Widget::~Chip8Widget()
{
    delete qImage;
    delete chip8;
}

void Chip8Widget::Tick()
{
    chip8->Tick();
    memcpy(qImage->bits(), chip8->display,
           (size_t)DISPLAY_WIDTH * DISPLAY_HEIGHT);
    update();
}

void Chip8Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawImage(rect(), *qImage);
}
