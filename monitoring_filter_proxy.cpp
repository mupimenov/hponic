#include "monitoring_filter_proxy.h"

MonitoringFilterProxy::MonitoringFilterProxy(QSharedPointer<Monitoring> monitoring,
                                             int numStart, int numEnd, FilterIoslotGroup group,
                                             QObject *parent) : QSortFilterProxyModel(parent),
    d_monitoring(monitoring),
    d_numStart(numStart),
    d_numEnd(numEnd),
    d_group(group)
{
    connect(d_monitoring->ioslotManager().data(), SIGNAL(ioslotAdded(int)), this, SLOT(invalidate()), Qt::DirectConnection);
    connect(d_monitoring->ioslotManager().data(), SIGNAL(ioslotUpdated(int)), this, SLOT(invalidate()), Qt::DirectConnection);
    connect(d_monitoring->ioslotManager().data(), SIGNAL(ioslotRemoved(int)), this, SLOT(invalidate()), Qt::DirectConnection);
}

bool MonitoringFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);

    if (sourceRow < d_numStart || sourceRow > d_numEnd)
        return false;

    int type = d_monitoring->valueType(sourceRow);
    if (d_group == InputIoslots)
        return (type == AnalogInputType || type == DiscreteInputType);
    else
        return (type == DiscreteOutputType);
}

