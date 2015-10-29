#ifndef TABLEITEMDELEGATE_H
#define TABLEITEMDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class TableItemDelegate : public QStyledItemDelegate
{
    public:
        TableItemDelegate(QObject * parent = 0);

        void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // TABLEITEMDELEGATE_H
