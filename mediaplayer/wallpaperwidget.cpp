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
    //Ϊnullptr���õ�Desktop
    if (!parentW) {
        if (desktopWnd) {
            setParent(nullptr);
            showFullScreen();
            SetParent((HWND)winId(), desktopWnd);
            //������һ�䣬��ƵǶ���������ʱˢ����ʱ��ˢ�£�ԭ��δ֪����ü�һ��
            ShowWindow((HWND)winId(), SW_SHOWMAXIMIZED);
            setVisible(true);
        }
    }
    //��Ϊnullptr���ص�Widget
    else {
        setParent(nullptr);
        //�ָ�������ʾ�������´����õ������޷����
        showNormal();
        setParent(parentW);
        parentW->layout()->addWidget(this);
        //�л������ں�Ҫ������ʾ
        setVisible(true);
    }
#endif
}
