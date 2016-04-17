#ifndef IOSLOTSBINCOMPOSER_H
#define IOSLOTSBINCOMPOSER_H

#include <QObject>
#include <QByteArray>

#include <QSharedPointer>

#include "ioslot.h"

class IoslotsBinComposer : public QObject
{
    Q_OBJECT
public:
    explicit IoslotsBinComposer(QObject *parent = 0);

    virtual QByteArray toArray(const QList<QSharedPointer<Ioslot> > &ioslots) = 0;
    virtual QList<QSharedPointer<Ioslot> > fromArray(const QByteArray &arr) = 0;

Q_SIGNALS:

public Q_SLOTS:
};

class IoslotsBinComposerV1 : public IoslotsBinComposer
{
    Q_OBJECT
public:
    explicit IoslotsBinComposerV1(QObject *parent = 0);

    virtual QByteArray toArray(const QList<QSharedPointer<Ioslot> > &ioslots);
    virtual QList<QSharedPointer<Ioslot> > fromArray(const QByteArray &arr);

Q_SIGNALS:

public Q_SLOTS:
};

#endif // IOSLOTSBINCOMPOSER_H
