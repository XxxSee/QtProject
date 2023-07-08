#include "basesetwidget.h"
#include "ui_basesetwidget.h"
#include <QIntValidator>
#include <QFileDialog>

BaseSetWidget::BaseSetWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseSetWidget)
{
    ui->setupUi(this);
    QIntValidator *intValidator = new QIntValidator(ui->wEdit);
    intValidator->setRange(100,10000);
    ui->wEdit->setValidator(intValidator);
    intValidator = new QIntValidator(ui->hEdit);
    intValidator->setRange(100,10000);
    ui->hEdit->setValidator(intValidator);
}

BaseSetWidget::~BaseSetWidget()
{
    delete ui;
}

void BaseSetWidget::on_wEdit_textEdited(const QString &text)
{
    if (text.toInt() >= 100) {
        w = text.toInt();
        if (!pix.isNull()) {
            if (ui->checkBox->isChecked()) {
                h = pix.height()*w/pix.width();
                ui->hEdit->setText(QString::number(h));
            }
        }
    }
}

void BaseSetWidget::on_hEdit_textEdited(const QString &text)
{
    if (text.toInt() >= 100) {
        h = text.toInt();
        if (!pix.isNull()) {
            if (ui->checkBox->isChecked()) {
                w = pix.width()*h/pix.height();
                ui->wEdit->setText(QString::number(w));
            }
        }
    }
}

void BaseSetWidget::on_pushButton_clicked()
{
    QString pixPath = QFileDialog::getOpenFileName(nullptr,"open","./","*.jpg;;*.png;;*.jpeg;;*.bmp");
    if (pixPath.isEmpty())
        return;
    QPixmap p(pixPath);
    if (!p.isNull()) {
        pix = p;
        w = p.width();
        h = p.height();
        ui->wEdit->setText(QString::number(w));
        ui->hEdit->setText(QString::number(h));
    }
}
