include(../include/common.pri)

SOURCES += \
    downitemwidget.cpp \
    download.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    downitemwidget.h \
    download.h \
    widget.h

FORMS += \
    downitemwidget.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += network

#TEMPLATE = app
