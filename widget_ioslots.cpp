#include "widget_ioslots.h"
#include "ui_widget_ioslots.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include <QMenu>

#include "ioslots_model.h"
#include "ioslot_providers.h"

#include "widget_config_slot.h"

WidgetIoslots::WidgetIoslots(QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetIOslots),
    d_hponic(hponic)
{
    ui->setupUi(this);

    createWidgets();
    createMenu();
    createLayouts();
    createConnections();
}

WidgetIoslots::~WidgetIoslots()
{
    delete ui;
}

#define setSlot(slotClass) \
    QItemSelectionModel *selection = ui->tvIoslots->selectionModel(); \
    QModelIndexList indexes = selection->selectedRows(); \
    if (indexes.count() == 0) \
        return; \
    foreach (QModelIndex index, indexes) { \
        QSharedPointer<Ioslot> slot = IoslotManager::create##slotClass(index.row() + 1); \
        d_hponic->ioslotManager()->replaceIoslot(index.row(), slot); \
        d_selectedSlot = slot; \
        swapWidgetConfigSlot(slot->providers()->editorProvider()->createEditor(d_hponic, this)); \
    }

void WidgetIoslots::setAnalogInputSlot()
{
    setSlot(AnalogInputSlot);
}

void WidgetIoslots::setDiscreteInputSlot()
{
    setSlot(DiscreteInputSlot);
}

void WidgetIoslots::setDiscreteOutputSlot()
{
    setSlot(DiscreteOutputSlot);
}

void WidgetIoslots::setDHTxxSlot()
{
    setSlot(DHTxxSlot);
}

void WidgetIoslots::setDallasTemperatureSlot()
{
    setSlot(DallasTemperatureSlot);
}

void WidgetIoslots::setMHZ19Slot()
{
    setSlot(MHZ19Slot);
}

void WidgetIoslots::setSHT2xSlot()
{
    setSlot(SHT2xSlot);
}

void WidgetIoslots::setEmptySlot()
{
    setSlot(EmptySlot);
}

void WidgetIoslots::onSetSlotClicked()
{
    bool enable = (ui->tvIoslots->selectionModel()->hasSelection());

    ui->actionSetAnalogInputSlot->setEnabled(enable);
    ui->actionSetDiscreteInputSlot->setEnabled(enable);
    ui->actionSetDiscreteOutputSlot->setEnabled(enable);
    ui->actionSetDallasTemperatureSlot->setEnabled(enable);
    ui->actionSetDHTxxSlot->setEnabled(enable);
    ui->actionSetSHT2xSlot->setEnabled(enable);
    ui->actionSetMHZ19Slot->setEnabled(enable);    

    ui->actionSetEmptySlot->setEnabled(enable);
}

void WidgetIoslots::onIoslotCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    QSharedPointer<Ioslot> slot = d_hponic->ioslotManager()->ioslot(current.row());
    if (!slot)
        return;

    if (d_selectedSlot != slot) {
        d_selectedSlot = slot;

        QWidget *widget = slot->providers()->editorProvider()->createEditor(d_hponic, this);
        swapWidgetConfigSlot(widget);
    }
}

void WidgetIoslots::onIoslotsDownloadStarted()
{
    ui->tbDownload->setEnabled(false);
}

void WidgetIoslots::onIoslotsDownloadFinished(bool success)
{
    ui->tbDownload->setEnabled(true);

    if (!success) {
        QMessageBox::warning(this, tr("Download slot list"), tr("Error on download slot list from controller"));
    }
}

void WidgetIoslots::onIoslotsUploadStarted()
{
    ui->tbUpload->setEnabled(false);
}

void WidgetIoslots::onIoslotsUploadFinished(bool success)
{
    ui->tbUpload->setEnabled(true);

    if (!success) {
        QMessageBox::warning(this, tr("Upload slot list"), tr("Error on upload slot list to controller"));
    }
}

void WidgetIoslots::showContextMenu(QPoint)
{
    QMenu menu;
    menu.addAction(ui->actionResetIoslots);

    menu.exec(QCursor::pos());

}

void WidgetIoslots::createWidgets()
{
    ui->tvIoslots->setModel(new IoslotsModel(d_hponic->ioslotManager(), this));
    d_widgetConfigSlot = new QWidget(this);

    d_splitter = new QSplitter(Qt::Horizontal, this);
}

void WidgetIoslots::createMenu()
{
    QMenu *menuSetSlot = new QMenu;
    menuSetSlot->addAction(ui->actionSetEmptySlot);
    menuSetSlot->addSeparator();
    menuSetSlot->addAction(ui->actionSetAnalogInputSlot);
    menuSetSlot->addAction(ui->actionSetDiscreteInputSlot);
    menuSetSlot->addAction(ui->actionSetDiscreteOutputSlot);
    menuSetSlot->addAction(ui->actionSetDallasTemperatureSlot);
    menuSetSlot->addAction(ui->actionSetDHTxxSlot);
    menuSetSlot->addAction(ui->actionSetSHT2xSlot);
    menuSetSlot->addAction(ui->actionSetMHZ19Slot);

    ui->tbSetSlot->setMenu(menuSetSlot);
}

void WidgetIoslots::createLayouts()
{
    QHBoxLayout *layoutControls = new QHBoxLayout;
    layoutControls->addWidget(ui->tbSetSlot);
    layoutControls->addSpacing(50);
    layoutControls->addWidget(ui->tbDownload);
    layoutControls->addWidget(ui->tbUpload);
    layoutControls->addStretch(1);

    QVBoxLayout *layoutIoslot = new QVBoxLayout;
    layoutIoslot->addLayout(layoutControls);
    layoutIoslot->addWidget(d_widgetConfigSlot, 1);

    ui->scrollArea->setLayout(layoutIoslot);

    d_splitter->addWidget(ui->tvIoslots);
    d_splitter->addWidget(ui->scrollArea);

    QHBoxLayout *layoutMain = new QHBoxLayout;
    layoutMain->addWidget(d_splitter, 1);

    setLayout(layoutMain);
}

void WidgetIoslots::createConnections()
{    
    connect(ui->actionSetAnalogInputSlot, SIGNAL(triggered()), this, SLOT(setAnalogInputSlot()), Qt::DirectConnection);
    connect(ui->actionSetDiscreteInputSlot, SIGNAL(triggered()), this, SLOT(setDiscreteInputSlot()), Qt::DirectConnection);
    connect(ui->actionSetDiscreteOutputSlot, SIGNAL(triggered()), this, SLOT(setDiscreteOutputSlot()), Qt::DirectConnection);
    connect(ui->actionSetDHTxxSlot, SIGNAL(triggered()), this, SLOT(setDHTxxSlot()), Qt::DirectConnection);
    connect(ui->actionSetDallasTemperatureSlot, SIGNAL(triggered()), this, SLOT(setDallasTemperatureSlot()), Qt::DirectConnection);
    connect(ui->actionSetMHZ19Slot, SIGNAL(triggered()), this, SLOT(setMHZ19Slot()), Qt::DirectConnection);
    connect(ui->actionSetSHT2xSlot, SIGNAL(triggered()), this, SLOT(setSHT2xSlot()), Qt::DirectConnection);

    connect(ui->actionSetEmptySlot, SIGNAL(triggered()), this, SLOT(setEmptySlot()), Qt::DirectConnection);

    connect(ui->tbSetSlot->menu(), SIGNAL(aboutToShow()), this, SLOT(onSetSlotClicked()), Qt::DirectConnection);

    connect(ui->actionResetIoslots, SIGNAL(triggered()), d_hponic.data(), SLOT(resetIoslots()), Qt::DirectConnection);

    connect(ui->tbDownload, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(downloadIoslots()), Qt::DirectConnection);
    connect(ui->tbUpload, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(uploadIoslots()), Qt::DirectConnection);

    connect(ui->tvIoslots->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onIoslotCurrentChanged(QModelIndex,QModelIndex)), Qt::DirectConnection);

    connect(ui->tvIoslots, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(ioslotsDownloadStarted()), this, SLOT(onIoslotsDownloadStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(ioslotsDownloadFinished(bool)), this, SLOT(onIoslotsDownloadFinished(bool)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(ioslotsUploadStarted()), this, SLOT(onIoslotsUploadStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(ioslotsUploadFinished(bool)), this, SLOT(onIoslotsUploadFinished(bool)), Qt::DirectConnection);
}

void WidgetIoslots::swapWidgetConfigSlot(QWidget *widget)
{
    if (widget == 0)
        return;

    if (d_widgetConfigSlot) {
        ui->scrollArea->layout()->replaceWidget(d_widgetConfigSlot, widget);
        d_widgetConfigSlot->deleteLater();
    }

    d_widgetConfigSlot = widget;
    d_widgetConfigSlot->show();
}
