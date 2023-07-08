#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_pDisk = new DiskWork(this);
    m_pSearchWgt = new SearchWidget(ui->stackedWidget);
    ui->stackedWidget->addWidget(m_pSearchWgt);
    connect(m_pSearchWgt,&SearchWidget::sigSearchChanged,m_pDisk,&DiskWork::slotQueryTable);
    m_pChartWgt = new ChartWidget(ui->stackedWidget);
    ui->stackedWidget->addWidget(m_pChartWgt);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("ÎÄ¼şËÑË÷");
}

void Widget::on_loadBtn_clicked()
{
    if (ui->stackedWidget->currentWidget() == m_pSearchWgt) {
        if (QMessageBox::question(nullptr,"confirm","traversal the system disk now?",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
            if (!m_pDisk->isRunning())
                m_pDisk->start();
        }
    }
}

void Widget::on_chartCbx_activated(int index)
{
    //pies
    if (index == 0) {
        m_pChartWgt->reset(ChartWidget::CT_Pie);
        m_pDisk->slotSelectTable("");
    }
    //bars
    else if (index == 1) {
        m_pChartWgt->reset(ChartWidget::CT_Bar);
        m_pDisk->slotSelectTable("");
    }
}

void Widget::on_leftBtn_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    if (index == 0) {
        index = ui->stackedWidget->count()-1;
    }
    else {
        index = index - 1;
    }
    ui->stackedWidget->setCurrentIndex(index);
}

void Widget::on_rightBtn_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    index = (index + 1) % ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(index);
}

BaseWidget *createWidget()
{
    return new Widget();
}
