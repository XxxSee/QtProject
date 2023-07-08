#include "videosurface.h"

VideoSurface::VideoSurface(QObject *parent) :
    QAbstractVideoSurface(parent)
{

}

bool VideoSurface::present(const QVideoFrame &frame)
{
    if (frame.isValid())
    {
        QVideoFrame cloneFrame(frame);
        cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
        QImage recvImage(cloneFrame.bits(), cloneFrame.width(), cloneFrame.height(), frame.bytesPerLine(), QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));
        cloneFrame.unmap(); 
        emit frameImage(recvImage);
        return true;
    }
    return false;
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if(handleType == QAbstractVideoBuffer::NoHandle)
    {
       return QList<QVideoFrame::PixelFormat>()
               << QVideoFrame::Format_RGB32;
    }
    else
       return QList<QVideoFrame::PixelFormat>();
}
