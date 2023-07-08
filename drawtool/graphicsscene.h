#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QPen>
#include <QBrush>
#include <QUndoStack>
#include <QDomDocument>
#include <QDomElement>
#include "graphicslayer.h"

class GraphicsScene : public QGraphicsScene
{
public:
    explicit GraphicsScene(QObject *parent = nullptr);
    ~GraphicsScene();
    void setBaseMapSize(int w, int h, QPixmap pix = QPixmap());
    void setUndo(bool bUndo);
    void open(const QString &fileName);
    void save(const QString &fileName);
    void saveToPixmap(const QString &fileName);
public slots:
    void slotGraphicsChanged(int id);
    void slotPenAndBrushChanged(const QPen &p, const QBrush &b);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    bool paintGraphics(QGraphicsSceneMouseEvent *event);
    void resetToInit();
    void saveItems(QDomElement &parentEle, QDomDocument &doc);
    void saveItem(GraphicsObject *obj, QDomElement &parentEle, QDomDocument &doc);
    void restoreItem(QDomElement &parentEle);
private:
    QGraphicsPixmapItem *m_pBackgroundItem;
    QUndoStack *m_pUndoStack;
    int m_dDrawType;
    bool m_bDrawPressd;
    QPainterPath m_dDrawPath;
    QGraphicsPathItem *m_pDrawItem;
    QPolygonF m_dDrawPoly;
    QPen m_dPen;
    QBrush m_dBrush;
    LayerManager *m_pLayerManager;
};

#endif // GRAPHICSSCENE_H
