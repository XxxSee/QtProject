#include "downitemwidget.h"
#include "ui_downitemwidget.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFileInfo>
#include <QDebug>

DownItemWidget::DownItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownItemWidget),
    m_pDown(nullptr),
    m_bInit(true)
{
    ui->setupUi(this);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    ui->progressBar->setValue(0);
}

DownItemWidget::~DownItemWidget()
{
    delete ui;
}

void DownItemWidget::setUrl(const QString &uid, const QUrl &url)
{
    m_dUrl = url;
    QFileInfo info(m_dUrl.path());
    m_dName = info.fileName();
    if (m_dName.isEmpty()) {
        m_dName = "index.html";
    }
    if (info.exists()) {
        m_dName = QStringLiteral("%1_¸±±¾.%2").arg(info.baseName()).arg(info.suffix());
    }
    ui->nameLab->setText(m_dName);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.head(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    QVariant var = reply->header(QNetworkRequest::ContentLengthHeader);
    m_dSize = var.toULongLong();
    ui->progressBar->setRange(0,m_dSize/1000/1000);
    if (reply->hasRawHeader("Accept-Ranges")) {
        m_dNum = 5;
        m_bMulti = true;
    }
    else {
        m_dNum = 1;
        m_bMulti = false;
    }
    delete reply;
    m_dUuid = uid;
    m_pDown = new DownloadManager(m_dUrl,m_dName,m_dSize,m_dNum,uid,false,this);
    connect(m_pDown,&DownloadManager::value,ui->progressBar,&QProgressBar::setValue);
}

void DownItemWidget::setResumeInfo(const QString &uid, const QUrl &url, uint size, bool multi, uint hasDownSize)
{
    m_dUrl = url;
    QFileInfo info(m_dUrl.path());
    m_dName = info.fileName();
    if (m_dName.isEmpty()) {
        m_dName = "index.html";
    }
    ui->nameLab->setText(m_dName);
    m_dSize = size;
    ui->progressBar->setRange(0,m_dSize/1000/1000);
    ui->progressBar->setValue(hasDownSize/1000/1000);
    if (multi) {
        m_dNum = 5;
        m_bMulti = true;
    }
    else {
        m_dNum = 1;
        m_bMulti = false;
    }
    m_dUuid = uid;
    m_pDown = new DownloadManager(m_dUrl,m_dName,m_dSize,m_dNum,uid,true,this);
    connect(m_pDown,&DownloadManager::value,ui->progressBar,&QProgressBar::setValue);
}

void DownItemWidget::on_downBtn_clicked()
{
    if (ui->downBtn->text() == QStringLiteral("ÏÂÔØ")) {
        if (m_bInit) {
            if (m_pDown->start()) {
                ui->downBtn->setText(QStringLiteral("ÔÝÍ£"));
                m_bInit = false;
            }
        }
        else {
            ui->downBtn->setText(QStringLiteral("ÔÝÍ£"));
            m_pDown->pause(false);
        }
    }
    else if (ui->downBtn->text() == QStringLiteral("ÔÝÍ£")) {
        if (!m_bInit) {
            ui->downBtn->setText(QStringLiteral("ÏÂÔØ"));
            m_pDown->pause(true);
        }
    }
}

void DownItemWidget::on_delBtn_clicked()
{
    if (m_pDown) {
        m_pDown->remove();
    }
    QFile::remove(m_dName);
    emit sigRemove(m_dUuid);
}
