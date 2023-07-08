#include "mediawidget.h"
#include "ui_mediawidget.h"
#include <QDebug>

MediaWidget::MediaWidget(int index, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MediaWidget)
{
    ui->setupUi(this);
    m_pVideo = new VideoDesktop(this);
    ui->stackedWidget->addWidget(m_pVideo);
    m_pGif = new GifDesktop(this);
    ui->stackedWidget->addWidget(m_pGif);
    m_pWeb = new WebDesktop(this);
    ui->stackedWidget->addWidget(m_pWeb);
    ui->stackedWidget->setCurrentIndex(index);
    connect(m_pVideo,&VideoDesktop::cancel,this,&MediaWidget::cancel);
    connect(m_pGif,&GifDesktop::cancel,this,&MediaWidget::cancel);
    connect(m_pWeb,&WebDesktop::cancel,this,&MediaWidget::cancel);
}

MediaWidget::~MediaWidget()
{
    delete ui;
}
