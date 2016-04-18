#ifndef WIDGET_IOSLOTS_H
#define WIDGET_IOSLOTS_H

#include <QWidget>
#include <QSplitter>

#include <QSharedPointer>

#include "hponic.h"

namespace Ui {
class WidgetIOslots;
}

class WidgetIoslots : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetIoslots(QSharedPointer<Hponic> hponic, QWidget *parent = 0);
    ~WidgetIoslots();

private Q_SLOTS:
    void setAnalogInputDriver();
    void setDiscreteInputDriver();
    void setDiscreteOutputDriver();
    void setDHT22TemperatureDriver();
    void setDHT22HumidityDriver();
    void setEmptySlotDriver();

    void onIoslotCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

    void onIoslotsDownloadStarted();
    void onIoslotsDownloadFinished(bool success);

    void onIoslotsUploadStarted();
    void onIoslotsUploadFinished(bool success);

private:
    void createWidgets();
    void createMenu();
    void createLayouts();
    void createConnections();

    void swapWidgetConfigSlot(QWidget *widget);

    Ui::WidgetIOslots *ui;
    QSplitter *d_splitter;

    QSharedPointer<Hponic> d_hponic;
    QWidget *d_widgetConfigSlot;
    QSharedPointer<Ioslot> d_selectedSlot;
};

#endif // WIDGET_IOSLOTS_H
