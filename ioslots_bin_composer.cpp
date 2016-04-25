#include "ioslots_bin_composer.h"

IoslotsBinComposer::IoslotsBinComposer(QObject *parent) : QObject(parent)
{

}


IoslotsBinComposerV1::IoslotsBinComposerV1(QObject *parent) : IoslotsBinComposer(parent)
{

}

void floatToArray(float f, QByteArray &slot, int offset)
{
    quint32 v = *((quint32*)&f);
    slot[offset] = v & 0xFF;
    slot[offset + 1] = (v >> 8) & 0xFF;
    slot[offset + 2] = (v >> 16) & 0xFF;
    slot[offset + 3] = (v >> 24) & 0xFF;
}

QByteArray IoslotsBinComposerV1::toArray(const QList<QSharedPointer<Ioslot> > &ioslots)
{
    QByteArray arr;

    for (int i = 0; i < ioslots.size(); ++i) {
        QSharedPointer<Ioslot> ioslot = ioslots[i];
        if (!ioslot)
            continue;

        QByteArray slot(16, char(0));
        slot[0] = ioslot->driver();
        slot[1] = ioslot->id();

        switch (ioslot->driver()) {
        case EmptySlotDriver:
        {
            break;
        }
        case AnalogInputDriver:
        {
            QSharedPointer<AnalogInputSlot> analogInput = IoslotConv::toSlot<AnalogInputSlot>(ioslot);
            slot[2] = analogInput->num();               // 1
            floatToArray(analogInput->k(), slot, 3);    // 4
            floatToArray(analogInput->b(), slot, 7);    // 4

            break;
        }
        case DiscreteInputDriver:
        {
            QSharedPointer<DiscreteInputSlot> discreteInput = IoslotConv::toSlot<DiscreteInputSlot>(ioslot);
            slot[2] = discreteInput->pin();
            slot[3] = discreteInput->inverse()? 0x01: 0x00;

            break;
        }
        case DiscreteOutputDriver:
        {
            QSharedPointer<DiscreteOutputSlot> discreteOutput = IoslotConv::toSlot<DiscreteOutputSlot>(ioslot);
            slot[2] = discreteOutput->operation();
            slot[3] = discreteOutput->pin();
            slot[4] = discreteOutput->inverse()? 0x01: 0x00;

            break;
        }
        case DHT22TemperatureDriver:
        {
            QSharedPointer<DHT22TemperatureSlot> dht22Temperature = IoslotConv::toSlot<DHT22TemperatureSlot>(ioslot);
            slot[2] = dht22Temperature->pin();

            break;
        }
        case DHT22HumidityDriver:
        {
            QSharedPointer<DHT22HumiditySlot> dht22Humidity = IoslotConv::toSlot<DHT22HumiditySlot>(ioslot);
            slot[2] = dht22Humidity->pin();

            break;
        }
        case DallasTemperatureDriver:
        {
            QSharedPointer<DallasTemperatureSlot> dallasTemperature = IoslotConv::toSlot<DallasTemperatureSlot>(ioslot);
            slot[2] = dallasTemperature->pin();
            break;
        }
        default:
            break;
        }

        arr.append(slot);
    }

    return arr;
}

float arrayToFloat(QByteArray &slot, int offset)
{
    quint32 v = ((quint32)slot.at(offset))
            + ((quint32)slot.at(offset + 1) << 8)
            + ((quint32)slot.at(offset + 2) << 16)
            + ((quint32)slot.at(offset + 3) << 24);
    float f = *((float*)&v);
    return f;
}

QList<QSharedPointer<Ioslot> > IoslotsBinComposerV1::fromArray(const QByteArray &arr)
{
    const int chunk = 16;

    QList<QSharedPointer<Ioslot> > ioslots;
    for (int offset = 0; offset + chunk <= arr.size(); offset += chunk) {
        QByteArray slot = arr.mid(offset, chunk);

        switch (slot.at(0)) {
        case EmptySlotDriver:
        {
            EmptySlot *emptySlot = new EmptySlot(slot.at(1));
            ioslots.append(QSharedPointer<Ioslot>(emptySlot));
            break;
        }
        case AnalogInputDriver:
        {
            AnalogInputSlot *analogInput = new AnalogInputSlot(slot.at(1));
            analogInput->setNum(slot.at(2)); // 1
            float k = arrayToFloat(slot, 3); // 4
            float b = arrayToFloat(slot, 7); // 4

            const float x1 = 0.0f;
            const float x2 = 1024.0f;
            float y1 = k * x1 + b;
            float y2 = k * x2 + b;

            analogInput->setLinear(x1, y1, x2, y2);

            ioslots.append(QSharedPointer<Ioslot>(analogInput));
            break;
        }
        case DiscreteInputDriver:
        {
            DiscreteInputSlot *discreteInput = new DiscreteInputSlot(slot.at(1));
            discreteInput->setPin(slot.at(2));
            discreteInput->setInverse(slot.at(3) == 0x01);

            ioslots.append(QSharedPointer<Ioslot>(discreteInput));
            break;
        }
        case DiscreteOutputDriver:
        {
            DiscreteOutputSlot *discreteOutput = new DiscreteOutputSlot(slot.at(1));
            discreteOutput->setOperation(slot.at(2));
            discreteOutput->setPin(slot.at(3));
            discreteOutput->setInverse(slot.at(4) == 0x01);

            ioslots.append(QSharedPointer<Ioslot>(discreteOutput));
            break;
        }
        case DHT22TemperatureDriver:
        {
            DHT22TemperatureSlot *dht22Temperature = new DHT22TemperatureSlot(slot.at(1));
            dht22Temperature->setPin(slot.at(2));

            ioslots.append(QSharedPointer<Ioslot>(dht22Temperature));
            break;
        }
        case DHT22HumidityDriver:
        {
            DHT22HumiditySlot *dht22Humidity = new DHT22HumiditySlot(slot.at(1));
            dht22Humidity->setPin(slot.at(2));

            ioslots.append(QSharedPointer<Ioslot>(dht22Humidity));
            break;
        }
        case DallasTemperatureDriver:
        {
            DallasTemperatureSlot *dallasTemperature = new DallasTemperatureSlot(slot.at(1));
            dallasTemperature->setPin(slot.at(2));

            ioslots.append(QSharedPointer<Ioslot>(dallasTemperature));
            break;
        }
        default:
            break;
        }
    }

    return ioslots;
}
