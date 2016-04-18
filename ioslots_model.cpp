#include "ioslots_model.h"

IoslotsModel::IoslotsModel(QSharedPointer<IoslotManager> ioslotManager, QObject *parent) : QAbstractItemModel(parent),
    d_ioslotManager(ioslotManager)
{
    connect(d_ioslotManager.data(), SIGNAL(ioslotAdded(int)), this, SLOT(onIoslotAdded(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotUpdated(int)), this, SLOT(onIoslotUpdated(int)), Qt::DirectConnection);
    connect(d_ioslotManager.data(), SIGNAL(ioslotRemoved(int)), this, SLOT(onIoslotRemoved(int)), Qt::DirectConnection);
}

QVariant IoslotsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    Ioslot *ioslot = static_cast<Ioslot*>(index.internalPointer());

    switch (index.column()) {
    case 0:
        return QVariant(ioslot->id());
    case 1:
        return QVariant(ioslot->name());
    default:
        return QVariant();
    }
}

QVariant IoslotsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return QVariant(tr("Number"));
        case 1: return QVariant(tr("Slot"));
        default: return QVariant();
        }
    }

    return QVariant();
}

QModelIndex IoslotsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row >= 0 && row < d_ioslotManager->ioslotCount())
        return createIndex(row, column, d_ioslotManager->ioslot(row).data());

    return QModelIndex();
}

QModelIndex IoslotsModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return QModelIndex();
}

int IoslotsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return d_ioslotManager->ioslotCount();
    return 0;
}

int IoslotsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}

Qt::ItemFlags IoslotsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void IoslotsModel::onIoslotAdded(int num)
{
    beginInsertRows(QModelIndex(), num, num);
    endInsertRows();
}

void IoslotsModel::onIoslotUpdated(int num)
{
    beginRemoveRows(QModelIndex(), num, num);
    endRemoveRows();
}

void IoslotsModel::onIoslotRemoved(int num)
{
    beginRemoveRows(QModelIndex(), num, num);
    endRemoveRows();
}
