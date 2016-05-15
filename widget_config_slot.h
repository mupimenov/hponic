#ifndef WIDGETCONFIGSLOT_H
#define WIDGETCONFIGSLOT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>

#include <QSharedPointer>

#include "ioslot.h"
#include "hponic.h"

class WidgetConfigEmptySlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigEmptySlot(QSharedPointer<EmptySlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<EmptySlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;
};

class WidgetConfigAnalogInputSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigAnalogInputSlot(QSharedPointer<AnalogInputSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void numChanged(int num);
    void adc1Changed(int adc1);
    void adc2Changed(int adc2);
    void physical1Changed(double physical1);
    void physical2Changed(double physical2);

    void adcValueChanged();

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<AnalogInputSlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lNum;
    QSpinBox *d_sbNum;

    QLabel *d_lCalibration;
    QLabel *d_lADC;
    QLabel *d_lValue;
    QSpinBox *d_sbADC1;
    QSpinBox *d_sbADC2;
    QLabel *d_lPhysical;
    QDoubleSpinBox *d_dsbPhysical1;
    QDoubleSpinBox *d_dsbPhysical2;
};

class WidgetConfigDiscreteInputSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigDiscreteInputSlot(QSharedPointer<DiscreteInputSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void pinChanged(int pin);
    void inverseChanged(bool inverse);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<DiscreteInputSlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lPin;
    QSpinBox *d_sbPin;

    QCheckBox *d_cbInverse;
};

class WidgetConfigDiscreteOutputSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigDiscreteOutputSlot(QSharedPointer<DiscreteOutputSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void operationChanged(int index);
    void pinChanged(int pin);
    void inverseChanged(bool inverse);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<DiscreteOutputSlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lOperation;
    QComboBox *d_cbOperation;
    QLabel *d_lPin;
    QSpinBox *d_sbPin;

    QCheckBox *d_cbInverse;
};

class WidgetConfigDHTxxSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigDHTxxSlot(QSharedPointer<DHTxxSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void modificationChanged(int index);
    void parameterChanged(int index);
    void pinChanged(int pin);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<DHTxxSlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lModification;
    QComboBox *d_cbModification;

    QLabel *d_lParameter;
    QComboBox *d_cbParameter;

    QLabel *d_lPin;
    QSpinBox *d_sbPin;
};

class WidgetConfigDallasTemperatureSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigDallasTemperatureSlot(QSharedPointer<DallasTemperatureSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void pinChanged(int pin);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<DallasTemperatureSlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lPin;
    QSpinBox *d_sbPin;
};

class WidgetConfigMhZ19Slot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigMhZ19Slot(QSharedPointer<MhZ19Slot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void receivePinChanged(int pin);
    void transmitPinChanged(int pin);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<MhZ19Slot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lReceivePin;
    QSpinBox *d_sbReceivePin;

    QLabel *d_lTransmitPin;
    QSpinBox *d_sbTransmitPin;
};

#endif // WIDGETCONFIGSLOT_H
