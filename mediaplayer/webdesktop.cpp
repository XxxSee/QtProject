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
       QString::fromLocal8Bit("打开视频链接"), QString::fromLocal8Bit("https:// "),
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
    //设置到desktop
    if (!bSet) {
        bSet = true;
        ui->wallpaperWgt->setDesktop(nullptr);
        ui->desktopBtn->setText(QStringLiteral("widget"));
#ifdef Q_OS_WIN
        //模拟点击屏幕(100,100)位置，否则web界面会全屏置顶
        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTDOWN, 100, 100, 0, 0);//瞎点一下，让系统响应
        ui->webWgt->setHook(true);
#endif
    }
    //返回到widget
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
