#include "widget_monitoring.h"
#include "ui_widget_monitoring.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "monitoring_model.h"
#include "monitoring_filter_proxy.h"
#include "monitoring_delegate.h"

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
    quint32 seconds = d_hponic->monitoring()->uptime() / 1000;

    ui->lDateTime->setText(dt.toString("dd-MM-yyyy hh:mm:ss"));
    quint32 hours = seconds / 3600UL;
    quint32 minutes = (seconds - (hours * 3600UL)) / 60UL;
    quint32 seconds2 = seconds - (hours * 3600UL) - (minutes * 60UL);

    ui->lUptime->setText(tr("Uptime %1:%2:%3").arg(hours, 2, 10, QChar('0'))
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

void WidgetMonitoring::onProgramsRestartStarted()
{
    ui->tbRestartPrograms->setEnabled(false);
}

void WidgetMonitoring::onProgramsRestartFinished(bool success)
{
    ui->tbRestartPrograms->setEnabled(true);

    if (!success) {

    } else {

    }
}

void WidgetMonitoring::createWidgets()
{
    // Input

    d_InputSplitter = new QSplitter(Qt::Horizontal, this);

    MonitoringModel *inputModel1 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *inputProxy1 = new MonitoringFilterProxy(d_hponic->monitoring(), 0, 19, MonitoringFilterProxy::InputIoslots);
    inputProxy1->setSourceModel(inputModel1);

    ui->tvInputSlots1->setModel(inputProxy1);
    ui->tvInputSlots1->setItemDelegate(new MonitoringDelegate(this));

    MonitoringModel *inputModel2 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *inputProxy2 = new MonitoringFilterProxy(d_hponic->monitoring(), 20, 39, MonitoringFilterProxy::InputIoslots);
    inputProxy2->setSourceModel(inputModel2);

    ui->tvInputSlots2->setModel(inputProxy2);
    ui->tvInputSlots2->setItemDelegate(new MonitoringDelegate(this));

    MonitoringModel *inputModel3 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *inputProxy3 = new MonitoringFilterProxy(d_hponic->monitoring(), 40, 59, MonitoringFilterProxy::InputIoslots);
    inputProxy3->setSourceModel(inputModel3);

    ui->tvInputSlots3->setModel(inputProxy3);
    ui->tvInputSlots3->setItemDelegate(new MonitoringDelegate(this));

    // Output

    d_OutputSplitter = new QSplitter(Qt::Horizontal, this);

    MonitoringModel *outputModel1 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *outputProxy1 = new MonitoringFilterProxy(d_hponic->monitoring(), 0, 19, MonitoringFilterProxy::OutputIoslots);
    outputProxy1->setSourceModel(outputModel1);

    ui->tvOutputSlots1->setModel(outputProxy1);
    ui->tvOutputSlots1->setItemDelegate(new MonitoringDelegate(this));

    MonitoringModel *outputModel2 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *outputProxy2 = new MonitoringFilterProxy(d_hponic->monitoring(), 20, 39, MonitoringFilterProxy::OutputIoslots);
    outputProxy2->setSourceModel(outputModel2);

    ui->tvOutputSlots2->setModel(outputProxy2);
    ui->tvOutputSlots2->setItemDelegate(new MonitoringDelegate(this));

    MonitoringModel *outputModel3 = new MonitoringModel(d_hponic->monitoring());
    MonitoringFilterProxy *outputProxy3 = new MonitoringFilterProxy(d_hponic->monitoring(), 40, 59, MonitoringFilterProxy::OutputIoslots);
    outputProxy3->setSourceModel(outputModel3);

    ui->tvOutputSlots3->setModel(outputProxy3);
    ui->tvOutputSlots3->setItemDelegate(new MonitoringDelegate(this));
}

void WidgetMonitoring::createMenu()
{

}

void WidgetMonitoring::createLayouts()
{
    QVBoxLayout *layoutControls = new QVBoxLayout;
    layoutControls->addWidget(ui->tbSetTime);
    layoutControls->addWidget(ui->tbRestartPrograms);
    layoutControls->addStretch(1);

    QHBoxLayout *layoutTime = new QHBoxLayout;
    layoutTime->addWidget(ui->lDateTime);
    layoutTime->addWidget(ui->lUptime);
    layoutTime->addStretch(1);
    layoutTime->addLayout(layoutControls);

    ui->frame->setLayout(layoutTime);

    d_InputSplitter->addWidget(ui->tvInputSlots1);
    d_InputSplitter->addWidget(ui->tvInputSlots2);
    d_InputSplitter->addWidget(ui->tvInputSlots3);

    d_OutputSplitter->addWidget(ui->tvOutputSlots1);
    d_OutputSplitter->addWidget(ui->tvOutputSlots2);
    d_OutputSplitter->addWidget(ui->tvOutputSlots3);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addWidget(ui->frame);
    layoutMain->addWidget(ui->lInputSlots, 0, Qt::AlignCenter);
    layoutMain->addWidget(d_InputSplitter, 1);
    layoutMain->addWidget(ui->lOutputSlots, 0, Qt::AlignCenter);
    layoutMain->addWidget(d_OutputSplitter, 1);

    setLayout(layoutMain);
}

void WidgetMonitoring::createConnections()
{
    connect(ui->tbSetTime, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(setTime()), Qt::DirectConnection);
    connect(ui->tbRestartPrograms, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(restartPrograms()), Qt::DirectConnection);

    connect(d_hponic->monitoring().data(), SIGNAL(commonValuesUpdated()), this, SLOT(onCommonValuesUpdated()), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(timeSetStarted()), this, SLOT(onTimeSetStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(timeSetFinished(bool)), this, SLOT(onTimeSetFinished(bool)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(programsRestartStarted()), this, SLOT(onProgramsRestartStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(programsRestartFinished(bool)), this, SLOT(onProgramsRestartFinished(bool)), Qt::DirectConnection);
}
