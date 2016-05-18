#ifndef PROGRAMSMODEL_H
#define PROGRAMSMODEL_H

#include <QAbstractItemModel>

#include <QSharedPointer>

#include "program_manager.h"

class ProgramsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ProgramsModel(QSharedPointer<ProgramManager> programManager, QObject *parent = 0);

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
    void onProgramAdded(int num);
    void onProgramReplacedUpdated(int num);
    void onProgramRemoved(int num);

private:
    QSharedPointer<ProgramManager> d_programManager;
};

#endif // PROGRAMSMODEL_H
