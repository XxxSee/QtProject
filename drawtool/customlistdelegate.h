#ifndef CUSTOMLISTDELEGATE_H
#define CUSTOMLISTDELEGATE_H

#include <QStyledItemDelegate>

class CustomListDelegate : public QStyledItemDelegate
{
public:
    enum Type{Pen,Brush};
    explicit CustomListDelegate(int type, QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    Type m_dType;
};

#endif // CUSTOMLISTDELEGATE_H
