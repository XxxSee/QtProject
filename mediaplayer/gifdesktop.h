#ifndef GIFDESKTOP_H
#define GIFDESKTOP_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class GifDesktop;
}

class GifDesktop : public QWidget
{
    Q_OBJECT
public:
    explicit GifDesktop(QWidget *parent = nullptr);
    ~GifDesktop();
signals:
    void cancel();
private slots:
    void on_startBtn_clicked();
    void on_pauseBtn_clicked();
    void on_stopBtn_clicked();
    void on_rateCbx_activated(const QString &text);
    void on_gifSlider_valueChanged(int value);
    void on_desktopBtn_clicked();
    void onMovieFrameChanged(int frame);
    void on_cancelBtn_clicked();
private:
    Ui::GifDesktop *ui;
    QMovie *m_pMovie;
};

#endif // GIFDESKTOP_H
