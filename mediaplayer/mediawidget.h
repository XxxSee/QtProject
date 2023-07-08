#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QWidget>
#include "videodesktop.h"
#include "gifdesktop.h"
#include "webdesktop.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MediaWidget; }
QT_END_NAMESPACE

class MediaWidget : public QWidget
{
    Q_OBJECT
public:
    MediaWidget(int index, QWidget *parent = nullptr);
    ~MediaWidget();
signals:
    void cancel();
private:
    Ui::MediaWidget *ui;
    VideoDesktop *m_pVideo;
    GifDesktop *m_pGif;
    WebDesktop *m_pWeb;
};
#endif // MEDIAWIDGET_H
