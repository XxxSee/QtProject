#include "mywebengnineview.h"
#include <QDebug>
#include <QApplication>

MyWebEngnineView::MyWebEngnineView(QWidget *parent): QWebEngineView(parent)
{
    QWebEngineSettings* settings = QWebEngineSettings::globalSettings();
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);//允许加载插件（flash等）
    setContextMenuPolicy(Qt::NoContextMenu);//把webview自带的迷之菜单栏取消掉
    connect(HookEvent::ins(),&HookEvent::mouseEvent,this,&MyWebEngnineView::onMouseEvent);
}

MyWebEngnineView::~MyWebEngnineView()
{
    m_dHook.UnHook();
}

void MyWebEngnineView::setHook(bool hook)
{
    hook ? m_dHook.SetHook() : m_dHook.UnHook();
}

bool MyWebEngnineView::event(QEvent *e)
{
    if (e->type() == QEvent::ChildAdded) {
        //QWebEngineView的鼠标事件会被子控件屏蔽，该控件为QOpenGLWidget类型
        QChildEvent *childe = static_cast<QChildEvent*>(e);
        childObj = childe->child();
    }
    return QWebEngineView::event(e);
}

QWebEngineView *MyWebEngnineView::createWindow(QWebEnginePage::WebWindowType)
{
    return this;//超链接的跳转……
}

void MyWebEngnineView::onMouseEvent(QMouseEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:   // 鼠标左键按下
        QApplication::sendEvent(childObj, event);
        break;
    case QEvent::MouseMove:     // 鼠标移动
        QApplication::sendEvent(childObj, event);
        break;
    case QEvent::MouseButtonRelease:     // 鼠标右键抬起
        QApplication::sendEvent(childObj, event);
        break;
    default:
        break;
    }
}
