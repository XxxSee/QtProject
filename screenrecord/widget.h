#ifndef WIDGET_H
#define WIDGET_H

#include "dll_global.h"
#include <QThread>
#include "gifwork.h"
#include "rectwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QTimer;
class Widget : public BaseWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QString dllName() override;
private slots:
    void on_startBtn_clicked();
    void on_stopBtn_clicked();
    void on_fullRadio_toggled(bool checked);
    void on_rectRadio_toggled(bool checked);
private:
    Ui::Widget *ui;
    GifWork *m_pGif;
    RectWidget m_dRectWgt;
};
#endif // WIDGET_H
