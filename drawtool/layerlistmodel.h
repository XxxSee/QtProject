#ifndef LAYERLISTMODEL_H
#define LAYERLISTMODEL_H

#include <QAbstractListModel>
#include <QMap>
class LayerListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LayerListModel(QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:

};

#endif // LAYERLISTMODEL_H
