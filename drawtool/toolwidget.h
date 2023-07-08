#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>

enum ActionType {
    AT_None,AT_FileOpen,AT_FileSave,AT_OutputPixmap,AT_BackgroundSet,AT_ZoomIn,AT_ZoomOut,AT_Undo,AT_Redo,AT_DrawSet,AT_GraphicsProperty,
};
Q_DECLARE_METATYPE(ActionType);

class ToolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolWidget(QWidget *parent = nullptr);
    void addAction(QAction *action);
signals:
    void sigActionTriggered(int type, bool checked = false);
private slots:
    void slotActionTriggered(bool checked);
private:
    QToolBar *m_pFileBar;
    QToolBar *m_pDrawBar;
    QToolBar *m_pWgtBar;
};

#endif // TOOLWIDGET_H
