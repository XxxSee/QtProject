#include "cameradialog.h"
#include "ui_cameradialog.h"
#include "QZXing.h"

CameraDialog::CameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraDialog),
    m_dTimeOut(0)
{
    ui->setupUi(this);
    m_pCamera = new QCamera(this);
    connect(m_pCamera,&QCamera::statusChanged,this,&CameraDialog::slotCameraStatusChanged);
    connect(m_pCamera,&QCamera::stateChanged,this,&CameraDialog::slotCameraStateChanged);
    m_pCamera->setCaptureMode(QCamera::CaptureVideo);
    m_pCamera->setViewfinder(ui->widget);
    m_pCapture = new QCameraImageCapture(m_pCamera);
    connect(m_pCapture,&QCameraImageCapture::imageCaptured,this,&CameraDialog::slotCameraImageCaptured);
    m_pCaptureTimer = new QTimer(this);
    connect(m_pCaptureTimer,&QTimer::timeout,this,&CameraDialog::slotCaptureTimerTimeOut);
    m_pCamera->start();
}

CameraDialog::~CameraDialog()
{
    delete ui;
}

void CameraDialog::slotCameraStatusChanged(QCamera::Status status)
{
    if (status == QCamera::ActiveStatus) {
        m_pCaptureTimer->start(100);
    }
}

void CameraDialog::slotCameraStateChanged(QCamera::State state)
{
    Q_UNUSED(state)
}

void CameraDialog::slotCameraImageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id)
    //QImage image = preview.scaled(width(),height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QImage image = preview;
    //开启横向镜像后无法识别
    //image = image.mirrored(true, false);

    QZXing decoder;
    //必要设置
    decoder.setDecoder(QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_CODE_128);
    //可选设置
    //decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    //decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);
    m_dCodeText = decoder.decodeImage(image);
    if (m_dCodeText.isEmpty()) {
        m_dTimeOut++;
        if (m_dTimeOut > 100) {
            reject();
        }
    }
    else {
        accept();
    }
}

void CameraDialog::slotCaptureTimerTimeOut()
{
    if (m_pCapture->isAvailable())
        m_pCapture->capture();
}
