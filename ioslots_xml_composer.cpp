#include "ioslots_xml_composer.h"

#include <QVariant>

#include "ioslot_manager.h"
#include "ioslot_providers.h"

static const char *ioslotsTag = "ioslots";
static const char *slotTag = "slot";
static const char *driverAttr = "driver";
static const char *idAttr = "id";

template<IoslotDriver t> struct IoslotDriverConv { static const char *toString() { return "Unknown"; } };
template<> struct IoslotDriverConv<EmptySlotDriver> { static const char *toString() { return "EmptySlot"; } };
template<> struct IoslotDriverConv<AnalogInputDriver> { static const char *toString() { return "AnalogInputSlot"; } };
template<> struct IoslotDriverConv<DiscreteInputDriver> { static const char *toString() { return "DiscreteInputSlot"; } };
template<> struct IoslotDriverConv<DiscreteOutputDriver> { static const char *toString() { return "DiscreteOutputSlot"; } };
template<> struct IoslotDriverConv<DHTxxDriver> { static const char *toString() { return "DHTxxSlot"; } };
template<> struct IoslotDriverConv<DallasTemperatureDriver> { static const char *toString() { return "DallasTemperatureSlot"; } };
template<> struct IoslotDriverConv<MHZ19Driver> { static const char *toString() { return "MHZ19Slot"; } };
template<> struct IoslotDriverConv<SHT2xDriver> { static const char *toString() { return "SHT2xSlot"; } };

IoslotsXmlComposer::IoslotsXmlComposer(QObject *parent) :
    QObject(parent) {}

IoslotsXmlComposerV1::IoslotsXmlComposerV1(QObject *parent) :
    IoslotsXmlComposer(parent) {}

QList<QSharedPointer<Ioslot> > IoslotsXmlComposerV1::fromElement(QDomElement &root)
{
    QList<QSharedPointer<Ioslot> > ioslots;

    QDomElement elem;
    QDomElement child = root.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == ioslotsTag) {
            elem = child;
            break;
        }
        child = child.nextSiblingElement();
    }

    if (elem.isNull()) return ioslots;

    child = elem.firstChildElement();
    while (!child.isNull()) {
        QString tagName = child.tagName();
        if (tagName == slotTag) {
            int id = child.attribute(idAttr).toInt();
            QString driver = child.attribute(driverAttr);
            if (driver.isEmpty())
                continue;

            QSharedPointer<Ioslot> ioslot;
            if (driver == IoslotDriverConv<AnalogInputDriver>::toString())
                ioslot = IoslotManager::createAnalogInputSlot(id);
            else if (driver == IoslotDriverConv<DiscreteInputDriver>::toString())
                ioslot = IoslotManager::createDiscreteInputSlot(id);
            else if (driver == IoslotDriverConv<DiscreteOutputDriver>::toString())
                ioslot = IoslotManager::createDiscreteOutputSlot(id);
            else if (driver == IoslotDriverConv<DHTxxDriver>::toString())
                ioslot = IoslotManager::createDHTxxSlot(id);
            else if (driver == IoslotDriverConv<DallasTemperatureDriver>::toString())
                ioslot = IoslotManager::createDallasTemperatureSlot(id);
            else if (driver == IoslotDriverConv<MHZ19Driver>::toString())
                ioslot = IoslotManager::createMHZ19Slot(id);
            else if (driver == IoslotDriverConv<SHT2xDriver>::toString())
                ioslot = IoslotManager::createSHT2xSlot(id);
            else
                ioslot = IoslotManager::createEmptySlot(id);

            ioslot->providers()->xmlProvider()->fromElement(child);

            ioslots.append(ioslot);
        }
        child = child.nextSibling().toElement();
    }

    return ioslots;
}

QDomElement IoslotsXmlComposerV1::toElement(const QList<QSharedPointer<Ioslot> > &ioslots, QDomDocument &doc)
{
    QDomElement elem = doc.createElement(ioslotsTag);

    for (int i = 0; i < ioslots.size(); ++i) {
        QSharedPointer<Ioslot> ioslot = ioslots[i];
        if (!ioslot)
            continue;

        QDomElement child = doc.createElement(slotTag);
        child.setAttribute(idAttr, QString::number(ioslot->id()));

        switch (ioslot->driver()) {
        case AnalogInputDriver:
            child.setAttribute(driverAttr, IoslotDriverConv<AnalogInputDriver>::toString());
            break;
        case DiscreteInputDriver:
            child.setAttribute(driverAttr, IoslotDriverConv<DiscreteInputDriver>::toString());
            break;
        case DiscreteOutputDriver:
            child.setAttribute(driverAttr, IoslotDriverConv<DiscreteOutputDriver>::toString());
            break;
        case DHTxxDriver:
            child.setAttribute(driverAttr, IoslotDriverConv<DHTxxDriver>::toString());
            break;
        case DallasTemperatureDriver:
            child.setAttribute(driverAttr, IoslotDriverConv<DallasTemperatureDriver>::toString());
            break;
        case MHZ19Driver:
            child.setAttribute(driverAttr, IoslotDriverConv<MHZ19Driver>::toString());
            break;
        case SHT2xDriver:
            child.setAttribute(driverAttr, IoslotDriverConv<SHT2xDriver>::toString());
            break;
        case EmptySlotDriver:
        default:
            child.setAttribute(driverAttr, IoslotDriverConv<EmptySlotDriver>::toString());
            break;
        }

        ioslot->providers()->xmlProvider()->toElement(child);

        elem.appendChild(child);
    }

    return elem;
}
