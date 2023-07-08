#ifndef WIDGET_H
#define WIDGET_H

#include "dll_global.h"
#include "diskwork.h"
#include "searchwidget.h"
#include "chartwidget.h"

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
    void on_loadBtn_clicked();
    void on_chartCbx_activated(int index);
    void on_leftBtn_clicked();
    void on_rightBtn_clicked();
private:
    Ui::Widget *ui;
    DiskWork *m_pDisk;
    SearchWidget *m_pSearchWgt;
    ChartWidget *m_pChartWgt;
};
#endif // WIDGET_H
