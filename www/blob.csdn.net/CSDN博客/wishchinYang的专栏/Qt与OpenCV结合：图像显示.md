# Qt与OpenCV结合：图像显示 - wishchinYang的专栏 - CSDN博客
2014年06月26日 10:30:36[wishchin](https://me.csdn.net/wishchin)阅读数：1298
参考链接： [http://www.cnblogs.com/emouse/archive/2013/03/29/2988717.html](http://www.cnblogs.com/emouse/archive/2013/03/29/2988717.html)
注意：为了防止min max 问题，要把opencv的包含放在包含列表的最后...
配置文件：自己替换即可.
```cpp
INCLUDEPATH += C:/Dev/CV2.4.6/opencv/build/include
LIBS +=C:/Dev/CV2.4.6/opencv/build/x86/vc10/lib/opencv_highgui246d.lib
LIBS += C:/Dev/CV2.4.6/opencv/build/x86/vc10/lib/opencv_core246d.lib
LIBS += C:/Dev/CV2.4.6/opencv/build/x86/vc10/lib/opencv_imgproc246d.lib
```
cpp包含文件：
```cpp
#include <QMainWindow>
#include <QTextEdit>
#include <QDockWidget>
#include <QWidget>
#include "draw/curveSimple.h"
#include "draw/sinusplot.h"
#include "draw/QwtDraw.h"
#include "draw/cruvePlot.h"
//#include <opencv/cv.h>
//#include <opencv2/core/core_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
```
使用控件显示图像的代码：
```cpp
void CWishGUI::showCvRGB(cv::Mat &image)
{
//    //1.利用QPixmap显示图像
//    QPixmap pixmap("./Image/SunFlowerMicro.jpg");
//    this->ui->WishLabel->setPixmap(pixmap);
    //2.利用OpenCV显示图像//在Label上显示
    cv::Mat rgb;
    QImage img;
    if(image.channels()==3)  {
        //cvt Mat BGR 2 QImage RGB
        cvtColor(image,rgb,CV_BGR2RGB);
        img =QImage((const unsigned char*)(rgb.data),
                    rgb.cols,rgb.rows,
                    rgb.cols*rgb.channels(),
                    QImage::Format_RGB888);
    }
    else {
        img =QImage((const unsigned char*)(image.data),
                    image.cols,image.rows,
                    image.cols*image.channels(),
                    QImage::Format_RGB888);
    }
    this->ui->WishLabel->setPixmap(QPixmap::fromImage(img));
    this->ui->WishLabel->resize(ui->WishLabel->pixmap()->size());
    return;
}
```

