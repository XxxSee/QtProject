#ifndef WALLPAPERWIDGET_H
#define WALLPAPERWIDGET_H

#include <QWidget>
#

class WallpaperWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WallpaperWidget(QWidget *parent = nullptr);
    ~WallpaperWidget();
    void setDesktop(QWidget *parent);
};

#endif // WALLPAPERWIDGET_H
