#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "graphicsobject.h"
#include <QGraphicsTextItem>
#include <QFont>

class TextItem : public GraphicsObject
{
public:
    TextItem(QPointF pos, QString text, QGraphicsItem *parent = nullptr);
    void setText(const QString &text);
    QString text() const;
    void setHtml(const QString &html);
    QString html() const;
    void setFont(QFont font);
    QFont font() const;
protected:
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) override;
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
private:
    QGraphicsTextItem *m_pText;
    QString m_dRecordText;
};

#endif // TEXTITEM_H
