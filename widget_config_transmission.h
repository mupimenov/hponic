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
    explicit WidgetConfigTransmission(QSharedPointer<Hponic> __hponic, QWidget *parent = 0);
    ~WidgetConfigTransmission();

private Q_SLOTS:
    void onTransmissionStatusChanged(Transmission::Status __status);
    void refreshPorts();
    void onPortChanged(const QString &__port);
    void onAddressChanged(int __address);
    void startStopTransmission();

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    Ui::WidgetConfigTransmission *ui;

    QSharedPointer<Hponic> d_hponic;
};

#endif // WIDGET_CONFIG_TRANSMISSION_H
