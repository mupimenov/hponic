#include "hponic_commands.h"

#define DEFAULT_TIMEOOUT 3000

DownloadFileCommand::DownloadFileCommand(QSharedPointer<Interface> interface, quint8 address, quint16 filenum, quint16 filesize, QObject *parent) :
    QObject(parent),
    Command(interface, Single, DEFAULT_TIMEOOUT),
    d_address(address),
    d_filenum(filenum),
    d_filesize(filesize)
{

}

Command::Result DownloadFileCommand::result() const
{
    return d_result;
}

Command::Result DownloadFileCommand::send()
{
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

const QByteArray &DownloadFileCommand::data() const
{
    return d_data;
}

DownloadIoslotsCommand::DownloadIoslotsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent) :
      DownloadFileCommand(interface, address, 0x0001, (16 * 60), parent)
{

}

DownloadProgramsCommand::DownloadProgramsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent) :
      DownloadFileCommand(interface, address, 0x0002, (48 * 30), parent)
{

}

UploadFileCommand::UploadFileCommand(QSharedPointer<Interface> interface, quint8 address, quint16 filenum, quint16 filesize, const QByteArray &data, QObject *parent) :
    QObject(parent),
    Command(interface, Single, DEFAULT_TIMEOOUT),
    d_address(address),
    d_filenum(filenum),
    d_filesize(filesize),
    d_data(data)
{

}

Command::Result UploadFileCommand::result() const
{
    return d_result;
}

Command::Result UploadFileCommand::send()
{
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

UploadIoslotsCommand::UploadIoslotsCommand(QSharedPointer<Interface> interface, quint8 address, const QByteArray &data, QObject *parent) :
    UploadFileCommand(interface, address, 0x0001, (16 * 60), data, parent)
{

}

UploadProgramsCommand::UploadProgramsCommand(QSharedPointer<Interface> interface, quint8 address, const QByteArray &data, QObject *parent) :
    UploadFileCommand(interface, address, 0x0002, (48 * 30), data, parent)
{

}

ReadIoslotValuesCommand::ReadIoslotValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent) :
    QObject(parent),
    Command(interface, Multiple, DEFAULT_TIMEOOUT),
    d_address(address)
{

}

Command::Result ReadIoslotValuesCommand::result() const
{
    return d_result;
}

Command::Result ReadIoslotValuesCommand::send()
{
    d_cmd = QSharedPointer<ReadInputRegistersCommand>(
                new ReadInputRegistersCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x1000, regsCount));

    d_result = d_cmd->send();
    Q_EMIT finished(this);
    return d_result;
}

float ReadIoslotValuesCommand::valueFloat(int num) const
{
    const QVector<quint16> &r = d_cmd->inputRegisters();
    if (r.size() == regsCount) {
        int offset = (num) * 2;
        quint32 v = r[offset] + (r[offset + 1] << 16);
        float f;
        memcpy(&f, &v, sizeof(f));
        return f;
    }
    return NAN;
}

quint32 ReadIoslotValuesCommand::valueUInt(int num) const
{
    const QVector<quint16> &r = d_cmd->inputRegisters();
    if (r.size() == regsCount) {
        int offset = (num) * 2;
        quint32 value = (qint32)r[offset] + ((qint32)r[offset + 1] << 16);
        return value;
    }
    return 0;
}

void ReadIoslotValuesCommand::setAddress(quint8 address)
{
    d_address = address;
}

ReadAdcValuesCommand::ReadAdcValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent) :
    QObject(parent),
    Command(interface, Multiple, DEFAULT_TIMEOOUT),
    d_address(address)
{

}

Command::Result ReadAdcValuesCommand::result() const
{
    return d_result;
}

Command::Result ReadAdcValuesCommand::send()
{
    d_cmd = QSharedPointer<ReadInputRegistersCommand>(
                new ReadInputRegistersCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x0000, regsCount));

    d_result = d_cmd->send();
    Q_EMIT finished(this);
    return d_result;
}

quint16 ReadAdcValuesCommand::value(int channel) const
{
    const QVector<quint16> &r = d_cmd->inputRegisters();
    if (channel < 0 || channel > regsCount
            || r.size() != regsCount)
        return 0;

    return r[channel];
}

void ReadAdcValuesCommand::setAddress(quint8 address)
{
    d_address = address;
}

ReadCommonValuesCommand::ReadCommonValuesCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent) :
    QObject(parent),
    Command(interface, Multiple, DEFAULT_TIMEOOUT),
    d_address(address)
{

}

Command::Result ReadCommonValuesCommand::result() const
{
    return d_result;
}

Command::Result ReadCommonValuesCommand::send()
{
    d_cmd = QSharedPointer<ReadHoldingRegistersCommand>(
                new ReadHoldingRegistersCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x0000, regsCount));

    d_result = d_cmd->send();
    Q_EMIT finished(this);
    return d_result;
}

QDateTime ReadCommonValuesCommand::clock() const
{
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

quint32 ReadCommonValuesCommand::uptime() const
{
    const QVector<quint16> &r = d_cmd->holdingRegisters();
    if (r.size() == regsCount) {
        quint32 mseconds = ((quint32)r[4] + (quint32)(r[5] << 16));
        return mseconds;
    }
    return 0;
}

void ReadCommonValuesCommand::setAddress(quint8 address)
{
    d_address = address;
}

SetTimeCommand::SetTimeCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent) :
    QObject(parent),
    Command(interface, Single, DEFAULT_TIMEOOUT),
    d_address(address)
{

}

Command::Result SetTimeCommand::result() const
{
    return d_result;
}

Command::Result SetTimeCommand::send()
{
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

RestartProgramsCommand::RestartProgramsCommand(QSharedPointer<Interface> interface, quint8 address, QObject *parent) :
    QObject(parent),
    Command(interface, Single, DEFAULT_TIMEOOUT),
    d_address(address)
{

}

Command::Result RestartProgramsCommand::result() const
{
    return d_result;
}

Command::Result RestartProgramsCommand::send()
{
    d_cmd = QSharedPointer<WriteSingleCoilCommand>(
                new WriteSingleCoilCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x0000, 0x01));

    d_result = d_cmd->send();
    Q_EMIT finished(this);
    return d_result;
}

ProgramAddressCommand::ProgramAddressCommand(QSharedPointer<Interface> interface, quint8 address, quint8 newAddress, QObject *parent) :
    QObject(parent),
    Command(interface, Single, DEFAULT_TIMEOOUT),
    d_address(address),
    d_newAddress(newAddress)
{

}

Command::Result ProgramAddressCommand::result() const
{
    return d_result;
}

quint8 ProgramAddressCommand::newAddress() const
{
    return d_newAddress;
}

Command::Result ProgramAddressCommand::send()
{
    d_cmd = QSharedPointer<WriteSingleRegisterCommand>(
                new WriteSingleRegisterCommand(d_interface, d_rythm, DEFAULT_TIMEOOUT, d_address, 0x000C, d_newAddress));

    d_result = d_cmd->send();
    Q_EMIT finished(this);
    return d_result;
}
