#ifndef WIDGETCONFIGPROGRAM_H
#define WIDGETCONFIGPROGRAM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>

#include <QSharedPointer>

#include "program.h"
#include "hponic.h"

class WidgetConfigEmptyProgram : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigEmptyProgram(QSharedPointer<EmptyProgram> __program,
                                      QSharedPointer<Hponic> __hponic,
                                      QWidget *parent = 0);

Q_SIGNALS:

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<EmptyProgram> d_program;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lType;
};

class WidgetConfigTimerControlProgram : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigTimerControlProgram(QSharedPointer<TimerControlProgram> __program,
                                             QSharedPointer<Hponic> __hponic,
                                             QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void nameChanged(const QString &__name);

    void timeConstrainsIndexChanged(int __index);
    void fromChanged(const QDateTime &__dt);
    void toChanged(const QDateTime &__dt);

    void cyclogramTypeIndexChanged(int __index);
    void cyclogramCountChanged(int __count);
    void cyclogramImpulseDurationChanged(int __duration);
    void cyclogramPauseDurationChanged(int __duration);

    void outputIndexActivated(int __index);
    void outputIndexChanged(int  __index);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<TimerControlProgram> d_program;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lType;

    QLabel *d_lName;
    QLineEdit *d_leName;

    QLabel *d_lTimeConstrains;
    QComboBox *d_cbTimeConstrains;

    QLabel *d_lFrom;
    QDateTimeEdit *d_dteFrom;

    QLabel *d_lTo;
    QDateTimeEdit *d_dteTo;

    QLabel *d_lCyclogram;
    QComboBox *d_cbCyclogramType;

    QLabel *d_lCyclogramCount;
    QSpinBox *d_sbCyclogramCount;

    QLabel *d_lCyclogramImpulseDuration;
    QSpinBox *d_sbCyclogramImpulseDuration;

    QLabel *d_lCyclogramPauseDuration;
    QSpinBox *d_sbCyclogramPauseDuration;

    QLabel *d_lOutput;
    QComboBox *d_cbOutput;

};

class WidgetConfigRelayControlProgram : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigRelayControlProgram(QSharedPointer<RelayControlProgram> __program,
                                             QSharedPointer<Hponic> __hponic,
                                             QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void nameChanged(const QString &__name);

    void timeConstrainsIndexChanged(int __index);
    void fromChanged(const QDateTime &__dt);
    void toChanged(const QDateTime &__dt);

    void inputIndexActivated(int __index);
    void inputIndexChanged(int  __index);

    void lowBoundChanged(double __lowBound);
    void highBoundChanged(double __highBound);

    void cyclogramTypeIndexChanged(int __index);
    void cyclogramCountChanged(int __count);
    void cyclogramImpulseDurationChanged(int __duration);
    void cyclogramPauseDurationChanged(int __duration);

    void outputIndexActivated(int __index);
    void outputIndexChanged(int  __index);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<RelayControlProgram> d_program;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lType;

    QLabel *d_lName;
    QLineEdit *d_leName;

    QLabel *d_lTimeConstrains;
    QComboBox *d_cbTimeConstrains;

    QLabel *d_lFrom;
    QDateTimeEdit *d_dteFrom;

    QLabel *d_lTo;
    QDateTimeEdit *d_dteTo;

    QLabel *d_lInput;
    QComboBox *d_cbInput;

    QLabel *d_lLowBound;
    QDoubleSpinBox *d_dsbLowBound;

    QLabel *d_lHighBound;
    QDoubleSpinBox *d_dsbHighBound;

    QLabel *d_lCyclogram;
    QComboBox *d_cbCyclogramType;

    QLabel *d_lCyclogramCount;
    QSpinBox *d_sbCyclogramCount;

    QLabel *d_lCyclogramImpulseDuration;
    QSpinBox *d_sbCyclogramImpulseDuration;

    QLabel *d_lCyclogramPauseDuration;
    QSpinBox *d_sbCyclogramPauseDuration;

    QLabel *d_lOutput;
    QComboBox *d_cbOutput;

};

class WidgetConfigPidControlProgram : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigPidControlProgram(QSharedPointer<PidControlProgram> __program,
                                           QSharedPointer<Hponic> __hponic,
                                           QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void nameChanged(const QString &__name);

    void timeConstrainsIndexChanged(int __index);
    void fromChanged(const QDateTime &__dt);
    void toChanged(const QDateTime &__dt);

    void inputIndexActivated(int __index);
    void inputIndexChanged(int  __index);

    void proportionalChanged(double __gain);
    void integralChanged(double __gain);
    void differentialChanged(double __gain);

    void outputIndexActivated(int __index);
    void outputIndexChanged(int  __index);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<PidControlProgram> d_program;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lType;

    QLabel *d_lName;
    QLineEdit *d_leName;

    QLabel *d_lTimeConstrains;
    QComboBox *d_cbTimeConstrains;

    QLabel *d_lFrom;
    QDateTimeEdit *d_dteFrom;

    QLabel *d_lTo;
    QDateTimeEdit *d_dteTo;

    QLabel *d_lInput;
    QComboBox *d_cbInput;

    QLabel *d_lProportional;
    QDoubleSpinBox *d_dsbProportional;

    QLabel *d_lIntegral;
    QDoubleSpinBox *d_dsbIntegral;

    QLabel *d_lDifferential;
    QDoubleSpinBox *d_dsbDifferential;

    QLabel *d_lOutput;
    QComboBox *d_cbOutput;

};

#endif // WIDGETCONFIGPROGRAM_H
