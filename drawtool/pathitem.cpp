#include "pathitem.h"
#include <QPainter>
#include <QGraphicsPathItem>
#include <QDebug>

PathItem::PathItem(QPainterPath path, GraphicsType type, QGraphicsItem *parent) :
    GraphicsObject(path.boundingRect().size(), type, parent),
    m_dPath(path),
    m_dRect(path.boundingRect().marginsAdded(QMarginsF(10,10,10,10)))
{
    //绘制区域平移了多少，这里再平移过来，回归原位
    setPos(m_dRect.center().x(),m_dRect.center().y());
}

QPainterPath PathItem::path() const
{
    return m_dPath;
}

void PathItem::setPen(const QPen &pen)
{
    m_dPen = pen;
    update();
}

QPen PathItem::pen() const
{
    return m_dPen;
}

void PathItem::setBrush(const QBrush &brush)
{
    m_dBrush = brush;
    update();
}

QBrush PathItem::brush() const
{
    return m_dBrush;
}

////绘制区域
//QRectF PathItem::boundingRect() const
//{
//    //这里相当于把绘制区域平移了(-m_dRect.center().x(),-m_dRect.center().y())
//    //当前位置由(0,0)点->(-m_dRect.center().x(),-m_dRect.center().y())
//    return QRectF(-m_dRect.width()/2.,-m_dRect.height()/2.,m_dRect.width(),m_dRect.height());
//}

//鼠标点击的区域
QPainterPath PathItem::shape() const
{
    QTransform trans;
    //绘制区域平移了，这里需要跟着平移
    trans.translate(-m_dRect.center().x(),-m_dRect.center().y());
    QPainterPath path = trans.map(m_dPath);
    //借用QGraphicsPathItem计算包括画笔宽度的区域
    //否则大于1宽度的直线不容易选中
    QGraphicsPathItem pathItem(path);
    pathItem.setPen(m_dPen);
    return pathItem.shape();
}

inline void PathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    painter->setPen(m_dPen);
    painter->setBrush(m_dBrush);
    //绘制区域平移了，这里需要跟着平移
    painter->translate(-m_dRect.center().x(),-m_dRect.center().y());
    painter->drawPath(m_dPath);
    painter->restore();
    GraphicsObject::paint(painter,option,widget);
}
