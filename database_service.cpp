#include "database_service.h"

#include <QDebug>

IoslotValueRecord::IoslotValueRecord() :
    d_id(0),
    d_timestamp(),
    d_values()
{

}

IoslotValueRecord::IoslotValueRecord(const QDateTime &__timestamp, const QList<IoslotValueRecord::RecordValue> &__values, int __id) :
    d_id(__id),
    d_timestamp(__timestamp),
    d_values(__values)
{

}

int IoslotValueRecord::id() const
{
    return d_id;
}

const QDateTime &IoslotValueRecord::timestamp() const
{
    return d_timestamp;
}

double IoslotValueRecord::value(int __slot_id) const
{
    QList<RecordValue>::const_iterator it = d_values.begin();
    for (; it != d_values.end(); ++it) {
        if (it->first == __slot_id)
            return it->second;
    }
    return NAN;
}

const QList<IoslotValueRecord::RecordValue> &IoslotValueRecord::values() const
{
    return d_values;
}

IoslotValueTable::IoslotValueTable(QObject *parent) : QObject(parent),
    d_id(0)
{

}

bool IoslotValueTable::init(const QString &__path)
{
    if (__path.isEmpty())
        return false;

    d_lock.lockForWrite();

    QSqlDatabase sdb = QSqlDatabase::database("local");
    if (!sdb.isValid())
         sdb = QSqlDatabase::addDatabase("QSQLITE", "local");
    sdb.setDatabaseName(__path);

    if (!sdb.open()) {
        d_lock.unlock();
        return false;
    }

    bool need_create = false;
    QSqlQuery query(sdb);

    if (!query.exec("SELECT id,timestamp FROM records WHERE id=1;")) {
        need_create = true;
    }

    if (!need_create) {
        if (!query.exec("SELECT id,ioslot_id,ioslot_value FROM rvalues WHERE id=1;")) {
            need_create = true;
        }
    }

    if (need_create) {
        QString str = "DROP TABLE records;";
        bool r = query.exec(str);

        str = "DROP TABLE rvalues;";
        r = query.exec(str);

        str = "CREATE TABLE records ("
              "id INT, "
              "timestamp CHAR(32), "
              "PRIMARY KEY (id)"
              ");";
        r = query.exec(str);

        str = "CREATE TABLE rvalues ("
              "record_id INT, "
              "ioslot_id INT, "
              "ioslot_value REAL, "
              "PRIMARY KEY (record_id, ioslot_id)"
              ");";
        r = query.exec(str);
        (void)r;
    }

    if (!query.exec("SELECT COUNT(*) FROM records;")) {
        d_lock.unlock();
        return false;
    }

    d_id = query.value(0).toInt() + 1;

    d_lock.unlock();
    return true;
}

QList<IoslotValueRecord> IoslotValueTable::selectRecords(const QDateTime &__from, const QDateTime &__to, int __limit)
{
    QList<IoslotValueRecord> list;

    d_lock.lockForRead();

    QSqlDatabase sdb = QSqlDatabase::database("local");
    QSqlQuery rquery(sdb);
    rquery.prepare("SELECT id,timestamp FROM records WHERE timestamp >= datetime(:from) AND timestamp <= datetime(:to) LIMIT :limit;");
    rquery.bindValue(":from", __from.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    rquery.bindValue(":to", __to.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    rquery.bindValue(":limit", QString::number(__limit));
    if (rquery.exec()) {
        while (rquery.next()) {
            int id = rquery.value(0).toInt();
            QDateTime timestamp = QDateTime::fromString(rquery.value(1).toString(), "yyyy-MM-dd hh:mm:ss.zzz");
            QSqlQuery vquery(sdb);
            vquery.prepare("SELECT record_id,ioslot_id,ioslot_value FROM rvalues WHERE record_id=:id;");
            vquery.bindValue(":id", id);
            if (!vquery.exec())
                continue;

            QList<IoslotValueRecord::RecordValue> values;
            while (vquery.next()) {
                int ioslot_id = vquery.value(1).toInt();
                double value = vquery.value(2).toDouble();
                values.append(IoslotValueRecord::RecordValue(ioslot_id, value));
            }

            list.append(IoslotValueRecord(timestamp, values, id));
        }
    }

    d_lock.unlock();

    return list;
}

void IoslotValueTable::insertRecord(const IoslotValueRecord &record)
{
    d_lock.lockForWrite();

    QSqlDatabase sdb = QSqlDatabase::database("local");
    QSqlQuery query(sdb);
    query.prepare("INSERT INTO records(id, timestamp) "
                  "VALUES (:id, :timestamp);");
    query.bindValue(":id", d_id);
    query.bindValue(":timestamp", record.timestamp().toString("yyyy-MM-dd hh:mm:ss.zzz"));

    query.exec();

    const QList<IoslotValueRecord::RecordValue> &values = record.values();
    QList<IoslotValueRecord::RecordValue>::const_iterator it = values.begin();
    for (; it != values.end(); ++it) {
        query.clear();

        query.prepare("INSERT INTO rvalues(record_id, ioslot_id, ioslot_value) "
                      "VALUES (:record_id, :ioslot_id, :ioslot_value);");
        query.bindValue(":record_id", d_id);
        query.bindValue(":ioslot_id", it->first);
        query.bindValue(":ioslot_value", it->second);

        query.exec();
    }

    ++d_id;

    d_lock.unlock();
}

bool IoslotValueTable::isValid() const
{
    return (d_id > 0);
}

IoslotValueProducer::IoslotValueProducer(QSharedPointer<Monitoring> __monitoring, QObject *parent) : QObject(parent),
    d_monitoring(__monitoring),
    d_period(5)
{
    connect(d_monitoring.data(), SIGNAL(valuesUpdated()), this, SLOT(onValuesUpdated()), Qt::DirectConnection);
}

void IoslotValueProducer::setPeriod(int __period)
{
    d_period = __period;
}

int IoslotValueProducer::period() const
{
    return d_period;
}

void IoslotValueProducer::onValuesUpdated()
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QList<IoslotValueRecord::RecordValue> rvalues;

    const QList<IoslotValue> values = d_monitoring->values();
    QList<IoslotValue>::const_iterator it = values.begin();
    for (; it != values.end(); ++it) {
        IoslotValueRecord::RecordValue v(it->first->id(), it->second.toDouble());
        rvalues.append(v);
    }    

    QTime currentTime = QTime::currentTime();
    if (d_lastTime.addSecs(d_period) >= currentTime
            || d_monitoring->discreteOutputDiffers()) {
        d_record = IoslotValueRecord(timestamp, rvalues);

        Q_EMIT recordUpdated(d_record);
    }
}

IoslotValueInserter::IoslotValueInserter(QSharedPointer<IoslotValueTable> __table,
                                         QObject *parent) : QThread(parent),
    d_table(__table),
    d_status(Stopped),
    d_stop(true)
{
    qRegisterMetaType<IoslotValueInserter::Status>("IoslotValueInserter::Status");
}

IoslotValueInserter::Status IoslotValueInserter::status()
{
    QMutexLocker locker(&d_mutex);
    return d_status;
}

#define RECORDS_BUF_LEN 10

void IoslotValueInserter::insertRecord(const IoslotValueRecord &__record)
{
    QMutexLocker locker(&d_mutex);
    if (d_status == Stopped)
        return;

    if (d_records.size() < RECORDS_BUF_LEN) {
        d_records.append(__record);
        d_conditionInsert.wakeOne();
    }
}

void IoslotValueInserter::stop()
{
    QMutexLocker locker(&d_mutex);
    d_stop = true;
    d_conditionInsert.wakeOne();
}

void IoslotValueInserter::run()
{
    {
        QMutexLocker locker(&d_mutex);
        d_status = Started;
        d_stop = false;
    }

    Q_EMIT statusChanged(d_status);

    while (1) {
        d_mutex.lock();
        d_conditionInsert.wait(&d_mutex);
        if (d_stop) {
            d_mutex.unlock();
            break;
        }

        IoslotValueRecord record = d_records.takeFirst();
        d_mutex.unlock();

        d_table->insertRecord(record);
    }

    {
        QMutexLocker locker(&d_mutex);
        d_status = Stopped;
        d_conditionWait.wakeOne();
    }

    Q_EMIT statusChanged(d_status);
}

IoslotValueExporter::IoslotValueExporter(QSharedPointer<IoslotValueTable> __table,
                                         const QString &__fileName,
                                         const QDateTime &__from, const QDateTime &__to,
                                         QObject *parent) :
    QThread(parent),
    d_table(__table),
    d_fileName(__fileName),
    d_from(__from),
    d_to(__to),
    d_stop(false)
{

}

void IoslotValueExporter::stop()
{
    QMutexLocker locker(&d_mutex);
    d_stop = true;
}

void IoslotValueExporter::run()
{

}
