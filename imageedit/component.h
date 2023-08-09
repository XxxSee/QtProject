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
        case ctBegin:ret=QStringLiteral("��ʼ");break;
        case ctEnd:ret=QStringLiteral("����");break;
        case ctOpen:ret=QStringLiteral("��");break;
        case ctGray:ret=QStringLiteral("�ҶȻ�");break;
        case ctGrayHistogram:ret=QStringLiteral("�Ҷ�ֱ��ͼ");break;
        case ctGrayBalance:ret=QStringLiteral("�ҶȾ���");break;
        case ctGradsSharpen:ret=QStringLiteral("�ݶ���");break;
        case ctLaplaceSharpen:ret=QStringLiteral("Laplace��");break;
        case ctRobertsEdge:ret=QStringLiteral("Roberts\n��Ե���");break;
        case ctSobelEdge:ret=QStringLiteral("Sobel\n��Ե���");break;
        case ctLaplaceEdge:ret=QStringLiteral("Laplace\n��Ե���");break;
        case ctPrewittEdge:ret=QStringLiteral("Prewitt\n��Ե���");break;
        case ctCannyEdge:ret=QStringLiteral("Canny\n��Ե���");break;
        case ctKrischEdge:ret=QStringLiteral("Krisch\n��Ե���");break;
        case ctSaltNoise:ret=QStringLiteral("��������");break;
        case ctGuassNoise:ret=QStringLiteral("��˹����");break;
        case ctAverageFilter:ret=QStringLiteral("��ֵ�˲�");break;
        case ctMiddleFilter:ret=QStringLiteral("��ֵ�˲�");break;
        case ctWindowFilter:ret=QStringLiteral("�ߴ��˲�");break;
        case ctMorphologyFilter:ret=QStringLiteral("��̬ѧ�˲�");break;
        case ctGuassFilter:ret=QStringLiteral("��˹�˲�");break;
        case ctAffine:ret=QStringLiteral("����任");break;
        case ctPerspective:ret=QStringLiteral("͸�ӱ任");break;
        case ctThresholdSeg:ret=QStringLiteral("��ֵ�ָ�");break;
        case ctOSTU:ret=QStringLiteral("OSTU");break;
        case ctKittler:ret=QStringLiteral("Kittler");break;
        case ctFrameDiff:ret=QStringLiteral("֡����");break;
        case ctGuassMixture:ret=QStringLiteral("��˹��ϱ���");break;
        case ctCircleLBP:ret=QStringLiteral("LBP");break;
        case ctHistogramDet:ret=QStringLiteral("ֱ��ͼ���");break;
        case ctTemplateMatch:ret=QStringLiteral("ģ��ƥ��");break;
        case ctCloaking:ret=QStringLiteral("����Ч��");break;
        case ctColorMatch:ret=QStringLiteral("��ɫƥ��");break;
        case ctGarberFilter:ret=QStringLiteral("Garber�˲�");break;
        case ctSIFT:ret=QStringLiteral("SIFT");break;
        case ctORB:ret=QStringLiteral("ORB");break;
        case ctSVM:ret=QStringLiteral("�����SVM");break;
        case ctCharTest:ret=QStringLiteral("�ַ�����");break;
        case ctHaarVer:ret=QStringLiteral("Haar�㷨-��ֱ");break;
        case ctHaarHor:ret=QStringLiteral("Haar�㷨-ˮƽ");break;
        case cvHaarFace:ret=QStringLiteral("face-Haar");break;
        case ctCameraLock:ret=QStringLiteral("������궨");break;
        case ctSolidMatch:ret=QStringLiteral("����ƥ��");break;
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
