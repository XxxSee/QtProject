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
    //��ȡ��������
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    //�Ƿ���������£��Լ����ڻ�������
    if (event->button() == Qt::LeftButton &&
    sr.contains(event->pos()) == false) {
        int newVal;
        if (orientation() == Qt::Vertical)
            newVal = minimum() + ((maximum() - minimum()) * (height() - event->y())) / height();
        else
            newVal = minimum() + ((maximum() - minimum()) * event->x()) / width();
        //sliderȡֵ��Χ�Ƿ��ǵߵ���
        if (invertedAppearance())
            setValue(maximum() - newVal);
        else
            setValue(newVal);
        event->accept();
    }
    QSlider::mousePressEvent(event);
}
