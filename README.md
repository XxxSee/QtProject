# QtProject
some windows tool programs written in QT

-mainprogram 

> will load each lib

![](https://github.com/XxxSee/QtProject/blob/main/image/overview.gif?raw=true)

-downloader 

> only support ”http download link address“

![](https://github.com/XxxSee/QtProject/blob/main/image/downloader.gif?raw=true)

-drawtool  

> a simple drawing tool for windows
>
> 支持保存工程、输出图片、图层管理、属性编辑、撤销/反撤销
>
> Attribute editing uses qtpropertybrowser library

![](https://github.com/XxxSee/QtProject/blob/main/image/drawtool.gif?raw=true)

-everything

> system file search
>
> multi-threaded traversal of system files and inserting file information into sqlite tables
>
> use QCustomPlot to show them
>
> 基于QCustomPlot编写了QCPPie饼状图

![](https://github.com/XxxSee/QtProject/blob/main/image/everything.gif?raw=true)

-handleorcode

> use qzxing library to decode OR code
> qzxing：https://github.com/ftylitak/qzxing.git
>
> use qrencode library to encode OR code
>
> compile qrencode library need libpng and zlib
>
> libpng：http://www.libpng.org/
>
> zlib：https://zlib.net/
>
> 使用生成的lib或者将需要的源码包含进项目中使用
>
> qzxing库本身已经包含了生成与识别，看需求选择

-mediaplayer

> windows live wallpaper
>
> include video、gif、web
>
> 在debug版本，视频播放画面会闪烁，在release不会出现此问题

![](https://github.com/XxxSee/QtProject/blob/main/image/mediaplayer.gif?raw=true)

-screenrecord

> only support gif record
>
> use gif library：https://github.com/charlietangora/gif-h.git

![](https://github.com/XxxSee/QtProject/blob/main/image/screenrecord.gif?raw=true)

-screenshot

> 全屏、选择矩形区域、不规则区域截图
