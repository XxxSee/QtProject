#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include <QCamera>
#include <QCameraImageCapture>
#include <QTimer>
#include <QImage>

namespace Ui {
class CameraDialog;
}

class CameraDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CameraDialog(QWidget *parent = nullptr);
    ~CameraDialog();
    QString codeText() { return m_dCodeText; }
private slots:
    void slotCameraStatusChanged(QCamera::Status status);
    void slotCameraStateChanged(QCamera::State state);
    void slotCameraImageCaptured(int id, const QImage &preview);
    void slotCaptureTimerTimeOut();
private:
    Ui::CameraDialog *ui;
    QCamera *m_pCamera;
    QCameraImageCapture *m_pCapture;
    QTimer *m_pCaptureTimer;
    QString m_dCodeText;
    int m_dTimeOut;
};

#endif // CAMERADIALOG_H
