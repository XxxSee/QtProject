#include "componentitem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include "componentdialog.h"
#include "componentmethod.h"

QImage mat2Image(Mat t)
{
    if (t.channels() == 1) {
        return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_Indexed8);
    }
    else if (t.channels() == 3) {
        return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_RGB888).rgbSwapped();
    }
    else if (t.channels() == 4) {
        return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_ARGB32).rgbSwapped();
    }
    return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_RGB888).rgbSwapped();
}

Mat image2Mat(QImage img)
{
    if (img.format() == QImage::Format_Indexed8) {
        return Mat(img.height(),img.width(),CV_8UC1,img.bits());
    }
    else if (img.format() == QImage::Format_RGB888) {
        return Mat(img.height(),img.width(),CV_8UC3,img.bits());
    }
    else if (img.format() == QImage::Format_ARGB32) {
        return Mat(img.height(),img.width(),CV_8UC4,img.bits());
    }
    else if (img.format() == QImage::Format_RGB32) {
        return Mat(img.height(),img.width(),CV_8UC4,img.bits());
    }
    return Mat(img.height(),img.width(),CV_8UC3,img.bits(),img.bytesPerLine());
}

ComponentLine::ComponentLine(BasicItem *out, BasicItem *in, QGraphicsItem *parent) :
    BasicItem(ctLine,parent),
    mOutItem(out),
    mInItem(in)
{
    setFlags(ItemIsSelectable | ItemIsFocusable);
}

QPointF ComponentLine::inScenePos() const
{
    if (mInItem) {
        return mInItem->inScenePos();
    }
    return QPointF();
}

QPointF ComponentLine::outScenePos() const
{
    if (mOutItem) {
        return mOutItem->outScenePos();
    }
    return QPointF();
}

void ComponentLine::addInItem(BasicItem *item)
{
    mInItem = item;
}

void ComponentLine::removeInItem()
{
    if (mInItem) {
        mInItem->removeInItem();
    }
}

void ComponentLine::addOutItem(BasicItem *item)
{
    mOutItem = item;
}

void ComponentLine::removeOutItem(BasicItem *)
{
    if (mOutItem) {
        mOutItem->removeOutItem(mInItem);
    }
}

QPainterPath ComponentLine::shape() const
{
    QPointF beginPos = outScenePos();
    QPointF endPos = inScenePos();
    QPointF turnPos1,turnPos2;
    if (beginPos.y() > endPos.y()) {
        turnPos1 = QPointF((beginPos.x()+endPos.x())/2.,beginPos.y());
        turnPos2 = QPointF((beginPos.x()+endPos.x())/2.,endPos.y());
    }
    else {
        turnPos1 = QPointF(beginPos.x(),(beginPos.y()+endPos.y())/2.);
        turnPos2 = QPointF(endPos.x(),(beginPos.y()+endPos.y())/2.);
    }
    QPainterPath path;
    path.moveTo(beginPos);
    path.lineTo(turnPos1);
    path.lineTo(turnPos2);
    path.lineTo(endPos);
    return path;
}

QRectF ComponentLine::boundingRect() const
{
    QPainterPath path = shape();
    return path.boundingRect();
}

void ComponentLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path = shape();
    painter->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    if (option->state & QStyle::State_Selected) {
        painter->setPen(Qt::red);
    }
    painter->drawPath(path);
}

ImageItem::ImageItem(Mat t, BasicItem *item, QGraphicsItem *parent) :
    BasicItem(ctImage,parent),
    mInItem(item)
{
    QPointF curP = mInItem->mapToScene(mInItem->boundingRect().topRight() + QPointF(300,100)) ;
    mShowImg = mat2Image(t);
    mShowImg = mShowImg.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    mRect = QRectF(-150,-150,300,300);
    setPos(curP);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsScenePositionChanges);
}

QPointF ImageItem::inScenePos() const
{
    return mapToScene(QPointF(mRect.center().x(),mRect.top()));
}

QPointF ImageItem::outScenePos() const
{
    return mapToScene(QPointF(mRect.center().x(),mRect.bottom()));
}

void ImageItem::addInItem(BasicItem *item)
{
    mInItem = item;
}

void ImageItem::removeInItem()
{
    if (mInItem) {
        mInItem->removeOutItem(this);
    }
}

void ImageItem::addOutItem(BasicItem *)
{

}

void ImageItem::removeOutItem(BasicItem *)
{

}

QRectF ImageItem::boundingRect() const
{
    return mRect;
}

void ImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    painter->drawImage(mRect.topLeft(),mShowImg);
}

QVariant ImageItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        scene()->update();
    }
    return QGraphicsItem::itemChange(change,value);
}

template<>
void ComponentItem<ctOpen>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctOpen> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.path = vars[0].toString();
    }
    BasicItem::mouseDoubleClickEvent(event);
}

template<>
void ComponentItem<ctGray>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctGray> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}

template<>
void ComponentItem<ctRobertsEdge>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctRobertsEdge> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}

template<>
void ComponentItem<ctSaltNoise>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctSaltNoise> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}
template<>
void ComponentItem<ctAverageFilter>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctAverageFilter> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}
template<>
void ComponentItem<ctAffine>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctAffine> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}
template<>
void ComponentItem<ctThresholdSeg>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctThresholdSeg> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}
template<>
void ComponentItem<ctCircleLBP>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctCircleLBP> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}
template<>
void ComponentItem<ctSIFT>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentDialog<ctSIFT> dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QVariantList vars = dlg.getVariantList();
        mMethod.type = (ComponentType)vars[0].toInt();
        mText = type2String(mMethod.type);
        update();
    }
    BasicItem::mouseDoubleClickEvent(event);
}


template<>
void ComponentItem<ctOpen>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mMethod.path.isEmpty())
        return;
    mMethod.invokeMethod(mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctGray>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctRobertsEdge>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctSaltNoise>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctAverageFilter>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctAffine>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctThresholdSeg>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctCircleLBP>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
template<>
void ComponentItem<ctSIFT>::exec(Mat mt)
{
    mSrcImg = mt;
    if (mSrcImg.empty())
        return;
    mMethod.invokeMethod(mSrcImg,mDstImg);
    mExec = true;
    addImageItem();
    for (int i{0}; i<mOutItems.size(); i++) {
        mOutItems[i]->exec(mDstImg);
    }
}
