#include "mousehook.h"
#include <QCursor>
#include <QMutex>

QMutex g_mtx;
QScopedPointer<HookEvent> HookEvent::m_ins(nullptr);
HookEvent *HookEvent::ins()
{
    if (!m_ins.data()) {
        QMutexLocker locker(&g_mtx);
        if (!m_ins.data()) {
            m_ins.reset(new HookEvent);
        }
    }
    return m_ins.data();
}

#ifdef Q_OS_WIN
HHOOK mouseHook=NULL;//mouse hook
HWND WinId=NULL;//web窗口的句柄
HWND hWorkerW=GetWorkerW();//获取WorkerW的句柄
UINT16 buttonMsg=0;//声明
UINT16 lastMsg=0;//声明
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam )//钩子回调函数
{
    if(GetForegroundWindow()==hWorkerW&&nCode==HC_ACTION)//当最前面的窗口是WorkerW（就是只有桌面）
    {
        QPoint point = QCursor::pos();  // 获取鼠标当前位置
        switch (wParam)
        {
        case WM_LBUTTONDOWN:   // 鼠标左键按下
            emit HookEvent::ins()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
            break;
        case WM_MOUSEMOVE:     // 鼠标移动
            emit HookEvent::ins()->mouseEvent(new QMouseEvent(QEvent::MouseMove, point, Qt::NoButton, Qt::NoButton, Qt::NoModifier));
            break;
        case WM_RBUTTONDOWN:   // 鼠标右键按下
            emit HookEvent::ins()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            break;
        case WM_RBUTTONUP:     // 鼠标右键抬起
            emit HookEvent::ins()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            break;
        case WM_LBUTTONUP:     // 鼠标左键抬起
            emit HookEvent::ins()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
            break;
        case WM_MOUSEWHEEL:    // 鼠标滚轮
        {
           MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
           //        qDebug() << QString("坐标：(%1, %2)").arg(msll->pt.x).arg(msll->pt.y);     // 获取鼠标坐标
           int delta = GET_WHEEL_DELTA_WPARAM(msll->mouseData);                     // 获取滚轮状态，向前：120，向后-120
           emit HookEvent::ins()->wheelEvent(new QWheelEvent(point, delta, Qt::MiddleButton, Qt::NoModifier));
           break;
        }
        default:
            break;
        }
    }
    return CallNextHookEx(NULL,nCode,wParam,lParam);
}
#endif

MouseHook::MouseHook()
{

}

void MouseHook::SetHook()
{
#ifdef Q_OS_WIN
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);//设置钩子
#endif
}

void MouseHook::UnHook()
{
#ifdef Q_OS_WIN
    //UnhookWindowsHookEx(mouseHook);//卸载钩子
    UnhookWindowsHook(WH_MOUSE_LL, mouseProc);
#endif
}
