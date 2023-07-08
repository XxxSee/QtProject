#ifndef RECTWIDGET_H
#define RECTWIDGET_H

#include <QWidget>

class RectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RectWidget(QWidget *parent = nullptr);
    void setCanChange(bool can);
    QRect recordRect();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void handleMouseEvent(QMouseEvent *event);
private:
    QRect m_dRect;
    QRect m_dScreenRect;
    bool m_bCanChange;
    bool m_bPressed;
    QPoint m_dPressPoint;
};

#endif // RECTWIDGET_H
