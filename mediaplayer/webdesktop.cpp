#include "webdesktop.h"
#include "ui_webdesktop.h"
#include <QInputDialog>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
WebDesktop::WebDesktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WebDesktop)
{
    ui->setupUi(this);
}

WebDesktop::~WebDesktop()
{
    delete ui;
}

void WebDesktop::on_startBtn_clicked()
{
    QString urlStr = QInputDialog::getText(this,
       QString::fromLocal8Bit("����Ƶ����"), QString::fromLocal8Bit("https:// "),
       QLineEdit::Normal,
       "http://iamthecu.be/");
    if (urlStr.isEmpty())
        return;
    ui->webWgt->load(QUrl(urlStr));
}

void WebDesktop::on_stopBtn_clicked()
{
    ui->webWgt->load(QUrl(""));
}

void WebDesktop::on_desktopBtn_clicked()
{
    static bool bSet = false;
    //���õ�desktop
    if (!bSet) {
        bSet = true;
        ui->wallpaperWgt->setDesktop(nullptr);
        ui->desktopBtn->setText(QStringLiteral("widget"));
#ifdef Q_OS_WIN
        //ģ������Ļ(100,100)λ�ã�����web�����ȫ���ö�
        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTDOWN, 100, 100, 0, 0);//Ϲ��һ�£���ϵͳ��Ӧ
        ui->webWgt->setHook(true);
#endif
    }
    //���ص�widget
    else {
        ui->webWgt->setHook(false);
        bSet = false;
        ui->wallpaperWgt->setDesktop(ui->widget);
        ui->desktopBtn->setText(QStringLiteral("desktop"));
    }
}

void WebDesktop::on_cancelBtn_clicked()
{
    emit cancel();
}
