#include "videowidget.h"
#include <QPainter>

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent),
    m_bRuning(false)
{

}

void VideoWidget::setRuning(bool run)
{
    m_bRuning = run;
    update();
}

void VideoWidget::paintEvent(QPaintEvent *e)
{
    if (m_bRuning) {
        if (!m_dImg.isNull()) {
            QPainter painter(this);
            painter.drawPixmap(this->rect(), QPixmap::fromImage(m_dImg.scaled(rect().size(),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        }
    }
    QWidget::paintEvent(e);
}

void VideoWidget::onShowImage(QImage img)
{
    m_dImg = img;
    update();
}
