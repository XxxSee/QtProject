#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include "graphicsscene.h"
#include <QList>
#include <QPoint>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(GraphicsScene *scene, QWidget *parent = nullptr);
    void analyseRule(QList<QPoint>& hpl, QList<QPoint>& vpl);
    void updateTickValue(int value);
signals:
    void sigUpdateRule();
    void sigCurPos(const QPointF &xpos, const QPointF &ypos);
protected:
    void resizeEvent(QResizeEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
private:
    QPoint m_dCurPos;
    int m_dTickValue;
};

#endif // GRAPHICSVIEW_H
