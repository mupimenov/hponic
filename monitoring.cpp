#include "monitoring.h"

Monitoring::Monitoring(QSharedPointer<IoslotManager> ioslotManager,
                       QSharedPointer<Transmission> transmission,
                       quint8 address,
                       QObject *parent) : QObject(parent),
    d_ioslotManager(ioslotManager),
    d_transmission(transmission),
    d_address(address)
{
    connect(d_ioslotManager.data(), SIGNAL(ioslotAdded(int)), this, SLOT(onIoslotAdded(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotUpdated(int)), this, SLOT(onIoslotUpdated(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotRemoved(int)), this, SLOT(onIoslotRemoved(int)), Qt::DirectConnection);
}

int Monitoring::valueCount() const
{
    return d_values.size();
}

IoslotValue Monitoring::value(int num) const
{
    if (num < 0 || num >= d_values.size())
        return IoslotValue();

    return d_values[num];
}

bool Monitoring::valueUnknown(int num) const
{
    return (d_values[num].first->type() == UnknownIoslotType);
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

void Monitoring::onIoslotAdded(int num)
{
    QSharedPointer<Ioslot> ioslot = d_ioslotManager->ioslot(num);
    d_values.append(IoslotValue(ioslot, QVariant()));
    Q_EMIT valueAdded(num);
}

void Monitoring::onIoslotUpdated(int num)
{
    QSharedPointer<Ioslot> ioslot = d_ioslotManager->ioslot(num);
    d_values.replace(num, IoslotValue(ioslot, QVariant()));
    Q_EMIT valueUpdated(num);
}

void Monitoring::onIoslotRemoved(int num)
{
    d_values.removeAt(num);
    Q_EMIT valueRemoved(num);
}

void Monitoring::updateValues(ReadIoslotValuesCommand *cmd)
{
    QList<IoslotValue>::iterator it = d_values.begin();
    d_discreteOutputDiffers = false;

    for (int num = 0; it != d_values.end(); ++it, ++num) {
        QVariant v;
        if (cmd->result() == Command::Ok) {
            switch ((*it).first->type()) {
            case UnknownIoslotType:
                break;
            case AnalogInputType:
                v.setValue(cmd->valueFloat(num));
                break;
            case DiscreteInputType:
                v.setValue(cmd->valueUInt(num));
                break;
            case DiscreteOutputType:
                v.setValue(cmd->valueUInt(num));
                if ((*it).second.toUInt() != v.toUInt())
                    d_discreteOutputDiffers = true;
                break;
            }
        }
        (*it).second.setValue(v);
    }

    Q_EMIT valuesUpdated();
}

void Monitoring::updateCommonValues(ReadCommonValuesCommand *cmd)
{
    if (cmd->result() == Command::Ok) {
        d_clock = cmd->clock();
        d_uptime = cmd->uptime();

        Q_EMIT commonValuesUpdated();
    } else {
        Q_EMIT commonValuesNotUpdated(cmd->result());
    }
}
