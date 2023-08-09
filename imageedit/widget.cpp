#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QEventLoop>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QFileInfo>
#include <QInputDialog>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/objdetect.hpp"
#include "flowchart.h"

using namespace cv;
using namespace std;

Widget::Widget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::Widget)
    , mRotation(0)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new FlowChart(this),QStringLiteral("流程图"));
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::dllName()
{
    return QString::fromLocal8Bit("图片编辑");
}

void Widget::on_openBtn_clicked()
{
    mSrcPath = QFileDialog::getOpenFileName(this,"open img","./","*.png;;*.jpg");
    if (mSrcPath.isEmpty())
            return;
    if (mSrcImg.load(mSrcPath)) {
        mShowImg = mSrcImg.scaled(ui->srcLab->width(), ui->srcLab->height(), \
                                  Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->srcLab->setPixmap(QPixmap::fromImage(mShowImg));
    }
}

void Widget::on_brightSpin_valueChanged(int)
{
    handleImage(htBrightContrast);
}

void Widget::on_contrastSpin_valueChanged(int)
{
    handleImage(htBrightContrast);
}

void Widget::on_clockwiseBtn_clicked()
{
    if (mSrcPath.isNull())
        return;
    mRotation += 90;
    QMatrix matrix;
    matrix.rotate(mRotation);
    mShowImg = mSrcImg.transformed(matrix,Qt::SmoothTransformation);
    mShowImg = mShowImg.scaled(ui->dstLab->width(), ui->dstLab->height(), \
                              Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->dstLab->setPixmap(QPixmap::fromImage(mShowImg));
}

void Widget::on_anticlockwiseBtn_clicked()
{
    if (mSrcPath.isNull())
        return;
    mRotation -= 90;
    QMatrix matrix;
    matrix.rotate(mRotation);
    mShowImg = mSrcImg.transformed(matrix,Qt::SmoothTransformation);
    mShowImg = mShowImg.scaled(ui->dstLab->width(), ui->dstLab->height(), \
                              Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->dstLab->setPixmap(QPixmap::fromImage(mShowImg));
}

void Widget::on_grayBtn_clicked()
{
    handleImage(htGray);
}

void Widget::on_snowBtn_clicked()
{
    handleImage(htSnow);
}

void Widget::on_guassBlurBtn_clicked()
{
    handleImage(htGuass);
}

void Widget::on_medianFilterBtn_clicked()
{
    handleImage(htMedian);
}

void Widget::on_groundGlassBtn_clicked()
{
    handleImage(htGroundGlass);
}

void Widget::on_XYBlurBtn_clicked()
{
    handleImage(htBlur);
}

void Widget::on_bilateralBlurBtn_clicked()
{
    handleImage(htBilateral);
}

void Widget::on_erodeBtn_clicked()
{
    handleImage(htErode);
}

void Widget::on_dilateBtn_clicked()
{
    handleImage(htDilate);
}

void Widget::on_contourBtn_clicked()
{
    if (mSrcPath.isNull())
        return;
    QEventLoop loop;
    int thresh = ui->threshSpin->value();
    QFuture<Mat> fut = QtConcurrent::run([&]()->Mat{
        Mat src, src_gray;
        RNG rng(12345);
        src = imread(mSrcPath.toLocal8Bit().data(), IMREAD_COLOR);
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        //threshold(src_gray, canny_output, thresh, 255, THRESH_BINARY);
        Canny(src_gray, canny_output, thresh, thresh*2, 3);
        findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for( size_t i = 0; i< contours.size(); i++ )
        {
          Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
          drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
        }
        return drawing;
    });

    QFutureWatcher<Mat> wat;
    connect(&wat,&QFutureWatcher<Mat>::finished,&loop,&QEventLoop::quit);
    wat.setFuture(fut);
    loop.exec();

    Mat ret = wat.result();
    QImage img = matToImage(ret);
    mShowImg = img.scaled(ui->dstLab->width(), ui->dstLab->height(), \
                              Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->dstLab->setPixmap(QPixmap::fromImage(mShowImg));
}

void Widget::on_mattingBtn_clicked()
{
    if (mSrcPath.isNull())
        return;
}

void Widget::on_faceBtn_clicked()
{
    if (mSrcPath.isNull())
        return;

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    Mat src, gray, dst;
    src = imread(mSrcPath.toLocal8Bit().data(), IMREAD_COLOR);
    vector<cv::Rect> faces;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);
    CascadeClassifier faceCascade;
    if (!faceCascade.load("D:/XxxSee/Source/QT/Dll/QtProject/imageedit/3part/opencv/opencv/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml")) {
        return;
    }
    faceCascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));//找出人脸
    for (size_t i=0; i<faces.size(); i++)
    {
        dst = gray(faces[i]);
        rectangle(src,faces[i],Scalar(0,0,255),2,8);
        break;
    }
    QString findKey;
    if(faces.size() > 0) {
        for (auto it=mImgInfo.begin(); it!=mImgInfo.end(); it++) {
            //对比当前人脸与已注册人脸的差值
            Mat comImg;
            cv::resize(it.value(), comImg, cv::Size(faces[0].width, faces[0].height)); // 确保两张图像尺寸相同
            cv::Mat diff;
            cv::absdiff(dst, comImg, diff);
            double similarity1 = cv::sum(diff)[0];

            //计算当前人脸与已注册人脸的直方图相似度
            cv::Mat currentFace = dst.clone();
            cv::Mat hist1,hist2;
            cv::calcHist(&currentFace, 1, 0, cv::Mat(), hist1, 1, &histSize, &histRange);
            cv::Mat currentCom = comImg.clone();
            cv::calcHist(&currentCom, 1, 0, cv::Mat(), hist2, 1, &histSize, &histRange);
            double similarity2 = cv::compareHist(hist1, hist2, cv::HISTCMP_CORREL);
            qDebug() << similarity1 << similarity2;
            //对比合适的阈值判断是否是同一人
            if (similarity2 > 0.2 && similarity1 < 20000000) {
                findKey = it.key();
                //cv::putText(src, it.key().toStdString(), cv::Point(faces[0].x,faces[0].y), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 3);
                break;
            }
        }
    }

    if (ui->faceInputChk->isChecked()) {
        QString name = QInputDialog::getText(this,"image name","please input:");
        if (!name.isEmpty()) {
            mImgInfo[name] = dst;
        }
    }
    QImage img = matToImage(src);
    if (!findKey.isEmpty()) {
        QPainter painter(&img);
        painter.setPen(Qt::green);
        painter.setFont(QFont("",40));
        painter.drawText(QPoint(faces[0].x,faces[0].y),findKey);
    }
    mShowImg = img.scaled(ui->dstLab->width(), ui->dstLab->height(), \
                              Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->dstLab->setPixmap(QPixmap::fromImage(mShowImg));

}


void Widget::handleImage(HandleType type)
{
    if (mSrcPath.isNull())
        return;
    int bright = ui->brightSpin->value();
    int contrast = ui->contrastSpin->value();
    int thresh = ui->threshSpin->value();
    int blurVal = (thresh/2)*2 + 1;
    qsrand(QDateTime::currentSecsSinceEpoch());
    QEventLoop loop;
    QFuture<Mat> fut = QtConcurrent::run([&]()->Mat{
        Mat src, dst;
        src = imread(mSrcPath.toLocal8Bit().data(), IMREAD_COLOR);
        if (type == htBrightContrast) {
            src.convertTo(dst,-1,bright/30.+1,contrast);
        }
        else if (type == htGray) {
            cvtColor(src,dst,COLOR_BGR2GRAY);
        }
        else if (type == htSnow) {
            int row = src.rows;
            int col = src.cols*src.channels();
            for (int i{0}; i<row; i++) {
                uchar * data = src.ptr<uchar>(i);
                for (int j{0}; j<col; j++) {
                    int q = rand()%col;
                    data[q]=thresh;
                }
            }
            dst = src;
        }
        else if (type == htGroundGlass) {
            RNG rng(12345);
            int random;
            int num = thresh;
            for(int i=0; i<src.rows-num; i++)
            {
                for(int j=0;j<src.cols-num; j++)
                {
                    random = rng.uniform(0,num);
                    src.at<Vec3b>(i,j)[0] = src.at<Vec3b>(i+random,j+random)[0];
                    src.at<Vec3b>(i,j)[1] = src.at<Vec3b>(i+random,j+random)[1];
                    src.at<Vec3b>(i,j)[2] = src.at<Vec3b>(i+random,j+random)[2];
                }
            }
            dst = src;
        }
        else if (type == htBlur) {
            blur(src,dst,Size(blurVal,blurVal));
        }
        else if (type == htGuass) {
            GaussianBlur(src,dst,Size(blurVal,blurVal),0,0);
        }
        else if (type == htMedian) {
            medianBlur(src,dst,blurVal);
        }
        else if (type == htBilateral) {
            bilateralFilter(src,dst,blurVal,blurVal*2,blurVal/2);
        }
        else if (type == htErode) {
            dst = getStructuringElement(MORPH_RECT,Size(thresh,thresh));
            erode(src,dst,dst);
        }
        else if (type == htDilate) {
            dst = getStructuringElement(MORPH_RECT,Size(thresh,thresh));
            dilate(src,dst,dst);
        }
        return dst;
    });
    QFutureWatcher<Mat> wat;
    connect(&wat,&QFutureWatcher<Mat>::finished,&loop,&QEventLoop::quit);
    wat.setFuture(fut);
    loop.exec();

    Mat ret = wat.result();

    QImage img = matToImage(ret);
    mShowImg = img.scaled(ui->dstLab->width(), ui->dstLab->height(), \
                              Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->dstLab->setPixmap(QPixmap::fromImage(mShowImg));
}

QImage Widget::matToImage(cv::Mat &t)
{
    if (t.channels() == 1) {
        return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_Indexed8);
    }
    else if (t.channels() == 3) {
        return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_RGB888).rgbSwapped();
    }
    else if (t.channels() == 4) {
        return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_ARGB32).rgbSwapped();
    }
    return QImage(t.data,t.cols,t.rows,t.step,QImage::Format_RGB888).rgbSwapped();
}

Mat Widget::imageToMat(QImage &img)
{
    if (img.format() == QImage::Format_Indexed8) {
        return Mat(img.height(),img.width(),CV_8UC1,img.bits());
    }
    else if (img.format() == QImage::Format_RGB888) {
        return Mat(img.height(),img.width(),CV_8UC3,img.bits());
    }
    else if (img.format() == QImage::Format_ARGB32) {
        return Mat(img.height(),img.width(),CV_8UC4,img.bits());
    }
    else if (img.format() == QImage::Format_RGB32) {
        return Mat(img.height(),img.width(),CV_8UC4,img.bits());
    }
    return Mat(img.height(),img.width(),CV_8UC3,img.bits(),img.bytesPerLine());
}

BaseWidget *createWidget()
{
    return new Widget;
}
