#ifndef WEBDESKTOP_H
#define WEBDESKTOP_H

#include <QWidget>

namespace Ui {
class WebDesktop;
}

class WebDesktop : public QWidget
{
    Q_OBJECT
public:
    explicit WebDesktop(QWidget *parent = nullptr);
    ~WebDesktop();
signals:
    void cancel();
private slots:
    void on_startBtn_clicked();
    void on_stopBtn_clicked();
    void on_desktopBtn_clicked();
    void on_cancelBtn_clicked();
private:
    Ui::WebDesktop *ui;
};

#endif // WEBDESKTOP_H
