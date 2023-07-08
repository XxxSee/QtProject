#include "pixmapitem.h"
#include <QPainter>
PixmapItem::PixmapItem(QPointF pos, const QPixmap &pix, QGraphicsItem *parent) :
    GraphicsObject(pix.size(), GT_Pixmap, parent),
    m_dPixmap(pix)
{
    setPos(pos);
}

void PixmapItem::setPixmap(const QPixmap &pix)
{
    m_dPixmap = pix;
    setSize(pix.size());
    update();
}

QPixmap PixmapItem::pixmap() const
{
    return m_dPixmap;
}

void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF pixRect = boundingRect().marginsRemoved(QMarginsF(10,10,10,10));
    painter->save();
    //从(0,0)点开始绘制，此时需要平移到左上角
    //painter->translate(-pixRect.width()/2.,-pixRect.height()/2.);
    //painter->drawPixmap(0,0,m_dPixmap);
    painter->drawPixmap(pixRect.toRect(),m_dPixmap);
    painter->restore();
    GraphicsObject::paint(painter,option,widget);
}
