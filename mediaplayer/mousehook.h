#ifndef MOUSEHOOK_H
#define MOUSEHOOK_H
#include <QtGlobal>
#include <QObject>
#include <QMouseEvent>
#include <QScopedPointer>
#include <QWheelEvent>
#include "finddesktop.h"

class HookEvent : public QObject
{
    Q_OBJECT
public:
    static HookEvent *ins();
signals:
    void mouseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
private:
    static QScopedPointer<HookEvent> m_ins;
};

class MouseHook
{
public:
    MouseHook();
    void SetHook();//设置钩子
    void UnHook();//卸载钩子
};
#endif // MOUSEHOOK_H
