#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QReadWriteLock>

#include <QSharedPointer>

#include <QtSql>

#include "monitoring.h"

class IoslotValueRecord
{
public:
    typedef QPair<int, double> RecordValue;

    IoslotValueRecord();
    IoslotValueRecord(const QDateTime &timestamp, const QList<RecordValue> &values, int id = 0);

    int id() const;
    const QDateTime &timestamp() const;
    double value(int slot_id) const;

    const QList<RecordValue> &values() const;

private:
    int d_id;
    QDateTime d_timestamp;
    QList<RecordValue> d_values;
};

class IoslotValueTable : public QObject
{
    Q_OBJECT
public:
    explicit IoslotValueTable(QObject *parent = 0);

    bool init(const QString &path);
    int recordCount(const QDateTime &from, const QDateTime &to);
    QList<IoslotValueRecord> records(const QDateTime &from, const QDateTime &to, int limit, int offset = 0);
    QList<IoslotValueRecord> granulated(const QDateTime &from, const QDateTime &to, int granules);
    void insert(const IoslotValueRecord &record);
    bool isValid() const;

private:
    int d_id;
};

class IoslotValueProducer : public QObject
{
    Q_OBJECT
public:
    explicit IoslotValueProducer(QSharedPointer<Monitoring> monitoring, QObject *parent = 0);

    int period() const;

public Q_SLOTS:
    void setPeriod(int period);

Q_SIGNALS:
    void recordUpdated(const IoslotValueRecord &record);

private Q_SLOTS:
    void onValuesUpdated();

private:
    QSharedPointer<Monitoring> d_monitoring;

    IoslotValueRecord d_record;
    int d_period;
    QTime d_lastTime;
};

class IoslotValueInserter : public QThread
{
    Q_OBJECT
public:
    explicit IoslotValueInserter(QSharedPointer<IoslotValueTable> table, QObject *parent = 0);

    enum Status {
        Stopped,
        Started
    };

    Status status();

Q_SIGNALS:
    void statusChanged(IoslotValueInserter::Status status);

public Q_SLOTS:
    void insertRecord(const IoslotValueRecord &record);

    void stop();

protected:
    virtual void run();

private:
    QSharedPointer<IoslotValueTable> d_table;
    QMutex d_mutex;
    Status d_status;
    bool d_stop;
    QList<IoslotValueRecord> d_records;
    QWaitCondition d_conditionInsert;
};

class IoslotValueExporter : public QThread
{
    Q_OBJECT
public:
    explicit IoslotValueExporter(QSharedPointer<IoslotManager> ioslotManager,
                                 QSharedPointer<IoslotValueTable> table,
                                 quint8 address,
                                 const QString &fileName,
                                 const QDateTime &from, const QDateTime &to,
                                 QObject *parent = 0);

Q_SIGNALS:
    void progress(int perc);

public Q_SLOTS:
    void stop();

protected:
    virtual void run();

private:
    QSharedPointer<IoslotManager> d_ioslotManager;
    QSharedPointer<IoslotValueTable> d_table;
    quint8 d_address;
    QString d_filename;
    QDateTime d_from;
    QDateTime d_to;
    QMutex d_mutex;
    bool d_stop;
};

#endif // DATABASESERVICE_H
