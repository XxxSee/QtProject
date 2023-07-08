#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::Widget),
    m_pMediaWgt(nullptr)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("¶¯Ì¬±ÚÖ½");
}

void Widget::on_pushButton_clicked()
{
    int index = ui->comboBox->currentIndex();
    if (parentWidget()){
        ui->widget->setVisible(false);
        if (m_pMediaWgt) {
            m_pMediaWgt->deleteLater();
            m_pMediaWgt = nullptr;
        }
        ui->widget->hide();
        m_pMediaWgt = new MediaWidget(index, parentWidget());
        ui->gridLayout->addWidget(m_pMediaWgt);
        connect(m_pMediaWgt,&MediaWidget::cancel,this,[&](){
            ui->gridLayout->removeWidget(m_pMediaWgt);
            m_pMediaWgt->deleteLater();
            m_pMediaWgt = nullptr;
            ui->widget->show();
        });
    }
    else {
        if (m_pMediaWgt) {
            m_pMediaWgt->deleteLater();
            m_pMediaWgt = nullptr;
        }
        m_pMediaWgt = new MediaWidget(index);
        hide();
        m_pMediaWgt->show();
        connect(m_pMediaWgt,&MediaWidget::cancel,this,[&](){
            m_pMediaWgt->deleteLater();
            m_pMediaWgt = nullptr;
            show();
        });
    }
}

BaseWidget *createWidget()
{
    return new Widget();
}
