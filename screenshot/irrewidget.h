#ifndef IRREWIDGET_H
#define IRREWIDGET_H

#include <QWidget>
#include <QPainterPath>

class IrreWidget : public QWidget
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
    explicit IrreWidget(QWidget *parent = nullptr);
signals:
    void grabPolygon(const QPolygon &poly);
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
    QPainterPath m_dPath;
    ResizeType m_dType;
    bool m_bMousePressed;
    QPoint m_dPressPoint;
};

#endif // IRREWIDGET_H
