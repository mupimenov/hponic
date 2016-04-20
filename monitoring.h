#ifndef MONITORING_H
#define MONITORING_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QVariant>

#include <QSharedPointer>

#include "ioslot_manager.h"
#include "transmission.h"
#include "hponic_commands.h"

typedef QPair<QSharedPointer<Ioslot>, QVariant> IoslotValue;

class Monitoring : public QObject
{
    Q_OBJECT
public:
    explicit Monitoring(QSharedPointer<IoslotManager> ioslotManager,
                        QSharedPointer<Transmission> transmission,
                        quint8 address,
                        QObject *parent = 0);

    int valueCount() const;
    IoslotValue value(int num) const;
    bool valueUnknown(int num) const;
    const QList<IoslotValue> &values() const;

    enum {
        NoValue = -1
    };

    QSharedPointer<IoslotManager> ioslotManager() const;

    const QDateTime &clock() const;
    quint32 uptime() const;

    quint16 adcValue(int channel) const;

    bool discreteOutputDiffers() const;

Q_SIGNALS:
    void valueAdded(int num);
    void valueUpdated(int num);
    void valueRemoved(int num);

    void valuesUpdated();

    void commonValuesUpdated();
    void commonValuesNotUpdated(Command::Result result);

    void adcValuesUpdated();
    void adcValuesNotUpdated(Command::Result result);

    void clockSet(bool success);

public Q_SLOTS:
    void updateValues(ReadIoslotValuesCommand *cmd);
    void updateCommonValues(ReadCommonValuesCommand *cmd);
    void updateAdcValues(ReadAdcValuesCommand *cmd);

private Q_SLOTS:
    void onIoslotAdded(int num);
    void onIoslotUpdated(int num);
    void onIoslotRemoved(int num);

private:
    QSharedPointer<IoslotManager> d_ioslotManager;
    QSharedPointer<Transmission> d_transmission;
    quint8 d_address;

    QList<IoslotValue> d_values;
    QDateTime d_clock;
    quint32 d_uptime;
    QVector<quint8> d_adc;
    bool d_discreteOutputDiffers;
};

#endif // MONITORING_H
