#ifndef VIDEODESKTOP_H
#define VIDEODESKTOP_H

#include <QWidget>
#include "videowork.h"
#include <QThread>

namespace Ui {
class VideoDesktop;
}

class VideoDesktop : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDesktop(QWidget *parent = nullptr);
    ~VideoDesktop();
signals:
    void cancel();
protected:
    void resizeEvent(QResizeEvent *) override;
private slots:
    void on_startBtn_clicked();
    void on_pauseBtn_clicked();
    void on_stopBtn_clicked();
    void on_rateCbx_activated(const QString &text);
    void on_desktopBtn_clicked();
    void on_seekSlider_valueChanged(int value);
    void on_volumeSlider_valueChanged(int value);
    void onStateChanged(QMediaPlayer::State state);
    void on_cancelBtn_clicked();
private:
    Ui::VideoDesktop *ui;
    VideoWork *m_pVideo;
    QThread *m_pThread;
    bool m_bSetDesktop;
};

#endif // VIDEODESKTOP_H
