QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WindowTool

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h \

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += PROJECT_LOAD_DLL

INCLUDEPATH += D:/XxxSee/Source/QT\Dll/QtProject/include

greaterThan(QT_MAJOR_VERSION,4){
        TARGET_ARCH=$${QT_ARCH}
}else{
        TARGET_ARCH=$${QMAKE_HOST.arch}
}

contains(TARGET_ARCH, x86_64){
    CONFIG(debug, debug|release) {
        DESTDIR = ..\bin\x64\debug
    }
    else {
        DESTDIR = ..\bin\x64\release
    }
}else{
    CONFIG(debug, debug|release) {
        DESTDIR = ..\bin\x86\debug
    }
    else {
        DESTDIR = ..\bin\x86\release
    }
}
