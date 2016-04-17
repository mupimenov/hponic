#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QMutex>
#include <QSharedPointer>
#include <QThread>

#include "command.h"

class Transmission : public QThread
{
    Q_OBJECT

public:
    Transmission(QSharedPointer<Interface> interface, QObject *parent = 0);

    void setInterface(QSharedPointer<Interface> iface);
    QSharedPointer<Interface> interface();

    void addCommand(QSharedPointer<Command> cmd);
    void reset();

    enum Status {
        Stopped,
        Started
    };

    Status status();

Q_SIGNALS:
    void statusChanged(Transmission::Status status);

public Q_SLOTS:
    void stop();

protected:
    virtual void run();

private:
    QSharedPointer<Interface>       d_interface;

    QList<QSharedPointer<Command> > d_commands;
    QMutex                          d_mutex;

    Status                          d_status;
    bool                            d_stop;
};

#endif // TRANSMISSION_H
