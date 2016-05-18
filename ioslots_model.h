#ifndef IOSLOTSMODEL_H
#define IOSLOTSMODEL_H

#include <QAbstractItemModel>

#include <QSharedPointer>

#include "ioslot_manager.h"

class IoslotsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    IoslotsModel(QSharedPointer<IoslotManager> ioslotManager, QObject *parent = 0);

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
    void onIoslotAdded(int num);
    void onIoslotReplacedUpdated(int num);
    void onIoslotRemoved(int num);

private:
    QSharedPointer<IoslotManager> d_ioslotManager;
};

#endif // IOSLOTSMODEL_H
