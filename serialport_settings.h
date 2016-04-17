#ifndef SERIALPORT_SETTINGS_H
#define SERIALPORT_SETTINGS_H

#include <QSerialPort>

struct PortSettings {
    QString                     portName;
    qint32                      baudRate;
    QSerialPort::FlowControl    flowControl;
    QSerialPort::Parity         parity;
    QSerialPort::DataBits       dataBits;
    QSerialPort::StopBits       stopBits;
    long                        timeoutMillisec;

    PortSettings() {
        portName =          QString("");
        baudRate =          QSerialPort::Baud9600;
        flowControl =       QSerialPort::NoFlowControl;
        parity =            QSerialPort::NoParity;
        dataBits =          QSerialPort::Data8;
        stopBits =          QSerialPort::OneStop;
        timeoutMillisec =   -1L;
    }

    PortSettings(const PortSettings &__ps) {
        portName =          __ps.portName;
        baudRate =          __ps.baudRate;
        flowControl =       __ps.flowControl;
        parity =            __ps.parity;
        dataBits =          __ps.dataBits;
        stopBits =          __ps.stopBits;
        timeoutMillisec =   __ps.timeoutMillisec;
    }

    PortSettings &operator=(const PortSettings &__ps) {
        if (this != &__ps) {
            portName =          __ps.portName;
            baudRate =          __ps.baudRate;
            flowControl =       __ps.flowControl;
            parity =            __ps.parity;
            dataBits =          __ps.dataBits;
            stopBits =          __ps.stopBits;
            timeoutMillisec =   __ps.timeoutMillisec;
        }

        return *this;
    }
};

#endif // SERIALPORT_SETTINGS_H
