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
    explicit Monitoring(QSharedPointer<IoslotManager> __ioslotManager,
                        QSharedPointer<Transmission> __transmission,
                        quint8 __address,
                        QObject *parent = 0);

    int valueCount() const;
    IoslotValue value(int __num) const;
    bool valueUnknown(int __num) const;
    const QList<IoslotValue> &values() const;

    enum {
        NoValue = -1
    };

    QSharedPointer<IoslotManager> ioslotManager() const;

    const QDateTime &clock() const;
    quint32 uptime() const;

    bool discreteOutputDiffers() const;

Q_SIGNALS:
    void valueAdded(int __num);
    void valueUpdated(int __num);
    void valueRemoved(int __num);

    void valuesUpdated();

    void commonValuesUpdated();

    void clockSet(bool success);

public Q_SLOTS:
    void updateValues(ReadIoslotValuesCommand *__cmd);
    void updateCommonValues(ReadCommonValuesCommand *__cmd);

private Q_SLOTS:
    void onIoslotAdded(int __num);
    void onIoslotUpdated(int __num);
    void onIoslotRemoved(int __num);    

private:
    QSharedPointer<IoslotManager> d_ioslotManager;
    QSharedPointer<Transmission> d_transmission;
    quint8 d_address;

    QList<IoslotValue> d_values;
    QDateTime d_clock;
    quint32 d_uptime;
    bool d_discreteOutputDiffers;
};

#endif // MONITORING_H
