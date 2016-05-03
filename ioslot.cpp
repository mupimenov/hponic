#include "ioslot.h"

#include <math.h>

Ioslot::Ioslot(int id, int type, int driver, QObject *parent) :
    QObject(parent),
    d_id(id),
    d_type(type),
    d_driver(driver)
{

}

Ioslot::~Ioslot()
{

}

void Ioslot::setName(const QString &name)
{
    d_name = name;
    Q_EMIT changed(this);
}

const QString &Ioslot::name() const
{
    return d_name;
}

int Ioslot::id() const
{
    return d_id;
}

int Ioslot::type() const
{
    return d_type;
}

int Ioslot::driver() const
{
    return d_driver;
}


EmptySlot::EmptySlot(int id, QObject *parent) :
    Ioslot(id, UnknownIoslotType, EmptySlotDriver, parent)
{
    setName(tr("Empty slot"));
}

EmptySlot::~EmptySlot()
{

}

AnalogInputSlot::AnalogInputSlot(int id, QObject *parent) :
    Ioslot(id, AnalogInputType, AnalogInputDriver, parent),
    d_num(0),
    d_x1(0),
    d_x2(255),
    d_y1(0.0f),    
    d_y2(1.0f)
{
    setName(tr("Analog input slot"));
}

AnalogInputSlot::~AnalogInputSlot()
{

}

void AnalogInputSlot::setNum(int num)
{
    d_num = num;
    Q_EMIT changed(this);
}

int AnalogInputSlot::num() const
{
    return d_num;
}

void AnalogInputSlot::setLinear(quint16 x1, float y1, quint16 x2, float y2)
{
    d_x1 = x1;
    d_x2 = x2;
    d_y1 = y1;   
    d_y2 = y2;
    Q_EMIT changed(this);
}

quint16 AnalogInputSlot::x1() const
{
    return d_x1;
}


quint16 AnalogInputSlot::x2() const
{
    return d_x2;
}

float AnalogInputSlot::y1() const
{
    return d_y1;
}

float AnalogInputSlot::y2() const
{
    return d_y2;
}

DiscreteInputSlot::DiscreteInputSlot(int id, QObject *parent) :
    Ioslot(id, DiscreteInputType, DiscreteInputDriver, parent),
    d_pin(0),
    d_inverse(false)
{
    setName(tr("Discrete input slot"));
}

DiscreteInputSlot::~DiscreteInputSlot()
{

}

void DiscreteInputSlot::setPin(int pin)
{
    d_pin = pin;
    Q_EMIT changed(this);
}

int DiscreteInputSlot::pin() const
{
    return d_pin;
}

void DiscreteInputSlot::setInverse(bool inverse)
{
    d_inverse = inverse;
    Q_EMIT changed(this);
}

bool DiscreteInputSlot::inverse() const
{
    return d_inverse;
}

DiscreteOutputSlot::DiscreteOutputSlot(int id, QObject *parent) :
    Ioslot(id, DiscreteOutputType, DiscreteOutputDriver, parent),
    d_operation(LogicOr),
    d_pin(0),
    d_inverse(false)
{
    setName(tr("Discrete ouput slot"));
}

DiscreteOutputSlot::~DiscreteOutputSlot()
{

}

void DiscreteOutputSlot::setPin(int pin)
{
    d_pin = pin;
}

int DiscreteOutputSlot::pin() const
{
    return d_pin;
}

void DiscreteOutputSlot::setInverse(bool inverse)
{
    d_inverse = inverse;
    Q_EMIT changed(this);
}

bool DiscreteOutputSlot::inverse() const
{
    return d_inverse;
}

void DiscreteOutputSlot::setOperation(int operation)
{
    d_operation = operation;
}

int DiscreteOutputSlot::operation() const
{
    return d_operation;
}

DHTxxSlot::DHTxxSlot(int id, QObject *parent) :
    Ioslot(id, AnalogInputType, DHTxxDriver, parent),
    d_modification(DHT11),
    d_parameter(DHTxxTemperature),
    d_pin(0)
{
    setName(tr("DHTxx sensor"));
}

DHTxxSlot::~DHTxxSlot()
{

}

void DHTxxSlot::setModification(int modification)
{
    d_modification = modification;
    Q_EMIT changed(this);
}

int DHTxxSlot::modification() const
{
    return d_modification;
}

void DHTxxSlot::setParameter(int parameter)
{
    d_parameter = parameter;
    Q_EMIT changed(this);
}

int DHTxxSlot::parameter() const
{
    return d_parameter;
}

void DHTxxSlot::setPin(int pin)
{
    d_pin = pin;
    Q_EMIT changed(this);
}

int DHTxxSlot::pin() const
{
    return d_pin;
}

DallasTemperatureSlot::DallasTemperatureSlot(int id, QObject *parent) :
    Ioslot(id, AnalogInputType, DallasTemperatureDriver, parent),
    d_pin(0)
{
    setName(tr("Dallas temperature sensor"));
}

DallasTemperatureSlot::~DallasTemperatureSlot()
{

}

void DallasTemperatureSlot::setPin(int pin)
{
    d_pin = pin;
    Q_EMIT changed(this);
}

int DallasTemperatureSlot::pin() const
{
    return d_pin;
}
