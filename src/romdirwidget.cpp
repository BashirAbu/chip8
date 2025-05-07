#include "mainwindow.h"
#include "romdirwidget.h"
#include "ui_romdirwidget.h"
#include <QFileDialog>
#include <QMouseEvent>
RomDirWidget::RomDirWidget(Config *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::RomDirWidget), config(config)
{
    ui->setupUi(this);
    ui->imgLabel->installEventFilter(this);
}

bool RomDirWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->imgLabel && event->type() == QEvent::MouseButtonPress)
    {
        auto e = dynamic_cast<QMouseEvent *>(event);
        if (e->button() == Qt::LeftButton)
        {

            emit ChangeRomsDir();

            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
RomDirWidget::~RomDirWidget() { delete ui; }
