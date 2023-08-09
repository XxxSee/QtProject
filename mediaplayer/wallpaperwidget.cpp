#include "wallpaperwidget.h"
#include "finddesktop.h"
#include <QLayout>
#include <QDebug>

WallpaperWidget::WallpaperWidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_QuitOnClose,false); 
}

WallpaperWidget::~WallpaperWidget()
{

}

void WallpaperWidget::setDesktop(QWidget *parentW)
{
#ifdef Q_OS_WIN
    static bool bSplit = false;
    if (!bSplit) {
        SendMessageToDesktop();
        bSplit = true;
    }
    static HWND desktopWnd = findDesktopIconWnd();
    //为nullptr设置到Desktop
    if (!parentW) {
        if (desktopWnd) {
            setParent(nullptr);
            showFullScreen();
            SetParent((HWND)winId(), desktopWnd);
            //不加这一句，视频嵌入桌面后有时刷新有时不刷新，原因未知，最好加一下
            ShowWindow((HWND)winId(), SW_SHOWMAXIMIZED);
            setVisible(true);
        }
    }
    //不为nullptr返回到Widget
    else {
        setParent(nullptr);
        //恢复正常显示，否则下次设置到桌面无法最大化
        showNormal();
        setParent(parentW);
        parentW->layout()->addWidget(this);
        //切换父窗口后，要重新显示
        setVisible(true);
    }
#endif
}
