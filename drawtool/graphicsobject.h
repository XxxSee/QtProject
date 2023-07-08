#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include "graphicsbox.h"

class GraphicsObject : public QGraphicsItem
{
public:
    enum GraphicsType {
        GT_Arrow,GT_Edit,GT_Line,GT_Curve,GT_Rect,GT_RoundedRect,
        GT_Ellipse,GT_Circle,GT_Polygon,GT_Pixmap,GT_Text,
    };
    GraphicsObject(QSizeF size, GraphicsType type, QGraphicsItem *parent = nullptr);
    virtual ~GraphicsObject();
    GraphicsType graphicsType() const;
    void setGraphicsLayer(const QString &name);
    QString graphicsLayer() const;
    void setGraphicsId(const QString &id);
    QString graphicsId() const;
    void setEidtEnable(bool bEdit);
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

    void setSize(QSizeF size);
    void setBoxVisible(bool bShow);
private:
    GraphicsType m_dType;
    QSizeF m_dSize;
    GraphicsBox *m_pBox;
    bool m_bBoxCanShow;
    bool m_bEditEnable;
    QString m_dLayerName;
    QPointF m_dRecordPos;
    QString m_dGraphicsId;
};

#endif // GRAPHICSOBJECT_H
