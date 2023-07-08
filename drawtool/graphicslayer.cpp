#include "graphicslayer.h"
#include <QDebug>
#include <QUuid>
#include <QMessageBox>
#include "graphicsobject.h"
#include "graphicsscene.h"
#include "drawglobal.h"

/************************GraphicsLayer**************************/
GraphicsLayer::GraphicsLayer(QObject *parent) : QObject(parent)
{

}

GraphicsLayer::~GraphicsLayer()
{
    for (GraphicsObject *obj : m_mItems) {
        QGraphicsScene *scene = obj->scene();
        if (scene)
            scene->removeItem(obj);
    }
    m_mItems.clear();
}

void GraphicsLayer::addItem(GraphicsObject * item)
{
    m_mItems[item->graphicsId()] = item;
}

void GraphicsLayer::removeItem(GraphicsObject * item)
{
    QGraphicsScene *scene = item->scene();
    if (scene)
        scene->removeItem(item);
    m_mItems.remove(item->graphicsId());
}

void GraphicsLayer::setLock(bool bLock)
{
    for (GraphicsObject *obj : m_mItems) {
        obj->setEidtEnable(!bLock);
    }
}

void GraphicsLayer::setHide(bool bHide)
{
    for (GraphicsObject *obj : m_mItems) {
        obj->setVisible(!bHide);
    }
}

QMap<QString,GraphicsObject *> &GraphicsLayer::items()
{
    return m_mItems;
}

GraphicsObject * GraphicsLayer::item(const QString &id)
{
    if (m_mItems.contains(id))
        return m_mItems[id];
    return nullptr;
}

void GraphicsLayer::clear()
{
    for (GraphicsObject *obj : m_mItems) {
        delete obj;
        obj = nullptr;
    }
    m_mItems.clear();
}

/************************LayerManager**************************/
LayerManager::LayerManager(GraphicsScene *scene, QObject *parent) :
    QObject(parent),
    m_pScene(scene)
{
    addLayer("default");
}

LayerManager::~LayerManager()
{
    m_mLayers.clear();
}

bool LayerManager::addItem(GraphicsObject *item)
{
    if (item->graphicsLayer().isEmpty()) {
        item->setGraphicsLayer(m_dNowLayer);
    }
    if (item->graphicsId().isEmpty()) {
        item->setGraphicsId(QUuid::createUuid().toString());
    }
    if (m_mLayers.contains(item->graphicsLayer())) {
        m_mLayers[item->graphicsLayer()]->addItem(item);
    }
    else {
        if (item->scene()) {
            item->scene()->removeItem(item);
        }
        return false;
    }
    if (!item->parentItem()) {
        m_pScene->addItem(item);
    }
    return true;
}

bool LayerManager::removeItem(GraphicsObject *item)
{
    if (m_mLayers.contains(item->graphicsLayer())) {
        m_mLayers[item->graphicsLayer()]->removeItem(item);
    }
    else
        return false;
    return true;
}

bool LayerManager::addLayer(const QString &name)
{
    if (m_mLayers.contains(name)) {
        QMessageBox::warning(nullptr,"warning",QStringLiteral("图层已存在"));
        return false;
    }
    QSharedPointer<GraphicsLayer> lay(new GraphicsLayer);
    m_mLayers[name] = lay;
    DrawGlobal::ins()->appendLayer(name);
    m_dNowLayer = name;
    return true;
}

bool LayerManager::removeLayer(const QString &name)
{
    if (!m_mLayers.contains(name)) {
        QMessageBox::warning(nullptr,"warning",QStringLiteral("图层不存在"));
        return false;
    }
    if (name == "default") {
        QMessageBox::warning(nullptr,"warning",QStringLiteral("默认图层无法删除"));
        return false;
    }
    m_mLayers.remove(name);
    DrawGlobal::ins()->removeLayer(name);
    m_dNowLayer = "default";
    return true;
}

void LayerManager::setCurrentLayer(const QString &name)
{
    m_dNowLayer = name;
}

QString LayerManager::CurrentLayer() const
{
    return m_dNowLayer;
}

void LayerManager::setLayerState(const QString &name, int flag, int state)
{
    if (!m_mLayers.contains(name))
        return;
    if (flag == DrawGlobal::LF_Lock) {
        m_mLayers[name]->setLock(flag & state);
    }
    else if (flag == DrawGlobal::LF_Hide) {
        m_mLayers[name]->setHide(flag & state);
    }
}

GraphicsObject * LayerManager::item(const QString &name, const QString &id)
{
    if (m_mLayers.contains(name))
        return m_mLayers[name]->item(id);
    return nullptr;
}

QList<GraphicsObject*> LayerManager::items(const QString &name)
{
    QList<GraphicsObject*> objs;
    if (m_mLayers.contains(name)) {
        objs = m_mLayers[name]->items().values();
    }
    return objs;
}

void LayerManager::clear()
{
    for (QSharedPointer<GraphicsLayer> layer : m_mLayers) {
        layer->clear();
    }
    m_mLayers.clear();
    DrawGlobal::ins()->clearLayers();
    addLayer("default");
}
