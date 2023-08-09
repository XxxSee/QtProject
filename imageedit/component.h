#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsItem>
#include <QString>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/objdetect.hpp"

using namespace cv;
namespace ct {
    enum ComponentType {ctNone=-1,ctLine=QGraphicsItem::UserType,ctImage,ctBegin,ctEnd,ctOpen,
                        ctGray,ctGrayHistogram,ctGrayBalance,ctGradsSharpen,ctLaplaceSharpen,
                        ctRobertsEdge,ctSobelEdge,ctLaplaceEdge,ctPrewittEdge,ctCannyEdge,ctKrischEdge,
                        ctSaltNoise,ctGuassNoise,
                        ctAverageFilter,ctMiddleFilter,ctWindowFilter,ctMorphologyFilter,ctGuassFilter,
                        ctAffine,ctPerspective,
                        ctThresholdSeg,ctOSTU,ctKittler,ctFrameDiff,ctGuassMixture,
                        ctCircleLBP,ctHistogramDet,ctTemplateMatch,ctCloaking,ctColorMatch,ctGarberFilter,
                        ctSIFT,ctORB,ctSVM,ctCharTest,ctHaarVer,ctHaarHor,cvHaarFace,
                        ctCameraLock,ctSolidMatch};
    static QString type2String(ComponentType ct)
    {
        QString ret;
        switch (ct) {
        case ctBegin:ret=QStringLiteral("开始");break;
        case ctEnd:ret=QStringLiteral("结束");break;
        case ctOpen:ret=QStringLiteral("打开");break;
        case ctGray:ret=QStringLiteral("灰度化");break;
        case ctGrayHistogram:ret=QStringLiteral("灰度直方图");break;
        case ctGrayBalance:ret=QStringLiteral("灰度均衡");break;
        case ctGradsSharpen:ret=QStringLiteral("梯度锐化");break;
        case ctLaplaceSharpen:ret=QStringLiteral("Laplace锐化");break;
        case ctRobertsEdge:ret=QStringLiteral("Roberts\n边缘检测");break;
        case ctSobelEdge:ret=QStringLiteral("Sobel\n边缘检测");break;
        case ctLaplaceEdge:ret=QStringLiteral("Laplace\n边缘检测");break;
        case ctPrewittEdge:ret=QStringLiteral("Prewitt\n边缘检测");break;
        case ctCannyEdge:ret=QStringLiteral("Canny\n边缘检测");break;
        case ctKrischEdge:ret=QStringLiteral("Krisch\n边缘检测");break;
        case ctSaltNoise:ret=QStringLiteral("椒盐噪声");break;
        case ctGuassNoise:ret=QStringLiteral("高斯噪声");break;
        case ctAverageFilter:ret=QStringLiteral("均值滤波");break;
        case ctMiddleFilter:ret=QStringLiteral("中值滤波");break;
        case ctWindowFilter:ret=QStringLiteral("边窗滤波");break;
        case ctMorphologyFilter:ret=QStringLiteral("形态学滤波");break;
        case ctGuassFilter:ret=QStringLiteral("高斯滤波");break;
        case ctAffine:ret=QStringLiteral("仿射变换");break;
        case ctPerspective:ret=QStringLiteral("透视变换");break;
        case ctThresholdSeg:ret=QStringLiteral("阈值分割");break;
        case ctOSTU:ret=QStringLiteral("OSTU");break;
        case ctKittler:ret=QStringLiteral("Kittler");break;
        case ctFrameDiff:ret=QStringLiteral("帧间差分");break;
        case ctGuassMixture:ret=QStringLiteral("高斯混合背景");break;
        case ctCircleLBP:ret=QStringLiteral("LBP");break;
        case ctHistogramDet:ret=QStringLiteral("直方图检测");break;
        case ctTemplateMatch:ret=QStringLiteral("模板匹配");break;
        case ctCloaking:ret=QStringLiteral("隐身效果");break;
        case ctColorMatch:ret=QStringLiteral("颜色匹配");break;
        case ctGarberFilter:ret=QStringLiteral("Garber滤波");break;
        case ctSIFT:ret=QStringLiteral("SIFT");break;
        case ctORB:ret=QStringLiteral("ORB");break;
        case ctSVM:ret=QStringLiteral("坐标点SVM");break;
        case ctCharTest:ret=QStringLiteral("字符测试");break;
        case ctHaarVer:ret=QStringLiteral("Haar算法-竖直");break;
        case ctHaarHor:ret=QStringLiteral("Haar算法-水平");break;
        case cvHaarFace:ret=QStringLiteral("face-Haar");break;
        case ctCameraLock:ret=QStringLiteral("摄像机标定");break;
        case ctSolidMatch:ret=QStringLiteral("立体匹配");break;
        default:break;
        }
        return ret;
    }

    class BasicItem : public QGraphicsItem
    {
    public:
        BasicItem(ComponentType ct, QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), mType(ct) {}
        virtual ~BasicItem() {}
        int type() const override { return mType; }
        virtual QPointF inScenePos() const = 0;
        virtual QPointF outScenePos() const = 0;
        virtual void addInItem(BasicItem *item) = 0;
        virtual void removeInItem() = 0;
        virtual void addOutItem(BasicItem *item) = 0;
        virtual void removeOutItem(BasicItem *item) = 0;
        virtual void exec(Mat mt = Mat()) {};
        virtual void resetItem() {}
    private:
        ComponentType mType;
    };
}
using namespace ct;
#endif // COMPONENT_H
