#include "videowork.h"
#include <QDebug>
#include <QThread>

VideoWork::VideoWork(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<Operat>("Operat");
    connect(this,&VideoWork::operatVideo,this,&VideoWork::onOperatVideo);
}

VideoWork::~VideoWork()
{
    if (m_pPlayer->state() != QMediaPlayer::StoppedState) {
        m_pPlayer->stop();
    }
}

void VideoWork::onOperatVideo(Operat op, QVariant value)
{
    if (op == En_Init) {
        m_pPlayer = new QMediaPlayer(this);
        m_pSurface = new VideoSurface(this);
        m_pPlayer->setVideoOutput(m_pSurface);
        connect(m_pSurface,&VideoSurface::frameImage,this,[=](QImage img){
            emit frameImage(img);
            emit updateValue(m_pPlayer->position());
        });
        connect(m_pPlayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration){
            emit updateRanage(0,duration);
        });
        connect(m_pPlayer,&QMediaPlayer::stateChanged,this,&VideoWork::stateChanged);
    }
    else if (op == En_Url) {
        if (m_pPlayer->state() != QMediaPlayer::StoppedState) {
            m_pPlayer->stop();
        }
        m_pPlayer->setMedia(QUrl(value.toString()));
        m_pPlayer->play();
    }
    if (op == En_Start) {
        if (m_pPlayer->state() != QMediaPlayer::PlayingState) {
           m_pPlayer->play();
        }
    }
    else if (op == En_Pause) {
        m_pPlayer->pause();
    }
    else if (op == En_Stop) {
        m_pPlayer->stop();
    }
    else if (op == En_Rate) {
        m_pPlayer->setPlaybackRate(value.toFloat());
    }
    else if (op == En_Seek) {
        m_pPlayer->setPosition((qint64)value.toInt());
    }
    else if (op == En_Volume) {
        m_pPlayer->setVolume(value.toInt());
    }
}
