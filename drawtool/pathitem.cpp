#include "pathitem.h"
#include <QPainter>
#include <QGraphicsPathItem>
#include <QDebug>

PathItem::PathItem(QPainterPath path, GraphicsType type, QGraphicsItem *parent) :
    GraphicsObject(path.boundingRect().size(), type, parent),
    m_dPath(path),
    m_dRect(path.boundingRect().marginsAdded(QMarginsF(10,10,10,10)))
{
    //��������ƽ���˶��٣�������ƽ�ƹ������ع�ԭλ
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

////��������
//QRectF PathItem::boundingRect() const
//{
//    //�����൱�ڰѻ�������ƽ����(-m_dRect.center().x(),-m_dRect.center().y())
//    //��ǰλ����(0,0)��->(-m_dRect.center().x(),-m_dRect.center().y())
//    return QRectF(-m_dRect.width()/2.,-m_dRect.height()/2.,m_dRect.width(),m_dRect.height());
//}

//�����������
QPainterPath PathItem::shape() const
{
    QTransform trans;
    //��������ƽ���ˣ�������Ҫ����ƽ��
    trans.translate(-m_dRect.center().x(),-m_dRect.center().y());
    QPainterPath path = trans.map(m_dPath);
    //����QGraphicsPathItem����������ʿ�ȵ�����
    //�������1��ȵ�ֱ�߲�����ѡ��
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
    //��������ƽ���ˣ�������Ҫ����ƽ��
    painter->translate(-m_dRect.center().x(),-m_dRect.center().y());
    painter->drawPath(m_dPath);
    painter->restore();
    GraphicsObject::paint(painter,option,widget);
}
