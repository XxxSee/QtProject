#ifndef SEEKSLIDER_H
#define SEEKSLIDER_H

#include <QSlider>

class SeekSlider : public QSlider
{
public:
    explicit SeekSlider(QWidget *parent = nullptr);
    explicit SeekSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
    void setValueBlock(int value);
protected:
    void mousePressEvent(QMouseEvent *) override;
};

#endif // SEEKSLIDER_H
