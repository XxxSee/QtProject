#include "customlistdelegate.h"
#include <QStyleOption>
#include <QPen>
#include <QApplication>
#include <QPainter>
#include <QDebug>

CustomListDelegate::CustomListDelegate(int type, QObject *parent) :
    QStyledItemDelegate(parent),
    m_dType((Type)type)
{

}

void CustomListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (m_dType == Pen) {
        Qt::PenStyle penStyle = index.data(Qt::UserRole).value<Qt::PenStyle>();
        //QString penStr = index.data().toString();
        QRect rect = option.rect.marginsRemoved(QMargins(2,2,2,2));
        painter->save();
        if (option.state & QStyle::State_Selected)
            painter->setPen(Qt::red);
        else
            painter->setPen(Qt::lightGray);
        painter->drawRect(rect);
        painter->setPen(QPen(Qt::black,3,penStyle));
        painter->drawLine(QLine(4,rect.top()+rect.height()/2,rect.width()-1,rect.top()+rect.height()/2));
        painter->restore();
    }
    else if (m_dType == Brush) {
        Qt::BrushStyle brushStyle = index.data(Qt::UserRole).value<Qt::BrushStyle>();
        //QString brushStr = index.data().toString();
        QRect rect = option.rect.marginsRemoved(QMargins(2,2,2,2));
        painter->save();
        if (option.state & QStyle::State_Selected)
            painter->setPen(Qt::red);
        else
            painter->setPen(Qt::lightGray);
        painter->setBrush(QBrush(Qt::black,brushStyle));
        painter->drawRect(rect);
        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter,option,index);
}
