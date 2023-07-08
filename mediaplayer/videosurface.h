#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QObject>
#include <QAbstractVideoSurface>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
     explicit VideoSurface(QObject *parent = 0);
signals:
    void frameImage(QImage img);
protected:
    bool present(const QVideoFrame &frame) override;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = \
            QAbstractVideoBuffer::NoHandle) const override;
};

#endif // VIDEOSURFACE_H
