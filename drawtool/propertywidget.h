#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>
#include <QMap>

class QtProperty;
class QtVariantProperty;
class GraphicsObject;

namespace Ui {
class PropertyWidget;
}

class PropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyWidget(QWidget *parent = nullptr);
    ~PropertyWidget();
private slots:
    void valueChanged(QtProperty *property, const QVariant &value);
    void valueChanged(QtProperty *property, double value);
    void valueChanged(QtProperty *property, int value);
    void valueChanged(QtProperty *property, const QString &value);
    void valueChanged(QtProperty *property, const QColor &value);
    void valueChanged(QtProperty *property, const QFont &value);
    void valueChanged(QtProperty *property, const QPointF &value);
    void valueChanged(QtProperty *property, const QSize &value);

    void itemChanged(GraphicsObject *obj, bool selected);
private:
    void addProperty(QtProperty *property, const QString &id);
    void updateExpandState();
private:
    Ui::PropertyWidget *ui;
    GraphicsObject *currentItem;

    class QtDoublePropertyManager *doubleManager;
    class QtStringPropertyManager *stringManager;
    class QtColorPropertyManager *colorManager;
    class QtFontPropertyManager *fontManager;
    class QtPointFPropertyManager *pointManager;
    class QtSizePropertyManager *sizeManager;
    class QtVariantPropertyManager *variantManager;
    class QtEnumPropertyManager *enumManager;
    class QtTreePropertyBrowser *propertyEditor;
    QMap<QtProperty *, QString> propertyToId;
    QMap<QString, QtProperty *> idToProperty;
    QMap<QString, bool> idToExpanded;
};

#endif // PROPERTYWIDGET_H
