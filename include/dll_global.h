#ifndef DLL_GLOBAL_H
#define DLL_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtWidgets/QWidget>
#ifdef PROJECT_LOAD_DLL
#define PROJECT_LIB Q_DECL_IMPORT
#else
#define PROJECT_LIB Q_DECL_EXPORT
#endif

class PROJECT_LIB BaseWidget : public QWidget
{
    Q_OBJECT
public:
    BaseWidget(QWidget *parent = nullptr):QWidget(parent){}
    virtual ~BaseWidget(){}
    virtual QString dllName() = 0;
};

extern "C" {
     BaseWidget PROJECT_LIB *createWidget();
}
#endif // DLL_GLOBAL_H
