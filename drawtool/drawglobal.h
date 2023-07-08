#ifndef DRAWGLOBAL_H
#define DRAWGLOBAL_H

#include <QObject>
#include <QScopedPointer>
#include <QGraphicsItem>
#include <QMap>
#include <QList>
class GraphicsObject;
class DrawGlobal : public QObject
{
    Q_OBJECT
public:
    enum LayerFlag {LF_Lock=0x01,LF_Hide=0x10,};
    enum LayerState {LF_NoLockAndNoHide=0x00,LF_LockAndNoHide=0x01,LF_NoLockAndHide=0x10,LF_LockAndHide=0x11,};
    enum ItemChangeType {ICT_Move,ICT_ZValue,ICT_Scale,ICT_Rotate,ICT_Pen,ICT_Brush,ICT_Text,ICT_Html,ICT_Font};
    static DrawGlobal *ins();

    void appendLayer(const QString &name);
    void removeLayer(const QString &name);

    void setLayerState(const QString &name, int flag, int state);
    int layerState(const QString &name);

    QList<QString>& layers();
    void clearLayers();
signals:
    void sigItemChanged(QGraphicsItem *item, int type, QVariant last, QVariant now);
    void sigResetLayer();
    void sigAddLayer(const QString &name);
    void sigRemoveLayer(const QString &name);
    void sigNowLayer(const QString &name);
    void sigLayerStateChanged(const QString &name, int flag, int state);
    void sigUpdateCurrentItem(GraphicsObject *obj, bool selected);
protected:
    explicit DrawGlobal(QObject *parent = nullptr);
private:
    static QScopedPointer<DrawGlobal> m_ins;
    QList<QString> m_lLayers;
    QMap<QString,int> m_mLayerState;
};

#endif // DRAWGLOBAL_H
