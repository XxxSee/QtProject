#include "propertywidget.h"
#include "ui_propertywidget.h"
#include <QMetaEnum>
#include <QtTreePropertyBrowser>
#include <QtVariantPropertyManager>
#include <QDebug>
#include "graphicsobject.h"
#include "drawglobal.h"
#include "pixmapitem.h"
#include "textitem.h"
#include "pathitem.h"
#include "qteditorfactory.h"
#include "qtpropertymanager.h"

PropertyWidget::PropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyWidget),
    currentItem(nullptr)
{
    ui->setupUi(this);
    connect(DrawGlobal::ins(),&DrawGlobal::sigUpdateCurrentItem,this,&PropertyWidget::itemChanged);

    doubleManager = new QtDoublePropertyManager(this);
    stringManager = new QtStringPropertyManager(this);
    colorManager = new QtColorPropertyManager(this);
    fontManager = new QtFontPropertyManager(this);
    pointManager = new QtPointFPropertyManager(this);
    sizeManager = new QtSizePropertyManager(this);
    variantManager = new QtVariantPropertyManager(this);
    enumManager = new QtEnumPropertyManager(this);

    connect(doubleManager, SIGNAL(valueChanged(QtProperty *, double)),
                this, SLOT(valueChanged(QtProperty *, double)));
    connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
                this, SLOT(valueChanged(QtProperty *, const QString &)));
    connect(colorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)),
                this, SLOT(valueChanged(QtProperty *, const QColor &)));
    connect(fontManager, SIGNAL(valueChanged(QtProperty *, const QFont &)),
                this, SLOT(valueChanged(QtProperty *, const QFont &)));
    connect(pointManager, SIGNAL(valueChanged(QtProperty *, const QPointF &)),
                this, SLOT(valueChanged(QtProperty *, const QPointF &)));
    connect(sizeManager, SIGNAL(valueChanged(QtProperty *, const QSize &)),
                this, SLOT(valueChanged(QtProperty *, const QSize &)));
    connect(variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(valueChanged(QtProperty *, const QVariant &)));
    connect(enumManager, SIGNAL(valueChanged(QtProperty *, int)),
                this, SLOT(valueChanged(QtProperty *, int)));

    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
    QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
    QtColorEditorFactory *colorEditorFactory = new QtColorEditorFactory(this);
    QtFontEditorFactory *fontEditorFactory = new QtFontEditorFactory(this);
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);

    propertyEditor = new QtTreePropertyBrowser(this);
    ui->verticalLayout->addWidget(propertyEditor);
    propertyEditor->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
    propertyEditor->setFactoryForManager(stringManager, lineEditFactory);
    propertyEditor->setFactoryForManager(colorManager,colorEditorFactory);
    propertyEditor->setFactoryForManager(enumManager,comboBoxFactory);
    propertyEditor->setFactoryForManager(fontManager, fontEditorFactory);
    propertyEditor->setFactoryForManager(variantManager, variantFactory);
    propertyEditor->setFactoryForManager(colorManager->subIntPropertyManager(), spinBoxFactory);
    propertyEditor->setFactoryForManager(fontManager->subIntPropertyManager(), spinBoxFactory);
    propertyEditor->setFactoryForManager(fontManager->subBoolPropertyManager(), checkBoxFactory);
    propertyEditor->setFactoryForManager(fontManager->subEnumPropertyManager(), comboBoxFactory);
    propertyEditor->setFactoryForManager(pointManager->subDoublePropertyManager(), doubleSpinBoxFactory);
    propertyEditor->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
}

PropertyWidget::~PropertyWidget()
{
    delete ui;
}

void PropertyWidget::valueChanged(QtProperty *property, const QVariant &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("xpos")) {
        emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Move,currentItem->pos(),QPointF(value.toDouble(),currentItem->pos().y()));
        //currentItem->setX(value.toDouble());
    }
    else if (id == QLatin1String("ypos")) {
        emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Move,currentItem->pos(),QPointF(currentItem->pos().x(),value.toDouble()));
        //currentItem->setY(value.toDouble());
    }
    else if (id == QLatin1String("zpos")) {
        emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_ZValue,currentItem->zValue(),value.toDouble());
        //currentItem->setZValue(value.toDouble());
    }
    else if (id == QLatin1String("layer")) {
        //currentItem->setGraphicsLayer(value.toString());
    }
    else if (id == QLatin1String("scale")) {
        emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Scale,currentItem->scale(),value.toDouble());
        //currentItem->setScale(value.toDouble());
    }
    else if (id == QLatin1String("rotation")) {
        emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Rotate,currentItem->rotation(),value.toDouble());
        //currentItem->setRotation(value.toDouble());
    }
    else if (currentItem->graphicsType() == GraphicsObject::GT_Text) {
        TextItem *item = dynamic_cast<TextItem*>(currentItem);
        if (!item) {
            return;
        }
        else if (id == QLatin1String("text")) {
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Text,item->text(),value.toString());
            //item->setText(value.toString());
        }
        else if (id == QLatin1String("html")) {
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Html,item->html(),value.toString());
            //item->setHtml(value.toString());
        }
        else if (id == QLatin1String("font")) {
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Font,item->font(),value.value<QFont>());
            //item->setFont(value.value<QFont>());
        }
    } else if (currentItem->graphicsType() == GraphicsObject::GT_Pixmap) {
        PixmapItem *item = dynamic_cast<PixmapItem*>(currentItem);
        if (!item) {
            return;
        }
    } else {
        PathItem *item = dynamic_cast<PathItem*>(currentItem);
        if (!item) {
            return;
        }
        else if (id == QLatin1String("pencolor")) {
            QPen pen = item->pen();
            pen.setColor(value.value<QColor>());
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Pen,item->pen(),pen);
            //item->setPen(pen);
        }
        else if (id == QLatin1String("penwidth")) {
            QPen pen = item->pen();
            pen.setWidthF(value.toDouble());
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Pen,item->pen(),pen);
            //item->setPen(pen);
        }
        else if (id == QLatin1String("penstyle")) {
            QPen pen = item->pen();
            pen.setStyle((Qt::PenStyle)value.toInt());
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Pen,item->pen(),pen);
            //item->setPen(pen);
        }
        else if (id == QLatin1String("brushcolor")) {
            QBrush brush = item->brush();
            brush.setColor(value.value<QColor>());
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Brush,item->brush(),brush);
            //item->setBrush(brush);
        }
        else if (id == QLatin1String("brushstyle")) {
            QBrush brush = item->brush();
            brush.setStyle((Qt::BrushStyle)value.toInt());
            emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Brush,item->brush(),brush);
            //item->setBrush(brush);
        }
    }
    currentItem->update();
}

void PropertyWidget::valueChanged(QtProperty *, double )
{

}

void PropertyWidget::valueChanged(QtProperty *property, int value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("penstyle")) {
        PathItem *item = dynamic_cast<PathItem*>(currentItem);
        if (!item)
            return;
        QPen pen = item->pen();
        pen.setStyle((Qt::PenStyle)value);
        emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Pen,item->pen(),pen);
        //item->setPen(pen);
    }
    else if (id == QLatin1String("brushstyle")) {
        PathItem *item = dynamic_cast<PathItem*>(currentItem);
        if (!item)
            return;
        QBrush brush = item->brush();
        brush.setStyle((Qt::BrushStyle)value);
        emit DrawGlobal::ins()->sigItemChanged(currentItem,DrawGlobal::ICT_Brush,item->brush(),brush);
        //item->setBrush(brush);
    }
}

void PropertyWidget::valueChanged(QtProperty *, const QString &)
{

}

void PropertyWidget::valueChanged(QtProperty *, const QColor &)
{

}

void PropertyWidget::valueChanged(QtProperty *, const QFont &)
{

}

void PropertyWidget::valueChanged(QtProperty *, const QPointF &)
{

}

void PropertyWidget::valueChanged(QtProperty *, const QSize &)
{

}

void PropertyWidget::itemChanged(GraphicsObject *obj, bool selected)
{
    updateExpandState();

    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
    while (itProp != propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }

    propertyToId.clear();
    idToProperty.clear();
    currentItem = obj;
    if (!selected || !currentItem) {
        currentItem = nullptr;
        return;
    }

    QtVariantProperty *property;
    property = variantManager->addProperty(QVariant::Double, QStringLiteral("坐标X"));
    property->setAttribute(QLatin1String("minimum"), -2000);
    property->setAttribute(QLatin1String("maximum"), 2000);
    property->setValue(currentItem->x());
    addProperty(property, QLatin1String("xpos"));

    property = variantManager->addProperty(QVariant::Double, QStringLiteral("坐标Y"));
    property->setAttribute(QLatin1String("minimum"), -2000);
    property->setAttribute(QLatin1String("maximum"), 2000);
    property->setValue(currentItem->y());
    addProperty(property, QLatin1String("ypos"));

    property = variantManager->addProperty(QVariant::Double, QStringLiteral("坐标Z"));
    property->setAttribute(QLatin1String("minimum"), -2000);
    property->setAttribute(QLatin1String("maximum"), 2000);
    property->setValue(currentItem->zValue());
    addProperty(property, QLatin1String("zpos"));

    property = variantManager->addProperty(QVariant::String, QStringLiteral("图层"));
    property->setValue(currentItem->graphicsLayer());
    property->setEnabled(false);
    addProperty(property, QLatin1String("layer"));

    property = variantManager->addProperty(QVariant::Double, QStringLiteral("缩放"));
    property->setAttribute(QLatin1String("minimum"), 0.1);
    property->setAttribute(QLatin1String("maximum"), 20);
    property->setValue(currentItem->scale());
    addProperty(property, QLatin1String("scale"));

    property = variantManager->addProperty(QVariant::Double, QStringLiteral("旋转"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 360);
    property->setValue(currentItem->rotation());
    addProperty(property, QLatin1String("rotation"));

    if (currentItem->graphicsType() == GraphicsObject::GT_Pixmap) {
        PixmapItem *item = dynamic_cast<PixmapItem*>(currentItem);
        if (!item) {
            return;
        }
    }
    else if (currentItem->graphicsType() == GraphicsObject::GT_Text) {
        TextItem *item = dynamic_cast<TextItem*>(currentItem);
        if (!item) {
            return;
        }
        else {
            property = variantManager->addProperty(QVariant::String, QStringLiteral("文本"));
            property->setValue(item->text());
            addProperty(property, QLatin1String("text"));

            property = variantManager->addProperty(QVariant::String, QStringLiteral("html"));
            property->setValue(item->text());
            addProperty(property, QLatin1String("html"));

            property = variantManager->addProperty(QVariant::Font, QStringLiteral("字体"));
            property->setValue(item->font());
            addProperty(property, QLatin1String("font"));
        }
    }
    else {
        PathItem *item = dynamic_cast<PathItem*>(currentItem);
        if (!item) {
            return;
        }
        else {
            property = variantManager->addProperty(QVariant::Color, QStringLiteral("画笔颜色"));
            property->setValue(item->pen().color());
            addProperty(property, QLatin1String("pencolor"));

            property = variantManager->addProperty(QVariant::Double, QStringLiteral("画笔线宽"));
            property->setAttribute(QLatin1String("minimum"), 0);
            property->setAttribute(QLatin1String("maximum"), 20);
            property->setValue(item->pen().widthF());
            addProperty(property, QLatin1String("penwidth"));

            QtProperty *enumProperty = enumManager->addProperty(QStringLiteral("画笔样式"));
            QMetaEnum penM = QMetaEnum::fromType<Qt::PenStyle>();
            QStringList penStyle;
            for (int i{0}; i<penM.keyCount(); i++) {
                penStyle.append(penM.key(i));
            }
            enumManager->setEnumNames(enumProperty,penStyle);
            enumManager->setValue(enumProperty,item->pen().style());
            addProperty(enumProperty, QLatin1String("penstyle"));

            property = variantManager->addProperty(QVariant::Color, QStringLiteral("画刷颜色"));
            property->setValue(item->brush().color());
            addProperty(property, QLatin1String("brushcolor"));

            enumProperty = enumManager->addProperty(QStringLiteral("画刷样式"));
            QMetaEnum brushM = QMetaEnum::fromType<Qt::BrushStyle>();
            QStringList brushStyle;
            for (int i{0}; i<brushM.keyCount(); i++) {
                brushStyle.append(brushM.key(i));
            }
            enumManager->setEnumNames(enumProperty,brushStyle);
            enumManager->setValue(enumProperty,item->brush().style());
            addProperty(enumProperty, QLatin1String("brushstyle"));
        }
    }
}

void PropertyWidget::addProperty(QtProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    QtBrowserItem *item = propertyEditor->addProperty(property);
    if (idToExpanded.contains(id))
        propertyEditor->setExpanded(item, idToExpanded[id]);
}

void PropertyWidget::updateExpandState()
{
    QList<QtBrowserItem *> list = propertyEditor->topLevelItems();
    QListIterator<QtBrowserItem *> it(list);
    while (it.hasNext()) {
        QtBrowserItem *item = it.next();
        QtProperty *prop = item->property();
        idToExpanded[propertyToId[prop]] = propertyEditor->isExpanded(item);
    }
}
