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

    static const char *programsTag;
    static const char *programTag;
    static const char *typeAttr;
    static const char *idAttr;
    static const char *nameAttr;
    static const char *inputAttr;
    static const char *outputAttr;
    static const char *fromAttr;
    static const char *toAttr;
    static const char *dateTimeFormat;
    static const char *cyclogramTypeAttr;
    static const char *cyclogramCountAttr;
    static const char *cyclogramImpulseDurationAttr;
    static const char *cyclogramPauseDurationAttr;
    static const char *lowBoundAttr;
    static const char *highBoundAttr;
    static const char *proportionalAttr;
    static const char *integralAttr;
    static const char *differentialAttr;

private:
    void cyclogramToElement(const Cyclogram &c, QDomElement &elem);
    void cyclogramFromElement(QDomElement &elem, Cyclogram &c);
};

#endif // PROGRAMXMLCOMPOSER_H
