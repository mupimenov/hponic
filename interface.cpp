#include "interface.h"

SerialInterface::SerialInterface(const PortSettings &settings, QObject *parent) :
    Interface(parent),
    d_port(new QSerialPort(this)),
    d_settings(settings)
{

}

SerialInterface::~SerialInterface()
{

}

bool SerialInterface::open()
{
    d_port->setPortName(d_settings.portName);

    if (d_port->open(QIODevice::ReadWrite) == true) {
        if (d_port->setBaudRate(d_settings.baudRate)
                && d_port->setDataBits(d_settings.dataBits)
                && d_port->setFlowControl(d_settings.flowControl)
                && d_port->setParity(d_settings.parity)
                && d_port->setStopBits(d_settings.stopBits)) {
            return true;
        }
    }
    return false;
}

void SerialInterface::close()
{
    d_port->close();
}

qint64 SerialInterface::bytesAvailable()
{
    return d_port->bytesAvailable();
}

qint64 SerialInterface::read(char *data, qint64 maxlen)
{
    return d_port->read(data, maxlen);
}

qint64 SerialInterface::write(const char *data, qint64 len)
{
    return d_port->write(data, len);
}

bool SerialInterface::isOpen()
{
    return d_port->isOpen();
}

void SerialInterface::setPortSettings(const PortSettings &settings)
{
    d_settings = settings;
}

const PortSettings &SerialInterface::portSettings()
{
    return d_settings;
}
