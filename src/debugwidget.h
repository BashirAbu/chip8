#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QWidget>

namespace Ui
{
    class DebugWidget;
}

class DebugWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit DebugWidget(class Chip8 *chip8, QWidget *parent = nullptr);
    ~DebugWidget();
    void Update();

  private:
    class Chip8 *chip8;
    Ui::DebugWidget *ui;
};

#endif // DEBUGWIDGET_H
