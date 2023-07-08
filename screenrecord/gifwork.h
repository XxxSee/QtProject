#ifndef GIFWORK_H
#define GIFWORK_H

#include <QThread>
#include <QRect>

class GifWork : public QThread
{
    Q_OBJECT
public:
    enum ImageQuality {IQ_Low=0,IQ_Normal,IQ_High};
    explicit GifWork(QObject *parent = nullptr);
    ~GifWork();
    bool start(uint32_t delay, bool bRect, QRect rec);
    void stop();
protected:
    void run() override;
private:
    ImageQuality m_dQuality;
    bool m_bBegin;
    uint32_t m_dDelay;
    bool m_bRect;
    QRect m_dRect;
};

#endif // GIFWORK_H
