#include "StatusColorDelegate.h"


// StatusColorDelegate.cpp

StatusColorDelegate::StatusColorDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {
    // 可在此添加初始化代码（如有需要）
}

void StatusColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString status = index.data().toString();

    if (status == "approved") {
        painter->fillRect(option.rect, QColor("#d4edda"));
    } else if (status == "rejected") {
        painter->fillRect(option.rect, QColor("#f8d7da"));
    }

    QStyledItemDelegate::paint(painter, option, index);
}
