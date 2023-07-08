#include "gifwork.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QDebug>
#include <QDateTime>
#include "gif.h"

GifWork::GifWork(QObject *parent) :
    QThread(parent),
    m_dQuality(IQ_High),
    m_bBegin(false),
    m_dDelay(10),
    m_bRect(false),
    m_dRect(0,0,0,0)
{

}

GifWork::~GifWork()
{
    stop();
    quit();
    wait();
}

bool GifWork::start(uint32_t delay, bool bRect, QRect rec)
{
    if (!isRunning()) {
        m_dDelay = delay;
        m_bRect = bRect;
        m_dRect = rec;
        QThread::start();
        return true;
    }
    return false;
}

void GifWork::stop()
{
    m_bBegin = false;
}

void GifWork::run()
{
    if (!m_bBegin) {
        QString filename = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".gif";
        int w = m_bRect ? m_dRect.width() : QApplication::primaryScreen()->geometry().width();
        int h = m_bRect ? m_dRect.height() : QApplication::primaryScreen()->geometry().height();
        GifWriter writer;
        if (GifBegin(&writer, filename.toStdString().c_str(), w, h, m_dDelay)) {
            m_bBegin = true;
            while (m_bBegin) {
                QImage img;
                if (!m_bRect) {
                    img = QApplication::primaryScreen()->grabWindow(0).toImage().convertToFormat(QImage::Format_RGBA8888);
                }
                else {
                    img = QApplication::primaryScreen()->grabWindow(0,m_dRect.x(),m_dRect.y(),m_dRect.width(),m_dRect.height()).toImage().convertToFormat(QImage::Format_RGBA8888);
                }
                GifWriteFrame(&writer, img.bits(), w, h, m_dDelay);
                msleep(1000/m_dDelay);
            }
            GifEnd(&writer);
        }
    }
}
