#include "layerlistmodel.h"
#include <QDebug>
#include "drawglobal.h"

LayerListModel::LayerListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(DrawGlobal::ins(),&DrawGlobal::sigResetLayer,this,[&](){
        beginResetModel();
        endResetModel();
    });
}

int LayerListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 1;
}

int LayerListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return DrawGlobal::ins()->layers().count();
}

QVariant LayerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole) {
        return DrawGlobal::ins()->layers()[index.row()];
    }
    return QVariant();
}
