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
    d_lSlotType = new QLabel(tr("<b>Empty slot</b>"), this);
    d_lStep0 = new QLabel(tr("Select new slot type and configure slot parameters"), this);
}

void WidgetConfigEmptySlot::createLayouts()
{
    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,  row, 0, 1, 1, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,     row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutControls);
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
    d_lCurrentAdcValue->setText(QString::number(d_hponic->monitoring()->adcValue(d_sbNum->value())));
}

#define SLOT_NAME_MINIMUM_WIDTH 320

void WidgetConfigAnalogInputSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("<b>Analog input</b>"), this);

    d_lStep0 = new QLabel(tr("<b>Step 0</b>. Enter the slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_leSlotName->setMinimumWidth(SLOT_NAME_MINIMUM_WIDTH);

    d_lStep1 = new QLabel(tr("<b>Step 1</b>. Enter the number of analog input:"), this);
    d_sbNum = new QSpinBox(this);
    d_sbNum->setRange(0, 15);
    d_sbNum->setValue(d_ioslot->num());

    d_lStep2 = new QLabel(tr("<b>Step 2</b>. Calibrate analog input:"), this);
    d_lADC = new QLabel(tr("ADC:"), this);
    d_lCurrentAdcValue = new QLabel(this);
    d_sbADC1 = new QSpinBox(this);
    d_sbADC1->setRange(0, 1023);
    d_sbADC1->setValue(d_ioslot->x1());
    d_sbADC2 = new QSpinBox(this);
    d_sbADC2->setRange(0, 1023);
    d_sbADC2->setValue(d_ioslot->x2());
    d_lPhysical = new QLabel(tr("Physical value:"), this);
    d_dsbPhysical1 = new QDoubleSpinBox(this);
    d_dsbPhysical1->setRange(-10000.0, 10000.0);
    d_dsbPhysical1->setValue(d_ioslot->y1());
    d_dsbPhysical2 = new QDoubleSpinBox(this);
    d_dsbPhysical2->setRange(-10000.0, 10000.0);
    d_dsbPhysical2->setValue(d_ioslot->y2());
}

void WidgetConfigAnalogInputSlot::createLayouts()
{
    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,          row, 0, 1, 5, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,             row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_leSlotName,         row, 1, 1, 4, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep1,             row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbNum,              row, 1, 1, 4, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep2,             row, 0, 3, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_lPhysical,          row, 1, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_dsbPhysical1,       row, 2, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_dsbPhysical2,       row, 3, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lADC,               row, 1, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbADC1,             row, 2, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbADC2,             row, 3, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCurrentAdcValue,   row, 2, 1, 1, Qt::AlignTop | Qt::AlignLeft);

    layoutControls->setColumnStretch(4, 1);
    layoutControls->setRowStretch(5, 1);

    QVBoxLayout *layoutMain = new QVBoxLayout;    
    layoutMain->addLayout(layoutControls);
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
    d_lSlotType = new QLabel(tr("<b>Discrete input</b>"), this);

    d_lStep0 = new QLabel(tr("<b>Step 0</b>. Enter the slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_leSlotName->setMinimumWidth(SLOT_NAME_MINIMUM_WIDTH);

    d_lStep1 = new QLabel(tr("<b>Step 1</b>. Enter the pin number:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());

    d_lStep2 = new QLabel(tr("<b>Step 2</b>. Check input inversion flag:"), this);
    d_cbInverse = new QCheckBox(tr("Inverse"), this);
    d_cbInverse->setChecked(d_ioslot->inverse());
}

void WidgetConfigDiscreteInputSlot::createLayouts()
{

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,      row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_leSlotName,     row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep1,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,          row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep2,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_cbInverse,      row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);

    layoutControls->setColumnStretch(3, 1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutControls);
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
    d_lSlotType = new QLabel(tr("<b>Discrete output</b>"), this);

    d_lStep0 = new QLabel(tr("<b>Step 0</b>. Enter the slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_leSlotName->setMinimumWidth(SLOT_NAME_MINIMUM_WIDTH);

    d_lStep1 = new QLabel(tr("<b>Step 1</b>. Select output logic function:"), this);
    d_cbOperation = new QComboBox(this);
    d_cbOperation->addItem(tr("OR"), QVariant(DiscreteOutputSlot::LogicOr));
    d_cbOperation->addItem(tr("AND"), QVariant(DiscreteOutputSlot::LogicAnd));
    d_cbOperation->setCurrentIndex(d_ioslot->operation());

    d_lStep2 = new QLabel(tr("<b>Step 2</b>. Enter the pin number:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());

    d_lStep3 = new QLabel(tr("<b>Step 3</b>. Check output inversion flag:"), this);
    d_cbInverse = new QCheckBox(tr("Inverse"), this);
    d_cbInverse->setChecked(d_ioslot->inverse());
}

void WidgetConfigDiscreteOutputSlot::createLayouts()
{
    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,      row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_leSlotName,     row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep1,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_cbOperation,    row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep2,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,          row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep3,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_cbInverse,      row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);

    layoutControls->setColumnStretch(3, 1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutControls);
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
    d_lSlotType = new QLabel(tr("<b>DHT11/DHT22 sensor</b>"), this);

    d_lStep0 = new QLabel(tr("<b>Step 0</b>. Enter the slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_leSlotName->setMinimumWidth(SLOT_NAME_MINIMUM_WIDTH);

    d_lStep1 = new QLabel(tr("<b>Step 1</b>. Select sensor modification:"), this);
    d_cbModification = new QComboBox(this);
    d_cbModification->addItem(tr("DHT11"), QVariant(DHT11));
    d_cbModification->addItem(tr("DHT22"), QVariant(DHT22));
    d_cbModification->setCurrentIndex(d_ioslot->modification());

    d_lStep2 = new QLabel(tr("<b>Step 2</b>. Select measured parameter:"), this);
    d_cbParameter = new QComboBox(this);
    d_cbParameter->addItem(tr("Temperature"), QVariant(DHTxxTemperature));
    d_cbParameter->addItem(tr("Humidity"), QVariant(DHTxxHumidity));
    d_cbParameter->setCurrentIndex(d_ioslot->parameter());

    d_lStep3 = new QLabel(tr("<b>Step 3</b>. Enter the pin number:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());
}

void WidgetConfigDHTxxSlot::createLayouts()
{
    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,      row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_leSlotName,     row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep1,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_cbModification, row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep2,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_cbParameter,    row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep3,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,          row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);

    layoutControls->setColumnStretch(3, 1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutControls);
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
    d_lSlotType = new QLabel(tr("<b>Dallas temperature sensor</b>"), this);

    d_lStep0 = new QLabel(tr("<b>Step 0</b>. Enter the slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_leSlotName->setMinimumWidth(SLOT_NAME_MINIMUM_WIDTH);

    d_lStep1 = new QLabel(tr("<b>Step 1</b>. Enter the pin number:"), this);
    d_sbPin = new QSpinBox(this);
    d_sbPin->setRange(0, 53);
    d_sbPin->setValue(d_ioslot->pin());
}

void WidgetConfigDallasTemperatureSlot::createLayouts()
{
    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,      row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_leSlotName,     row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep1,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbPin,          row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);

    layoutControls->setColumnStretch(3, 1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutControls);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigDallasTemperatureSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbPin, SIGNAL(valueChanged(int)), this, SLOT(pinChanged(int)), Qt::DirectConnection);
}

WidgetConfigMHZ19Slot::WidgetConfigMHZ19Slot(QSharedPointer<MHZ19Slot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigMHZ19Slot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigMHZ19Slot::receivePinChanged(int pin)
{
    d_ioslot->setReceivePin(pin);
}

void WidgetConfigMHZ19Slot::transmitPinChanged(int pin)
{
    d_ioslot->setTransmitPin(pin);
}

void WidgetConfigMHZ19Slot::createWidgets()
{
    d_lSlotType = new QLabel(tr("<b>MH-Z19 sensor</b>"), this);

    d_lStep0 = new QLabel(tr("<b>Step 0</b>. Enter the slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_leSlotName->setMinimumWidth(SLOT_NAME_MINIMUM_WIDTH);

    d_lStep1 = new QLabel(tr("<b>Step 1</b>. Enter the RX pin number:"), this);
    d_sbReceivePin = new QSpinBox(this);
    d_sbReceivePin->setRange(0, 69);
    d_sbReceivePin->setValue(d_ioslot->receivePin());

    d_lStep2 = new QLabel(tr("<b>Step 2</b>. Enter the TX pin number:"), this);
    d_sbTransmitPin = new QSpinBox(this);
    d_sbTransmitPin->setRange(0, 69);
    d_sbTransmitPin->setValue(d_ioslot->transmitPin());
}

void WidgetConfigMHZ19Slot::createLayouts()
{
    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,      row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_leSlotName,     row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep1,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbReceivePin,   row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep2,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbTransmitPin,  row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);

    layoutControls->setColumnStretch(3, 1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutControls);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigMHZ19Slot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_sbReceivePin, SIGNAL(valueChanged(int)), this, SLOT(receivePinChanged(int)), Qt::DirectConnection);
    connect(d_sbTransmitPin, SIGNAL(valueChanged(int)), this, SLOT(transmitPinChanged(int)), Qt::DirectConnection);
}

WidgetConfigSHT2xSlot::WidgetConfigSHT2xSlot(QSharedPointer<SHT2xSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    d_ioslot(ioslot),
    d_hponic(hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigSHT2xSlot::slotNameChanged(const QString &name)
{
    d_ioslot->setName(name);
}

void WidgetConfigSHT2xSlot::parameterChanged(int index)
{
    int par = d_cbParameter->itemData(index).toInt();
    d_ioslot->setParameter(par);
}

void WidgetConfigSHT2xSlot::sdaPinChanged(int pin)
{
    d_ioslot->setSdaPin(pin);
}

void WidgetConfigSHT2xSlot::sclPinChanged(int pin)
{
    d_ioslot->setSclPin(pin);
}

void WidgetConfigSHT2xSlot::createWidgets()
{
    d_lSlotType = new QLabel(tr("<b>SHT2x sensor</b>"), this);

    d_lStep0 = new QLabel(tr("<b>Step 0</b>. Enter the slot name:"), this);
    d_leSlotName = new QLineEdit(d_ioslot->name(), this);
    d_leSlotName->setMinimumWidth(SLOT_NAME_MINIMUM_WIDTH);

    d_lStep1 = new QLabel(tr("<b>Step 1</b>. Select measured parameter:"), this);
    d_cbParameter = new QComboBox(this);
    d_cbParameter->addItem(tr("Temperature"), QVariant(SHT2xTemperature));
    d_cbParameter->addItem(tr("Humidity"), QVariant(SHT2xHumidity));
    d_cbParameter->setCurrentIndex(d_ioslot->parameter());

    d_lStep2 = new QLabel(tr("<b>Step 2</b>. Enter the SDA pin number:"), this);
    d_sbSdaPin = new QSpinBox(this);
    d_sbSdaPin->setRange(0, 69);
    d_sbSdaPin->setValue(d_ioslot->sdaPin());

    d_lStep3 = new QLabel(tr("<b>Step 3</b>. Enter the SCL pin number:"), this);
    d_sbSclPin = new QSpinBox(this);
    d_sbSclPin->setRange(0, 69);
    d_sbSclPin->setValue(d_ioslot->sclPin());
}

void WidgetConfigSHT2xSlot::createLayouts()
{
    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lSlotType,      row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lStep0,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_leSlotName,     row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep1,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_cbParameter,    row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep2,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbSdaPin,       row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lStep3,         row, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layoutControls->addWidget(d_sbSclPin,       row, 1, 1, 3, Qt::AlignTop | Qt::AlignLeft);

    layoutControls->setColumnStretch(3, 1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutControls);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigSHT2xSlot::createConnections()
{
    connect(d_leSlotName, SIGNAL(textChanged(QString)), this, SLOT(slotNameChanged(QString)), Qt::DirectConnection);
    connect(d_cbParameter, SIGNAL(currentIndexChanged(int)), this, SLOT(parameterChanged(int)), Qt::DirectConnection);
    connect(d_sbSdaPin, SIGNAL(valueChanged(int)), this, SLOT(sdaPinChanged(int)), Qt::DirectConnection);
    connect(d_sbSclPin, SIGNAL(valueChanged(int)), this, SLOT(sclPinChanged(int)), Qt::DirectConnection);
}
