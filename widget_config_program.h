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
    explicit WidgetConfigEmptyProgram(QSharedPointer<EmptyProgram> program,
                                      QSharedPointer<Hponic> hponic,
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
    explicit WidgetConfigTimerControlProgram(QSharedPointer<TimerControlProgram> program,
                                             QSharedPointer<Hponic> hponic,
                                             QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void nameChanged(const QString &name);

    void timeConstrainsIndexChanged(int index);
    void fromChanged(const QDateTime &dt);
    void toChanged(const QDateTime &dt);

    void cyclogramTypeIndexChanged(int index);
    void cyclogramCountChanged(int count);
    void cyclogramImpulseDurationChanged(int duration);
    void cyclogramPauseDurationChanged(int duration);

    void outputIndexActivated(int index);
    void outputIndexChanged(int index);

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
    explicit WidgetConfigRelayControlProgram(QSharedPointer<RelayControlProgram> program,
                                             QSharedPointer<Hponic> hponic,
                                             QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void nameChanged(const QString &name);

    void timeConstrainsIndexChanged(int index);
    void fromChanged(const QDateTime &dt);
    void toChanged(const QDateTime &dt);

    void inputIndexActivated(int index);
    void inputIndexChanged(int index);

    void lowBoundChanged(double lowBound);
    void highBoundChanged(double highBound);

    void cyclogramTypeIndexChanged(int index);
    void cyclogramCountChanged(int count);
    void cyclogramImpulseDurationChanged(int duration);
    void cyclogramPauseDurationChanged(int duration);

    void inverseChanged(bool inverse);

    void outputIndexActivated(int index);
    void outputIndexChanged(int index);

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

    QCheckBox *d_cbInverse;

    QLabel *d_lOutput;
    QComboBox *d_cbOutput;

};

class WidgetConfigPidControlProgram : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigPidControlProgram(QSharedPointer<PidControlProgram> program,
                                           QSharedPointer<Hponic> hponic,
                                           QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void nameChanged(const QString &name);

    void timeConstrainsIndexChanged(int index);
    void fromChanged(const QDateTime &dt);
    void toChanged(const QDateTime &dt);

    void inputIndexActivated(int index);
    void inputIndexChanged(int index);

    void desiredChanged(double desired);

    void proportionalChanged(double gain);
    void integralChanged(double gain);
    void differentialChanged(double gain);

    void inverseChanged(bool inverse);

    void outputIndexActivated(int index);
    void outputIndexChanged(int index);

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

    QLabel *d_lDesired;
    QDoubleSpinBox *d_dsbDesired;

    QLabel *d_lProportional;
    QDoubleSpinBox *d_dsbProportional;

    QLabel *d_lIntegral;
    QDoubleSpinBox *d_dsbIntegral;

    QLabel *d_lDifferential;
    QDoubleSpinBox *d_dsbDifferential;

    QCheckBox *d_cbInverse;

    QLabel *d_lOutput;
    QComboBox *d_cbOutput;

};

#endif // WIDGETCONFIGPROGRAM_H
