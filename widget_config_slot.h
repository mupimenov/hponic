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

    // 0
    QLabel *d_lStep0;
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

    // 0
    QLabel *d_lStep0;
    QLineEdit *d_leSlotName;

    // 1
    QLabel *d_lStep1;
    QSpinBox *d_sbNum;

    // 2
    QLabel *d_lStep2;
    QLabel *d_lADC;
    QLabel *d_lCurrentAdcValue;
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

    // 0
    QLabel *d_lStep0;
    QLineEdit *d_leSlotName;

    // 1
    QLabel *d_lStep1;
    QLabel *d_lPin;
    QSpinBox *d_sbPin;

    // 2
    QLabel *d_lStep2;
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

    // 0
    QLabel *d_lStep0;
    QLineEdit *d_leSlotName;

    // 1
    QLabel *d_lStep1;
    QComboBox *d_cbOperation;

    // 2
    QLabel *d_lStep2;
    QSpinBox *d_sbPin;

    // 3
    QLabel *d_lStep3;
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

    // 0
    QLabel *d_lStep0;
    QLineEdit *d_leSlotName;

    // 1
    QLabel *d_lStep1;
    QComboBox *d_cbModification;

    // 2
    QLabel *d_lStep2;
    QComboBox *d_cbParameter;

    // 3
    QLabel *d_lStep3;
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

    // 0
    QLabel *d_lStep0;
    QLineEdit *d_leSlotName;

    // 1
    QLabel *d_lStep1;
    QSpinBox *d_sbPin;
};

class WidgetConfigMHZ19Slot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigMHZ19Slot(QSharedPointer<MHZ19Slot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void receivePinChanged(int pin);
    void transmitPinChanged(int pin);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<MHZ19Slot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    // 0
    QLabel *d_lStep0;
    QLineEdit *d_leSlotName;

    // 1
    QLabel *d_lStep1;
    QSpinBox *d_sbReceivePin;

    // 2
    QLabel *d_lStep2;
    QSpinBox *d_sbTransmitPin;
};

class WidgetConfigSHT2xSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetConfigSHT2xSlot(QSharedPointer<SHT2xSlot> ioslot, QSharedPointer<Hponic> hponic, QWidget *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void slotNameChanged(const QString &name);
    void parameterChanged(int index);
    void sdaPinChanged(int pin);
    void sclPinChanged(int pin);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QSharedPointer<SHT2xSlot> d_ioslot;
    QSharedPointer<Hponic> d_hponic;

    QLabel *d_lSlotType;

    // 0
    QLabel *d_lStep0;
    QLineEdit *d_leSlotName;

    // 1
    QLabel *d_lStep1;
    QComboBox *d_cbParameter;

    // 2
    QLabel *d_lStep2;
    QSpinBox *d_sbSdaPin;

    // 3
    QLabel *d_lStep3;
    QSpinBox *d_sbSclPin;
};

#endif // WIDGETCONFIGSLOT_H
