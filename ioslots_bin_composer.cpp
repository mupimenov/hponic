#include "ioslots_bin_composer.h"

IoslotsBinComposer::IoslotsBinComposer(QObject *parent) : QObject(parent)
{

}


IoslotsBinComposerV1::IoslotsBinComposerV1(QObject *parent) : IoslotsBinComposer(parent)
{

}

void floatToArray(float f, QByteArray &slot, int offset)
{
    quint32 v;
    memcpy(&v, &f, sizeof(v));
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
            slot[3] = analogInput->x1();                // 1
            slot[4] = analogInput->x1() >> 8;           // 1
            slot[5] = analogInput->x2();                // 1
            slot[6] = analogInput->x2() >> 8;           // 1
            floatToArray(analogInput->y1(), slot, 7);   // 4
            floatToArray(analogInput->y2(), slot, 11);  // 4

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
        case DHTxxDriver:
        {
            QSharedPointer<DHTxxSlot> dhtxx = IoslotConv::toSlot<DHTxxSlot>(ioslot);
            slot[2] = dhtxx->modification();
            slot[3] = dhtxx->parameter();
            slot[4] = dhtxx->pin();

            break;
        }
        case DallasTemperatureDriver:
        {
            QSharedPointer<DallasTemperatureSlot> dallasTemperature = IoslotConv::toSlot<DallasTemperatureSlot>(ioslot);
            slot[2] = dallasTemperature->pin();
            break;
        }
        case MhZ19Driver:
        {
            QSharedPointer<MhZ19Slot> mhZ19 = IoslotConv::toSlot<MhZ19Slot>(ioslot);
            slot[2] = mhZ19->receivePin();
            slot[3] = mhZ19->transmitPin();
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
    quint32 v = ((quint8)slot.at(offset))
            + ((quint8)slot.at(offset + 1) << 8)
            + ((quint8)slot.at(offset + 2) << 16)
            + ((quint8)slot.at(offset + 3) << 24);
    float f;
    memcpy(&f, &v, sizeof(f));
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
            quint16 x1 = quint8(slot.at(3)) + (quint8(slot.at(4)) << 8); // 2
            quint16 x2 = quint8(slot.at(5)) + (quint8(slot.at(6)) << 8); // 2
            float y1 = arrayToFloat(slot, 7); // 4
            float y2 = arrayToFloat(slot, 11); // 4

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
        case DHTxxDriver:
        {
            DHTxxSlot *dhtxx = new DHTxxSlot(slot.at(1));
            dhtxx->setModification(slot.at(2));
            dhtxx->setParameter(slot.at(3));
            dhtxx->setPin(slot.at(4));

            ioslots.append(QSharedPointer<Ioslot>(dhtxx));
            break;
        }
        case DallasTemperatureDriver:
        {
            DallasTemperatureSlot *dallasTemperature = new DallasTemperatureSlot(slot.at(1));
            dallasTemperature->setPin(slot.at(2));

            ioslots.append(QSharedPointer<Ioslot>(dallasTemperature));
            break;
        }
        case MhZ19Driver:
        {
            MhZ19Slot *mhZ19 = new MhZ19Slot(slot.at(1));
            mhZ19->setReceivePin(slot.at(2));
            mhZ19->setTransmitPin(slot.at(3));

            ioslots.append(QSharedPointer<Ioslot>(mhZ19));
            break;
        }
        default:
            break;
        }
    }

    return ioslots;
}
