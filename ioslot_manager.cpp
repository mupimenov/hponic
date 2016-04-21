#include "ioslot_manager.h"

IoslotManager::IoslotManager(QObject *parent) : QObject(parent)
{

}

int IoslotManager::ioslotCount() const
{
    return d_ioslots.size();
}

QSharedPointer<Ioslot> IoslotManager::ioslot(int num) const
{
    if (num < 0 || num >= d_ioslots.size())
        return QSharedPointer<Ioslot>();

    return d_ioslots[num];
}

const QList<QSharedPointer<Ioslot> > &IoslotManager::ioslots() const
{
    return d_ioslots;
}

int IoslotManager::ioslotNumber(Ioslot * ioslot) const
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (int j = 0; it != d_ioslots.end(); ++it, ++j) {
        if ((*it).data() == ioslot)
            return j;
    }

    return NoIoslot;
}

QSharedPointer<Ioslot> IoslotManager::ioslotById(int id) const
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (; it != d_ioslots.end(); ++it) {
        if ((*it)->id() == id)
            return (*it);
    }

    return QSharedPointer<Ioslot>();
}

QList<QSharedPointer<Ioslot> > IoslotManager::ioslotsByType(const QList<int> &types) const
{
    QList<QSharedPointer<Ioslot> > list;
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (; it != d_ioslots.end(); ++it) {
        if (types.contains((*it)->type()))
            list.append(*it);
    }

    return list;
}

int IoslotManager::addIoslot(QSharedPointer<Ioslot> ioslot)
{
    d_ioslots.append(ioslot);

    connect(ioslot.data(), SIGNAL(changed(Ioslot*)), this, SLOT(onIoslotChanged(Ioslot*)), Qt::DirectConnection);
    int num = d_ioslots.size() - 1;

    Q_EMIT ioslotAdded(num);
    return num;
}

void IoslotManager::removeIoslot(int num)
{
    d_ioslots.removeAt(num);
    Q_EMIT ioslotRemoved(num);
}

void IoslotManager::replaceIoslot(int num, QSharedPointer<Ioslot> ioslot, bool keepname)
{
    if (keepname) {
        QSharedPointer<Ioslot> current = this->ioslot(num);
        if (current->driver() == ioslot->driver())
            ioslot->setName(current->name());
    }

    d_ioslots.replace(num, ioslot);
    connect(ioslot.data(), SIGNAL(changed(Ioslot*)), this, SLOT(onIoslotChanged(Ioslot*)), Qt::DirectConnection);
    Q_EMIT ioslotReplaced(num);
}

void IoslotManager::onIoslotChanged(Ioslot *slot)
{
    int num = ioslotNumber(slot);
    if (num == NoIoslot)
        return;

    Q_EMIT ioslotUpdated(num);
}

void IoslotManager::replaceIoslots(const QList<QSharedPointer<Ioslot> > &ioslots, bool keepname)
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = ioslots.begin();
    for (int j = 0; it != ioslots.end(); ++it, ++j)
        replaceIoslot(j, *it, keepname);
}
