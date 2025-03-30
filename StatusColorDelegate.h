#ifndef STATUSCOLORDELEGATE_H
#define STATUSCOLORDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class StatusColorDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit StatusColorDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // STATUSCOLORDELEGATE_H
