#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QMap>
class GraphicsObject;
class GraphicsScene;
class GraphicsLayer : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsLayer(QObject *parent = nullptr);
    ~GraphicsLayer();
    void addItem(GraphicsObject * item);
    void removeItem(GraphicsObject * item);
    void setLock(bool bLock);
    void setHide(bool bHide);
    QMap<QString,GraphicsObject *> &items();
    GraphicsObject *item(const QString &id);
    void clear();
private:
    QMap<QString,GraphicsObject*> m_mItems;
};

class LayerManager : public QObject
{
public:
    LayerManager(GraphicsScene *scene, QObject *parent = nullptr);
    ~LayerManager();
    bool addItem(GraphicsObject *);
    bool removeItem(GraphicsObject *);
    bool addLayer(const QString &name);
    bool removeLayer(const QString &name);
    void setCurrentLayer(const QString &name);
    QString CurrentLayer() const;
    void setLayerState(const QString &name, int flag, int state);
    GraphicsObject * item(const QString &name, const QString &id);
    QList<GraphicsObject *> items(const QString &name);
    void clear();
private:
    QMap<QString,QSharedPointer<GraphicsLayer>> m_mLayers;
    GraphicsScene *m_pScene;
    QString m_dNowLayer;
};

#endif // GRAPHICSLAYER_H
