#ifndef GRAPHICSBOX_H
#define GRAPHICSBOX_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QList>

class GraphicsBox : public QGraphicsItem
{
public:
    GraphicsBox(QSizeF size, QGraphicsItem *parent = nullptr);
    void setSize(QSizeF size);
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    void initTransformItems();
private:
    QSizeF m_dSize;
};

#endif // GRAPHICSBOX_H
