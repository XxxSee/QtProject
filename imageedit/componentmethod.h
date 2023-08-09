#ifndef COMPONENTMETHOD_H
#define COMPONENTMETHOD_H

#include "component.h"

class BasicMethod
{
public:
    BasicMethod() {}
};

template<ComponentType CT>
class ComponentMethod : public BasicMethod
{
public:
    void invokeMethod(Mat , Mat ) {}
};

template<>
class ComponentMethod<ctOpen> : public BasicMethod
{
public:
    QString path;
    void invokeMethod(Mat &src) {
        if (!path.isEmpty())
            src = cv::imread(path.toLocal8Bit().data(), cv::IMREAD_COLOR);
    }
};

template<>
class ComponentMethod<ctGrayHistogram> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        QVector<int> pixel(256,0);

        for(int i = 0 ; i < src.rows ; i++)
            for(int j = 0 ; j < src.cols ; j++){
                pixel[src.at<uchar>(i,j)]++;
            }

        Mat gray_level;
        gray_level.create(350, 256, CV_8UC1);

        int max_rows = 0;
        for(int i = 0;i <= 255;i++){
            if(pixel[i] > max_rows){
                max_rows = pixel[i];
            }
        }

        for(int i = 0;i < 256;i++){
            for(int j = 0;j < 350 ; j++){
                gray_level.at<uchar>(j,i) = 255;
            }
        }

        for(int i = 0;i < 256;i++){
            for(int j = 0;j < 350 - int(320.*float(pixel[i])/float(max_rows)) ; j++){
                gray_level.at<uchar>(j,i) = 0;
            }
        }
        dst = gray_level;
    }
};

template<>
class ComponentMethod<ctGrayBalance> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        QVector<int> pixel(256,0);
        QVector<float> pixel_gray(256,0.);
        float sum=0;
        Mat srcImg = src;
        for(int i = 0 ; i < srcImg.rows ; i++)
           for(int j = 0 ; j < srcImg.cols ; j++){
               pixel[srcImg.at<uchar>(i,j)]++;
           }

        for(int i = 0 ; i < pixel.size() ; i++){
           sum += pixel[i];
        }

        for(int i = 0 ; i < 256 ; i++){
           float num = 0;
           for(int j = 0 ; j <= i ; j++){
               num += pixel[j];
           }
           pixel_gray[i] = 255*num/sum;
        }
        Mat balance;
        balance.create(srcImg.rows, srcImg.cols, CV_8UC1);
        dst.create(srcImg.rows, srcImg.cols, CV_8UC1);
        for(int i = 0 ; i < src.rows ; i++)
           for(int j = 0 ; j < src.cols ; j++){
               balance.at<uchar>(i,j) = pixel_gray[dst.at<uchar>(i,j)];
           }
        dst = balance;
    }
};

template<>
class ComponentMethod<ctGradsSharpen> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        Mat grad,gray2Img;
        gray2Img.create(src.rows, src.cols, CV_8UC1);
        QImage Qtemp,Qtemp2;
        grad.create(gray2Img.rows, gray2Img.cols, CV_8UC1);
        for(int i = 0 ; i < gray2Img.rows - 1 ; i++)
            for(int j = 0 ; j < gray2Img.cols - 1 ; j++){
                grad.at<uchar>(i,j) = saturate_cast<uchar>(max(fabs(src.at<uchar>(i+1, j)-src.at<uchar>(i,j)),fabs(src.at<uchar>(i, j+1)-src.at<uchar>(i,j))));
                gray2Img.at<uchar>(i,j) = saturate_cast<uchar>(src.at<uchar>(i,j) - grad.at<uchar>(i,j));
            }
        dst = gray2Img;
    }
};

template<>
class ComponentMethod<ctLaplaceSharpen> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        Mat gradimg,gray2Img;
        gray2Img.create(src.rows, src.cols, CV_8UC1);
        gradimg.create(src.rows, src.cols, CV_8UC1);
        for (int i = 1; i < src.rows - 1; i++)
        {
           for (int j = 1; j < src.cols - 1; j++)
           {
               gradimg.at<uchar>(i, j) = saturate_cast<uchar>(- 4 * src.at<uchar>(i, j) + src.at<uchar>(i + 1 , j)
                                                             + src.at<uchar>(i, j + 1) + src.at<uchar>(i - 1, j)
                                                             + src.at<uchar>(i, j - 1));
               gray2Img.at<uchar>(i, j) = saturate_cast<uchar>(5 * src.at<uchar>(i, j) - src.at<uchar>(i + 1, j)
                                                           - src.at<uchar>(i, j + 1) - src.at<uchar>(i - 1, j)
                                                           - src.at<uchar>(i, j - 1));
           }
        }
        dst = gray2Img;
    }
};

template<>
class ComponentMethod<ctGray> : public BasicMethod
{
public:
    ComponentType type = ctGray;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctGray) {
            cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
        }
        else if (type == ctGrayHistogram) {
            ComponentMethod<ctGrayHistogram> method;
            method.invokeMethod(src,dst);
        }
        else if (type == ctGrayBalance) {
            ComponentMethod<ctGrayBalance> method;
            method.invokeMethod(src,dst);
        }
        else if (type == ctGradsSharpen) {
            ComponentMethod<ctGradsSharpen> method;
            method.invokeMethod(src,dst);
        }
        else if (type == ctLaplaceSharpen) {
            ComponentMethod<ctLaplaceSharpen> method;
            method.invokeMethod(src,dst);
        }
    }
};

template<>
class ComponentMethod<ctSobelEdge> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        Mat gradimg,gray2Img,f_x,f_y;
        QImage Qtemp,Qtemp2;
        gray2Img.create(src.rows, src.cols, CV_8UC1);
        gradimg.create(src.rows, src.cols, CV_8UC1);
        f_x.create(src.rows, src.cols, CV_8UC1);
        f_y.create(src.rows, src.cols, CV_8UC1);
        for (int i = 1; i < src.rows - 1; i++)
        {
           for (int j = 1; j < src.cols - 1; j++)
           {
               f_x.at<uchar>(i, j) = saturate_cast<uchar>(fabs(src.at<uchar>(i + 1, j - 1) + 2*src.at<uchar>(i + 1, j) + src.at<uchar>(i + 1, j + 1)
                                                               - src.at<uchar>(i - 1, j - 1) - 2*src.at<uchar>(i - 1, j) - src.at<uchar>(i - 1, j + 1)));
               f_y.at<uchar>(i, j) = saturate_cast<uchar>(fabs(src.at<uchar>(i - 1, j + 1) + 2*src.at<uchar>(i, j + 1) + src.at<uchar>(i + 1, j + 1)
                                                               - src.at<uchar>(i - 1, j - 1) - 2*src.at<uchar>(i, j - 1) - src.at<uchar>(i + 1, j - 1)));
               gradimg.at<uchar>(i, j) = f_x.at<uchar>(i, j) + f_y.at<uchar>(i, j);
               gray2Img.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
           }
        }
        dst = gray2Img;
    }
};

template<>
class ComponentMethod<ctLaplaceEdge> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        Mat gradimg,gray2Img;
        gray2Img.create(src.rows, src.cols, CV_8UC1);
        gradimg.create(src.rows, src.cols, CV_8UC1);
        for (int i = 1; i < src.rows - 1; i++)
        {
            for (int j = 1; j < src.cols - 1; j++)
            {
                gradimg.at<uchar>(i, j) = saturate_cast<uchar>(- 4 * src.at<uchar>(i, j) + src.at<uchar>(i + 1 , j)
                                                              + src.at<uchar>(i, j + 1) + src.at<uchar>(i - 1, j)
                                                              + src.at<uchar>(i, j - 1));
                gray2Img.at<uchar>(i, j) = saturate_cast<uchar>(5 * src.at<uchar>(i, j) - src.at<uchar>(i + 1, j)
                                                            - src.at<uchar>(i, j + 1) - src.at<uchar>(i - 1, j)
                                                            - src.at<uchar>(i, j - 1));
            }
            dst = gray2Img;
        }
    }
};

template<>
class ComponentMethod<ctPrewittEdge> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        Mat gradimg,gray2Img,f_x,f_y;
        gray2Img.create(src.rows, src.cols, CV_8UC1);
        gradimg.create(src.rows, src.cols, CV_8UC1);
        f_x.create(src.rows, src.cols, CV_8UC1);
        f_y.create(src.rows, src.cols, CV_8UC1);
        for (int i = 1; i < src.rows - 1; i++)
        {
            for (int j = 1; j < src.cols - 1; j++)
            {
                f_x.at<uchar>(i, j) = saturate_cast<uchar>(fabs(src.at<uchar>(i + 1, j - 1) + src.at<uchar>(i + 1, j) + src.at<uchar>(i + 1, j + 1)
                                                                - src.at<uchar>(i - 1, j - 1) - src.at<uchar>(i - 1, j) - src.at<uchar>(i - 1, j + 1)));
                f_y.at<uchar>(i, j) = saturate_cast<uchar>(fabs(src.at<uchar>(i - 1, j + 1) + src.at<uchar>(i, j + 1) + src.at<uchar>(i + 1, j + 1)
                                                                - src.at<uchar>(i - 1, j - 1) - src.at<uchar>(i, j - 1) - src.at<uchar>(i + 1, j - 1)));
                gradimg.at<uchar>(i, j) = max(f_x.at<uchar>(i, j),f_y.at<uchar>(i, j));
                gray2Img.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
            }
        }
        dst = gray2Img;
    }
};

template<>
class ComponentMethod<ctCannyEdge> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctKrischEdge> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {
        Mat gradimg,gray2Img,f_1,f_2,f_3,f_4,f_5,f_6,f_7,f_8;
        gray2Img.create(src.rows, src.cols, CV_8UC1);
        gradimg.create(src.rows, src.cols, CV_8UC1);
        f_1.create(src.rows, src.cols, CV_8UC1);
        f_2.create(src.rows, src.cols, CV_8UC1);
        f_3.create(src.rows, src.cols, CV_8UC1);
        f_4.create(src.rows, src.cols, CV_8UC1);
        f_5.create(src.rows, src.cols, CV_8UC1);
        f_6.create(src.rows, src.cols, CV_8UC1);
        f_7.create(src.rows, src.cols, CV_8UC1);
        f_8.create(src.rows, src.cols, CV_8UC1);
        for (int i = 1; i < src.rows - 1; i++)
        {
           for (int j = 1; j < src.cols - 1; j++)
           {
               f_1.at<uchar>(i, j) = saturate_cast<uchar>(fabs(-5*src.at<uchar>(i - 1, j - 1) -5* src.at<uchar>(i - 1, j) -5* src.at<uchar>(i - 1, j + 1)
                                                              +3*src.at<uchar>(i, j - 1) +3*src.at<uchar>(i, j + 1)
                                                              + 3*src.at<uchar>(i + 1, j - 1) + 3*src.at<uchar>(i + 1, j) + 3*src.at<uchar>(i + 1, j + 1)));
               f_2.at<uchar>(i, j) = saturate_cast<uchar>(fabs(3*src.at<uchar>(i - 1, j - 1) -5* src.at<uchar>(i - 1, j) -5* src.at<uchar>(i - 1, j + 1)
                                                              +3*src.at<uchar>(i, j - 1) -5*src.at<uchar>(i, j + 1)
                                                              + 3*src.at<uchar>(i + 1, j - 1) + 3*src.at<uchar>(i + 1, j) + 3*src.at<uchar>(i + 1, j + 1)));
               f_3.at<uchar>(i, j) = saturate_cast<uchar>(fabs(3*src.at<uchar>(i - 1, j - 1) +3* src.at<uchar>(i - 1, j) -5* src.at<uchar>(i - 1, j + 1)
                                                              +3*src.at<uchar>(i, j - 1) -5*src.at<uchar>(i, j + 1)
                                                              + 3*src.at<uchar>(i + 1, j - 1) + 3*src.at<uchar>(i + 1, j) -5*src.at<uchar>(i + 1, j + 1)));
               f_4.at<uchar>(i, j) = saturate_cast<uchar>(fabs(3*src.at<uchar>(i - 1, j - 1) +3* src.at<uchar>(i - 1, j) +3* src.at<uchar>(i - 1, j + 1)
                                                              +3*src.at<uchar>(i, j - 1) -5*src.at<uchar>(i, j + 1)
                                                              + 3*src.at<uchar>(i + 1, j - 1) -5*src.at<uchar>(i + 1, j) -5*src.at<uchar>(i + 1, j + 1)));
               f_5.at<uchar>(i, j) = saturate_cast<uchar>(fabs(3*src.at<uchar>(i - 1, j - 1) +3* src.at<uchar>(i - 1, j) +3* src.at<uchar>(i - 1, j + 1)
                                                              +3*src.at<uchar>(i, j - 1) +3*src.at<uchar>(i, j + 1)
                                                              -5*src.at<uchar>(i + 1, j - 1) -5*src.at<uchar>(i + 1, j) -5*src.at<uchar>(i + 1, j + 1)));
               f_6.at<uchar>(i, j) = saturate_cast<uchar>(fabs(3*src.at<uchar>(i - 1, j - 1) +3* src.at<uchar>(i - 1, j) +3* src.at<uchar>(i - 1, j + 1)
                                                              -5*src.at<uchar>(i, j - 1) +3*src.at<uchar>(i, j + 1)
                                                              -5*src.at<uchar>(i + 1, j - 1) -5*src.at<uchar>(i + 1, j) +3*src.at<uchar>(i + 1, j + 1)));
               f_7.at<uchar>(i, j) = saturate_cast<uchar>(fabs(-5*src.at<uchar>(i - 1, j - 1) +3* src.at<uchar>(i - 1, j) +3* src.at<uchar>(i - 1, j + 1)
                                                              -5*src.at<uchar>(i, j - 1) +3*src.at<uchar>(i, j + 1)
                                                              -5*src.at<uchar>(i + 1, j - 1) +3*src.at<uchar>(i + 1, j) +3*src.at<uchar>(i + 1, j + 1)));
               f_8.at<uchar>(i, j) = saturate_cast<uchar>(fabs(-5*src.at<uchar>(i - 1, j - 1) -5* src.at<uchar>(i - 1, j) +3* src.at<uchar>(i - 1, j + 1)
                                                              -5*src.at<uchar>(i, j - 1) +3*src.at<uchar>(i, j + 1)
                                                              +3*src.at<uchar>(i + 1, j - 1) +3*src.at<uchar>(i + 1, j) +3*src.at<uchar>(i + 1, j + 1)));
               QVector<int> goal = {f_1.at<uchar>(i, j),f_2.at<uchar>(i, j),f_3.at<uchar>(i, j),f_4.at<uchar>(i, j),f_5.at<uchar>(i, j),f_6.at<uchar>(i, j),f_7.at<uchar>(i, j),f_8.at<uchar>(i, j)};
               int max_8 = 0;
               for(int i = 0 ; i<8;i++){
                   if(goal[i] > max_8){
                       max_8 = goal[i];
                   }
               }

               gradimg.at<uchar>(i, j) = max_8;
               gray2Img.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
           }
        }
        dst = gray2Img;
    }
};

template<>
class ComponentMethod<ctRobertsEdge> : public BasicMethod
{
public:
    ComponentType type = ctRobertsEdge;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctRobertsEdge) {
            Mat gradimg,gray2Img;
            gray2Img.create(src.rows, src.cols, CV_8UC1);
            gradimg.create(src.rows, src.cols, CV_8UC1);
            for (int i = 0; i < src.rows - 1; i++)
            {
                for (int j = 0; j < src.cols - 1; j++)
                {
                    gradimg.at<uchar>(i, j) = saturate_cast<uchar>(fabs(src.at<uchar>(i, j) - src.at<uchar>(i + 1, j + 1)) + fabs(src.at<uchar>(i + 1, j) - src.at<uchar>(i, j + 1)));
                    gray2Img.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
                }
            }
            dst = gray2Img;
        }
        else if (type == ctSobelEdge) {
            ComponentMethod<ctSobelEdge> method;
            method.invokeMethod(src,dst);
        }
        else if (type == ctLaplaceEdge) {
            ComponentMethod<ctLaplaceEdge> method;
            method.invokeMethod(src,dst);
        }
        else if (type == ctPrewittEdge) {
            ComponentMethod<ctPrewittEdge> method;
            method.invokeMethod(src,dst);
        }
        else if (type == ctCannyEdge) {
            ComponentMethod<ctCannyEdge> method;
            method.invokeMethod(src,dst);
        }
        else if (type == ctKrischEdge) {
            ComponentMethod<ctKrischEdge> method;
            method.invokeMethod(src,dst);
        }
    }
};

template<>
class ComponentMethod<ctGuassNoise> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctSaltNoise> : public BasicMethod
{
public:
    ComponentType type = ctSaltNoise;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctSaltNoise) {

        }
        else if (type == ctGuassNoise) {

        }
    }
};

template<>
class ComponentMethod<ctMiddleFilter> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctWindowFilter> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctMorphologyFilter> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctGuassFilter> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctAverageFilter> : public BasicMethod
{
public:
    ComponentType type = ctAverageFilter;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctAverageFilter) {

        }
        else if (type == ctMiddleFilter) {

        }
        else if (type == ctWindowFilter) {

        }
        else if (type == ctMorphologyFilter) {

        }
        else if (type == ctGuassFilter) {

        }
    }
};

template<>
class ComponentMethod<ctPerspective> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctAffine> : public BasicMethod
{
public:
    ComponentType type = ctAffine;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctAffine) {

        }
        else if (type == ctPerspective) {

        }
    }
};

template<>
class ComponentMethod<ctOSTU> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctKittler> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctFrameDiff> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctGuassMixture> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctThresholdSeg> : public BasicMethod
{
public:
    ComponentType type = ctThresholdSeg;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctThresholdSeg) {

        }
        else if (type == ctOSTU) {

        }
        else if (type == ctKittler) {

        }
        else if (type == ctFrameDiff) {

        }
        else if (type == ctGuassMixture) {

        }
    }
};

template<>
class ComponentMethod<ctHistogramDet> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctTemplateMatch> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctCloaking> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctColorMatch> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctGarberFilter> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctCircleLBP> : public BasicMethod
{
public:
    ComponentType type = ctCircleLBP;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctCircleLBP) {

        }
        else if (type == ctHistogramDet) {

        }
        else if (type == ctTemplateMatch) {

        }
        else if (type == ctCloaking) {

        }
        else if (type == ctColorMatch) {

        }
        else if (type == ctGarberFilter) {

        }
    }
};

template<>
class ComponentMethod<ctORB> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctSVM> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctCharTest> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctHaarVer> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctHaarHor> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<cvHaarFace> : public BasicMethod
{
public:
    void invokeMethod(Mat &src, Mat &dst) {

    }
};

template<>
class ComponentMethod<ctSIFT> : public BasicMethod
{
public:
    ComponentType type = ctSIFT;
    void invokeMethod(Mat &src, Mat &dst) {
        if (type == ctSIFT) {

        }
        else if (type == ctORB) {

        }
        else if (type == ctSVM) {

        }
        else if (type == ctCharTest) {

        }
        else if (type == ctHaarVer) {

        }
        else if (type == ctHaarHor) {

        }
        else if (type == cvHaarFace) {

        }
    }
};

#endif // COMPONENTMETHOD_H
