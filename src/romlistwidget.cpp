#include "configdialog.h"
#include "romlistwidget.h"
#include "ui_romlistwidget.h"
#include <QListWidgetItem>
#include <QMenu>
#include <qaction.h>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qevent.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qnamespace.h>
RomListItem::RomListItem(Rom &rom, QWidget *parent) : QWidget(parent), rom(rom)
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
    ui->romsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->romsList, &QListWidget::customContextMenuRequested, this,
            &RomListWidget::ShowContextMenu);
    ui->directoryLabel->setText(
        (config.romsDir.generic_string() + ":").c_str());
    for (auto &rom : roms)
    {
        auto romItem = new RomListItem(rom, this);

        auto item = new QListWidgetItem(ui->romsList);
        item->setSizeHint(romItem->sizeHint());

        ui->romsList->setItemWidget(item, romItem);
    }
}

RomListWidget::~RomListWidget() { delete ui; }

QListWidget *RomListWidget::GetListWidget() { return ui->romsList; }

void RomListWidget::ShowContextMenu(const QPoint &point)
{
    QListWidgetItem *item = ui->romsList->itemAt(point);

    if (!item)
    {
        return;
    }

    QMenu menu(ui->romsList);
    QAction *play = menu.addAction("Play");
    QAction *config = menu.addAction("Config");

    auto *romItem = dynamic_cast<RomListItem *>(ui->romsList->itemWidget(item));

    connect(play, &QAction::triggered, this,
            [=] { emit romItem->PlayRom(romItem->rom); });
    connect(config, &QAction::triggered, this,
            [&] { EmitConfigRomFileSignal(&romItem->rom); });
    auto list = ui->romsList;
    menu.exec(ui->romsList->viewport()->mapToGlobal(point));
}

void RomListWidget::EmitConfigRomFileSignal(Rom *rom)
{
    ConfigDialog *dialog = new ConfigDialog(nullptr, rom, this);
    dialog->show();
    emit ConfigRomFile(rom);
}
