#include "widget_config_program.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

WidgetConfigEmptyProgram::WidgetConfigEmptyProgram(QSharedPointer<EmptyProgram> __program,
                                                   QSharedPointer<Hponic> __hponic,
                                                   QWidget *parent) : QWidget(parent),
    d_program(__program),
    d_hponic(__hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}


void WidgetConfigEmptyProgram::createWidgets()
{
    d_lType = new QLabel(tr("No program"), this);
    QFont f = d_lType->font();
    f.setBold(true);
    d_lType->setFont(f);
}

void WidgetConfigEmptyProgram::createLayouts()
{
    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addWidget(d_lType);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigEmptyProgram::createConnections()
{

}

WidgetConfigTimerControlProgram::WidgetConfigTimerControlProgram(QSharedPointer<TimerControlProgram> __program,
                                                                 QSharedPointer<Hponic> __hponic,
                                                                 QWidget *parent) : QWidget(parent),
    d_program(__program),
    d_hponic(__hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigTimerControlProgram::nameChanged(const QString &__name)
{
    d_program->setName(__name);
}

void WidgetConfigTimerControlProgram::timeConstrainsIndexChanged(int __index)
{
    int constrains = d_cbTimeConstrains->itemData(__index).toInt();
    d_program->setConstrains(constrains);

    bool enable = true;
    switch (constrains) {
    case ALL_TIME:
        enable = false;
        break;
    default:
        break;
    }

    d_lFrom->setEnabled(enable);
    d_dteFrom->setEnabled(enable);
    d_lTo->setEnabled(enable);
    d_dteTo->setEnabled(enable);
}

void WidgetConfigTimerControlProgram::fromChanged(const QDateTime &__dt)
{
    d_program->setFrom(__dt);
}

void WidgetConfigTimerControlProgram::toChanged(const QDateTime &__dt)
{
    d_program->setTo(__dt);
}

void WidgetConfigTimerControlProgram::cyclogramTypeIndexChanged(int __index)
{
    Cyclogram c = d_program->cyclogram();
    c.type = static_cast<Cyclogram::Type>(d_cbCyclogramType->itemData(__index).toInt());
    d_program->setCyclogram(c);

    bool enable = true;
    switch (c.type) {
    case Cyclogram::Simple:
        enable = false;
        break;
    default:
        break;
    }

    d_lCyclogramCount->setEnabled(enable);
    d_sbCyclogramCount->setEnabled(enable);
    d_lCyclogramImpulseDuration->setEnabled(enable);
    d_sbCyclogramImpulseDuration->setEnabled(enable);
    d_lCyclogramPauseDuration->setEnabled(enable);
    d_sbCyclogramPauseDuration->setEnabled(enable);
}

void WidgetConfigTimerControlProgram::cyclogramCountChanged(int __count)
{
    Cyclogram c = d_program->cyclogram();
    c.count = __count;
    d_program->setCyclogram(c);
}

void WidgetConfigTimerControlProgram::cyclogramImpulseDurationChanged(int __duration)
{
    Cyclogram c = d_program->cyclogram();
    c.impulseDuration = __duration;
    d_program->setCyclogram(c);
}

void WidgetConfigTimerControlProgram::cyclogramPauseDurationChanged(int __duration)
{
    Cyclogram c = d_program->cyclogram();
    c.pauseDuration = __duration;
    d_program->setCyclogram(c);
}

void WidgetConfigTimerControlProgram::outputIndexActivated(int __index)
{
    Q_UNUSED(__index);
}

void WidgetConfigTimerControlProgram::outputIndexChanged(int __index)
{
    QVariant data = d_cbOutput->itemData(__index);
    QSharedPointer<Ioslot> ioslot = data.value<QSharedPointer<Ioslot> >();

    d_program->setOutput(ioslot->id());
}

void WidgetConfigTimerControlProgram::createWidgets()
{
    d_lType = new QLabel(tr("Timer control program"), this);
    QFont f = d_lType->font();
    f.setBold(true);
    d_lType->setFont(f);

    d_lName = new QLabel(tr("Program name:"), this);
    d_leName = new QLineEdit(d_program->name(), this);

    d_lTimeConstrains = new QLabel(tr("Time constrains:"), this);
    d_cbTimeConstrains = new QComboBox(this);
    d_cbTimeConstrains->addItem(tr("All time"), QVariant(ALL_TIME));
    d_cbTimeConstrains->addItem(tr("Strict equality"), QVariant(STRICT_EQUALITY));
    d_cbTimeConstrains->addItem(tr("Every day"), QVariant(EVERY_DAY));
    d_cbTimeConstrains->addItem(tr("Every month"), QVariant(EVERY_MONTH));
    d_cbTimeConstrains->addItem(tr("Every year"), QVariant(EVERY_YEAR));
    d_cbTimeConstrains->setCurrentIndex(d_program->constrains());

    d_lFrom = new QLabel(tr("From:"), this);
    d_dteFrom = new QDateTimeEdit(d_program->from(), this);
    d_lTo = new QLabel(tr("To:"), this);
    d_dteTo = new QDateTimeEdit(d_program->to(), this);

    d_lCyclogram = new QLabel(tr("Cyclogram:"), this);

    d_cbCyclogramType = new QComboBox(this);
    d_cbCyclogramType->addItem(tr("Simple"), QVariant(static_cast<int>(Cyclogram::Simple)));
    d_cbCyclogramType->addItem(tr("Impulse"), QVariant(static_cast<int>(Cyclogram::Impulse)));
    d_cbCyclogramType->setCurrentIndex(d_program->cyclogram().type == Cyclogram::Simple? 0: 1);    

    d_lCyclogramCount = new QLabel(tr("Count:"), this);
    d_sbCyclogramCount = new QSpinBox(this);
    d_sbCyclogramCount->setRange(0, 65535);
    d_sbCyclogramCount->setValue(d_program->cyclogram().count);

    d_lCyclogramImpulseDuration = new QLabel(tr("Impulse duration:"), this);
    d_sbCyclogramImpulseDuration = new QSpinBox(this);
    d_sbCyclogramImpulseDuration->setRange(0, 65535);
    d_sbCyclogramImpulseDuration->setValue(d_program->cyclogram().impulseDuration);

    d_lCyclogramPauseDuration = new QLabel(tr("Pause duration:"), this);
    d_sbCyclogramPauseDuration = new QSpinBox(this);
    d_sbCyclogramPauseDuration->setRange(0, 65535);
    d_sbCyclogramPauseDuration->setValue(d_program->cyclogram().pauseDuration);

    d_lOutput = new QLabel(tr("Output slot:"), this);
    d_cbOutput = new QComboBox(this);
    QList<int> types;
    types.append(DiscreteOutputType);
    QList<QSharedPointer<Ioslot> > ioslots = d_hponic->ioslotManager()->ioslotsByType(types);
    QList<QSharedPointer<Ioslot> >::iterator i = ioslots.begin();
    int index = 0;
    for (int j = 0; i != ioslots.end(); ++i, ++j) {
        QVariant v;
        v.setValue(*i);
        d_cbOutput->addItem((*i)->name(), v);
        if ((*i)->id() == d_program->output())
            index = j;
    }
    d_cbOutput->setCurrentIndex(index);

    // update controls
    timeConstrainsIndexChanged(d_cbTimeConstrains->currentIndex());
    cyclogramTypeIndexChanged(d_cbCyclogramType->currentIndex());
}

void WidgetConfigTimerControlProgram::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lType,           row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lName,           row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lTimeConstrains, row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbTimeConstrains,row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lFrom,           row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dteFrom,         row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lTo,             row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dteTo,           row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogram,      row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbCyclogramType, row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogramCount, row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbCyclogramCount,row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogramImpulseDuration,   row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbCyclogramImpulseDuration,  row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogramPauseDuration,   row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbCyclogramPauseDuration,  row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lOutput,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbOutput,      row, 1, 1, 3, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigTimerControlProgram::createConnections()
{
    connect(d_leName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)), Qt::DirectConnection);

    connect(d_cbTimeConstrains, SIGNAL(currentIndexChanged(int)), this, SLOT(timeConstrainsIndexChanged(int)), Qt::DirectConnection);
    connect(d_dteFrom, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(fromChanged(QDateTime)), Qt::DirectConnection);
    connect(d_dteTo, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(toChanged(QDateTime)), Qt::DirectConnection);

    connect(d_cbCyclogramType, SIGNAL(currentIndexChanged(int)), this, SLOT(cyclogramTypeIndexChanged(int)), Qt::DirectConnection);
    connect(d_sbCyclogramCount, SIGNAL(valueChanged(int)), this, SLOT(cyclogramCountChanged(int)), Qt::DirectConnection);
    connect(d_sbCyclogramImpulseDuration, SIGNAL(valueChanged(int)), this, SLOT(cyclogramImpulseDurationChanged(int)), Qt::DirectConnection);
    connect(d_sbCyclogramPauseDuration, SIGNAL(valueChanged(int)), this, SLOT(cyclogramPauseDurationChanged(int)), Qt::DirectConnection);

    connect(d_cbOutput, SIGNAL(currentIndexChanged(int)), this, SLOT(outputIndexChanged(int)), Qt::DirectConnection);
}

WidgetConfigRelayControlProgram::WidgetConfigRelayControlProgram(QSharedPointer<RelayControlProgram> __program,
                                                                 QSharedPointer<Hponic> __hponic,
                                                                 QWidget *parent) : QWidget(parent),
    d_program(__program),
    d_hponic(__hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigRelayControlProgram::nameChanged(const QString &__name)
{
    d_program->setName(__name);
}

void WidgetConfigRelayControlProgram::timeConstrainsIndexChanged(int __index)
{
    int constrains = d_cbTimeConstrains->itemData(__index).toInt();
    d_program->setConstrains(constrains);

    bool enable = true;
    switch (constrains) {
    case ALL_TIME:
        enable = false;
        break;
    default:
        break;
    }

    d_lFrom->setEnabled(enable);
    d_dteFrom->setEnabled(enable);
    d_lTo->setEnabled(enable);
    d_dteTo->setEnabled(enable);
}

void WidgetConfigRelayControlProgram::fromChanged(const QDateTime &__dt)
{
    Q_UNUSED(__dt);
}

void WidgetConfigRelayControlProgram::toChanged(const QDateTime &__dt)
{
    Q_UNUSED(__dt);
}

void WidgetConfigRelayControlProgram::inputIndexActivated(int __index)
{
    Q_UNUSED(__index);
}

void WidgetConfigRelayControlProgram::inputIndexChanged(int __index)
{
    QVariant data = d_cbInput->itemData(__index);
    QSharedPointer<Ioslot> ioslot = data.value<QSharedPointer<Ioslot> >();

    d_program->setInput(ioslot->id());
}

void WidgetConfigRelayControlProgram::lowBoundChanged(double __lowBound)
{
    d_program->setLowBound(__lowBound);
}

void WidgetConfigRelayControlProgram::highBoundChanged(double __highBound)
{
    d_program->setHighBound(__highBound);
}

void WidgetConfigRelayControlProgram::cyclogramTypeIndexChanged(int __index)
{
    Cyclogram c = d_program->cyclogram();
    c.type = static_cast<Cyclogram::Type>(d_cbCyclogramType->itemData(__index).toInt());
    d_program->setCyclogram(c);

    bool enable = true;
    switch (c.type) {
    case Cyclogram::Simple:
        enable = false;
        break;
    default:
        break;
    }

    d_lCyclogramCount->setEnabled(enable);
    d_sbCyclogramCount->setEnabled(enable);
    d_lCyclogramImpulseDuration->setEnabled(enable);
    d_sbCyclogramImpulseDuration->setEnabled(enable);
    d_lCyclogramPauseDuration->setEnabled(enable);
    d_sbCyclogramPauseDuration->setEnabled(enable);
}

void WidgetConfigRelayControlProgram::cyclogramCountChanged(int __count)
{
    Cyclogram c = d_program->cyclogram();
    c.count = __count;
    d_program->setCyclogram(c);
}

void WidgetConfigRelayControlProgram::cyclogramImpulseDurationChanged(int __duration)
{
    Cyclogram c = d_program->cyclogram();
    c.impulseDuration = __duration;
    d_program->setCyclogram(c);
}

void WidgetConfigRelayControlProgram::cyclogramPauseDurationChanged(int __duration)
{
    Cyclogram c = d_program->cyclogram();
    c.pauseDuration = __duration;
    d_program->setCyclogram(c);
}

void WidgetConfigRelayControlProgram::outputIndexActivated(int __index)
{
    Q_UNUSED(__index);
}

void WidgetConfigRelayControlProgram::outputIndexChanged(int __index)
{
    QVariant data = d_cbOutput->itemData(__index);
    QSharedPointer<Ioslot> ioslot = data.value<QSharedPointer<Ioslot> >();

    d_program->setOutput(ioslot->id());
}

void WidgetConfigRelayControlProgram::createWidgets()
{
    d_lType = new QLabel(tr("Relay control program"), this);
    QFont f = d_lType->font();
    f.setBold(true);
    d_lType->setFont(f);

    d_lName = new QLabel(tr("Program name:"), this);
    d_leName = new QLineEdit(d_program->name(), this);

    d_lTimeConstrains = new QLabel(tr("Time constrains:"), this);
    d_cbTimeConstrains = new QComboBox(this);
    d_cbTimeConstrains->addItem(tr("All time"), QVariant(ALL_TIME));
    d_cbTimeConstrains->addItem(tr("Strict equality"), QVariant(STRICT_EQUALITY));
    d_cbTimeConstrains->addItem(tr("Every day"), QVariant(EVERY_DAY));
    d_cbTimeConstrains->addItem(tr("Every month"), QVariant(EVERY_MONTH));
    d_cbTimeConstrains->addItem(tr("Every year"), QVariant(EVERY_YEAR));
    d_cbTimeConstrains->setCurrentIndex(d_program->constrains());

    d_lFrom = new QLabel(tr("From:"), this);
    d_dteFrom = new QDateTimeEdit(this);
    d_lTo = new QLabel(tr("To:"), this);
    d_dteTo = new QDateTimeEdit(this);

    d_lInput = new QLabel(tr("Input slot:"), this);
    d_cbInput = new QComboBox(this);

    QList<int> types;
    types.append(AnalogInputType);
    QList<QSharedPointer<Ioslot> > ioslots = d_hponic->ioslotManager()->ioslotsByType(types);
    QList<QSharedPointer<Ioslot> >::iterator i = ioslots.begin();
    int index = 0;
    for (int j = 0; i != ioslots.end(); ++i, ++j) {
        QVariant v;
        v.setValue(*i);
        d_cbInput->addItem((*i)->name(), v);
        if ((*i)->id() == d_program->input())
            index = j;
    }
    d_cbInput->setCurrentIndex(index);

    d_lLowBound = new QLabel(tr("Low bound:"), this);
    d_dsbLowBound = new QDoubleSpinBox(this);
    d_dsbLowBound->setValue(d_program->lowBound());
    d_lHighBound = new QLabel(tr("High bound:"), this);
    d_dsbHighBound = new QDoubleSpinBox(this);
    d_dsbHighBound->setValue(d_program->highBound());

    d_lCyclogram = new QLabel(tr("Cyclogram:"), this);
    d_cbCyclogramType = new QComboBox(this);
    d_cbCyclogramType->addItem(tr("Simple"), QVariant(static_cast<int>(Cyclogram::Simple)));
    d_cbCyclogramType->addItem(tr("Impulse"), QVariant(static_cast<int>(Cyclogram::Impulse)));
    d_cbCyclogramType->setCurrentIndex(d_program->cyclogram().type == Cyclogram::Simple? 0: 1);

    d_lCyclogramCount = new QLabel(tr("Count:"), this);
    d_sbCyclogramCount = new QSpinBox(this);
    d_sbCyclogramCount->setRange(0, 65535);
    d_sbCyclogramCount->setValue(d_program->cyclogram().count);

    d_lCyclogramImpulseDuration = new QLabel(tr("Impulse duration:"), this);
    d_sbCyclogramImpulseDuration = new QSpinBox(this);
    d_sbCyclogramImpulseDuration->setRange(0, 65535);
    d_sbCyclogramImpulseDuration->setValue(d_program->cyclogram().impulseDuration);

    d_lCyclogramPauseDuration = new QLabel(tr("Pause duration:"), this);
    d_sbCyclogramPauseDuration = new QSpinBox(this);
    d_sbCyclogramPauseDuration->setRange(0, 65535);
    d_sbCyclogramPauseDuration->setValue(d_program->cyclogram().pauseDuration);

    d_lOutput = new QLabel(tr("Output slot:"), this);
    d_cbOutput = new QComboBox(this);
    types.clear();
    types.append(DiscreteOutputType);
    ioslots = d_hponic->ioslotManager()->ioslotsByType(types);
    i = ioslots.begin();
    index = 0;
    for (int j = 0; i != ioslots.end(); ++i, ++j) {
        QVariant v;
        v.setValue(*i);
        d_cbOutput->addItem((*i)->name(), v);
        if ((*i)->id() == d_program->output())
            index = j;
    }
    d_cbOutput->setCurrentIndex(index);

    // update controls
    timeConstrainsIndexChanged(d_cbTimeConstrains->currentIndex());
    cyclogramTypeIndexChanged(d_cbCyclogramType->currentIndex());
}

void WidgetConfigRelayControlProgram::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lType,           row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lName,           row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lTimeConstrains, row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbTimeConstrains,row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lFrom,           row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dteFrom,         row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lTo,             row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dteTo,           row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lInput,          row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbInput,         row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lLowBound,       row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dsbLowBound,     row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lHighBound,      row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dsbHighBound,    row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogram,      row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbCyclogramType, row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogramCount, row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbCyclogramCount,row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogramImpulseDuration,   row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbCyclogramImpulseDuration,  row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lCyclogramPauseDuration,   row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_sbCyclogramPauseDuration,  row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lOutput,       row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbOutput,      row, 1, 1, 3, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigRelayControlProgram::createConnections()
{
    connect(d_leName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)), Qt::DirectConnection);

    connect(d_cbTimeConstrains, SIGNAL(currentIndexChanged(int)), this, SLOT(timeConstrainsIndexChanged(int)), Qt::DirectConnection);
    connect(d_dteFrom, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(fromChanged(QDateTime)), Qt::DirectConnection);
    connect(d_dteTo, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(toChanged(QDateTime)), Qt::DirectConnection);

    connect(d_cbInput, SIGNAL(activated(int)), this, SLOT(inputIndexActivated(int)), Qt::DirectConnection);
    connect(d_cbInput, SIGNAL(currentIndexChanged(int)), this, SLOT(inputIndexChanged(int)), Qt::DirectConnection);

    connect(d_dsbLowBound, SIGNAL(valueChanged(double)), this, SLOT(lowBoundChanged(double)), Qt::DirectConnection);
    connect(d_dsbHighBound, SIGNAL(valueChanged(double)), this, SLOT(highBoundChanged(double)), Qt::DirectConnection);

    connect(d_cbCyclogramType, SIGNAL(currentIndexChanged(int)), this, SLOT(cyclogramTypeIndexChanged(int)), Qt::DirectConnection);
    connect(d_sbCyclogramCount, SIGNAL(valueChanged(int)), this, SLOT(cyclogramCountChanged(int)), Qt::DirectConnection);
    connect(d_sbCyclogramImpulseDuration, SIGNAL(valueChanged(int)), this, SLOT(cyclogramImpulseDurationChanged(int)), Qt::DirectConnection);
    connect(d_sbCyclogramPauseDuration, SIGNAL(valueChanged(int)), this, SLOT(cyclogramPauseDurationChanged(int)), Qt::DirectConnection);

    connect(d_cbOutput, SIGNAL(currentIndexChanged(int)), this, SLOT(outputIndexChanged(int)), Qt::DirectConnection);
}

WidgetConfigPidControlProgram::WidgetConfigPidControlProgram(QSharedPointer<PidControlProgram> __program,
                                                             QSharedPointer<Hponic> __hponic,
                                                             QWidget *parent) : QWidget(parent),
    d_program(__program),
    d_hponic(__hponic)
{
    createWidgets();
    createLayouts();
    createConnections();
}

void WidgetConfigPidControlProgram::nameChanged(const QString &__name)
{
    d_program->setName(__name);
}

void WidgetConfigPidControlProgram::timeConstrainsIndexChanged(int __index)
{
    int constrains = d_cbTimeConstrains->itemData(__index).toInt();
    d_program->setConstrains(constrains);

    bool enable = true;
    switch (constrains) {
    case ALL_TIME:
        enable = false;
        break;
    default:
        break;
    }

    d_lFrom->setEnabled(enable);
    d_dteFrom->setEnabled(enable);
    d_lTo->setEnabled(enable);
    d_dteTo->setEnabled(enable);
}

void WidgetConfigPidControlProgram::fromChanged(const QDateTime &__dt)
{
    Q_UNUSED(__dt);
}

void WidgetConfigPidControlProgram::toChanged(const QDateTime &__dt)
{
    Q_UNUSED(__dt);
}

void WidgetConfigPidControlProgram::inputIndexActivated(int __index)
{
    Q_UNUSED(__index);
}

void WidgetConfigPidControlProgram::inputIndexChanged(int __index)
{
    QVariant data = d_cbInput->itemData(__index);
    QSharedPointer<Ioslot> ioslot = data.value<QSharedPointer<Ioslot> >();

    d_program->setInput(ioslot->id());
}

void WidgetConfigPidControlProgram::proportionalChanged(double __gain)
{
    d_program->setProportional(__gain);
}

void WidgetConfigPidControlProgram::integralChanged(double __gain)
{
    d_program->setIntegral(__gain);
}

void WidgetConfigPidControlProgram::differentialChanged(double __gain)
{
    d_program->setDifferential(__gain);
}

void WidgetConfigPidControlProgram::outputIndexActivated(int __index)
{
    Q_UNUSED(__index);
}

void WidgetConfigPidControlProgram::outputIndexChanged(int __index)
{
    QVariant data = d_cbOutput->itemData(__index);
    QSharedPointer<Ioslot> ioslot = data.value<QSharedPointer<Ioslot> >();

    d_program->setOutput(ioslot->id());
}

void WidgetConfigPidControlProgram::createWidgets()
{
    d_lType = new QLabel(tr("PID control program"), this);
    QFont f = d_lType->font();
    f.setBold(true);
    d_lType->setFont(f);

    d_lName = new QLabel(tr("Program name:"), this);
    d_leName = new QLineEdit(d_program->name(), this);

    d_lTimeConstrains = new QLabel(tr("Time constrains:"), this);
    d_cbTimeConstrains = new QComboBox(this);
    d_cbTimeConstrains->addItem(tr("All time"), QVariant(ALL_TIME));
    d_cbTimeConstrains->addItem(tr("Strict equality"), QVariant(STRICT_EQUALITY));
    d_cbTimeConstrains->addItem(tr("Every day"), QVariant(EVERY_DAY));
    d_cbTimeConstrains->addItem(tr("Every month"), QVariant(EVERY_MONTH));
    d_cbTimeConstrains->addItem(tr("Every year"), QVariant(EVERY_YEAR));
    d_cbTimeConstrains->setCurrentIndex(d_program->constrains());

    d_lFrom = new QLabel(tr("From:"), this);
    d_dteFrom = new QDateTimeEdit(this);
    d_lTo = new QLabel(tr("To:"), this);
    d_dteTo = new QDateTimeEdit(this);

    d_lInput = new QLabel(tr("Input slot:"), this);
    d_cbInput = new QComboBox(this);

    QList<int> types;
    types.append(AnalogInputType);
    QList<QSharedPointer<Ioslot> > ioslots = d_hponic->ioslotManager()->ioslotsByType(types);
    QList<QSharedPointer<Ioslot> >::iterator i = ioslots.begin();
    int index = 0;
    for (int j = 0; i != ioslots.end(); ++i, ++j) {
        QVariant v;
        v.setValue(*i);
        d_cbInput->addItem((*i)->name(), v);
        if ((*i)->id() == d_program->input())
            index = j;
    }
    d_cbInput->setCurrentIndex(index);

    d_lProportional = new QLabel(tr("Proportional gain:"), this);
    d_dsbProportional = new QDoubleSpinBox(this);
    d_dsbProportional->setValue(d_program->proportional());
    d_lIntegral = new QLabel(tr("Integral gain:"), this);
    d_dsbIntegral = new QDoubleSpinBox(this);
    d_dsbIntegral->setValue(d_program->integral());
    d_lDifferential = new QLabel(tr("Differential gain:"), this);
    d_dsbDifferential = new QDoubleSpinBox(this);
    d_dsbDifferential->setValue(d_program->differential());

    d_lOutput = new QLabel(tr("Output slot:"), this);
    d_cbOutput = new QComboBox(this);
    types.clear();
    types.append(DiscreteOutputType);
    ioslots = d_hponic->ioslotManager()->ioslotsByType(types);
    i = ioslots.begin();
    index = 0;
    for (int j = 0; i != ioslots.end(); ++i, ++j) {
        QVariant v;
        v.setValue(*i);
        d_cbOutput->addItem((*i)->name(), v);
        if ((*i)->id() == d_program->output())
            index = j;
    }
    d_cbOutput->setCurrentIndex(index);

    // update controls
    timeConstrainsIndexChanged(d_cbTimeConstrains->currentIndex());
}

void WidgetConfigPidControlProgram::createLayouts()
{
    QHBoxLayout *layoutName = new QHBoxLayout;
    layoutName->addWidget(d_leName, 1);

    QGridLayout *layoutControls = new QGridLayout;
    int row = 0;
    layoutControls->addWidget(d_lType,           row, 0, 1, 4, Qt::AlignCenter);
    ++row;
    layoutControls->addWidget(d_lName,           row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addLayout(layoutName,        row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lTimeConstrains, row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbTimeConstrains,row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lFrom,           row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dteFrom,         row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lTo,             row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dteTo,           row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lInput,          row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbInput,         row, 1, 1, 3, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lProportional,   row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dsbProportional, row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lIntegral,       row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dsbIntegral,     row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lDifferential,   row, 1, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_dsbDifferential, row, 2, 1, 1, Qt::AlignLeft);
    ++row;
    layoutControls->addWidget(d_lOutput,         row, 0, 1, 1, Qt::AlignLeft);
    layoutControls->addWidget(d_cbOutput,        row, 1, 1, 3, Qt::AlignLeft);

    QHBoxLayout *layoutGrid = new QHBoxLayout;
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(layoutControls);
    layoutGrid->addStretch(1);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutGrid);
    layoutMain->addStretch(1);

    setLayout(layoutMain);
}

void WidgetConfigPidControlProgram::createConnections()
{
    connect(d_leName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)), Qt::DirectConnection);

    connect(d_cbTimeConstrains, SIGNAL(currentIndexChanged(int)), this, SLOT(timeConstrainsIndexChanged(int)), Qt::DirectConnection);
    connect(d_dteFrom, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(fromChanged(QDateTime)), Qt::DirectConnection);
    connect(d_dteTo, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(toChanged(QDateTime)), Qt::DirectConnection);

    connect(d_cbInput, SIGNAL(activated(int)), this, SLOT(inputIndexActivated(int)), Qt::DirectConnection);
    connect(d_cbInput, SIGNAL(currentIndexChanged(int)), this, SLOT(inputIndexChanged(int)), Qt::DirectConnection);

    connect(d_dsbProportional, SIGNAL(valueChanged(double)), this, SLOT(proportionalChanged(double)), Qt::DirectConnection);
    connect(d_dsbIntegral, SIGNAL(valueChanged(double)), this, SLOT(integralChanged(double)), Qt::DirectConnection);
    connect(d_dsbDifferential, SIGNAL(valueChanged(double)), this, SLOT(differentialChanged(double)), Qt::DirectConnection);

    connect(d_cbOutput, SIGNAL(currentIndexChanged(int)), this, SLOT(outputIndexChanged(int)), Qt::DirectConnection);
}
