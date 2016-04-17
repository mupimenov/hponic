#include "widget_config_transmission.h"
#include "ui_widget_config_transmission.h"

#include <QHBoxLayout>

#include <QSerialPortInfo>

WidgetConfigTransmission::WidgetConfigTransmission(QSharedPointer<Hponic> __hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConfigTransmission),
    d_hponic(__hponic)
{
    ui->setupUi(this);

    createWidgets();
    createLayouts();
    createConnections();

    refreshPorts();
}

WidgetConfigTransmission::~WidgetConfigTransmission()
{
    delete ui;
}

void WidgetConfigTransmission::onTransmissionStatusChanged(Transmission::Status __status)
{
    bool disable = __status == Transmission::Started;
    ui->cbPort->setEnabled(!disable);
    ui->sbAddress->setEnabled(!disable);

    if (__status == Transmission::Started) {
        ui->pbConnect->setText(tr("Disconnect"));
        ui->lStatus->setText(tr("Connected"));
    } else {
        ui->pbConnect->setText(tr("Connect"));
        ui->lStatus->setText(tr("Disconnected"));
    }
}

void WidgetConfigTransmission::refreshPorts()
{
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    QString prevPortName = ui->cbPort->currentText();
    ui->cbPort->clear();
    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList)
        ui->cbPort->insertItem(ui->cbPort->count(), serialPortInfo.portName());
    ui->cbPort->setCurrentText(prevPortName);
}

void WidgetConfigTransmission::onPortChanged(const QString &__port)
{
    PortSettings ps = d_hponic->portSettings();
    ps.portName = __port;
    d_hponic->setPortSettings(ps);
}

void WidgetConfigTransmission::onAddressChanged(int __address)
{
    d_hponic->setAddress(__address);
}

void WidgetConfigTransmission::startStopTransmission()
{
    if (d_hponic->transmissionStatus() == Transmission::Stopped)
        d_hponic->startTransmission();
    else
        d_hponic->stopTransmission();
}

void WidgetConfigTransmission::createWidgets()
{
    ui->cbPort->setCurrentText(d_hponic->portSettings().portName);
}

void WidgetConfigTransmission::createLayouts()
{
    QHBoxLayout *layoutMain = new QHBoxLayout;
    layoutMain->addWidget(ui->lPort);
    layoutMain->addWidget(ui->cbPort);
    layoutMain->addWidget(ui->lAddress);
    layoutMain->addWidget(ui->sbAddress);
    layoutMain->addWidget(ui->pbConnect);
    layoutMain->addWidget(ui->lStatus, 1);

    setLayout(layoutMain);
}

void WidgetConfigTransmission::createConnections()
{
    connect(d_hponic.data(), SIGNAL(transmissionStatusChanged(Transmission::Status)),
            this, SLOT(onTransmissionStatusChanged(Transmission::Status)), Qt::DirectConnection);

    connect(ui->cbPort, SIGNAL(editTextChanged(QString)), this, SLOT(onPortChanged(QString)), Qt::DirectConnection);
    connect(ui->sbAddress, SIGNAL(valueChanged(int)), this, SLOT(onAddressChanged(int)), Qt::DirectConnection);
    connect(ui->pbConnect, SIGNAL(clicked(bool)), this, SLOT(startStopTransmission()), Qt::DirectConnection);
}
