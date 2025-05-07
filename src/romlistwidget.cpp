#include "romlistwidget.h"
#include "ui_romlistwidget.h"
#include <QListWidgetItem>
#include <qboxlayout.h>
#include <qevent.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qnamespace.h>
RomListItem::RomListItem(const Rom &rom, QWidget *parent)
    : QWidget(parent), rom(rom)
{

    hlayout = new QHBoxLayout(this);

    this->setLayout(hlayout);
    romTitle = new QLabel(this);
    hlayout->addWidget(romTitle);
    QString romName = rom.path.stem().string().c_str();
    romTitle->setText(romName);
    romTitle->setAlignment(Qt::AlignLeft);
    romTitle->setFont(QFont("Arial", 18));
}

void RomListItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        emit PlayRom(rom);
    }
}

RomListItem::~RomListItem() {}
RomListWidget::RomListWidget(const Config &config, std::vector<Rom> &roms,
                             QWidget *parent)
    : QWidget(parent), ui(new Ui::RomListWidget), roms(roms)
{
    ui->setupUi(this);
    ui->directoryLabel->setText(
        (config.romsDir.generic_string() + ":").c_str());
    for (const auto &rom : roms)
    {
        auto romItem = new RomListItem(rom, this);

        auto item = new QListWidgetItem(ui->romsList);
        item->setSizeHint(romItem->sizeHint());

        ui->romsList->setItemWidget(item, romItem);
    }
}

RomListWidget::~RomListWidget() { delete ui; }

QListWidget *RomListWidget::GetListWidget() { return ui->romsList; }
