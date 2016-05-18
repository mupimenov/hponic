#include "transmission.h"

#include <QTimer>
#include <QEventLoop>
#include <QDebug>

#include "interface.h"

Transmission::Transmission(QSharedPointer<Interface> interface, QObject *parent) :
    QThread(parent),
    d_interface(interface),
    d_status(Stopped),
    d_stop(true)
{
    interface->moveToThread(this);
}

void Transmission::setInterface(QSharedPointer<Interface> iface)
{
    d_interface = iface;
}

QSharedPointer<Interface> Transmission::interface()
{
    return d_interface;
}

void Transmission::stop()
{    
    QMutexLocker locker(&d_mutex);
    d_stop = true;
}

void Transmission::run()
{
    if (!d_interface)
        return;

    if (!d_interface->open())
        return;

    {
        QMutexLocker locker(&d_mutex);
        d_status = Started;
        d_stop = false;
    }

    Q_EMIT statusChanged(d_status);

    while (1) {
        {
            QMutexLocker locker(&d_mutex);
            if (d_stop)
                break;
        }

        if (d_commands.size() == 0)
            break;

        QSharedPointer<Command> cmd;
        {
            QMutexLocker locker(&d_mutex);
            cmd = d_commands.takeFirst();
        }

        Command::Result result = cmd->send();
        Q_EMIT commandSend(result);

        if (cmd->rythm() == Command::Multiple) {
            QMutexLocker locker(&d_mutex);
            d_commands.push_back(cmd);
        }
    }

    d_interface->close();

    {
        QMutexLocker locker(&d_mutex);
        d_status = Stopped;
    }

    Q_EMIT statusChanged(d_status);
}

void Transmission::addCommand(QSharedPointer<Command> cmd)
{
    QMutexLocker locker(&d_mutex);
    d_commands.push_front(cmd);
}

void Transmission::reset()
{
    QMutexLocker locker(&d_mutex);
    d_commands.clear();
}

Transmission::Status Transmission::status()
{
    QMutexLocker locker(&d_mutex);
    return d_status;
}
