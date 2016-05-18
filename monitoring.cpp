#include "monitoring.h"

Monitoring::Monitoring(QSharedPointer<IoslotManager> ioslotManager,
                       QSharedPointer<Transmission> transmission,
                       quint8 address,
                       QObject *parent) : QObject(parent),
    d_ioslotManager(ioslotManager),
    d_transmission(transmission),
    d_address(address),
    d_adc(16)
{
    connect(d_ioslotManager.data(), SIGNAL(ioslotAdded(int)), this, SLOT(onIoslotAdded(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotReplaced(int)), this, SLOT(onIoslotReplacedUpdated(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotUpdated(int)), this, SLOT(onIoslotReplacedUpdated(int)), Qt::DirectConnection);
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

int Monitoring::valueType(int num) const
{
    return (d_values[num].ioslot->type());
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

quint16 Monitoring::adcValue(int channel) const
{
    if (channel < 0 || channel >= d_adc.size())
        return 0;

    return d_adc[channel];
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

void Monitoring::onIoslotReplacedUpdated(int num)
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
    if (cmd->result() == Command::Ok) {
        QList<IoslotValue>::iterator it = d_values.begin();
        d_discreteOutputDiffers = false;

        for (int num = 0; it != d_values.end(); ++it, ++num) {
            QVariant v;
            if (cmd->result() == Command::Ok) {
                switch (it->ioslot->type()) {
                case UnknownIoslotType:
                    break;
                case AnalogInputType:
                    v.setValue(cmd->valueFloat(num));
                    break;
                case DiscreteInputType:
                    v.setValue(cmd->valueFloat(num) > 0.5f? 1: 0);
                    break;
                case DiscreteOutputType:
                    v.setValue(cmd->valueFloat(num) > 0.5f? 1: 0);
                    if (it->value.toUInt() != v.toUInt())
                        d_discreteOutputDiffers = true;
                    break;
                }
            }
            it->value.setValue(v);
        }

        Q_EMIT valuesUpdated();
    }
}

void Monitoring::updateCommonValues(ReadCommonValuesCommand *cmd)
{
    if (cmd->result() == Command::Ok) {
        d_clock = cmd->clock();
        d_uptime = cmd->uptime();

        Q_EMIT commonValuesUpdated();
    }
}

void Monitoring::updateAdcValues(ReadAdcValuesCommand *cmd)
{
    if (cmd->result() == Command::Ok) {
        for (int channel = 0; channel < d_adc.size(); ++channel)
            d_adc[channel] = cmd->value(channel);

        Q_EMIT adcValuesUpdated();
    }
}

IoslotValue::IoslotValue() :
    ioslot(),
    value()
{

}

IoslotValue::IoslotValue(QSharedPointer<Ioslot> i, const QVariant &v) :
    ioslot(i),
    value(v)
{

}
