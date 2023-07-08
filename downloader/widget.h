#ifndef WIDGET_H
#define WIDGET_H

#include "dll_global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public BaseWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QString dllName() override;
private slots:
    void on_pushButton_clicked();
private:
    void readResumeInfo();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
