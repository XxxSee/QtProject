#include "commands.h"
#include <QDebug>
#include "graphicsobject.h"
#include "pixmapitem.h"
#include "pathitem.h"
#include "textitem.h"
#include "graphicslayer.h"
#include "graphicsscene.h"
#include "drawglobal.h"

UndoCommand::UndoCommand(QUndoCommand *parent) : QUndoCommand(parent)
{

}

UndoCommand::~UndoCommand()
{

}

AddItemCommand::AddItemCommand(GraphicsObject *item, LayerManager *layer, QUndoCommand *parent) :
    UndoCommand(parent),
    pItem(item),
    pLayer(layer)
{
    Q_ASSERT(pItem && pLayer);
    parentItem = item->parentItem();
}

void AddItemCommand::undo()
{
    if (pItem) {
        pLayer->removeItem(pItem);
    }
}

void AddItemCommand::redo()
{
    if (pItem) {
        pItem->setParentItem(parentItem);
        pLayer->addItem(pItem);
    }
}

RemoveItemCommand::RemoveItemCommand(GraphicsObject *item, LayerManager *layer, QUndoCommand *parent) :
    UndoCommand(parent),
    pItem(item),
    pLayer(layer)
{
    Q_ASSERT(pItem && pLayer);
    childCmds.clear();
    parentItem = item->parentItem();
}

void RemoveItemCommand::undo()
{
    if (pItem) {
        pItem->setParentItem(parentItem);
        pLayer->addItem(pItem);
    }
    for (QUndoCommand *cmd : childCmds) {
        cmd->undo();
    }
}

void RemoveItemCommand::redo()
{
    if (pItem) {
        pLayer->removeItem(pItem);
    }
    for (QUndoCommand *cmd : childCmds) {
        cmd->redo();
    }
}

void RemoveItemCommand::appendChild(QUndoCommand *cmd)
{
    qDebug() << "appendchild";
    childCmds.append(cmd);
}

MoveItemCommand::MoveItemCommand(QGraphicsItem *item, QPointF last, QPointF now, QUndoCommand *parent) :
    UndoCommand(parent),
    pItem(item),
    lastPos(last),
    nowPos(now)
{
    Q_ASSERT(pItem);
}

void MoveItemCommand::undo()
{
    pItem->setPos(lastPos);
}

void MoveItemCommand::redo()
{
    if (pItem->pos() != nowPos) {
        pItem->setPos(nowPos);
    }
}

TransformItemCommand::TransformItemCommand(QGraphicsItem *item, QTransform last, QTransform now, QUndoCommand *parent) :
    UndoCommand(parent),
    pItem(item),
    lastTrans(last),
    nowTrans(now)
{
    Q_ASSERT(pItem);
}

void TransformItemCommand::undo()
{
    pItem->setTransform(lastTrans);
}

void TransformItemCommand::redo()
{
    if (pItem->transform() != nowTrans) {
        pItem->setTransform(nowTrans);
    }
}

ChangeItemCommand::ChangeItemCommand(QGraphicsItem *item, int type, QVariant last, QVariant now, QUndoCommand *parent) :
    UndoCommand(parent),
    pItem(item),
    changeType(type),
    lastVal(last),
    nowVal(now)
{
    Q_ASSERT(pItem);
}

void ChangeItemCommand::undo()
{
    switch (changeType) {
    case DrawGlobal::ICT_Move:
    {
        pItem->setPos(lastVal.value<QPointF>());
    }
        break;
    case DrawGlobal::ICT_ZValue:
    {
        pItem->setZValue(lastVal.value<qreal>());
    }
        break;
    case DrawGlobal::ICT_Scale:
    {
        pItem->setScale(lastVal.value<qreal>());
    }
        break;
    case DrawGlobal::ICT_Rotate:
    {
        pItem->setRotation(lastVal.value<qreal>());
    }
        break;
    case DrawGlobal::ICT_Pen:
    {
        PathItem *item = dynamic_cast<PathItem*>(pItem);
        if (item) {
            item->setPen(lastVal.value<QPen>());
        }
    }
        break;
    case DrawGlobal::ICT_Brush:
    {
        PathItem *item = dynamic_cast<PathItem*>(pItem);
        if (item) {
            item->setBrush(lastVal.value<QBrush>());
        }
    }
        break;
    case DrawGlobal::ICT_Text:
    {
        TextItem *item = dynamic_cast<TextItem*>(pItem);
        if (item) {
            item->setText(lastVal.value<QString>());
        }
    }
        break;
    case DrawGlobal::ICT_Html:
    {
        TextItem *item = dynamic_cast<TextItem*>(pItem);
        if (item) {
            item->setHtml(lastVal.value<QString>());
        }
    }
        break;
    case DrawGlobal::ICT_Font:
    {
        TextItem *item = dynamic_cast<TextItem*>(pItem);
        if (item) {
            item->setFont(lastVal.value<QFont>());
        }
    }
        break;
    default:
        break;
    }
}

void ChangeItemCommand::redo()
{
    switch (changeType) {
    case DrawGlobal::ICT_Move:
    {
        pItem->setPos(nowVal.value<QPointF>());
    }
        break;
    case DrawGlobal::ICT_ZValue:
    {
        pItem->setZValue(nowVal.value<qreal>());
    }
        break;
    case DrawGlobal::ICT_Scale:
    {
        pItem->setScale(nowVal.value<qreal>());
    }
        break;
    case DrawGlobal::ICT_Rotate:
    {
        pItem->setRotation(nowVal.value<qreal>());
    }
        break;
    case DrawGlobal::ICT_Pen:
    {
        PathItem *item = dynamic_cast<PathItem*>(pItem);
        if (item) {
            item->setPen(nowVal.value<QPen>());
        }
    }
        break;
    case DrawGlobal::ICT_Brush:
    {
        PathItem *item = dynamic_cast<PathItem*>(pItem);
        if (item) {
            item->setBrush(nowVal.value<QBrush>());
        }
    }
        break;
    case DrawGlobal::ICT_Text:
    {
        TextItem *item = dynamic_cast<TextItem*>(pItem);
        if (item) {
            item->setText(nowVal.value<QString>());
        }
    }
        break;
    case DrawGlobal::ICT_Html:
    {
        TextItem *item = dynamic_cast<TextItem*>(pItem);
        if (item) {
            item->setHtml(nowVal.value<QString>());
        }
    }
        break;
    case DrawGlobal::ICT_Font:
    {
        TextItem *item = dynamic_cast<TextItem*>(pItem);
        if (item) {
            item->setFont(nowVal.value<QFont>());
        }
    }
        break;
    default:
        break;
    }
}

AddLayerCommand::AddLayerCommand(LayerManager *layer, QString name, QUndoCommand *parent) :
    UndoCommand(parent),
    layerManager(layer),
    layerName(name)
{

}

void AddLayerCommand::undo()
{
    layerManager->removeLayer(layerName);
}

void AddLayerCommand::redo()
{
    layerManager->addLayer(layerName);
}

RemoveLayerCommand::RemoveLayerCommand(LayerManager *layer, QString name, QUndoCommand *parent) :
    UndoCommand(parent),
    layerManager(layer),
    layerName(name)
{

}

void RemoveLayerCommand::undo()
{
    layerManager->addLayer(layerName);
    for (QUndoCommand *cmd : childCmds) {
        cmd->undo();
    }
}

void RemoveLayerCommand::redo()
{
    layerManager->removeLayer(layerName);
    for (QUndoCommand *cmd : childCmds) {
        cmd->redo();
    }
}

void RemoveLayerCommand::appendChild(QUndoCommand *cmd)
{
    childCmds.append(cmd);
}
