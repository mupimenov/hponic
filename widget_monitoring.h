#ifndef WIDGET_MONITORING_H
#define WIDGET_MONITORING_H

#include <QWidget>
#include <QSplitter>

#include <QSharedPointer>

#include "hponic.h"

namespace Ui {
class WidgetMonitoring;
}

class WidgetMonitoring : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMonitoring(QSharedPointer<Hponic> hponic, QWidget *parent = 0);
    ~WidgetMonitoring();

private Q_SLOTS:
    void onCommonValuesUpdated();

    void onTimeSetStarted();
    void onTimeSetFinished(bool success);

    void onProgramsRestartStarted();
    void onProgramsRestartFinished(bool success);

private:
    void createWidgets();
    void createMenu();
    void createLayouts();
    void createConnections();

    Ui::WidgetMonitoring *ui;
    QSplitter *d_InputSplitter;
    QSplitter *d_OutputSplitter;

    QSharedPointer<Hponic> d_hponic;

};

#endif // WIDGET_MONITORING_H
