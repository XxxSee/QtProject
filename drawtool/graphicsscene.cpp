#include "graphicsscene.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QFileDialog>
#include <QByteArray>
#include <QBuffer>
#include <QDomNodeList>
#include <QPainter>
#include "pathitem.h"
#include "pixmapitem.h"
#include "textitem.h"
#include "commands.h"
#include "drawglobal.h"

QString pixmapToBase64(QPixmap pix)
{
    QByteArray bytearray;
    QBuffer buffer(&bytearray);
    buffer.open(QIODevice::WriteOnly);
    pix.save(&buffer,"png");
    QByteArray img64 = bytearray.toBase64();//base64编码
    return img64.toStdString().c_str();
}

QPixmap base64ToPixmap(const QString &base64)
{
    QByteArray img = QByteArray::fromBase64(base64.toLocal8Bit());
    QPixmap pix;
    if (pix.loadFromData(img)) {
        return pix;
    }
    return QPixmap();
}

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    m_pUndoStack(new QUndoStack(this)),
    m_dDrawType(0),
    m_bDrawPressd(false),
    m_pDrawItem(nullptr),
    m_pLayerManager(new LayerManager(this,this))
{
    setBackgroundBrush(Qt::lightGray);
    m_pBackgroundItem = new QGraphicsPixmapItem();
    m_pBackgroundItem->setZValue(-99);
    addItem(m_pBackgroundItem);

    connect(DrawGlobal::ins(),&DrawGlobal::sigItemChanged,this,[&](QGraphicsItem *item, int type, QVariant last, QVariant now){
        m_pUndoStack->push(new ChangeItemCommand(item,type,last,now));
    });
    connect(DrawGlobal::ins(),&DrawGlobal::sigAddLayer,this,[&](const QString &name){
        //m_pLayerManager->addLayer(name);
        m_pUndoStack->push(new AddLayerCommand(m_pLayerManager,name));
    });
    connect(DrawGlobal::ins(),&DrawGlobal::sigRemoveLayer,this,[&](const QString &name){
        //m_pLayerManager->removeLayer(name);
        RemoveLayerCommand *removeLayerCmd = new RemoveLayerCommand(m_pLayerManager,name);
        QList<GraphicsObject*> items = m_pLayerManager->items(name);
        for (GraphicsObject *item : items) {
            removeLayerCmd->appendChild(new RemoveItemCommand(item,m_pLayerManager,removeLayerCmd));
        }
        m_pUndoStack->push(removeLayerCmd);
    });
    connect(DrawGlobal::ins(),&DrawGlobal::sigNowLayer,this,[&](const QString &name){
        m_pLayerManager->setCurrentLayer(name);
    });
    connect(DrawGlobal::ins(),&DrawGlobal::sigLayerStateChanged,this,[&](const QString &name, int flag, int state){
        m_pLayerManager->setLayerState(name,flag,state);
    });
}

GraphicsScene::~GraphicsScene()
{
    m_pLayerManager->clear();
}

void GraphicsScene::setBaseMapSize(int w, int h, QPixmap pix)
{
    if (pix.isNull()) {
        pix = QPixmap(w, h);
        pix.fill(Qt::white);
    }
    else {
        pix = pix.scaled(w,h,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }
    m_pBackgroundItem->setPixmap(pix);
    setSceneRect(QRectF(0,0,w,h));
}

void GraphicsScene::setUndo(bool bUndo)
{
    if (bUndo) {
        if (m_pUndoStack->canUndo()) {
            m_pUndoStack->undo();
        }
    }
    else {
        if (m_pUndoStack->canRedo()) {
            m_pUndoStack->redo();
        }
    }
}

void GraphicsScene::open(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << fileName << "open failed";
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        qDebug() << fileName << "setContent failed";
        return;
    }
    file.close();
    resetToInit();

    QDomElement root = doc.documentElement();
    QDomElement baseMap = root.elementsByTagName("BaseMap").at(0).toElement();
    if (baseMap.childNodes().size() == 3) {
        QString wStr = baseMap.elementsByTagName("width").at(0).childNodes().at(0).toText().nodeValue();
        QString hStr = baseMap.elementsByTagName("height").at(0).childNodes().at(0).toText().nodeValue();
        QString pixStr = baseMap.elementsByTagName("pixmap").at(0).childNodes().at(0).toText().nodeValue();
        QPixmap pix = base64ToPixmap(pixStr);
        setBaseMapSize(wStr.toInt(),hStr.toInt(),pix);
    }
    QDomElement layerEle = root.elementsByTagName("Layers").at(0).toElement();
    QDomNodeList layers = layerEle.childNodes();
    for (int i{0}; i<layers.size(); i++) {
        QString name = layers.at(i).childNodes().at(0).toText().nodeValue();
        m_pLayerManager->addLayer(name);
    }
    QDomElement itemsEle = root.elementsByTagName("Items").at(0).toElement();
    QDomNodeList itemEles = root.elementsByTagName("Item");
    for(int i{0}; i<itemEles.size(); i++) {
        QDomElement ele = itemEles.at(i).toElement();
        restoreItem(ele);
    }
}

void GraphicsScene::save(const QString &fileName)
{
    QDomDocument doc;
//    QDomProcessingInstruction instruction;
//    instruction = doc.createProcessingInstruction("xml","version=\"1.0\"encoding=\"UTF-8\"");
//    doc.appendChild(instruction);
    QDomElement root = doc.createElement("DrawProject");
    doc.appendChild(root);
    QDomElement baseMap = doc.createElement("BaseMap");
    root.appendChild(baseMap);
    QDomElement ele = doc.createElement("width");
    ele.appendChild(doc.createTextNode(QString::number(m_pBackgroundItem->pixmap().width())));
    baseMap.appendChild(ele);
    ele = doc.createElement("height");
    ele.appendChild(doc.createTextNode(QString::number(m_pBackgroundItem->pixmap().height())));
    baseMap.appendChild(ele);
    ele = doc.createElement("pixmap");
    ele.appendChild(doc.createTextNode(pixmapToBase64(m_pBackgroundItem->pixmap())));
    baseMap.appendChild(ele);
    QDomElement layersEle = doc.createElement("Layers");
    root.appendChild(layersEle);
    QList<QString> &layers = DrawGlobal::ins()->layers();
    for (QString lay : layers) {
        if (lay == "default")
            continue;
        ele = doc.createElement("name");
        ele.appendChild(doc.createTextNode(lay));
        layersEle.appendChild(ele);
    }
    QDomElement items = doc.createElement("Items");
    root.appendChild(items);
    saveItems(items,doc);
    QFile file(fileName);
    if (file.open(QIODevice::Truncate | QIODevice::ReadWrite)) {
        QTextStream out(&file);
        doc.save(out,4);
        file.close();
    }
}

void GraphicsScene::saveToPixmap(const QString &fileName)
{
    QImage image(m_pBackgroundItem->pixmap().size(),QImage::Format_RGB32);
    QPainter painter(&image);
    render(&painter);//关键函数
    image.save(fileName);
}

void GraphicsScene::slotGraphicsChanged(int id)
{
    if (m_dDrawType != id) {
        if (m_pDrawItem) {
            delete m_pDrawItem;
            m_pDrawItem = nullptr;
        }
        m_dDrawType = id;
        if (m_dDrawType == GraphicsObject::GT_Polygon) {
            m_dDrawPoly.clear();
        }
    }
}

void GraphicsScene::slotPenAndBrushChanged(const QPen &p, const QBrush &b)
{
    m_dPen = p;
    m_dBrush = b;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dDrawType == GraphicsObject::GT_Arrow) {
        return;
    }
    else if (m_dDrawType > GraphicsObject::GT_Edit) {
        if (m_pDrawItem) {
            delete m_pDrawItem;
            m_pDrawItem = nullptr;
        }
        m_pDrawItem = new QGraphicsPathItem;
        m_pDrawItem->setPen(m_dPen);
        m_pDrawItem->setBrush(m_dBrush);
        addItem(m_pDrawItem);
        m_bDrawPressd = true;
        //正常绘图中，屏蔽鼠标事件继续传递
        if (!paintGraphics(event)) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
    else
        QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dDrawType == GraphicsObject::GT_Arrow) {
        return;
    }
    else if (m_bDrawPressd) {
        if (!paintGraphics(event)) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
    else if (m_dDrawType == GraphicsObject::GT_Polygon) {
        if (!paintGraphics(event)) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dDrawType == (int)GraphicsObject::GT_Arrow) {
        return;
    }
    else if (m_bDrawPressd) {
        if (!paintGraphics(event)) {
            QGraphicsScene::mouseMoveEvent(event);
        }
        m_bDrawPressd = false;
    }
    else
        QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dDrawType == (int)GraphicsObject::GT_Arrow) {
        return;
    }
    else if (m_dDrawType == GraphicsObject::GT_Polygon) {
        QPainterPath path;
        path.addPolygon(m_dDrawPoly);
        path.closeSubpath();
        removeItem(m_pDrawItem);
        if (path.isEmpty())
            return;
        PathItem *item = new PathItem(path, GraphicsObject::GT_Polygon, m_pBackgroundItem);
        item->setPen(m_dPen);
        item->setBrush(m_dBrush);
        m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
        m_dDrawPoly.clear();
    }
    else
        QGraphicsScene::mouseDoubleClickEvent(event);
}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        QList<QGraphicsItem*> items = selectedItems();
        RemoveItemCommand *removeItemCmd = nullptr;
        if (items.size() > 0) {
            GraphicsObject *obj = dynamic_cast<GraphicsObject*>(items.front());
            if (obj) {
                removeItemCmd = new RemoveItemCommand(obj,m_pLayerManager);
            }
            items.pop_front();
        }
        for (QGraphicsItem *item : items) {
            GraphicsObject *obj = dynamic_cast<GraphicsObject*>(item);
            if (obj) {
                //m_pUndoStack->push(new RemoveItemCommand(obj,m_pLayerManager,removeItemCmd));
                removeItemCmd->appendChild(new RemoveItemCommand(obj,m_pLayerManager,removeItemCmd));
            }
        }
        m_pUndoStack->push(removeItemCmd);
    }
    else
        QGraphicsScene::keyPressEvent(event);
}

bool GraphicsScene::paintGraphics(QGraphicsSceneMouseEvent *event)
{
    static QPointF pressPos;
    if (!m_pDrawItem) {
        return false;
    }
    switch (m_dDrawType) {
    case GraphicsObject::GT_Line:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            pressPos = event->scenePos();
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QPainterPath path;
            path.moveTo(pressPos);
            path.lineTo(event->scenePos());
            m_pDrawItem->setPath(path);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            QPainterPath path;
            path.moveTo(pressPos);
            path.lineTo(event->scenePos());
            removeItem(m_pDrawItem);
            if (path.isEmpty())
                return true;
            PathItem *item = new PathItem(path,GraphicsObject::GT_Line,m_pBackgroundItem);
            item->setPen(m_dPen);
            item->setBrush(m_dBrush);
            m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
            return true;
        }
    }
        break;
    case GraphicsObject::GT_Curve:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            m_dDrawPath = QPainterPath();
            m_dDrawPath.moveTo(event->scenePos());
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            m_dDrawPath.lineTo(event->scenePos());
            m_pDrawItem->setPath(m_dDrawPath);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            removeItem(m_pDrawItem);
            if (m_dDrawPath.isEmpty())
                return true;
            PathItem *item = new PathItem(m_dDrawPath,GraphicsObject::GT_Curve,m_pBackgroundItem);
            item->setPen(m_dPen);
            item->setBrush(m_dBrush);
            m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
            return true;
        }
    }
        break;
    case GraphicsObject::GT_Rect:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            pressPos = event->scenePos();
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QPainterPath path;
            path.addRect(QRectF(pressPos, event->scenePos()));
            m_pDrawItem->setPath(path);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            QPainterPath path;
            path.addRect(QRectF(pressPos, event->scenePos()));
            removeItem(m_pDrawItem);
            if (path.isEmpty())
                return true;
            PathItem *item = new PathItem(path,GraphicsObject::GT_Rect,m_pBackgroundItem);
            item->setPen(m_dPen);
            item->setBrush(m_dBrush);
            m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
            return true;
        }
    }
        break;
    case GraphicsObject::GT_RoundedRect:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            pressPos = event->scenePos();
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QPainterPath path;
            path.addRoundedRect(QRectF(pressPos, event->scenePos()), 10, 10);
            m_pDrawItem->setPath(path);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            QPainterPath path;
            path.addRoundedRect(QRectF(pressPos, event->scenePos()), 10, 10);
            removeItem(m_pDrawItem);
            if (path.isEmpty())
                return true;
            PathItem *item = new PathItem(path,GraphicsObject::GT_RoundedRect,m_pBackgroundItem);
            item->setPen(m_dPen);
            item->setBrush(m_dBrush);
            m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
            return true;
        }
    }
        break;
    case GraphicsObject::GT_Ellipse:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            pressPos = event->scenePos();
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QPainterPath path;
            path.addEllipse(QRectF(pressPos, event->scenePos()));
            m_pDrawItem->setPath(path);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            QPainterPath path;
            path.addEllipse(QRectF(pressPos, event->scenePos()));
            removeItem(m_pDrawItem);
            if (path.isEmpty())
                return true;
            PathItem *item = new PathItem(path,GraphicsObject::GT_Ellipse,m_pBackgroundItem);
            item->setPen(m_dPen);
            item->setBrush(m_dBrush);
            m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
            return true;
        }
    }
        break;
    case GraphicsObject::GT_Circle:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            pressPos = event->scenePos();
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QPainterPath path;
            path.addEllipse(pressPos.x(), pressPos.y(), event->scenePos().x() - pressPos.x(), event->scenePos().x() - pressPos.x());
            m_pDrawItem->setPath(path);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            QPainterPath path;
            path.addEllipse(pressPos.x(), pressPos.y(), event->scenePos().x() - pressPos.x(), event->scenePos().x() - pressPos.x());
            removeItem(m_pDrawItem);
            if (path.isEmpty())
                return true;
            PathItem *item = new PathItem(path,GraphicsObject::GT_Circle,m_pBackgroundItem);
            item->setPen(m_dPen);
            item->setBrush(m_dBrush);
            m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
            return true;
        }
    }
        break;
    case GraphicsObject::GT_Polygon:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QPolygonF poly = m_dDrawPoly;
            poly.append(event->scenePos());
            QPainterPath path;
            path.addPolygon(poly);
            m_pDrawItem->setPath(path);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QPolygonF poly = m_dDrawPoly;
            poly.append(event->scenePos());
            QPainterPath path;
            path.addPolygon(poly);
            m_pDrawItem->setPath(path);
            return true;
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            m_dDrawPoly.append(event->scenePos());
            QPainterPath path;
            path.addPolygon(m_dDrawPoly);
            m_pDrawItem->setPath(path);
            return true;
        }
    }
        break;
    case GraphicsObject::GT_Pixmap:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QString imgPath = QFileDialog::getOpenFileName(nullptr,"open image", "./", "*.jpg;;*.jpeg;;*.png;;*.bmp");
            if (!imgPath.isEmpty()) {
                QPixmap pixmap;
                if (pixmap.load(imgPath)) {
                    PixmapItem *item = new PixmapItem(event->scenePos(),pixmap,m_pBackgroundItem);
                    m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
                    return true;
                }
            }
        }
    }
        break;
    case GraphicsObject::GT_Text:
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            TextItem *item = new TextItem(event->scenePos(),"please input",m_pBackgroundItem);
            m_pUndoStack->push(new AddItemCommand(item,m_pLayerManager));
            return true;
        }
    }
        break;
    default:break;
    }
    return false;
}

void GraphicsScene::resetToInit()
{
    m_pLayerManager->clear();
    m_pUndoStack->clear();
    m_dDrawType = 0;
    m_dPen = QPen();
    m_dBrush = QBrush();
}

void GraphicsScene::saveItems(QDomElement &parentEle, QDomDocument &doc)
{
    QList<QString> &layers = DrawGlobal::ins()->layers();
    for (QString lay : layers) {
        QList<GraphicsObject *> objs = m_pLayerManager->items(lay);
        for (GraphicsObject *obj : objs) {
            QDomElement ele = doc.createElement("Item");
            saveItem(obj,ele,doc);
            parentEle.appendChild(ele);
        }
    }
}

void GraphicsScene::saveItem(GraphicsObject *obj, QDomElement &parentEle, QDomDocument &doc)
{
    if (!obj)
        return;
    int type = obj->graphicsType();
    QDomElement ele = doc.createElement("type");
    ele.appendChild(doc.createTextNode(QString::number(type)));
    parentEle.appendChild(ele);
    ele = doc.createElement("layer");
    ele.appendChild(doc.createTextNode(obj->graphicsLayer()));
    parentEle.appendChild(ele);
    ele = doc.createElement("id");
    ele.appendChild(doc.createTextNode(obj->graphicsId()));
    parentEle.appendChild(ele);
    ele = doc.createElement("zvalue");
    ele.appendChild(doc.createTextNode(QString::number(obj->zValue(),'f',6)));
    parentEle.appendChild(ele);
    ele = doc.createElement("scale");
    ele.appendChild(doc.createTextNode(QString::number(obj->scale(),'f',6)));
    parentEle.appendChild(ele);
    ele = doc.createElement("rotation");
    ele.appendChild(doc.createTextNode(QString::number(obj->rotation(),'f',6)));
    parentEle.appendChild(ele);
    QDomElement posEle = doc.createElement("ItemPos");
    ele = doc.createElement("x");
    ele.appendChild(doc.createTextNode(QString::number(obj->pos().x(),'f',2)));
    posEle.appendChild(ele);
    ele = doc.createElement("y");
    ele.appendChild(doc.createTextNode(QString::number(obj->pos().y(),'f',2)));
    posEle.appendChild(ele);
    parentEle.appendChild(posEle);
    if (type == GraphicsObject::GT_Pixmap) {
        PixmapItem *item = dynamic_cast<PixmapItem*>(obj);
        if (item) {
            ele = doc.createElement("pixmap");
            ele.appendChild(doc.createTextNode(pixmapToBase64(item->pixmap())));
            parentEle.appendChild(ele);

        }
    }
    else if (type == GraphicsObject::GT_Text) {
        TextItem *item = dynamic_cast<TextItem*>(obj);
        if (item) {
            ele = doc.createElement("html");
            ele.appendChild(doc.createTextNode(item->html()));
            parentEle.appendChild(ele);
            ele = doc.createElement("text");
            ele.appendChild(doc.createTextNode(item->text()));
            parentEle.appendChild(ele);
        }
    }
    else {
        PathItem *item = dynamic_cast<PathItem*>(obj);
        if (item) {
            QDomElement penEle = doc.createElement("ItemPen");
            ele = doc.createElement("width");
            ele.appendChild(doc.createTextNode(QString::number(item->pen().widthF(),'f',2)));
            penEle.appendChild(ele);
            ele = doc.createElement("color");
            ele.appendChild(doc.createTextNode(item->pen().color().name(QColor::HexArgb)));
            penEle.appendChild(ele);
            ele = doc.createElement("style");
            ele.appendChild(doc.createTextNode(QString::number(item->pen().style())));
            penEle.appendChild(ele);
            parentEle.appendChild(penEle);
            QDomElement brushEle = doc.createElement("ItemBrush");
            ele = doc.createElement("color");
            ele.appendChild(doc.createTextNode(item->brush().color().name(QColor::HexArgb)));
            brushEle.appendChild(ele);
            ele = doc.createElement("style");
            ele.appendChild(doc.createTextNode(QString::number(item->brush().style())));
            brushEle.appendChild(ele);
            parentEle.appendChild(brushEle);
            QDomElement polyEle = doc.createElement("Polygon");
            QPolygonF poly = item->path().toFillPolygon();
            for (int i{0}; i<poly.size(); i++) {
                QDomElement pointEle = doc.createElement("Point");
                ele = doc.createElement("x");
                ele.appendChild(doc.createTextNode(QString::number(poly.at(i).x(),'f',2)));
                pointEle.appendChild(ele);
                ele = doc.createElement("y");
                ele.appendChild(doc.createTextNode(QString::number(poly.at(i).y(),'f',2)));
                pointEle.appendChild(ele);
                polyEle.appendChild(pointEle);
            }
            parentEle.appendChild(polyEle);
        }
    }
}

void GraphicsScene::restoreItem(QDomElement &parentEle)
{
    if (parentEle.isNull())
        return;
    int type = parentEle.elementsByTagName("type").at(0).childNodes().at(0).nodeValue().toInt();
    QString lay = parentEle.elementsByTagName("layer").at(0).childNodes().at(0).nodeValue();
    QString id = parentEle.elementsByTagName("id").at(0).childNodes().at(0).nodeValue();
    qreal zVal = parentEle.elementsByTagName("zvalue").at(0).childNodes().at(0).nodeValue().toFloat();
    qreal scaleVal = parentEle.elementsByTagName("scale").at(0).childNodes().at(0).nodeValue().toFloat();
    qreal rotationVal = parentEle.elementsByTagName("rotation").at(0).childNodes().at(0).nodeValue().toFloat();
    QDomElement itemPos = parentEle.elementsByTagName("ItemPos").at(0).toElement();
    qreal x = itemPos.elementsByTagName("x").at(0).childNodes().at(0).nodeValue().toFloat();
    qreal y = itemPos.elementsByTagName("y").at(0).childNodes().at(0).nodeValue().toFloat();
    if (type == GraphicsObject::GT_Pixmap) {
        QString pixStr = parentEle.elementsByTagName("pixmap").at(0).childNodes().at(0).toText().nodeValue();
        QPixmap pix = base64ToPixmap(pixStr);
        PixmapItem *item = new PixmapItem(QPointF(x,y),pix,m_pBackgroundItem);
        item->setGraphicsLayer(lay);
        item->setGraphicsId(id);
        item->setScale(scaleVal);
        item->setRotation(rotationVal);
        item->setZValue(zVal);
        m_pLayerManager->addItem(item);
    }
    else if (type == GraphicsObject::GT_Text) {
        QString text = parentEle.elementsByTagName("text").at(0).childNodes().at(0).toText().nodeValue();
        QString html = parentEle.elementsByTagName("html").at(0).childNodes().at(0).toText().nodeValue();
        TextItem *item = new TextItem(QPointF(x,y),text,m_pBackgroundItem);
        if (!html.isEmpty())
            item->setHtml(html);
        item->setGraphicsLayer(lay);
        item->setGraphicsId(id);
        item->setScale(scaleVal);
        item->setRotation(rotationVal);
        item->setZValue(zVal);
        m_pLayerManager->addItem(item);
    }
    else {
        QDomElement penEle = parentEle.elementsByTagName("ItemPen").at(0).toElement();
        qreal width = penEle.elementsByTagName("width").at(0).childNodes().at(0).toText().nodeValue().toFloat();
        QColor color(penEle.elementsByTagName("color").at(0).childNodes().at(0).toText().nodeValue());
        int style = penEle.elementsByTagName("style").at(0).childNodes().at(0).toText().nodeValue().toInt();
        QPen pen;
        pen.setWidthF(width);
        pen.setColor(color);
        pen.setStyle((Qt::PenStyle)style);
        QDomElement brushEle = parentEle.elementsByTagName("ItemBrush").at(0).toElement();
        color = QColor(brushEle.elementsByTagName("color").at(0).childNodes().at(0).toText().nodeValue());
        style = brushEle.elementsByTagName("style").at(0).childNodes().at(0).toText().nodeValue().toInt();
        QBrush brush;
        brush.setColor(color);
        brush.setStyle((Qt::BrushStyle)style);
        QDomElement polyEle = parentEle.elementsByTagName("Polygon").at(0).toElement();
        QDomNodeList pointEles = polyEle.childNodes();
        QPolygonF poly;
        for (int i{0}; i<pointEles.size(); i++) {
            QDomElement pointEle = pointEles.at(i).toElement();
            qreal x = pointEle.elementsByTagName("x").at(0).childNodes().at(0).nodeValue().toFloat();
            qreal y = pointEle.elementsByTagName("y").at(0).childNodes().at(0).nodeValue().toFloat();
            poly << QPointF(x,y);
        }
        QPainterPath path;
        path.addPolygon(poly);
        PathItem *item = new PathItem(path,(GraphicsObject::GraphicsType)type,m_pBackgroundItem);
        item->setPen(pen);
        item->setBrush(brush);
        item->setPos(QPointF(x,y));
        item->setGraphicsLayer(lay);
        item->setGraphicsId(id);
        item->setScale(scaleVal);
        item->setRotation(rotationVal);
        item->setZValue(zVal);
        m_pLayerManager->addItem(item);
    }
}
