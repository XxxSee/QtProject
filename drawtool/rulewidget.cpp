#include "rulewidget.h"
#include <QPainter>
#include <QLine>
RuleWidget::RuleWidget(QWidget *parent) :
    QWidget(parent),
    m_dCurScale(1)
{

}

void RuleWidget::updateRule(QList<QPoint> &hpl, QList<QPoint> &vpl)
{
    m_dHRuleValues = hpl;
    m_dVRuleValues = vpl;
    update();
}

void RuleWidget::updateScale(qreal scale)
{
    m_dCurScale = scale;
    update();
}

void RuleWidget::updateBaseSize(QSize size)
{
    m_dSize = size;
    update();
}

void RuleWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    painter.setPen(QPen(Qt::black,1));
    //Paint H Max Tick Lines
    if (!m_dHRuleValues.isEmpty()) {
        QVector<QLine> lines;
        int lineY1 = rect().top() + 25;
        int lineY2 = rect().top() + 40;
        for (QPoint p : m_dHRuleValues) {
            lines << QLine(p.x(), lineY1, p.x(), lineY2);
            painter.drawText(QPoint(p.x()+2, lineY1 + 5), QString::number(p.y()));
        }
        painter.drawLines(lines);
        //Paint Min Tick Lines
        paintMinorTick(&painter, true);
    }
    //Paint V Max Tick Lines
    if (!m_dVRuleValues.isEmpty()) {
        QVector<QLine> lines;
        int lineX1 = rect().left() + 25;
        int lineX2 = rect().left() + 40;
        for (QPoint p : m_dVRuleValues) {
            lines << QLine(lineX1, p.y(), lineX2, p.y());
            painter.save();
            painter.translate(QPoint(lineX1 + 5, p.y()-2));
            painter.rotate(-90);
            painter.translate(QPoint(-lineX1 - 5, -p.y()+2));
            painter.drawText(QPoint(lineX1 + 5, p.y()-2), QString::number(p.x()));
            painter.restore();
        }
        painter.drawLines(lines);
        //Paint Min Tick Lines
        paintMinorTick(&painter, false);
    }
    //Paint Cur Pos Line
    painter.save();
    painter.setPen(Qt::red);
    QVector<QLine> lines;
    lines << QLine(m_dCurXPos.y(), rect().top() + 25, m_dCurXPos.y(), rect().top() + 40);
    lines << QLine(rect().left() + 25, m_dCurYPos.x(), rect().left() + 40, m_dCurYPos.x());
    painter.drawLines(lines);
    painter.restore();
    //Paint Cur Info Text
    QString infoStr = QStringLiteral("当前坐标:%1,%2 | 当前缩放:%3 | 当前尺寸:%4*%5")
            .arg(m_dCurXPos.x()).arg(m_dCurYPos.y()).arg(m_dCurScale).arg(m_dSize.width()).arg(m_dSize.height());
    painter.drawText(QPoint(rect().left(),rect().bottom()),infoStr);
    QWidget::paintEvent(e);
}

void RuleWidget::paintMinorTick(QPainter *painter, bool bHor)
{
    if (m_dHRuleValues.size() < 2 || m_dVRuleValues.size() < 2)
        return;
    if (bHor) {
        int lineY1 = rect().top() + 35;
        int lineY2 = rect().top() + 40;
        qreal xStep = (m_dHRuleValues.at(1).x() - m_dHRuleValues.at(0).x())/10.;
        if (m_dHRuleValues.at(0).x() > 40) {
            QVector<QLineF> lines;
            for (qreal x=m_dHRuleValues.at(0).x()-xStep; x>=40; x-=xStep) {
                lines << QLineF(x, lineY1, x, lineY2);
            }
            painter->drawLines(lines);
        }
        for (int i=0; i<m_dHRuleValues.size(); i++) {
            QVector<QLineF> lines;
            for (qreal x=m_dHRuleValues.at(i).x()+xStep; x<m_dHRuleValues.at(i).x()+xStep*10.; x+=xStep) {
                lines << QLineF(x, lineY1, x, lineY2);
            }
            painter->drawLines(lines);
        }
    }
    else {
        int lineX1 = rect().left() + 35;
        int lineX2 = rect().left() + 40;
        qreal yStep = (m_dVRuleValues.at(1).y() - m_dVRuleValues.at(0).y())/10.;
        if (m_dVRuleValues.at(0).x() > 40) {
            QVector<QLineF> lines;
            for (qreal y=m_dVRuleValues.at(0).y()-yStep; y>=40; y-=yStep) {
                lines << QLineF(lineX1, y, lineX2, y);
            }
            painter->drawLines(lines);
        }
        for (int i=0; i<m_dVRuleValues.size(); i++) {
            QVector<QLineF> lines;
            for (qreal y=m_dVRuleValues.at(i).y()+yStep; y<m_dVRuleValues.at(i).y()+yStep*10. && y<rect().bottom()-20; y+=yStep) {
                lines << QLineF(lineX1, y, lineX2, y);
            }
            painter->drawLines(lines);
        }
    }
}

void RuleWidget::slotCurPos(const QPointF &xpos, const QPointF &ypos)
{
    m_dCurXPos = xpos;
    m_dCurYPos = ypos;
    update();
}
