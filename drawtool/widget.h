#ifndef WIDGET_H
#define WIDGET_H

#include "dll_global.h"
#include "rulewidget.h"
#include "graphicsview.h"
#include "graphicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public BaseWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QString dllName() override;
protected:
    void resizeEvent(QResizeEvent *) override;
private slots:
    void slotUpdateRule();
    void slotActionTriggered(int type, bool checked = false);
private:
    void handleFileOpen();
    void handleFileSave();
    void handleOutputPixmap();
    void handleZoomIn();
    void handleZoomOut();
    void handleDrawSet();
    void handleGraphicsProperty();
    void handleBackgroundSet();
    void handleUndo();
    void handleRedo();
    void handleResizeWgts();
private:
    Ui::Widget *ui;
    RuleWidget *m_pRuleWgt;
    GraphicsScene *m_pScene;
    GraphicsView *m_pView;
};
#endif // WIDGET_H
