#ifndef MYWEBENGNINEVIEW_H
#define MYWEBENGNINEVIEW_H
#include <QWebEngineView>
#include <QWebEngineSettings>
#include "mousehook.h"

class MyWebEngnineView : public QWebEngineView//自定义一个webview类型
{
    Q_OBJECT
public:
    explicit MyWebEngnineView(QWidget *parent = nullptr);
    ~MyWebEngnineView();
    void setHook(bool hook);
protected:
    bool event(QEvent *e) override;
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType);//重写这个函数，以实现超链接的跳转
private slots:
    void onMouseEvent(QMouseEvent *);
private:
    MouseHook m_dHook;
    QObject *childObj;
};

#endif // MYWEBENGNINEVIEW_H
