#include "database_service.h"

#include <QFile>
#include <QDebug>

#include <xlsxdocument.h>

IoslotValueRecord::IoslotValueRecord() :
    d_id(0),
    d_timestamp(),
    d_values()
{

}

IoslotValueRecord::IoslotValueRecord(const QDateTime &timestamp, const QList<IoslotValueRecord::RecordValue> &values, int id) :
    d_id(id),
    d_timestamp(timestamp),
    d_values(values)
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

double IoslotValueRecord::value(int slot_id) const
{
    QList<RecordValue>::const_iterator it = d_values.begin();
    for (; it != d_values.end(); ++it) {
        if (it->first == slot_id)
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

bool IoslotValueTable::init(const QString &path)
{
    if (path.isEmpty())
        return false;

    QSqlDatabase sdb = QSqlDatabase::database("local");
    if (!sdb.isValid())
         sdb = QSqlDatabase::addDatabase("QSQLITE", "local");
    sdb.setDatabaseName(path);

    if (!sdb.open()) {
        return false;
    }

    d_id = 0;

    bool need_create = false;
    QSqlQuery query(sdb);

    if (!query.exec("SELECT id,timestamp FROM records WHERE id=1;")) {
        need_create = true;
    }

    if (!need_create) {
        if (!query.exec("SELECT record_id,ioslot_id,ioslot_value FROM rvalues WHERE record_id=1;")) {
            need_create = true;
        }
    }

    if (need_create) {
        if (!sdb.transaction()) {
            return false;
        }

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

        query.clear();

        sdb.commit();
    }

    if (!query.exec("SELECT COUNT(*) AS 'recordCount' FROM records;")) {
        return false;
    }

    query.next();
    d_id = query.value(0).toInt() + 1;

    return true;
}

int IoslotValueTable::recordCount(const QDateTime &from, const QDateTime &to)
{
    int count = 0;

    QSqlDatabase sdb = QSqlDatabase::database("local");
    if (!sdb.isValid()) return count;

    QSqlQuery rquery(sdb);
    rquery.prepare("SELECT COUNT(*) AS 'recordCount' FROM records WHERE timestamp >= datetime(:from) AND timestamp <= datetime(:to);");
    rquery.bindValue(":from", from.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    rquery.bindValue(":to", to.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    if (rquery.exec()) {
        if (rquery.next()) {
            count = rquery.value(0).toInt();
        }
    }

    return count;
}

static QList<IoslotValueRecord::RecordValue> recordValues(QSqlDatabase &sdb, int id)
{
    QList<IoslotValueRecord::RecordValue> values;

    QSqlQuery query(sdb);
    query.prepare("SELECT record_id,ioslot_id,ioslot_value FROM rvalues WHERE record_id=:id;");
    query.bindValue(":id", id);
    if (!query.exec())
        return values;

    while (query.next()) {
        int ioslot_id = query.value(1).toInt();
        double value = query.value(2).toDouble();
        values.append(IoslotValueRecord::RecordValue(ioslot_id, value));
    }

    return values;
}

QList<IoslotValueRecord> IoslotValueTable::records(const QDateTime &from, const QDateTime &to, int limit, int offset)
{
    QList<IoslotValueRecord> list;

    QSqlDatabase sdb = QSqlDatabase::database("local");
    if (!sdb.isValid()) return list;

    QSqlQuery query(sdb);
    query.prepare("SELECT id,timestamp FROM records WHERE timestamp >= datetime(:from) AND timestamp <= datetime(:to) LIMIT :limit OFFSET :offset;");
    query.bindValue(":from", from.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(":to", to.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(":limit", QString::number(limit));
    query.bindValue(":offset", QString::number(offset));

    if (query.exec()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QDateTime timestamp = QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd hh:mm:ss.zzz");
            QList<IoslotValueRecord::RecordValue> values = recordValues(sdb, id);
            if (values.empty())
                continue;

            list.push_back(IoslotValueRecord(timestamp, values, id));
        }
    }

    return list;
}

QList<IoslotValueRecord> IoslotValueTable::newerThan(const QDateTime &dt, int limit, int mod)
{
    QList<IoslotValueRecord> list;

    QSqlDatabase sdb = QSqlDatabase::database("local");
    if (!sdb.isValid()) return list;

    QSqlQuery query(sdb);
    if (mod <= 1) {
        query.prepare("SELECT id,timestamp FROM records WHERE timestamp >= datetime(:dt) LIMIT :limit;");
    } else {
        query.prepare("SELECT id,timestamp FROM records WHERE timestamp >= datetime(:dt) AND id % :mod = 0 LIMIT :limit;");
        query.bindValue(":mod", mod);
    }
    query.bindValue(":dt", dt.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(":limit", QString::number(limit));

    if (query.exec()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QDateTime timestamp = QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd hh:mm:ss.zzz");
            QList<IoslotValueRecord::RecordValue> values = recordValues(sdb, id);
            if (values.empty())
                continue;

            list.push_back(IoslotValueRecord(timestamp, values, id));
        }
    }

    return list;
}

QList<IoslotValueRecord> IoslotValueTable::olderThan(const QDateTime &dt, int limit, int mod)
{
    QList<IoslotValueRecord> list;

    QSqlDatabase sdb = QSqlDatabase::database("local");
    if (!sdb.isValid()) return list;

    QSqlQuery query(sdb);
    if (mod <= 1) {
        query.prepare("SELECT id,timestamp FROM records WHERE timestamp <= datetime(:dt) ORDER BY id DESC LIMIT :limit;");
    } else {
        query.prepare("SELECT id,timestamp FROM records WHERE timestamp <= datetime(:dt) AND id % :mod = 0 ORDER BY id DESC LIMIT :limit;");
        query.bindValue(":mod", mod);
    }
    query.bindValue(":dt", dt.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(":limit", QString::number(limit));

    if (query.exec()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QDateTime timestamp = QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd hh:mm:ss.zzz");
            QList<IoslotValueRecord::RecordValue> values = recordValues(sdb, id);
            if (values.empty())
                continue;

            list.push_front(IoslotValueRecord(timestamp, values, id));
        }
    }

    return list;
}

QList<IoslotValueRecord> IoslotValueTable::median(const QDateTime &from, const QDateTime &to, int limit)
{
    int count = recordCount(from, to);

    qint64 w1 = from.toMSecsSinceEpoch() - to.toMSecsSinceEpoch();
    qint64 w2 = double(limit) / double(count) * w1;
    qint16 dw = (w1 - w2) / 2;

    QDateTime dt1 = from;
    QDateTime dt2 = to;
    if (dw > 0) {
        dt1 = QDateTime::fromMSecsSinceEpoch(dt1.toMSecsSinceEpoch() + dw);
        dt2 = QDateTime::fromMSecsSinceEpoch(dt2.toMSecsSinceEpoch() - dw);
    }

    return records(dt1, dt2, limit);
}

void IoslotValueTable::insert(const IoslotValueRecord &record)
{
    QSqlDatabase sdb = QSqlDatabase::database("local");
    if (!sdb.isValid()) return;

    if (sdb.transaction()) {
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

        Q_EMIT recordInserted(d_id);

        ++d_id;

        query.clear();
        sdb.commit();
    }
}

bool IoslotValueTable::isValid() const
{
    return (d_id > 0);
}

int IoslotValueTable::id() const
{
    return d_id;
}

IoslotValueProducer::IoslotValueProducer(QSharedPointer<Monitoring> monitoring, QObject *parent) : QObject(parent),
    d_monitoring(monitoring),
    d_period(5),
    d_lastTime(QTime::currentTime())
{
    connect(d_monitoring.data(), SIGNAL(valuesUpdated()), this, SLOT(onValuesUpdated()), Qt::DirectConnection);
}

void IoslotValueProducer::setPeriod(int period)
{
    d_period = period;
}

int IoslotValueProducer::period() const
{
    return d_period;
}

void IoslotValueProducer::onValuesUpdated()
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QList<IoslotValueRecord::RecordValue> rvalues;

    const QList<IoslotValue> &values = d_monitoring->values();
    QList<IoslotValue>::const_iterator it = values.begin();
    for (; it != values.end(); ++it) {
        IoslotValueRecord::RecordValue v(it->ioslot->id(), it->value.toDouble());
        rvalues.append(v);
    }    

    QTime currentTime = QTime::currentTime();
    if (currentTime > d_lastTime.addSecs(d_period)
            || d_monitoring->discreteOutputDiffers()) {

        d_lastTime = currentTime;
        d_record = IoslotValueRecord(timestamp, rvalues);

        Q_EMIT recordUpdated(d_record);
    }
}

IoslotValueInserter::IoslotValueInserter(QSharedPointer<IoslotValueTable> table,
                                         QObject *parent) : QThread(parent),
    d_table(table),
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

void IoslotValueInserter::insertRecord(const IoslotValueRecord &record)
{
    QMutexLocker locker(&d_mutex);
    if (d_status == Stopped)
        return;

    if (d_records.size() < RECORDS_BUF_LEN) {
        d_records.append(record);
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

        d_table->insert(record);
    }

    {
        QMutexLocker locker(&d_mutex);
        d_status = Stopped;
    }

    Q_EMIT statusChanged(d_status);
}

IoslotValueExporter::IoslotValueExporter(QSharedPointer<IoslotManager> ioslotManager,
                                         QSharedPointer<IoslotValueTable> table,
                                         quint8 address,
                                         FileType filetype,
                                         const QString &filename,
                                         const QDateTime &from, const QDateTime &to,
                                         QObject *parent) :
    QThread(parent),
    d_ioslotManager(ioslotManager),
    d_table(table),
    d_address(address),
    d_filetype(filetype),
    d_filename(filename),
    d_from(from),
    d_to(to),
    d_stop(false)
{

}

void IoslotValueExporter::stop()
{
    QMutexLocker locker(&d_mutex);
    d_stop = true;
}

#define XLSX_PROGRAM_NAME_ROW 1
#define XLSX_KEY_COL 1
#define XLSX_VALUE_COL 2
#define XLSX_VERSION_ROW 2
#define XLSX_DEVICE_ADDRESS_ROW 3
#define XLSX_HEADER_ROW 5
#define XLSX_DATA_START_ROW 6

void IoslotValueExporter::run()
{
    switch (d_filetype) {
    case Excel:
        toExcel();
        break;
    case CSV:
        toCSV();
        break;
    }
}

void IoslotValueExporter::toExcel()
{
    QXlsx::Document xlsx;

    // Title
    QXlsx::Format format;
    format.setFontSize(18);
    format.setFontBold(true);
    format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    xlsx.write(XLSX_PROGRAM_NAME_ROW, XLSX_KEY_COL, tr("Hydroponic system configurator"), format);

    format.setFontSize(10);
    format.setFontBold(false);
    format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    xlsx.write(XLSX_VERSION_ROW, XLSX_KEY_COL, tr("Version"), format);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    xlsx.write(XLSX_VERSION_ROW, XLSX_VALUE_COL, tr("1.0"), format);

    format.setFontSize(12);
    format.setFontBold(true);
    format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    xlsx.write(XLSX_DEVICE_ADDRESS_ROW, XLSX_KEY_COL, tr("Address"), format);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    xlsx.write(XLSX_DEVICE_ADDRESS_ROW, XLSX_VALUE_COL, d_address, format);

    QVector<bool> valueEmpty(d_ioslotManager->ioslotCount(), false);

    // Table header
    format.setFontSize(12);
    format.setFontBold(true);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setTopBorderStyle(QXlsx::Format::BorderThick);
    format.setBottomBorderStyle(QXlsx::Format::BorderThick);
    format.setLeftBorderStyle(QXlsx::Format::BorderThin);
    format.setRightBorderStyle(QXlsx::Format::BorderThin);

    int col = 1;
    xlsx.write(XLSX_HEADER_ROW, col, tr("Timestamp"), format);
    ++col;

    for (int num = 0; num < d_ioslotManager->ioslotCount(); ++num) {
        if (d_ioslotManager->ioslot(num)->type() == UnknownIoslotType) {
            valueEmpty[num] = true;
            continue;
        }

        const QString &name = d_ioslotManager->ioslot(num)->name();
        xlsx.write(XLSX_HEADER_ROW, col, name, format);
        ++col;
    }

    // Table content
    format.setFontSize(12);
    format.setFontBold(false);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setTopBorderStyle(QXlsx::Format::BorderThin);
    format.setBottomBorderStyle(QXlsx::Format::BorderThin);
    format.setLeftBorderStyle(QXlsx::Format::BorderThin);
    format.setRightBorderStyle(QXlsx::Format::BorderThin);

    int count = d_table->recordCount(d_from, d_to);
    const int limit = 100;

    int row = 0;
    for (int offset = 0; offset < count; offset += limit) {
        QList<IoslotValueRecord> records = d_table->records(d_from, d_to, limit, offset);
        QList<IoslotValueRecord>::iterator it = records.begin();

        for (; it != records.end(); ++it, ++row) {
            IoslotValueRecord &record = *it;

            int col = 1;
            xlsx.write(XLSX_DATA_START_ROW + row, col,
                       record.timestamp().toString("yyyy-MM-dd hh:mm:ss.zzz"),
                       format);
            ++col;

            const QList<IoslotValueRecord::RecordValue> &values = record.values();
            QList<IoslotValueRecord::RecordValue>::const_iterator it2 = values.begin();

            for (int num = 0; it2 != values.end(); ++it2, ++num) {
                if (valueEmpty[num]) continue;

                const IoslotValueRecord::RecordValue &value = *it2;
                xlsx.write(XLSX_DATA_START_ROW + row, col, value.second, format);
                ++col;
            }
        }

        Q_EMIT progress(offset, count);
    }

    (void)xlsx.saveAs(d_filename);
}

void IoslotValueExporter::toCSV()
{
    QFile data(d_filename);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        QString str;

        // Table header
        str += tr("Timestamp") + ";";

        QVector<bool> valueEmpty(d_ioslotManager->ioslotCount(), false);
        for (int num = 0; num < d_ioslotManager->ioslotCount(); ++num) {
            if (d_ioslotManager->ioslot(num)->type() == UnknownIoslotType) {
                valueEmpty[num] = true;
                continue;
            }

            const QString &name = d_ioslotManager->ioslot(num)->name();
            str += name + ";";
        }

        out << str << endl;

        // Table content
        int count = d_table->recordCount(d_from, d_to);
        const int limit = 100;

        int row = 0;
        for (int offset = 0; offset < count; offset += limit) {
            QList<IoslotValueRecord> records = d_table->records(d_from, d_to, limit, offset);
            QList<IoslotValueRecord>::iterator it = records.begin();

            for (; it != records.end(); ++it, ++row) {
                IoslotValueRecord &record = *it;

                str = record.timestamp().toString("yyyy-MM-dd hh:mm:ss.zzz") + ";";

                const QList<IoslotValueRecord::RecordValue> &values = record.values();
                QList<IoslotValueRecord::RecordValue>::const_iterator it2 = values.begin();

                for (int num = 0; it2 != values.end(); ++it2, ++num) {
                    if (valueEmpty[num]) continue;

                    const IoslotValueRecord::RecordValue &value = *it2;
                    str += QString::number(value.second) + ";";
                }

                out << str << endl;
            }

            Q_EMIT progress(offset, count);
        }
    }
}
