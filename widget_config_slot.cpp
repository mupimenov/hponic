#include "widget_config_slot.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

WidgetConfigEmptySlot::WidgetConfigEmptySlot(QSharedPointer<EmptySlot> __ioslot, QWidget *parent)  : QWidget(parent),
    d_ioslot(__ioslot)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigEmptySlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("Empty slot"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);
}

void WidgetConfigEmptySlot::createLayouts()
{
    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addWidget(d_lSlotType);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigEmptySlot::createConnections()
{

}

WidgetConfigAnalogInputSlot::WidgetConfigAnalogInputSlot(QSharedPointer<AnalogInputSlot> __ioslot, QWidget *parent) : QWidget(parent),
    d_ioslot(__ioslot)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigAnalogInputSlot::slotNameChanged(const QString &__name)
{
    d_ioslot->setName(__name);
}

void WidgetConfigAnalogInputSlot::numChanged(int __num)
{
    d_ioslot->setNum(__num);
}

void WidgetConfigAnalogInputSlot::adc1Changed(int __adc1)
{
    d_ioslot->setLinear(__adc1, d_ioslot->y1(), d_ioslot->x2(), d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::adc2Changed(int __adc2)
{
    d_ioslot->setLinear(d_ioslot->x1(), d_ioslot->y1(), __adc2, d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::physical1Changed(double __physical1)
{
    d_ioslot->setLinear(d_ioslot->x1(), __physical1, d_ioslot->x2(), d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::physical2Changed(double __physical2)
{
    d_ioslot->setLinear(d_ioslot->x1(), d_ioslot->y1(), d_ioslot->x2(), __physical2);
}

void WidgetConfigAnalogInputSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("Analog input"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);
    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_lNum = new QLabel(tr("Number:"), this);
    d_sbNum = new QSpinBox(this);
    d_sbNum->setRange(0, 15);
    d_sbNum->setValue(d_ioslot->num());
    d_lCalibration = new QLabel(tr("Input calibration"), this);
    f = d_lCalibration->font();
    f.setItalic(true);
    d_lCalibration->setFont(f);
    d_lADC = new QLabel(tr("ADC:"), this);
    d_sbADC1 = new QSpinBox(this);
    d_sbADC1->setRange(0, 1023);
    d_sbADC1->setValue(d_ioslot->x1());
    d_sbADC2 = new QSpinBox(this);
    d_sbADC2->setRange(0, 1023);
    d_sbADC2->setValue(d_ioslot->x2());
    d_lPhysical = new QLabel(tr("Physical:"), this);
    d_dsbPhysical1 = new QDoubleSpinBox(this);
    d_dsbPhysical1->setValue(d_ioslot->y1());
    d_dsbPhysical2 = new QDoubleSpinBox(this);
    d_dsbPhysical2->setValue(d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leSlotName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,       row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lSlotName,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lNum,            row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbNum,           row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCalibration,    row, 0, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lADC,            row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbADC1,          row, 2, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbADC2,          row, 3, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lPhysical,       row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dsbPhysical1,    row, 2, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dsbPhysical2,    row, 3, 1, 1, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;    
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigAnalogInputSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbNum, SIGNAL(valueChanged(int)), this, SLOT(numChanged(int)), Qt::DirectConnection);
    connect(d_sbADC1, SIGNAL(valueChanged(int)), this, SLOT(adc1Changed(int)), Qt::DirectConnection);
    connect(d_sbADC2, SIGNAL(valueChanged(int)), this, SLOT(adc2Changed(int)), Qt::DirectConnection);
    connect(d_dsbPhysical1, SIGNAL(valueChanged(double)), this, SLOT(physical1Changed(double)), Qt::DirectConnection);
    connect(d_dsbPhysical2, SIGNAL(valueChanged(double)), this, SLOT(physical2Changed(double)), Qt::DirectConnection);
}

WidgetConfigDiscreteInputSlot::WidgetConfigDiscreteInputSlot(QSharedPointer<DiscreteInputSlot> __ioslot, QWidget *parent) : QWidget(parent),
    d_ioslot(__ioslot)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDiscreteInputSlot::slotNameChanged(const QString &__name)
{
    d_ioslot->setName(__name);
}

void WidgetConfigDiscreteInputSlot::pinChanged(int __pin)
{
    d_ioslot->setPin(__pin);
}

void WidgetConfigDiscreteInputSlot::inverseChanged(bool __inverse)
{
    d_ioslot->setInverse(__inverse);
}

void WidgetConfigDiscreteInputSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("Discrete input"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);
    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_lPin = new QLabel(tr("Pin:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());
    d_cbInverse = new QCheckBox(tr("Inverse"), this);
    d_cbInverse->setChecked(d_ioslot->inverse());
}

void WidgetConfigDiscreteInputSlot::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leSlotName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,       row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lSlotName,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lPin,            row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,           row, 1, 1, 2, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_cbInverse,       row, 0, 1, 4, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigDiscreteInputSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
    connect(d_cbInverse, SIGNAL(toggled(bool)), this, SLOT(inverseChanged(bool)), Qt::DirectConnection);
}

WidgetConfigDiscreteOutputSlot::WidgetConfigDiscreteOutputSlot(QSharedPointer<DiscreteOutputSlot> __ioslot, QWidget *parent) : QWidget(parent),
    d_ioslot(__ioslot)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDiscreteOutputSlot::slotNameChanged(const QString &__name)
{
    d_ioslot->setName(__name);
}

void WidgetConfigDiscreteOutputSlot::pinChanged(int __pin)
{
    d_ioslot->setPin(__pin);
}

void WidgetConfigDiscreteOutputSlot::inverseChanged(bool __inverse)
{
    d_ioslot->setInverse(__inverse);
}

void WidgetConfigDiscreteOutputSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("Discrete output"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);
    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_lPin = new QLabel(tr("Pin:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());
    d_cbInverse = new QCheckBox(tr("Inverse"), this);
    d_cbInverse->setChecked(d_ioslot->inverse());
}

void WidgetConfigDiscreteOutputSlot::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leSlotName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,       row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lSlotName,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lPin,            row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,           row, 1, 1, 2, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_cbInverse,       row, 0, 1, 4, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigDiscreteOutputSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
    connect(d_cbInverse, SIGNAL(toggled(bool)), this, SLOT(inverseChanged(bool)), Qt::DirectConnection);
}

WidgetConfigDHT22TemperatureSlot::WidgetConfigDHT22TemperatureSlot(QSharedPointer<DHT22TemperatureSlot> __ioslot, QWidget *parent) : QWidget(parent),
    d_ioslot(__ioslot)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDHT22TemperatureSlot::slotNameChanged(const QString &__name)
{
    d_ioslot->setName(__name);
}

void WidgetConfigDHT22TemperatureSlot::pinChanged(int __pin)
{
    d_ioslot->setPin(__pin);
}

void WidgetConfigDHT22TemperatureSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("DHT22 temperature"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);
    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_lPin = new QLabel(tr("Pin:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());
}

void WidgetConfigDHT22TemperatureSlot::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leSlotName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,       row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lSlotName,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lPin,            row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,           row, 1, 1, 2, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigDHT22TemperatureSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
}

WidgetConfigDHT22HumiditySlot::WidgetConfigDHT22HumiditySlot(QSharedPointer<DHT22HumiditySlot> __ioslot, QWidget *parent) : QWidget(parent),
    d_ioslot(__ioslot)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDHT22HumiditySlot::slotNameChanged(const QString &__name)
{
    d_ioslot->setName(__name);
}

void WidgetConfigDHT22HumiditySlot::pinChanged(int __pin)
{
    d_ioslot->setPin(__pin);
}

void WidgetConfigDHT22HumiditySlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("DHT22 humidity"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);
    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_lPin = new QLabel(tr("Pin:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());
}

void WidgetConfigDHT22HumiditySlot::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leSlotName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,       row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lSlotName,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lPin,            row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,           row, 1, 1, 2, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigDHT22HumiditySlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
}
