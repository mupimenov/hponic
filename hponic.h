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
    void setPortSettings(const PortSettings &__ps);

    Interface *interface();

    Transmission::Status transmissionStatus();

    void setAddress(quint8 __address);
    quint8 address() const;

Q_SIGNALS:
    void transmissionStatusChanged(Transmission::Status status);
    void databaseInserterStatusChanged(IoslotValueInserter::Status status);

    void exportStarted();
    void exportStopped();
    void exportProgress(int perc);

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
    bool saveConfig(const QString &__filename);
    bool loadConfig(const QString &__filename);

    void exportToExcel(const QString &__fileName, const QDateTime &__from, const QDateTime &__to);

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
    void downloadIoslotsCommandFinished(DownloadFileCommand *__cmd);
    void uploadIoslotsCommandFinished(UploadFileCommand *__cmd);

    void downloadProgramsCommandFinished(DownloadFileCommand *__cmd);
    void uploadProgramsCommandFinished(UploadFileCommand *__cmd);

    void setTimeCommandFinished(SetTimeCommand *__cmd);

private:
    void createIoslots();
    void createPrograms();
    void createCommands();

    QSharedPointer<IoslotManager>       d_ioslotManager;
    QSharedPointer<ProgramManager>      d_programManager;
    QSharedPointer<Monitoring>          d_monitoring;
    QSharedPointer<IoslotValueTable>    d_dataBaseTable;
    QSharedPointer<IoslotValueProducer> d_dataBaseProducer;
    QSharedPointer<IoslotValueInserter> d_databaseInserter;
    QSharedPointer<IoslotValueExporter> d_databaseExporter;

    QSharedPointer<ReadIoslotValuesCommand> d_readIoslotValuesCommand;
    QSharedPointer<ReadCommonValuesCommand> d_readCommonValuesCommand;

    QString                             d_configFilename;

    QSharedPointer<Transmission>        d_transmission;
    QSharedPointer<SerialInterface>     d_serial;
    PortSettings                        d_portSettings;

    quint8                              d_address;
};

#endif // HPONIC_H
