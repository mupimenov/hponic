#ifndef MONITORINGFILTERPROXY_H
#define MONITORINGFILTERPROXY_H

#include <QSortFilterProxyModel>
#include <QSharedPointer>

#include "monitoring.h"

class MonitoringFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum FilterIoslotGroup
    {
        InputIoslots,
        OutputIoslots
    };

    explicit MonitoringFilterProxy(QSharedPointer<Monitoring> monitoring,
                                   int numStart, int numEnd, FilterIoslotGroup group,
                                   QObject *parent = 0);



protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

Q_SIGNALS:

public Q_SLOTS:

private:
    QSharedPointer<Monitoring> d_monitoring;
    int d_numStart;
    int d_numEnd;
    FilterIoslotGroup d_group;
};

#endif // MONITORINGFILTERPROXY_H
