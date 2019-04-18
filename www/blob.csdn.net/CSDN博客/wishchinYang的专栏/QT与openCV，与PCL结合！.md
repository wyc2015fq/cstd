# QT与openCV，与PCL结合！ - wishchinYang的专栏 - CSDN博客
2013年10月20日 13:41:54[wishchin](https://me.csdn.net/wishchin)阅读数：2927
                
（1）：详解QT多媒体框架：给予视频播放器
原文链接：[http://mobile.51cto.com/symbian-271123.htm](http://mobile.51cto.com/symbian-271123.htm)
   对于使用主框架的QT程序，实现Qimage的转换可借鉴下面程序段：
```cpp
void MainWindow::on_openButton_clicked()
{
    //第3个参数表示查找文件时从哪个目录开始，如果为"."的话，表示从该工程目录开始查找，最后那个参数的过滤器的名字之间
    //要用空格，否则识别不出来
    QString img_name = QFileDialog::getOpenFileName( this, tr("Open Image"), ".",
                                                     tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    //toAscii()返回8位描述的string,为QByteArray,data()表示返回QByteArray的指针，QByteArray为字节指针
    Mat src = imread( img_name.toAscii().data() );
    cvtColor( src, src, CV_BGR2RGB );
    QImage img = QImage( (const unsigned char*)(src.data), src.cols, src.rows, QImage::Format_RGB888 );
//    namedWindow( "Image_Show", WINDOW_AUTOSIZE );
//    imshow( "Image_Show", src );
    ui->label->setPixmap( QPixmap::fromImage(img) );
 //   ui->label->resize( ui->label->pixmap()->size() );
}
```
（2）：详解QT 快速应用OPENCV ，图片示例：基于widget！！！
原文链接：[http://mobile.51cto.com/symbian-271260.htm](http://mobile.51cto.com/symbian-271260.htm)

QT 框架中快速应用OpenCV 是本文要结束的内容，和MFC比较起来，QT的信号槽机制比MFC的信号机制慢，但是因为能很好的实现跨平台，所以我在这里总结一下可能对一些人有点用。OpenCV.China论坛上有一个帖子叫做《在MFC框架中快速应用OpenCV》看了后就想结合QT写一下。
0搭建环境：OpenCV + QT 4.6
我的实验是基于VS2008来做的，QT官方虽然提供了VS2008-add-in的插件，我没有用。直接下载器编译好的库文件进行配置,OpenCV的在VS2008下面的配置方法Google一下到处都是，这里不再补充。首先需要做的是在VS2008里面你需要使QT下和OpenCV的程序能分别跑起来。对于QT在VS的配置其实挺简单，有头文件和相应的链接库，保证调用的时候路径正确，一般就没有问题了。常用命令行make程序的人应该会很清楚那些IDE只不过是层画皮。
.pro配置：
**[cpp]**[view plain](http://blog.csdn.net/xiaojidan2011/article/details/9421193#)[copy](http://blog.csdn.net/xiaojidan2011/article/details/9421193#)
```cpp
INCLUDEPATH += d:\opencv\build\include\  
INCLUDEPATH += d:\opencv\build\include\opencv\  
INCLUDEPATH += d:\opencv\build\include\opencv2\  
  
CONFIG(debug,debug|release) {  
LIBS += -Ld:\opencv\build\x86\vc10\lib \  
    -lopencv_core243d \  
    -lopencv_highgui243d \  
    -lopencv_imgproc243d \  
    -lopencv_features2d243d \  
    -lopencv_calib3d243d  
} else {  
LIBS += -Ld:\opencv\build\x86\vc10\lib \  
    -lopencv_core243 \  
    -lopencv_highgui243 \  
    -lopencv_imgproc243 \  
    -lopencv_features2d243 \  
    -lopencv_calib3d243  
}
```
1.显示图像
QWidget是QObject下的第一个子类，使用它显示图像会减少不必要的开销。首先定制一个自己需要的QWidget：
- class myWidget : public QWidget  
- {  
-   Q_OBJECT  
-   public:  
-     myWidget(const IplImage *img,QWidget *parent = 0);  
-     ~myWidget();  
-   protected:  
-     void paintEvent(QPaintEvent *e);  
-   private:  
-     IplImage* iplImg;  
-     QImage *qImg;  
- }； 
需要绘制一个图像，我重载paintEvent(QpaintEvent *e)，我在这里面使用QPainter进行绘制。
- void myWidget::paintEvent(QPaintEvent *e)  
- {  
-     QPainter painter(this);  
-     painter.drawImage(QPoint(5,5),*qImg);  
- } 
rawImage(QPoint(5,5),qImg);的作用是将qImg绘制在左上顶点位于QPoint(5,5)处。
这里面有可能两个问题，第一个问题是要显示的图片太小，创建的Widget太大，最后显示比较丑陋。这时可以在此函数里面获得qImg的宽高，然后resize一下就好了。另外一个问题是：绘制的时候使用的是QImage，不是IplImage类型。关于这个问题论坛上有人专门写了IplImage <-> QImage的转换代码，我在这里不重复那个做法，一是有人已经做了，另外处于效率考虑，这里提供另一种方法。
通常同学们都是用cvLoadImage来读图片，保存在IplImage里面，在这里这个图片我们保存在img里面，然后通过img传进QWidget，然后我new一个QImage
- qImg = new QImage(QSize(img->width,img->height),QImage::Format_RGB888); 
我这里假设iplImg是RGB格式，且每个通道大小为8。然后创建一个IplImage 的文件头
- iplImg = cvCreateImageHeader(cvSize(img.width(),img.height()),8,3); 
此iplImage和QImage的不同之处在于QImage没有直接提供创建文件头的方法，可以通过如下方式创建只有文件头数据的QImage
- qImg = new QImage(QSize(0,0),QImage::Format_RGB888); 
另外两者的图像矩阵像素排列有点不同，比如IplImage中的BGR到了QImage中应该是RGB，当然单通道的灰度图是一样的，值得庆幸的是两者的像素矩阵都是形状相同的多维数组。这样我们可以通过指针共享这部分数据，一种方法如下：
- iplImg->imageData = (char*)qImg.bits(); 
将iplImg的图像矩阵指到qImg那里，以后我们只需要对IplImage运用opencv里面的函数进行处理，其实就直接在处理qImg里面的数据了。但是现在的图像数据还在img里面，首先得把数据搞到手，然后放到iplImg和qImg的共享区中去，另外将颜色排列以QImage中的RGB顺序为标准。
- if (img->origin == IPL_ORIGIN_TL)  
- {  
-  cvCopy(img,iplImg,0);  
- }  
- else  
- {  
-  cvFlip(img,iplImg,0);  
- }  
- cvCvtColor(iplImg,iplImg,CV_BGR2RGB); 
实际上只要做到这里图片就能显示了。如下图所示
![QT 框架中快速应用OpenCV 基于图片](http://images.51cto.com/files/uploadimg/20110624/170328455.png)
给出myWidget.cpp完整代码
- #include "myWidget.h"  
- #include <QtGui\QPainter>
- #include <QtCore\QPoint>
- 
- myWidget::myWidget(const IplImage *img,QWidget *parent /* = 0 */) : QWidget(parent)  
- {  
- qImg = new QImage(QSize(img->width,img->height),  
-    QImage::Format_RGB888);  
- iplImg = cvCreateImageHeader(cvSize(img->width,img->height),  
-   8,3);  
-  iplImg->imageData = (char*)qImg->bits();  
- 
-  if (img->origin == IPL_ORIGIN_TL)  
-  {  
-   cvCopy(img,iplImg,0);  
-  }  
-  else  
-  {  
-   cvFlip(img,iplImg,0);  
-  }  
-  cvCvtColor(iplImg,iplImg,CV_BGR2RGB);  
-  this->resize(img->width,img->height);  
- }  
- myWidget::~myWidget()  
- {  
-  cvReleaseImage(&iplImg);  
-  delete qImg;  
- }  
- void myWidget::paintEvent(QPaintEvent *e)  
- {  
-  QPainter painter(this);  
-  painter.drawImage(QPoint(0,0),*qImg);  
- } 
调用的代码很简单：
- int main(int argc,char* argv[])  
- {  
-  QApplication app(argc,argv);  
- 
-  IplImage *img = cvLoadImage("460.jpg",1);  
-  if (img)  
-  {  
-   myWidget *mw = new myWidget(img);  
-   mw->show();  
-  }  
-  int re = app.exec();  
-  cvReleaseImage(&img);  
-  return re;  
- } 
小结：关于详解 QT 框架中快速应用OpenCV 上篇内容介绍完了

（3）：利用Qt与OpenCV简单实现摄像头图像捕捉
原文链接：[http://blog.const.net.cn/a/8763.htm](http://blog.const.net.cn/a/8763.htm)
为了pro工程文件看起来简单点，创建一个pri文件，包含opencv相关信息。分别为VS2008、Mingw和Linux设置OpenCV头文件和库的路径。OPENCVPATH_MSVC=D:/Qt/OpenCV-2.1....
为了pro工程文件看起来简单点，创建一个 pri 文件，包含 opencv相关信息。
分别为 VS2008、Mingw和Linux设置OpenCV头文件和库的路径。
OPENCVPATH_MSVC = D:/Qt/OpenCV-2.1.0-vs2008
win32-msvc*{
     INCLUDEPATH += $${OPENCVPATH_MSVC}/include
     CONFIG(debug, debug|release) {
         LIBS+=-L$${OPENCVPATH_MSVC}/lib/debug 
               -lcxcore210d 
               -lhighgui210d
     } else {
         LIBS += -L$${OPENCVPATH_MSVC}/lib/release 
                 -lcxcore210 
                 -lhighgui210
    }
}
OPENCVPATH_MINGW = D:/Qt/OpenCV-2.1.0-mingw
win32-g++{
    INCLUDEPATH += $${OPENCVPATH_MINGW}/include
    LIBS += -L$${OPENCVPATH_MINGW}/lib 
            -lcxcore210 
            -lhighgui210
}
unix:LIBS += -lcv 
    -lhighgui
## 读取类
创建一个视频捕捉类，提供start和stop槽来控制摄像头打开和关闭，通过定时器定时捕捉图像，并通过信号将图片传出。
//cameradevice.h#ifndef CAMERADEVICE_H#define CAMERADEVICE_H#include <QtCore/QObject>QT_BEGIN_NAMESPACEclassQTimer;classQImage;QT_END_NAMESPACEnamespacecv{classVideoCapture;classMat;}classCameraDevice : publicQObject{Q_OBJECTpublic:explicitCameraDevice(QObject *parent = 0);    ~CameraDevice();signals:voidimageReady(constQImage& image);publicslots:boolstart();boolstop();privateslots:voidonTimeout();private:QImageimageFromMat(constcv::Mat& frame);cv::VideoCapture * m_capture;QTimer * m_timer;};#endif // CAMERADEVICE_H
//cameradevice.cpp#include <QtCore/QTimer>#include <QtGui/QImage>#include "opencv/cv.h"#include "opencv/highgui.h"#include "cameradevice.h"CameraDevice::CameraDevice(QObject *parent) :QObject(parent){m_capture = newcv::VideoCapture;m_timer = newQTimer(this);connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));}CameraDevice::~CameraDevice(){deletem_capture;m_capture = NULL;}boolCameraDevice::start(){if (m_capture->isOpened()) {returntrue;    }m_capture->open(CV_CAP_ANY);if (m_capture->isOpened()) {m_timer->start(40);    }returnm_capture->isOpened();}boolCameraDevice::stop(){if (m_capture->isOpened()) {m_capture->release();    }returntrue;}voidCameraDevice::onTimeout(){if (!m_capture->isOpened()) {return;    }staticcv::Matframe;    *m_capture >> frame;if (frame.cols) {emitimageReady(imageFromMat(frame));    }}QImageCameraDevice::imageFromMat(constcv::Mat &frame){constunsignedchar* src = frame.ptr();QImageimage(frame.cols, frame.rows, QImage::Format_RGB32);unsignedchar * des = image.bits();unsignedlongcount = frame.cols * frame.rows;for (unsignedlongi = 0; i < count; ++i)    {        *des++ = *src++;        *des++ = *src++;        *des++ = *src++;        *des++ = 0xff;    }returnimage;}
## 界面类
对话框类就简单了，只需要一个 label 来显示图像，两个按钮来控制开启与关闭即可
//dialog.h#ifndef DIALOG_H#define DIALOG_H#include <QtGui/QDialog>namespaceUi {classDialog;}classCameraDevice;classDialog : publicQDialog{Q_OBJECTpublic:explicitDialog(QWidget *parent = 0);    ~Dialog();privateslots:voidonImageArrival(constQImage & image);private:Ui::Dialog *ui;CameraDevice * m_camera;};#endif // DIALOG_H[喝小酒的网摘]http://blog.const.net.cn/a/8763.htm
//dialog.cpp#include "dialog.h"#include "ui_dialog.h"#include "cameradevice.h"Dialog::Dialog(QWidget *parent) :QDialog(parent),ui(newUi::Dialog),m_camera(newCameraDevice(this)){ui->setupUi(this);connect(m_camera, SIGNAL(imageReady(QImage)), this, SLOT(onImageArrival(QImage)));connect(ui->startButton, SIGNAL(clicked()), m_camera, SLOT(start()));connect(ui->stopButton, SIGNAL(clicked()), m_camera, SLOT(stop()));}Dialog::~Dialog(){deleteui;}voidDialog::onImageArrival(constQImage &image){ui->view->setPixmap(QPixmap::fromImage(image));}
## 主程序
//main.cpp#include <QtGui/QApplication>#include "dialog.h"intmain(intargc, char *argv[]){QApplicationa(argc, argv);Dialogw;w.show();returna.exec();}
## 问题
用一个定时器来做似乎存在些问题，反复打开关闭，在windows下可发现资源泄漏。不清楚是什么原因。
简单调整几行代码，改为多线程程序后，该问题似乎消失了。
- 改动主要在对话框类中：
#include <QtCore/QThread>#include "dialog.h"#include "ui_dialog.h"#include "cameradevice.h"Dialog::Dialog(QWidget *parent) :QDialog(parent),ui(newUi::Dialog),m_camera(newCameraDevice){ui->setupUi(this);QThread * thread = newQThread(this);m_camera->moveToThread(thread);thread->start();connect(m_camera, SIGNAL(imageReady(QImage)), this, SLOT(onImageArrival(QImage)));connect(ui->startButton, SIGNAL(clicked()), m_camera, SLOT(start()));connect(ui->stopButton, SIGNAL(clicked()), m_camera, SLOT(stop()));}Dialog::~Dialog(){m_camera->deleteLater();deleteui;}voidDialog::onImageArrival(constQImage &image){ui->view->setPixmap(QPixmap::fromImage(image));}
改为多线程后，可将捕捉类中的定时器的时间设为 0
**（4）：QThread的使用方法**
昨天不小心看到Qt开发人员( Bradley T. Hughes)Blog中的一片文章 [you
 are-doing-it-wrong](http://labs.trolltech.com/blogs/2010/06/17/youre-doing-it-wrong/) 。 结果看得头昏脑胀：好歹也自学了近1年的Qt，也一直很小心、很认真地阅读Qt和manual和例子等资料，却被突然告知，QThread的正确使用方法 是一种自己从没见过，而且Qt manual、example、书籍中都没有提到过的一种方法。到底怎么了... [喝小酒的网摘]http://blog.const.net.cn/a/8760.htm
莫非manual、exmaple以及资料中的介绍都是错的？？
认真看看其他的人的评论，总算理清了一点头绪。所有事情源于 QThread 的事件循环！
QThread 的两种使用方法
1. 不使用事件循环。这是官方的 Manual 、example 以及相关书籍中都介绍的一种的方法。
a. 子类化 QThread
b. 重载 run 函数，run函数内有一个 while 或 for 的死循环
c. 设置一个标记为来控制死循环的退出。
2. 使用事件循环。（博客 [you
 are-doing-it-wrong](http://labs.trolltech.com/blogs/2010/06/17/youre-doing-it-wrong/) 批驳的就是这种情况下的 一种用法。）
a. 子类化 QThread，
b. 重载 run 使其调用 QThread::exec() 
c. 并为该类定义信号和槽，这样一来，由于槽函数并不会在新开的 thread 运行，很多人为了解决这个问题在构造函数中调用
```
moveToThread(this); 
而争论和不解正是这样的一条语句造成的。
```
Bradley T. Hughes 给出说明是： QThread
 应该被看做是操作系统线程的接口或控制点，而不应该包含需要在新线程中运行的代码。需要运行的代码应该放到一个QObject的子类中，然后将该子类的对象moveToThread到新线程中。
另外：
在Qt4.3(包括)之前，run 是虚函数，必须子类化QThread来实现run函数。
而从Qt4.4开始，[qthreads-no-longer-abstract](http://labs.trolltech.com/blogs/2007/07/05/qthreads-no-longer-abstract/)   
 ，run 默认调用 QThread::exec() 。这样一来不需要子类化 QThread 了，只需要子类化一个 QObject 就够了，这正是被 Bradley T. Hughes推荐的方法。
终于看懂了，但
不管怎么说，都应该是 QThread 当初的设计导致的这种问题，而所有文档和例子中都没有提到该如何使用Qthread 进一步加剧了对QThread的这种误用。
相关链接：
http://labs.qt.nokia.com/blogs/2010/06/17/youre-doing-it-wrong/
http://labs.qt.nokia.com/blogs/2006/12/04/threading-without-the-headache/
http://labs.qt.nokia.com/blogs/2007/07/05/qthreads-no-longer-abstract/
http://gitorious.org/qthreadhowto/qthreadhowto/trees/master
http://blog.exys.org/entries/2010/QThread_affinity.html
http://thesmithfam.org/blog/2010/02/07/talking-to-qt-threads/[喝小酒的网摘]http://blog.const.net.cn/a/8760.htm
**（5）：使用opencv和QT实现摄像头采集**
在网上收罗了很多资料，QT没有专门操作摄像头的类，这个得自己写。网上也有很多关务openCV和V4l的一些介绍，由于我项目要在window下开发，所以就选择了openCV。由于以前没有用过openCV，所以就只看了关于openCVS摄像头操作的这部分，其他的还没时间去看。
openCV ： http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.1/  
刚开始下载的是2.3.1的，安装后发现没有lib库所以后面选择了2.1的
openCV中文学习 pdf  ：
现在，开始详细的介绍如何在QT中实时的采集摄像头数据。
打开QTcreator （我用的是QT 2.3的 中文版）
新建一个widget工程
![](http://image49.360doc.com/DownloadImg/2012/02/2115/21692165_1.gif)
在界面上放两个label 分别用来显示摄像头采集到的数据和照的照片。
![](http://image49.360doc.com/DownloadImg/2012/02/2115/21692165_2.gif)
编辑camaraget.h 文件
**[cpp]**[view plain](http://blog.csdn.net/llh318724/article/details/7007661#)[copy](http://blog.csdn.net/llh318724/article/details/7007661#)[?](http://www.360doc.com/Edit/editor.htm?id=content&mode=1&imagePath=5205dde9#)
```cpp
#ifndef CAMARAGET_H   
    #define CAMARAGET_H   
      
    #include <QWidget>   
    #include <QImage>   
    #include <QTimer>     // 设置采集数据的间隔时间   
      
    #include <highgui.h>  //包含opencv库头文件   
    #include <cv.h>   
      
    namespace Ui {  
        class camaraGet;  
    }  
      
    class camaraGet : public QWidget  
    {  
        Q_OBJECT  
      
    public:  
        explicit camaraGet(QWidget *parent = 0);  
        ~camaraGet();  
      
    private slots:  
        void openCamara();      // 打开摄像头   
        void readFarme();       // 读取当前帧信息   
        void closeCamara();     // 关闭摄像头。   
        void takingPictures();  // 拍照   
      
    private:  
        Ui::camaraGet *ui;  
        QTimer    *timer;  
        QImage    *imag;  
        CvCapture *cam;// 视频获取结构， 用来作为视频获取函数的一个参数   
        IplImage  *frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像   
    };  
      
    #endif // CAMARAGET_H
```
编辑camaraget.cpp
**[cpp]**[view plain](http://blog.csdn.net/llh318724/article/details/7007661#)[copy](http://blog.csdn.net/llh318724/article/details/7007661#)[?](http://www.360doc.com/Edit/editor.htm?id=content&mode=1&imagePath=5205dde9#)
```cpp
#include "camaraget.h"   
    #include "ui_camaraget.h"   
      
    camaraGet::camaraGet(QWidget *parent) :  
        QWidget(parent),  
        ui(new Ui::camaraGet)  
    {  
        ui->setupUi(this);  
      
        cam     = NULL;  
        timer   = new QTimer(this);  
        imag    = new QImage();         // 初始化   
      
        /*信号和槽*/  
        connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息   
        connect(ui->open, SIGNAL(clicked()), this, SLOT(openCamara()));  
        connect(ui->pic, SIGNAL(clicked()), this, SLOT(takingPictures()));  
        connect(ui->closeCam, SIGNAL(clicked()), this, SLOT(closeCamara()));  
    }  
      
    /****************************** 
    ********* 打开摄像头 *********** 
    *******************************/  
    void camaraGet::openCamara()  
    {  
        cam = cvCreateCameraCapture(0);//打开摄像头，从摄像头中获取视频   
      
        timer->start(33);              // 开始计时，超时则发出timeout()信号   
    }  
      
    /********************************* 
    ********* 读取摄像头信息 *********** 
    **********************************/  
    void camaraGet::readFarme()  
    {  
        frame = cvQueryFrame(cam);// 从摄像头中抓取并返回每一帧   
        // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。   
        QImage image((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888);  
        ui->label->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上   
    }  
      
    /************************* 
    ********* 拍照 *********** 
    **************************/  
    void camaraGet::takingPictures()  
    {  
        frame = cvQueryFrame(cam);// 从摄像头中抓取并返回每一帧   
      
        // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。   
        QImage image((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888);  
      
        ui->label_2->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上   
    }  
      
    /******************************* 
    ***关闭摄像头，释放资源，必须释放*** 
    ********************************/  
    void camaraGet::closeCamara()  
    {  
        timer->stop();         // 停止读取数据。   
      
        cvReleaseCapture(&cam);//释放内存；   
    }  
      
    camaraGet::~camaraGet()  
    {  
        delete ui;  
    }
```
好了，全部代码都OK了（当然，创建工程时，会生成main.cpp，不必去改动它），但现在你点运行，依然会产生错误，为什么呢？因为还没有把openCV的库包含进去。
在*.pro 文件中 加入：
INCLUDEPATH+=C:\OpenCV2.1\include\opencv
	LIBS+=C:\OpenCV2.1\lib\highgui210.lib\C:\OpenCV2.1\lib\cxcore210.lib\C:\OpenCV2.1\lib\cv210.libOK，大功告成，运行后，在widget中点击打开摄像头，就可以看到自己了。运行后的效果：
![](http://image49.360doc.com/DownloadImg/2012/02/2115/21692165_3.gif)
后来我发现这个效果不怎么好，就改了一下：改了以后的运行效果也贴出来：
就改了一句：
**[cpp]**[view plain](http://blog.csdn.net/llh318724/article/details/7007661#)[copy](http://blog.csdn.net/llh318724/article/details/7007661#)[?](http://www.360doc.com/Edit/editor.htm?id=content&mode=1&imagePath=5205dde9#)
- QImage image((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888);  
**[cpp]**[view plain](http://blog.csdn.net/llh318724/article/details/7007661#)[copy](http://blog.csdn.net/llh318724/article/details/7007661#)[?](http://www.360doc.com/Edit/editor.htm?id=content&mode=1&imagePath=5205dde9#)
- 改为了 QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();  
![](http://image49.360doc.com/DownloadImg/2012/02/2115/21692165_4.gif)
（6）：求道勿远，求心勿近 的一个博客：[Qt Opencv 在Linux下摄像头简单示例v1.0](http://blog.chinaunix.net/uid-24641004-id-3459705.html)
   原文链接：[http://blog.chinaunix.net/uid-24641004-id-3459705.html](http://blog.chinaunix.net/uid-24641004-id-3459705.html)
下面写的文章也许网上也有类似的，但是大多数都没有给出思路及背景，让初学者每次都只能学到一点皮毛，不少知识需要大量搜索零碎地拼凑起来。题外话，虽然现在是碎片化信息时代，但正是这样信息整合能力也显得非常重要，为读者提供高质量的文章是我以后的目标。我以后会注意分析应用背景及些过程的解析。
不想看下面笔者对编译环境的理解的，可以直接在本页Ctrl+F查找“编译过程”看，因为下面有很大段笔者心酸的回忆，嘻嘻。
资源介绍
opencv是一个非常优秀的图形图像处理类库，里面的类或者结构体，封装了很多实用的图像处理算法，调用其提供的API，等于使用一些复杂的图像处理算法，真是解放生产力啊。这里笔者用的是linux版本的opencv2.2源码，几十M，稳定版本够用即可，没必要时刻追着新版本。不过在linux上的opencv各种版本都是如笔者那样编译即可用。
思考一下，为什么我们要下源码来编译？不是像windows上的直接发布一个压缩包即可用么？我们这次编译为是得到什么？其实很简单，opencv里有专门处理摄像头的API，我们要用它，就得有它的头文件及动态库（或静态库）。而我们拿到源码往往不是为了去研究其代码（如果是做这算法方面的研究便是例外了），而是拿到“本地”编译。至于为何，笔者认为是大家内核版本版本的不统一，编译器版本也不统一导致这样做的的。如果使用Ubuntu之类的，很简单，一个apt-get什么都不用管，因为Ubuntu提供了适应你的内核版本编译器编译的库。知道的同学希望指教一下。
下载请移步：[http://www.opencv.org.cn/index.php/Download](http://www.opencv.org.cn/index.php/Download)
虚拟机上的摄像头，一般都是用USB连接，如果是笔记本自带的，也是把虚拟机里的connect上即可。连接外设的方法：在虚拟机顶栏菜单，VM->Removable Devices->XX WebCam->connect  && show in status Bar(前者把设备从win环境解除，连接到虚拟机环境，后者是显示在虚拟机右下角的状态栏)。
具体看下图:
   QT，版本其实与opencv版本没有什么关系，因为代码编译出来，最终都是翻译成机器码，QT源码是C++写的，OPENCV源码也是C/C++写的，编译器认识他们即可。笔者一起也是担心版本问题，网上的文章往往都是写着qt4.7.2+opencv2.0XXX，或者qtcreator2.0+opencv2.3.1在Ubuntu11上编译成功之类的文章，我那时就很担心，又要确定QT版本,又要确定opencv版本，还得确定linux版本吗？linux怎么那么复杂啊？就是他们的标题及其内容都没有明确指出他们方法的通用性。笔者在这里就说明一下：这文章在linux上的适用，QT版本(4.6,4.7,5.0等无论什么版本都可以，whatever),opencv版本（1.0,2.0,2.2,2.3,2.4,whatever都可以），linux（各种发行版本（内核是2.6以上的），只要别用2.4的内核就好。）
编译过程：
摄像头在虚拟机上（当然，从物理上说你还是得有摄像头才行）安装上面的图看一下即可。下面主要是opencv的编译：主要参考opencv中文论坛的文章
- ./configure//头文件和库文件一般都是默认安装到/usr/local/include/usr/local/lib内
- 
- make 
- 
- make install
具体代码：
先讲最终要实现什么，很简单，一个窗口，里面有一个label显示摄像头的图像。
然后讲讲原理：每隔一段时间我们就去摄像头抓取一帧图像，然后放到ui->label上面，如果取的时间快，就造成“视频”的感觉了。（可以想到，其实摄像头拍照也是很简单，我们就把其中一帧取出来即可。）
新建一个工程，继承QDialog（用什么窗体都可以），在UI上拖出一个label放在中间，
拉到适当大小。
在工程xxx.pro里面添加必要的库及头文件的路径：
```cpp
INCLUDEPATH +=/usr/local/include/opencv
    LIBS +=/usr/local/lib/libcv.so\
            /usr/local/lib/libhighgui.so\
            /usr/local/lib/libcxcore.so\
//dialog.h
    #ifndef DIALOG_H
    #define DIALOG_H
    #include <QDialog>
    #include <cv.h>
    #include <highgui.h>
    #include <QTimer>
    #include <QPixmap>
    namespace Ui {
        class Dialog;
    }
    class Dialog :public QDialog
    {
        Q_OBJECT
    public:
        explicit Dialog(QWidget*parent= 0);
        ~Dialog();
    private:
        Ui::Dialog*ui;
        CvCapture *capture;//highgui 里提供的一个专门处理摄像头图像的结构体
        IplImage *frame;//摄像头每次抓取的图像为一帧，使用该指针指向一帧图像的内存空间
        QTimer *timer;//定时器用于定时取帧，上面说的隔一段时间就去取就是用这个实现。
    private slots:
        void getFrame();//实现定时从摄像头取图并显示在label上的功能。
    };
    #endif // DIALOG_H
//dialog.cpp
    #include "dialog.h"
    #include "ui_dialog.h"
    #include <QDebug>
    Dialog::Dialog(QWidget*parent):
        QDialog(parent),
        ui(new Ui::Dialog)
    {
        ui->setupUi(this);
        timer = new QTimer(this);
        capture = cvCaptureFromCAM(0);//cvCaptureFromCAM其实是一个宏，就是cvCreateCameraCapture的别名，0代表第一个摄像头。-1代表默认摄像头。
        if(capture==NULL){
            qDebug()<<"error!";
        }
        timer->start(50);//1000为1秒，50毫秒去取一帧
        connect(timer,SIGNAL(timeout()),this,SLOT(getFrame()));//超时就去取
    }
    void Dialog::getFrame()
    {
        frame = cvQueryFrame(capture);//从摄像头取帧
        QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();//简单地转换一下为Image对象，rgbSwapped是为了显示效果色彩好一些。
        ui->label->setPixmap(QPixmap::fromImage(image));
    }
    Dialog::~Dialog()
    {
        timer->stop();//停止取帧
        cvReleaseCapture(&capture);//释放资源是个好习惯
        delete ui;
    }
```
本文参考资料：
源码编译：
[http://www.opencv.org.cn/index.php/%E6%BA%90%E7%A0%81%E7%BC%96%E8%AF%91](http://www.opencv.org.cn/index.php/%E6%BA%90%E7%A0%81%E7%BC%96%E8%AF%91)
QT中使用OPENCV库
[http://www.opencv.org.cn/index.php/%E5%9C%A8Qt%E4%B8%AD%E4%BD%BF%E7%94%A8OpenCV%E5%BA%93](http://www.opencv.org.cn/index.php/%E5%9C%A8Qt%E4%B8%AD%E4%BD%BF%E7%94%A8OpenCV%E5%BA%93)
Debian/Ubuntu
[http://www.opencv.org.cn/index.php/Debian%E4%B8%8B%E5%AE%89%E8%A3%85](http://www.opencv.org.cn/index.php/Debian%E4%B8%8B%E5%AE%89%E8%A3%85)
