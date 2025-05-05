
#pragma once
#include "chip_8_widget.h"
#include <QtWidgets/QMainWindow>
#include <qobjectdefs.h>
class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  public slots:
    void Tick();

  private:
    Chip8Widget *chip8Widget;
    QTimer *timer;
};
