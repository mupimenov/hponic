#include "widget_config_transmission.h"
#include "ui_widget_config_transmission.h"

#include <QHBoxLayout>

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

    refreshPorts();
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

    highlight(status == Transmission::Started? Connected: Disconnected);
}

void WidgetConfigTransmission::onCommonValuesNotUpdated(Command::Result result)
{
    Q_UNUSED(result);

    highlight(Error);
}

void WidgetConfigTransmission::onCommonValuesUpdated()
{
    highlight(Normal);
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
    connect(d_hponic->monitoring().data(), SIGNAL(commonValuesNotUpdated(Command::Result)),
            this, SLOT(onCommonValuesNotUpdated(Command::Result)), Qt::DirectConnection);
    connect(d_hponic->monitoring().data(), SIGNAL(commonValuesUpdated()),
            this, SLOT(onCommonValuesUpdated()), Qt::DirectConnection);

    connect(ui->cbPort, SIGNAL(editTextChanged(QString)), this, SLOT(onPortChanged(QString)), Qt::DirectConnection);
    connect(ui->sbAddress, SIGNAL(valueChanged(int)), this, SLOT(onAddressChanged(int)), Qt::DirectConnection);
    connect(ui->pbConnect, SIGNAL(clicked(bool)), this, SLOT(startStopTransmission()), Qt::DirectConnection);
}

void WidgetConfigTransmission::highlight(WidgetConfigTransmission::HighlightState state)
{
    static QColor defaultColor = palette().color(backgroundRole());

    switch (state)
    {
    case Connected:
    case Normal:
    {
        QPalette p(palette());
        p.setColor(backgroundRole(), QColor("#c5ffa0"));
        setPalette(p);
        break;
    }
    case Disconnected:
    {
        QPalette p(palette());
        p.setColor(backgroundRole(), defaultColor);
        setPalette(p);
        break;
    }
    case Error:
    {
        QPalette p(palette());
        p.setColor(backgroundRole(), QColor("#ff9595"));
        setPalette(p);
        break;
    }
    }
}
