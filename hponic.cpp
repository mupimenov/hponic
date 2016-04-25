#include "hponic.h"

#include <iostream>

#include <QFile>
#include <QVariant>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "ioslots_xml_composer.h"
#include "programs_xml_composer.h"

#include "ioslots_bin_composer.h"
#include "programs_bin_composer.h"

#define SLOTS_COUNT 60
#define PROGRAMS_COUNT 30

Hponic::Hponic(QObject *parent) : QObject(parent),
    d_address(1)
{
    d_ioslotManager = QSharedPointer<IoslotManager>(new IoslotManager);
    d_programManager = QSharedPointer<ProgramManager>(new ProgramManager);

    d_serial = QSharedPointer<SerialInterface>(new SerialInterface(d_portSettings));
    d_transmission = QSharedPointer<Transmission>(new Transmission(d_serial));

    d_monitoring = QSharedPointer<Monitoring>(new Monitoring(d_ioslotManager, d_transmission, d_address));

    d_dataBaseTable = QSharedPointer<IoslotValueTable>(new IoslotValueTable);
    d_dataBaseProducer = QSharedPointer<IoslotValueProducer>(new IoslotValueProducer(d_monitoring));
    d_databaseInserter = QSharedPointer<IoslotValueInserter>(new IoslotValueInserter(d_dataBaseTable));

    //{
    connect(d_transmission.data(), SIGNAL(statusChanged(Transmission::Status)),
            this, SIGNAL(transmissionStatusChanged(Transmission::Status)),
            Qt::BlockingQueuedConnection);
    connect(d_databaseInserter.data(), SIGNAL(statusChanged(IoslotValueInserter::Status)),
            this, SIGNAL(databaseInserterStatusChanged(IoslotValueInserter::Status)),
            Qt::BlockingQueuedConnection);

    connect(d_dataBaseProducer.data(), SIGNAL(recordUpdated(IoslotValueRecord)),
            d_databaseInserter.data(), SLOT(insertRecord(IoslotValueRecord)), Qt::DirectConnection);
    //}

    createIoslots();
    createPrograms();
    createCommands();
}

Hponic::~Hponic()
{
    disconnect(d_databaseInserter.data(), SIGNAL(statusChanged(IoslotValueInserter::Status)),
               this, SIGNAL(databaseInserterStatusChanged(IoslotValueInserter::Status)));
    disconnect(d_transmission.data(), SIGNAL(statusChanged(Transmission::Status)),
            this, SIGNAL(transmissionStatusChanged(Transmission::Status)));

    stopDatabaseInserter();
    stopTransmission();    

    d_databaseInserter->wait();
    d_transmission->wait();
}

QSharedPointer<IoslotManager> Hponic::ioslotManager() const
{
    return d_ioslotManager;
}

QSharedPointer<ProgramManager> Hponic::programManager() const
{
    return d_programManager;
}

QSharedPointer<Monitoring> Hponic::monitoring() const
{
    return d_monitoring;
}

QSharedPointer<IoslotValueTable> Hponic::databaseTable() const
{
    return d_dataBaseTable;
}

QSharedPointer<IoslotValueProducer> Hponic::databaseProducer() const
{
    return d_dataBaseProducer;
}

QSharedPointer<IoslotValueInserter> Hponic::databaseInserter() const
{
    return d_databaseInserter;
}

const QString &Hponic::configFilename() const
{
    return d_configFilename;
}

const PortSettings &Hponic::portSettings() const
{
    return d_serial->portSettings();
}

void Hponic::setPortSettings(const PortSettings &ps)
{
    d_serial->setPortSettings(ps);
}

Interface *Hponic::interface()
{
    return d_serial.data();
}

Transmission::Status Hponic::transmissionStatus()
{
    return d_transmission->status();
}

void Hponic::setAddress(quint8 address)
{
    if (d_address == address)
        return;

    d_address = address;

    d_readAdcValuesCommand->setAddress(d_address);
    d_readIoslotValuesCommand->setAddress(d_address);

    d_readCommonValuesCommand->setAddress(d_address);    
}

quint8 Hponic::address() const
{
    return d_address;
}

bool Hponic::saveConfig(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error: Cannot write to file " << qPrintable(filename)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    QDomImplementation impl = QDomDocument().implementation();

    QTextStream stream(&file);
    QDomDocument doc(impl.createDocumentType("Hponic", "", ""));
    QDomElement root = doc.createElement("hponic");
    doc.appendChild(root);

    IoslotsXmlComposerV1 ioslotsComposer;
    QDomElement elem = ioslotsComposer.toElement(d_ioslotManager->ioslots(), doc);
    root.appendChild(elem);

    ProgramsXmlComposerV1 programsComposer;
    elem = programsComposer.toElement(d_programManager->programs(), doc);
    root.appendChild(elem);

    doc.save(stream, 2);

    d_configFilename = filename;
    return true;
}

bool Hponic::loadConfig(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(filename)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine,
                        &errorColumn)) {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
        return false;
    }

    QDomElement root = doc.documentElement();
    IoslotsXmlComposerV1 ioslotComposer;
    QList<QSharedPointer<Ioslot> > ioslots = ioslotComposer.fromElement(root);
    d_ioslotManager->replaceIoslots(ioslots);

    ProgramsXmlComposerV1 programComposer;
    QList<QSharedPointer<Program> > programs = programComposer.fromElement(root);
    d_programManager->replacePrograms(programs);

    d_configFilename = filename;

    return true;
}

void Hponic::exportToExcel(const QString &filename, const QDateTime &from, const QDateTime &to)
{
    exportTo(IoslotValueExporter::Excel, filename, from, to);
}

void Hponic::exportToCSV(const QString &filename, const QDateTime &from, const QDateTime &to)
{
    exportTo(IoslotValueExporter::CSV, filename, from, to);
}

void Hponic::startTransmission()
{
    if (d_transmission->status() == Transmission::Started)
        return;

    d_transmission->start();
}

void Hponic::stopTransmission()
{
    if (d_transmission->status() == Transmission::Stopped)
        return;

    d_transmission->stop();
}

void Hponic::startDatabaseInserter()
{
    if (!d_dataBaseTable->isValid())
        return;

    if (d_databaseInserter->status() == IoslotValueInserter::Started)
        return;

    d_databaseInserter->start();
}

void Hponic::stopDatabaseInserter()
{
    if (d_databaseInserter->status() == IoslotValueInserter::Stopped)
        return;

    d_databaseInserter->stop();
}

void Hponic::downloadIoslots()
{
    if (d_transmission->status() == Transmission::Stopped)
        return;

    DownloadIoslotsCommand *downloadIoslotsCommand = new DownloadIoslotsCommand(d_transmission->interface(), d_address);
    connect(downloadIoslotsCommand, SIGNAL(finished(DownloadFileCommand*)),
            this, SLOT(downloadIoslotsCommandFinished(DownloadFileCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(QSharedPointer<Command>(downloadIoslotsCommand));

    Q_EMIT ioslotsDownloadStarted();
}

void Hponic::uploadIoslots()
{
    if (d_transmission->status() == Transmission::Stopped)
        return;

    IoslotsBinComposerV1 composer;
    QByteArray data = composer.toArray(d_ioslotManager->ioslots());

    UploadIoslotsCommand *uploadIoslotsCommand = new UploadIoslotsCommand(d_transmission->interface(), d_address, data);
    connect(uploadIoslotsCommand, SIGNAL(finished(UploadFileCommand*)),
            this, SLOT(uploadIoslotsCommandFinished(UploadFileCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(QSharedPointer<Command>(uploadIoslotsCommand));

    Q_EMIT ioslotsUploadStarted();
}

void Hponic::downloadPrograms()
{
    if (d_transmission->status() == Transmission::Stopped)
        return;

    DownloadProgramsCommand *downloadProgramsCommand = new DownloadProgramsCommand(d_transmission->interface(), d_address);
    connect(downloadProgramsCommand, SIGNAL(finished(DownloadFileCommand*)),
            this, SLOT(downloadProgramsCommandFinished(DownloadFileCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(QSharedPointer<Command>(downloadProgramsCommand));

    Q_EMIT programsDownloadStarted();
}

void Hponic::uploadPrograms()
{
    if (d_transmission->status() == Transmission::Stopped)
        return;

    ProgramsBinComposerV1 composer;
    QByteArray data = composer.toArray(d_programManager->programs());

    UploadProgramsCommand *uploadProgramsCommand = new UploadProgramsCommand(d_transmission->interface(), d_address, data);
    connect(uploadProgramsCommand, SIGNAL(finished(UploadFileCommand*)),
            this, SLOT(uploadProgramsCommandFinished(UploadFileCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(QSharedPointer<Command>(uploadProgramsCommand));

    Q_EMIT programsUploadStarted();
}

void Hponic::setTime()
{
    if (d_transmission->status() == Transmission::Stopped)
        return;

    SetTimeCommand *setTimeCommand = new SetTimeCommand(d_transmission->interface(), d_address);
    connect(setTimeCommand, SIGNAL(finished(SetTimeCommand*)),
            this, SLOT(setTimeCommandFinished(SetTimeCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(QSharedPointer<Command>(setTimeCommand));

    Q_EMIT timeSetStarted();
}

void Hponic::resetIoslots()
{
    for (int i = 0; i < SLOTS_COUNT; ++i)
        d_ioslotManager->replaceIoslot(i, QSharedPointer<Ioslot>(new EmptySlot(i + 1)));
}

void Hponic::resetPrograms()
{
    for (int i = 0; i < SLOTS_COUNT; ++i)
        d_programManager->replaceProgram(i, QSharedPointer<Program>(new EmptyProgram(i + 1)));
}

void Hponic::downloadIoslotsCommandFinished(DownloadFileCommand *cmd)
{
    if (cmd->result() == Command::Ok) {
        const QByteArray &data = cmd->data();
        IoslotsBinComposerV1 composer;
        QList<QSharedPointer<Ioslot> > ioslots = composer.fromArray(data);

        d_ioslotManager->replaceIoslots(ioslots, true);
    }

    Q_EMIT ioslotsDownloadFinished(cmd->result() == Command::Ok);
}

void Hponic::uploadIoslotsCommandFinished(UploadFileCommand *cmd)
{
    Q_EMIT ioslotsUploadFinished(cmd->result() == Command::Ok);
}

void Hponic::downloadProgramsCommandFinished(DownloadFileCommand *cmd)
{
    if (cmd->result() == Command::Ok) {
        const QByteArray &data = cmd->data();
        ProgramsBinComposerV1 composer;
        QList<QSharedPointer<Program> > programs = composer.fromArray(data);

        d_programManager->replacePrograms(programs, true);
    }

    Q_EMIT programsDownloadFinished(cmd->result() == Command::Ok);
}

void Hponic::uploadProgramsCommandFinished(UploadFileCommand *cmd)
{
    Q_EMIT programsUploadFinished(cmd->result() == Command::Ok);
}

void Hponic::setTimeCommandFinished(SetTimeCommand *cmd)
{
    Q_EMIT timeSetFinished(cmd->result() == Command::Ok);
}

void Hponic::createIoslots()
{
    for (int i = 0; i < SLOTS_COUNT; ++i)
        d_ioslotManager->addIoslot(QSharedPointer<Ioslot>(new EmptySlot(i + 1)));
}

void Hponic::createPrograms()
{
    for (int i = 0; i < PROGRAMS_COUNT; ++i)
        d_programManager->addProgram(QSharedPointer<Program>(new EmptyProgram(i + 1)));
}

void Hponic::createCommands()
{
    // ADC values
    d_readAdcValuesCommand = QSharedPointer<ReadAdcValuesCommand>(
                new ReadAdcValuesCommand(d_transmission->interface(), d_address));

    connect(d_readAdcValuesCommand.data(), SIGNAL(finished(ReadAdcValuesCommand*)),
            d_monitoring.data(), SLOT(updateAdcValues(ReadAdcValuesCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(d_readAdcValuesCommand.dynamicCast<Command>());

    // Ioslot values
    d_readIoslotValuesCommand = QSharedPointer<ReadIoslotValuesCommand>(
            new ReadIoslotValuesCommand(d_transmission->interface(), d_address));

    connect(d_readIoslotValuesCommand.data(), SIGNAL(finished(ReadIoslotValuesCommand*)),
            d_monitoring.data(), SLOT(updateValues(ReadIoslotValuesCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(d_readIoslotValuesCommand.dynamicCast<Command>());

    // Clock, uptime
    d_readCommonValuesCommand = QSharedPointer<ReadCommonValuesCommand>(
            new ReadCommonValuesCommand(d_transmission->interface(), d_address));

    connect(d_readCommonValuesCommand.data(), SIGNAL(finished(ReadCommonValuesCommand*)),
            d_monitoring.data(), SLOT(updateCommonValues(ReadCommonValuesCommand*)),
            Qt::BlockingQueuedConnection);

    d_transmission->addCommand(d_readCommonValuesCommand.dynamicCast<Command>());
}

void Hponic::exportTo(IoslotValueExporter::FileType filetype, const QString &filename, const QDateTime &from, const QDateTime &to)
{
    if (d_databaseExporter) {
        if (d_databaseExporter->isRunning())
            return;
    }

    d_databaseExporter = QSharedPointer<IoslotValueExporter>(
                new IoslotValueExporter(d_ioslotManager,
                                        d_dataBaseTable,
                                        d_address,
                                        filetype,
                                        filename,
                                        from, to));

    connect(d_databaseExporter.data(), SIGNAL(started()), this, SIGNAL(exportStarted()));
    connect(d_databaseExporter.data(), SIGNAL(finished()), this, SIGNAL(exportStopped()));
    connect(d_databaseExporter.data(), SIGNAL(progress(int,int)), this, SIGNAL(exportProgress(int,int)));

    d_databaseExporter->start();
}
