#include "tableitemdelegate.h"

#include <QtGui/QPainter>

TableItemDelegate::TableItemDelegate(QObject * parent) : QStyledItemDelegate(parent)
{

}

void TableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);

    if (index.column() == 1 || index.column() == 2) {
        opt.displayAlignment |= Qt::AlignCenter;
    }

    return QStyledItemDelegate::paint(painter, opt, index);
}

