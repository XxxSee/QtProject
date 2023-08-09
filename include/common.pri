  QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += $$PWD/../include/

TEMPLATE = lib

greaterThan(QT_MAJOR_VERSION,4){
        TARGET_ARCH=$${QT_ARCH}
}else{
        TARGET_ARCH=$${QMAKE_HOST.arch}
}

contains(TARGET_ARCH, x86_64){
    CONFIG(debug, debug|release) {
        DESTDIR = ..\bin\x64\debug\thirdplugin
    }
    else {
        DESTDIR = ..\bin\x64\release\thirdplugin
    }
}else{
    CONFIG(debug, debug|release) {
        DESTDIR = ..\bin\x86\debug\thirdplugin
    }
    else {
        DESTDIR = ..\bin\x86\release\thirdplugin
    }
}

# 指定自定义目标在生成目标文件之后执行
QMAKE_POST_LINK += $$quote(del /Q $$DESTDIR\*.exp $$DESTDIR\*.ilk)

HEADERS += \
    $$PWD/../include/dll_global.h \
