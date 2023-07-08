#include "widget.h"
#include "ui_widget.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QInputDialog>
#include <QSettings>
#include <QApplication>
#include <QTimer>
#include <QUuid>
#include "downitemwidget.h"

Widget::Widget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QTimer::singleShot(0,this,&Widget::readResumeInfo);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("下载器");
}

void Widget::on_pushButton_clicked()
{
    QInputDialog dlg;
    dlg.setParent(nullptr);
    dlg.setWindowTitle(QStringLiteral("添加任务"));
    dlg.setTextValue("https://epicgames-download1.akamaized.net/Builds/UnrealEngineLauncher/Installers/Win32/EpicInstaller-15.7.0.msi?launcherfilename=EpicInstaller-15.7.0.msi");
    if (dlg.exec() == QDialog::Accepted) {
        QUrl url(dlg.textValue());
        if (!url.isValid())
            return;
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        ui->listWidget->addItem(item);
        QSize size = item->sizeHint();
        item->setSizeHint(QSize(size.width(),100));
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        DownItemWidget *itemWgt = new DownItemWidget();
        itemWgt->setSizeIncrement(QSize(size.width(),100));
        QString uid = QUuid::createUuid().toString();
        itemWgt->setUrl(uid,url);
        item->setData(Qt::UserRole,uid);
        ui->listWidget->setItemWidget(item,itemWgt);
        connect(itemWgt,&DownItemWidget::sigRemove,this,[&](const QString &uid){
            for (int i{0}; i<ui->listWidget->count(); i++) {
                if (ui->listWidget->item(i)->data(Qt::UserRole).toString() == uid) {
                    QListWidgetItem *curItem = ui->listWidget->takeItem(i);
                    delete curItem;
                    curItem = nullptr;
                    break;
                }
            }
        });
    }
}

void Widget::readResumeInfo()
{
    QString path = QApplication::applicationDirPath()+"/resume.ini";
    QSettings sets(path,QSettings::IniFormat);
    int size = sets.beginReadArray("FileID");
    for (int i = 0; i < size; ++i) {
        sets.setArrayIndex(i);
        QString uid = sets.value("id").toString();
        sets.endArray();
        sets.beginGroup(uid);

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        ui->listWidget->addItem(item);
        QSize size = item->sizeHint();
        item->setSizeHint(QSize(size.width(),100));
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setData(Qt::UserRole,uid);
        DownItemWidget *itemWgt = new DownItemWidget();
        ui->listWidget->setItemWidget(item,itemWgt);
        itemWgt->setSizeIncrement(QSize(size.width(),100));
        itemWgt->setResumeInfo(uid,QUrl(sets.value("url").toString()),sets.value("size").toUInt(),sets.value("multi").toBool(),sets.value("hasDownSize").toUInt());
        connect(itemWgt,&DownItemWidget::sigRemove,this,[&](const QString &uid){
            for (int i{0}; i<ui->listWidget->count(); i++) {
                if (ui->listWidget->item(i)->data(Qt::UserRole).toString() == uid) {
                    QListWidgetItem *curItem = ui->listWidget->takeItem(i);
                    delete curItem;
                    curItem = nullptr;
                    break;
                }
            }
        });

        sets.endGroup();

        sets.beginReadArray("FileID");
    }
    sets.endArray();
}

BaseWidget *createWidget()
{
    return new Widget();
}
