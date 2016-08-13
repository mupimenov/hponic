#include "widget_plot.h"
#include "ui_widget_plot.h"

#include <QDateTime>
#include <QGridLayout>
#include <QPointF>

#include <QFileDialog>

#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_legenditem.h>
#include <qwt_plot_picker.h>

#include <qwt_legend.h>
#include <qwt_legend_label.h>

#include <qwt_painter.h>
#include <qwt_scale_map.h>
#include <qwt_scale_draw.h>

#include <qwt_symbol.h>

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
        gradient.setColorAt(0.0, QColor(255, 255, 255));
        gradient.setColorAt(1.0, QColor(255, 255, 255));

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
        if (d_values.size() == points) {
            QPointF last = d_values.takeLast();
            reset();
            d_values.append(last);
        }

        d_values.append(sample);

        // adjust the bounding rectangle

        if (d_boundingRect.width() < 0 || d_boundingRect.height() < 0) {
            if (!qIsNaN(sample.x())
                    && !qIsNaN(sample.y()))
                d_boundingRect.setRect(sample.x(), sample.y(), 0.0, 0.0);
        } else {
            if (!qIsNaN(sample.x()))
                d_boundingRect.setRight(sample.x());

            if (!qIsNaN(sample.y())
                    && sample.y() > d_boundingRect.bottom())
                d_boundingRect.setBottom(sample.y());

            if (!qIsNaN(sample.y())
                    && sample.y() < d_boundingRect.top())
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

class LegendItem: public QwtPlotLegendItem
{
public:
    LegendItem()
    {
        setRenderHint(QwtPlotItem::RenderAntialiased);

        QColor color(Qt::black);

        setTextPen(color);
#if 1
        setBorderPen(color);

        QColor c(Qt::gray);
        c.setAlpha(200);

        setBackgroundBrush(c);
#endif
    }

    virtual void updateLegend(const QwtPlotItem *plotItem,
                              const QList<QwtLegendData> &data)
    {
        if (!plotItem->isVisible())
            QwtPlotLegendItem::updateLegend(plotItem, QList<QwtLegendData>());
        else
            QwtPlotLegendItem::updateLegend(plotItem, data);
    }

    virtual QSize minimumSize( const QwtLegendData &data ) const
    {
        QSize size = QwtPlotLegendItem::minimumSize(data);

        int w = 0;
        int h = 0;

        const QwtText text = QString("10000.00");
        if (!text.isEmpty())
        {
            const QSizeF sz = text.textSize(font());

            w += qCeil(sz.width());
            h = qMax(h, qCeil(sz.height()));
            w += itemSpacing();
        }
        size += QSize(w, 0);
        return size;
    }

protected:
    virtual void drawLegendData(QPainter *painter,
                                const QwtPlotItem *plotItem,
                                const QwtLegendData &data,
                                const QRectF &rect) const
    {
        const int m = itemMargin();
        const QRectF r = rect.toRect().adjusted(m, m, -m, -m);

        painter->setClipRect(r, Qt::IntersectClip);

        int off = 0;

        const QwtGraphic graphic = data.icon();
        if (!graphic.isEmpty())
        {
            QRectF iconRect(r.topLeft(), graphic.defaultSize());

            iconRect.moveCenter(QPoint(iconRect.center().x(), rect.center().y()));

            graphic.render(painter, iconRect, Qt::KeepAspectRatio);

            off += iconRect.width() + itemSpacing();
        }

        QwtText text = data.title();
        if (!text.isEmpty())
        {
            painter->setPen(textPen());
            painter->setFont(font());

            const QRectF textRect = r.adjusted(off, 0, 0, 0);
            text.draw(painter, textRect);

            off += text.textSize(font()).width() + itemSpacing();
        }

        const QwtPlotCurve *curve = dynamic_cast<const QwtPlotCurve *>(plotItem);
        if (!curve)
            return;

        const CurveData *curveData = static_cast<const CurveData *>(curve->data());

        if (curveData->size())
            text = QString::number(curveData->sample(curveData->size() - 1).y(), 'f', 2);
        else
            text = QString();

        if (!text.isEmpty())
        {
            painter->setPen(textPen());
            painter->setFont(font());

            const QRectF textRect = r.adjusted(off, 0, 0, 0);
            text.draw( painter, textRect );
        }
    }
};

class QwtPlotGappedCurve: public QwtPlotCurve
{
public:
    explicit QwtPlotGappedCurve(const QString &title = QString::null) : QwtPlotCurve(title)
    {

    }

    virtual void drawSeries(QPainter *painter, const QwtScaleMap &xMap,
                            const QwtScaleMap &yMap, const QRectF &canvRect, int from, int to) const
    {
        if (!painter || dataSize() <= 0)
            return;

        if (to < 0)
            to = dataSize() - 1;

        int start = -1;
        int i = from;
        for (; i < to; ++i) {
            double y = sample(i).y();
            if (start == -1 && !qIsNaN(y))
                start = i;
            if (start != -1 && qIsNaN(y)) {
                QwtPlotCurve::drawSeries(painter, xMap, yMap, canvRect, start, i - 1);
                start = -1;
            }
        }
        if (start != -1)
            QwtPlotCurve::drawSeries(painter, xMap, yMap, canvRect, start, i - 1);
    }
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

class QwtMyPlotPicker : public QwtPlotPicker
{
public:
    explicit QwtMyPlotPicker(int xAxis, int yAxis,
        RubberBand rubberBand, DisplayMode trackerMode, QWidget *w) :
        QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, w)
    {

    }

protected:
    virtual QwtText trackerTextF(const QPointF &pos) const
    {
        QString text;
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(pos.x()));
        QString x = dt.toString("hh:mm:ss.zzz");

        switch (rubberBand())
        {
        case HLineRubberBand:
            text.sprintf("%.2f", pos.y());
            break;
        case VLineRubberBand:
            text = x;
            break;
        default:
            text = x + QString(", ") + QString::number(pos.y(), 'f', 2);
        }
        return QwtText(text);
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

#if 1
static const char *colors[] =
{
    "#ffc0cb", // 1
    "#ff0000",
    "#008080",
    "#0000ff",
    "#00ffff",
    "#787878",
    "#40e0d0",
    "#ffd700",
    "#ff7373",
    "#89AEB3", // 10
    "#595959",
    "#FF8173",
    "#AFFFA6",
    "#5C5C5C",
    "#f6546a",
    "#666666",
    "#333333",
    "#ffa500",
    "#7fffd4",
    "#003366", // 20
    "#00ff00",
    "#468499",
    "#66B8FF",
    "#800080",
    "#00ced1",
    "#ffff00",
    "#ffc3a0",
    "#660066",
    "#66cdaa",
    "#F5926E", // 30
    "#088da5",
    "#20b2aa",
    "#95B9DE",
    "#ffb6c1",
    "#fa8072",
    "#fff68f",
    "#800000",
    "#C7A16D",
    "#ADADCC",
    "#81d8d0", // 40
    "#990000",
    "#917460",
    "#97CFCF",
    "#ccff00",
    "#ff00ff",
    "#7D7D25",
    "#ff6666",
    "#008000",
    "#66cccc",
    "#6E1E1E", // 50
    "#00ff7f",
    "#8C6666",
    "#FC8C28",
    "#ff4040",
    "#0DDB68",
    "#daa520",
    "#171717",
    "#3b5998",
    "#7ECC7E",
    "#668294"
};
#else
static const char *colors[] =
{
    "#FFDEAD", // 1
    "#E6E6FA",
    "#2F4F4F",
    "#708090",
    "#191970",
    "#000080",
    "#483D8B",
    "#0000CD",
    "#0000FF",
    "#4682B4", // 10
    "#00CED1",
    "#00FFFF",
    "#5F9EA0",
    "#006400",
    "#556B2F",
    "#3CB371",
    "#7CFC00",
    "#228B22",
    "#FFFF00",
    "#FFD700", // 20
    "#DAA520",
    "#BC8F8F",
    "#CD5C5C",
    "#8B4513",
    "#A0522D",
    "#CD853F",
    "#B22222",
    "#A52A2A",
    "#FA8072",
    "#FFA500", // 30
    "#FF7F50",
    "#FF4500",
    "#FF0000",
    "#FF69B4",
    "#FF1493",
    "#DB7093",
    "#B03060",
    "#C71585",
    "#D02090",
    "#FF00FF", // 40
    "#BA55D3",
    "#9932CC",
    "#9400D3",
    "#8A2BE2",
    "#A020F0",
    "#8B8989",
    "#8B8682",
    "#8B8378",
    "#4F4F4F",
    "#00008B", // 50
    "#008B8B",
    "#8B008B",
    "#8B0000",
    "#FF7F00",
    "#FFA500",
    "#FF8C69",
    "#8B7355",
    "#68838B",
    "#6E7B8B",
    "#4A708B"
};
#endif

static const int numColors = sizeof(colors) / sizeof(colors[0]);

static QwtSymbol::Style symbols[] = {
    QwtSymbol::Ellipse,
    QwtSymbol::Rect,
    QwtSymbol::Diamond,
    QwtSymbol::Triangle,
    QwtSymbol::Cross,
    QwtSymbol::Star1,
    QwtSymbol::Star2
};

static const int numSymbols = sizeof(symbols) / sizeof(symbols[0]);

enum CurveType {
    ContinousCurve,
    StageCurve
};

static QwtPlotCurve *createCurve(CurveType curveType, const QString &name, int num) {
    QwtPlotCurve *curve = new QwtPlotGappedCurve(name);
    QColor color = QColor(colors[(num + 1) % numColors]);
    color.setAlpha(200);
    curve->setPen(color, 2);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setData(new CurveData);

    curve->setSymbol(new QwtSymbol(symbols[(num + 1) % numSymbols], QColor(colors[(numColors - num) % numColors]),
            QPen(color), QSize(5, 5)));

    if (curveType == ContinousCurve) {

    } else {
        curve->setStyle(QwtPlotCurve::Steps);
        curve->setCurveAttribute(QwtPlotCurve::Inverted);
    }

    return curve;
}

void WidgetPlot::onIoslotAdded(int num)
{
    QSharedPointer<Ioslot> ioslot = d_hponic->ioslotManager()->ioslot(num);
    if (ioslot->type() != UnknownIoslotType) {
        CurveType curveType = (ioslot->type() == AnalogInputType)? ContinousCurve: StageCurve;
        QwtPlotCurve *curve = createCurve(curveType, ioslot->name(), num);
        curve->attach(d_plot);

        d_curves.append(curve);
        showCurve(curve, false);
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

    QSharedPointer<Ioslot> ioslot = d_hponic->ioslotManager()->ioslot(num);
    if (ioslot->type() != UnknownIoslotType) {
        CurveType curveType = (ioslot->type() == AnalogInputType)? ContinousCurve: StageCurve;
        QwtPlotCurve *curve = createCurve(curveType, ioslot->name(), num);
        curve->attach(d_plot);

        d_curves[num] = curve;
        showCurve(curve, false);
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

void WidgetPlot::onImmediatetyUpdated(const IoslotValueRecord &record)
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
    d_dteFrom->setToolTip(tr("Only in offline mode. First time point of the data"));

    d_lTo = new QLabel(tr("To:"), this);
    d_dteTo = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    d_dteTo->setEnabled(false);
    d_dteTo->setToolTip(tr("Only in offline mode. Last time point of the data"));

    d_pbResetTo = new QPushButton(tr("C"), this);
    d_pbResetTo->setEnabled(false);
    d_pbResetTo->setToolTip(tr("Reset last time point to the current date and time"));

    d_tbSelect = new QToolButton(this);
    d_tbSelect->setIcon(QIcon("://icons/database_go.png"));
    d_tbSelect->setText(tr("Select"));
    d_tbSelect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    d_tbSelect->setEnabled(false);
    d_tbSelect->setToolTip(tr("Select data from the database and show them on the plot"));

    d_lRecordCount = new QLabel(this);

    d_tbExportTo = new QToolButton(this);
    d_tbExportTo->setPopupMode(QToolButton::InstantPopup);
    d_tbExportTo->setIcon(QIcon("://icons/document_export.png"));
    d_tbExportTo->setText(tr("Export to"));
    d_tbExportTo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    d_tbExportTo->setEnabled(false);
    d_tbExportTo->setToolTip(tr("Export selected data to a specific file format"));

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

    d_legendItem = new LegendItem;
    d_legendItem->attach(d_plot);

    d_legendItem->setMaxColumns(1);
    d_legendItem->setAlignment(Qt::AlignRight);
    d_legendItem->setBackgroundMode(QwtPlotLegendItem::LegendBackground);
    d_legendItem->setBorderRadius(8);
    d_legendItem->setMargin(4);
    d_legendItem->setSpacing(2);
    d_legendItem->setItemMargin(0);

    QwtMyPlotPicker *picker = new QwtMyPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                                  QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
                                                  d_plot->canvas());
    picker->setRubberBandPen(QColor(Qt::green));
    picker->setRubberBand(QwtPicker::CrossRubberBand );
    picker->setTrackerPen(QColor(Qt::black));

    d_cbAutoScale = new QCheckBox(tr("Auto scale"), this);
    d_cbAutoScale->setChecked(false);

    d_dsbMaxy = new QDoubleSpinBox(this);
    d_dsbMaxy->setRange(-10000.0, 10000.0);
    d_dsbMaxy->setValue(1024.0);
    d_dsbMaxy->setToolTip(tr("Upper limit on the plot"));

    d_dsbMiny = new QDoubleSpinBox(this);
    d_dsbMiny->setRange(-10000.0, 10000.0);
    d_dsbMiny->setValue(0.0);
    d_dsbMiny->setToolTip(tr("Lower limit on the plot"));

    d_sbOffset = new QScrollBar(Qt::Horizontal, this);
    d_sbOffset->setRange(0, 0);
    d_sbOffset->setValue(0);

    d_teInterval = new QTimeEdit(this);
    d_teInterval->setDisplayFormat("hh:mm:ss");
    d_teInterval->setTime(QTime(d_interval.width() / (1000.0 * 3600.0),
                                d_interval.width() / (1000.0 * 60.0),
                                d_interval.width() / (1000.0 * 1.0)));
    d_teInterval->setToolTip(tr("Width of the time window"));

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
    layoutMain->addWidget(d_dsbMaxy,    row, 0, 1, 1, Qt::AlignLeft);
    layoutMain->addWidget(d_cbAutoScale,row, 1, 1, 1, Qt::AlignLeft);
    ++row;
    layoutMain->addLayout(layoutPlot,   row, 0, 3, 4, Qt::AlignCenter);
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
    connect(d_hponic->databaseProducer().data(), SIGNAL(immediatelyUpdated(IoslotValueRecord)), this, SLOT(onImmediatetyUpdated(IoslotValueRecord)), Qt::DirectConnection);

    connect(d_hponic.data(), SIGNAL(exportStarted()), this, SLOT(onExportStarted()), Qt::DirectConnection);
    connect(d_hponic.data(), SIGNAL(exportStopped()), this, SLOT(onExportStopped()), Qt::DirectConnection);
}

void WidgetPlot::createCurves()
{
    for (int num = 0; num < d_hponic->ioslotManager()->ioslotCount(); ++num) {
        QSharedPointer<Ioslot> ioslot = d_hponic->ioslotManager()->ioslot(num);
        if (ioslot->type() != UnknownIoslotType) {
            CurveType curveType = (ioslot->type() == AnalogInputType)? ContinousCurve: StageCurve;
            QwtPlotCurve *curve = createCurve(curveType, ioslot->name(), num);
            curve->attach(d_plot);

            d_curves.append(curve);
            showCurve(curve, false);
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

    for (int num = 0; it != d_curves.end(); ++it, ++it2, ++num) {
        QwtPlotCurve *curve = *it;
        if (!curve) continue;

        CurveData *data = static_cast<CurveData *>(curve->data());
        data->append(QPointF(x, it2->second));
    }

    d_plot->replot();
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
        if (legendLabel) {
            legendLabel->setChecked(on);
            d_plot->updateLegend(item);
        }
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
