#ifndef WIDGET_H
#define WIDGET_H

#include "dll_global.h"
#include <QImage>

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
    void on_genBtn_clicked();
    void on_downBtn_clicked();
    void on_iconBtn_clicked();
    void on_idenBtn_clicked();
    void on_scanBtn_clicked();
private:
    QImage encodeImage(const QString& s, int bulk);
private:
    Ui::Widget *ui;
    QImage m_dIconImg;
    QImage m_dCodeImg;
};
#endif // WIDGET_H
