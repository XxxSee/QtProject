#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include "graphicsobject.h"
#include <QPixmap>
class PixmapItem : public GraphicsObject
{
public:
    explicit PixmapItem(QPointF pos, const QPixmap &pix = QPixmap(800,500), QGraphicsItem *parent = nullptr);
    void setPixmap(const QPixmap &pix);
    QPixmap pixmap() const;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QPixmap m_dPixmap;
};

#endif // PIXMAPITEM_H
