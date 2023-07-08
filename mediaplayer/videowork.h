#ifndef VIDEOWORK_H
#define VIDEOWORK_H

#include <QObject>
#include <QMediaPlayer>
#include "videosurface.h"

class VideoWork : public QObject
{
    Q_OBJECT
public:
    enum Operat {
        En_Init,
        En_Url,
        En_Start,
        En_Pause,
        En_Stop,
        En_Rate,
        En_Seek,
        En_Volume,
    };
    explicit VideoWork(QObject *parent = nullptr);
    ~VideoWork();
signals:
    //recv sigg
    void operatVideo(Operat op, QVariant value = QVariant());
    //send sig
    void frameImage(QImage img);
    void updateRanage(int min, int max);
    void updateValue(int value);
    void stateChanged(QMediaPlayer::State state);
private slots:
   void onOperatVideo(Operat op, QVariant value = QVariant());
private:
    QMediaPlayer *m_pPlayer;
    VideoSurface *m_pSurface;
};

#endif // VIDEOWORK_H
