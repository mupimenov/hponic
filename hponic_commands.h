#ifndef HPONIC_COMMANDS_H
#define HPONIC_COMMANDS_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

#include <QSharedPointer>

#include "command.h"

class DownloadFileCommand : public QObject, public Command
{
    Q_OBJECT
public:
    DownloadFileCommand(QSharedPointer<Interface> interface, quint8 address, quint16 filenum, quint16 filesize, QObject *parent = 0);
    Result result() const;

    virtual Result send();

    const QByteArray &data() const;

Q_SIGNALS:
    void finished(DownloadFileCommand *cmd);

public Q_SLOTS:

private:
    Result d_result;
    quint8 d_address;
    quint16 d_filenum;
    quint16 d_filesize;
    QByteArray d_data;
};

class DownloadIoslotsCommand : public DownloadFileCommand
{
    Q_OBJECT
public:
    DownloadIoslotsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0);
};

class DownloadProgramsCommand : public DownloadFileCommand
{
    Q_OBJECT
public:
    DownloadProgramsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0);
};

/*
 * */

class UploadFileCommand : public QObject, public Command
{
    Q_OBJECT
public:
    UploadFileCommand(QSharedPointer<Interface> interface, quint8 address, quint16 filenum, quint16 filesize, const QByteArray &data, QObject *parent = 0);

    Result result() const;

    virtual Result send();

Q_SIGNALS:
    void finished(UploadFileCommand *cmd);

public Q_SLOTS:

private:
    Result d_result;
    quint8 d_address;
    quint16 d_filenum;
    quint16 d_filesize;
    QByteArray d_data;
};

class UploadIoslotsCommand : public UploadFileCommand
{
    Q_OBJECT
public:
    UploadIoslotsCommand(QSharedPointer<Interface> interface, quint8 address, const QByteArray &data, QObject *parent = 0);
};

class UploadProgramsCommand : public UploadFileCommand
{
    Q_OBJECT
public:
    UploadProgramsCommand(QSharedPointer<Interface> interface, quint8 address, const QByteArray &data, QObject *parent = 0);
};

/*
 * */

class ReadIoslotValuesCommand : public QObject, public Command
{
    Q_OBJECT
public:
    ReadIoslotValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0);

    Result result() const;

    virtual Result send();

    float valueFloat(int num) const;
    quint32 valueUInt(int num) const;

Q_SIGNALS:
    void finished(ReadIoslotValuesCommand *cmd);

public Q_SLOTS:
    void setAddress(quint8 address);

private:
    QSharedPointer<ReadInputRegistersCommand> d_cmd;
    Result d_result;
    quint8 d_address;

    static const int regsCount = 60 * 2;
};

/*
 * */

class ReadAdcValuesCommand : public QObject, public Command
{
    Q_OBJECT
public:
    ReadAdcValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0);

    Result result() const;

    virtual Result send();

    quint16 value(int channel) const;

Q_SIGNALS:
    void finished(ReadAdcValuesCommand *cmd);

public Q_SLOTS:
    void setAddress(quint8 address);

private:
    QSharedPointer<ReadInputRegistersCommand> d_cmd;
    Result d_result;
    quint8 d_address;

    static const int regsCount = 16;
};

/*
 * */

class ReadCommonValuesCommand : public QObject, public Command
{
    Q_OBJECT
public:
    ReadCommonValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0);

    Result result() const;

    virtual Result send();

    QDateTime clock() const;
    quint32 uptime() const;

Q_SIGNALS:
    void finished(ReadCommonValuesCommand *cmd);

public Q_SLOTS:
    void setAddress(quint8 address) ;

private:
    QSharedPointer<ReadHoldingRegistersCommand> d_cmd;
    Result d_result;
    quint8 d_address;

    static const int regsCount = 6;
};

/*
 * */

class SetTimeCommand : public QObject, public Command
{
    Q_OBJECT
public:
    SetTimeCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0);

    Result result() const;

    virtual Result send();

Q_SIGNALS:
    void finished(SetTimeCommand *cmd);

public Q_SLOTS:

private:
    Result d_result;
    quint8 d_address;
};

/*
 * */

class RestartProgramsCommand : public QObject, public Command
{
    Q_OBJECT
public:
    RestartProgramsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0);

    Result result() const;

    virtual Result send();

Q_SIGNALS:
    void finished(RestartProgramsCommand *cmd);

public Q_SLOTS:

private:
    Result d_result;
    quint8 d_address;
};

/*
 * */

class ProgramAddressCommand : public QObject, public Command
{
    Q_OBJECT
public:
    ProgramAddressCommand(QSharedPointer<Interface> interface, quint8 address, quint8 newAddress, QObject *parent = 0);

    Result result() const;
    quint8 newAddress() const;

    virtual Result send();

Q_SIGNALS:
    void finished(ProgramAddressCommand *cmd);

public Q_SLOTS:

private:
    Result d_result;
    quint8 d_address;
    quint8 d_newAddress;
};

#endif // HPONIC_COMMANDS_H
