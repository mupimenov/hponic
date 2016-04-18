#include "widget_monitoring.h"
#include "ui_widget_monitoring.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "monitoring_model.h"
#include "monitoring_filter_proxy.h"

WidgetMonitoring::WidgetMonitoring(QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMonitoring),
    d_hponic(hponic)
{
    ui->setupUi(this);

    createWidgets();
    createMenu();
    createLayouts();
    createConnections();
}

WidgetMonitoring::~WidgetMonitoring()
{
    delete ui;
}

void WidgetMonitoring::onCommonValuesUpdated()
{
    QDateTime dt = d_hponic->monitoring()->clock();
    quint32 seconds = d_hponic->monitoring()->uptime();

    ui->lDateTime->setText(dt.toString("dd-MM-yyyy hh:mm:ss"));
    quint32 hours = seconds / 3600UL;
    quint32 minutes = (seconds - (hours * 3600UL)) / 60UL;
    quint32 seconds2 = seconds - (hours * 3600UL) - (minutes * 60UL);

    ui->lUptime->setText(QString("Uptime %1:%2:%3").arg(hours, 2, 10, QChar('0'))
                         .arg(minutes, 2, 10, QChar('0'))
                         .arg(seconds2, 2, 10, QChar('0')));
}

void WidgetMonitoring::onTimeSetStarted()
{
    ui->tbSetTime->setEnabled(false);
}

void WidgetMonitoring::onTimeSetFinished(bool success)
{
    ui->tbSetTime->setEnabled(true);

    if (!success) {

    } else {

    }
}

void WidgetMonitoring::createWidgets()
{
    d_splitter = new QSplitter(Qt::Horizontal, this);

    MonitoringModel *model1 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *proxy1 = new MonitoringFilterProxy(d_hponic->monitoring(), 0, 19);
    proxy1->setSourceModel(model1);

    ui->tvInputs1->setModel(proxy1);

    MonitoringModel *model2 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *proxy2 = new MonitoringFilterProxy(d_hponic->monitoring(), 20, 39);
    proxy2->setSourceModel(model2);

    ui->tvInputs2->setModel(proxy2);

    MonitoringModel *model3 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *proxy3 = new MonitoringFilterProxy(d_hponic->monitoring(), 40, 59);
    proxy3->setSourceModel(model3);

    ui->tvInputs3->setModel(proxy3);
}

void WidgetMonitoring::createMenu()
{

}

void WidgetMonitoring::createLayouts()
{
    QVBoxLayout *layoutControls = new QVBoxLayout;
    layoutControls->addWidget(ui->tbSetTime);
    layoutControls->addStretch(1);

    QHBoxLayout *layoutTime = new QHBoxLayout;
    layoutTime->addWidget(ui->lDateTime);
    layoutTime->addWidget(ui->lUptime);
    layoutTime->addStretch(1);
    layoutTime->addLayout(layoutControls);

    ui->frame->setLayout(layoutTime);

    d_splitter->addWidget(ui->tvInputs1);
    d_splitter->addWidget(ui->tvInputs2);
    d_splitter->addWidget(ui->tvInputs3);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addWidget(ui->frame);
    layoutMain->addWidget(d_splitter, 1);

    setLayout(layoutMain);
}

void WidgetMonitoring::createConnections()
{
    connect(ui->tbSetTime, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(setTime()), Qt::DirectConnection);

    connect(d_hponic->monitoring().data(), SIGNAL(commonValuesUpdated()), this, SLOT(onCommonValuesUpdated()), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(timeSetStarted()), this, SLOT(onTimeSetStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(timeSetFinished(bool)), this, SLOT(onTimeSetFinished(bool)), Qt::DirectConnection);
}
