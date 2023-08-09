#include "componentdialog.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QComboBox>
#include <QDebug>

BasicDialog::BasicDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *vLay = new QVBoxLayout(this);
    vLay->setMargin(0);
    mParamWgt = new QWidget(this);
    vLay->addWidget(mParamWgt,1);
    mBtnWgt = new QWidget(this);
    vLay->addWidget(mBtnWgt,0);
    QHBoxLayout *hLay = new QHBoxLayout(mBtnWgt);
    hLay->addStretch(1);
    QPushButton *btn = new QPushButton(QStringLiteral("确定"),mBtnWgt);
    connect(btn,&QPushButton::clicked,this,&QDialog::accept);
    hLay->addWidget(btn);
    btn = new QPushButton(QStringLiteral("返回"),mBtnWgt);
    connect(btn,&QPushButton::clicked,this,&QDialog::reject);
    hLay->addWidget(btn);
}

QVariantList BasicDialog::getVariantList()
{
    return mVals;
}

template<>
void ComponentDialog<ctOpen>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("图片路径:"),mParamWgt);
    hLay->addWidget(lab);
    QLineEdit *edit = new QLineEdit(mParamWgt);
    hLay->addWidget(edit);
    QPushButton *btn = new QPushButton(QStringLiteral("打开"),mParamWgt);
    hLay->addWidget(btn);
    connect(btn,&QPushButton::clicked,this,[edit,this](){
        QString path = QFileDialog::getOpenFileName(this,"open image","./","*.jpg;;*.png");
        edit->setText(path);
        mVals[0] = path;
    });
}

template<>
void ComponentDialog<ctGray>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("选择灰度处理:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctGray),(int)ctGray);
    cbx->addItem(type2String(ctGrayHistogram),(int)ctGrayHistogram);
    cbx->addItem(type2String(ctGrayBalance),(int)ctGrayBalance);
    cbx->addItem(type2String(ctGradsSharpen),(int)ctGradsSharpen);
    cbx->addItem(type2String(ctLaplaceSharpen),(int)ctLaplaceSharpen);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}

template<>
void ComponentDialog<ctRobertsEdge>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("选择边缘检测算法:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctRobertsEdge),(int)ctRobertsEdge);
    cbx->addItem(type2String(ctSobelEdge),(int)ctSobelEdge);
    cbx->addItem(type2String(ctLaplaceEdge),(int)ctLaplaceEdge);
    cbx->addItem(type2String(ctPrewittEdge),(int)ctPrewittEdge);
    cbx->addItem(type2String(ctCannyEdge),(int)ctCannyEdge);
    cbx->addItem(type2String(ctKrischEdge),(int)ctKrischEdge);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}

template<>
void ComponentDialog<ctSaltNoise>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("选择噪音算法:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctSaltNoise),(int)ctSaltNoise);
    cbx->addItem(type2String(ctGuassNoise),(int)ctGuassNoise);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}

template<>
void ComponentDialog<ctAverageFilter>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("选择过滤算法:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctAverageFilter),(int)ctAverageFilter);
    cbx->addItem(type2String(ctMiddleFilter),(int)ctMiddleFilter);
    cbx->addItem(type2String(ctWindowFilter),(int)ctWindowFilter);
    cbx->addItem(type2String(ctMorphologyFilter),(int)ctMorphologyFilter);
    cbx->addItem(type2String(ctGuassFilter),(int)ctGuassFilter);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}

template<>
void ComponentDialog<ctAffine>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("选择变换算法:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctAffine),(int)ctAffine);
    cbx->addItem(type2String(ctPerspective),(int)ctPerspective);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}

template<>
void ComponentDialog<ctThresholdSeg>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("背景处理:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctThresholdSeg),(int)ctThresholdSeg);
    cbx->addItem(type2String(ctOSTU),(int)ctOSTU);
    cbx->addItem(type2String(ctKittler),(int)ctKittler);
    cbx->addItem(type2String(ctFrameDiff),(int)ctFrameDiff);
    cbx->addItem(type2String(ctGuassMixture),(int)ctGuassMixture);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}

template<>
void ComponentDialog<ctCircleLBP>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("特征明显:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctCircleLBP),(int)ctCircleLBP);
    cbx->addItem(type2String(ctHistogramDet),(int)ctHistogramDet);
    cbx->addItem(type2String(ctTemplateMatch),(int)ctTemplateMatch);
    cbx->addItem(type2String(ctCloaking),(int)ctCloaking);
    cbx->addItem(type2String(ctColorMatch),(int)ctColorMatch);
    cbx->addItem(type2String(ctGarberFilter),(int)ctGarberFilter);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}

template<>
void ComponentDialog<ctSIFT>::init()
{
    mVals << QVariant();
    QHBoxLayout *hLay = new QHBoxLayout(mParamWgt);
    QLabel *lab = new QLabel(QStringLiteral("特征提取:"),mParamWgt);
    hLay->addWidget(lab);
    QComboBox *cbx = new QComboBox(mParamWgt);
    cbx->addItem(type2String(ctSIFT),(int)ctSIFT);
    cbx->addItem(type2String(ctORB),(int)ctORB);
    cbx->addItem(type2String(ctSVM),(int)ctSVM);
    cbx->addItem(type2String(ctCharTest),(int)ctCharTest);
    cbx->addItem(type2String(ctHaarVer),(int)ctHaarVer);
    cbx->addItem(type2String(ctHaarHor),(int)ctHaarHor);
    cbx->addItem(type2String(cvHaarFace),(int)cvHaarFace);
    hLay->addWidget(cbx);
    void (QComboBox::*pFun)(int) = &QComboBox::activated;
    connect(cbx,pFun,this,[cbx,this](int){
        int type = cbx->currentData().toInt();
        mVals[0] = type;
    });
}
