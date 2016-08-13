#ifndef IOSLOTPROVIDERS_H
#define IOSLOTPROVIDERS_H

#include <QByteArray>
#include <QSharedPointer>
#include <QList>

#include <QWidget>

#include <QDomDocument>
#include <QDomElement>

class Ioslot;
class Hponic;

class EmptySlot;
class AnalogInputSlot;
class DiscreteInputSlot;
class DiscreteOutputSlot;
class DHTxxSlot;
class DallasTemperatureSlot;
class MHZ19Slot;
class SHT2xSlot;

float arrayToFloat(const QByteArray &arr, int offset);
void floatToArray(float f, QByteArray &arr, int offset);

/* Basic */

class IoslotEditorProvider
{
public:
    virtual ~IoslotEditorProvider() {}

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0) = 0;

};

class IoslotBinProvider
{
public:
    virtual ~IoslotBinProvider() {}

    virtual QByteArray toArray() = 0;
    virtual void fromArray(const QByteArray &arr) = 0;

};

class IoslotXmlProvider
{
public:
    virtual ~IoslotXmlProvider() {}

    virtual void toElement(QDomElement &elem) = 0;
    virtual void fromElement(QDomElement &elem) = 0;

};

/* EmptySlot */

class EmptySlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit EmptySlotEditorProvider(QSharedPointer<EmptySlot> slot);
    virtual ~EmptySlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<EmptySlot> d_slot;
};

class EmptySlotBinProvider : public IoslotBinProvider
{
public:
    explicit EmptySlotBinProvider(QSharedPointer<EmptySlot> slot);
    virtual ~EmptySlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<EmptySlot> d_slot;
};

class EmptySlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit EmptySlotXmlProvider(QSharedPointer<EmptySlot> slot);
    virtual ~EmptySlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<EmptySlot> d_slot;
};

/* AnalogInputSlot */

class AnalogInputSlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit AnalogInputSlotEditorProvider(QSharedPointer<AnalogInputSlot> slot);
    virtual ~AnalogInputSlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<AnalogInputSlot> d_slot;
};

class AnalogInputSlotBinProvider : public IoslotBinProvider
{
public:
    explicit AnalogInputSlotBinProvider(QSharedPointer<AnalogInputSlot> slot);
    virtual ~AnalogInputSlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<AnalogInputSlot> d_slot;
};

class AnalogInputSlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit AnalogInputSlotXmlProvider(QSharedPointer<AnalogInputSlot> slot);
    virtual ~AnalogInputSlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<AnalogInputSlot> d_slot;
};

/* DiscreteInputSlot */

class DiscreteInputSlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit DiscreteInputSlotEditorProvider(QSharedPointer<DiscreteInputSlot> slot);
    virtual ~DiscreteInputSlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<DiscreteInputSlot> d_slot;
};

class DiscreteInputSlotBinProvider : public IoslotBinProvider
{
public:
    explicit DiscreteInputSlotBinProvider(QSharedPointer<DiscreteInputSlot> slot);
    virtual ~DiscreteInputSlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<DiscreteInputSlot> d_slot;
};

class DiscreteInputSlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit DiscreteInputSlotXmlProvider(QSharedPointer<DiscreteInputSlot> slot);
    virtual ~DiscreteInputSlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<DiscreteInputSlot> d_slot;
};

/* DiscreteOutputSlot */

class DiscreteOutputSlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit DiscreteOutputSlotEditorProvider(QSharedPointer<DiscreteOutputSlot> slot);
    virtual ~DiscreteOutputSlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<DiscreteOutputSlot> d_slot;
};

class DiscreteOutputSlotBinProvider : public IoslotBinProvider
{
public:
    explicit DiscreteOutputSlotBinProvider(QSharedPointer<DiscreteOutputSlot> slot);
    virtual ~DiscreteOutputSlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<DiscreteOutputSlot> d_slot;
};

class DiscreteOutputSlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit DiscreteOutputSlotXmlProvider(QSharedPointer<DiscreteOutputSlot> slot);
    virtual ~DiscreteOutputSlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<DiscreteOutputSlot> d_slot;
};

/* DHTxxSlot */

class DHTxxSlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit DHTxxSlotEditorProvider(QSharedPointer<DHTxxSlot> slot);
    virtual ~DHTxxSlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<DHTxxSlot> d_slot;
};

class DHTxxSlotBinProvider : public IoslotBinProvider
{
public:
    explicit DHTxxSlotBinProvider(QSharedPointer<DHTxxSlot> slot);
    virtual ~DHTxxSlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<DHTxxSlot> d_slot;
};

class DHTxxSlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit DHTxxSlotXmlProvider(QSharedPointer<DHTxxSlot> slot);
    virtual ~DHTxxSlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<DHTxxSlot> d_slot;
};

/* DallasTemperatureSlot */

class DallasTemperatureSlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit DallasTemperatureSlotEditorProvider(QSharedPointer<DallasTemperatureSlot> slot);
    virtual ~DallasTemperatureSlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<DallasTemperatureSlot> d_slot;
};

class DallasTemperatureSlotBinProvider : public IoslotBinProvider
{
public:
    explicit DallasTemperatureSlotBinProvider(QSharedPointer<DallasTemperatureSlot> slot);
    virtual ~DallasTemperatureSlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<DallasTemperatureSlot> d_slot;
};

class DallasTemperatureSlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit DallasTemperatureSlotXmlProvider(QSharedPointer<DallasTemperatureSlot> slot);
    virtual ~DallasTemperatureSlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<DallasTemperatureSlot> d_slot;
};

/* MHZ19Slot */

class MHZ19SlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit MHZ19SlotEditorProvider(QSharedPointer<MHZ19Slot> slot);
    virtual ~MHZ19SlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<MHZ19Slot> d_slot;
};

class MHZ19SlotBinProvider : public IoslotBinProvider
{
public:
    explicit MHZ19SlotBinProvider(QSharedPointer<MHZ19Slot> slot);
    virtual ~MHZ19SlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<MHZ19Slot> d_slot;
};

class MHZ19SlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit MHZ19SlotXmlProvider(QSharedPointer<MHZ19Slot> slot);
    virtual ~MHZ19SlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<MHZ19Slot> d_slot;
};

/* SHT2xSlot */

class SHT2xSlotEditorProvider : public IoslotEditorProvider
{
public:
    explicit SHT2xSlotEditorProvider(QSharedPointer<SHT2xSlot> slot);
    virtual ~SHT2xSlotEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<SHT2xSlot> d_slot;
};

class SHT2xSlotBinProvider : public IoslotBinProvider
{
public:
    explicit SHT2xSlotBinProvider(QSharedPointer<SHT2xSlot> slot);
    virtual ~SHT2xSlotBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<SHT2xSlot> d_slot;
};

class SHT2xSlotXmlProvider : public IoslotXmlProvider
{
public:
    explicit SHT2xSlotXmlProvider(QSharedPointer<SHT2xSlot> slot);
    virtual ~SHT2xSlotXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<SHT2xSlot> d_slot;
};

#endif // IOSLOTPROVIDERS_H
