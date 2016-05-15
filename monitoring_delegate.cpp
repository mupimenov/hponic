#include "monitoring_delegate.h"

#include <QPainter>
#include <QFontMetrics>

#include "monitoring.h"

MonitoringDelegate::MonitoringDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void MonitoringDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    QVariant variant = index.model()->data(index, Qt::DisplayRole);
    if (variant.canConvert<IoslotValue>()) {
        IoslotValue ioslotValue = variant.value<IoslotValue>();

        switch (index.column()) {
        case 0:
            painter->drawText(option.rect, Qt::AlignLeft, ioslotValue.ioslot->name());
            break;
        case 1:
        {
            QString str;
            if (ioslotValue.ioslot->type() == AnalogInputType)
                str = QString::number(ioslotValue.value.toDouble(), 'f', 2);
            else if (ioslotValue.ioslot->type() == DiscreteInputType)
                str = ioslotValue.value.toUInt() != 0? tr("ON"): tr("off");
            else if (ioslotValue.ioslot->type() == DiscreteOutputType) {
                str = ioslotValue.value.toUInt() != 0? tr("ON"): tr("off");

                if (ioslotValue.value.toUInt() != 0) {
                    painter->save();
                    painter->setPen(QPen(Qt::NoPen));
                    painter->setBrush(QColor("#c5ffa0"));
                    painter->drawRect(option.rect);
                    painter->restore();
                }
            } else
                str = tr("-");
            painter->drawText(option.rect, Qt::AlignLeft, str);
            break;
        }
        default:
            break;
        }
    }
}

QSize MonitoringDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant variant = index.model()->data(index, Qt::DisplayRole);
    if (variant.canConvert<IoslotValue>()) {
        IoslotValue ioslotValue = variant.value<IoslotValue>();
        switch (index.column()) {
        case 0:
            return QSize(option.fontMetrics.width(ioslotValue.ioslot->name()), option.fontMetrics.height());
        case 1:
            return QSize(option.fontMetrics.width("Some text"), option.fontMetrics.height());
        default:
            return QSize();
        }
    }

    return QSize();
}

