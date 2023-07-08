#include "textitem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "drawglobal.h"

TextItem::TextItem(QPointF pos, QString text, QGraphicsItem *parent) :
    GraphicsObject(QSizeF(100,40), GT_Text, parent),
    m_pText(new QGraphicsTextItem(text,this))
{
    setPos(pos);
    setFiltersChildEvents(true);
    //���ô�ʱ�޷��༭,˫��ʱ����༭״̬
    m_pText->setTextInteractionFlags(Qt::NoTextInteraction);
    //QGraphicsTextItem�Ǵ�(0,0)�㿪ʼ�����ı��ģ���ʱ(0,0)���������м䣬������Ҫ�����Ͻ�ƽ�Ƴ����һ��ľ���
    m_pText->setPos(QPointF(-m_pText->boundingRect().width()/2., -m_pText->boundingRect().height()/2.));
}

void TextItem::setText(const QString &text)
{
    m_pText->setPlainText(text);
    setSize(m_pText->boundingRect().size());
    //�ָ����м�λ��
    m_pText->setPos(QPointF(-m_pText->boundingRect().width() / 2, -m_pText->boundingRect().height() / 2));
}

QString TextItem::text() const
{
    return m_pText->toPlainText();
}

void TextItem::setHtml(const QString &html)
{
    m_pText->setHtml(html);
    setSize(m_pText->boundingRect().size());
    //�ָ����м�λ��
    m_pText->setPos(QPointF(-m_pText->boundingRect().width() / 2, -m_pText->boundingRect().height() / 2));
}

QString TextItem::html() const
{
    return m_pText->toHtml();
}

void TextItem::setFont(QFont font)
{
    m_pText->setFont(font);
    setSize(m_pText->boundingRect().size());
    //�ָ����м�λ��
    m_pText->setPos(QPointF(-m_pText->boundingRect().width() / 2, -m_pText->boundingRect().height() / 2));
}

QFont TextItem::font() const
{
    return m_pText->font();
}

QPainterPath TextItem::shape() const
{
    QRectF textRect = m_pText->boundingRect().marginsAdded(QMarginsF(10,10,10,10));;
    QPainterPath path;
    path.addRect(QRectF(-textRect.width()/2.,-textRect.height()/2.,textRect.width(),textRect.height()));
    return path;
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GraphicsObject::paint(painter,option,widget);
}

void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    //˫������༭״̬
    if (m_pText) {
        m_pText->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_pText->setFocus();
        m_dRecordText = m_pText->toPlainText();
    }
    GraphicsObject::mouseDoubleClickEvent(event);
}

bool TextItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (watched == m_pText) {
        if (event->type() == QEvent::FocusIn) {
            setBoxVisible(false);
        }
        else if (event->type() == QEvent::FocusOut) {
            setBoxVisible(true);
            //���»��������С
            setSize(m_pText->boundingRect().size());
            //�ָ����м�λ��
            m_pText->setPos(QPointF(-m_pText->boundingRect().width() / 2, -m_pText->boundingRect().height() / 2));
            m_pText->setTextInteractionFlags(Qt::NoTextInteraction);
            emit DrawGlobal::ins()->sigItemChanged(this,DrawGlobal::ICT_Text,m_dRecordText,m_pText->toPlainText());
        }
    }
    return GraphicsObject::sceneEventFilter(watched, event);
}
