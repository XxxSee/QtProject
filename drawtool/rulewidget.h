#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QWidget>

class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(QWidget *parent = nullptr);
    void updateRule(QList<QPoint> &hpl, QList<QPoint> &vpl);
    void updateScale(qreal scale);
    void updateBaseSize(QSize size);
protected:
    void paintEvent(QPaintEvent *e) override;
    void paintMinorTick(QPainter *painter, bool bHor);
public:
    void slotCurPos(const QPointF &xpos, const QPointF &ypos);
private:
    QList<QPoint> m_dHRuleValues;
    QList<QPoint> m_dVRuleValues;
    QPointF m_dCurXPos;
    QPointF m_dCurYPos;
    qreal m_dCurScale;
    QSize m_dSize;
};

#endif // RULEWIDGET_H
