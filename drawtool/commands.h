#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QPainterPath>
#include <QPixmap>
#include <QPen>
#include <QBrush>
#include <QVariant>
#include <QList>

class QGraphicsItem;
class GraphicsObject;
class LayerManager;
class UndoCommand : public QUndoCommand
{
public:
    UndoCommand(QUndoCommand *parent = nullptr);
    virtual ~UndoCommand();
};

class AddItemCommand : public UndoCommand
{
public:
    AddItemCommand(GraphicsObject *item, LayerManager *layer, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    GraphicsObject *pItem;
    LayerManager *pLayer;
    QGraphicsItem *parentItem;
};

class RemoveItemCommand : public UndoCommand
{
public:
    RemoveItemCommand(GraphicsObject *item, LayerManager *layer, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
    void appendChild(QUndoCommand *cmd);
private:
    GraphicsObject *pItem;
    LayerManager *pLayer;
    QGraphicsItem *parentItem;
    QList<QUndoCommand *> childCmds;

};

class MoveItemCommand : public UndoCommand
{
public:
    MoveItemCommand(QGraphicsItem *item, QPointF last, QPointF now, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    QGraphicsItem *pItem;
    QPointF lastPos;
    QPointF nowPos;
};

class TransformItemCommand : public UndoCommand
{
public:
    TransformItemCommand(QGraphicsItem *item, QTransform last, QTransform now, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    QGraphicsItem *pItem;
    QTransform lastTrans;
    QTransform nowTrans;
};

class ChangeItemCommand : public UndoCommand
{
public:
    ChangeItemCommand(QGraphicsItem *item, int type, QVariant last, QVariant now, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    QGraphicsItem *pItem;
    int changeType;
    QVariant lastVal;
    QVariant nowVal;
};

class AddLayerCommand : public UndoCommand
{
public:
    AddLayerCommand(LayerManager *layer, QString name, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    LayerManager *layerManager;
    QString layerName;
};

class RemoveLayerCommand : public UndoCommand
{
public:
    RemoveLayerCommand(LayerManager *layer, QString name, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
    void appendChild(QUndoCommand *cmd);
private:
    LayerManager *layerManager;
    QString layerName;
    QList<QUndoCommand *> childCmds;
};

#endif // COMMANDS_H
