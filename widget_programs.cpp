#include "widget_programs.h"
#include "ui_widget_programs.h"

#include <QMenu>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "programs_model.h"
#include "widget_config_program.h"

WidgetPrograms::WidgetPrograms(QSharedPointer<Hponic> __hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPrograms),
    d_hponic(__hponic)
{
    ui->setupUi(this);

    createWidgets();
    createMenu();
    createLayouts();
    createConnections();
}

WidgetPrograms::~WidgetPrograms()
{
    delete ui;
}

#define setProgramType(programClass) \
    QItemSelectionModel *selection = ui->tvPrograms->selectionModel(); \
    QModelIndexList indexes = selection->selectedRows(); \
    if (indexes.count() == 0) \
        return; \
    foreach (QModelIndex index, indexes) { \
        programClass *program = new programClass(index.row() + 1); \
        QSharedPointer<Program> ptr(program); \
        d_hponic->programManager()->updateProgram(index.row(), ptr); \
        swapWidgetConfigProgram(new WidgetConfig##programClass(ProgramConv::toProgram<programClass>(ptr), d_hponic, this)); \
    }

void WidgetPrograms::setUnknownProgramType()
{
    setProgramType(EmptyProgram);
}

void WidgetPrograms::setTimerControlType()
{
    setProgramType(TimerControlProgram);
}

void WidgetPrograms::setRelayControlType()
{
    setProgramType(RelayControlProgram);
}

void WidgetPrograms::setPidControlType()
{
    setProgramType(PidControlProgram);
}

void WidgetPrograms::onProgramActivated(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    Program *ptr = static_cast<Program*>(index.internalPointer());
    QSharedPointer<Program> program = d_hponic->programManager()->programById(ptr->id());

    if (d_selectedProgram != program) {
        d_selectedProgram = program;

        QWidget *widget = 0;
        switch (program->type()) {
        case EmptyProgramType:
        {
            QSharedPointer<EmptyProgram> emptyProgram = ProgramConv::toProgram<EmptyProgram>(program);
            if (emptyProgram)
                widget = new WidgetConfigEmptyProgram(emptyProgram, d_hponic, this);
            break;
        }
        case TimerControlType:
        {
            QSharedPointer<TimerControlProgram> timerControlProgram = ProgramConv::toProgram<TimerControlProgram>(program);
            if (timerControlProgram)
                widget = new WidgetConfigTimerControlProgram(timerControlProgram, d_hponic, this);
            break;
        }
        case RelayControlType:
        {
            QSharedPointer<RelayControlProgram> relayControlProgram = ProgramConv::toProgram<RelayControlProgram>(program);
            if (relayControlProgram)
                widget = new WidgetConfigRelayControlProgram(relayControlProgram, d_hponic, this);
            break;
        }
        case PidControlType:
        {
            QSharedPointer<PidControlProgram> pidControlProgram = ProgramConv::toProgram<PidControlProgram>(program);
            if (pidControlProgram)
                widget = new WidgetConfigPidControlProgram(pidControlProgram, d_hponic, this);
            break;
        }
        default:
            break;
        }

        swapWidgetConfigProgram(widget);
    }
}

void WidgetPrograms::onProgramsDownloadStarted()
{
    ui->tbDownload->setEnabled(false);
}

void WidgetPrograms::onProgramsDownloadFinished(bool success)
{
    if (!success) {
        ui->lStatus->setText(tr("Programs download failed"));
    } else {
        ui->lStatus->setText("");
    }

    ui->tbDownload->setEnabled(true);
}

void WidgetPrograms::onProgramsUploadStarted()
{
    ui->tbUpload->setEnabled(false);
}

void WidgetPrograms::onProgramsUploadFinished(bool success)
{
    if (!success) {
        ui->lStatus->setText(tr("Programs upload failed"));
    } else {
        ui->lStatus->setText("");
    }

    ui->tbUpload->setEnabled(true);
}

void WidgetPrograms::createWidgets()
{
    ui->tvPrograms->setModel(new ProgramsModel(d_hponic->programManager(), this));
    d_widgetConfigProgram = new QWidget(this);

    d_splitter = new QSplitter(Qt::Horizontal, this);
}

void WidgetPrograms::createMenu()
{
    QMenu *menuSetProgramType = new QMenu;
    menuSetProgramType->addAction(ui->actionSetUnknownProgramType);
    menuSetProgramType->addSeparator();
    menuSetProgramType->addAction(ui->actionSetTimerControlType);
    menuSetProgramType->addAction(ui->actionSetRelayControlType);
    menuSetProgramType->addAction(ui->actionSetPidControlType);

    ui->tbSetProgramType->setMenu(menuSetProgramType);
}

void WidgetPrograms::createLayouts()
{
    QHBoxLayout *layoutControls = new QHBoxLayout;
    layoutControls->addWidget(ui->tbSetProgramType);
    layoutControls->addStretch(1);
    layoutControls->addWidget(ui->tbDownload);
    layoutControls->addWidget(ui->tbUpload);

    QVBoxLayout *layoutPrograms = new QVBoxLayout;
    layoutPrograms->addLayout(layoutControls);
    layoutPrograms->addWidget(ui->lStatus, 0, Qt::AlignRight);
    layoutPrograms->addWidget(d_widgetConfigProgram, 1);

    ui->scrollArea->setLayout(layoutPrograms);

    d_splitter->addWidget(ui->tvPrograms);
    d_splitter->addWidget(ui->scrollArea);

    QHBoxLayout *layoutMain = new QHBoxLayout;
    layoutMain->addWidget(d_splitter);

    setLayout(layoutMain);
}

void WidgetPrograms::createConnections()
{
    connect(ui->actionSetUnknownProgramType, SIGNAL(triggered()), this, SLOT(setUnknownProgramType()), Qt::DirectConnection);

    connect(ui->actionSetTimerControlType, SIGNAL(triggered()), this, SLOT(setTimerControlType()), Qt::DirectConnection);
    connect(ui->actionSetRelayControlType, SIGNAL(triggered()), this, SLOT(setRelayControlType()), Qt::DirectConnection);
    connect(ui->actionSetPidControlType, SIGNAL(triggered()), this, SLOT(setPidControlType()), Qt::DirectConnection);

    connect(ui->tbDownload, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(downloadPrograms()), Qt::DirectConnection);
    connect(ui->tbUpload, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(uploadPrograms()), Qt::DirectConnection);

    connect(ui->tvPrograms, SIGNAL(clicked(QModelIndex)), this, SLOT(onProgramActivated(QModelIndex)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(programsDownloadStarted()), this, SLOT(onProgramsDownloadStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(programsDownloadFinished(bool)), this, SLOT(onProgramsDownloadFinished(bool)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(programsUploadStarted()), this, SLOT(onProgramsUploadStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(programsUploadFinished(bool)), this, SLOT(onProgramsUploadFinished(bool)), Qt::DirectConnection);
}

void WidgetPrograms::swapWidgetConfigProgram(QWidget *__widget)
{
    if (__widget == 0)
        return;

    if (d_widgetConfigProgram) {
        ui->scrollArea->layout()->replaceWidget(d_widgetConfigProgram, __widget);
        d_widgetConfigProgram->deleteLater();
    }

    d_widgetConfigProgram = __widget;
    d_widgetConfigProgram->show();
}
