#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include <QList>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QStyleOptionGraphicsItem>
#include "component.h"
#include "componentdialog.h"
#include "componentmethod.h"

QImage mat2Image(Mat m);
Mat image2Mat(QImage img);

//item同时具备输入in和输出out两个属性, 在不同的场景可能是作为输入item也可能是作为输出item
class ComponentLine : public BasicItem
{
public:
    ComponentLine(BasicItem *out, BasicItem *in, QGraphicsItem *parent = nullptr);
    QPointF inScenePos() const override;
    QPointF outScenePos() const override;
    void addInItem(BasicItem *item) override;
    void removeInItem() override;
    void addOutItem(BasicItem *item) override;
    void removeOutItem(BasicItem *item) override;
protected:
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    friend class BasicItem;
    BasicItem *mOutItem;
    BasicItem *mInItem;
};

class ImageItem : public BasicItem
{
public:
    explicit ImageItem(Mat t, BasicItem *in, QGraphicsItem *parent = nullptr);
    QPointF inScenePos() const override;
    QPointF outScenePos() const override;
    void addInItem(BasicItem *item) override;
    void removeInItem() override;
    void addOutItem(BasicItem *item) override;
    void removeOutItem(BasicItem *item) override;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:
    friend class BasicItem;
    QImage mShowImg;
    QRectF mRect;
    BasicItem *mInItem;
};

template<ComponentType CT>
class ComponentItem : public BasicItem
{
public:
    explicit ComponentItem(QGraphicsItem *parent = nullptr) : BasicItem(CT,parent)
    {
        mInItem = nullptr;
        mRect = QRectF(-50,-25,100,50);
        mExec = false;
        mText = type2String(CT);
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsScenePositionChanges);
    }
    QPointF inScenePos() const override
    {
        return mapToScene(QPointF(mRect.center().x(),mRect.top()));
    }
    QPointF outScenePos() const override
    {
        return mapToScene(QPointF(mRect.center().x(),mRect.bottom()));
    }
    void addInItem(BasicItem *item) override
    {
        removeInItem();
        mInItem = item;
    }
    void removeInItem() override
    {
        if (mInItem) {
            mInItem->removeOutItem(this);
            mInItem = nullptr;
        }
    }
    void addOutItem(BasicItem *item) override
    {
        if (!mOutItems.contains(item)) {
            mOutItems.append(item);
            ComponentLine *line = new ComponentLine(this,item);
            scene()->addItem(line);
            mOutLines.append(line);
        }
    }
    void removeOutItem(BasicItem *item) override
    {
        if (item) {
            for (int i{0}; i<mOutItems.size(); i++) {
                if (mOutItems[i] == item) {
                    mOutItems.removeAt(i);
                    scene()->removeItem(mOutLines.at(i));
                    mOutLines.removeAt(i);
                    break;
                }
            }
        }
        else {
            for (int i{0}; mOutItems.size() > 0; ) {
                mOutItems[i]->removeInItem();
            }
        }
        scene()->update();
    }
    void exec(Mat mt = Mat()) override;
    void resetItem() override
    {
        mSrcImg = Mat();
        mDstImg = Mat();
        mExec = false;
        scene()->update();
    }
protected:
    QRectF boundingRect() const override
    {
        return mRect;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        painter->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        painter->drawText(mRect,Qt::AlignCenter,mText);
        if (option->state & QStyle::State_Selected) {
            painter->setPen(Qt::red);
        }
        else if (mExec) {
            painter->setPen(Qt::green);
        }
        painter->drawRect(mRect);
    }
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
    {
        if (change == ItemPositionChange) {
            scene()->update();
        }
        return QGraphicsItem::itemChange(change,value);
    }
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    void addImageItem()
    {
        if (scene()) {
            BasicItem *item = new ImageItem(mDstImg,this);
            scene()->addItem(item);
            addOutItem(item);
        }
    }
private:
    Mat mSrcImg;
    Mat mDstImg;
    QRectF mRect;
    BasicItem *mInItem;
    QList<BasicItem*> mOutItems;
    QList<ComponentLine*> mOutLines;
    ComponentMethod<CT> mMethod;
    bool mExec;
    QString mText;
};

template<ComponentType CT>
void ComponentItem<CT>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    BasicItem::mouseDoubleClickEvent(event);
}
template<>
void ComponentItem<ctOpen>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctGray>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctRobertsEdge>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctSaltNoise>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctAverageFilter>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctAffine>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctThresholdSeg>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctCircleLBP>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
template<>
void ComponentItem<ctSIFT>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


template<ComponentType CT>
void ComponentItem<CT>::exec(Mat mt)
{
    mSrcImg = mt;
    mExec = true;
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}

template<>
void ComponentItem<ctOpen>::exec(Mat mt);
template<>
void ComponentItem<ctGray>::exec(Mat mt);
template<>
void ComponentItem<ctRobertsEdge>::exec(Mat mt);
template<>
void ComponentItem<ctSaltNoise>::exec(Mat mt);
template<>
void ComponentItem<ctAverageFilter>::exec(Mat mt);
template<>
void ComponentItem<ctAffine>::exec(Mat mt);
template<>
void ComponentItem<ctThresholdSeg>::exec(Mat mt);
template<>
void ComponentItem<ctCircleLBP>::exec(Mat mt);
template<>
void ComponentItem<ctSIFT>::exec(Mat mt);
#endif // COMPONENTITEM_H
