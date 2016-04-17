#ifndef IOSLOTMANAGER_H
#define IOSLOTMANAGER_H

#include <QObject>
#include <QList>

#include <QSharedPointer>

#include "ioslot.h"

class IoslotManager : public QObject
{
    Q_OBJECT
public:
    explicit IoslotManager(QObject *parent = 0);

    int ioslotCount() const;
    QSharedPointer<Ioslot> ioslot(int __num) const;
    const QList<QSharedPointer<Ioslot> > &ioslots() const;

    enum {
        NoIoslot = -1
    };

    int ioslotNumber(Ioslot *__ioslot) const;

    QSharedPointer<Ioslot> ioslotById(int __id) const;
    QList<QSharedPointer<Ioslot> > ioslotsByType(const QList<int> &__types) const;

    int addIoslot(QSharedPointer<Ioslot> __ioslot);
    void removeIoslot(int __num);
    void updateIoslot(int __num, QSharedPointer<Ioslot> __ioslot);

    void updateIoslots(const QList<QSharedPointer<Ioslot> > &ioslots);

Q_SIGNALS:
    void ioslotAdded(int __num);
    void ioslotUpdated(int __num);
    void ioslotRemoved(int __num);

public Q_SLOTS:

private Q_SLOTS:
    void onIoslotChanged(Ioslot *__slot);

private:
    QList<QSharedPointer<Ioslot> >  d_ioslots;
};

#endif // IOSLOTMANAGER_H
