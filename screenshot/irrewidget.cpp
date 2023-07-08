#include "irrewidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

IrreWidget::IrreWidget(QWidget *parent)
    : QWidget(parent)
    , m_dType(RT_None)
    , m_bMousePressed(false)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
}

void IrreWidget::paintEvent(QPaintEvent *e)
{
    if (!m_dPath.isEmpty()) {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing);
        painter.setPen(Qt::black);
        painter.drawPath(m_dPath);
        if (!m_bMousePressed) {
            painter.setPen(QPen(Qt::blue,2));
            painter.drawRect(m_dPath.boundingRect());
            paintAssist(&painter);
        }
    }
    QPainter painter(this);
    painter.setOpacity(0.1);
    painter.setBrush(QColor(255, 255, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
    QWidget::paintEvent(e);
}

void IrreWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        close();
    }
    QWidget::keyPressEvent(e);
}

void IrreWidget::mousePressEvent(QMouseEvent *e)
{
    m_bMousePressed = true;
    m_dPressPoint = e->pos();
    if (m_dType == RT_None) {
        m_dPath = QPainterPath();
        m_dPath.moveTo(e->pos());
    }
    QWidget::mousePressEvent(e);
}

void IrreWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_bMousePressed = false;
    m_dPath.closeSubpath();
    update();
    QWidget::mouseReleaseEvent(e);
}

void IrreWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_bMousePressed) {
        handleResize(e);
    }
    else {
        handleHover(e);
    }
    QWidget::mouseMoveEvent(e);
}

void IrreWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_dPath.boundingRect().contains(e->pos())) {
        emit grabPolygon(m_dPath.toFillPolygon().toPolygon());
        close();
    }
    QWidget::mouseDoubleClickEvent(e);
}

void IrreWidget::paintAssist(QPainter *painter)
{
    QRect rect = m_dPath.boundingRect().toRect();
    QRect leftRect(rect.left()-2,rect.top()+rect.height()/2,4,4);
    QRect topRect(rect.left()+rect.width()/2,rect.top()-2,4,4);
    QRect rightRect(rect.right()-1,rect.top()+rect.height()/2,4,4);
    QRect bottomRect(rect.left()+rect.width()/2,rect.bottom()-1,4,4);
    painter->drawRect(leftRect);
    painter->drawRect(topRect);
    painter->drawRect(rightRect);
    painter->drawRect(bottomRect);
}

void IrreWidget::handleHover(QMouseEvent *e)
{
    QRect rect = m_dPath.boundingRect().toRect();
    QRect leftRect(rect.left()-2,rect.top()+rect.height()/2,4,4);
    QRect topRect(rect.left()+rect.width()/2,rect.top()-2,4,4);
    QRect rightRect(rect.right()-1,rect.top()+rect.height()/2,4,4);
    QRect bottomRect(rect.left()+rect.width()/2,rect.bottom()-1,4,4);
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
    else if (rect.contains(e->pos())) {
        setCursor(Qt::SizeAllCursor);
        m_dType = RT_Move;
    }
    else {
        setCursor(Qt::ArrowCursor);
        m_dType = RT_None;
    }
}

void IrreWidget::handleResize(QMouseEvent *e)
{
    QRect rect = m_dPath.boundingRect().toRect();
    if (m_dType == RT_None) {
       m_dPath.lineTo(e->pos());
    }
    else if (m_dType == RT_Move) {
        QPoint diffP = e->pos() - m_dPressPoint;
        QTransform transform;
        transform.translate(diffP.x(),diffP.y());
        m_dPath = transform.map(m_dPath);
//        QPolygon poly;
//        QList<QPoint> ps = m_dPath.toFillPolygon().toPolygon().toList();
//        for (QPoint curP : ps) {
//            QPoint pos = curP + diffP;
//            poly.append(pos);
//        }
//        QPainterPath path;
//        path.addPolygon(poly);
//        m_dPath = path;
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Left) {
        QPoint diffP = m_dPressPoint - e->pos();
        QTransform transform;
        transform.translate(rect.right(),rect.top()+rect.height()/2);
        qreal scaleX = (diffP.x()+rect.width())*1./rect.width();
        transform.scale(scaleX,1);
        transform.translate(-rect.right(),-rect.top()-rect.height()/2);
        m_dPath = transform.map(m_dPath);
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Top) {
        QPoint diffP = m_dPressPoint - e->pos();
        QTransform transform;
        transform.translate(rect.left()+rect.width()/2,rect.bottom());
        qreal scaleY = (diffP.y()+rect.height())*1./rect.height();
        transform.scale(1,scaleY);
        transform.translate(-rect.left()-rect.width()/2,-rect.bottom());
        m_dPath = transform.map(m_dPath);
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Right) {
        QPoint diffP = e->pos() - m_dPressPoint;
        QTransform transform;
        transform.translate(rect.left(),rect.top()+rect.height()/2);
        qreal scaleX = (diffP.x()+rect.width())*1./rect.width();
        transform.scale(scaleX,1);
        transform.translate(-rect.left(),-rect.top()-rect.height()/2);
        m_dPath = transform.map(m_dPath);
        m_dPressPoint = e->pos();
    }
    else if (m_dType == RT_Bottom) {
        QPoint diffP = e->pos() - m_dPressPoint;
        QTransform transform;
        transform.translate(rect.left()+rect.width()/2,rect.top());
        qreal scaleY = (diffP.y()+rect.height())*1./rect.height();
        transform.scale(1,scaleY);
        transform.translate(-rect.left()-rect.width()/2,-rect.top());
        m_dPath = transform.map(m_dPath);
        m_dPressPoint = e->pos();
    }
    update();
}
