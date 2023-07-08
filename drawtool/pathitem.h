#ifndef PATHITEM_H
#define PATHITEM_H

#include "graphicsobject.h"
#include <QPainterPath>

class PathItem : public GraphicsObject
{
public:
    PathItem(QPainterPath path, GraphicsType type, QGraphicsItem *parent = nullptr);
    QPainterPath path() const;
    void setPen(const QPen &pen);
    QPen pen() const;
    void setBrush(const QBrush &brush);
    QBrush brush() const;
protected:
    //QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QPainterPath m_dPath;
    QRectF m_dRect;
    QPen m_dPen;
    QBrush m_dBrush;
};

#endif // PATHITEM_H
