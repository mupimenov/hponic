#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QSerialPort>
#include <QSharedPointer>

#include "serialport_settings.h"

class Interface : public QObject
{
    Q_OBJECT

public:
    explicit Interface(QObject *parent = 0) : QObject(parent) {}
    virtual ~Interface() {}

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual qint64 bytesAvailable() = 0;
    virtual qint64 read(char *data, qint64 maxlen) = 0;
    virtual qint64 write(const char *data, qint64 len) = 0;
    virtual bool isOpen() = 0;

Q_SIGNALS:

public Q_SLOTS:

};

class SerialInterface : public Interface
{
public:
    SerialInterface(const PortSettings &settings = PortSettings(), QObject *parent = 0);
    virtual ~SerialInterface();

    virtual bool open();
    virtual void close();
    virtual qint64 bytesAvailable();
    virtual qint64 read(char *data, qint64 maxlen);
    virtual qint64 write(const char *data, qint64 len);
    virtual bool isOpen();

    void setPortSettings(const PortSettings &settings);
    const PortSettings &portSettings();

private:
    QSharedPointer<QSerialPort> d_port;
    PortSettings                d_settings;
};

#endif // INTERFACE_H
