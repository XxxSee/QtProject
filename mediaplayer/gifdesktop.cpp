#include "gifdesktop.h"
#include "ui_gifdesktop.h"
#include <QFileDialog>
#include <QDebug>

GifDesktop::GifDesktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GifDesktop)
{
    ui->setupUi(this);
    m_pMovie = new QMovie(this);
    m_pMovie->setCacheMode(QMovie::CacheAll);
    connect(m_pMovie,&QMovie::frameChanged,this,&GifDesktop::onMovieFrameChanged);
    connect(m_pMovie,&QMovie::frameChanged,ui->gifSlider,&SeekSlider::setValue);
}

GifDesktop::~GifDesktop()
{
    delete ui;
}

void GifDesktop::on_startBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(nullptr,"open gif","./","*.gif");
    if (path.isEmpty())
        return;
    m_pMovie->setFileName(path);
    ui->gifSlider->setRange(0,m_pMovie->frameCount());
    ui->gifSlider->setValue(0);
    m_pMovie->setScaledSize(ui->gifLab->size());
    m_pMovie->start();
}

void GifDesktop::on_pauseBtn_clicked()
{
    QMovie::MovieState state = m_pMovie->state();
    if (state == QMovie::NotRunning)
        return;
    else if (state == QMovie::Paused)
        m_pMovie->setPaused(false);
    else if (state == QMovie::Running)
        m_pMovie->setPaused(true);
}

void GifDesktop::on_stopBtn_clicked()
{
    m_pMovie->stop();
}

void GifDesktop::on_rateCbx_activated(const QString &text)
{
    //100%制,x1=100,x2=200
    qreal rate = text.toDouble();
    int speed = 100 * rate;
    m_pMovie->setSpeed(speed);
}

void GifDesktop::on_gifSlider_valueChanged(int value)
{
    disconnect(m_pMovie,&QMovie::frameChanged,ui->gifSlider,&SeekSlider::setValue);
    //如果QMovie不设置缓冲区setCacheMode(QMovie::CacheAll),jump会一直失败
    if (!m_pMovie->jumpToFrame(value)) {
        qDebug() << "jumpToFrame failed";
        m_pMovie->start();
    }
    connect(m_pMovie,&QMovie::frameChanged,ui->gifSlider,&SeekSlider::setValue);
}

void GifDesktop::on_desktopBtn_clicked()
{
    static bool bSet = false;
    //设置到desktop
    if (!bSet) {
        bSet = true;
        ui->wallpaperWgt->setDesktop(nullptr);
        ui->desktopBtn->setText(QStringLiteral("widget"));
    }
    //返回到widget
    else {
        bSet = false;
        ui->wallpaperWgt->setDesktop(ui->widget);
        ui->desktopBtn->setText(QStringLiteral("desktop"));
    }
}

void GifDesktop::onMovieFrameChanged(int frame)
{
    ui->gifSlider->setValue(frame);
    QPixmap pix = m_pMovie->currentPixmap();
    ui->gifLab->setPixmap(pix);
}

void GifDesktop::on_cancelBtn_clicked()
{
    emit cancel();
}
