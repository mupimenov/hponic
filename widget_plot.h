#ifndef WIDGET_PLOT_H
#define WIDGET_PLOT_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTimeEdit>
#include <QScrollBar>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QToolButton>
#include <QMenu>

#include <QSharedPointer>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>

#include <qwt_interval.h>

#include "hponic.h"

namespace Ui {
class WidgetPlot;
}

class WidgetPlot : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPlot(QSharedPointer<Hponic> hponic, QWidget *parent = 0);
    ~WidgetPlot();

private Q_SLOTS:    
    void resetTo();
    void select();
    void exportToExcel();
    void exportToCSV();

    void onAutoScaleChanged(bool on);
    void onMaxyChanged(double maxY);
    void onMinxChanged(double minY);
    void onOffsetChanged(int offset);
    void onIntervalChanged(const QTime &interval);
    void onModeChanged(int index);

    void onIoslotAdded(int num);
    void onIoslotUpdated(int num);
    void onIoslotRemoved(int num);

    void onRecordUpdated(const IoslotValueRecord &record);

    void onExportStarted();
    void onExportStopped();

    void legendChecked(const QVariant &itemInfo, bool on, int index);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    void createCurves();

    void updateCurveData(const QList<IoslotValueRecord> &records);
    void updateCurveData(const IoslotValueRecord &record);
    void updateCurve(QwtPlotCurve *curve);

    void updatePlot();

    void showCurve(QwtPlotItem *item, bool on);

    void enableExportControls(bool enable);

    Ui::WidgetPlot *ui;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lMode;
    QComboBox *d_cbMode;
    QLabel *d_lFrom;
    QDateTimeEdit *d_dteFrom;
    QLabel *d_lTo;
    QDateTimeEdit *d_dteTo;
    QPushButton *d_pbResetTo;
    QToolButton *d_tbSelect;
    QLabel *d_lRecordCount;
    QToolButton *d_tbExportTo;
    QAction *d_aExportToExcel;
    QAction *d_aExportToCSV;
    QwtPlot *d_plot;
    QCheckBox *d_cbAutoScale;
    QDoubleSpinBox *d_dsbMaxy;
    QDoubleSpinBox *d_dsbMiny;
    QScrollBar *d_sbOffset;
    QTimeEdit *d_teInterval;

    QwtInterval d_interval;
    double d_width;
    double d_firstTime;
    double d_lastTime;

    QList<QwtPlotCurve*> d_curves;
    QwtPlotDirectPainter *d_directPainter;
    bool d_online;

    int d_recordCount;
    IoslotValueRecord d_firstRecord;
    IoslotValueRecord d_lastRecord;
};

#endif // WIDGET_PLOT_H
