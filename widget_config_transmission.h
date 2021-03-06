#ifndef WIDGET_CONFIG_TRANSMISSION_H
#define WIDGET_CONFIG_TRANSMISSION_H

#include <QWidget>

#include <QSharedPointer>

#include "hponic.h"

namespace Ui {
class WidgetConfigTransmission;
}

class WidgetConfigTransmission : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetConfigTransmission(QSharedPointer<Hponic> hponic, QWidget *parent = 0);
    ~WidgetConfigTransmission();

public Q_SLOTS:
    void setPort(const QString &port);
    void setAddress(quint8 address);

private Q_SLOTS:
    void onTransmissionStatusChanged(Transmission::Status status);
    void onPortChanged(const QString &port);
    void onAddressChanged(int address);
    void onAddressProgramStarted();
    void onAddressProgramFinished(bool success, quint8 newAddress);

    void updatePortList();

    void startStopTransmission();
    void programAddress();

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    Ui::WidgetConfigTransmission *ui;

    QSharedPointer<Hponic> d_hponic;
};

#endif // WIDGET_CONFIG_TRANSMISSION_H
