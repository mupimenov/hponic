#ifndef COMMAND_H
#define COMMAND_H

#include <QtCore>
#include <QVector>
#include <QMutex>

#include <QSharedPointer>

class Interface;

class Command {
public:
    enum Result
    {
        Ok,
        Timeout,
        Aborted,
        BadChecksum,

        IllegalFunction,
        IllegalDataAddress,
        IllegalDataValue,

        Unknown
    };

    enum Rhythm
    {
        Single,
        Multiple
    };

    Command(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout);
    virtual ~Command();

    virtual void abort();
    Rhythm rythm();

    virtual Result send() = 0;

protected:
    void addCrc(QByteArray &cmd);
    bool checkCrc(QByteArray &answ);

    void sendCmd(const QByteArray &cmd);
    Result recvAnsw(QByteArray &answ, quint64 len);

    QSharedPointer<Interface> d_interface;
    Rhythm              d_rythm;
    qint64              d_timeout;

    bool                d_abort;
    QMutex              d_mutex;
};

/* BITS */

class ReadBitsCommand : public Command {
public:
    ReadBitsCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                    quint8 address, quint8 function, quint16 startNumber, quint16 count);
    virtual ~ReadBitsCommand();

    virtual Result send();

protected:
    quint8              d_address;
    quint8              d_function;
    quint16             d_startNumber;
    quint16             d_count;

    QVector<quint8>     d_bits;
};

class ReadCoilsCommand : public ReadBitsCommand {
public:
    ReadCoilsCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                     quint8 address, quint16 startNumber, quint16 count);
    virtual ~ReadCoilsCommand();

    const QVector<quint8> &coils();
};

class ReadDiscreteInputsCommand : public ReadBitsCommand {
public:
    ReadDiscreteInputsCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                              quint8 address, quint16 startNumber, quint16 count);
    virtual ~ReadDiscreteInputsCommand();

    const QVector<quint8> &discreteInputs();
};

class WriteSingleCoilCommand : public Command {
public:
    WriteSingleCoilCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                     quint8 address, quint16 startNumber, quint8 state);
    virtual ~WriteSingleCoilCommand();

    virtual Result send();

private:
    quint8              d_address;
    quint16             d_startNumber;
    quint8              d_state;
};

class WriteMultipleCoilsCommand : public Command {
public:
    WriteMultipleCoilsCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                     quint8 address, quint16 startNumber, const QList<quint8> &coils);
    virtual ~WriteMultipleCoilsCommand();

    virtual Result send();

private:
    quint8              d_address;
    quint16             d_startNumber;
    QList<quint8>       d_coils;
};

/* REGS */

class ReadRegistersCommand : public Command {
public:
    ReadRegistersCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                         quint8 address, quint8 function, quint16 startNumber, quint16 count);
    virtual ~ReadRegistersCommand();

    virtual Result send();

protected:
    quint8              d_address;
    quint8              d_function;
    quint16             d_startNumber;
    quint16             d_count;

    QVector<quint16>    d_regs;
};

class ReadInputRegistersCommand : public ReadRegistersCommand {
public:
    ReadInputRegistersCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                              quint8 address, quint16 startNumber, quint16 count);
    virtual ~ReadInputRegistersCommand();

    const QVector<quint16> &inputRegisters();
};

class ReadHoldingRegistersCommand : public ReadRegistersCommand {
public:
    ReadHoldingRegistersCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                              quint8 address, quint16 startNumber, quint16 count);
    virtual ~ReadHoldingRegistersCommand();

    const QVector<quint16> &holdingRegisters();
};

class WriteSingleRegisterCommand : public Command {
public:
    WriteSingleRegisterCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                         quint8 address, quint16 startNumber, quint16 value);
    virtual ~WriteSingleRegisterCommand();

    virtual Result send();

private:
    quint8              d_address;
    quint16             d_startNumber;
    quint16             d_value;
};

class WriteMultipleRegistersCommand : public Command {
public:
    WriteMultipleRegistersCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                     quint8 address, quint16 startNumber, const QList<quint16> &regs);
    virtual ~WriteMultipleRegistersCommand();

    virtual Result send();

private:
    quint8              d_address;
    quint16             d_startNumber;
    QList<quint16>      d_regs;
};

/* Read General Reference */

class ReadGeneralReferenceCommand : public Command {
public:
    ReadGeneralReferenceCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                         quint8 address, quint16 filenum, quint16 startAddress, quint16 count);
    virtual ~ReadGeneralReferenceCommand();

    virtual Result send();

    const QByteArray &data() const;

protected:
    quint8              d_address;
    quint16             d_filenum;
    quint16             d_startAddress;
    quint16             d_count;
    QByteArray          d_data;
};

/* Write General Reference */

class WriteGeneralReferenceCommand : public Command {
public:
    WriteGeneralReferenceCommand(QSharedPointer<Interface> interface, Rhythm rythm, qint64 timeout,
                         quint8 address, quint16 filenum, quint16 startAddress, const QByteArray &data);
    virtual ~WriteGeneralReferenceCommand();

    virtual Result send();

protected:
    quint8              d_address;
    quint16             d_filenum;
    quint16             d_startAddress;
    QByteArray          d_data;
};

#endif // COMMAND_H
