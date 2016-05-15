#include "widget_config_slot.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

WidgetConfigEmptySlot::WidgetConfigEmptySlot(QSharedPointer<EmptySlot> ioslot, QSharedPointer<Hponic> hponic,
                                             QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
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

WidgetConfigAnalogInputSlot::WidgetConfigAnalogInputSlot(QSharedPointer<AnalogInputSlot> ioslot,
                                                         QSharedPointer<Hponic> hponic,
                                                         QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigAnalogInputSlot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigAnalogInputSlot::numChanged(int num)
{
    d_ioslot->setNum(num);
}

void WidgetConfigAnalogInputSlot::adc1Changed(int adc1)
{
    d_ioslot->setLinear(adc1, d_ioslot->y1(), d_ioslot->x2(), d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::adc2Changed(int adc2)
{
    d_ioslot->setLinear(d_ioslot->x1(), d_ioslot->y1(), adc2, d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::physical1Changed(double physical1)
{
    d_ioslot->setLinear(d_ioslot->x1(), physical1, d_ioslot->x2(), d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::physical2Changed(double physical2)
{
    d_ioslot->setLinear(d_ioslot->x1(), d_ioslot->y1(), d_ioslot->x2(), physical2);
}

void WidgetConfigAnalogInputSlot::adcValueChanged()
{
    d_lValue->setText(QString::number(d_hponic->monitoring()->adcValue(d_sbNum->value())));
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
    d_lValue = new QLabel(this);
    d_sbADC1 = new QSpinBox(this);
    d_sbADC1->setRange(0, 1023);
    d_sbADC1->setValue(d_ioslot->x1());
    d_sbADC2 = new QSpinBox(this);
    d_sbADC2->setRange(0, 1023);
    d_sbADC2->setValue(d_ioslot->x2());
    d_lPhysical = new QLabel(tr("Physical:"), this);
    d_dsbPhysical1 = new QDoubleSpinBox(this);
    d_dsbPhysical1->setRange(-10000.0, 10000.0);
    d_dsbPhysical1->setValue(d_ioslot->y1());
    d_dsbPhysical2 = new QDoubleSpinBox(this);
    d_dsbPhysical2->setRange(-10000.0, 10000.0);
    d_dsbPhysical2->setValue(d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leSlotName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,       row, 0, 1, 5, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lSlotName,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 4, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lNum,            row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbNum,           row, 1, 1, 4, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCalibration,    row, 0, 1, 5, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lADC,            row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbADC1,          row, 2, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbADC2,          row, 3, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_lValue,          row, 4, 1, 1, Qt::AlignLeft);
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

    connect(d_hponic->monitoring().data(), SIGNAL(adcValuesUpdated()), this, SLOT(adcValueChanged()), Qt::DirectConnection);
}

WidgetConfigDiscreteInputSlot::WidgetConfigDiscreteInputSlot(QSharedPointer<DiscreteInputSlot> ioslot,
                                                             QSharedPointer<Hponic> hponic,
                                                             QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDiscreteInputSlot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigDiscreteInputSlot::pinChanged(int pin)
{
    d_ioslot->setPin(pin);
}

void WidgetConfigDiscreteInputSlot::inverseChanged(bool inverse)
{
    d_ioslot->setInverse(inverse);
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

WidgetConfigDiscreteOutputSlot::WidgetConfigDiscreteOutputSlot(QSharedPointer<DiscreteOutputSlot> ioslot,
                                                               QSharedPointer<Hponic> hponic,
                                                               QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDiscreteOutputSlot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigDiscreteOutputSlot::operationChanged(int index)
{
    int op = d_cbOperation->itemData(index).toInt();
    d_ioslot->setOperation(op);
}

void WidgetConfigDiscreteOutputSlot::pinChanged(int pin)
{
    d_ioslot->setPin(pin);
}

void WidgetConfigDiscreteOutputSlot::inverseChanged(bool inverse)
{
    d_ioslot->setInverse(inverse);
}

void WidgetConfigDiscreteOutputSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("Discrete output"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);

    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);

    d_lOperation = new QLabel(tr("Logic operation:"), this);
    d_cbOperation = new QComboBox(this);
    d_cbOperation->addItem(tr("OR"), QVariant(DiscreteOutputSlot::LogicOr));
    d_cbOperation->addItem(tr("AND"), QVariant(DiscreteOutputSlot::LogicAnd));
    d_cbOperation->setCurrentIndex(d_ioslot->operation());

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
    layoutControls->addWidget(d_lOperation,      row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbOperation,     row, 1, 1, 2, Qt::AlignLeft);
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
    connect(d_cbOperation, SIGNAL(currentIndexChanged(int)), this, SLOT(operationChanged(int)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
    connect(d_cbInverse, SIGNAL(toggled(bool)), this, SLOT(inverseChanged(bool)), Qt::DirectConnection);
}

WidgetConfigDHTxxSlot::WidgetConfigDHTxxSlot(QSharedPointer<DHTxxSlot> ioslot,
                                                                   QSharedPointer<Hponic> hponic,
                                                                   QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDHTxxSlot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigDHTxxSlot::modificationChanged(int index)
{
    int mod = d_cbModification->itemData(index).toInt();
    d_ioslot->setModification(mod);
}

void WidgetConfigDHTxxSlot::parameterChanged(int index)
{
    int par = d_cbParameter->itemData(index).toInt();
    d_ioslot->setParameter(par);
}

void WidgetConfigDHTxxSlot::pinChanged(int pin)
{
    d_ioslot->setPin(pin);
}

void WidgetConfigDHTxxSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("DHTxx"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);

    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);

    d_lModification = new QLabel(tr("Modification:"), this);
    d_cbModification = new QComboBox(this);
    d_cbModification->addItem(tr("DHT11"), QVariant(DHT11));
    d_cbModification->addItem(tr("DHT22"), QVariant(DHT22));
    d_cbModification->setCurrentIndex(d_ioslot->modification());

    d_lParameter = new QLabel(tr("Parameter:"), this);
    d_cbParameter = new QComboBox(this);
    d_cbParameter->addItem(tr("Temperature"), QVariant(DHTxxTemperature));
    d_cbParameter->addItem(tr("Humidity"), QVariant(DHTxxHumidity));
    d_cbParameter->setCurrentIndex(d_ioslot->parameter());

    d_lPin = new QLabel(tr("Pin:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());
}

void WidgetConfigDHTxxSlot::createLayouts()
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
    layoutControls->addWidget(d_lModification,   row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbModification,  row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lParameter,      row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbParameter,     row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lPin,            row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,           row, 1, 1, 3, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigDHTxxSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_cbModification, SIGNAL(currentIndexChanged(int)), this, SLOT(modificationChanged(int)), Qt::DirectConnection);
    connect(d_cbParameter, SIGNAL(currentIndexChanged(int)), this, SLOT(parameterChanged(int)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
}

WidgetConfigDallasTemperatureSlot::WidgetConfigDallasTemperatureSlot(QSharedPointer<DallasTemperatureSlot> ioslot,
                                                                     QSharedPointer<Hponic> hponic,
                                                                     QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigDallasTemperatureSlot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigDallasTemperatureSlot::pinChanged(int pin)
{
    d_ioslot->setPin(pin);
}

void WidgetConfigDallasTemperatureSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("Dallas temperature"), this);
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

void WidgetConfigDallasTemperatureSlot::createLayouts()
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

void WidgetConfigDallasTemperatureSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
}

WidgetConfigMhZ19Slot::WidgetConfigMhZ19Slot(QSharedPointer<MhZ19Slot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigMhZ19Slot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigMhZ19Slot::receivePinChanged(int pin)
{
    d_ioslot->setReceivePin(pin);
}

void WidgetConfigMhZ19Slot::transmitPinChanged(int pin)
{
    d_ioslot->setTransmitPin(pin);
}

void WidgetConfigMhZ19Slot::createWidgets()
{
    d_lSlotType = new QLabel(tr("MH-Z19 driver"), this);
    QFont f = d_lSlotType->font();
    f.setBold(true);
    d_lSlotType->setFont(f);

    d_lSlotName = new QLabel(tr("Slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);

    d_lReceivePin = new QLabel(tr("Receive pin:"), this);
    d_sbReceivePin = new QSpinBox(this);
    d_sbReceivePin->setRange(0, 53);
    d_sbReceivePin->setValue(d_ioslot->receivePin());

    d_lTransmitPin = new QLabel(tr("Transmit pin:"), this);
    d_sbTransmitPin = new QSpinBox(this);
    d_sbTransmitPin->setRange(0, 53);
    d_sbTransmitPin->setValue(d_ioslot->transmitPin());
}

void WidgetConfigMhZ19Slot::createLayouts()
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
    layoutControls->addWidget(d_lReceivePin,     row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbReceivePin,    row, 1, 1, 2, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lTransmitPin,     row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbTransmitPin,    row, 1, 1, 2, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigMhZ19Slot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbReceivePin, SIGNAL(valueChanged(int)), this, SLOT(receivePinChanged(int)), Qt::DirectConnection);
    connect(d_sbTransmitPin, SIGNAL(valueChanged(int)), this, SLOT(transmitPinChanged(int)), Qt::DirectConnection);
}
