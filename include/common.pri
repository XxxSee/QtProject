  QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += $$PWD/../include/

TEMPLATE = lib

CONFIG(debug, debug|release) {
    DESTDIR = ..\bin\debug\thirdplugin
}
else {
    DESTDIR = ..\bin\release\thirdplugin
}

# ָ���Զ���Ŀ��������Ŀ���ļ�֮��ִ��
QMAKE_POST_LINK += $$quote(del /Q $$DESTDIR\*.exp $$DESTDIR\*.ilk)

HEADERS += \
    $$PWD/../include/dll_global.h \
