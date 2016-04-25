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
};

#endif // IOSLOTSXMLCOMPOSER_H
