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

class WidgetConfigDHT22TemperatureSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigDHT22TemperatureSlot(QSharedPointer<DHT22TemperatureSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void pinChanged(int pin);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<DHT22TemperatureSlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lPin;
    QSpinBox *d_sbPin;
};

class WidgetConfigDHT22HumiditySlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigDHT22HumiditySlot(QSharedPointer<DHT22HumiditySlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void pinChanged(int pin);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<DHT22HumiditySlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    QLabel *d_lSlotName;
    QLineEdit *d_leSlotName;

    QLabel *d_lPin;
    QSpinBox *d_sbPin;
};

#endif // WIDGETCONFIGSLOT_H
