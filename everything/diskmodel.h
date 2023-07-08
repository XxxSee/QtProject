#ifndef DISKMODEL_H
#define DISKMODEL_H

#include <QAbstractItemModel>
#include <QSharedPointer>
#include <QDateTime>
#include <QDebug>
#include "diskstruct.h"
#include "datareader.h"

class AbstractModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    AbstractModel(QObject *parent = nullptr) : QAbstractItemModel(parent) {}
};

enum ViewType {
    VT_Table,
    VT_Tree,
};

template <class CT, ViewType VT = VT_Table>
class DiskModel : public AbstractModel
{
public:
    explicit DiskModel(QObject *parent = nullptr) : AbstractModel(parent) {}
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void append(qint64 id, const CT &d) {
        if (m_ids.contains(id))
            return;
        beginInsertRows(QModelIndex(),m_data.count(),m_data.count());
        m_ids << id;
        m_data << d;
        endInsertRows();
    }
    void clear() {
        beginResetModel();
        m_data.clear();
        endResetModel();
    }
private:
    QList<CT> m_data;
    QSet<qint64> m_ids;
    MemberField<CT> m_dataReader;
};

template <class CT, ViewType VT>
QModelIndex DiskModel<CT,VT>::index(int row, int column,const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row,column,nullptr);
}

template <class CT, ViewType VT>
QModelIndex DiskModel<CT,VT>::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

template <class CT, ViewType VT>
int DiskModel<CT,VT>::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.count();
}

template <class CT, ViewType VT>
int DiskModel<CT,VT>::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_dataReader.memberCount();
}

template <class CT, ViewType VT>
QVariant DiskModel<CT,VT>::data(const QModelIndex &index, int role) const
{
    QVariant var;
    if (role == Qt::DisplayRole) {
        m_dataReader.value(index.column(),m_data[index.row()],var);
    }
    return var;
}

template <class CT, ViewType VT>
QVariant DiskModel<CT,VT>::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_dataReader.label(section);
    }
    return QVariant();
}

#endif // DISKMODEL_H
