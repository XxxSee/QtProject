#include "graphicsview.h"
#include <QMouseEvent>
#include <QtMath>
#include <QScrollBar>
#include <QDebug>

GraphicsView::GraphicsView(GraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene,parent),
    m_dCurPos(0,0),
    m_dTickValue(100)
{
    setMouseTracking(true);
    connect(horizontalScrollBar(),&QScrollBar::valueChanged,this,&GraphicsView::sigUpdateRule);
    connect(verticalScrollBar(),&QScrollBar::valueChanged,this,&GraphicsView::sigUpdateRule);
}

void GraphicsView::resizeEvent(QResizeEvent *e)
{
    emit sigUpdateRule();
    QGraphicsView::resizeEvent(e);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QPointF scenePos = mapToScene(e->pos());
    QPoint viewPos = mapFromScene(scenePos);
    QPoint rulePos = mapToParent(viewPos);
    QPointF xp(scenePos.x(), rulePos.x()), yp(rulePos.y(), scenePos.y());
    emit sigCurPos(xp,yp);
    QGraphicsView::mouseMoveEvent(e);
}

void GraphicsView::analyseRule(QList<QPoint>& hpl,  QList<QPoint>& vpl)
{
    QPointF ltPos = mapToScene(rect().topLeft());
    QPointF rtPos = mapToScene(rect().topRight());
    int ltx = qCeil(ltPos.x());
    for (;;++ltx) {
        if (ltx % m_dTickValue == 0)
            break;
    }
    int rtx = qFloor(rtPos.x());
    for (;;--rtx) {
        if (rtx % m_dTickValue == 0)
            break;
    }
    QList<QPoint> hRuleValues;
    for (int x=ltx; x<=rtx; x+=m_dTickValue) {
        QPointF scenePos(x,ltPos.y());
        QPoint viewPos = mapFromScene(scenePos);
        QPoint rulePos = mapToParent(viewPos);
        rulePos.setY(x);
        hRuleValues << rulePos;
    }
    hpl = hRuleValues;

    QPointF lbPos = mapToScene(rect().bottomLeft());
    int lty = qCeil(ltPos.y());
    for (;;++lty) {
        if (lty % m_dTickValue == 0)
            break;
    }
    int lby = qFloor(lbPos.y());
    for (;;--lby) {
        if (lby % m_dTickValue == 0)
            break;
    }
    QList<QPoint> vRuleValues;
    for (int y=lty; y<=lby; y+=m_dTickValue) {
        QPointF scenePos(ltPos.x(),y);
        QPoint viewPos = mapFromScene(scenePos);
        QPoint rulePos = mapToParent(viewPos);
        rulePos.setX(y);
        vRuleValues << rulePos;
    }
    vpl = vRuleValues;
}

void GraphicsView::updateTickValue(int value)
{
    if (m_dTickValue != value) {
        m_dTickValue = value;
        emit sigUpdateRule();
    }
}
