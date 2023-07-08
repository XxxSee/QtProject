#include "videodesktop.h"
#include "ui_videodesktop.h"
#include <QInputDialog>
#include <QDebug>
VideoDesktop::VideoDesktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoDesktop),
    m_bSetDesktop(false)
{
    ui->setupUi(this);
    m_pVideo = new VideoWork();
    m_pThread = new QThread(this);
    m_pVideo->moveToThread(m_pThread);
    m_pThread->start();
    emit m_pVideo->operatVideo(VideoWork::En_Init);
    connect(m_pVideo,&VideoWork::frameImage,ui->videoWgt,&VideoWidget::onShowImage);
    connect(m_pVideo,&VideoWork::updateRanage,ui->seekSlider,&SeekSlider::setRange);
    connect(m_pVideo,&VideoWork::updateValue,ui->seekSlider,&SeekSlider::setValueBlock);
    connect(m_pVideo,&VideoWork::stateChanged,this,&VideoDesktop::onStateChanged);
    ui->volumeSlider->setValue(50);
    ui->rateCbx->setCurrentText("1");
    ui->volumeWgt->setVisible(false);
    ui->seekWgt->setVisible(false);
}

VideoDesktop::~VideoDesktop()
{
    m_pVideo->deleteLater();
    m_pThread->quit();
    m_pThread->wait();
    delete ui;
}

void VideoDesktop::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void VideoDesktop::on_startBtn_clicked()
{
    QInputDialog dlg;
    dlg.setWindowTitle(QStringLiteral("请输入视频地址:"));
    dlg.setTextValue("https://vod.pipi.cn/43903a81vodtransgzp1251246104/bbd4f07a5285890808066187974/v.f42906.mp4");
    if (dlg.exec() == QDialog::Accepted) {
        QString url = dlg.textValue();
        //QString url = "https://vod.pipi.cn/43903a81vodtransgzp1251246104/bbd4f07a5285890808066187974/v.f42906.mp4";
        if (!url.isEmpty()){
            ui->pauseBtn->setText("pause");
            emit m_pVideo->operatVideo(VideoWork::En_Url, url);
            emit m_pVideo->operatVideo(VideoWork::En_Volume,ui->volumeSlider->value());
        }
    }
}

void VideoDesktop::on_pauseBtn_clicked()
{
    if (ui->pauseBtn->text() == "pause") {
        ui->pauseBtn->setText("continue");
        emit m_pVideo->operatVideo(VideoWork::En_Pause);
    }
    else {
        ui->pauseBtn->setText("pause");
        emit m_pVideo->operatVideo(VideoWork::En_Start);
    }
}

void VideoDesktop::on_stopBtn_clicked()
{
    ui->pauseBtn->setText("pause");
    emit m_pVideo->operatVideo(VideoWork::En_Stop);
}

void VideoDesktop::on_rateCbx_activated(const QString &text)
{
    qreal rate = text.toDouble();
    emit m_pVideo->operatVideo(VideoWork::En_Rate,rate);
}

void VideoDesktop::on_desktopBtn_clicked()
{
    //设置到desktop
    if (!m_bSetDesktop) {
        m_bSetDesktop = true;
        ui->wallpaperWgt->setDesktop(nullptr);
        ui->desktopBtn->setText(QStringLiteral("widget"));
    }
    //返回到widget
    else {
        m_bSetDesktop = false;
        ui->wallpaperWgt->setDesktop(ui->widget);
        ui->desktopBtn->setText(QStringLiteral("desktop"));
    }
}

void VideoDesktop::on_seekSlider_valueChanged(int value)
{
    disconnect(m_pVideo,&VideoWork::updateRanage,ui->seekSlider,&SeekSlider::setRange);
    emit m_pVideo->operatVideo(VideoWork::En_Seek,value);
    connect(m_pVideo,&VideoWork::updateRanage,ui->seekSlider,&SeekSlider::setRange);
}

void VideoDesktop::on_volumeSlider_valueChanged(int value)
{
    emit m_pVideo->operatVideo(VideoWork::En_Volume,value);
}

void VideoDesktop::onStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        ui->videoWgt->setRuning(true);
        ui->volumeWgt->setVisible(true);
        ui->seekWgt->setVisible(true);
    }
    else if (state == QMediaPlayer::StoppedState) {
        ui->videoWgt->setRuning(false);
        ui->volumeWgt->setVisible(false);
        ui->seekWgt->setVisible(false);
        if (ui->loopChk->isChecked())
            emit m_pVideo->operatVideo(VideoWork::En_Start);
    }
}

void VideoDesktop::on_cancelBtn_clicked()
{
    emit cancel();
}
