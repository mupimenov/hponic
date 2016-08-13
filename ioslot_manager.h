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

    static QSharedPointer<Ioslot> createEmptySlot(int id);
    static QSharedPointer<Ioslot> createAnalogInputSlot(int id);
    static QSharedPointer<Ioslot> createDiscreteInputSlot(int id);
    static QSharedPointer<Ioslot> createDiscreteOutputSlot(int id);
    static QSharedPointer<Ioslot> createDHTxxSlot(int id);
    static QSharedPointer<Ioslot> createDallasTemperatureSlot(int id);
    static QSharedPointer<Ioslot> createMHZ19Slot(int id);
    static QSharedPointer<Ioslot> createSHT2xSlot(int id);

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
    void replaceIoslot(int num, QSharedPointer<Ioslot> ioslot, bool keepname = false);

    void replaceIoslots(const QList<QSharedPointer<Ioslot> > &ioslots, bool keepname = false);

Q_SIGNALS:
    void ioslotAdded(int num);
    void ioslotReplaced(int num);
    void ioslotUpdated(int num);
    void ioslotRemoved(int num);

public Q_SLOTS:

private Q_SLOTS:
    void onIoslotChanged(Ioslot *slot);

private:
    QList<QSharedPointer<Ioslot> >  d_ioslots;
};

#endif // IOSLOTMANAGER_H
