#ifndef IOSLOTSXMLCOMPOSER_H
#define IOSLOTSXMLCOMPOSER_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>

#include <QSharedPointer>

#include "ioslot.h"

class IoslotsXmlComposer : public QObject
{
    Q_OBJECT
public:
    explicit IoslotsXmlComposer(QObject *parent = 0);

    virtual QDomElement toElement(const QList<QSharedPointer<Ioslot> > &ioslots, QDomDocument &doc) = 0;
    virtual QList<QSharedPointer<Ioslot> > fromElement(QDomElement &root) = 0;

Q_SIGNALS:

public Q_SLOTS:
};

class IoslotsXmlComposerV1 : public IoslotsXmlComposer
{
    Q_OBJECT
public:
    explicit IoslotsXmlComposerV1(QObject *parent = 0);

    virtual QDomElement toElement(const QList<QSharedPointer<Ioslot> > &ioslots, QDomDocument &doc);
    virtual QList<QSharedPointer<Ioslot> > fromElement(QDomElement &root);

    static const char *ioslotsTag;
    static const char *slotTag;
    static const char *driverAttr;
    static const char *idAttr;
    static const char *nameAttr;
    static const char *numAttr;
    static const char *x1Attr;
    static const char *y1Attr;
    static const char *x2Attr;
    static const char *y2Attr;
    static const char *pinAttr;
    static const char *inverseAttr;
    static const char *operationAttr;
};

#endif // IOSLOTSXMLCOMPOSER_H
