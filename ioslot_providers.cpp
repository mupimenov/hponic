#include "ioslot_providers.h"

#include "widget_config_slot.h"

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

float arrayToFloat(const QByteArray &arr, int offset)
{
    quint32 v = ((quint8)arr[offset])
            + ((quint8)arr[offset + 1] << 8)
            + ((quint8)arr[offset + 2] << 16)
            + ((quint8)arr[offset + 3] << 24);
    float f;
    memcpy(&f, &v, sizeof(f));
    return f;
}

void floatToArray(float f, QByteArray &arr, int offset)
{
    quint32 v;
    memcpy(&v, &f, sizeof(v));
    arr[offset] = v & 0xFF;
    arr[offset + 1] = (v >> 8) & 0xFF;
    arr[offset + 2] = (v >> 16) & 0xFF;
    arr[offset + 3] = (v >> 24) & 0xFF;
}

/* EmptySlot */

EmptySlotEditorProvider::EmptySlotEditorProvider(QSharedPointer<EmptySlot> slot) :
    d_slot(slot) {}

EmptySlotEditorProvider::~EmptySlotEditorProvider() {}

QWidget *EmptySlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigEmptySlot(d_slot, hponic, parent));
}

EmptySlotBinProvider::EmptySlotBinProvider(QSharedPointer<EmptySlot> slot) :
    d_slot(slot) {}

EmptySlotBinProvider::~EmptySlotBinProvider() {}

void EmptySlotBinProvider::fromArray(const QByteArray &arr)
{
    Q_UNUSED(arr);
}

QByteArray EmptySlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();
    arr[1] = d_slot->id();
    return arr;
}

EmptySlotXmlProvider::EmptySlotXmlProvider(QSharedPointer<EmptySlot> slot) :
    d_slot(slot) {}

EmptySlotXmlProvider::~EmptySlotXmlProvider() {}

void EmptySlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    d_slot->setName(name);
}

void EmptySlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());
}

/* AnalogInputSlot */

AnalogInputSlotEditorProvider::AnalogInputSlotEditorProvider(QSharedPointer<AnalogInputSlot> slot) :
    d_slot(slot) {}

AnalogInputSlotEditorProvider::~AnalogInputSlotEditorProvider() {}

QWidget *AnalogInputSlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigAnalogInputSlot(d_slot, hponic, parent));
}

AnalogInputSlotBinProvider::AnalogInputSlotBinProvider(QSharedPointer<AnalogInputSlot> slot) :
    d_slot(slot) {}

AnalogInputSlotBinProvider::~AnalogInputSlotBinProvider() {}

void AnalogInputSlotBinProvider::fromArray(const QByteArray &arr)
{
    d_slot->setNum(arr[2]);                                 // 1
    quint16 x1 = quint8(arr[3]) + (quint8(arr[4]) << 8);    // 2
    quint16 x2 = quint8(arr[5]) + (quint8(arr[6]) << 8);    // 2
    float y1 = arrayToFloat(arr, 7);                        // 4
    float y2 = arrayToFloat(arr, 11);                       // 4

    d_slot->setLinear(x1, y1, x2, y2);
}

QByteArray AnalogInputSlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();            // 1
    arr[1] = d_slot->id();                // 1
    arr[2] = d_slot->num();               // 1
    arr[3] = d_slot->x1();                // 1
    arr[4] = d_slot->x1() >> 8;           // 1
    arr[5] = d_slot->x2();                // 1
    arr[6] = d_slot->x2() >> 8;           // 1
    floatToArray(d_slot->y1(), arr, 7);   // 4
    floatToArray(d_slot->y2(), arr, 11);  // 4
    return arr;
}

AnalogInputSlotXmlProvider::AnalogInputSlotXmlProvider(QSharedPointer<AnalogInputSlot> slot) :
    d_slot(slot) {}

AnalogInputSlotXmlProvider::~AnalogInputSlotXmlProvider() {}

void AnalogInputSlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int num = elem.attribute(numAttr).toInt();
    quint16 x1 = elem.attribute(x1Attr).toUInt();
    quint16 x2 = elem.attribute(x2Attr).toUInt();
    float y1 = elem.attribute(y1Attr).toFloat();
    float y2 = elem.attribute(y2Attr).toFloat();

    d_slot->setName(name);
    d_slot->setNum(num);
    d_slot->setLinear(x1, y1, x2, y2);
}

void AnalogInputSlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());
    elem.setAttribute(numAttr, QString::number(d_slot->num()));
    elem.setAttribute(x1Attr, QString::number(d_slot->x1()));
    elem.setAttribute(x2Attr, QString::number(d_slot->x2()));
    elem.setAttribute(y1Attr, QString::number(d_slot->y1()));
    elem.setAttribute(y2Attr, QString::number(d_slot->y2()));
}

/* DiscreteInputSlot */

DiscreteInputSlotEditorProvider::DiscreteInputSlotEditorProvider(QSharedPointer<DiscreteInputSlot> slot) :
    d_slot(slot) {}

DiscreteInputSlotEditorProvider::~DiscreteInputSlotEditorProvider() {}

QWidget *DiscreteInputSlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigDiscreteInputSlot(d_slot, hponic, parent));
}

DiscreteInputSlotBinProvider::DiscreteInputSlotBinProvider(QSharedPointer<DiscreteInputSlot> slot) :
    d_slot(slot) {}

DiscreteInputSlotBinProvider::~DiscreteInputSlotBinProvider() {}

void DiscreteInputSlotBinProvider::fromArray(const QByteArray &arr)
{
    d_slot->setPin(arr[2]);
    d_slot->setInverse(arr[3] == 0x01);
}

QByteArray DiscreteInputSlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();
    arr[1] = d_slot->id();
    arr[2] = d_slot->pin();
    arr[3] = d_slot->inverse()? 0x01: 0x00;
    return arr;
}

DiscreteInputSlotXmlProvider::DiscreteInputSlotXmlProvider(QSharedPointer<DiscreteInputSlot> slot) :
    d_slot(slot) {}

DiscreteInputSlotXmlProvider::~DiscreteInputSlotXmlProvider() {}

void DiscreteInputSlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int pin = elem.attribute(pinAttr).toInt();
    bool inverse =
            elem.attribute(inverseAttr) == QString("true")?
                true:
                false;

    d_slot->setName(name);
    d_slot->setPin(pin);
    d_slot->setInverse(inverse);
}

void DiscreteInputSlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());
    elem.setAttribute(pinAttr, QString::number(d_slot->pin()));
    QVariant v(d_slot->inverse());
    elem.setAttribute(inverseAttr, v.toString());
}

/* DiscreteOutputSlot */

DiscreteOutputSlotEditorProvider::DiscreteOutputSlotEditorProvider(QSharedPointer<DiscreteOutputSlot> slot) :
    d_slot(slot) {}

DiscreteOutputSlotEditorProvider::~DiscreteOutputSlotEditorProvider() {}

QWidget *DiscreteOutputSlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigDiscreteOutputSlot(d_slot, hponic, parent));
}

DiscreteOutputSlotBinProvider::DiscreteOutputSlotBinProvider(QSharedPointer<DiscreteOutputSlot> slot) :
    d_slot(slot) {}

DiscreteOutputSlotBinProvider::~DiscreteOutputSlotBinProvider() {}

void DiscreteOutputSlotBinProvider::fromArray(const QByteArray &arr)
{
    d_slot->setOperation(arr[2]);
    d_slot->setPin(arr[3]);
    d_slot->setInverse(arr[4] == 0x01);
}

QByteArray DiscreteOutputSlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();
    arr[1] = d_slot->id();
    arr[2] = d_slot->operation();
    arr[3] = d_slot->pin();
    arr[4] = d_slot->inverse()? 0x01: 0x00;
    return arr;
}

DiscreteOutputSlotXmlProvider::DiscreteOutputSlotXmlProvider(QSharedPointer<DiscreteOutputSlot> slot) :
    d_slot(slot) {}

DiscreteOutputSlotXmlProvider::~DiscreteOutputSlotXmlProvider() {}

void DiscreteOutputSlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    QString op = elem.attribute(operationAttr);
    if (op == DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicOr>::toString())
        d_slot->setOperation(DiscreteOutputSlot::LogicOr);
    else
        d_slot->setOperation(DiscreteOutputSlot::LogicAnd);
    int pin = elem.attribute(pinAttr).toInt();
    bool inverse =
            elem.attribute(inverseAttr) == QString("true")?
                true:
                false;

    d_slot->setName(name);
    d_slot->setPin(pin);
    d_slot->setInverse(inverse);
}

void DiscreteOutputSlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());
    if (d_slot->operation() == DiscreteOutputSlot::LogicOr)
        elem.setAttribute(operationAttr, DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicOr>::toString());
    else
        elem.setAttribute(operationAttr, DiscreteOutputSlotLogicOperationConv<DiscreteOutputSlot::LogicAnd>::toString());
    elem.setAttribute(pinAttr, QString::number(d_slot->pin()));
    QVariant v(d_slot->inverse());
    elem.setAttribute(inverseAttr, v.toString());
}

/* DHTxxSlot */

DHTxxSlotEditorProvider::DHTxxSlotEditorProvider(QSharedPointer<DHTxxSlot> slot) :
    d_slot(slot) {}

DHTxxSlotEditorProvider::~DHTxxSlotEditorProvider() {}

QWidget *DHTxxSlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigDHTxxSlot(d_slot, hponic, parent));
}

DHTxxSlotBinProvider::DHTxxSlotBinProvider(QSharedPointer<DHTxxSlot> slot) :
    d_slot(slot) {}

DHTxxSlotBinProvider::~DHTxxSlotBinProvider() {}

void DHTxxSlotBinProvider::fromArray(const QByteArray &arr)
{
    d_slot->setModification(arr[2]);
    d_slot->setParameter(arr[3]);
    d_slot->setPin(arr[4]);
}

QByteArray DHTxxSlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();
    arr[1] = d_slot->id();
    arr[2] = d_slot->modification();
    arr[3] = d_slot->parameter();
    arr[4] = d_slot->pin();
    return arr;
}

DHTxxSlotXmlProvider::DHTxxSlotXmlProvider(QSharedPointer<DHTxxSlot> slot) :
    d_slot(slot) {}

DHTxxSlotXmlProvider::~DHTxxSlotXmlProvider() {}

void DHTxxSlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    QString modification = elem.attribute(modificationAttr);
    QString parameter = elem.attribute(parameterAttr);
    int pin = elem.attribute(pinAttr).toInt();

    d_slot->setName(name);

    if (modification == DHTxxModificationConv<DHT11>::toString())
        d_slot->setModification(DHT11);
    else
        d_slot->setModification(DHT22);

    if (parameter == DHTxxParameterConv<DHTxxTemperature>::toString())
        d_slot->setParameter(DHTxxTemperature);
    else
        d_slot->setParameter(DHTxxHumidity);

    d_slot->setPin(pin);
}

void DHTxxSlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());

    if (d_slot->modification() == DHT11)
        elem.setAttribute(modificationAttr, DHTxxModificationConv<DHT11>::toString());
    else
        elem.setAttribute(modificationAttr, DHTxxModificationConv<DHT22>::toString());

    if (d_slot->parameter() == DHTxxTemperature)
        elem.setAttribute(parameterAttr, DHTxxParameterConv<DHTxxTemperature>::toString());
    else
        elem.setAttribute(parameterAttr, DHTxxParameterConv<DHTxxHumidity>::toString());

    elem.setAttribute(pinAttr, QString::number(d_slot->pin()));
}

/* DallasTemperatureSlot */

DallasTemperatureSlotEditorProvider::DallasTemperatureSlotEditorProvider(QSharedPointer<DallasTemperatureSlot> slot) :
    d_slot(slot) {}

DallasTemperatureSlotEditorProvider::~DallasTemperatureSlotEditorProvider() {}

QWidget *DallasTemperatureSlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigDallasTemperatureSlot(d_slot, hponic, parent));
}

DallasTemperatureSlotBinProvider::DallasTemperatureSlotBinProvider(QSharedPointer<DallasTemperatureSlot> slot) :
    d_slot(slot) {}

DallasTemperatureSlotBinProvider::~DallasTemperatureSlotBinProvider() {}

void DallasTemperatureSlotBinProvider::fromArray(const QByteArray &arr)
{
    d_slot->setPin(arr[2]);
}

QByteArray DallasTemperatureSlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();
    arr[1] = d_slot->id();
    arr[2] = d_slot->pin();
    return arr;
}

DallasTemperatureSlotXmlProvider::DallasTemperatureSlotXmlProvider(QSharedPointer<DallasTemperatureSlot> slot) :
    d_slot(slot) {}

DallasTemperatureSlotXmlProvider::~DallasTemperatureSlotXmlProvider() {}

void DallasTemperatureSlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int pin = elem.attribute(pinAttr).toInt();

    d_slot->setName(name);
    d_slot->setPin(pin);
}

void DallasTemperatureSlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());
    elem.setAttribute(pinAttr, QString::number(d_slot->pin()));
}

/* MHZ19Slot */

MHZ19SlotEditorProvider::MHZ19SlotEditorProvider(QSharedPointer<MHZ19Slot> slot) :
    d_slot(slot) {}

MHZ19SlotEditorProvider::~MHZ19SlotEditorProvider() {}

QWidget *MHZ19SlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigMHZ19Slot(d_slot, hponic, parent));
}

MHZ19SlotBinProvider::MHZ19SlotBinProvider(QSharedPointer<MHZ19Slot> slot) :
    d_slot(slot) {}

MHZ19SlotBinProvider::~MHZ19SlotBinProvider() {}

void MHZ19SlotBinProvider::fromArray(const QByteArray &arr)
{
    d_slot->setReceivePin(arr[2]);
    d_slot->setTransmitPin(arr[3]);
}

QByteArray MHZ19SlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();
    arr[1] = d_slot->id();
    arr[2] = d_slot->receivePin();
    arr[3] = d_slot->transmitPin();
    return arr;
}

MHZ19SlotXmlProvider::MHZ19SlotXmlProvider(QSharedPointer<MHZ19Slot> slot) :
    d_slot(slot) {}

MHZ19SlotXmlProvider::~MHZ19SlotXmlProvider() {}

void MHZ19SlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int receivePin = elem.attribute(receivePinAttr).toInt();
    int transmitPin = elem.attribute(transmitPinAttr).toInt();

    d_slot->setName(name);
    d_slot->setReceivePin(receivePin);
    d_slot->setTransmitPin(transmitPin);
}

void MHZ19SlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());
    elem.setAttribute(receivePinAttr, QString::number(d_slot->receivePin()));
    elem.setAttribute(transmitPinAttr, QString::number(d_slot->transmitPin()));
}

/* SHT2xSlot */

SHT2xSlotEditorProvider::SHT2xSlotEditorProvider(QSharedPointer<SHT2xSlot> slot) :
    d_slot(slot) {}

SHT2xSlotEditorProvider::~SHT2xSlotEditorProvider() {}

QWidget *SHT2xSlotEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigSHT2xSlot(d_slot, hponic, parent));
}

SHT2xSlotBinProvider::SHT2xSlotBinProvider(QSharedPointer<SHT2xSlot> slot) :
    d_slot(slot) {}

SHT2xSlotBinProvider::~SHT2xSlotBinProvider() {}

void SHT2xSlotBinProvider::fromArray(const QByteArray &arr)
{
    d_slot->setParameter(arr[2]);
    d_slot->setSdaPin(arr[3]);
    d_slot->setSclPin(arr[4]);
}

QByteArray SHT2xSlotBinProvider::toArray()
{
    QByteArray arr(16, char(0));
    arr[0] = d_slot->driver();
    arr[1] = d_slot->id();
    arr[2] = d_slot->parameter();
    arr[3] = d_slot->sdaPin();
    arr[4] = d_slot->sclPin();
    return arr;
}

SHT2xSlotXmlProvider::SHT2xSlotXmlProvider(QSharedPointer<SHT2xSlot> slot) :
    d_slot(slot) {}

SHT2xSlotXmlProvider::~SHT2xSlotXmlProvider() {}

void SHT2xSlotXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int parameter = elem.attribute(parameterAttr).toInt();
    int sdaPin = elem.attribute(sdaPinAttr).toInt();
    int sclPin = elem.attribute(sclPinAttr).toInt();

    d_slot->setName(name);
    d_slot->setParameter(parameter);
    d_slot->setSdaPin(sdaPin);
    d_slot->setSclPin(sclPin);
}

void SHT2xSlotXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_slot->name());
    elem.setAttribute(parameterAttr, QString::number(d_slot->parameter()));
    elem.setAttribute(sdaPinAttr, QString::number(d_slot->sdaPin()));
    elem.setAttribute(sclPinAttr, QString::number(d_slot->sclPin()));
}
