#include"finddesktop.h"
#ifdef Q_OS_WIN
HWND WorkerW=NULL;
HWND findDesktopIconWnd()
{
    HWND hWorkerW = NULL;//WorkerW的句柄
    HWND hDefView = NULL;//SHELLDLL_DefView的句柄

    //找到WorkerW类的窗口
    hWorkerW = FindWindowExA(NULL, NULL, "WorkerW", NULL);
    //通过遍历找到包含SHELLDLL_DefView的WorkerW
    while ((!hDefView) && hWorkerW)
    {
        hDefView = FindWindowExA(hWorkerW, NULL, "SHELLDLL_DefView", NULL);
        WorkerW=hWorkerW;//得到WorkerW
        hWorkerW = FindWindowExA(NULL, hWorkerW, "WorkerW", NULL);
    }
    //这里用分裂出的空闲WorkerW窗口
    ShowWindow(hWorkerW, SW_SHOW);
    return hWorkerW;

    //隐藏窗口，不让mainwindow被挡住
    //ShowWindow(hWorkerW,0);
    //return FindWindowA("Progman",NULL);
}
void SendMessageToDesktop()
{
     PDWORD_PTR result = NULL;
     //发送消息，召唤WorkerW
     //参考：https://www.codeproject.com/articles/856020/draw-behind-desktop-icons-in-windows
     SendMessageTimeout(FindWindowA("Progman",NULL), 0x52c, 0, 0, SMTO_NORMAL, 1000, result);
}

HWND GetWorkerW()
{
    findDesktopIconWnd();
    //现在桌面所在的窗口
    return WorkerW;//得到WorkerW
}
#endif
