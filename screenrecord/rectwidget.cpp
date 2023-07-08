#include "rectwidget.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QDebug>

RectWidget::RectWidget(QWidget *parent) :
    QWidget(parent),
    m_bCanChange(true),
    m_bPressed(false)
{
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_QuitOnClose,false);
    setMouseTracking(true);
    m_dScreenRect = QApplication::primaryScreen()->geometry();
    m_dRect = m_dScreenRect.marginsRemoved(QMargins(100,100,100,100));
}

void RectWidget::setCanChange(bool can)
{
    m_bCanChange = can;
    if (m_bCanChange) {
        setMouseTracking(true);
    }
    else {
        setMouseTracking(false);
    }
}

QRect RectWidget::recordRect()
{
    return m_dRect.marginsRemoved(QMargins(3,3,3,3));
}

void RectWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::red,6));
    painter.drawRect(m_dRect);
    QWidget::paintEvent(event);
}

void RectWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_bCanChange) {
        handleMouseEvent(event);
    }
    else
        QWidget::mousePressEvent(event);
}

void RectWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_bCanChange) {
        handleMouseEvent(event);
    }
    else
        QWidget::mouseReleaseEvent(event);
}

void RectWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bCanChange) {
        handleMouseEvent(event);
    }
    else
        QWidget::mouseMoveEvent(event);
}

void RectWidget::handleMouseEvent(QMouseEvent *event)
{
    static bool bMove = true;
    QRect resizeRect = QRect(m_dRect.bottomRight().x(),m_dRect.bottomRight().y(),1,1).marginsAdded(QMargins(10,10,10,10));
    if (event->type() == QEvent::MouseButtonPress) {
        m_bPressed = true;
        m_dPressPoint =  event->pos();
        if (resizeRect.contains(event->pos())) {
            bMove = false;
        }
        else {
            bMove = true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        m_bPressed = false;
    }
    else if (event->type() == QEvent::MouseMove) {
        if (m_bPressed) {
            if (!bMove) {
                QPoint p = m_dRect.bottomRight() + event->pos() - m_dPressPoint;
                m_dRect.setBottomRight(p);
                update();
            }
            else {
                QPoint p = m_dRect.topLeft() + event->pos() - m_dPressPoint;
                if (p.x() > m_dScreenRect.left() && p.x() < (m_dScreenRect.right()-m_dRect.width()) &&
                    p.y() > m_dScreenRect.top() && p.y() < (m_dScreenRect.bottom()-m_dRect.height())) {
                    m_dRect.moveTo(p);
                    update();
                }
            }
            m_dPressPoint = event->pos();
        }
        else {
            if (resizeRect.contains(event->pos())) {
                setCursor(Qt::SizeFDiagCursor);
            }
            else {
                setCursor(Qt::SizeAllCursor);
            }
        }
    }
}
