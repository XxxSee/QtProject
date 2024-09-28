#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("3D");
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    BaseWidget::keyPressEvent(event);
}

void Widget::on_pushButton_clicked()
{
    ui->widget->test();
}

BaseWidget *createWidget()
{
    return new Widget();
}
