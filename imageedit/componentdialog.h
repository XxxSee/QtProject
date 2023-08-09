#ifndef COMPONENTDIALOG_H
#define COMPONENTDIALOG_H

#include <QDialog>
#include <QVariantList>
#include "component.h"

class BasicDialog : public QDialog
{
public:
    BasicDialog(QWidget *parent = nullptr);
    QVariantList getVariantList();
protected:
    QWidget *mParamWgt;
    QWidget *mBtnWgt;
    QVariantList mVals;
};

template<ComponentType CT>
class ComponentDialog : public BasicDialog
{
public:
    ComponentDialog(QWidget *parent = nullptr) : BasicDialog(parent) {init();}
protected:
    void init();
};

template<>
void ComponentDialog<ctOpen>::init();
template<>
void ComponentDialog<ctGray>::init();
template<>
void ComponentDialog<ctRobertsEdge>::init();
template<>
void ComponentDialog<ctSaltNoise>::init();
template<>
void ComponentDialog<ctAverageFilter>::init();
template<>
void ComponentDialog<ctAffine>::init();
template<>
void ComponentDialog<ctThresholdSeg>::init();
template<>
void ComponentDialog<ctCircleLBP>::init();
template<>
void ComponentDialog<ctSIFT>::init();
#endif // COMPONENTDIALOG_H
