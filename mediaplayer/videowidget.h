#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);
    void setRuning(bool run);
protected:
    void paintEvent(QPaintEvent *) override;
public slots:
    void onShowImage(QImage img);
private:
    QImage m_dImg;
    bool m_bRuning;
};

#endif // VIDEOWIDGET_H
