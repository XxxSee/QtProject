#ifndef WIDGET_H
#define WIDGET_H

#include "dll_global.h"
#include "mediawidget.h"

namespace Ui {
class Widget;
}

class Widget : public BaseWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    virtual QString dllName();
private slots:
    void on_pushButton_clicked();
private:
    Ui::Widget *ui;
    MediaWidget *m_pMediaWgt;
};

#endif // WIDGET_H
