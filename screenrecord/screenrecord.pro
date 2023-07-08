include(../include/common.pri)

SOURCES += \
    gifwork.cpp \
    main.cpp \
    rectwidget.cpp \
    widget.cpp

HEADERS += \
    gif.h \
    gifwork.h \
    rectwidget.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#TEMPLATE = app
