#include "widget_main.h"
#include "ui_widget_main.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFileDialog>
#include <QMessageBox>

WidgetMain::WidgetMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    createGlobals();
    createWidgets();
    createLayouts();
    createConnections();
}

WidgetMain::~WidgetMain()
{
    delete ui;
}

void WidgetMain::saveConfig()
{
    QString fileName = d_hponic->configFilename();
    if (fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName(this, tr("Save config"),
                                                    "/",
                                                    tr("Config (*.hponic)"));
    if (fileName.isEmpty())
        return;

    bool err = false;
    if (!d_hponic->saveConfig(fileName))
        err = true;

    if (err)
        QMessageBox::critical(this, tr("Save config"),
                              tr("Errors occurred!\n"
                                 "Could not save config to file %1").arg(fileName),
                              QMessageBox::Ok);
}

void WidgetMain::saveConfigAs()
{

}

void WidgetMain::openConfig()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open config"),
                                                    "/",
                                                    tr("Config (*.hponic)"));
    if (fileName.isEmpty())
        return;

    bool err = false;
    if (!d_hponic->loadConfig(fileName))
        err = true;

    if (err)
        QMessageBox::critical(this, tr("Open config"),
                              tr("Errors occurred!\n"
                                 "Could not open config file %1\nWrong format").arg(fileName),
                              QMessageBox::Ok);
}

void WidgetMain::createGlobals()
{
    d_hponic = QSharedPointer<Hponic>(new Hponic);
}

void WidgetMain::createWidgets()
{
    d_widgetIoslots = new WidgetIoslots(d_hponic, this);
    d_widgetPrograms = new WidgetPrograms(d_hponic, this);
    d_widgetConfigTransmission = new WidgetConfigTransmission(d_hponic, this);
    d_widgetMonitoring = new WidgetMonitoring(d_hponic, this);
    d_widgetDatabase = new WidgetDatabase(d_hponic, this);
    d_widgetPlot = new WidgetPlot(d_hponic, this);


    ui->tabWidget->addTab(d_widgetIoslots, tr("IO slots"));
    ui->tabWidget->addTab(d_widgetPrograms, tr("Programs"));
    ui->tabWidget->addTab(d_widgetMonitoring, tr("Monitoring"));
    ui->tabWidget->addTab(d_widgetDatabase, tr("Database"));
    ui->tabWidget->addTab(d_widgetPlot, tr("Plot"));

    ui->mainToolBar->addWidget(d_widgetConfigTransmission);
}

void WidgetMain::createLayouts()
{
    setCentralWidget(ui->tabWidget);
}

void WidgetMain::createConnections()
{
    connect(ui->actionConfigSave, SIGNAL(triggered()), this, SLOT(saveConfig()), Qt::DirectConnection);
    connect(ui->actionConfigSaveAs, SIGNAL(triggered()), this, SLOT(saveConfigAs()), Qt::DirectConnection);
    connect(ui->actionConfigOpen, SIGNAL(triggered()), this, SLOT(openConfig()), Qt::DirectConnection);
}
