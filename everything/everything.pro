include(../include/common.pri)

SOURCES += \
    chartwidget.cpp \
    connectionpool.cpp \
    diskdatabase.cpp \
    diskglobal.cpp \
    diskmodel.cpp \
    diskwork.cpp \
    main.cpp \
    qcppie.cpp \
    qcustomplot.cpp \
    searchwidget.cpp \
    threadpool.cpp \
    widget.cpp

HEADERS += \
    chartwidget.h \
    connectionpool.h \
    datareader.h \
    diskdatabase.h \
    diskglobal.h \
    diskmodel.h \
    diskstruct.h \
    diskwork.h \
    qcppie.h \
    qcustomplot.h \
    searchwidget.h \
    threadpool.h \
    widget.h

FORMS += \
    chartwidget.ui \
    searchwidget.ui \
    widget.ui

QT += sql printsupport

#TEMPLATE = app
