#ifndef MONITORINGMODEL_H
#define MONITORINGMODEL_H

#include <QAbstractItemModel>

#include <QSharedPointer>

#include "monitoring.h"

class MonitoringModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    MonitoringModel(QSharedPointer<Monitoring> monitoring, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:
    void onValueAdded(int num);
    void onValueUpdated(int num);
    void onValueRemoved(int num);

private:
    QSharedPointer<Monitoring> d_monitoring;
};

#endif // MONITORINGMODEL_H
