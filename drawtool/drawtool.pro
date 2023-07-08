include(../include/common.pri)
include(qtpropertybrowser/src/qtpropertybrowser.pri)
SOURCES += \
    basesetwidget.cpp \
    commands.cpp \
    customlistdelegate.cpp \
    drawglobal.cpp \
    drawsetwidget.cpp \
    graphicsbox.cpp \
    graphicslayer.cpp \
    graphicsobject.cpp \
    graphicsscene.cpp \
    graphicsview.cpp \
    layerlistmodel.cpp \
    main.cpp \
    pathitem.cpp \
    pixmapitem.cpp \
    propertywidget.cpp \
    rulewidget.cpp \
    textitem.cpp \
    toolwidget.cpp \
    widget.cpp

HEADERS += \
    basesetwidget.h \
    commands.h \
    customlistdelegate.h \
    drawglobal.h \
    drawsetwidget.h \
    graphicsbox.h \
    graphicslayer.h \
    graphicsobject.h \
    graphicsscene.h \
    graphicsview.h \
    layerlistmodel.h \
    pathitem.h \
    pixmapitem.h \
    propertywidget.h \
    rulewidget.h \
    textitem.h \
    toolwidget.h \
    widget.h

FORMS += \
    basesetwidget.ui \
    drawsetwidget.ui \
    propertywidget.ui \
    widget.ui

QT += xml

#TEMPLATE = app
