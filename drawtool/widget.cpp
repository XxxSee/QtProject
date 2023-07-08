#include "widget.h"
#include "ui_widget.h"
#include "basesetwidget.h"
#include <QFileDialog>
Widget::Widget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_pRuleWgt = new RuleWidget(ui->grapWgt);
    m_pScene = new GraphicsScene(this);
    m_pScene->setBaseMapSize(1200,800);
    m_pRuleWgt->updateBaseSize(QSize(1200,800));
    m_pView = new GraphicsView(m_pScene, ui->grapWgt);
    connect(m_pView,&GraphicsView::sigUpdateRule,this,&Widget::slotUpdateRule);
    connect(m_pView,&GraphicsView::sigCurPos,m_pRuleWgt,&RuleWidget::slotCurPos);
    connect(ui->setWgt,&DrawSetWidget::sigGraphicsChanged,m_pScene,&GraphicsScene::slotGraphicsChanged);
    connect(ui->setWgt,&DrawSetWidget::sigPenAndBrushChanged,m_pScene,&GraphicsScene::slotPenAndBrushChanged);
    connect(ui->toolWgt,&ToolWidget::sigActionTriggered,this,&Widget::slotActionTriggered);
    ui->widget->setMinimumWidth(500);
    ui->setWgt->setVisible(false);
    ui->propertyWgt->setVisible(false);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("»­Í¼");
}

void Widget::resizeEvent(QResizeEvent *e)
{
    handleResizeWgts();
    QWidget::resizeEvent(e);
}

void Widget::slotUpdateRule()
{
    QList<QPoint> hpl, vpl;
    m_pView->analyseRule(hpl, vpl);
    m_pRuleWgt->updateRule(hpl, vpl);
}

void Widget::slotActionTriggered(int type, bool checked)
{
    Q_UNUSED(checked)
    switch (type) {
    case AT_None:break;
    case AT_FileOpen:handleFileOpen();break;
    case AT_FileSave:handleFileSave();break;
    case AT_OutputPixmap:handleOutputPixmap();break;
    case AT_BackgroundSet:handleBackgroundSet();break;
    case AT_ZoomIn:handleZoomIn();break;
    case AT_ZoomOut:handleZoomOut();break;
    case AT_Undo:handleUndo();break;
    case AT_Redo:handleRedo();break;
    case AT_DrawSet:handleDrawSet();break;
    case AT_GraphicsProperty:handleGraphicsProperty();break;
    default:break;
    }
}

void Widget::handleFileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,"save","./","*.xml");
    if (!fileName.isEmpty())
        m_pScene->open(fileName);
}

void Widget::handleFileSave()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,"save","./","*.xml");
    if (!fileName.isEmpty())
        m_pScene->save(fileName);
}

void Widget::handleOutputPixmap()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,"save","./","*.png;;*.jpg;;*.jpeg;;*.bmp");
    if (!fileName.isEmpty())
        m_pScene->saveToPixmap(fileName);
}

void Widget::handleZoomIn()
{
    QTransform transform = m_pView->transform();
    transform.scale(1.1,1.1);
    if (transform.m11() < 0.4) {
        return;
    }
    else if (transform.m11() < 0.5) {
        m_pView->updateTickValue(250);
    }
    else if (transform.m11() < 0.6) {
        m_pView->updateTickValue(200);
    }
    else if (transform.m11() < 0.7) {
        m_pView->updateTickValue(150);
    }
    else if (transform.m11() < 1.4) {
        m_pView->updateTickValue(100);
    }
    else if (transform.m11() < 2.1) {
        m_pView->updateTickValue(50);
    }
    else {
        return;
    }
    m_pView->setTransform(transform);
    m_pRuleWgt->updateScale(transform.m11());
    //m_pView->scale(1.1,1.1);
    slotUpdateRule();
}

void Widget::handleZoomOut()
{
    QTransform transform = m_pView->transform();
    transform.scale(1/1.1,1/1.1);
    if (transform.m11() < 0.4) {
        return;
    }
    else if (transform.m11() < 0.5) {
        m_pView->updateTickValue(250);
    }
    else if (transform.m11() < 0.6) {
        m_pView->updateTickValue(200);
    }
    else if (transform.m11() < 0.7) {
        m_pView->updateTickValue(150);
    }
    else if (transform.m11() < 1.4) {
        m_pView->updateTickValue(100);
    }
    else if (transform.m11() < 2.1) {
        m_pView->updateTickValue(50);
    }
    else {
        return;
    }
    m_pView->setTransform(transform);
    m_pRuleWgt->updateScale(transform.m11());
    //m_pView->scale(1/1.1,1/1.1);
    slotUpdateRule();
}

void Widget::handleDrawSet()
{
    ui->setWgt->setVisible(!ui->setWgt->isVisible());
    handleResizeWgts();
}

void Widget::handleGraphicsProperty()
{
    ui->propertyWgt->setVisible(!ui->propertyWgt->isVisible());
    handleResizeWgts();
}

void Widget::handleBackgroundSet()
{
    BaseSetWidget dlg;
    if (dlg.exec() == QDialog::Accepted) {
        m_pScene->setBaseMapSize(dlg.w,dlg.h,dlg.pix);
        m_pRuleWgt->updateBaseSize(QSize(dlg.w,dlg.h));
    }
}

void Widget::handleUndo()
{
    m_pScene->setUndo(true);
}

void Widget::handleRedo()
{
    m_pScene->setUndo(false);
}

void Widget::handleResizeWgts()
{
    if (ui->setWgt->isVisible() && ui->propertyWgt->isVisible()) {
        ui->setWgt->setGeometry(0,0,250,ui->widget->height());
        ui->grapWgt->setGeometry(250,0,ui->widget->width()-500,ui->widget->height());
        ui->propertyWgt->setGeometry(ui->widget->width()-250,0,250,ui->widget->height());
    }
    else if (ui->setWgt->isVisible()) {
        ui->setWgt->setGeometry(0,0,250,ui->widget->height());
        ui->grapWgt->setGeometry(250,0,ui->widget->width()-250,ui->widget->height());
        ui->propertyWgt->setGeometry(0,0,0,0);
    }
    else if (ui->propertyWgt->isVisible()) {
        ui->setWgt->setGeometry(0,0,0,0);
        ui->grapWgt->setGeometry(0,0,ui->widget->width()-250,ui->widget->height());
        ui->propertyWgt->setGeometry(ui->widget->width()-250,0,250,ui->widget->height());
    }
    else {
        ui->setWgt->setGeometry(0,0,0,0);
        ui->grapWgt->setGeometry(0,0,ui->widget->width(),ui->widget->height());
        ui->propertyWgt->setGeometry(0,0,0,0);
    }
    m_pView->setGeometry(40, 40, ui->grapWgt->width()-40, ui->grapWgt->height()-60);
    m_pRuleWgt->setGeometry(0, 0, ui->grapWgt->width(), ui->grapWgt->height());
}

BaseWidget *createWidget()
{
    return new Widget();
}

