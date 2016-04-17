#include "ioslot_manager.h"

IoslotManager::IoslotManager(QObject *parent) : QObject(parent)
{

}

int IoslotManager::ioslotCount() const
{
    return d_ioslots.size();
}

QSharedPointer<Ioslot> IoslotManager::ioslot(int __num) const
{
    if (__num < 0 || __num >= d_ioslots.size())
        return QSharedPointer<Ioslot>();

    return d_ioslots[__num];
}

const QList<QSharedPointer<Ioslot> > &IoslotManager::ioslots() const
{
    return d_ioslots;
}

int IoslotManager::ioslotNumber(Ioslot * __ioslot) const
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (int j = 0; it != d_ioslots.end(); ++it, ++j) {
        if ((*it).data() == __ioslot)
            return j;
    }

    return NoIoslot;
}

QSharedPointer<Ioslot> IoslotManager::ioslotById(int __id) const
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (; it != d_ioslots.end(); ++it) {
        if ((*it)->id() == __id)
            return (*it);
    }

    return QSharedPointer<Ioslot>();
}

QList<QSharedPointer<Ioslot> > IoslotManager::ioslotsByType(const QList<int> &__types) const
{
    QList<QSharedPointer<Ioslot> > list;
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (; it != d_ioslots.end(); ++it) {
        if (__types.contains((*it)->type()))
            list.append(*it);
    }

    return list;
}

int IoslotManager::addIoslot(QSharedPointer<Ioslot> __ioslot)
{
    d_ioslots.append(__ioslot);

    connect(__ioslot.data(), SIGNAL(changed(Ioslot*)), this, SLOT(onIoslotChanged(Ioslot*)), Qt::DirectConnection);
    int num = d_ioslots.size() - 1;

    Q_EMIT ioslotAdded(num);
    return num;
}

void IoslotManager::removeIoslot(int __num)
{
    d_ioslots.removeAt(__num);
    Q_EMIT ioslotRemoved(__num);
}

void IoslotManager::updateIoslot(int __num, QSharedPointer<Ioslot> __ioslot)
{
    d_ioslots.replace(__num, __ioslot);
    connect(__ioslot.data(), SIGNAL(changed(Ioslot*)), this, SLOT(onIoslotChanged(Ioslot*)), Qt::DirectConnection);
    Q_EMIT ioslotUpdated(__num);
}

void IoslotManager::onIoslotChanged(Ioslot *__slot)
{
    int num = ioslotNumber(__slot);
    if (num == NoIoslot)
        return;

    Q_EMIT ioslotUpdated(num);
}

void IoslotManager::updateIoslots(const QList<QSharedPointer<Ioslot> > &ioslots)
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = ioslots.begin();
    for (int j = 0; it != ioslots.end(); ++it, ++j)
        updateIoslot(j, *it);
}
