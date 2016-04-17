#ifndef WIDGET_PLOT_H
#define WIDGET_PLOT_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTimeEdit>
#include <QScrollBar>
#include <QComboBox>
#include <QLabel>

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
    explicit WidgetPlot(QSharedPointer<Hponic> __hponic, QWidget *parent = 0);
    ~WidgetPlot();

private Q_SLOTS:
    void changedMaxY(double __maxY);
    void changedMinY(double __minY);
    void changedOffset(int __offset);
    void changedInterval(const QTime &interval);
    void changedMode(int __index);

    void onIoslotAdded(int __num);
    void onRecordUpdated(const IoslotValueRecord &__record);

    void setInterval(double interval);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    void updateCurve(QwtPlotCurve *__curve);

    Ui::WidgetPlot *ui;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lMode;
    QComboBox *d_cbMode;
    QLabel *d_lFrom;
    QDateTimeEdit *d_dteFrom;
    QLabel *d_lTo;
    QDateTimeEdit *d_dteTo;
    QwtPlot *d_plot;
    QDoubleSpinBox *d_dsbMaxY;
    QDoubleSpinBox *d_dsbMinY;
    QScrollBar *d_sbOffset;
    QTimeEdit *d_teInterval;

    QwtInterval d_interval;

    QList<QwtPlotCurve*> d_curves;
    QwtPlotDirectPainter *d_directPainter;
    bool d_online;
};

#endif // WIDGET_PLOT_H
