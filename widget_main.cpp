#include "widget_main.h"
#include "ui_widget_main.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFileDialog>
#include <QMessageBox>

#include "version.h"

WidgetMain::WidgetMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    createGlobals();
    createWidgets();
    createLayouts();
    createConnections();

    QSettings settings;
    int w = settings.value("window/width", width()).toInt();
    int h = settings.value("window/height", height()).toInt();
    bool maximized = settings.value("window/maximized", false).toBool();
    d_widgetConfigTransmission->setPort(settings.value("transmission/port", "COM1").toString());
    d_widgetConfigTransmission->setAddress(settings.value("transmission/address", QVariant(uint(1))).toUInt());

    if (maximized)
        setWindowState(Qt::WindowMaximized);
    else
        resize(w, h);
}

WidgetMain::~WidgetMain()
{
    delete ui;
}

bool WidgetMain::event(QEvent *event)
{
    if (event->type() == QEvent::Close) {
        if (d_hponic->transmissionStatus() == Transmission::Started) {
            event->ignore();
            return true;
        }

        if (1) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle(tr("Hydroponic configurator closing"));
            msgBox.setText(tr("Do you want to save configuration?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Cancel) {
                event->ignore();
                return true;
            }

            if (ret == QMessageBox::Yes)
                saveConfig();
        }

        QSettings settings;
        settings.setValue("window/width", width());
        settings.setValue("window/height", height());
        settings.setValue("window/maximized", isMaximized());
        settings.setValue("transmission/port", d_hponic->portSettings().portName);
        settings.setValue("transmission/address", d_hponic->address());
    }

    return QMainWindow::event(event);
}

void WidgetMain::saveConfig()
{
    saveConfigImpl(d_hponic->configFilename());
}

void WidgetMain::saveConfigAs()
{
    saveConfigImpl(QString());
}

void WidgetMain::openConfig()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open config"),
                                                    "/",
                                                    tr("Config (*.hponic)"));
    if (filename.isEmpty())
        return;

    bool err = false;
    if (!d_hponic->loadConfig(filename))
        err = true;

    if (err) {
        QMessageBox::critical(this, tr("Open config"),
                              tr("Open error occurred!\n"
                                 "Could not open config file %1").arg(filename),
                              QMessageBox::Ok);
    } else {
        setWindowTitle(tr("Hydroponic system configurator (%1)").arg(filename));
        statusBar()->showMessage(tr("Config file opened"), 2000);
    }
}

void WidgetMain::showAbout()
{
    QMessageBox::about(this, tr("About"),
                       tr("<p><b>Hydroponic system configurator (%1)</b><p>"
                          "<p>Author: Mikhail Pimenov (mupimenov@gmail.com)</p>"
                          "<p>2016</p>").arg(VERSION));
}

void WidgetMain::onCommonValuesNotUpdated(Command::Result result)
{
    const int timeout = 2000;

    switch (result) {
    case Command::Timeout:
        statusBar()->showMessage(tr("Timeout"), timeout);
        break;
    case Command::Aborted:
        statusBar()->showMessage(tr("Aborted"), timeout);
        break;
    case Command::BadChecksum:
        statusBar()->showMessage(tr("Bad checksum"), timeout);
        break;
    default:
        statusBar()->showMessage(tr("Protocol error"), timeout);
        break;
    }
}

void WidgetMain::onExportStarted()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
}

void WidgetMain::onExportStopped()
{
    ui->progressBar->setVisible(false);
}

void WidgetMain::onExportProgress(int offset, int count)
{
    ui->progressBar->setValue(offset / count);
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
    ui->tabWidget->addTab(d_widgetMonitoring, QIcon("://icons/layouts_three_grid.png"), tr("Monitoring"));
    ui->tabWidget->addTab(d_widgetDatabase, QIcon("://icons/database.png"), tr("Database"));
    ui->tabWidget->addTab(d_widgetPlot, QIcon("://icons/chart_curve.png"), tr("Plot"));

    ui->mainToolBar->addWidget(d_widgetConfigTransmission);

    statusBar()->addPermanentWidget(ui->progressBar);
    ui->progressBar->setVisible(false);
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
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()), Qt::DirectConnection);

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()), Qt::DirectConnection);
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()), Qt::DirectConnection);

    connect(d_hponic->monitoring().data(), SIGNAL(commonValuesNotUpdated(Command::Result)),
            this, SLOT(onCommonValuesNotUpdated(Command::Result)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(exportStarted()), this, SLOT(onExportStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(exportStopped()), this, SLOT(onExportStopped()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(exportProgress(int,int)), this, SLOT(onExportProgress(int,int)), Qt::DirectConnection);
}

void WidgetMain::saveConfigImpl(const QString &currentFilename)
{
    QString filename = currentFilename;
    if (filename.isEmpty())
        filename = QFileDialog::getSaveFileName(this, tr("Save config"),
                                                    "/",
                                                    tr("Config (*.hponic)"));
    if (filename.isEmpty())
        return;

    bool err = false;
    if (!d_hponic->saveConfig(filename))
        err = true;

    if (err) {
        QMessageBox::critical(this, tr("Save config"),
                              tr("Save error occurred!\n"
                                 "Could not save config to file %1").arg(filename),
                              QMessageBox::Ok);
    } else {
        setWindowTitle(tr("Hydroponic system configurator (%1)").arg(filename));
        statusBar()->showMessage(tr("Config file saved"), 2000);
    }
}
