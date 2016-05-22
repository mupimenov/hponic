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
template<> struct IoslotDriverConv<DHTxxDriver> { static const char *toString() { return "DHTxxSlot"; } };
template<> struct IoslotDriverConv<DallasTemperatureDriver> { static const char *toString() { return "DallasTemperatureSlot"; } };
template<> struct IoslotDriverConv<MHZ19Driver> { static const char *toString() { return "MHZ19Slot"; } };
template<> struct IoslotDriverConv<SHT2xDriver> { static const char *toString() { return "SHT2xSlot"; } };

template<DiscreteOutputSlot::LogicOperation o> struct DiscreteOutputSlotLogicOperationConv { static const char *toString() { return "Unknown"; } };
template<> struct DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicOr> { static const char *toString() { return "OR"; } };
template<> struct DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicAnd> { static const char *toString() { return "AND"; } };

template<DHTxxModification m> struct DHTxxModificationConv { static const char *toString() { return "Unknown"; } };
template<> struct DHTxxModificationConv<DHT11> { static const char *toString() { return "DHT11"; } };
template<> struct DHTxxModificationConv<DHT22> { static const char *toString() { return "DHT22"; } };

template<DHTxxParameter p> struct DHTxxParameterConv { static const char *toString() { return "Unknown"; } };
template<> struct DHTxxParameterConv<DHTxxTemperature> { static const char *toString() { return "Temperature"; } };
template<> struct DHTxxParameterConv<DHTxxHumidity> { static const char *toString() { return "Humidity"; } };

template<SHT2xParameter p> struct SHT2xParameterConv { static const char *toString() { return "Unknown"; } };
template<> struct SHT2xParameterConv<SHT2xTemperature> { static const char *toString() { return "Temperature"; } };
template<> struct SHT2xParameterConv<SHT2xHumidity> { static const char *toString() { return "Humidity"; } };

IoslotsXmlComposerV1::IoslotsXmlComposerV1(QObject *parent) : IoslotsXmlComposer(parent)
{

}

static const char *ioslotsTag = "ioslots";
static const char *slotTag = "slot";
static const char *driverAttr = "driver";
static const char *idAttr = "id";
static const char *nameAttr = "name";
static const char *numAttr = "num";
static const char *x1Attr = "x1";
static const char *y1Attr = "y1";
static const char *x2Attr = "x2";
static const char *y2Attr = "y2";
static const char *pinAttr = "pin";
static const char *inverseAttr = "inverse";
static const char *operationAttr = "operation";
static const char *modificationAttr = "modification";
static const char *parameterAttr = "parameter";
static const char *receivePinAttr = "receivePin";
static const char *transmitPinAttr = "transmitPin";
static const char *sdaPinAttr = "sdaPin";
static const char *sclPinAttr = "sclPin";

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
                quint16 x1 = child.attribute(x1Attr).toUInt();
                quint16 x2 = child.attribute(x2Attr).toUInt();
                float y1 = child.attribute(y1Attr).toFloat();                
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
                QString op = child.attribute(operationAttr);
                if (op == DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicOr>::toString())
                    discreteOutput->setOperation(DiscreteOutputSlot::LogicOr);
                else
                    discreteOutput->setOperation(DiscreteOutputSlot::LogicAnd);
                int pin = child.attribute(pinAttr).toInt();
                bool inverse =
                        child.attribute(inverseAttr) == QString("true")?
                            true:
                            false;

                discreteOutput->setName(name);
                discreteOutput->setPin(pin);
                discreteOutput->setInverse(inverse);

                ioslots.append(QSharedPointer<Ioslot>(discreteOutput));

            } else if (driver == IoslotDriverConv<DHTxxDriver>::toString()) {
                DHTxxSlot *dhtxx = new DHTxxSlot(id);
                QString modification = child.attribute(modificationAttr);
                QString parameter = child.attribute(parameterAttr);
                int pin = child.attribute(pinAttr).toInt();

                dhtxx->setName(name);

                if (modification == DHTxxModificationConv<DHT11>::toString())
                    dhtxx->setModification(DHT11);
                else
                    dhtxx->setModification(DHT22);

                if (parameter == DHTxxParameterConv<DHTxxTemperature>::toString())
                    dhtxx->setParameter(DHTxxTemperature);
                else
                    dhtxx->setParameter(DHTxxHumidity);

                dhtxx->setPin(pin);

                ioslots.append(QSharedPointer<Ioslot>(dhtxx));

            } else if (driver == IoslotDriverConv<DallasTemperatureDriver>::toString()) {
                DallasTemperatureSlot *dallasTemperature = new DallasTemperatureSlot(id);
                int pin = child.attribute(pinAttr).toInt();

                dallasTemperature->setName(name);
                dallasTemperature->setPin(pin);

                ioslots.append(QSharedPointer<Ioslot>(dallasTemperature));

            } else if (driver == IoslotDriverConv<MHZ19Driver>::toString()) {
                MHZ19Slot *mhZ19 = new MHZ19Slot(id);
                int receivePin = child.attribute(receivePinAttr).toInt();
                int transmitPin = child.attribute(transmitPinAttr).toInt();

                mhZ19->setName(name);
                mhZ19->setReceivePin(receivePin);
                mhZ19->setTransmitPin(transmitPin);

                ioslots.append(QSharedPointer<Ioslot>(mhZ19));

            } else if (driver == IoslotDriverConv<SHT2xDriver>::toString()) {
                SHT2xSlot *sht2x = new SHT2xSlot(id);
                int parameter = child.attribute(parameterAttr).toInt();
                int sdaPin = child.attribute(sdaPinAttr).toInt();
                int sclPin = child.attribute(sclPinAttr).toInt();

                sht2x->setName(name);
                sht2x->setParameter(parameter);
                sht2x->setSdaPin(sdaPin);
                sht2x->setSclPin(sclPin);

                ioslots.append(QSharedPointer<Ioslot>(sht2x));

            } else {
                EmptySlot *emptySlot = new EmptySlot(id);
                ioslots.append(QSharedPointer<Ioslot>(emptySlot));
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
            child.setAttribute(x2Attr, QString::number(analogInput->x2()));
            child.setAttribute(y1Attr, QString::number(analogInput->y1()));            
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
            if (discreteOutput->operation() == DiscreteOutputSlot::LogicOr)
                child.setAttribute(operationAttr, DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicOr>::toString());
            else
                child.setAttribute(operationAttr, DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicAnd>::toString());
            child.setAttribute(pinAttr, QString::number(discreteOutput->pin()));
            QVariant v(discreteOutput->inverse());
            child.setAttribute(inverseAttr, v.toString());
            break;
        }
        case DHTxxDriver:
        {
            QSharedPointer<DHTxxSlot> dhtxx = IoslotConv::toSlot<DHTxxSlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<DHTxxDriver>::toString());

            if (dhtxx->modification() == DHT11)
                child.setAttribute(modificationAttr, DHTxxModificationConv<DHT11>::toString());
            else
                child.setAttribute(modificationAttr, DHTxxModificationConv<DHT22>::toString());

            if (dhtxx->parameter() == DHTxxTemperature)
                child.setAttribute(parameterAttr, DHTxxParameterConv<DHTxxTemperature>::toString());
            else
                child.setAttribute(parameterAttr, DHTxxParameterConv<DHTxxHumidity>::toString());

            child.setAttribute(pinAttr, QString::number(dhtxx->pin()));
            break;
        }
        case DallasTemperatureDriver:
        {
            QSharedPointer<DallasTemperatureSlot> dallasTemperature = IoslotConv::toSlot<DallasTemperatureSlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<DallasTemperatureDriver>::toString());
            child.setAttribute(pinAttr, QString::number(dallasTemperature->pin()));
            break;
        }
        case MHZ19Driver:
        {
            QSharedPointer<MHZ19Slot> mhZ19 = IoslotConv::toSlot<MHZ19Slot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<MHZ19Driver>::toString());
            child.setAttribute(receivePinAttr, QString::number(mhZ19->receivePin()));
            child.setAttribute(transmitPinAttr, QString::number(mhZ19->transmitPin()));
            break;
        }
        case SHT2xDriver:
        {
            QSharedPointer<SHT2xSlot> sht2x = IoslotConv::toSlot<SHT2xSlot>(ioslot);
            child.setAttribute(driverAttr, IoslotDriverConv<SHT2xDriver>::toString());
            child.setAttribute(parameterAttr, QString::number(sht2x->parameter()));
            child.setAttribute(sdaPinAttr, QString::number(sht2x->sdaPin()));
            child.setAttribute(sclPinAttr, QString::number(sht2x->sclPin()));
            break;
        }
        default:
            break;
        }

        elem.appendChild(child);
    }

    return elem;
}
