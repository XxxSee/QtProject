#ifndef BASESETWIDGET_H
#define BASESETWIDGET_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class BaseSetWidget;
}

class BaseSetWidget : public QDialog
{
    Q_OBJECT
public:
    explicit BaseSetWidget(QWidget *parent = nullptr);
    ~BaseSetWidget();
    int w,h;
    QPixmap pix;
private slots:
    void on_wEdit_textEdited(const QString &text);
    void on_hEdit_textEdited(const QString &text);
    void on_pushButton_clicked();
private:
    Ui::BaseSetWidget *ui;
};

#endif // BASESETWIDGET_H
