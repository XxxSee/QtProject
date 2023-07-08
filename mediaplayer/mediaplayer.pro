include(../include/common.pri)

QT += multimedia webenginewidgets

SOURCES += \
    finddesktop.cpp \
    gifdesktop.cpp \
    main.cpp \
    mediawidget.cpp \
    mousehook.cpp \
    mywebengnineview.cpp \
    seekslider.cpp \
    videodesktop.cpp \
    videosurface.cpp \
    videowidget.cpp \
    videowork.cpp \
    wallpaperwidget.cpp \
    webdesktop.cpp \
    widget.cpp

HEADERS += \
    finddesktop.h \
    gifdesktop.h \
    mediawidget.h \
    mousehook.h \
    mywebengnineview.h \
    seekslider.h \
    videodesktop.h \
    videosurface.h \
    videowidget.h \
    videowork.h \
    wallpaperwidget.h \
    webdesktop.h \
    widget.h

FORMS += \
    gifdesktop.ui \
    mediawidget.ui \
    videodesktop.ui \
    webdesktop.ui \
    widget.ui

#TEMPLATE = app
