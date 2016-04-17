#ifndef PROGRAMBINCOMPOSER_H
#define PROGRAMBINCOMPOSER_H

#include <QObject>
#include <QByteArray>

#include <QSharedPointer>

#include "program.h"

class ProgramsBinComposer : public QObject
{
    Q_OBJECT
public:
    explicit ProgramsBinComposer(QObject *parent = 0);

    virtual QByteArray toArray(const QList<QSharedPointer<Program> > &programs) = 0;
    virtual QList<QSharedPointer<Program> > fromArray(const QByteArray &arr) = 0;

Q_SIGNALS:

public Q_SLOTS:
};

class ProgramsBinComposerV1 : public ProgramsBinComposer
{
    Q_OBJECT
public:
    explicit ProgramsBinComposerV1(QObject *parent = 0);

    virtual QByteArray toArray(const QList<QSharedPointer<Program> > &programs);
    virtual QList<QSharedPointer<Program> > fromArray(const QByteArray &arr);

Q_SIGNALS:

public Q_SLOTS:
};

extern void floatToArray(float f, QByteArray &slot, int offset);
extern float arrayToFloat(QByteArray &slot, int offset);

#endif // PROGRAMBINCOMPOSER_H
