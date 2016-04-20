#ifndef HPONIC_H
#define HPONIC_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QSharedPointer>
#include <QDateTime>

#include "ioslot.h"
#include "program.h"

#include "interface.h"
#include "transmission.h"

#include "hponic_commands.h"

#include "ioslot_manager.h"
#include "program_manager.h"

#include "monitoring.h"
#include "database_service.h"

class Hponic : public QObject
{
    Q_OBJECT
public:
    explicit Hponic(QObject *parent = 0);
    virtual ~Hponic();

    QSharedPointer<IoslotManager> ioslotManager() const;
    QSharedPointer<ProgramManager> programManager() const;
    QSharedPointer<Monitoring> monitoring() const;
    QSharedPointer<IoslotValueTable> databaseTable() const;
    QSharedPointer<IoslotValueProducer> databaseProducer() const;
    QSharedPointer<IoslotValueInserter> databaseInserter() const;

    const QString &configFilename() const;

    /*
     * Transmission
     * */

    const PortSettings &portSettings(void) const;
    void setPortSettings(const PortSettings &ps);

    Interface *interface();

    Transmission::Status transmissionStatus();

    void setAddress(quint8 address);
    quint8 address() const;

Q_SIGNALS:
    void transmissionStatusChanged(Transmission::Status status);
    void databaseInserterStatusChanged(IoslotValueInserter::Status status);

    void exportStarted();
    void exportStopped();
    void exportProgress(int offset, int count);

    void timeSetStarted();
    void timeSetFinished(bool success);

    void ioslotsDownloadStarted();
    void ioslotsDownloadFinished(bool success);

    void ioslotsUploadStarted();
    void ioslotsUploadFinished(bool success);

    void programsDownloadStarted();
    void programsDownloadFinished(bool success);

    void programsUploadStarted();
    void programsUploadFinished(bool success);

public Q_SLOTS:
    bool saveConfig(const QString &filename);
    bool loadConfig(const QString &filename);

    void exportToExcel(const QString &filename, const QDateTime &from, const QDateTime &to);
    void exportToCSV(const QString &filename, const QDateTime &from, const QDateTime &to);

    void startTransmission();
    void stopTransmission();

    void startDatabaseInserter();
    void stopDatabaseInserter();

    void downloadIoslots();
    void uploadIoslots();

    void downloadPrograms();
    void uploadPrograms();

    void setTime();

private Q_SLOTS:
    void downloadIoslotsCommandFinished(DownloadFileCommand *cmd);
    void uploadIoslotsCommandFinished(UploadFileCommand *cmd);

    void downloadProgramsCommandFinished(DownloadFileCommand *cmd);
    void uploadProgramsCommandFinished(UploadFileCommand *cmd);

    void setTimeCommandFinished(SetTimeCommand *cmd);

private:
    void createIoslots();
    void createPrograms();
    void createCommands();

    void exportTo(IoslotValueExporter::FileType filetype, const QString &filename, const QDateTime &from, const QDateTime &to);

    QSharedPointer<IoslotManager>       d_ioslotManager;
    QSharedPointer<ProgramManager>      d_programManager;
    QSharedPointer<Monitoring>          d_monitoring;
    QSharedPointer<IoslotValueTable>    d_dataBaseTable;
    QSharedPointer<IoslotValueProducer> d_dataBaseProducer;
    QSharedPointer<IoslotValueInserter> d_databaseInserter;
    QSharedPointer<IoslotValueExporter> d_databaseExporter;

    QSharedPointer<ReadIoslotValuesCommand> d_readIoslotValuesCommand;
    QSharedPointer<ReadCommonValuesCommand> d_readCommonValuesCommand;
    QSharedPointer<ReadAdcValuesCommand>    d_readAdcValuesCommand;

    QString                             d_configFilename;

    QSharedPointer<Transmission>        d_transmission;
    QSharedPointer<SerialInterface>     d_serial;
    PortSettings                        d_portSettings;

    quint8                              d_address;
};

#endif // HPONIC_H
