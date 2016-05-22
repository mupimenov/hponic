#include "widget_config_transmission.h"
#include "ui_widget_config_transmission.h"

#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>

#include <QSerialPortInfo>

WidgetConfigTransmission::WidgetConfigTransmission(QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConfigTransmission),
    d_hponic(hponic)
{
    ui->setupUi(this);
    setAutoFillBackground(true);

    createWidgets();
    createLayouts();
    createConnections();

    updatePortList();
}

WidgetConfigTransmission::~WidgetConfigTransmission()
{
    delete ui;
}

void WidgetConfigTransmission::setPort(const QString &port)
{
    ui->cbPort->setCurrentText(port);
}

void WidgetConfigTransmission::setAddress(quint8 address)
{
    ui->sbAddress->setValue(address);
}

void WidgetConfigTransmission::onTransmissionStatusChanged(Transmission::Status status)
{
    bool disable = status == Transmission::Started;
    ui->cbPort->setEnabled(!disable);
    ui->sbAddress->setEnabled(!disable);

    if (status == Transmission::Started) {
        ui->pbConnect->setText(tr("Disconnect"));
        ui->lStatus->setText(tr("Connected"));
    } else {
        ui->pbConnect->setText(tr("Connect"));
        ui->lStatus->setText(tr("Disconnected"));
    }
}

void WidgetConfigTransmission::updatePortList()
{
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    QString prevPortName = ui->cbPort->currentText();
    ui->cbPort->clear();
    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList)
        ui->cbPort->insertItem(ui->cbPort->count(), serialPortInfo.portName());
    ui->cbPort->setCurrentText(prevPortName);
}

void WidgetConfigTransmission::onPortChanged(const QString &port)
{
    PortSettings ps = d_hponic->portSettings();
    ps.portName = port;
    d_hponic->setPortSettings(ps);
}

void WidgetConfigTransmission::onAddressChanged(int address)
{
    d_hponic->setAddress(address);
}

void WidgetConfigTransmission::onAddressProgramStarted()
{
    ui->pbProgramAddress->setEnabled(false);
}

void WidgetConfigTransmission::onAddressProgramFinished(bool success, quint8 newAddress)
{
    ui->pbProgramAddress->setEnabled(true);

    if (success) {

    } else {
        QMessageBox::warning(this, tr("Program address"), tr("Error on program address"));
    }
}

void WidgetConfigTransmission::startStopTransmission()
{
    if (d_hponic->transmissionStatus() == Transmission::Stopped)
        d_hponic->startTransmission();
    else
        d_hponic->stopTransmission();
}

void WidgetConfigTransmission::programAddress()
{
    if (d_hponic->transmissionStatus() == Transmission::Started) {
        bool ok;
        int address = QInputDialog::getInt(this,
                                           tr("Program address"), tr("Type new controller address"),
                                           d_hponic->address(), 1, 247, 1, &ok);
        if (ok)
            d_hponic->programAddress(address);
    }
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
    layoutMain->addWidget(ui->pbProgramAddress);

    setLayout(layoutMain);
}

void WidgetConfigTransmission::createConnections()
{
    connect(d_hponic.data(), SIGNAL(transmissionStatusChanged(Transmission::Status)),
            this, SLOT(onTransmissionStatusChanged(Transmission::Status)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(addressProgramStarted()),
            this, SLOT(onAddressProgramStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(addressProgramFinished(bool,quint8)),
            this, SLOT(onAddressProgramFinished(bool,quint8)), Qt::DirectConnection);

    connect(ui->cbPort, SIGNAL(editTextChanged(QString)), this, SLOT(onPortChanged(QString)), Qt::DirectConnection);
    connect(ui->sbAddress, SIGNAL(valueChanged(int)), this, SLOT(onAddressChanged(int)), Qt::DirectConnection);
    connect(ui->pbConnect, SIGNAL(clicked(bool)), this, SLOT(startStopTransmission()), Qt::DirectConnection);
    connect(ui->pbProgramAddress, SIGNAL(clicked(bool)), this, SLOT(programAddress()), Qt::DirectConnection);
}
