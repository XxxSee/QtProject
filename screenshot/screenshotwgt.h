#ifndef SCREENSHOTWGT_H
#define SCREENSHOTWGT_H

#include "dll_global.h"
#include <QPixmap>

namespace Ui {
class ScreenShotWgt;
}

class ScreenShotWgt : public BaseWidget
{
    Q_OBJECT
public:
    explicit ScreenShotWgt(QWidget *parent = nullptr);
    ~ScreenShotWgt();
    QString dllName() override;
private slots:
    void on_fullBtn_clicked();
    void on_rectBtn_clicked();
    void on_irreBtn_clicked();
    void on_copyBtn_clicked();
    void on_saveBtn_clicked();
private:
    void setMainWidgetVisible(bool visible);
    void handleIrreScreenShot(const QPolygon &poly);
private:
    Ui::ScreenShotWgt *ui;
    QPixmap m_dPixmap;
};
#endif // SCREENSHOTWGT_H
