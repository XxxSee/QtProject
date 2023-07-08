#include "widget.h"

#include <QApplication>
#include "screenshotwgt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenShotWgt w;
    w.show();
    return a.exec();
}
