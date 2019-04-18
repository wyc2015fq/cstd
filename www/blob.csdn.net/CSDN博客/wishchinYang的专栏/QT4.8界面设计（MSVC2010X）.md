# QT4.8界面设计（MSVC2010X） - wishchinYang的专栏 - CSDN博客
2014年06月18日 12:13:56[wishchin](https://me.csdn.net/wishchin)阅读数：1028
个人分类：[QT./Linux](https://blog.csdn.net/wishchin/article/category/1721683)
1.C++ IDE设计
MFC这种半死不活的windows C++平台已经被抛弃，很无奈。转向Qt的C++还是不错的选择。
QT的QML才是最新的亮点，可惜没有时间经历这些东西了。
**2.程序代码**
**2.1 开始哥的配置文件：**
```cpp
QT       += core gui
TARGET = QtGUI
TEMPLATE = app
INCLUDEPATH += C:/PCL_1.6/3rdParty/Boost/include
INCLUDEPATH += C:/PCL_1.6/3rdParty/Eigen/include
INCLUDEPATH += C:/PCL_1.6/3rdParty/FLANN/include
INCLUDEPATH += C:/PCL_1.6/3rdParty/Qhull/include
INCLUDEPATH += C:/PCL_1.6/3rdParty/VTK/include
INCLUDEPATH += C:/Dev/HDF5_1.8.11/include
INCLUDEPATH += C:/Dev/CV2.4.6/opencv/build/include
SOURCES += main.cpp\
        wishgui.cpp
HEADERS  += wishgui.h 
//SOURCES +=  cwishguigo.cpp
//HEADERS  += cwishguigo.h
//HEADERS  += include/LSH.h
//SOURCES  += include/LSH.cpp
HEADERS  += include/CvDraw.h
SOURCES  += include/CvDraw.cpp
//LIBS += C:/PCL_1.6/3rdParty/Boost/lib/boost_filesystem-vc100-mt-gd-1_50.lib
//LIBS += C:/PCL_1.6/3rdParty/Boost/lib/boost_system-vc100-mt-gd-1_50.lib
//LIBS += C:/PCL_1.6/3rdParty/Boost/lib/libboost_filesystem-vc100-mt-1_50.lib
//LIBS += C:/PCL_1.6/3rdParty/Boost/lib/libboost_system-vc100-mt-1_50.lib
LIBS += C:/PCL_1.6/3rdParty/Boost/lib/libboost_filesystem-vc100-mt-gd-1_50.lib
LIBS += C:/PCL_1.6/3rdParty/Boost/lib/libboost_system-vc100-mt-gd-1_50.lib
LIBS += C:/PCL_1.6/3rdParty/Boost/lib/libboost_date_time-vc100-mt-gd-1_50.lib
LIBS += C:/PCL_1.6/3rdParty/Boost/lib/libboost_thread-vc100-mt-gd-1_50.lib
LIBS += C:/PCL_1.6/3rdParty/FLANN/lib/flann_cpp_s-gd.lib
LIBS += C:/PCL_1.6/3rdParty/Qhull/lib/qhull6_d.lib
LIBS += C:/PCL_1.6/3rdParty/VTK/lib/vtk-5.8/QVTK-gd.lib
LIBS += C:/Dev/HDF5_1.8.11/lib/hdf5.lib
//使用MSVC调试器，下面注释掉两行为MinGW调试器所使用
//LIBS += C:/Dev/CV2.4.6/opencv/build/x86/mingw/lib/libopencv_highgui246.dll.a
//LIBS += C:/Dev/CV2.4.6/opencv/build/x86/mingw/lib/libopencv_core246.dll.a
LIBS += C:/Dev/CV2.4.6/opencv/build/x86/vc10/lib/opencv_highgui246d.lib
LIBS += C:/Dev/CV2.4.6/opencv/build/x86/vc10/lib/opencv_core246d.lib
FORMS    += wishgui.ui
```
**2. 2 程序入口**
```cpp
#include <QtGui/QApplication>
#include <QtGui>
#include "wishgui.h"
//#include "include/LSH.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //SetBackGround...
    CWishGUI  *frame= new CWishGUI;
    frame->setObjectName("myframe");
    frame->resize(831,580);
    frame->setStyleSheet("CWishGUI#myframe{border-image:url(src/GrassCropReform.png)}" );
    frame->show();
    return app.exec();
}
```
**2.3 UI文件**
**注意事项：资源文件要放在 生成目录的 PupilGuiWish-build-desktop-Qt_4_8_6_qmake____ 文件夹！**
![](https://img-blog.csdn.net/20140618100227453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20140618100246812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
是不是界面太丑了？来，改进一下设计：
**2.3.1 按钮控件透明处理**
找了半天没有找到，只实现了Tab的透明化。图片如下：
![](https://img-blog.csdn.net/20140618120722875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
修改的UI类代码为：
```cpp
CWishGUI::CWishGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CWishGUI)
{
    ui->setupUi(this);
    QPixmap pixmap("./Image/Spark.jpg");
    ui->WishLabel->setPixmap(pixmap);
    //ui->ShowRGB->setStyleSheet("QPushButton:pane {background:  transparent; }");
    //ui->ShowRGB->setAttribute(Qt::WA_TranslucentBackground, true);//it is no use!
    ui->ShowRGB->setAttribute(Qt::WA_StyledBackground, true);
    //主要修改代码
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:2px solid #e8f3f9;border-right:2px solid #e8f3f9; border-left:2px solid #e8f3f9;border-bottom:2px solid #e8f3f9;background:  transparent; }");
    //ui->tabWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    //ui->tabWidget->setWindowOpacity(0.2);
    ui->lineEdit_PoseX->setStyleSheet("QLineEdit:pane {border-top:1px solid #e8f3f9;border-right:1px solid #e8f3f9; border-left:1px solid #e8f3f9;border-bottom:1px solid #e8f3f9;background:  transparent; }");
    connect( ui->Connect  , SIGNAL(clicked()), this, SLOT(setImageShow)  );
}
```
**3.程序结果**
弄了大半天，只设计了一个UI，没有对程序进行类的规划继承，可以说是雏形的雏形。
