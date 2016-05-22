#include "widget_programs.h"
#include "ui_widget_programs.h"

#include <QMenu>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "programs_model.h"
#include "widget_config_program.h"

WidgetPrograms::WidgetPrograms(QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPrograms),
    d_hponic(hponic)
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

#define setProgram(programClass) \
    QItemSelectionModel *selection = ui->tvPrograms->selectionModel(); \
    QModelIndexList indexes = selection->selectedRows(); \
    if (indexes.count() == 0) \
        return; \
    foreach (QModelIndex index, indexes) { \
        programClass *program = new programClass(index.row() + 1); \
        QSharedPointer<Program> ptr(program); \
        d_hponic->programManager()->replaceProgram(index.row(), ptr); \
        d_selectedProgram = ptr; \
        swapWidgetConfigProgram(new WidgetConfig##programClass(ProgramConv::toProgram<programClass>(ptr), d_hponic, this)); \
    }

void WidgetPrograms::setUnknownProgramType()
{
    setProgram(EmptyProgram);
}

void WidgetPrograms::setTimerControlType()
{
    setProgram(TimerControlProgram);
}

void WidgetPrograms::setRelayControlType()
{
    setProgram(RelayControlProgram);
}

void WidgetPrograms::setPidControlType()
{
    setProgram(PidControlProgram);
}

void WidgetPrograms::setButtonControlType()
{
    setProgram(ButtonControlProgram);
}

void WidgetPrograms::onSetProgramTypeClicked()
{
    bool enable = ui->tvPrograms->selectionModel()->hasSelection();

    ui->actionSetTimerControlProgram->setEnabled(enable);
    ui->actionSetRelayControlProgram->setEnabled(enable);
    ui->actionSetPidControlProgram->setEnabled(enable);
    ui->actionSetButtonControlProgram->setEnabled(enable);

    ui->actionSetUnknownProgram->setEnabled(enable);
}

void WidgetPrograms::showContextMenu(QPoint)
{
    QMenu menu;
    menu.addAction(ui->actionResetPrograms);

    menu.exec(QCursor::pos());
}

void WidgetPrograms::onProgramCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    QSharedPointer<Program> program = d_hponic->programManager()->program(current.row());
    if (!program)
        return;

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
        case ButtonControlType:
        {
            QSharedPointer<ButtonControlProgram> buttonControlProgram = ProgramConv::toProgram<ButtonControlProgram>(program);
            if (buttonControlProgram)
                widget = new WidgetConfigButtonControlProgram(buttonControlProgram, d_hponic, this);
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
    ui->tbDownload->setEnabled(true);

    if (!success) {
        QMessageBox::warning(this, tr("Download program list"), tr("Error on download program list from controller"));
    }
}

void WidgetPrograms::onProgramsUploadStarted()
{
    ui->tbUpload->setEnabled(false);
}

void WidgetPrograms::onProgramsUploadFinished(bool success)
{
    ui->tbUpload->setEnabled(true);

    if (!success) {
        QMessageBox::warning(this, tr("Upload program list"), tr("Error on upload program list to controller"));
    }
}

void WidgetPrograms::createWidgets()
{
    ui->tvPrograms->setModel(new ProgramsModel(d_hponic->programManager(), this));
    d_widgetConfigProgram = new QWidget(this);

    d_splitter = new QSplitter(Qt::Horizontal, this);
}

void WidgetPrograms::createMenu()
{
    QMenu *menuSetProgram = new QMenu;
    menuSetProgram->addAction(ui->actionSetUnknownProgram);
    menuSetProgram->addSeparator();
    menuSetProgram->addAction(ui->actionSetTimerControlProgram);
    menuSetProgram->addAction(ui->actionSetRelayControlProgram);
    menuSetProgram->addAction(ui->actionSetPidControlProgram);
    menuSetProgram->addAction(ui->actionSetButtonControlProgram);

    ui->tbSetProgram->setMenu(menuSetProgram);
}

void WidgetPrograms::createLayouts()
{
    QHBoxLayout *layoutControls = new QHBoxLayout;
    layoutControls->addWidget(ui->tbSetProgram);
    layoutControls->addSpacing(50);
    layoutControls->addWidget(ui->tbDownload);
    layoutControls->addWidget(ui->tbUpload);
    layoutControls->addStretch(1);

    QVBoxLayout *layoutPrograms = new QVBoxLayout;
    layoutPrograms->addLayout(layoutControls);
    layoutPrograms->addWidget(d_widgetConfigProgram, 1);

    ui->scrollArea->setLayout(layoutPrograms);

    d_splitter->addWidget(ui->tvPrograms);
    d_splitter->addWidget(ui->scrollArea);

    QHBoxLayout *layoutMain = new QHBoxLayout;
    layoutMain->addWidget(d_splitter, 1);

    setLayout(layoutMain);
}

void WidgetPrograms::createConnections()
{
    connect(ui->actionSetUnknownProgram, SIGNAL(triggered()), this, SLOT(setUnknownProgramType()), Qt::DirectConnection);

    connect(ui->actionSetTimerControlProgram, SIGNAL(triggered()), this, SLOT(setTimerControlType()), Qt::DirectConnection);
    connect(ui->actionSetRelayControlProgram, SIGNAL(triggered()), this, SLOT(setRelayControlType()), Qt::DirectConnection);
    connect(ui->actionSetPidControlProgram, SIGNAL(triggered()), this, SLOT(setPidControlType()), Qt::DirectConnection);
    connect(ui->actionSetButtonControlProgram, SIGNAL(triggered()), this, SLOT(setButtonControlType()), Qt::DirectConnection);

    connect(ui->tbSetProgram->menu(), SIGNAL(aboutToShow()), this, SLOT(onSetProgramTypeClicked()), Qt::DirectConnection);

    connect(ui->actionResetPrograms, SIGNAL(triggered()), d_hponic.data(), SLOT(resetPrograms()), Qt::DirectConnection);

    connect(ui->tbDownload, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(downloadPrograms()), Qt::DirectConnection);
    connect(ui->tbUpload, SIGNAL(clicked(bool)), d_hponic.data(), SLOT(uploadPrograms()), Qt::DirectConnection);

    connect(ui->tvPrograms->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onProgramCurrentChanged(QModelIndex,QModelIndex)), Qt::DirectConnection);

    connect(ui->tvPrograms, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(programsDownloadStarted()), this, SLOT(onProgramsDownloadStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(programsDownloadFinished(bool)), this, SLOT(onProgramsDownloadFinished(bool)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(programsUploadStarted()), this, SLOT(onProgramsUploadStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(programsUploadFinished(bool)), this, SLOT(onProgramsUploadFinished(bool)), Qt::DirectConnection);
}

void WidgetPrograms::swapWidgetConfigProgram(QWidget *widget)
{
    if (widget == 0)
        return;

    if (d_widgetConfigProgram) {
        ui->scrollArea->layout()->replaceWidget(d_widgetConfigProgram, widget);
        d_widgetConfigProgram->deleteLater();
    }

    d_widgetConfigProgram = widget;
    d_widgetConfigProgram->show();
}
