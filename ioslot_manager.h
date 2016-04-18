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
    QSharedPointer<Ioslot> ioslot(int num) const;
    const QList<QSharedPointer<Ioslot> > &ioslots() const;

    enum {
        NoIoslot = -1
    };

    int ioslotNumber(Ioslot *ioslot) const;

    QSharedPointer<Ioslot> ioslotById(int id) const;
    QList<QSharedPointer<Ioslot> > ioslotsByType(const QList<int> &types) const;

    int addIoslot(QSharedPointer<Ioslot> ioslot);
    void removeIoslot(int num);
    void updateIoslot(int num, QSharedPointer<Ioslot> ioslot);

    void updateIoslots(const QList<QSharedPointer<Ioslot> > &ioslots);

Q_SIGNALS:
    void ioslotAdded(int num);
    void ioslotUpdated(int num);
    void ioslotRemoved(int num);

public Q_SLOTS:

private Q_SLOTS:
    void onIoslotChanged(Ioslot *slot);

private:
    QList<QSharedPointer<Ioslot> >  d_ioslots;
};

#endif // IOSLOTMANAGER_H
