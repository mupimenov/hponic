#ifndef WIDGET_DATABASE_H
#define WIDGET_DATABASE_H

#include <QWidget>

#include <QSharedPointer>

#include "hponic.h"

namespace Ui {
class WidgetDatabase;
}

class WidgetDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDatabase(QSharedPointer<Hponic> hponic, QWidget *parent = 0);
    ~WidgetDatabase();

private Q_SLOTS:
    void newDatabaseFile();
    void openDatabaseFile();

    void onDatabaseInserterStatusChanged(IoslotValueInserter::Status status);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    bool initDatabaseFile(const QString &filename);

    Ui::WidgetDatabase *ui;
    QSharedPointer<Hponic> d_hponic;
};

#endif // WIDGET_DATABASE_H
