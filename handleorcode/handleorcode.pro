include(../include/common.pri)
include(qzxing/QZXing.pri)
include(qrencode/qrencode.pri)

SOURCES += \
    cameradialog.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    cameradialog.h \
    widget.h

FORMS += \
    cameradialog.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += multimedia multimediawidgets

#INCLUDEPATH += $${PWD}/../thirdpart/include/qrencode \
#               $${PWD}/../thirdpart/include/qzxing

#LIBS += -L$${PWD}/../thirdpart/lib/qrencode -lqrencoded

#LIBS += -L$${PWD}/../thirdpart/lib/qzxing -lQZXing3

#TEMPLATE = app

