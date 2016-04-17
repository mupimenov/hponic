#include "monitoring.h"

Monitoring::Monitoring(QSharedPointer<IoslotManager> __ioslotManager,
                       QSharedPointer<Transmission> __transmission,
                       quint8 __address,
                       QObject *parent) : QObject(parent),
    d_ioslotManager(__ioslotManager),
    d_transmission(__transmission),
    d_address(__address)
{
    connect(d_ioslotManager.data(), SIGNAL(ioslotAdded(int)), this, SLOT(onIoslotAdded(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotUpdated(int)), this, SLOT(onIoslotUpdated(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotRemoved(int)), this, SLOT(onIoslotRemoved(int)), Qt::DirectConnection);
}

int Monitoring::valueCount() const
{
    return d_values.size();
}

IoslotValue Monitoring::value(int __num) const
{
    if (__num < 0 || __num >= d_values.size())
        return IoslotValue();

    return d_values[__num];
}

bool Monitoring::valueUnknown(int __num) const
{
    return (d_values[__num].first->type() == UnknownIoslotType);
}

const QList<IoslotValue> &Monitoring::values() const
{
    return d_values;
}

QSharedPointer<IoslotManager> Monitoring::ioslotManager() const
{
    return d_ioslotManager;
}

const QDateTime &Monitoring::clock() const
{
    return d_clock;
}

quint32 Monitoring::uptime() const
{
    return d_uptime;
}

bool Monitoring::discreteOutputDiffers() const
{
    return d_discreteOutputDiffers;
}

void Monitoring::onIoslotAdded(int __num)
{
    QSharedPointer<Ioslot> ioslot = d_ioslotManager->ioslot(__num);
    d_values.append(IoslotValue(ioslot, QVariant()));
    Q_EMIT valueAdded(__num);
}

void Monitoring::onIoslotUpdated(int __num)
{
    QSharedPointer<Ioslot> ioslot = d_ioslotManager->ioslot(__num);
    d_values.replace(__num, IoslotValue(ioslot, QVariant()));
    Q_EMIT valueUpdated(__num);
}

void Monitoring::onIoslotRemoved(int __num)
{
    d_values.removeAt(__num);
    Q_EMIT valueRemoved(__num);
}

void Monitoring::updateValues(ReadIoslotValuesCommand *__cmd)
{
    QList<IoslotValue>::iterator it = d_values.begin();
    d_discreteOutputDiffers = false;

    for (int num = 0; it != d_values.end(); ++it, ++num) {
        QVariant v;
        if (__cmd->result() == Command::Ok) {
            switch ((*it).first->type()) {
            case UnknownIoslotType:
                break;
            case AnalogInputType:
                v.setValue(__cmd->valueFloat(num));
                break;
            case DiscreteInputType:
                v.setValue(__cmd->valueUInt(num));
                break;
            case DiscreteOutputType:
                v.setValue(__cmd->valueUInt(num));
                if ((*it).second.toUInt() != v.toUInt())
                    d_discreteOutputDiffers = true;
                break;
            }
        }
        (*it).second.setValue(v);
    }

    Q_EMIT valuesUpdated();
}

void Monitoring::updateCommonValues(ReadCommonValuesCommand *__cmd)
{
    if (__cmd->result() == Command::Ok) {
        d_clock = __cmd->clock();
        d_uptime = __cmd->uptime();

        Q_EMIT commonValuesUpdated();
    }
}
