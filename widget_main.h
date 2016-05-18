#ifndef WIDGET_MAIN_H
#define WIDGET_MAIN_H

#include <QMainWindow>

#include <QSharedPointer>

#include "widget_ioslots.h"
#include "widget_programs.h"
#include "widget_config_transmission.h"
#include "widget_monitoring.h"
#include "widget_database.h"
#include "widget_plot.h"

#include "hponic.h"

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = 0);
    ~WidgetMain();

protected:
    virtual bool event(QEvent *event);

private Q_SLOTS:
    void saveConfig();
    void saveConfigAs();
    void openConfig();

    void showAbout();

    void onTransmissionStatusChanged(Transmission::Status status);
    void onTransmissionCommandSend(Command::Result result);

    void onExportStarted();
    void onExportStopped();
    void onExportProgress(int offset, int count);

private:
    void createGlobals();
    void createWidgets();
    void createLayouts();
    void createConnections();

    void saveConfigImpl(const QString &currentFilename);

    enum HighlightState
    {
        Connected,
        Disconnected,
        Error,
        Normal
    };

    void highlight(HighlightState state);

    Ui::WidgetMain *ui;

    WidgetIoslots *d_widgetIoslots;
    WidgetPrograms *d_widgetPrograms;
    WidgetConfigTransmission *d_widgetConfigTransmission;
    WidgetMonitoring *d_widgetMonitoring;
    WidgetDatabase *d_widgetDatabase;
    WidgetPlot *d_widgetPlot;

    QSharedPointer<Hponic> d_hponic;
};

#endif // WIDGET_MAIN_H
