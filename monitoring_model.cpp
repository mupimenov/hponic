#include "monitoring_model.h"

MonitoringModel::MonitoringModel(QSharedPointer<Monitoring> monitoring, QObject *parent) : QAbstractItemModel(parent),
    d_monitoring(monitoring)
{
    connect(d_monitoring.data(), SIGNAL(valueAdded(int)), this, SLOT(onValueAdded(int)), Qt::DirectConnection);
    connect(d_monitoring.data(), SIGNAL(valueUpdated(int)), this, SLOT(onValueUpdated(int)), Qt::DirectConnection);
    connect(d_monitoring.data(), SIGNAL(valuesUpdated()), this, SLOT(onValuesUpdated()), Qt::DirectConnection);
    connect(d_monitoring.data(), SIGNAL(valueRemoved(int)), this, SLOT(onValueRemoved(int)), Qt::DirectConnection);
}

QVariant MonitoringModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    IoslotValue value = d_monitoring->value(index.row());

    switch (index.column()) {
    case 0:
        return QVariant(value.first->name());
    case 1:
        return value.second;
    default:
        return QVariant();
    }
}

QVariant MonitoringModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return QVariant(tr("Slot"));
        case 1: return QVariant(tr("Value"));
        default: return QVariant();
        }
    }

    return QVariant();
}

QModelIndex MonitoringModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row >= 0 && row < d_monitoring->valueCount())
        return createIndex(row, column);

    return QModelIndex();
}

QModelIndex MonitoringModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return QModelIndex();
}

int MonitoringModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return d_monitoring->valueCount();
    return 0;
}

int MonitoringModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}

Qt::ItemFlags MonitoringModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void MonitoringModel::onValueAdded(int num)
{
    beginInsertRows(QModelIndex(), num, num);
    endInsertRows();
}

void MonitoringModel::onValueUpdated(int num)
{
    QModelIndex topLeft = index(num, 0);
    QModelIndex bottomRight = index(num, columnCount() - 1);

    Q_EMIT dataChanged(topLeft, bottomRight);
}

void MonitoringModel::onValuesUpdated()
{
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);

    Q_EMIT dataChanged(topLeft, bottomRight);
}

void MonitoringModel::onValueRemoved(int num)
{
    beginRemoveRows(QModelIndex(), num, num);
    endRemoveRows();
}
