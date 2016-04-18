#include "ioslots_xml_composer.h"

#include <QVariant>

IoslotsXmlComposer::IoslotsXmlComposer(QObject *parent) : QObject(parent)
{

}

template<IoslotDriver t> struct IoslotDriverConv { static const char *toString() { return "Unknown"; } };
template<> struct IoslotDriverConv<EmptySlotDriver> { static const char *toString() { return "EmptySlot"; } };
template<> struct IoslotDriverConv<AnalogInputDriver> { static const char *toString() { return "AnalogInputSlot"; } };
template<> struct IoslotDriverConv<DiscreteInputDriver> { static const char *toString() { return "DiscreteInputSlot"; } };
template<> struct IoslotDriverConv<DiscreteOutputDriver> { static const char *toString() { return "DiscreteOutputSlot"; } };
template<> struct IoslotDriverConv<DHT22TemperatureDriver> { static const char *toString() { return "DHT22TemperatureSlot"; } };
template<> struct IoslotDriverConv<DHT22HumidityDriver> { static const char *toString() { return "DHT22HumiditySlot"; } };

IoslotsXmlComposerV1::IoslotsXmlComposerV1(QObject *parent) : IoslotsXmlComposer(parent)
{

}

const char *IoslotsXmlComposerV1::ioslotsTag = "ioslots";
const char *IoslotsXmlComposerV1::slotTag = "slot";
const char *IoslotsXmlComposerV1::driverAttr = "driver";
const char *IoslotsXmlComposerV1::idAttr = "id";
const char *IoslotsXmlComposerV1::nameAttr = "name";
const char *IoslotsXmlComposerV1::numAttr = "num";
const char *IoslotsXmlComposerV1::x1Attr = "x1";
const char *IoslotsXmlComposerV1::y1Attr = "y1";
const char *IoslotsXmlComposerV1::x2Attr = "x2";
const char *IoslotsXmlComposerV1::y2Attr = "y2";
const char *IoslotsXmlComposerV1::pinAttr = "pin";
const char *IoslotsXmlComposerV1::inverseAttr = "inverse";

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
            QString name = child.attribute(nameAttr);
            QString driver = child.attribute(driverAttr);
            if (driver.isEmpty())
                continue;

            if (driver == IoslotDriverConv<EmptySlotDriver>::toString()) {
                EmptySlot *emptySlot = new EmptySlot(id);
                emptySlot->setName(name);

                ioslots.append(QSharedPointer<Ioslot>(emptySlot));

            } else if (driver == IoslotDriverConv<AnalogInputDriver>::toString()) {
                AnalogInputSlot *analogInput = new AnalogInputSlot(id);
                int num = child.attribute(numAttr).toInt();
                float x1 = child.attribute(x1Attr).toFloat();
                float y1 = child.attribute(y1Attr).toFloat();
                float x2 = child.attribute(x2Attr).toFloat();
                float y2 = child.attribute(y2Attr).toFloat();

                analogInput->setName(name);
                analogInput->setNum(num);
                analogInput->setLinear(x1, y1, x2, y2);

                ioslots.append(QSharedPointer<Ioslot>(analogInput));

            } else if (driver == IoslotDriverConv<DiscreteInputDriver>::toString()) {
                DiscreteInputSlot *discreteInput = new DiscreteInputSlot(id);
                int pin = child.attribute(pinAttr).toInt();
                bool inverse =
                        child.attribute(inverseAttr) == QString("true")?
                            true:
                            false;

                discreteInput->setName(name);
                discreteInput->setPin(pin);
                discreteInput->setInverse(inverse);

                ioslots.append(QSharedPointer<Ioslot>(discreteInput));

            } else if (driver == IoslotDriverConv<DiscreteOutputDriver>::toString()) {
                DiscreteOutputSlot *discreteOutput = new DiscreteOutputSlot(id);
                int pin = child.attribute(pinAttr).toInt();
                bool inverse =
                        child.attribute(inverseAttr) == QString("true")?
                            true:
                            false;

                discreteOutput->setName(name);
                discreteOutput->setPin(pin);
                discreteOutput->setInverse(inverse);

                ioslots.append(QSharedPointer<Ioslot>(discreteOutput));

            } else if (driver == IoslotDriverConv<DHT22TemperatureDriver>::toString()) {
                DHT22TemperatureSlot *dht22Temperature = new DHT22TemperatureSlot(id);
                int pin = child.attribute(pinAttr).toInt();

                dht22Temperature->setName(name);
                dht22Temperature->setPin(pin);

                ioslots.append(QSharedPointer<Ioslot>(dht22Temperature));

            } else if (driver == IoslotDriverConv<DHT22HumidityDriver>::toString()) {
                DHT22HumiditySlot *dht22Humidity = new DHT22HumiditySlot(id);
                int pin = child.attribute(pinAttr).toInt();

                dht22Humidity->setName(name);
                dht22Humidity->setPin(pin);

                ioslots.append(QSharedPointer<Ioslot>(dht22Humidity));

            }
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
        child.setAttribute(nameAttr, ioslot->name());

        switch (ioslot->driver()) {
        case EmptySlotDriver:
        {
            child.setAttribute(driverAttr, IoslotDriverConv<EmptySlotDriver>::toString());
            break;
        }
        case AnalogInputDriver:
        {
            QSharedPointer<AnalogInputSlot> analogInput = IoslotConv::toSlot<AnalogInputSlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<AnalogInputDriver>::toString());
            child.setAttribute(numAttr, QString::number(analogInput->num()));
            child.setAttribute(x1Attr, QString::number(analogInput->x1()));
            child.setAttribute(y1Attr, QString::number(analogInput->y1()));
            child.setAttribute(x2Attr, QString::number(analogInput->x2()));
            child.setAttribute(y2Attr, QString::number(analogInput->y2()));
            break;
        }
        case DiscreteInputDriver:
        {
            QSharedPointer<DiscreteInputSlot> discreteInput = IoslotConv::toSlot<DiscreteInputSlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<DiscreteInputDriver>::toString());
            child.setAttribute(pinAttr, QString::number(discreteInput->pin()));
            QVariant v(discreteInput->inverse());
            child.setAttribute(inverseAttr, v.toString());
            break;
        }
        case DiscreteOutputDriver:
        {
            QSharedPointer<DiscreteOutputSlot> discreteOutput = IoslotConv::toSlot<DiscreteOutputSlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<DiscreteOutputDriver>::toString());
            child.setAttribute(pinAttr, QString::number(discreteOutput->pin()));
            QVariant v(discreteOutput->inverse());
            child.setAttribute(inverseAttr, v.toString());
            break;
        }
        case DHT22TemperatureDriver:
        {
            QSharedPointer<DHT22TemperatureSlot> dht22Temperature = IoslotConv::toSlot<DHT22TemperatureSlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<DHT22TemperatureDriver>::toString());
            child.setAttribute(pinAttr, QString::number(dht22Temperature->pin()));
            break;
        }
        case DHT22HumidityDriver:
        {
            QSharedPointer<DHT22HumiditySlot> dht22Humidity = IoslotConv::toSlot<DHT22HumiditySlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<DHT22HumidityDriver>::toString());
            child.setAttribute(pinAttr, QString::number(dht22Humidity->pin()));
            break;
        }
        default:
            break;
        }

        elem.appendChild(child);
    }

    return elem;
}