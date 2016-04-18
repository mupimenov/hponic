#include "programs_model.h"

ProgramsModel::ProgramsModel(QSharedPointer<ProgramManager> programManager, QObject *parent) : QAbstractItemModel(parent),
    d_programManager(programManager)
{
    connect(d_programManager.data(), SIGNAL(programAdded(int)), this, SLOT(onProgramAdded(int)), Qt::DirectConnection);
    connect(d_programManager.data(), SIGNAL(programUpdated(int)), this, SLOT(onProgramUpdated(int)), Qt::DirectConnection);
    connect(d_programManager.data(), SIGNAL(programRemoved(int)), this, SLOT(onProgramRemoved(int)), Qt::DirectConnection);
}

QVariant ProgramsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    Program *program = static_cast<Program*>(index.internalPointer());

    switch (index.column()) {
    case 0:
        return QVariant(program->id());
    case 1:
        return QVariant(program->name());
    default:
        return QVariant();
    }
}

QVariant ProgramsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return QVariant(tr("Number"));
        case 1: return QVariant(tr("Program"));
        default: return QVariant();
        }
    }

    return QVariant();
}

QModelIndex ProgramsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row >= 0 && row < d_programManager->programCount())
        return createIndex(row, column, d_programManager->program(row).data());

    return QModelIndex();
}

QModelIndex ProgramsModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return QModelIndex();
}

int ProgramsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return d_programManager->programCount();
    return 0;
}

int ProgramsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}

Qt::ItemFlags ProgramsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void ProgramsModel::onProgramAdded(int num)
{
    beginInsertRows(QModelIndex(), num, num);
    endInsertRows();
}

void ProgramsModel::onProgramUpdated(int num)
{
    beginRemoveRows(QModelIndex(), num, num);
    endRemoveRows();
}

void ProgramsModel::onProgramRemoved(int num)
{
    beginRemoveRows(QModelIndex(), num, num);
    endRemoveRows();
}
