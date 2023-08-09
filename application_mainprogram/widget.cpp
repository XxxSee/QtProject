#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include "dll_global.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_bBtnWgtShow(true)
{
    ui->setupUi(this);
    initWidget();
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *o, QEvent *e)
{
    if (o == ui->btnWgt) {
        if (e->type() == QEvent::Enter) {
            ui->visibleBtn->setVisible(true);
        }
        else if (e->type() == QEvent::Leave) {
            ui->visibleBtn->setVisible(false);
        }
    }
    return QWidget::eventFilter(o, e);
}

void Widget::resizeEvent(QResizeEvent *e)
{
    if (m_bBtnWgtShow) {
        ui->btnWgt->setGeometry(0, 0, 120, rect().height());
        ui->dllWgt->setGeometry(120, 0, rect().width()-120, rect().height());
        ui->visibleBtn->setGeometry(120-15, (ui->btnWgt->rect().top() + ui->btnWgt->rect().height())/2 - 30, 12, 60);
    }
    else {
        ui->btnWgt->setGeometry(0, 0, 15, rect().height());
        ui->dllWgt->setGeometry(15, 0, rect().width()-15, rect().height());
        ui->visibleBtn->setGeometry(3, (ui->btnWgt->rect().top() + ui->btnWgt->rect().height())/2 - 30, 12, 60);
    }
    QWidget::resizeEvent(e);
}

void Widget::initWidget()
{
    setObjectName("MainWidget");
    //°´Å¥Òþ²Ø/ÏÔÊ¾Âß¼­
    ui->visibleBtn->setVisible(false);
    ui->btnWgt->installEventFilter(this);
    QVBoxLayout *vlay = new QVBoxLayout(ui->btnWgt);
    vlay->setContentsMargins(15,15,0,15);
    m_pBtnGrp = new QButtonGroup(this);
    m_pStackedLay = new QStackedLayout(ui->dllWgt);
    void (QButtonGroup::*pFun)(int) = &QButtonGroup::buttonClicked;
    connect(m_pBtnGrp,pFun,m_pStackedLay,&QStackedLayout::setCurrentIndex);
    QTimer::singleShot(0,this,&Widget::loadPlugin);

    m_pAniGrp1 = new QParallelAnimationGroup(this);
    m_pAniGrp2 = new QParallelAnimationGroup(this);
}

void Widget::loadPlugin()
{
    QDir dir("thirdplugin");
    QFileInfoList files = dir.entryInfoList(QStringList() << "*.dll", QDir::Files, QDir::Name);
    bool bLoad = false;
    QVBoxLayout *vlay = dynamic_cast<QVBoxLayout*>(ui->btnWgt->layout());
    for (auto file : files) {
        QLibrary *pLib = new QLibrary(file.absoluteFilePath(), this);
        if (pLib->load()){
            using FunType = BaseWidget* (*)();
            FunType createWgt = (FunType)pLib->resolve("createWidget");
            if (createWgt) {
                BaseWidget *w = createWgt();
                if (w) {
                    m_mLibs[w->dllName()] = pLib;
                    int idx = m_pStackedLay->addWidget(w);
                    m_mWgts[w->dllName()] = idx;
                    QPushButton *btn = new QPushButton(w->dllName(), ui->btnWgt);
                    vlay->addWidget(btn);
                    m_pBtnGrp->addButton(btn,idx);
                    bLoad = true;
                }
            }
        }
        if (!bLoad) {
            pLib->deleteLater();
            pLib = nullptr;
        }
        bLoad = false;
        QApplication::processEvents();
    }
    vlay->addStretch(1);
}

void Widget::on_visibleBtn_clicked()
{
    //ui->btnWgt->setVisible(!ui->btnWgt->isVisible());
    if (m_bBtnWgtShow) {
        m_pAniGrp1->clear();
        QPropertyAnimation *ani = new QPropertyAnimation(ui->btnWgt,"geometry",m_pAniGrp1);
        ani->setStartValue(QRect(0, 0, 120, rect().height()));
        ani->setEndValue(QRect(0, 0, 15, rect().height()));
        ani->setDuration(1000);
        ani->setEasingCurve(QEasingCurve::InOutQuint);
        m_pAniGrp1->addAnimation(ani);
        ani = new QPropertyAnimation(ui->dllWgt,"geometry",m_pAniGrp1);
        ani->setStartValue(QRect(120, 0, rect().width()-120, rect().height()));
        ani->setEndValue(QRect(15, 0, rect().width()-15, rect().height()));
        ani->setDuration(1000);
        ani->setEasingCurve(QEasingCurve::InOutQuint);
        m_pAniGrp1->addAnimation(ani);
        ani = new QPropertyAnimation(ui->visibleBtn,"geometry",m_pAniGrp1);
        ani->setStartValue(QRect(120-15, (ui->btnWgt->rect().top() + ui->btnWgt->rect().height())/2 - 30, 12, 60));
        ani->setEndValue(QRect(3, (ui->btnWgt->rect().top() + ui->btnWgt->rect().height())/2 - 30, 12, 60));
        ani->setDuration(1000);
        ani->setEasingCurve(QEasingCurve::InOutQuint);
        m_pAniGrp1->addAnimation(ani);

        m_pAniGrp1->start();
        m_bBtnWgtShow = false;
    }
    else {
        m_pAniGrp2->clear();
        QPropertyAnimation *ani = new QPropertyAnimation(ui->btnWgt,"geometry",m_pAniGrp2);
        ani->setStartValue(QRect(0, 0, 15, rect().height()));
        ani->setEndValue(QRect(0, 0, 120, rect().height()));
        ani->setDuration(1000);
        ani->setEasingCurve(QEasingCurve::InOutQuint);
        m_pAniGrp2->addAnimation(ani);
        ani = new QPropertyAnimation(ui->dllWgt,"geometry",m_pAniGrp2);
        ani->setStartValue(QRect(15, 0, rect().width()-15, rect().height()));
        ani->setEndValue(QRect(120, 0, rect().width()-120, rect().height()));
        ani->setDuration(1000);
        ani->setEasingCurve(QEasingCurve::InOutQuint);
        m_pAniGrp2->addAnimation(ani);
        ani = new QPropertyAnimation(ui->visibleBtn,"geometry",m_pAniGrp2);
        ani->setStartValue(QRect(3, (ui->btnWgt->rect().top() + ui->btnWgt->rect().height())/2 - 30, 12, 60));
        ani->setEndValue(QRect(120-15, (ui->btnWgt->rect().top() + ui->btnWgt->rect().height())/2 - 30, 12, 60));
        ani->setDuration(1000);
        ani->setEasingCurve(QEasingCurve::InOutQuint);
        m_pAniGrp2->addAnimation(ani);

        m_pAniGrp2->start();
        m_bBtnWgtShow = true;
    }
}

