#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include "renderer.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    void test();
protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    Renderer mRenderer;
};

#endif // CANVAS_H
