#include "seekslider.h"
#include <QMouseEvent>
#include <QStyleOptionSlider>

SeekSlider::SeekSlider(QWidget *parent) :
    QSlider(parent)
{

}

SeekSlider::SeekSlider(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation,parent)
{

}

void SeekSlider::setValueBlock(int value)
{
    blockSignals(true);
    setValue(value);
    blockSignals(false);
}

void SeekSlider::mousePressEvent(QMouseEvent *event)
{
    //获取滑块区域
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    //是否是左键按下，以及不在滑块区域
    if (event->button() == Qt::LeftButton &&
    sr.contains(event->pos()) == false) {
        int newVal;
        if (orientation() == Qt::Vertical)
            newVal = minimum() + ((maximum() - minimum()) * (height() - event->y())) / height();
        else
            newVal = minimum() + ((maximum() - minimum()) * event->x()) / width();
        //slider取值范围是否是颠倒的
        if (invertedAppearance())
            setValue(maximum() - newVal);
        else
            setValue(newVal);
        event->accept();
    }
    QSlider::mousePressEvent(event);
}
