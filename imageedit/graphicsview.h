#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMessageBox>
#include "componentitem.h"

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = nullptr);
    GraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    ~GraphicsView();
    void startExec();
    void clearResult();
    template<ComponentType ct>
    void addItem() {
        if (ct <= ctImage) {
            QMessageBox::warning(this,"warngin","invalid item!");
            return;
        }
        if (mBegin && ct == ctBegin) {
            QMessageBox::warning(this,"warngin","unique begin item already exists!");
            return;
        }
        if (mAdd) {
            delete mAdd;
            mAdd = nullptr;
        }
        mAdd = new ComponentItem<ct>();
        setMouseTracking(true);
    }
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    QGraphicsScene *mScene;
    bool mShift;
    bool mLine;
    BasicItem *mBegin;
    BasicItem *mAdd;
};

#endif // GRAPHICSVIEW_H
