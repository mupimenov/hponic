#include "ioslot.h"

#include <math.h>

Ioslot::Ioslot(int __id, int __type, int __driver, QObject *parent) : QObject(parent),
    d_id(__id),
    d_type(__type),
    d_driver(__driver)
{

}

Ioslot::~Ioslot()
{

}

void Ioslot::setName(const QString &__name)
{
    d_name = __name;
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


EmptySlot::EmptySlot(int __id, QObject *parent) : Ioslot(__id, UnknownIoslotType, EmptySlotDriver, parent)
{
    setName(tr("Empty slot"));
}

EmptySlot::~EmptySlot()
{

}

AnalogInputSlot::AnalogInputSlot(int __id, QObject *parent) : Ioslot(__id, AnalogInputType, AnalogInputDriver, parent),
    d_num(0),
    d_x1(0.0f),
    d_y1(0.0f),
    d_x2(1.0f),
    d_y2(1.0f)
{
    setName(tr("Analog input slot"));
}

AnalogInputSlot::~AnalogInputSlot()
{

}

void AnalogInputSlot::setNum(int __num)
{
    d_num = __num;
    Q_EMIT changed(this);
}

int AnalogInputSlot::num() const
{
    return d_num;
}

void AnalogInputSlot::setLinear(float __x1, float __y1, float __x2, float __y2)
{
    d_x1 = __x1;
    d_y1 = __y1;
    d_x2 = __x2;
    d_y2 = __y2;
    Q_EMIT changed(this);
}

float AnalogInputSlot::x1() const
{
    return d_x1;
}

float AnalogInputSlot::y1() const
{
    return d_y1;
}

float AnalogInputSlot::x2() const
{
    return d_x2;
}

float AnalogInputSlot::y2() const
{
    return d_y2;
}

float AnalogInputSlot::k() const
{
    float c = d_x2 - d_x1;
    if (c != 0.0f)
        return ((d_y2 - d_y1) / c);
    return NAN;
}

float AnalogInputSlot::b() const
{
    float c = d_x2 - d_x1;
    if (c != 0.0f) {
        float k = ((d_y2 - d_y1) / c);
        return (d_y2 - k * d_x2);
    }
    return NAN;
}

DiscreteInputSlot::DiscreteInputSlot(int __id, QObject *parent) : Ioslot(__id, DiscreteInputType, DiscreteInputDriver, parent),
    d_pin(0),
    d_inverse(false)
{
    setName(tr("Discrete input slot"));
}

DiscreteInputSlot::~DiscreteInputSlot()
{

}

void DiscreteInputSlot::setPin(int __pin)
{
    d_pin = __pin;
    Q_EMIT changed(this);
}

int DiscreteInputSlot::pin() const
{
    return d_pin;
}

void DiscreteInputSlot::setInverse(bool __inverse)
{
    d_inverse = __inverse;
    Q_EMIT changed(this);
}

bool DiscreteInputSlot::inverse() const
{
    return d_inverse;
}

DiscreteOutputSlot::DiscreteOutputSlot(int __id, QObject *parent) : Ioslot(__id, DiscreteOutputType, DiscreteOutputDriver, parent),
    d_pin(0),
    d_inverse(false)
{
    setName(tr("Discrete ouput slot"));
}

DiscreteOutputSlot::~DiscreteOutputSlot()
{

}

void DiscreteOutputSlot::setPin(int __pin)
{
    d_pin = __pin;
}

int DiscreteOutputSlot::pin() const
{
    return d_pin;
}

void DiscreteOutputSlot::setInverse(bool __inverse)
{
    d_inverse = __inverse;
    Q_EMIT changed(this);
}

bool DiscreteOutputSlot::inverse() const
{
    return d_inverse;
}

DHT22TemperatureSlot::DHT22TemperatureSlot(int __id, QObject *parent) : Ioslot(__id, AnalogInputType, DHT22TemperatureDriver, parent),
    d_pin(0)
{
    setName(tr("DHT22 temperature sensor"));
}

DHT22TemperatureSlot::~DHT22TemperatureSlot()
{

}

void DHT22TemperatureSlot::setPin(int __pin)
{
    d_pin = __pin;
    Q_EMIT changed(this);
}

int DHT22TemperatureSlot::pin() const
{
    return d_pin;
}

DHT22HumiditySlot::DHT22HumiditySlot(int __id, QObject *parent) : Ioslot(__id, AnalogInputType, DHT22HumidityDriver, parent),
    d_pin(0)
{
    setName(tr("DHT22 humidity sensor"));
}

DHT22HumiditySlot::~DHT22HumiditySlot()
{

}

void DHT22HumiditySlot::setPin(int __pin)
{
    d_pin = __pin;
    Q_EMIT changed(this);
}

int DHT22HumiditySlot::pin() const
{
    return d_pin;
}
