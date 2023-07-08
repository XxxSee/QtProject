#include "drawsetwidget.h"
#include "ui_DrawSetWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QColorDialog>
#include <QListView>
#include <QAction>
#include <QMetaEnum>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include "graphicsobject.h"
#include "layerlistmodel.h"
#include "customlistdelegate.h"
#include "drawglobal.h"

DrawSetWidget::DrawSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawSetWidget),
    m_pLayerModel(new LayerListModel(this))
{
    ui->setupUi(this);
    //ͼԪ
    m_pBtnGrp = new QButtonGroup(this);
    QGridLayout *lay = new QGridLayout(ui->page);
    lay->setMargin(0);
    for (int i=GraphicsObject::GT_Arrow; i<=GraphicsObject::GT_Text; i++) {
        QPushButton *btn = new QPushButton(ui->page);
        btn->setObjectName(QString("GT_Btn%1").arg(i));
        btn->setText(getBtnName(i));
        btn->setCheckable(true);
        btn->setFixedSize(50,50);
        lay->addWidget(btn,i/4,i%4);
        m_pBtnGrp->addButton(btn,i);
    }
    m_pBtnGrp->setExclusive(true);
    void (QButtonGroup::*pFun)(int) = &QButtonGroup::buttonClicked;
    connect(m_pBtnGrp,pFun,this,&DrawSetWidget::slotButtonClicked);
    //���ʻ�ˢ
    m_dPen.setColor(QColor(0,0,0));
    ui->penColor->setStyleSheet("border: 5px solid rgb(255,255,255); background-color: rgb(0,0,0);");
    ui->penWidth->setRange(1,20.);
    QMetaEnum penEnum = QMetaEnum::fromType<Qt::PenStyle>();
    for (int i=0; i<penEnum.keyCount(); ++i) {
        const char *enStr = penEnum.key(i);
        int enValue = penEnum.keyToValue(enStr);
        ui->penStyle->addItem(enStr,QVariant::fromValue<Qt::PenStyle>((Qt::PenStyle)enValue));
    }
    ui->penStyle->setItemDelegate(new CustomListDelegate(0,ui->penStyle));
    ui->brushColor->setStyleSheet("border: 5px solid rgb(255,255,255); background-color: rgb(0,0,0);");
    QMetaEnum brushEnum = QMetaEnum::fromType<Qt::BrushStyle>();
    for (int i=0; i<brushEnum.keyCount(); ++i) {
        const char *enStr = brushEnum.key(i);
        int enValue = brushEnum.keyToValue(enStr);
        ui->brushStyle->addItem(enStr,QVariant::fromValue<Qt::BrushStyle>((Qt::BrushStyle)enValue));
    }
    ui->brushStyle->setItemDelegate(new CustomListDelegate(1,ui->penStyle));
    //ͼ��
    ui->listView->setModel(m_pLayerModel);
}

DrawSetWidget::~DrawSetWidget()
{
    delete ui;
}

void DrawSetWidget::slotButtonClicked(int id)
{
    emit sigGraphicsChanged(id);
}

void DrawSetWidget::on_penColor_clicked()
{
    QColor color = QColorDialog::getColor(m_dPen.color(),nullptr);
    m_dPen.setColor(color);
    ui->penColor->setStyleSheet(QString("border: 5px solid rgb(255,255,255);" \
                                        "background-color: rgb(%1,%2,%3,%4);")
                                .arg(color.red())
                                .arg(color.green())
                                .arg(color.blue())
                                .arg(color.alpha()));
    emit sigPenAndBrushChanged(m_dPen, m_dBrush);
}

void DrawSetWidget::on_brushColor_clicked()
{
    QColor color = QColorDialog::getColor(m_dBrush.color(),nullptr);
    m_dBrush.setColor(color);
    ui->brushColor->setStyleSheet(QString("border: 5px solid rgb(255,255,255);" \
                                        "background-color: rgb(%1,%2,%3,%4);")
                                .arg(color.red())
                                .arg(color.green())
                                .arg(color.blue())
                                  .arg(color.alpha()));
    emit sigPenAndBrushChanged(m_dPen, m_dBrush);
}

void DrawSetWidget::on_penWidth_valueChanged(double d)
{
    m_dPen.setWidthF(d);
    emit sigPenAndBrushChanged(m_dPen, m_dBrush);
}

void DrawSetWidget::on_penStyle_activated(int index)
{
    m_dPen.setStyle(ui->penStyle->itemData(index).value<Qt::PenStyle>());
    emit sigPenAndBrushChanged(m_dPen, m_dBrush);
}

void DrawSetWidget::on_brushStyle_activated(int index)
{
    m_dBrush.setStyle(ui->brushStyle->itemData(index).value<Qt::BrushStyle>());
    emit sigPenAndBrushChanged(m_dPen, m_dBrush);
}

void DrawSetWidget::on_listView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    QModelIndex index = ui->listView->currentIndex();
    QMenu menu;
    QAction *action = menu.addAction(QStringLiteral("���ͼ��"));
    connect(action,&QAction::triggered,this,[&](){
        QInputDialog dlg;
        dlg.setLabelText(QStringLiteral("������ͼ����:"));
        dlg.setWindowTitle(QStringLiteral("���ͼ��"));
        if (dlg.exec() == QDialog::Accepted) {
            QString layerName = dlg.textValue();
            emit DrawGlobal::ins()->sigAddLayer(layerName);
        }
    });

    action = menu.addAction(QStringLiteral("ɾ��ͼ��"));
    action->setEnabled(index.isValid());
    connect(action,&QAction::triggered,this,[&](){
        QString layerName = m_pLayerModel->data(ui->listView->currentIndex()).toString();
        if (!layerName.isEmpty()) {
            QMessageBox box;
            box.setText(QStringLiteral("ȷ��ɾ��ͼ��%1").arg(layerName));
            box.addButton(QMessageBox::Ok);
            box.addButton(QMessageBox::Cancel);
            if (box.exec() == QMessageBox::Ok) {
                emit DrawGlobal::ins()->sigRemoveLayer(layerName);
            }
        }
    });

    action = menu.addAction(QStringLiteral("����ͼ��"));
    action->setCheckable(true);
    if (index.isValid()) {
        QString layerName = m_pLayerModel->data(ui->listView->currentIndex()).toString();
        int state = DrawGlobal::ins()->layerState(layerName);
        if (state & DrawGlobal::LF_Lock) {
            action->setChecked(true);
        }
        else {
            action->setChecked(false);
        }
        connect(action,&QAction::triggered,this,[&](bool checked){
            if (checked) {
                state = state | DrawGlobal::LF_Lock;
            }
            else {
                state = state & DrawGlobal::LF_Hide;
            }
            DrawGlobal::ins()->setLayerState(layerName,DrawGlobal::LF_Lock,state);
        });
    }
    else
        action->setEnabled(false);

    action = menu.addAction(QStringLiteral("����ͼ��"));
    action->setCheckable(true);
    if (index.isValid()) {
        QString layerName = m_pLayerModel->data(ui->listView->currentIndex()).toString();
        int state = DrawGlobal::ins()->layerState(layerName);
        if (state & DrawGlobal::LF_Hide) {
            action->setChecked(true);
        }
        else {
            action->setChecked(false);
        }
        connect(action,&QAction::triggered,this,[&](bool checked){
            if (checked) {
                state = state | DrawGlobal::LF_Hide;
            }
            else {
                state = state & DrawGlobal::LF_Lock;
            }
            DrawGlobal::ins()->setLayerState(layerName,DrawGlobal::LF_Hide,state);
        });
    }
    else
        action->setEnabled(false);
    menu.exec(QCursor::pos());
}

void DrawSetWidget::on_listView_clicked(const QModelIndex &index)
{
    QString layerName = m_pLayerModel->data(index).toString();
    emit DrawGlobal::ins()->sigNowLayer(layerName);
}

QString DrawSetWidget::getBtnName(int index)
{
    QString btnName = "";
    switch (index) {
    case GraphicsObject::GT_Arrow:
        btnName = QStringLiteral("����\n�༭");
        break;
    case GraphicsObject::GT_Edit:
        btnName = QStringLiteral("�ɱ༭");
        break;
    case GraphicsObject::GT_Line:
        btnName = QStringLiteral("ֱ��");
        break;
    case GraphicsObject::GT_Curve:
        btnName = QStringLiteral("����");
        break;
    case GraphicsObject::GT_Rect:
        btnName = QStringLiteral("����");
        break;
    case GraphicsObject::GT_RoundedRect:
        btnName = QStringLiteral("Բ��\n����");
        break;
    case GraphicsObject::GT_Ellipse:
        btnName = QStringLiteral("��Բ");
        break;
    case GraphicsObject::GT_Circle:
        btnName = QStringLiteral("Բ��");
        break;
    case GraphicsObject::GT_Polygon:
        btnName = QStringLiteral("�����");
        break;
    case GraphicsObject::GT_Pixmap:
        btnName = QStringLiteral("ͼƬ");
        break;
    case GraphicsObject::GT_Text:
        btnName = QStringLiteral("�ı�");
        break;
    default:break;
    }
    return btnName;
}
