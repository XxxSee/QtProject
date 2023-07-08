#include "screenshotwgt.h"
#include "ui_screenshotwgt.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QDebug>
#include <QHBoxLayout>
#include <QClipboard>
#include <QFileDialog>
#include "rectwidget.h"
#include "irrewidget.h"

ScreenShotWgt::ScreenShotWgt(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ScreenShotWgt)
{
    ui->setupUi(this);
}

ScreenShotWgt::~ScreenShotWgt()
{
    delete ui;
}

QString ScreenShotWgt::dllName()
{
    return QString::fromLocal8Bit("��ͼ");
}

void ScreenShotWgt::on_fullBtn_clicked()
{
    setMainWidgetVisible(false);
    m_dPixmap = QApplication::primaryScreen()->grabWindow(0);
    ui->imgLab->setPixmap(m_dPixmap);
    setMainWidgetVisible(true);
}

void ScreenShotWgt::on_rectBtn_clicked()
{
    setMainWidgetVisible(false);
    RectWidget *w = new RectWidget;
    w->showFullScreen();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w,&RectWidget::grabRect,this,[=](const QRect &rect){
        w->setWindowOpacity(0);
        m_dPixmap = QApplication::primaryScreen()->grabWindow(0,rect.x(),rect.y(),rect.width(),rect.height());
        ui->imgLab->setPixmap(m_dPixmap);
        setMainWidgetVisible(true);
    });
}

void ScreenShotWgt::on_irreBtn_clicked()
{
    setMainWidgetVisible(false);
    IrreWidget *w = new IrreWidget;
    w->showFullScreen();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w,&IrreWidget::grabPolygon,this,[=](const QPolygon &poly){
        w->setWindowOpacity(0);
        handleIrreScreenShot(poly);
    });
}

void ScreenShotWgt::on_copyBtn_clicked()
{
    if (m_dPixmap.isNull())
        return;
    QClipboard *clip = QApplication::clipboard();
    clip->setPixmap(m_dPixmap);
}

void ScreenShotWgt::on_saveBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,"save","./","*.jpg;;*.jpeg;;*.png;;*.bmp");
    if (fileName.isEmpty())
        return;
    m_dPixmap.save(fileName);
}

void ScreenShotWgt::setMainWidgetVisible(bool visible)
{
    if (!ui->hideChk->isChecked())
        return;
    QWidgetList topLevelWidgets = qApp->topLevelWidgets();
    QWidget *appWidget = nullptr;
    for (auto w : topLevelWidgets) {
        if (w->objectName() == "MainWidget") {
            appWidget = w;
            break;
        }
    }
    if (appWidget) {
         visible ? appWidget->setWindowOpacity(1)
                 : appWidget->setWindowOpacity(0);
    }
    else {
        appWidget = topLevelWidgets.first();
        visible ? appWidget->setWindowOpacity(1)
                : appWidget->setWindowOpacity(0);
    }
}

void ScreenShotWgt::handleIrreScreenShot(const QPolygon &poly)
{
    QRegion region(poly);
    QPixmap pix = QApplication::primaryScreen()->grabWindow(0);
    //��Ļ����
    QRect geo = QApplication::primaryScreen()->availableGeometry();
    QWidget *pw = new QWidget;
    pw->setAttribute(Qt::WA_DeleteOnClose);
    pw->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    //����͸�����ӿؼ�������ʾ
    pw->setAttribute(Qt::WA_TranslucentBackground);
    pw->setGeometry(geo);
    QWidget *w = new QWidget(pw);
    w->setGeometry(geo);
    w->setAutoFillBackground(true);
    //���ñ���ͼ
    QPalette pal;
    pal.setBrush(QPalette::Background,QBrush(pix));
    w->setPalette(pal);
    //ֻ����region���֣���������͸��
    w->setMask(region);
    m_dPixmap = pw->grab(region.boundingRect());
    pw->close();
    ui->imgLab->setPixmap(m_dPixmap);
    setMainWidgetVisible(true);
}

BaseWidget* createWidget() {
    return new ScreenShotWgt();
}
