#include "ioslots_bin_composer.h"

#include "ioslot_manager.h"
#include "ioslot_providers.h"

IoslotsBinComposer::IoslotsBinComposer(QObject *parent) :
    QObject(parent) {}


IoslotsBinComposerV1::IoslotsBinComposerV1(QObject *parent) :
    IoslotsBinComposer(parent) {}

QByteArray IoslotsBinComposerV1::toArray(const QList<QSharedPointer<Ioslot> > &ioslots)
{
    QByteArray arr;

    for (int i = 0; i < ioslots.size(); ++i) {
        QSharedPointer<Ioslot> ioslot = ioslots[i];
        if (!ioslot)
            continue;

        arr.append(ioslot->providers()->binProvider()->toArray());
    }

    return arr;
}

QList<QSharedPointer<Ioslot> > IoslotsBinComposerV1::fromArray(const QByteArray &arr)
{
    const int chunk = 16;

    QList<QSharedPointer<Ioslot> > ioslots;
    for (int offset = 0; offset + chunk <= arr.size(); offset += chunk) {
        QByteArray slotArr = arr.mid(offset, chunk);
        QSharedPointer<Ioslot> ioslot;

        switch (slotArr[0]) {
        case AnalogInputDriver:
            ioslot = IoslotManager::createAnalogInputSlot(slotArr[1]);
            break;
        case DiscreteInputDriver:
            ioslot = IoslotManager::createDiscreteInputSlot(slotArr[1]);
            break;
        case DiscreteOutputDriver:
            ioslot = IoslotManager::createDiscreteOutputSlot(slotArr[1]);
            break;
        case DHTxxDriver:
            ioslot = IoslotManager::createDHTxxSlot(slotArr[1]);
            break;
        case DallasTemperatureDriver:
            ioslot = IoslotManager::createDallasTemperatureSlot(slotArr[1]);
            break;
        case MHZ19Driver:
            ioslot = IoslotManager::createMHZ19Slot(slotArr[1]);
            break;
        case SHT2xDriver:
            ioslot = IoslotManager::createSHT2xSlot(slotArr[1]);
            break;
        case EmptySlotDriver:
        default:
            ioslot = IoslotManager::createEmptySlot(slotArr[1]);
        }

        ioslot->providers()->binProvider()->fromArray(slotArr);

        ioslots.append(ioslot);
    }

    return ioslots;
}
