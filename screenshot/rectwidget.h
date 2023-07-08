#ifndef RECTWIDGET_H
#define RECTWIDGET_H

#include <QWidget>

class RectWidget : public QWidget
{
    Q_OBJECT
    enum ResizeType {
        RT_None,
        RT_Move,
        RT_Left,
        RT_Top,
        RT_Right,
        RT_Bottom,
    };

public:
    explicit RectWidget(QWidget *parent = nullptr);
signals:
    void grabRect(const QRect &rect);
protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
private:
    void paintAssist(QPainter *painter);
    void handleHover(QMouseEvent *e);
    void handleResize(QMouseEvent *e);
private:
    QRect m_dRect;
    ResizeType m_dType;
    bool m_bMousePressed;
    QPoint m_dPressPoint;
};


#endif // RECTWIDGET_H
