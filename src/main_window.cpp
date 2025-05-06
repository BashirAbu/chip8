#include "main_window.h"
#include <QTimer>
#include <qboxlayout.h>
#include <qmainwindow.h>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->resize(1920, 1080);

    chip8Widget = new Chip8Widget(this);
    setCentralWidget(chip8Widget);

    timer = new QTimer(this);

    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::Tick);

    timer->start(1);
}

void MainWindow::Tick() { chip8Widget->Tick(); }
MainWindow::~MainWindow() {}
