#include "widget_database.h"
#include "ui_widget_database.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QFileDialog>
#include <QMessageBox>

WidgetDatabase::WidgetDatabase(QSharedPointer<Hponic> __hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDatabase),
    d_hponic(__hponic)
{
    ui->setupUi(this);

    createWidgets();
    createLayouts();
    createConnections();
}

WidgetDatabase::~WidgetDatabase()
{
    delete ui;
}

void WidgetDatabase::newDatabaseFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Create database file"),
                                                    "/",
                                                    tr("Sqlite3 database (*.sqlite3)"));
    if (fileName.isEmpty())
        return;

    bool err = false;
    if (!d_hponic->databaseTable()->init(fileName))
        err = true;

    if (err)
        QMessageBox::critical(this, tr("Create database file"),
                              tr("Errors occurred!\n"
                                 "Could not create file %1").arg(fileName),
                              QMessageBox::Ok);
    else
        ui->leDatabaseFile->setText(fileName);
}

void WidgetDatabase::openDatabaseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open database file"),
                                                    "/",
                                                    tr("Sqlite3 database (*.sqlite3)"));
    if (fileName.isEmpty())
        return;

    bool err = false;
    if (!d_hponic->databaseTable()->init(fileName))
        err = true;

    if (err)
        QMessageBox::critical(this, tr("Open database file"),
                              tr("Errors occurred!\n"
                                 "Could not open file %1\nWrong format").arg(fileName),
                              QMessageBox::Ok);
    else
        ui->leDatabaseFile->setText(fileName);
}

void WidgetDatabase::onDatabaseInserterStatusChanged(IoslotValueInserter::Status status)
{
    bool enable = (status == IoslotValueInserter::Stopped);
    ui->pbNew->setEnabled(enable);
    ui->pbOpen->setEnabled(enable);
    ui->tbStart->setEnabled(enable);
    ui->tbStop->setEnabled(!enable);
}

void WidgetDatabase::createWidgets()
{
    ui->sbPeriod->setValue(d_hponic->databaseProducer()->period());
}

void WidgetDatabase::createLayouts()
{
    QGridLayout *layoutSettings = new QGridLayout;
    int row = 0;
    layoutSettings->addWidget(ui->lPeriod, row, 0, 1, 1);
    layoutSettings->addWidget(ui->sbPeriod, row, 1, 1, 1);

    QHBoxLayout *layoutSettingsP = new QHBoxLayout;
    layoutSettingsP->addLayout(layoutSettings);
    layoutSettingsP->addStretch(1);

    ui->gbSettings->setLayout(layoutSettingsP);

    QHBoxLayout *layoutFile = new QHBoxLayout;
    layoutFile->addWidget(ui->leDatabaseFile, 1);

    QGridLayout *layoutDatabase = new QGridLayout;
    row = 0;
    layoutDatabase->addWidget(ui->lDatabaseFile,    row, 0, 1, 2, Qt::AlignLeft);
    layoutDatabase->addLayout(layoutFile,           row, 2, 1, 2, Qt::AlignLeft);
    layoutDatabase->addWidget(ui->pbNew,            row, 4, 1, 1, Qt::AlignLeft);
    layoutDatabase->addWidget(ui->pbOpen,           row, 5, 1, 1, Qt::AlignLeft);
    ++row;
    layoutDatabase->addWidget(ui->tbStart,          row, 0, 1, 1, Qt::AlignLeft);
    layoutDatabase->addWidget(ui->tbStop,           row, 1, 1, 1, Qt::AlignLeft);

    layoutDatabase->setColumnStretch(2, 1);

    ui->gbDatabase->setLayout(layoutDatabase);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addWidget(ui->gbSettings);
    layoutMain->addWidget(ui->gbDatabase);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetDatabase::createConnections()
{
    connect(ui->sbPeriod, SIGNAL(valueChanged(int)), d_hponic->databaseProducer().data(), SLOT(setPeriod(int)), Qt::DirectConnection);

    connect(ui->pbNew, SIGNAL(clicked(bool)), this, SLOT(newDatabaseFile()), Qt::DirectConnection);
    connect(ui->pbOpen, SIGNAL(clicked(bool)), this, SLOT(openDatabaseFile()), Qt::DirectConnection);

    connect(ui->tbStart, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(startDatabaseInserter()), Qt::DirectConnection);
    connect(ui->tbStop, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(stopDatabaseInserter()), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(databaseInserterStatusChanged(IoslotValueInserter::Status)),
            this, SLOT(onDatabaseInserterStatusChanged(IoslotValueInserter::Status)), Qt::DirectConnection);
}
