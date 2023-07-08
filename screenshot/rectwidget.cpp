#include "rectwidget.h"
#include <QPalette>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

RectWidget::RectWidget(QWidget *parent)
    : QWidget(parent)
    , m_dType(RT_None)
    , m_bMousePressed(false)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
//    setWindowOpacity(0.1);
//    QPalette pal = palette();
//    pal.setBrush(QPalette::Background,Qt::lightGray);
//    setPalette(pal);
//    setAutoFillBackground(true);
}

void RectWidget::paintEvent(QPaintEvent *e)
{
    if (m_dRect.isValid()) {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::blue,2));
        painter.drawRect(m_dRect);
        paintAssist(&painter);
    }
    QPainter painter(this);
    painter.setOpacity(0.1);
    painter.setBrush(QColor(255, 255, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
    QWidget::paintEvent(e);
}

void RectWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        close();
    }
    QWidget::keyPressEvent(e);
}

void RectWidget::mousePressEvent(QMouseEvent *e)
{
    m_bMousePressed = true;
    m_dPressPoint = e->pos();
    QWidget::mousePressEvent(e);
}

void RectWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_bMousePressed = false;
    QWidget::mouseReleaseEvent(e);
}

void RectWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_bMousePressed) {
        handleResize(e);
    }
    else {
        handleHover(e);
    }
    QWidget::mouseMoveEvent(e);
}

void RectWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_dRect.contains(e->pos())) {
        emit grabRect(m_dRect);
        close();
    }
    QWidget::mouseDoubleClickEvent(e);
}

void RectWidget::paintAssist(QPainter *painter)
{
    QRect leftRect(m_dRect.left()-2,m_dRect.top()+m_dRect.height()/2,4,4);
    QRect topRect(m_dRect.left()+m_dRect.width()/2,m_dRect.top()-2,4,4);
    QRect rightRect(m_dRect.right()-1,m_dRect.top()+m_dRect.height()/2,4,4);
    QRect bottomRect(m_dRect.left()+m_dRect.width()/2,m_dRect.bottom()-1,4,4);
    painter->drawRect(leftRect);
    painter->drawRect(topRect);
    painter->drawRect(rightRect);
    painter->drawRect(bottomRect);
}


void RectWidget::handleHover(QMouseEvent *e)
{
    QRect leftRect(m_dRect.left()-2,m_dRect.top()+m_dRect.height()/2,4,4);
    QRect topRect(m_dRect.left()+m_dRect.width()/2,m_dRect.top()-2,4,4);
    QRect rightRect(m_dRect.right()-1,m_dRect.top()+m_dRect.height()/2,4,4);
    QRect bottomRect(m_dRect.left()+m_dRect.width()/2,m_dRect.bottom()-1,4,4);
    if (leftRect.contains(e->pos())) {
        setCursor(Qt::SizeHorCursor);
        m_dType = RT_Left;
    }
    else if (topRect.contains(e->pos())) {
        setCursor(Qt::SizeVerCursor);
        m_dType = RT_Top;
    }
    else if (rightRect.contains(e->pos())) {
        setCursor(Qt::SizeHorCursor);
        m_dType = RT_Right;
    }
    else if (bottomRect.contains(e->pos())) {
        setCursor(Qt::SizeVerCursor);
        m_dType = RT_Bottom;
    }
    else if (m_dRect.contains(e->pos())) {
        setCursor(Qt::SizeAllCursor);
        m_dType = RT_Move;
    }
    else {
        setCursor(Qt::ArrowCursor);
        m_dType = RT_None;
    }
}

void RectWidget::handleResize(QMouseEvent *e)
{
    if (m_dType == RT_None) {
       m_dRect = QRect(m_dPressPoint, e->pos());
       m_dRect = m_dRect.normalized();
    }
    else if (m_dType == RT_Move) {
        QPoint p = m_dRect.topLeft() + e->pos() - m_dPressPoint;
        m_dRect.moveTop(p.y());
        m_dRect.moveLeft(p.x());
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Left) {
        m_dRect.setLeft(m_dRect.left() + e->pos().x() - m_dPressPoint.x());
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Top) {
        m_dRect.setTop(m_dRect.top() + e->pos().y() - m_dPressPoint.y());
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Right) {
        m_dRect.setRight(m_dRect.right() + e->pos().x() - m_dPressPoint.x());
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Bottom) {
        m_dRect.setBottom(m_dRect.bottom() + e->pos().y() - m_dPressPoint.y());
        m_dPressPoint = e->pos();
    }
    update();
}
