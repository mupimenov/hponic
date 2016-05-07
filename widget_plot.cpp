#include "widget_plot.h"
#include "ui_widget_plot.h"

#include <QDateTime>
#include <QGridLayout>
#include <QPointF>

#include <QFileDialog>

#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>
#include <qwt_legend_label.h>

#include <qwt_painter.h>
#include <qwt_scale_map.h>
#include <qwt_scale_draw.h>

enum PlotMode {
    PlotDisabled,
    PlotOnline,
    PlotFromDatabase
};

class Canvas: public QwtPlotCanvas
{
public:
    Canvas( QwtPlot *plot = NULL ):
        QwtPlotCanvas( plot )
    {
        setPaintAttribute(QwtPlotCanvas::BackingStore, false);
        setBorderRadius(10);

        if (QwtPainter::isX11GraphicsSystem()) {
#if QT_VERSION < 0x050000
            setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif
            if (testPaintAttribute(QwtPlotCanvas::BackingStore)) {
                setAttribute(Qt::WA_PaintOnScreen, true);
                setAttribute(Qt::WA_NoSystemBackground, true);
            }
        }

        setupPalette();
    }

private:
    void setupPalette()
    {
        QPalette pal = palette();

#if QT_VERSION >= 0x040400
        QLinearGradient gradient;
        gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
        gradient.setColorAt(0.0, QColor(0, 49, 110));
        gradient.setColorAt(1.0, QColor(0, 87, 174));

        pal.setBrush(QPalette::Window, QBrush(gradient));
#else
        pal.setBrush(QPalette::Window, QBrush(color));
#endif

        pal.setColor(QPalette::WindowText, Qt::green);

        setPalette(pal);
    }
};

class CurveData: public QwtSeriesData<QPointF>
{    
public:
    static const int points = 1000;

    CurveData() : QwtSeriesData<QPointF>(),
        d_boundingRect(1.0, 1.0, -2.0, -2.0)  {
        d_values.reserve(points);
    }

    virtual QPointF sample(size_t i) const {
        return d_values[i];
    }

    virtual size_t size() const {
        return d_values.size();
    }

    virtual QRectF boundingRect() const {
        return d_boundingRect;
    }

    void append(const QPointF &sample) {
        if (d_values.size() == points)
            reset();

        d_values.append(sample);

        // adjust the bounding rectangle

        if (d_boundingRect.width() < 0 || d_boundingRect.height() < 0) {
            d_boundingRect.setRect(sample.x(), sample.y(), 0.0, 0.0);
        } else {
            d_boundingRect.setRight(sample.x());

            if (sample.y() > d_boundingRect.bottom())
                d_boundingRect.setBottom(sample.y());

            if (sample.y() < d_boundingRect.top())
                d_boundingRect.setTop(sample.y());
        }
    }

    void reset() {
        d_values.clear();
        d_boundingRect.setRect(1.0, 1.0, -2.0, -2.0);
    }

private:
    QRectF d_boundingRect;
    QVector<QPointF> d_values;
};

class TimeScaleDraw: public QwtScaleDraw
{
public:
    virtual QwtText label(double v) const
    {
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(v));
        return dt.toString("dd.MM.yyyy\nhh:mm:ss.zzz");
    }
};

WidgetPlot::WidgetPlot(QSharedPointer<Hponic> hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPlot),
    d_hponic(hponic),
    d_interval(0.0, 10 * 1000.0),
    d_recordCount(0)
{
    ui->setupUi(this);

    createWidgets();
    createLayouts();
    createConnections();

    createCurves();
}

WidgetPlot::~WidgetPlot()
{
    delete d_directPainter;
    delete ui;
}

void WidgetPlot::resetTo()
{
    d_dteTo->setDateTime(QDateTime::currentDateTime());
}

void WidgetPlot::select()
{
    d_recordCount = d_hponic->databaseTable()->recordCount(d_dteFrom->dateTime(), d_dteTo->dateTime());
    if (d_recordCount) {
        QList<IoslotValueRecord> list = d_hponic->databaseTable()->records(d_dteFrom->dateTime(), d_dteTo->dateTime(), 1);
        if (list.isEmpty())
            return;

        d_firstTime = list.at(0).timestamp().toMSecsSinceEpoch();

        list = d_hponic->databaseTable()->records(d_dteFrom->dateTime(), d_dteTo->dateTime(), 1, d_recordCount - 1);
        if (list.isEmpty())
            return;

        d_lastTime = list.at(0).timestamp().toMSecsSinceEpoch();

        d_sbOffset->blockSignals(true);
        d_sbOffset->setRange(0, d_lastTime - d_firstTime);
        d_sbOffset->setValue(0);
        d_sbOffset->blockSignals(false);

        d_dataBeginTime = 0;
        d_dataEndTime = 0;

        updateCurvesRight();
        updateInterval(d_dataBeginTime);

        updatePlot();
    }

    d_lRecordCount->setText(tr("Record count: %1").arg(d_recordCount));
}

void WidgetPlot::exportToExcel()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Exporting to Excel Workbook"),
                                                    "/",
                                                    tr("Excel (*.xlsx)"));
    if (filename.isEmpty())
        return;

    d_hponic->exportToExcel(filename, d_dteFrom->dateTime(), d_dteTo->dateTime());
}

void WidgetPlot::exportToCSV()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Exporting to CSV file"),
                                                    "/",
                                                    tr("CSV (*.txt)"));
    if (filename.isEmpty())
        return;

    d_hponic->exportToCSV(filename, d_dteFrom->dateTime(), d_dteTo->dateTime());
}

void WidgetPlot::onAutoScaleChanged(bool on)
{
    d_dsbMaxy->setEnabled(!on);
    d_dsbMiny->setEnabled(!on);

    d_plot->setAxisAutoScale(QwtPlot::yLeft, on);
    if (!on)
        d_plot->setAxisScale(QwtPlot::yLeft, d_dsbMiny->value(), d_dsbMaxy->value());

    d_plot->replot();
}

void WidgetPlot::onMaxyChanged(double maxY)
{
    d_plot->setAxisScale(QwtPlot::yLeft, d_dsbMiny->value(), maxY);
    d_plot->replot();
}

void WidgetPlot::onMinxChanged(double minY)
{
    d_plot->setAxisScale(QwtPlot::yLeft, minY, d_dsbMaxy->value());
    d_plot->replot();
}

void WidgetPlot::onOffsetChanged(int offset)
{
    int mode = d_cbMode->currentData().toInt();

    updateInterval(d_firstTime + offset);

    if (mode == PlotFromDatabase) {
        if (d_width <= (d_lastTime - d_firstTime)) {
            if ((d_firstTime + offset - d_width / 2.0) < d_dataBeginTime)
                updateCurvesLeft();
            else if ((d_firstTime + offset + d_width / 2.0) > d_dataEndTime)
                updateCurvesRight();
        }
    }

    updatePlot();
}

void WidgetPlot::onIntervalChanged(const QTime &interval)
{
    double width = (-1) * interval.msecsTo(QTime(0, 0, 0, 0));
    if (width > 0) {
        d_width = width;

        updateMaxTime();
        updateInterval(d_lastTime);
        updatePlot();
    }
}

void WidgetPlot::onModeChanged(int index)
{
    int mode = d_cbMode->itemData(index).toInt();
    bool enable = false;

    d_width = (-1) * d_teInterval->time().msecsTo(QTime(0, 0, 0, 0));

    if (mode == PlotDisabled || mode == PlotOnline) {
        resetCurveData();

        d_firstTime = d_lastTime = QDateTime::currentMSecsSinceEpoch();
        updateMaxTime();

        d_sbOffset->blockSignals(true);
        d_sbOffset->setRange(0, 0);
        d_sbOffset->setValue(0);
        d_sbOffset->blockSignals(false);

        d_lRecordCount->setText(QString());

        updateInterval(d_lastTime);

        updatePlot();
    }

    if (mode == PlotFromDatabase)
        enable = true;

    d_dteFrom->setEnabled(enable);
    d_dteTo->setEnabled(enable);
    d_pbResetTo->setEnabled(enable);
    d_tbSelect->setEnabled(enable);
    d_tbExportTo->setEnabled(enable);
    d_lRecordCount->setEnabled(enable);
}

static const char *colors[] =
{
    "LightSalmon",
    "SteelBlue",
    "Yellow",
    "Fuchsia",
    "PaleGreen",
    "PaleTurquoise",
    "Cornsilk",
    "HotPink",
    "Peru",
    "Maroon"
};

static const int numColors = sizeof(colors) / sizeof(colors[0]);

static QwtPlotCurve *createCurve(const QString &name, int num) {
    QwtPlotCurve *curve = new QwtPlotCurve(name);
    curve->setPen(QColor(colors[(num + 1) % numColors]), 2);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setData(new CurveData);
    curve->setVisible(true);
    return curve;
}

void WidgetPlot::onIoslotAdded(int num)
{
    if (d_hponic->ioslotManager()->ioslot(num)->type() != UnknownIoslotType) {
        QwtPlotCurve *curve = createCurve(d_hponic->ioslotManager()->ioslot(num)->name(), num);
        curve->attach(d_plot);

        d_curves.append(curve);
        showCurve(curve, true);
    } else {
        d_curves.append(NULL);
    }
}

void WidgetPlot::onIoslotUpdated(int num)
{
    if (num < 0 || num >= d_curves.size())
        return;

    if (d_curves[num] != NULL)
        delete d_curves[num];

    if (d_hponic->ioslotManager()->ioslot(num)->type() != UnknownIoslotType) {
        QwtPlotCurve *curve = createCurve(d_hponic->ioslotManager()->ioslot(num)->name(), num);
        curve->attach(d_plot);

        d_curves[num] = curve;
        showCurve(curve, true);
    } else {
        d_curves[num] = NULL;
    }
}

void WidgetPlot::onIoslotRemoved(int num)
{
    if (num < 0 || num >= d_curves.size())
        return;

    if (d_curves[num] != NULL)
        delete d_curves[num];

    d_curves.removeAt(num);
}

void WidgetPlot::onRecordUpdated(const IoslotValueRecord &record)
{
    int mode = d_cbMode->currentData().toInt();
    if (mode != PlotOnline)
        return;

    updateCurveData(record);

    d_lastTime = static_cast<double>(record.timestamp().toMSecsSinceEpoch());
    if (d_lastTime > d_maxTime) {
        updateMaxTime();
        updateOffset();
    }
}

void WidgetPlot::onExportStarted()
{
    enableExportControls(false);
}

void WidgetPlot::onExportStopped()
{
    enableExportControls(true);
}

void WidgetPlot::legendChecked(const QVariant &itemInfo, bool on, int index)
{
    Q_UNUSED(index);

    QwtPlotItem *plotItem = d_plot->infoToItem(itemInfo);
    if (plotItem)
        showCurve(plotItem, on);
}

void WidgetPlot::createWidgets()
{
    d_lMode = new QLabel(tr("Mode:"), this);
    d_cbMode = new QComboBox(this);
    d_cbMode->addItem(tr("Disabled"), QVariant(PlotDisabled));
    d_cbMode->addItem(tr("Online"), QVariant(PlotOnline));
    d_cbMode->addItem(tr("Offline"), QVariant(PlotFromDatabase));

    d_lFrom = new QLabel(tr("From:"), this);
    d_dteFrom = new QDateTimeEdit(this);
    d_dteFrom->setEnabled(false);

    d_lTo = new QLabel(tr("To:"), this);
    d_dteTo = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    d_dteTo->setEnabled(false);

    d_pbResetTo = new QPushButton(tr("C"), this);
    d_pbResetTo->setEnabled(false);

    d_tbSelect = new QToolButton(this);
    d_tbSelect->setIcon(QIcon("://icons/database_go.png"));
    d_tbSelect->setText(tr("Select"));
    d_tbSelect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    d_tbSelect->setEnabled(false);

    d_lRecordCount = new QLabel(this);

    d_tbExportTo = new QToolButton(this);
    d_tbExportTo->setPopupMode(QToolButton::InstantPopup);
    d_tbExportTo->setIcon(QIcon("://icons/document_export.png"));
    d_tbExportTo->setText(tr("Export to"));
    d_tbExportTo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    d_tbExportTo->setEnabled(false);

    d_aExportToExcel = new QAction(QIcon("://icons/export_excel.png"), tr("Export to Excel..."), this);
    d_aExportToCSV = new QAction(tr("Export to CSV..."), this);

    QMenu *menuExportTo = new QMenu(d_tbExportTo);
    menuExportTo->addAction(d_aExportToExcel);
    menuExportTo->addAction(d_aExportToCSV);

    d_tbExportTo->setMenu(menuExportTo);

    d_plot = new QwtPlot(this);
    d_plot->setAutoReplot(false);
    d_plot->setCanvas(new Canvas());

    d_plot->plotLayout()->setAlignCanvasToScales(true);

    d_plot->setAxisTitle(QwtPlot::xBottom, tr("Timestamp"));
    d_plot->setAxisScale(QwtPlot::xBottom, d_interval.minValue(), d_interval.maxValue());
    d_plot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw);
    d_plot->setAxisScale(QwtPlot::yLeft, 0.0, 1000.0);

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setPen(Qt::gray, 0.0, Qt::DotLine);
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(d_plot);

    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode(QwtLegendData::Checkable);
    d_plot->insertLegend(legend, QwtPlot::LeftLegend);
    legend->setVisible(true);

    connect(legend, SIGNAL(checked(QVariant,bool,int)),
            SLOT(legendChecked(QVariant,bool,int)), Qt::DirectConnection);    

    d_cbAutoScale = new QCheckBox(tr("Auto scale"), this);
    d_cbAutoScale->setChecked(false);

    d_dsbMaxy = new QDoubleSpinBox(this);
    d_dsbMaxy->setRange(-10000.0, 10000.0);
    d_dsbMaxy->setValue(1024.0);

    d_dsbMiny = new QDoubleSpinBox(this);
    d_dsbMiny->setRange(-10000.0, 10000.0);
    d_dsbMiny->setValue(0.0);

    d_sbOffset = new QScrollBar(Qt::Horizontal, this);
    d_sbOffset->setRange(0, 0);
    d_sbOffset->setValue(0);

    d_teInterval = new QTimeEdit(this);
    d_teInterval->setDisplayFormat("hh:mm:ss");
    d_teInterval->setTime(QTime(d_interval.width() / (1000.0 * 3600.0),
                                d_interval.width() / (1000.0 * 60.0),
                                d_interval.width() / (1000.0 * 1.0)));

    d_directPainter = new QwtPlotDirectPainter();
}

void WidgetPlot::createLayouts()
{
    QHBoxLayout *layoutMode = new QHBoxLayout;
    layoutMode->addWidget(d_lMode);
    layoutMode->addWidget(d_cbMode);
    layoutMode->addWidget(d_lFrom);
    layoutMode->addWidget(d_dteFrom);
    layoutMode->addWidget(d_lTo);
    layoutMode->addWidget(d_dteTo);
    layoutMode->addWidget(d_pbResetTo);
    layoutMode->addWidget(d_tbSelect);
    layoutMode->addStretch(1);
    layoutMode->addWidget(d_lRecordCount);
    layoutMode->addWidget(d_tbExportTo);

    QHBoxLayout *layoutPlot = new QHBoxLayout;
    layoutPlot->addWidget(d_plot, 1);

    QHBoxLayout *layoutOffset = new QHBoxLayout;
    layoutOffset->addWidget(d_sbOffset, 1);

    QGridLayout *layoutMain = new QGridLayout;
    int row = 0;
    layoutMain->addLayout(layoutMode,   row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutMain->addWidget(d_cbAutoScale,row, 0, 1, 1, Qt::AlignLeft);
    ++row;
    layoutMain->addWidget(d_dsbMaxy,    row, 0, 1, 1, Qt::AlignLeft);
    layoutMain->addLayout(layoutPlot,   row, 1, 3, 3, Qt::AlignCenter);
    row += 3;
    layoutMain->addWidget(d_dsbMiny,    row, 0, 1, 1, Qt::AlignLeft);
    layoutMain->addLayout(layoutOffset, row, 1, 1, 3, Qt::AlignCenter);
    ++row;
    layoutMain->addWidget(d_teInterval, row, 2, 1, 1, Qt::AlignCenter);

    layoutMain->setRowStretch(3, 1);
    layoutMain->setColumnStretch(1, 1);

    setLayout(layoutMain);
}

void WidgetPlot::createConnections()
{
    connect(d_pbResetTo, SIGNAL(clicked()), this, SLOT(resetTo()), Qt::DirectConnection);
    connect(d_tbSelect, SIGNAL(clicked()), this, SLOT(select()), Qt::DirectConnection);
    connect(d_aExportToExcel, SIGNAL(triggered()), this, SLOT(exportToExcel()), Qt::DirectConnection);
    connect(d_aExportToCSV, SIGNAL(triggered()), this, SLOT(exportToCSV()), Qt::DirectConnection);

    connect(d_cbAutoScale, SIGNAL(clicked(bool)), this, SLOT(onAutoScaleChanged(bool)), Qt::DirectConnection);
    connect(d_dsbMaxy, SIGNAL(valueChanged(double)), this, SLOT(onMaxyChanged(double)), Qt::DirectConnection);
    connect(d_dsbMiny, SIGNAL(valueChanged(double)), this, SLOT(onMinxChanged(double)), Qt::DirectConnection);
    connect(d_sbOffset, SIGNAL(valueChanged(int)), this, SLOT(onOffsetChanged(int)), Qt::DirectConnection);
    connect(d_teInterval, SIGNAL(timeChanged(QTime)), this, SLOT(onIntervalChanged(QTime)), Qt::DirectConnection);
    connect(d_cbMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onModeChanged(int)), Qt::DirectConnection);

    connect(d_hponic->ioslotManager().data(), SIGNAL(ioslotAdded(int)), this, SLOT(onIoslotAdded(int)), Qt::DirectConnection);
    connect(d_hponic->ioslotManager().data(), SIGNAL(ioslotReplaced(int)), this, SLOT(onIoslotUpdated(int)), Qt::DirectConnection);
    connect(d_hponic->ioslotManager().data(), SIGNAL(ioslotUpdated(int)), this, SLOT(onIoslotUpdated(int)), Qt::DirectConnection);
    connect(d_hponic->ioslotManager().data(), SIGNAL(ioslotRemoved(int)), this, SLOT(onIoslotRemoved(int)), Qt::DirectConnection);
    connect(d_hponic->databaseProducer().data(), SIGNAL(recordUpdated(IoslotValueRecord)), this, SLOT(onRecordUpdated(IoslotValueRecord)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(exportStarted()), this, SLOT(onExportStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(exportStopped()), this, SLOT(onExportStopped()), Qt::DirectConnection);
}

void WidgetPlot::createCurves()
{
    for (int num = 0; num < d_hponic->ioslotManager()->ioslotCount(); ++num) {
        if (d_hponic->ioslotManager()->ioslot(num)->type() != UnknownIoslotType) {
            QwtPlotCurve *curve = createCurve(d_hponic->ioslotManager()->ioslot(num)->name(), num);
            curve->attach(d_plot);

            d_curves.append(curve);
            showCurve(curve, true);
        } else {
            d_curves.append(NULL);
        }
    }
}

void WidgetPlot::resetCurveData()
{
    QList<QwtPlotCurve*>::iterator it = d_curves.begin();
    for (; it != d_curves.end(); ++it) {
        QwtPlotCurve *curve = *it;
        if (!curve) continue;

        CurveData *data = static_cast<CurveData *>(curve->data());
        data->reset();
    }
}

void WidgetPlot::updateCurveData(const IoslotValueRecord &record)
{
    double x = static_cast<double>(record.timestamp().toMSecsSinceEpoch());

    QList<QwtPlotCurve*>::iterator it = d_curves.begin();
    QList<IoslotValueRecord::RecordValue>::const_iterator it2 = record.values().begin();

    for (; it != d_curves.end(); ++it, ++it2) {
        QwtPlotCurve *curve = *it;
        if (!curve) continue;

        CurveData *data = static_cast<CurveData *>(curve->data());
        data->append(QPointF(x, it2->second));

        updateCurve(curve);
    }
}

void WidgetPlot::updateCurve(QwtPlotCurve *curve)
{
    if (!curve->isVisible())
        return;

    CurveData *data = static_cast<CurveData *>(curve->data());

    const int numPoints = data->size();
    if (numPoints) {
        const bool doClip = !d_plot->canvas()->testAttribute(Qt::WA_PaintOnScreen);
        if (doClip) {
            const QwtScaleMap xMap = d_plot->canvasMap(curve->xAxis());
            const QwtScaleMap yMap = d_plot->canvasMap(curve->yAxis());

            QRectF br = qwtBoundingRect(*data,
                numPoints - 2, numPoints - 1);

            const QRect clipRect = QwtScaleMap::transform(xMap, yMap, br).toRect();
            d_directPainter->setClipRegion(clipRect);
        }

        d_directPainter->drawSeries(curve,
            numPoints - 2, numPoints - 1);
    }
}

void WidgetPlot::updatePlot()
{
    d_plot->setAxisScale(QwtPlot::xBottom,
        d_interval.minValue(), d_interval.maxValue());

    d_plot->replot();
}

void WidgetPlot::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);

    QwtLegend *lgd = qobject_cast<QwtLegend *>(d_plot->legend());
    QList<QWidget *> legendWidgets = lgd->legendWidgets(d_plot->itemToInfo(item));

    if (legendWidgets.size() == 1) {
        QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>(legendWidgets[0]);
        if (legendLabel)
            legendLabel->setChecked(on);
    }

    d_plot->replot();
}

void WidgetPlot::enableExportControls(bool enable)
{
    d_cbMode->setEnabled(enable);
    d_tbExportTo->setEnabled(enable);
}

void WidgetPlot::updateMaxTime()
{
    d_maxTime = d_lastTime + d_width / 2.0;
}

void WidgetPlot::updateInterval(double center)
{
    d_interval.setInterval(center - d_width / 2.0, center + d_width / 2.0);
}

void WidgetPlot::updateOffset()
{
    bool autoExpand = d_sbOffset->value() == d_sbOffset->maximum();

    d_sbOffset->blockSignals(true);

    d_sbOffset->setMaximum(d_lastTime - d_firstTime);
    if (autoExpand) {
        d_sbOffset->setValue(d_sbOffset->maximum());

        updateInterval(d_lastTime);
        updatePlot();
    }

    d_sbOffset->blockSignals(false);
}

void WidgetPlot::updateCurvesLeft()
{
    if (d_dataBeginTime == d_firstTime)
        return;

    QDateTime begin = QDateTime::fromMSecsSinceEpoch(d_dataBeginTime - d_width);
    QDateTime end = QDateTime::fromMSecsSinceEpoch(d_dataBeginTime + d_width);

    int count = d_hponic->databaseTable()->recordCount(begin, end);
    QList<IoslotValueRecord> records = d_hponic->databaseTable()->olderThan(end, CurveData::points, count / CurveData::points + 1);
    updateCurvesCommon(records);
}

void WidgetPlot::updateCurvesRight()
{
    if (d_dataEndTime == d_lastTime)
        return;

    QDateTime begin = QDateTime::fromMSecsSinceEpoch(d_dataEndTime - d_width);
    QDateTime end = QDateTime::fromMSecsSinceEpoch(d_dataEndTime + d_width);

    int count = d_hponic->databaseTable()->recordCount(begin, end);
    QList<IoslotValueRecord> records = d_hponic->databaseTable()->newerThan(begin, CurveData::points, count / CurveData::points + 1);
    updateCurvesCommon(records);
}

void WidgetPlot::updateCurvesCommon(QList<IoslotValueRecord> &records)
{
    d_dataBeginTime = records.first().timestamp().toMSecsSinceEpoch();
    d_dataEndTime = records.last().timestamp().toMSecsSinceEpoch();

    resetCurveData();

    QList<IoslotValueRecord>::const_iterator it = records.begin();
    for (; it != records.end(); ++it) {
        double x = static_cast<double>(it->timestamp().toMSecsSinceEpoch());

        QList<QwtPlotCurve*>::iterator it2 = d_curves.begin();
        QList<IoslotValueRecord::RecordValue>::const_iterator it3 = it->values().begin();
        for (; it2 != d_curves.end(); ++it2, ++it3) {
            QwtPlotCurve *curve = *it2;
            if (!curve) continue;

            CurveData *data = static_cast<CurveData *>(curve->data());
            data->append(QPointF(x, it3->second));
        }
    }
}
