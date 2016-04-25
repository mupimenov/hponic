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
    DHT22TemperatureDriver,
    DHT22HumidityDriver,
    DallasTemperatureDriver
};

enum IoslotType
{
    UnknownIoslotType,
    AnalogInputType,
    DiscreteInputType,
    DiscreteOutputType
};

class Ioslot : public QObject
{
    Q_OBJECT
public:
    explicit Ioslot(int id, int type, int driver, QObject *parent = 0);
    virtual ~Ioslot();

    void setName(const QString &name);
    const QString &name() const;

    int id() const;
    int type() const;
    int driver() const;

Q_SIGNALS:
    void changed(Ioslot *sender = 0);

public Q_SLOTS:

protected:
    int d_id;
    int d_type;
    int d_driver;
    QString d_name;
};

/*
 * EmptySlot
 * */

class EmptySlot : public Ioslot
{
    Q_OBJECT
public:
    explicit EmptySlot(int id, QObject *parent = 0);
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
    explicit AnalogInputSlot(int id, QObject *parent = 0);
    virtual ~AnalogInputSlot();

    void setNum(int num);
    int num() const;

    void setLinear(float x1, float y1, float x2, float y2);
    float x1() const;
    float y1() const;
    float x2() const;
    float y2() const;

    float k() const;
    float b() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_num;
    float d_x1;
    float d_y1;
    float d_x2;
    float d_y2;
};

/*
 * DiscreteInputSlot
 * */

class DiscreteInputSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DiscreteInputSlot(int id, QObject *parent = 0);
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
    explicit DiscreteOutputSlot(int id, QObject *parent = 0);
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
 * DHT22TemperatureSlot
 * */

class DHT22TemperatureSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DHT22TemperatureSlot(int id, QObject *parent = 0);
    virtual ~DHT22TemperatureSlot();

    void setPin(int pin);
    int pin() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_pin;
};

/*
 * DHT22HumiditySlot
 * */

class DHT22HumiditySlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DHT22HumiditySlot(int id, QObject *parent = 0);
    virtual ~DHT22HumiditySlot();

    void setPin(int pin);
    int pin() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_pin;
};

/*
 * DallasTemperatureSlot
 * */

class DallasTemperatureSlot : public Ioslot
{
    Q_OBJECT
public:
    explicit DallasTemperatureSlot(int id, QObject *parent = 0);
    virtual ~DallasTemperatureSlot();

    void setPin(int pin);
    int pin() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_pin;
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
