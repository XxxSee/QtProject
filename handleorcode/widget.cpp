#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QDebug>
#include <QFileDialog>
#include <QtMath>
#include "qrencode.h"
#include "QZXing.h"
#include "cameradialog.h"

//#define ENABLE_ENCODER_GENERIC

Widget::Widget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_dIconImg = QImage(100,100,QImage::Format_RGBA8888);
    m_dIconImg.fill(Qt::white);
    ui->fColor->setEnabled(false);
    ui->bColor->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("识别&&生成\n二维码");
}

void Widget::on_genBtn_clicked()
{
    QString text = ui->codeEdit->text();
    if (text.isEmpty())
        return;
    int bulk = ui->spinBox->value();
    //m_dCodeImg = QZXing::encodeData(text,QZXing::EncoderFormat_QR_CODE);
    m_dCodeImg = encodeImage(text,bulk);
    ui->genLab->setPixmap(QPixmap::fromImage(m_dCodeImg));
}

void Widget::on_downBtn_clicked()
{
    if (m_dCodeImg.isNull())
        return;
    QString path = QFileDialog::getSaveFileName(nullptr,"save code","./","*.png");
    m_dCodeImg.save(path,"PNG");
}

void Widget::on_iconBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(nullptr,QStringLiteral("打开图标"),"./","*.png;;*.jpg;;*.jpeg;;*.bmp");
    if (path.isEmpty())
        return;
    m_dIconImg.load(path);
}

void Widget::on_idenBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(nullptr,QStringLiteral("打开图标"),"./","*.png;;*.jpg;;*.jpeg;;*.bmp");
    if (path.isEmpty())
        return;
    QImage img;
    if (img.load(path)) {
        QZXing decoder;
        //必要设置
        decoder.setDecoder(QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13);
        //可选设置
        decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
        decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);
        QString str = decoder.decodeImage(img);
        if(!str.isEmpty()){
            ui->codeEdit->setText(str);
        }
        else {
            ui->codeEdit->setText(QStringLiteral("无法识别二维码"));
        }
    }
}

void Widget::on_scanBtn_clicked()
{
    CameraDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        ui->codeEdit->setText(dlg.codeText());
    }
}

QImage Widget::encodeImage(const QString &s, int bulk)
{
    QImage codeImage;
    qDebug() << s.toUtf8();
    QRcode* qren = QRcode_encodeString(s.toUtf8(), 1, QR_ECLEVEL_Q, QR_MODE_8, 1);
    if (qren) {
        int allBulk = (qren->width) * bulk;
        QRect codeRect(0, 0, allBulk, allBulk);
        codeImage = QImage(allBulk, allBulk, QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&codeImage);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRect(codeRect);
        painter.setBrush(Qt::black);
        for (int y{0}; y<qren->width; y++) {
            for (int x{0}; x<qren->width; x++) {
                if (qren->data[y*qren->width+x] & 1) {
                    QRect r(x*bulk,y*bulk,bulk,bulk);
                    painter.drawRect(r);
                }
            }
        }
        if (ui->iconChk->isChecked()) {
            int iconW = qFloor(allBulk/5.);
            QRect iconRect(codeRect.center().x()-iconW/2,codeRect.center().y()-iconW/2,iconW,iconW);
            painter.drawImage(iconRect,m_dIconImg);
        }
        QRcode_free(qren);
    }
    else
        qDebug() << QStringLiteral("生成失败");
    return codeImage;
}

BaseWidget *createWidget()
{
    return new Widget();
}
