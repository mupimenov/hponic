#ifndef IOSLOT_H
#define IOSLOT_H

#include <QObject>

#include <QSharedPointer>

enum IoslotDriver
{
    EmptySlotDriver = 0,
    AnalogInputDriver,
    DiscreteInputDriver,
    DiscreteOutputDriver,
    DHTxxDriver,
    DallasTemperatureDriver,
    MHZ19Driver,
    SHT2xDriver
};

enum IoslotType
{
    UnknownIoslotType,
    AnalogInputType,
    DiscreteInputType,
    DiscreteOutputType
};

class IoslotEditorProvider;
class IoslotBinProvider;
class IoslotXmlProvider;

class IoslotProviders : public QObject {
    Q_OBJECT
public:
    virtual QSharedPointer<IoslotEditorProvider> editorProvider() = 0;
    virtual QSharedPointer<IoslotBinProvider> binProvider() = 0;
    virtual QSharedPointer<IoslotXmlProvider> xmlProvider() = 0;
};

class IoslotProvidersV1 : public IoslotProviders {
    Q_OBJECT
public:
    explicit IoslotProvidersV1(QSharedPointer<IoslotEditorProvider> editorProvider_,
                               QSharedPointer<IoslotBinProvider> binProvider_,
                               QSharedPointer<IoslotXmlProvider> xmlProvider_);

    virtual QSharedPointer<IoslotEditorProvider> editorProvider();
    virtual QSharedPointer<IoslotBinProvider> binProvider();
    virtual QSharedPointer<IoslotXmlProvider> xmlProvider();

private:
    QSharedPointer<IoslotEditorProvider> d_editorProvider;
    QSharedPointer<IoslotBinProvider> d_binProvider;
    QSharedPointer<IoslotXmlProvider> d_xmlProvider;
};

class Ioslot : public QObject
{
    Q_OBJECT
public:
    explicit Ioslot(int id,
                    int type,
                    int driver,
                    QObject *parent = 0);
    virtual ~Ioslot();

    void setName(const QString &name);
    const QString &name() const;

    int id() const;
    int type() const;
    int driver() const;

    void setProviders(QSharedPointer<IoslotProviders> providers);
    QSharedPointer<IoslotProviders> providers() const;

Q_SIGNALS:
    void changed(Ioslot *sender = 0);

public Q_SLOTS:

private:
    int d_id;
    int d_type;
    int d_driver;
    QString d_name;
    QSharedPointer<IoslotProviders> d_providers;
};

/*
 * EmptySlot
 * */

class EmptySlot : public Ioslot
{
    Q_OBJECT
public:
    explicit EmptySlot(int id,
                       QObject *parent = 0);
    virtual ~EmptySlot();

Q_SIGNALS:

public Q_SLOTS:

};

/*
 * AnalogInputSlot
 * */

class AnalogInputSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit AnalogInputSlot(int id,
                             QObject *parent = 0);
    virtual ~AnalogInputSlot();

    void setNum(int num);
    int num() const;

    void setLinear(quint16 x1, float y1, quint16 x2, float y2);
    quint16 x1() const;
    quint16 x2() const;

    float y1() const;    
    float y2() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_num;
    quint16 d_x1;
    quint16 d_x2;

    float d_y1;
    float d_y2;
};

/*
 * DiscreteInputSlot
 * */

class DiscreteInputSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DiscreteInputSlot(int id,
                               QObject *parent = 0);
    virtual ~DiscreteInputSlot();

    void setPin(int pin);
    int pin() const;

    void setInverse(bool inverse);
    bool inverse() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_pin;
    bool d_inverse;
};

/*
 * DiscreteOutputSlot
 * */

class DiscreteOutputSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DiscreteOutputSlot(int id,
                                QObject *parent = 0);
    virtual ~DiscreteOutputSlot();

    void setPin(int pin);
    int pin() const;

    void setInverse(bool inverse);
    bool inverse() const;

    enum LogicOperation {
        LogicOr = 0,
        LogicAnd
    };

    void setOperation(int operation);
    int operation() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_operation;
    int d_pin;
    bool d_inverse;
};

/*
 * DHTxxSlot
 * */

enum DHTxxModification {
    DHT11,
    DHT22
};

enum DHTxxParameter {
    DHTxxTemperature,
    DHTxxHumidity
};

class DHTxxSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DHTxxSlot(int id,
                       QObject *parent = 0);
    virtual ~DHTxxSlot();

    void setModification(int modification);
    int modification() const;

    void setParameter(int parameter);
    int parameter() const;

    void setPin(int pin);
    int pin() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_modification;
    int d_parameter;
    int d_pin;
};

/*
 * DallasTemperatureSlot
 * */

class DallasTemperatureSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DallasTemperatureSlot(int id,
                                   QObject *parent = 0);
    virtual ~DallasTemperatureSlot();

    void setPin(int pin);
    int pin() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_pin;
};

/*
 * MH-Z19
 * */

class MHZ19Slot : public Ioslot
{
    Q_OBJECT
public:
    explicit MHZ19Slot(int id,
                       QObject *parent = 0);
    virtual ~MHZ19Slot();

    void setReceivePin(int pin);
    int receivePin() const;

    void setTransmitPin(int pin);
    int transmitPin() const;
Q_SIGNALS:

public Q_SLOTS:

private:
    int d_receivePin;
    int d_transmitPin;
};

/*
 * SHT2x
 * */

enum SHT2xParameter {
    SHT2xTemperature,
    SHT2xHumidity
};

class SHT2xSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit SHT2xSlot(int id,
                       QObject *parent = 0);
    virtual ~SHT2xSlot();

    void setParameter(int parameter);
    int parameter() const;

    void setSdaPin(int pin);
    int sdaPin() const;

    void setSclPin(int pin);
    int sclPin() const;
Q_SIGNALS:

public Q_SLOTS:

private:
    int d_parameter;
    int d_sdaPin;
    int d_sclPin;
};

/*
 * IoslotConv
 * */

class IoslotConv
{
public:
    template<class S>
    static QSharedPointer<S> toSlot(QSharedPointer<Ioslot> &ioslot) {
        QSharedPointer<S> s = ioslot.dynamicCast<S>();
        return s;
    }
};

Q_DECLARE_METATYPE(QSharedPointer<Ioslot>)

#endif // IOSLOT_H
