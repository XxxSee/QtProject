#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QApplication>
#include <QScreen>
#include <QDateTime>
//#include "gif.h"

//GifWriter g_dWriter;
Widget::Widget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::Widget)
    , m_pGif(new GifWork(this))
{
    ui->setupUi(this);
    ui->fullRadio->setChecked(true);
    ui->delayCbk->setCurrentText("10");
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("б╪фа");
}

void Widget::on_startBtn_clicked()
{
    if (m_pGif->start(ui->delayCbk->currentText().toInt(),ui->rectRadio->isChecked(),m_dRectWgt.recordRect())) {
        m_dRectWgt.setCanChange(false);
    }
}

void Widget::on_stopBtn_clicked()
{
    m_pGif->stop();
    m_dRectWgt.setCanChange(true);
}

void Widget::on_fullRadio_toggled(bool checked)
{
    if (checked) {
        m_dRectWgt.hide();
    }
}

void Widget::on_rectRadio_toggled(bool checked)
{
    if (checked) {
        m_dRectWgt.showFullScreen();
    }
}

BaseWidget *createWidget()
{
    return new Widget();
}
