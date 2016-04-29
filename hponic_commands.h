#ifndef HPONIC_COMMANDS_H
#define HPONIC_COMMANDS_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

#include <QSharedPointer>

#include "command.h"

#define DEFAULT_TIMEOOUT 2000

class DownloadFileCommand : public QObject, public Command
{
    Q_OBJECT
public:
    DownloadFileCommand(QSharedPointer<Interface> interface, quint8 address, quint16 filenum, quint16 filesize, QObject *parent = 0) :
        QObject(parent),
        Command(interface, Single, DEFAULT_TIMEOOUT),
        d_address(address),
        d_filenum(filenum),
        d_filesize(filesize) {

    }

    Result result() {
        return d_result;
    }

    virtual Result send() {
        const quint16 chunk = 200;
        const quint16 count = d_filesize / 2;

        d_data.clear();

        for (quint16 offset = 0; offset < count;) {
            quint16 c = chunk / 2;
            if (offset + c > count)
                c = count - offset;

            d_cmd = QSharedPointer<ReadGeneralReferenceCommand>(
                    new ReadGeneralReferenceCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, d_filenum, offset, c));

            d_result = d_cmd->send();
            if (d_result != Command::Ok) break;

            d_data.append(d_cmd->data());

            offset += c;
        }

        Q_EMIT finished(this);
        return d_result;
    }

    const QByteArray &data() const {
        return d_data;
    }

Q_SIGNALS:
    void finished(DownloadFileCommand *cmd);

public Q_SLOTS:

private:
    QSharedPointer<ReadGeneralReferenceCommand> d_cmd;
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
    DownloadIoslotsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0) :
          DownloadFileCommand(interface, address, 0x0001, (16 * 60), parent) {

    }
};

class DownloadProgramsCommand : public DownloadFileCommand
{
    Q_OBJECT
public:
    DownloadProgramsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0) :
          DownloadFileCommand(interface, address, 0x0002, (48 * 30), parent) {

    }
};

/*
 * */

class UploadFileCommand : public QObject, public Command
{
    Q_OBJECT
public:
    UploadFileCommand(QSharedPointer<Interface> interface, quint8 address, quint16 filenum, quint16 filesize, const QByteArray &data, QObject *parent = 0) :
        QObject(parent),
        Command(interface, Single, DEFAULT_TIMEOOUT),
        d_address(address),
        d_filenum(filenum),
        d_filesize(filesize),
        d_data(data) {

    }

    Result result() {
        return d_result;
    }

    virtual Result send() {
        const quint16 chunk = 200;
        const quint16 count = d_filesize / 2;

        if (d_data.size() < d_filesize)
            d_data.append(QByteArray(d_filesize - d_data.size(), char(0)));

        for (quint16 offset = 0; offset < count;) {
            quint16 c = chunk / 2;
            if (offset + c > count)
                c = count - offset;

            QByteArray d(d_data.mid(offset * 2, c * 2));
            d_cmd = QSharedPointer<WriteGeneralReferenceCommand>(
                    new WriteGeneralReferenceCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, d_filenum, offset, d));

            d_result = d_cmd->send();
            if (d_result != Command::Ok) break;

            offset += c;
        }

        Q_EMIT finished(this);
        return d_result;
    }

Q_SIGNALS:
    void finished(UploadFileCommand *cmd);

public Q_SLOTS:

private:
    QSharedPointer<WriteGeneralReferenceCommand> d_cmd;
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
    UploadIoslotsCommand(QSharedPointer<Interface> interface, quint8 address, const QByteArray &data, QObject *parent = 0) :
        UploadFileCommand(interface, address, 0x0001, (16 * 60), data, parent) {

    }
};

class UploadProgramsCommand : public UploadFileCommand
{
    Q_OBJECT
public:
    UploadProgramsCommand(QSharedPointer<Interface> interface, quint8 address, const QByteArray &data, QObject *parent = 0) :
        UploadFileCommand(interface, address, 0x0002, (48 * 30), data, parent) {

    }
};

/*
 * */

class ReadIoslotValuesCommand : public QObject, public Command
{
    Q_OBJECT
public:
    ReadIoslotValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0) :
        QObject(parent),
        Command(interface, Multiple, DEFAULT_TIMEOOUT),
        d_address(address) {
    }

    Result result() {
        return d_result;
    }

    virtual Result send() {
        d_cmd = QSharedPointer<ReadInputRegistersCommand>(
                    new ReadInputRegistersCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x1000, regsCount));

        d_result = d_cmd->send();
        Q_EMIT finished(this);
        return d_result;
    }

    float valueFloat(int num) const {
        const QVector<quint16> &r = d_cmd->inputRegisters();
        if (r.size() == regsCount) {
            int offset = (num) * 2;
            quint32 v = (qint32)r[offset] + ((qint32)r[offset + 1] << 16);
            float f;
            memcpy(&f, &v, sizeof(f));
            return f;
        }
        return NAN;
    }

    quint32 valueUInt(int num) const {
        const QVector<quint16> &r = d_cmd->inputRegisters();
        if (r.size() == regsCount) {
            int offset = (num) * 2;
            quint32 value = (qint32)r[offset] + ((qint32)r[offset + 1] << 16);
            return value;
        }
        return 0;
    }

Q_SIGNALS:
    void finished(ReadIoslotValuesCommand *cmd);

public Q_SLOTS:
    void setAddress(quint8 address) {
        d_address = address;
    }

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
    ReadAdcValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0) :
        QObject(parent),
        Command(interface, Multiple, DEFAULT_TIMEOOUT),
        d_address(address) {
    }

    Result result() {
        return d_result;
    }

    virtual Result send() {
        d_cmd = QSharedPointer<ReadInputRegistersCommand>(
                    new ReadInputRegistersCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x0000, regsCount));

        d_result = d_cmd->send();
        Q_EMIT finished(this);
        return d_result;
    }

    quint16 value(int channel) const {
        const QVector<quint16> &r = d_cmd->inputRegisters();
        if (channel < 0 || channel > regsCount
                || r.size() != regsCount)
            return 0;

        return r[channel];
    }

Q_SIGNALS:
    void finished(ReadAdcValuesCommand *cmd);

public Q_SLOTS:
    void setAddress(quint8 address) {
        d_address = address;
    }

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
    ReadCommonValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0) :
        QObject(parent),
        Command(interface, Multiple, DEFAULT_TIMEOOUT),
        d_address(address) {
    }

    Result result() {
        return d_result;
    }

    virtual Result send() {
        d_cmd = QSharedPointer<ReadHoldingRegistersCommand>(
                    new ReadHoldingRegistersCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x0000, regsCount));

        d_result = d_cmd->send();
        Q_EMIT finished(this);
        return d_result;
    }


    QDateTime clock() const {
        const QVector<quint16> &r = d_cmd->holdingRegisters();
        if (r.size() == regsCount) {
            quint8 seconds = r[0] & 0xFF;
            quint8 minutes = r[0] >> 8;
            quint8 hours = r[1] & 0xFF;
            quint8 day = r[1] >> 8;
            quint8 month = r[2] & 0xFF;
            quint8 year = r[2] >> 8;

            return QDateTime(QDate(year + 2000, month, day), QTime(hours, minutes, seconds));
        }
        return QDateTime();
    }

    quint32 uptime() const {
        const QVector<quint16> &r = d_cmd->holdingRegisters();
        if (r.size() == regsCount) {
            quint32 mseconds = ((quint32)r[4] + (quint32)(r[5] << 16));
            return mseconds;
        }
        return 0;
    }

Q_SIGNALS:
    void finished(ReadCommonValuesCommand *cmd);

public Q_SLOTS:
    void setAddress(quint8 address) {
        d_address = address;
    }

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
    SetTimeCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0) :
        QObject(parent),
        Command(interface, Single, DEFAULT_TIMEOOUT),
        d_address(address) {
    }

    Result result() {
        return d_result;
    }

    virtual Result send() {
        QDateTime dt = QDateTime::currentDateTime();

        QList<quint16> regs;
        regs.append(quint16(dt.time().second()) | (quint16(dt.time().minute()) << 8));
        regs.append(quint16(dt.time().hour()) | (quint16(dt.date().day()) << 8));
        regs.append(quint16(dt.date().month()) | (quint16(dt.date().year() % 100) << 8));

        d_cmd = QSharedPointer<WriteMultipleRegistersCommand>(
                    new WriteMultipleRegistersCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x0000, regs));

        d_result = d_cmd->send();
        Q_EMIT finished(this);
        return d_result;
    }

Q_SIGNALS:
    void finished(SetTimeCommand *cmd);

public Q_SLOTS:

private:
    QSharedPointer<WriteMultipleRegistersCommand> d_cmd;
    Result d_result;
    quint8 d_address;
};

/*
 * */

class RestartProgramsCommand : public QObject, public Command
{
    Q_OBJECT
public:
    RestartProgramsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent = 0) :
        QObject(parent),
        Command(interface, Single, DEFAULT_TIMEOOUT),
        d_address(address) {
    }

    Result result() {
        return d_result;
    }

    virtual Result send() {
        d_cmd = QSharedPointer<WriteSingleCoilCommand>(
                    new WriteSingleCoilCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x0000, 0x01));

        d_result = d_cmd->send();
        Q_EMIT finished(this);
        return d_result;
    }

Q_SIGNALS:
    void finished(RestartProgramsCommand *cmd);

public Q_SLOTS:

private:
    QSharedPointer<WriteSingleCoilCommand> d_cmd;
    Result d_result;
    quint8 d_address;
};

#endif // HPONIC_COMMANDS_H
