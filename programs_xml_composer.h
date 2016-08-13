#ifndef PROGRAMXMLCOMPOSER_H
#define PROGRAMXMLCOMPOSER_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>

#include <QSharedPointer>

#include "program.h"

class ProgramsXmlComposer : public QObject
{
    Q_OBJECT
public:
    explicit ProgramsXmlComposer(QObject *parent = 0);

    virtual QDomElement toElement(const QList<QSharedPointer<Program> > &programs, QDomDocument &doc) = 0;
    virtual QList<QSharedPointer<Program> > fromElement(QDomElement &root) = 0;

Q_SIGNALS:

public Q_SLOTS:
};

class ProgramsXmlComposerV1 : public ProgramsXmlComposer
{
    Q_OBJECT
public:
    explicit ProgramsXmlComposerV1(QObject *parent = 0);

    virtual QDomElement toElement(const QList<QSharedPointer<Program> > &programs, QDomDocument &doc);
    virtual QList<QSharedPointer<Program> > fromElement(QDomElement &root);
};

#endif // PROGRAMXMLCOMPOSER_H
