#include "toolwidget.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QStyle>
#include <QDebug>

ToolWidget::ToolWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setMargin(0);
    lay->setSpacing(0);
    m_pFileBar = new QToolBar(this);
    lay->addWidget(m_pFileBar);
    QIcon icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)42);
    QAction *action = m_pFileBar->addAction(icon,QStringLiteral("��"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_FileOpen);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)43);
    action = m_pFileBar->addAction(icon,QStringLiteral("����"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_FileSave);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)13);
    action = m_pFileBar->addAction(icon,QStringLiteral("���ͼƬ"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_OutputPixmap);

    m_pDrawBar = new QToolBar(this);
    lay->addWidget(m_pDrawBar);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)41);
    action = m_pDrawBar->addAction(icon,QStringLiteral("��ͼ����"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_BackgroundSet);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)35);
    action = m_pDrawBar->addAction(icon,QStringLiteral("�Ŵ�"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_ZoomIn);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)33);
    action = m_pDrawBar->addAction(icon,QStringLiteral("��С"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_ZoomOut);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)52);
    action = m_pDrawBar->addAction(icon,QStringLiteral("����"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_Undo);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)53);
    action = m_pDrawBar->addAction(icon,QStringLiteral("�ָ�����"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_Redo);

    m_pWgtBar = new QToolBar(this);
    lay->addWidget(m_pWgtBar);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)46);
    action = m_pWgtBar->addAction(icon,QStringLiteral("��ͼ����"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_DrawSet);
    action->setCheckable(true);
    icon = QApplication::style()->standardIcon((QStyle::StandardPixmap)36);
    action = m_pWgtBar->addAction(icon,QStringLiteral("ͼԪ����"),this,&ToolWidget::slotActionTriggered);
    action->setData(AT_GraphicsProperty);
    action->setCheckable(true);

    lay->addStretch(1);
}

void ToolWidget::addAction(QAction *action)
{
    m_pDrawBar->addAction(action);
}

void ToolWidget::slotActionTriggered(bool checked)
{
    QAction *action = (QAction*)sender();
    emit sigActionTriggered(action->data().value<ActionType>(), checked);
}
