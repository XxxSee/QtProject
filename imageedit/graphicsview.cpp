#include "graphicsview.h"
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    mShift(false),
    mLine(false),
    mBegin(nullptr),
    mAdd(nullptr)
{
    mScene = new QGraphicsScene(this);
    setScene(mScene);
    BasicItem *item = new ComponentItem<ctBegin>();
    mBegin = item;
    mScene->addItem(item);
}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene,parent)
{

}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::startExec()
{
    if (mBegin) {
        clearResult();
        mBegin->exec();
    }
}

void GraphicsView::clearResult()
{
    QList<QGraphicsItem*> items = mScene->items();
    if (items.size() > 0) {
        for (int i{0}; i<items.size(); i++) {
            BasicItem *item = dynamic_cast<BasicItem*>(items.at(i));
            if (item) {
                item->resetItem();
                if (item->type() == ctImage) {
                    item->removeInItem();
                    item->removeOutItem(nullptr);
                    mScene->removeItem(item);
                }
            }
        }
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (mAdd) {
        if (!mAdd->scene())
            mScene->addItem(mAdd);
        mAdd->setPos(mapToScene(event->pos())+QPointF(50,25));
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    static BasicItem *mOut = nullptr;
    static BasicItem *mIn = nullptr;
    if (mAdd) {
        setMouseTracking(false);
        mAdd = nullptr;
    }
    if (mShift) {
        QList<QGraphicsItem*> items = mScene->items(mapToScene(event->pos()));
        if (items.size() > 0) {
            mOut = dynamic_cast<BasicItem*>(items.at(0));
            if (mOut) {
                if (mOut->type() != ctEnd) {
                    mLine = true;
                    setCursor(Qt::CrossCursor);
                }
            }
        }
    }
    else if (mLine) {
        QList<QGraphicsItem*> items = mScene->items(mapToScene(event->pos()));
        if (items.size() > 0) {
            mIn = dynamic_cast<BasicItem*>(items.at(0));
            if (mIn) {
                if ((mIn->type() != ctBegin)) {
                    mOut->addOutItem(mIn);
                    mIn->addInItem(mOut);
                }
            }
        }
        mLine = false;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        mShift = true;
    }
    else if (event->key() == Qt::Key_Delete) {
        QList<QGraphicsItem*> items = mScene->selectedItems();
        if (items.size() > 0) {
            for (int i{0}; i<items.size(); i++) {
                BasicItem *item = dynamic_cast<BasicItem*>(items.at(i));
                if (item && item->type() != ctLine) {
                    if (item->type() == ctBegin) {
                        mBegin = nullptr;
                    }
                    item->removeInItem();
                    item->removeOutItem(nullptr);
                    mScene->removeItem(item);
                }
            }
        }
        items = mScene->selectedItems();
        if (items.size() > 0) {
            for (int i{0}; i<items.size(); i++) {
                BasicItem *item = dynamic_cast<BasicItem*>(items.at(i));
                if (item && item->type() == ctLine) {
                    item->removeInItem();
                    item->removeOutItem(nullptr);
                    mScene->removeItem(item);
                }
            }
        }
    }
    QGraphicsView::keyPressEvent(event);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        mShift = false;
    }
    QGraphicsView::keyReleaseEvent(event);
}
