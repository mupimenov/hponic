#ifndef MONITORINGDELEGATE_H
#define MONITORINGDELEGATE_H

#include <QStyledItemDelegate>

class MonitoringDelegate : public QStyledItemDelegate
{
public:
    MonitoringDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // MONITORINGDELEGATE_H
