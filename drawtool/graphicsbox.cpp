#include "graphicsbox.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>
#include <QPen>
#include <QtMath>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include "drawglobal.h"

static qreal calcPoint2PointAngle(QPointF p1, QPointF p2, QPointF center)
{
    qreal x1 = p1.x(), y1 = p1.y(), x2 = p2.x(), y2 = p2.y(), x3 = center.x(), y3 = center.y();
    qreal theta = atan2(x1 - x3, y1 - y3) - atan2(x2 - x3, y2 - y3);
    if (theta > M_PI)
        theta -= 2 * M_PI;
    if (theta < -M_PI)
        theta += 2 * M_PI;
    theta = theta * 180.0 / M_PI;
    return theta;
}

class TransformItem : public QGraphicsRectItem
{
public:
    enum PointType {
        PT_None,PT_TopLeft,PT_Top,PT_TopRight,PT_Left,
        PT_Right,PT_BottomLeft,PT_Bottom,PT_BottomRight
    };
    explicit TransformItem(QGraphicsItem *parent, QGraphicsItem *transform, PointType t = PT_None, bool bScale = true);
    void updatePos(QRectF rect);
    void setModel(bool bScale);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QGraphicsItem *m_pTransform;
    PointType m_dType;
    bool m_bScaled;
    bool m_bPressed;
    QPointF m_dPoint;
    qreal m_dRecord;
};

/*************************TransformItem********************************/
TransformItem::TransformItem(QGraphicsItem *parent, QGraphicsItem *transform, PointType t, bool bScale) :
    QGraphicsRectItem(parent),
    m_pTransform(transform),
    m_dType(t),
    m_bScaled(bScale),
    m_bPressed(false)
{
    Q_ASSERT(parent && transform);
    setPen(QPen(QColor(202, 81, 0), 1));
    setBrush(QColor(255, 242, 157));
    setAcceptHoverEvents(true);
    //���Ը�item��λ�á����š���ת�ȱ任
    setFlags(QGraphicsItem::ItemIgnoresTransformations);
    setRect(-3,-3,6,6);
}

void TransformItem::updatePos(QRectF rect)
{
    QPointF itemPos;
    switch (m_dType) {
    case PT_TopLeft:
        itemPos = rect.topLeft();
        break;
    case PT_Top:
        itemPos = QPointF(rect.center().x(),rect.top());
        break;
    case PT_TopRight:
        itemPos = rect.topRight();
        break;
    case PT_Left:
        itemPos = QPointF(rect.left(),rect.center().y());
        break;
    case PT_Right:
        itemPos = QPointF(rect.right(),rect.center().y());
        break;
    case PT_BottomLeft:
        itemPos = rect.bottomLeft();
        break;
    case PT_Bottom:
        itemPos = QPointF(rect.center().x(),rect.bottom());
        break;
    case PT_BottomRight:
        itemPos = rect.bottomRight();
        break;
    default:break;
    }
    setPos(itemPos);
}

void TransformItem::setModel(bool bScale)
{
    m_bScaled = bScale;
}

void TransformItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_bScaled) {
        setCursor(Qt::CrossCursor);
    }
    else {
        setCursor(Qt::PointingHandCursor);
    }
    QGraphicsRectItem::hoverEnterEvent(event);
}

void TransformItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void TransformItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_bPressed = true;
    if (m_bScaled) {
        m_dRecord = m_pTransform->scale();
    }
    else {
        m_dRecord = m_pTransform->rotation();
    }
    m_dPoint = event->scenePos();
}

void TransformItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bPressed) {
        QPointF centerPos = m_pTransform->mapToScene(m_pTransform->boundingRect().center());
        QPointF curPos = event->scenePos();
        if (m_bScaled) {
            qreal src_dis = qSqrt(qPow(centerPos.x()-m_dPoint.x(),2)+qPow(centerPos.y()-m_dPoint.y(),2));
            qreal cur_dis = qSqrt(qPow(centerPos.x()-curPos.x(),2)+qPow(centerPos.y()-curPos.y(),2));
            //������괦�˴������ű���
            qreal dScale = cur_dis * 1. / src_dis;
            //ʹ��setScale��ʽ
            m_pTransform->setScale(dScale);
            //������������˴���ʹ��setTransform��ʽ
            //ʹ��setTransform��ʽ
            //QTransform trans = m_pTransform->transform();
            //if (m_dType == PT_TopRight || m_dType == PT_TopLeft || m_dType == PT_BottomRight || m_dType == PT_BottomLeft) {
            //    trans.scale(dScale,dScale);
            //}
            //
            //else if (m_dType == PT_Top || m_dType == PT_Bottom) {
            //    //ˮƽ���򱣳ֲ���
            //    trans.scale(1,dScale);
            //}
            //
            //else if (m_dType == PT_Left || m_dType == PT_Right) {
            //    //��ֱ���򱣳ֲ���
            //    trans.scale(dScale,1);
            //}
            //m_pTransform->setTransform(trans);
        }
        else {
            qreal ang = calcPoint2PointAngle(m_dPoint,curPos,centerPos);
            //setRotation��ʽ���ö��پ��Ƕ��٣�������Ҫ�ۼӵ�ǰ��ת����
            ang = ang + m_pTransform->rotation();
            ang = ang > 360 ? ang - 360 : ang < -360 ? ang + 360 : ang;
            m_pTransform->setRotation(ang);
            //transform��ʽ
            //��ˮƽ�����봹ֱ���������ϵ����һ��ʱ��ͼ�λᷢ��Ť��
            //QTransform trans = m_pTransform->transform();
            //trans.rotate(ang);
            //m_pTransform->setTransform(trans);
        }
        //setTransform��ʽscale��Ҫ���¼���㣬��Ϊ���۳�
        //rotate���ַ�ʽ����Ҫ���¼����
        if (!m_bScaled)
            m_dPoint = event->scenePos();
    }
}

void TransformItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    if (m_bScaled) {
        if (m_dRecord != m_pTransform->scale()) {
            emit DrawGlobal::ins()->sigItemChanged(m_pTransform,DrawGlobal::ICT_Scale,m_dRecord,m_pTransform->scale());
        }
    }
    else {
        if (m_dRecord != m_pTransform->rotation()) {
            emit DrawGlobal::ins()->sigItemChanged(m_pTransform,DrawGlobal::ICT_Rotate,m_dRecord,m_pTransform->rotation());
        }
    }
    m_bPressed = false;
}

/*************************GraphicsBox********************************/
GraphicsBox::GraphicsBox(QSizeF size, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    m_dSize(size-QSizeF(10,10))
{
    Q_ASSERT(parent);
    initTransformItems();
}

void GraphicsBox::setSize(QSizeF size)
{
    m_dSize = size-QSizeF(10,10);
    QRectF curRect = QRectF(-m_dSize.width()/2.,-m_dSize.height()/2.,m_dSize.width(),m_dSize.height());
    //���¿��Ƶ�����
    foreach (QGraphicsItem *item , childItems()) {
        TransformItem *trans = dynamic_cast<TransformItem*>(item);
        if (trans)
            trans->updatePos(curRect);
    }
    update();
}

QRectF GraphicsBox::boundingRect() const
{
    return QRectF(-m_dSize.width()/2., -m_dSize.height()/2., m_dSize.width(), m_dSize.height()).marginsAdded(QMarginsF(2,2,2,2));
}

void GraphicsBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)
    painter->save();
    QPen pen;
    pen.setColor(QColor(255, 242, 157));
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawRect(QRectF(-m_dSize.width()/2.,-m_dSize.height()/2.,m_dSize.width(),m_dSize.height()));
    painter->restore();
}

void GraphicsBox::initTransformItems()
{
    QRectF curRect = QRectF(-m_dSize.width()/2.,-m_dSize.height()/2.,m_dSize.width(),m_dSize.height());
    TransformItem *item = new TransformItem(this,parentItem(),TransformItem::PT_TopLeft);
    item->updatePos(curRect);
    item = new TransformItem(this,parentItem(),TransformItem::PT_Top,false);
    item->updatePos(curRect);
    item = new TransformItem(this,parentItem(),TransformItem::PT_TopRight);
    item->updatePos(curRect);
    item = new TransformItem(this,parentItem(),TransformItem::PT_Left,false);
    item->updatePos(curRect);
    item = new TransformItem(this,parentItem(),TransformItem::PT_Right,false);
    item->updatePos(curRect);
    item = new TransformItem(this,parentItem(),TransformItem::PT_BottomLeft);
    item->updatePos(curRect);
    item = new TransformItem(this,parentItem(),TransformItem::PT_Bottom,false);
    item->updatePos(curRect);
    item = new TransformItem(this,parentItem(),TransformItem::PT_BottomRight);
    item->updatePos(curRect);
}
