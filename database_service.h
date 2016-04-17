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
    IoslotValueRecord(const QDateTime &__timestamp, const QList<RecordValue> &__values, int __id = 0);

    int id() const;
    const QDateTime &timestamp() const;
    double value(int __slot_id) const;

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

    bool init(const QString &__path);
    QList<IoslotValueRecord> selectRecords(const QDateTime &__from, const QDateTime &__to, int __limit);
    void insertRecord(const IoslotValueRecord &record);
    bool isValid() const;

private:
    int d_id;
    QReadWriteLock d_lock;
};

class IoslotValueProducer : public QObject
{
    Q_OBJECT
public:
    explicit IoslotValueProducer(QSharedPointer<Monitoring> __monitoring, QObject *parent = 0);

    int period() const;

public Q_SLOTS:
    void setPeriod(int __period);

Q_SIGNALS:
    void recordUpdated(const IoslotValueRecord &__record);

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
    explicit IoslotValueInserter(QSharedPointer<IoslotValueTable> __table, QObject *parent = 0);

    enum Status {
        Stopped,
        Started
    };

    Status status();

Q_SIGNALS:
    void statusChanged(IoslotValueInserter::Status status);

public Q_SLOTS:
    void insertRecord(const IoslotValueRecord &__record);

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
    QWaitCondition d_conditionWait;
};

class IoslotValueExporter : public QThread
{
    Q_OBJECT
public:
    explicit IoslotValueExporter(QSharedPointer<IoslotValueTable> __table,
                                 const QString &__fileName,
                                 const QDateTime &__from, const QDateTime &__to,
                                 QObject *parent = 0);

Q_SIGNALS:
    void progress(int perc);

public Q_SLOTS:
    void stop();

protected:
    virtual void run();

private:
    QSharedPointer<IoslotValueTable> d_table;
    QString d_fileName;
    QDateTime d_from;
    QDateTime d_to;
    QMutex d_mutex;
    bool d_stop;
};

#endif // DATABASESERVICE_H
