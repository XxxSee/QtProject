#ifndef WIDGET_H
#define WIDGET_H

#include "dll_global.h"
#include <QImage>
#include <QMap>
#include "opencv2/core.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public BaseWidget
{
    Q_OBJECT
    enum HandleType {
        htNone,
        htBrightContrast,
        htGray,
        htSnow,
        htGroundGlass,
        htBlur,
        htGuass,
        htMedian,
        htBilateral,
        htErode,
        htDilate
    };
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QString dllName() override;
private slots:
    void on_openBtn_clicked();
    void on_brightSpin_valueChanged(int i);
    void on_contrastSpin_valueChanged(int i);
    void on_clockwiseBtn_clicked();
    void on_anticlockwiseBtn_clicked();
    void on_grayBtn_clicked();
    void on_snowBtn_clicked();
    void on_groundGlassBtn_clicked();
    void on_XYBlurBtn_clicked();
    void on_guassBlurBtn_clicked();
    void on_medianFilterBtn_clicked();
    void on_bilateralBlurBtn_clicked();
    void on_erodeBtn_clicked();
    void on_dilateBtn_clicked();

    void on_contourBtn_clicked();
    void on_mattingBtn_clicked();
    void on_faceBtn_clicked();
private:
    void handleImage(HandleType type);
    QImage matToImage(cv::Mat &t);
    cv::Mat imageToMat(QImage &img);
private:
    Ui::Widget *ui;
    QImage mSrcImg;
    QImage mShowImg;
    QString mSrcPath;
    QMap<QString,cv::Mat> mImgInfo;
    qreal mRotation;
};
#endif // WIDGET_H
