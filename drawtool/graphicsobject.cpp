#include "graphicsobject.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneWheelEvent>
#include <QDebug>
#include "drawglobal.h"
#include <QGraphicsScene>

GraphicsObject::GraphicsObject(QSizeF size, GraphicsType type, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    m_dType(type),
    m_dSize(size+QSize(20,20)),
    m_bBoxCanShow(true),
    m_bEditEnable(true),
    m_dGraphicsId("")
{
    m_pBox = new GraphicsBox(m_dSize, this);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

GraphicsObject::~GraphicsObject()
{
    if (scene())
        scene()->removeItem(this);
}

GraphicsObject::GraphicsType GraphicsObject::graphicsType() const
{
    return m_dType;
}

void GraphicsObject::setGraphicsLayer(const QString &name)
{
    m_dLayerName = name;
}

QString GraphicsObject::graphicsLayer() const
{
    return m_dLayerName;
}

void GraphicsObject::setGraphicsId(const QString &id)
{
    m_dGraphicsId = id;
}

QString GraphicsObject::graphicsId() const
{
    return m_dGraphicsId;
}

QRectF GraphicsObject::boundingRect() const
{
    return QRectF(-m_dSize.width()/2., -m_dSize.height()/2., m_dSize.width(), m_dSize.height());
}

void GraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(widget)
    if (option->state & QStyle::State_Selected) {
        if (m_bBoxCanShow && m_pBox && !m_pBox->isVisible()) {
            m_pBox->setVisible(true);
        }
        else if (!m_bBoxCanShow && m_pBox && m_pBox->isVisible()) {
            m_pBox->setVisible(false);
        }
    }
    else {
        if (m_pBox && m_pBox->isVisible()) {
            m_pBox->setVisible(false);
        }
    }
}

void GraphicsObject::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (isSelected()) {
        if (event->delta() > 0) {
            QTransform trans = transform();
            trans.scale(1.1,1.1);
            setTransform(trans);
        }
        else {
            QTransform trans = transform();
            trans.scale(1/1.1,1/1.1);
            setTransform(trans);
        }
    }
    else
        QGraphicsItem::wheelEvent(event);
}

void GraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_dRecordPos = pos();
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dRecordPos != pos()) {
        emit DrawGlobal::ins()->sigItemChanged(this,DrawGlobal::ICT_Move,m_dRecordPos,pos());
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant GraphicsObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        emit DrawGlobal::ins()->sigUpdateCurrentItem(this,isSelected());
    }
    return QGraphicsItem::itemChange(change,value);
}

void GraphicsObject::setSize(QSizeF size)
{
    m_dSize = size+QSizeF(20,20);
    m_pBox->setSize(m_dSize);
    update();
}

void GraphicsObject::setBoxVisible(bool bShow)
{
    m_bBoxCanShow = bShow;
    update();
}

void GraphicsObject::setEidtEnable(bool bEdit)
{
    if (m_bEditEnable != bEdit) {
        m_bEditEnable = bEdit;
        if (m_bEditEnable) {
            setFlags(flags() | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        }
        else {
            setFlags(flags() & ~QGraphicsItem::ItemIsMovable & ~QGraphicsItem::ItemIsSelectable);
        }
    }
}
