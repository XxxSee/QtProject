#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include <QLibrary>
#include <QMap>
#include <QButtonGroup>
#include <QParallelAnimationGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    bool eventFilter(QObject *o, QEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
private:
    void initWidget();
    void loadPlugin();
private slots:
    void on_visibleBtn_clicked();
private:
    Ui::Widget *ui;
    QStackedLayout *m_pStackedLay;
    QButtonGroup *m_pBtnGrp;
    QMap<QString, QLibrary*> m_mLibs;
    QMap<QString, int> m_mWgts;
    QParallelAnimationGroup *m_pAniGrp1, *m_pAniGrp2;
    bool m_bBtnWgtShow;
};
#endif // WIDGET_H
