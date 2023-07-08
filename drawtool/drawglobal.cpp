#include "drawglobal.h"
#include <QMutex>
#include <QMutexLocker>

QMutex g_mtx;
QScopedPointer<DrawGlobal> DrawGlobal::m_ins(nullptr);

DrawGlobal *DrawGlobal::ins()
{
    if (!m_ins.data()) {
        QMutexLocker locker(&g_mtx);
        if (!m_ins.data()) {
            m_ins.reset(new DrawGlobal);
        }
    }
    return m_ins.data();
}

void DrawGlobal::appendLayer(const QString &name)
{
    m_lLayers << name;
    m_mLayerState[name] = LF_NoLockAndNoHide;
    emit sigResetLayer();
}

void DrawGlobal::removeLayer(const QString &name)
{
    m_lLayers.removeOne(name);
    m_mLayerState.remove(name);
    emit sigResetLayer();
}

QList<QString> &DrawGlobal::layers()
{
    return m_lLayers;
}

void DrawGlobal::setLayerState(const QString &name, int flag, int state)
{
    m_mLayerState[name] = state;
    emit sigLayerStateChanged(name, flag, state);
}

int DrawGlobal::layerState(const QString &name)
{
    if (m_mLayerState.contains(name)) {
        return m_mLayerState[name];
    }
    return 0;
}

void DrawGlobal::clearLayers()
{
    m_mLayerState.clear();
    m_lLayers.clear();
}

DrawGlobal::DrawGlobal(QObject *parent) : QObject(parent)
{
    //qRegisterMetaType<GraphicsObject*>("GraphicsObject*");
}
