#ifndef DRAWSETWIDGET_H
#define DRAWSETWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QPen>
#include "layerlistmodel.h"

namespace Ui {
class DrawSetWidget;
}

class DrawSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawSetWidget(QWidget *parent = nullptr);
    ~DrawSetWidget();
signals:
    void sigGraphicsChanged(int id);
    void sigPenAndBrushChanged(const QPen &p, const QBrush &b);
private slots:
    void slotButtonClicked(int id);
    void on_penColor_clicked();
    void on_brushColor_clicked();
    void on_penWidth_valueChanged(double d);
    void on_penStyle_activated(int index);
    void on_brushStyle_activated(int index);
    void on_listView_customContextMenuRequested(const QPoint &pos);
    void on_listView_clicked(const QModelIndex &index);
private:
    QString getBtnName(int index);
private:
    Ui::DrawSetWidget *ui;
    QButtonGroup *m_pBtnGrp;
    QPen m_dPen;
    QBrush m_dBrush;
    LayerListModel *m_pLayerModel;
};

#endif // DRAWSETWIDGET_H
