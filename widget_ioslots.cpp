#include "widget_ioslots.h"
#include "ui_widget_ioslots.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QMenu>

#include "ioslots_model.h"

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

#define setSlotDriver(slotClass) \
    QItemSelectionModel *selection = ui->tvIoslots->selectionModel(); \
    QModelIndexList indexes = selection->selectedRows(); \
    if (indexes.count() == 0) \
        return; \
    foreach (QModelIndex index, indexes) { \
        slotClass *slot = new slotClass(index.row() + 1); \
        QSharedPointer<Ioslot> ptr(slot); \
        d_hponic->ioslotManager()->replaceIoslot(index.row(), ptr); \
        d_selectedSlot = ptr; \
        swapWidgetConfigSlot(new WidgetConfig##slotClass(IoslotConv::toSlot<slotClass>(ptr), d_hponic, this)); \
    }

void WidgetIoslots::setAnalogInputDriver()
{
    setSlotDriver(AnalogInputSlot);
}

void WidgetIoslots::setDiscreteInputDriver()
{
    setSlotDriver(DiscreteInputSlot);
}

void WidgetIoslots::setDiscreteOutputDriver()
{
    setSlotDriver(DiscreteOutputSlot);
}

void WidgetIoslots::setDHTxxDriver()
{
    setSlotDriver(DHTxxSlot);
}

void WidgetIoslots::setDallasTemperatureDriver()
{
    setSlotDriver(DallasTemperatureSlot);
}

void WidgetIoslots::setEmptySlotDriver()
{
    setSlotDriver(EmptySlot);
}

void WidgetIoslots::onIoslotCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    Ioslot *ptr = static_cast<Ioslot*>(current.internalPointer());
    QSharedPointer<Ioslot> ioslot = d_hponic->ioslotManager()->ioslotById(ptr->id());
    if (!ioslot)
        return;

    if (d_selectedSlot != ioslot) {
        d_selectedSlot = ioslot;

        QWidget *widget = 0;
        switch (ioslot->driver()) {
        case EmptySlotDriver:
        {
            QSharedPointer<EmptySlot> emptySlot = IoslotConv::toSlot<EmptySlot>(ioslot);
            if (emptySlot)
                widget = new WidgetConfigEmptySlot(emptySlot, d_hponic, this);
            break;
        }
        case AnalogInputDriver:
        {
            QSharedPointer<AnalogInputSlot> analogInput = IoslotConv::toSlot<AnalogInputSlot>(ioslot);
            if (analogInput)
                widget = new WidgetConfigAnalogInputSlot(analogInput, d_hponic, this);
            break;
        }
        case DiscreteInputDriver:
        {
            QSharedPointer<DiscreteInputSlot> discreteInput = IoslotConv::toSlot<DiscreteInputSlot>(ioslot);
            if (discreteInput)
                widget = new WidgetConfigDiscreteInputSlot(discreteInput, d_hponic, this);
            break;
        }
        case DiscreteOutputDriver:
        {
            QSharedPointer<DiscreteOutputSlot> discreteOutput = IoslotConv::toSlot<DiscreteOutputSlot>(ioslot);
            if (discreteOutput)
                widget = new WidgetConfigDiscreteOutputSlot(discreteOutput, d_hponic, this);
            break;
        }
        case DHTxxDriver:
        {
            QSharedPointer<DHTxxSlot> dhtxx = IoslotConv::toSlot<DHTxxSlot>(ioslot);
            if (dhtxx)
                widget = new WidgetConfigDHTxxSlot(dhtxx, d_hponic, this);
            break;
        }
        case DallasTemperatureDriver:
        {
            QSharedPointer<DallasTemperatureSlot> dallasTemperature = IoslotConv::toSlot<DallasTemperatureSlot>(ioslot);
            if (dallasTemperature)
                widget = new WidgetConfigDallasTemperatureSlot(dallasTemperature, d_hponic, this);
            break;
        }

        default:
            break;
        }

        swapWidgetConfigSlot(widget);
    }
}

void WidgetIoslots::onIoslotsDownloadStarted()
{
    ui->tbDownload->setEnabled(false);
    ui->lStatus->clear();
}

void WidgetIoslots::onIoslotsDownloadFinished(bool success)
{
    if (!success) {
        ui->lStatus->setText(tr("IO slots download failed"));
    }

    ui->tbDownload->setEnabled(true);
}

void WidgetIoslots::onIoslotsUploadStarted()
{
    ui->tbUpload->setEnabled(false);
    ui->lStatus->clear();
}

void WidgetIoslots::onIoslotsUploadFinished(bool success)
{
    if (!success) {
        ui->lStatus->setText(tr("IO slots upload failed"));
    }

    ui->tbUpload->setEnabled(true);
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
    QMenu *menuSetSlotDriver = new QMenu;
    menuSetSlotDriver->addAction(ui->actionSetEmptySlotDriver);
    menuSetSlotDriver->addSeparator();
    menuSetSlotDriver->addAction(ui->actionSetAnalogInputDriver);
    menuSetSlotDriver->addAction(ui->actionSetDiscreteInputDriver);
    menuSetSlotDriver->addAction(ui->actionSetDiscreteOutputDriver);
    menuSetSlotDriver->addAction(ui->actionSetDHTxxDriver);
    menuSetSlotDriver->addAction(ui->actionSetDallasTemperatureDriver);

    ui->tbSetIoslotDriver->setMenu(menuSetSlotDriver);
}

void WidgetIoslots::createLayouts()
{
    QHBoxLayout *layoutControls = new QHBoxLayout;
    layoutControls->addWidget(ui->tbSetIoslotDriver);
    layoutControls->addStretch(1);
    layoutControls->addWidget(ui->tbDownload);
    layoutControls->addWidget(ui->tbUpload);

    QVBoxLayout *layoutIoslot = new QVBoxLayout;
    layoutIoslot->addLayout(layoutControls);
    layoutIoslot->addWidget(ui->lStatus, 0, Qt::AlignRight);
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
    connect(ui->actionSetAnalogInputDriver, SIGNAL(triggered()), this, SLOT(setAnalogInputDriver()), Qt::DirectConnection);
    connect(ui->actionSetDiscreteInputDriver, SIGNAL(triggered()), this, SLOT(setDiscreteInputDriver()), Qt::DirectConnection);
    connect(ui->actionSetDiscreteOutputDriver, SIGNAL(triggered()), this, SLOT(setDiscreteOutputDriver()), Qt::DirectConnection);
    connect(ui->actionSetDHTxxDriver, SIGNAL(triggered()), this, SLOT(setDHTxxDriver()), Qt::DirectConnection);
    connect(ui->actionSetDallasTemperatureDriver, SIGNAL(triggered()), this, SLOT(setDallasTemperatureDriver()), Qt::DirectConnection);

    connect(ui->actionSetEmptySlotDriver, SIGNAL(triggered()), this, SLOT(setEmptySlotDriver()), Qt::DirectConnection);

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
