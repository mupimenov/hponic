#include "widget_plot.h"
#include "ui_widget_plot.h"

#include <QDateTime>
#include <QGridLayout>
#include <QPointF>

#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>

#include <qwt_painter.h>
#include <qwt_scale_map.h>
#include <qwt_scale_draw.h>

#define MODE_DISABLED 0
#define MODE_ONLINE 1
#define MODE_OFFLINE 2

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
    CurveData() : QwtSeriesData<QPointF>(),
        d_boundingRect(1.0, 1.0, -2.0, -2.0)  {
        d_values.reserve(1000);
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
        if (d_values.size() == 1000) {
            d_values.clear();
            d_boundingRect.setRect(1.0, 1.0, -2.0, -2.0);
        }

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

WidgetPlot::WidgetPlot(QSharedPointer<Hponic> __hponic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPlot),
    d_hponic(__hponic),
    d_interval(0.0, 10 * 1000.0),
    d_online(false)
{
    ui->setupUi(this);

    createWidgets();
    createLayouts();
    createConnections();
}

WidgetPlot::~WidgetPlot()
{
    delete d_directPainter;
    delete ui;
}

void WidgetPlot::changedMaxY(double __maxY)
{
    d_plot->setAxisScale(QwtPlot::yLeft, d_dsbMinY->value(), __maxY);
}

void WidgetPlot::changedMinY(double __minY)
{
    d_plot->setAxisScale(QwtPlot::yLeft, __minY, d_dsbMaxY->value());
}

void WidgetPlot::changedOffset(int __offset)
{
    if (d_curves.size() == 0)
        return;

    QwtPlotCurve *curve = d_curves[0];
    CurveData *data = static_cast<CurveData *>(curve->data());
    QRectF rect = data->boundingRect();
    double k = (rect.right() - rect.left()) / (d_sbOffset->maximum());
    double min = k * __offset + rect.left();

    d_interval.setInterval(min, min + d_interval.width());
    d_plot->setAxisScale(QwtPlot::xBottom,
        d_interval.minValue(), d_interval.maxValue());

    d_plot->replot();
}

void WidgetPlot::changedInterval(const QTime &interval)
{
    setInterval(-interval.msecsTo(QTime(0, 0, 0, 0)));
}

void WidgetPlot::changedMode(int __index)
{
    int mode = d_cbMode->itemData(__index).toInt();
    bool enable = true;
    switch (mode) {
    case MODE_DISABLED:
        enable = false;
        d_online = false;
        break;
    case MODE_ONLINE:
        enable = false;
        d_online = true;
        break;
    case MODE_OFFLINE:
        break;
    default:
        break;
    }

    d_dteFrom->setEnabled(enable);
    d_dteTo->setEnabled(enable);
}

void WidgetPlot::onIoslotAdded(int __num)
{
    static int counter = 1;

    const char *colors[] =
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
    const int numColors = sizeof( colors ) / sizeof( colors[0] );

    QwtPlotCurve *curve = new QwtPlotCurve(d_hponic->ioslotManager()->ioslot(__num)->name());
    curve->setPen(QColor(colors[counter % numColors]), 2);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setData(new CurveData);
    curve->attach(d_plot);

    d_curves.append(curve);
    ++counter;
}

void WidgetPlot::onRecordUpdated(const IoslotValueRecord &__record)
{
    if (!d_online) return;

    double x = static_cast<double>(__record.timestamp().toMSecsSinceEpoch());
    QList<QwtPlotCurve*>::iterator it = d_curves.begin();
    QList<IoslotValueRecord::RecordValue>::const_iterator it2 = __record.values().begin();
    for (; it != d_curves.end(); ++it, ++it2) {
        QwtPlotCurve *curve = *it;
        CurveData *data = static_cast<CurveData *>(curve->data());
        data->append(QPointF(x, it2->second));

        updateCurve(curve);
    }

    if (x >= d_interval.maxValue()) {
        d_interval.setInterval(x, x + d_interval.width());

        d_plot->setAxisScale(QwtPlot::xBottom,
            d_interval.minValue(), d_interval.maxValue());

        d_plot->replot();
    }
}

void WidgetPlot::setInterval(double interval)
{
    if (interval > 0.0 && interval != d_interval.width()) {
        double min = double(QDateTime::currentMSecsSinceEpoch());
        d_interval.setInterval(min, min + interval);
        d_plot->setAxisScale(QwtPlot::xBottom,
            d_interval.minValue(), d_interval.maxValue());

        d_plot->replot();
    }
}

void WidgetPlot::createWidgets()
{
    d_lMode = new QLabel(tr("Mode:"), this);
    d_cbMode = new QComboBox(this);
    d_cbMode->addItem(tr("Disabled"), QVariant(MODE_DISABLED));
    d_cbMode->addItem(tr("Online"), QVariant(MODE_ONLINE));
    d_cbMode->addItem(tr("Offline"), QVariant(MODE_OFFLINE));

    d_lFrom = new QLabel(tr("From:"), this);
    d_dteFrom = new QDateTimeEdit(this);
    d_dteFrom->setEnabled(false);

    d_lTo = new QLabel(tr("To:"), this);
    d_dteTo = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    d_dteTo->setEnabled(false);

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

    d_dsbMaxY = new QDoubleSpinBox(this);
    d_dsbMaxY->setRange(-1024.0, 1024.0);
    d_dsbMaxY->setValue(1.0);

    d_dsbMinY = new QDoubleSpinBox(this);
    d_dsbMinY->setRange(-1024.0, 1024.0);
    d_dsbMinY->setValue(0.0);

    d_sbOffset = new QScrollBar(Qt::Horizontal, this);
    d_sbOffset->setRange(0, 100);
    d_sbOffset->setValue(100);

    d_teInterval = new QTimeEdit(this);
    d_teInterval->setDisplayFormat("hh:mm:ss");
    d_teInterval->setTime(QTime(0, 1, 0));

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
    layoutMode->addStretch(1);

    QHBoxLayout *layoutPlot = new QHBoxLayout;
    layoutPlot->addWidget(d_plot, 1);

    QHBoxLayout *layoutOffset = new QHBoxLayout;
    layoutOffset->addWidget(d_sbOffset, 1);

    QGridLayout *layoutMain = new QGridLayout;
    int row = 0;
    layoutMain->addLayout(layoutMode,   row, 0, 1, 4, Qt::AlignCenter);
    row += 1;
    layoutMain->addWidget(d_dsbMaxY,     row, 0, 1, 1, Qt::AlignLeft);
    layoutMain->addLayout(layoutPlot,   row, 1, 3, 3, Qt::AlignCenter);
    row += 3;
    layoutMain->addWidget(d_dsbMinY,     row, 0, 1, 1, Qt::AlignLeft);
    layoutMain->addLayout(layoutOffset, row, 1, 1, 3, Qt::AlignCenter);
    row += 1;
    layoutMain->addWidget(d_teInterval,  row, 2, 1, 1, Qt::AlignCenter);

    layoutMain->setRowStretch(1, 1);
    layoutMain->setColumnStretch(1, 1);

    setLayout(layoutMain);
}

void WidgetPlot::createConnections()
{
    connect(d_dsbMaxY, SIGNAL(valueChanged(double)), this, SLOT(changedMaxY(double)), Qt::DirectConnection);
    connect(d_dsbMinY, SIGNAL(valueChanged(double)), this, SLOT(changedMinY(double)), Qt::DirectConnection);
    connect(d_sbOffset, SIGNAL(valueChanged(int)), this, SLOT(changedOffset(int)), Qt::DirectConnection);
    connect(d_teInterval, SIGNAL(timeChanged(QTime)), this, SLOT(changedInterval(QTime)), Qt::DirectConnection);
    connect(d_cbMode, SIGNAL(currentIndexChanged(int)), this, SLOT(changedMode(int)), Qt::DirectConnection);

    connect(d_hponic->ioslotManager().data(), SIGNAL(ioslotAdded(int)), this, SLOT(onIoslotAdded(int)), Qt::DirectConnection);
    connect(d_hponic->databaseProducer().data(), SIGNAL(recordUpdated(IoslotValueRecord)), this, SLOT(onRecordUpdated(IoslotValueRecord)), Qt::DirectConnection);
}

void WidgetPlot::updateCurve(QwtPlotCurve *__curve)
{
    CurveData *data = static_cast<CurveData *>(__curve->data());

    const int numPoints = data->size();
    if (numPoints) {
        const bool doClip = !d_plot->canvas()->testAttribute(Qt::WA_PaintOnScreen);
        if (doClip) {
            const QwtScaleMap xMap = d_plot->canvasMap(__curve->xAxis());
            const QwtScaleMap yMap = d_plot->canvasMap(__curve->yAxis());

            QRectF br = qwtBoundingRect(*data,
                numPoints - 2, numPoints - 1);

            const QRect clipRect = QwtScaleMap::transform(xMap, yMap, br).toRect();
            d_directPainter->setClipRegion(clipRect);
        }

        d_directPainter->drawSeries(__curve,
            numPoints - 2, numPoints - 1);
    }
}
