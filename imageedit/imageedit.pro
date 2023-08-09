include(../include/common.pri)

SOURCES += \
    componentdialog.cpp \
    componentitem.cpp \
    componentmethod.cpp \
    flowchart.cpp \
    graphicsview.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    component.h \
    componentdialog.h \
    componentitem.h \
    componentmethod.h \
    flowchart.h \
    graphicsview.h \
    widget.h

FORMS += \
    flowchart.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$${PWD}/3part/opencv/build/lib/Debug \
        -lopencv_core460d \
        -lopencv_highgui460d \
        -lopencv_imgcodecs460d \
        -lopencv_imgproc460d \
        -lopencv_video460d \
        -lopencv_videoio460d \
        -lopencv_features2d460d \
        -lopencv_objdetect460d \
        -lopencv_flann460d \
        -lopencv_dnn460d

INCLUDEPATH += $${PWD}/3part/opencv/build \
               $${PWD}/3part/opencv/opencv/modules/core/include \
               $${PWD}/3part/opencv/opencv/modules/highgui/include \
               $${PWD}/3part/opencv/opencv/modules/imgcodecs/include \
               $${PWD}/3part/opencv/opencv/modules/imgproc/include \
               $${PWD}/3part/opencv/opencv/modules/video/include \
               $${PWD}/3part/opencv/opencv/modules/videoio/include \
               $${PWD}/3part/opencv/opencv/modules/features2d/include \
               $${PWD}/3part/opencv/opencv/modules/objdetect/include \
               $${PWD}/3part/opencv/opencv/modules/flann/include \
               $${PWD}/3part/opencv/opencv/modules/dnn/include

QT += concurrent

TEMPLATE = app
