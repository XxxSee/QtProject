#include "canvas.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{

}

void Canvas::test()
{
    mRenderer.addObj(100,100,100);
    mRenderer.addObj(100,0,100);
    mRenderer.addObj(100,0,0);
    mRenderer.addObj(100,100,0);
    mRenderer.addObj(0,100,100);
    mRenderer.addObj(0,0,100);
    mRenderer.addObj(0,0,0);
    mRenderer.addObj(0,100,0);
    update();
}

void Canvas::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());
    painter.restore();
    mRenderer.drawObj(&painter);
    QWidget::paintEvent(e);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) {
        mRenderer.camera().setX(mRenderer.camera().x()+sin(mRenderer.dirY()));
        mRenderer.camera().setZ(mRenderer.camera().z()+cos(mRenderer.dirY()));
        update();
    }
    else if (event->key() == Qt::Key_S) {
        mRenderer.camera().setX(mRenderer.camera().x()-sin(mRenderer.dirY()));
        mRenderer.camera().setZ(mRenderer.camera().z()-cos(mRenderer.dirY()));
        update();
    }
    else if (event->key() == Qt::Key_A) {
        mRenderer.camera().setX(mRenderer.camera().x()+cos(mRenderer.dirY()));
        mRenderer.camera().setZ(mRenderer.camera().z()-sin(mRenderer.dirY()));
        update();
    }
    else if (event->key() == Qt::Key_D) {
        mRenderer.camera().setX(mRenderer.camera().x()-cos(mRenderer.dirY()));
        mRenderer.camera().setZ(mRenderer.camera().z()+sin(mRenderer.dirY()));
        update();
    }
    else if (event->key() == Qt::Key_Q) {
        mRenderer.camera().setY(mRenderer.camera().y()-1);
        update();
    }
    else if (event->key() == Qt::Key_E) {
        mRenderer.camera().setY(mRenderer.camera().y()+1);
        update();
    }
    else if (event->key() == Qt::Key_I) {
        mRenderer.dirY() = mRenderer.dirY() - 0.1;
        mRenderer.camera().setY(mRenderer.camera().y()-mRenderer.dirY());
        update();
    }
    else if (event->key() == Qt::Key_J) {
        mRenderer.dirX() = mRenderer.dirX() - 0.1;
        mRenderer.camera().setX(mRenderer.camera().y()-mRenderer.dirX());
        update();
    }
    else if (event->key() == Qt::Key_K) {
        mRenderer.dirY() = mRenderer.dirY() + 0.1;
        mRenderer.camera().setY(mRenderer.camera().y()-mRenderer.dirY());
        update();
    }
    else if (event->key() == Qt::Key_L) {
        mRenderer.dirX() = mRenderer.dirX() + 0.1;
        mRenderer.camera().setX(mRenderer.camera().y()-mRenderer.dirX());
        update();
    }
    QWidget::keyPressEvent(event);
}
