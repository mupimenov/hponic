#include "command.h"

#include <QDebug>

#include "interface.h"

#define MODBUS_READ_COILS               0x01
#define MODBUS_READ_DISCRETE_INPUTS     0x02
#define MODBUS_READ_HOLDING_REGISTERS   0x03
#define MODBUS_READ_INPUT_REGISTERS     0x04
#define MODBUS_WRITE_SINGLE_COIL        0x05
#define MODBUS_WRITE_SINGLE_REGISTER    0x06
#define MODBUS_READ_EXCEPTION_STATUS    0x07
#define MODBUS_WRITE_MULTIPLE_COILS     0x0F
#define MODBUS_WRITE_MULTIPLE_REGISTERS 0x10
#define MODBUS_REPORT_SLAVE_ID          0x11
#define MODBUS_READ_GENERAL_REFERENCE   0x14
#define MODBUS_WRITE_GENERAL_REFERENCE  0x15
#define MODBUS_MASK_WRITE_REGISTER 		0x16
#define MODBUS_WRITE_AND_READ_REGISTERS 0x17

#define MODBUS_ERROR_MESSAGE_LEN        0x05
#define MODBUS_READ_CMD_EXTRA_LEN       0x05

static const quint8 table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static const quint8 table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static quint16 crc16(const quint8 *buffer, qint16 buffer_length)
{
    quint8 crc_hi = 0xFF; /* high CRC byte initialized */
    quint8 crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--)
    {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}


#define MODBUS_ERROR_ILLEGAL_FUNCTION 		0x01
#define MODBUS_ERROR_ILLEGAL_DATA_ADDRESS 	0x02
#define MODBUS_ERROR_ILLEGAL_DATA_VALUE 	0x03
#define MODBUS_ERROR_SERVICE_DEVICE_FAILURE 0x04
#define MODBUS_ERROR_ACKNOWLEDGE 			0x05
#define MODBUS_ERROR_SERVER_DEVICE_BUSY 	0x06
#define MODBUS_ERROR_MEMORY_PARITY_ERROR 	0x08

static Command::Result getResult(quint8 code)
{
    switch (code) {
    case MODBUS_ERROR_ILLEGAL_FUNCTION:
        return Command::IllegalFunction;
    case MODBUS_ERROR_ILLEGAL_DATA_ADDRESS:
        return Command::IllegalDataAddress;
    case MODBUS_ERROR_ILLEGAL_DATA_VALUE:
        return Command::IllegalDataValue;
    default:
        break;
    }

    return Command::Unknown;
}

Command::Command(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout) :
    d_interface(interface),
    d_rythm(rythm),
    d_timeout(timeout),
    d_abort(false)
{

}

Command::~Command()
{

}

void Command::abort()
{
    QMutexLocker locker(&d_mutex);
    d_abort = true;
}

Command::Rhythm Command::rythm()
{
    return d_rythm;
}

void Command::addCrc(QByteArray &cmd)
{
    quint16 crc = crc16((quint8*)cmd.data(), cmd.size());
    cmd.append(crc >> 8);
    cmd.append(crc & 0xFF);
}

bool Command::checkCrc(QByteArray &answ)
{
    quint16 calculatedCrc = crc16((quint8*)answ.data(), answ.size() - 2);
    quint16 receivedCrc = (quint8)(answ.at(answ.size() - 1)) +
            ((quint8)(answ.at(answ.size() - 2)) << 8);
    return (calculatedCrc == receivedCrc);
}

void Command::sendCmd(const QByteArray &cmd)
{
    d_interface->write(cmd.data(), cmd.size());
}

Command::Result Command::recvAnsw(QByteArray &answ, quint64 len)
{
    qint64 to = d_timeout;
    while (1) {
        qint64 available = d_interface->bytesAvailable();
        if (available) {
            QVector<char> r(available);
            d_interface->read(r.data(), available);
            answ.append(r.data(), available);
        }

        if (quint64(answ.size()) >= len)
            return Ok;

        if (to) {
            --to;
            QEventLoop loop;
            QTimer timer;
            timer.singleShot(1, &loop, SLOT(quit()));
            loop.exec();
        } else {
            return Timeout;
        }

        {
            QMutexLocker locker(&d_mutex);
            if (d_abort)
                return Aborted;
        }
    }
    return Unknown;
}

/* BITS */

ReadBitsCommand::ReadBitsCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                 quint8 address, quint8 function, quint16 startNumber, quint16 count) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_function(function),
    d_startNumber(startNumber),
    d_count(count)
{

}

ReadBitsCommand::~ReadBitsCommand()
{

}

Command::Result ReadBitsCommand::send()
{
    QByteArray cmd;

    cmd.append(d_address);
    cmd.append(d_function);
    cmd.append(d_startNumber >> 8);
    cmd.append(d_startNumber & 0xFF);
    cmd.append(d_count >> 8);
    cmd.append(d_count & 0xFF);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = (d_count * 2);

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb + MODBUS_READ_CMD_EXTRA_LEN);
        if (res == Ok) {
            if (checkCrc(answ)) {
                quint16 offset = 3;

                d_bits.clear();

                for (quint16 i = 0; i < d_count; i++) {
                    /* shift reg hi_byte to temp OR with lo_byte */
                    quint16 data = ((quint8)answ.at(offset + 2 * i) << 8) |
                        (quint8)answ.at(offset + 2 * i + 1);
                    d_bits.append(data);
                }
            } else {
                res = BadChecksum;
            }
        }
    }

    return res;
}

ReadCoilsCommand::ReadCoilsCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                   quint8 address, quint16 startNumber, quint16 count) :
    ReadBitsCommand(interface, rythm, timeout, address, MODBUS_READ_COILS, startNumber, count)
{

}

ReadCoilsCommand::~ReadCoilsCommand()
{

}

const QVector<quint8> &ReadCoilsCommand::coils()
{
    return d_bits;
}

ReadDiscreteInputsCommand::ReadDiscreteInputsCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                                                     quint8 address, quint16 startNumber, quint16 count) :
    ReadBitsCommand(interface, rythm, timeout, address, MODBUS_READ_DISCRETE_INPUTS, startNumber, count)
{

}

ReadDiscreteInputsCommand::~ReadDiscreteInputsCommand()
{

}

const QVector<quint8> &ReadDiscreteInputsCommand::discreteInputs()
{
    return d_bits;
}

WriteSingleCoilCommand::WriteSingleCoilCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                   quint8 address, quint16 startNumber, quint8 state) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_startNumber(startNumber),
    d_state(state)
{

}

WriteSingleCoilCommand::~WriteSingleCoilCommand()
{

}

Command::Result WriteSingleCoilCommand::send()
{
    QByteArray cmd;
    quint16 value = d_state == 1? 0xFF00: 0xFFFF;

    cmd.append(d_address);
    cmd.append(MODBUS_WRITE_SINGLE_COIL);
    cmd.append(d_startNumber >> 8);
    cmd.append(d_startNumber & 0xFF);
    cmd.append(value >> 8);
    cmd.append(value & 0xFF);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = cmd.size();

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb);
        if (res == Ok) {
            if (checkCrc(answ)) {
                quint16 startNumber_ = ((quint8)answ.at(2) << 8) |
                        (quint8)answ.at(3);
                quint16 value_ = ((quint8)answ.at(4) << 8) |
                        (quint8)answ.at(5);
                if (answ.at(1) != MODBUS_WRITE_SINGLE_COIL
                        || startNumber_ != d_startNumber
                        || value_ != value)
                    res = Unknown;
            } else {
                res = BadChecksum;
            }
        }
    }

    return res;
}

/* REGS */

ReadRegistersCommand::ReadRegistersCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                           quint8 address, quint8 function, quint16 startNumber, quint16 count) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_function(function),
    d_startNumber(startNumber),
    d_count(count)
{

}

ReadRegistersCommand::~ReadRegistersCommand()
{

}

Command::Result ReadRegistersCommand::send()
{
    QByteArray cmd;

    cmd.append(d_address);
    cmd.append(d_function);
    cmd.append(d_startNumber >> 8);
    cmd.append(d_startNumber & 0xFF);
    cmd.append(d_count >> 8);
    cmd.append(d_count & 0xFF);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = (d_count * 2);

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            qDebug() << answ.size() << QString::number(quint8(answ.at(0)), 16)
                    << QString::number(quint8(answ.at(1)), 16)
                    << QString::number(quint8(answ.at(2)), 16)
                    << QString::number(quint8(answ.at(3)), 16)
                    << QString::number(quint8(answ.at(4)), 16);

            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb + MODBUS_READ_CMD_EXTRA_LEN);
        if (res == Ok) {
            if (checkCrc(answ)) {
                quint16 offset = 3;

                d_regs.clear();

                for (quint16 i = 0; i < d_count; i++) {
                    /* shift reg hi_byte to temp OR with lo_byte */
                    quint16 data = ((quint8)answ.at(offset + 2 * i) << 8) |
                        (quint8)answ.at(offset + 2 * i + 1);
                    d_regs.append(data);
                }
            } else {
                res = BadChecksum;
            }
        }
    }

    qDebug() << answ.size();
    if (res == Timeout && answ.size() >= 5) {
        qDebug() << answ.size() << QString::number(quint8(answ.at(0)), 16)
                << QString::number(quint8(answ.at(1)), 16)
                << QString::number(quint8(answ.at(2)), 16)
                << QString::number(quint8(answ.at(3)), 16)
                << QString::number(quint8(answ.at(4)), 16);
    }

    return res;
}

ReadInputRegistersCommand::ReadInputRegistersCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                                     quint8 address, quint16 startNumber, quint16 count) :
    ReadRegistersCommand(interface, rythm, timeout, address, MODBUS_READ_INPUT_REGISTERS, startNumber, count)
{

}

ReadInputRegistersCommand::~ReadInputRegistersCommand()
{

}

const QVector<quint16> &ReadInputRegistersCommand::inputRegisters()
{
    return d_regs;
}

ReadHoldingRegistersCommand::ReadHoldingRegistersCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                                         quint8 address, quint16 startNumber, quint16 count) :
    ReadRegistersCommand(interface, rythm, timeout, address, MODBUS_READ_HOLDING_REGISTERS, startNumber, count)
{

}

ReadHoldingRegistersCommand::~ReadHoldingRegistersCommand()
{

}

const QVector<quint16> &ReadHoldingRegistersCommand::holdingRegisters()
{
    return d_regs;
}

WriteSingleRegisterCommand::WriteSingleRegisterCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                                       quint8 address, quint16 startNumber, quint16 value) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_startNumber(startNumber),
    d_value(value)
{

}

WriteSingleRegisterCommand::~WriteSingleRegisterCommand()
{

}

Command::Result WriteSingleRegisterCommand::send()
{
    QByteArray cmd;

    cmd.append(d_address);
    cmd.append(MODBUS_WRITE_SINGLE_REGISTER);
    cmd.append(d_startNumber >> 8);
    cmd.append(d_startNumber & 0xFF);
    cmd.append(d_value >> 8);
    cmd.append(d_value & 0xFF);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = cmd.size();

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb);
        if (res == Ok) {
            if (checkCrc(answ)) {
                quint16 startNumber_ = ((quint8)answ.at(2) << 8) |
                        (quint8)answ.at(3);
                quint16 value_ = ((quint8)answ.at(4) << 8) |
                        (quint8)answ.at(5);
                if (answ.at(0) != d_address
                        || answ.at(1) != MODBUS_WRITE_SINGLE_REGISTER
                        || startNumber_ != d_startNumber
                        || value_ != d_value)
                    res = Unknown;
            } else {
                res = BadChecksum;
            }
        }
    }

    return res;
}

WriteMultipleCoilsCommand::WriteMultipleCoilsCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                                     quint8 address, quint16 startNumber, const QList<quint8> &coils) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_startNumber(startNumber),
    d_coils(coils)
{

}

WriteMultipleCoilsCommand::~WriteMultipleCoilsCommand()
{

}

Command::Result WriteMultipleCoilsCommand::send()
{
    QByteArray values;
    quint8 v = 0;
    int j = 0;
    for (int i = 0; i < d_coils.size(); ++i) {
        if (d_coils[i] == 1)
            v |= (1 << j);
        else
            v &= ~(1 << j);

        j++;
        j %= 8;

        if (j == 0)
            values.append(v);
    }

    if (j)
        values.append(v);

    quint16 count = d_coils.size();
    quint8 bytes = values.size();

    QByteArray cmd;

    cmd.append(d_address);
    cmd.append(MODBUS_WRITE_MULTIPLE_COILS);
    cmd.append(d_startNumber >> 8);
    cmd.append(d_startNumber & 0xFF);
    cmd.append(count >> 8);
    cmd.append(count & 0xFF);
    cmd.append(bytes);
    cmd.append(values);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = cmd.size();

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb);
        if (res == Ok) {
            if (checkCrc(answ)) {
                quint16 startNumber_ = ((quint8)answ.at(2) << 8) |
                        (quint8)answ.at(3);
                quint16 count_ = ((quint8)answ.at(4) << 8) |
                        (quint8)answ.at(5);
                if (answ.at(0) != d_address
                        || answ.at(1) != MODBUS_WRITE_MULTIPLE_COILS
                        || startNumber_ != d_startNumber
                        || count_ != count)
                    res = Unknown;
            } else {
                res = BadChecksum;
            }
        }
    }

    return res;
}

WriteMultipleRegistersCommand::WriteMultipleRegistersCommand(QSharedPointer<Interface> interface, Command::Rhythm rythm, qint64 timeout,
                                                             quint8 address, quint16 startNumber, const QList<quint16> &regs) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_startNumber(startNumber),
    d_regs(regs)
{

}

WriteMultipleRegistersCommand::~WriteMultipleRegistersCommand()
{

}

Command::Result WriteMultipleRegistersCommand::send()
{
    QByteArray values;
    for (int i = 0; i < d_regs.size(); ++i) {
        values.append(d_regs[i] >> 8);
        values.append(d_regs[i] & 0xFF);
    }

    quint16 count = d_regs.size();
    quint8 bytes = values.size();

    QByteArray cmd;

    cmd.append(d_address);
    cmd.append(MODBUS_WRITE_MULTIPLE_REGISTERS);
    cmd.append(d_startNumber >> 8);
    cmd.append(d_startNumber & 0xFF);
    cmd.append(count >> 8);
    cmd.append(count & 0xFF);
    cmd.append(bytes);
    cmd.append(values);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = cmd.size();

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb);
        if (res == Ok) {
            if (checkCrc(answ)) {
                quint16 startNumber_ = ((quint8)answ.at(2) << 8) |
                        (quint8)answ.at(3);
                quint16 count_ = ((quint8)answ.at(4) << 8) |
                        (quint8)answ.at(5);
                if (answ.at(0) != d_address
                        || answ.at(1) != MODBUS_WRITE_MULTIPLE_REGISTERS
                        || startNumber_ != d_startNumber
                        || count_ != count)
                    res = Unknown;
            } else {
                res = BadChecksum;
            }
        }
    }

    return res;
}

ReadGeneralReferenceCommand::ReadGeneralReferenceCommand(QSharedPointer<Interface> interface,
                                                         Command::Rhythm rythm,
                                                         qint64 timeout,
                                                         quint8 address, quint16 filenum, quint16 startAddress, quint16 count) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_filenum(filenum),
    d_startAddress(startAddress),
    d_count(count)
{

}

ReadGeneralReferenceCommand::~ReadGeneralReferenceCommand()
{

}

Command::Result ReadGeneralReferenceCommand::send()
{
    QByteArray cmd;

    cmd.append(d_address);
    cmd.append(MODBUS_READ_GENERAL_REFERENCE);
    cmd.append(7);
    cmd.append(0x06);
    cmd.append(d_filenum >> 8);
    cmd.append(d_filenum & 0xFF);
    cmd.append(d_startAddress >> 8);
    cmd.append(d_startAddress & 0xFF);
    cmd.append(d_count >> 8);
    cmd.append(d_count & 0xFF);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = d_count * 2;

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb + 0x05 + 0x02);
        if (res == Ok) {
            if (checkCrc(answ)) {
                const quint16 offset = 5;

                d_data.clear();

                for (quint16 i = 0; i < d_count; i++) {
                    /* shift reg hi_byte to temp OR with lo_byte */
                    quint16 data = ((quint8)answ.at(offset + 2 * i) << 8) |
                        (quint8)answ.at(offset + 2 * i + 1);
                    d_data.append(data & 0xFF);
                    d_data.append(data >> 8);
                }
            } else {
                res = BadChecksum;
            }
        }
    }

    qDebug() << answ.size();

    return res;
}

const QByteArray &ReadGeneralReferenceCommand::data() const
{
    return d_data;
}

WriteGeneralReferenceCommand::WriteGeneralReferenceCommand(QSharedPointer<Interface> interface,
                                                           Command::Rhythm rythm, qint64 timeout,
                                                           quint8 address, quint16 filenum,
                                                           quint16 startAddress, const QByteArray &data) :
    Command(interface, rythm, timeout),
    d_address(address),
    d_filenum(filenum),
    d_startAddress(startAddress),
    d_data(data)
{

}

WriteGeneralReferenceCommand::~WriteGeneralReferenceCommand()
{

}

Command::Result WriteGeneralReferenceCommand::send()
{
    QByteArray values;
    for (int i = 0; i < d_data.size() / 2; ++i) {
        values.append(d_data[2*i + 1]);
        values.append(d_data[2*i]);
    }

    quint16 count = d_data.size() / 2;
    quint8 bytes = values.size();

    QByteArray cmd;

    cmd.append(d_address);
    cmd.append(MODBUS_WRITE_GENERAL_REFERENCE);
    cmd.append(bytes + 0x07);
    cmd.append(0x06);
    cmd.append(d_filenum >> 8);
    cmd.append(d_filenum & 0xFF);
    cmd.append(d_startAddress >> 8);
    cmd.append(d_startAddress & 0xFF);
    cmd.append(count >> 8);
    cmd.append(count & 0xFF);
    cmd.append(values);
    addCrc(cmd);

    sendCmd(cmd);

    quint16 nb = cmd.size();

    QByteArray answ;
    Result res = recvAnsw(answ, MODBUS_ERROR_MESSAGE_LEN);
    if (res == Ok) {
        if (answ.size() == MODBUS_ERROR_MESSAGE_LEN && checkCrc(answ)) {
            if (quint8(answ.at(1)) >= 0x80)
                return getResult(quint8(answ.at(2)));
            return Unknown;
        }

        res = recvAnsw(answ, nb);
        if (res == Ok) {
            if (checkCrc(answ)) {
                if (answ.at(0) != d_address
                        || answ.at(1) != MODBUS_WRITE_GENERAL_REFERENCE)
                    res = Unknown;
            } else {
                res = BadChecksum;
            }
        }
    }

    return res;
}
