# 三维重建：QT+OpenNI+Kinect图像校正 - wishchinYang的专栏 - CSDN博客
2013年11月26日 12:17:09[wishchin](https://me.csdn.net/wishchin)阅读数：4617
**后记**：
        当时能不放弃这个方向是因为这里面涉及了一种很有效的三位场景存储方式，可能给出除图元建模之外的一种三维场景描述方式。这和Flash与位图的对比一样，基于图元的flash始终抵不过基于点描述的位图格式。
        总结：OpenNI已经有了一个专门的语句对标定进行了封装，我们不需要再费力去使用自己的代码了。
        原文链接：[http://www.cnblogs.com/tornadomeet/archive/2012/09/27/2706417.html](http://www.cnblogs.com/tornadomeet/archive/2012/09/27/2706417.html)
[系列文章，作者相当给力！](http://www.cnblogs.com/tornadomeet/archive/2012/09/27/2706417.html)
        不过有一个更为强大的文章：[http://blog.csdn.net/chenyusiyuan/article/details/6279762](http://blog.csdn.net/chenyusiyuan/article/details/6279762)
### [OpenCV学习笔记（20）Kinect + OpenNI + OpenCV + OpenGL 组合体验](http://blog.csdn.net/chenyusiyuan/article/details/6279762)
# [Kinect+OpenNI学习笔记之(获取kinect的颜色图像和深度图像)](http://www.cnblogs.com/tornadomeet/archive/2012/09/27/2706417.html)
**前言**
　　网上有不少使用Qt做界面，OpenNI为库来开发kinect。或许大家的第一个问题就是询问该怎样使用Kinect来获取颜色信息图和深度信息图呢？这一节就是简单来回答这个问题的。
　　开发环境：QtCreator2.5.1+OpenNI1.5.4.0+Qt4.8.2；原因：Qt与VTK可以完美融合，虽然工作量不大，但是还是没有时间了。
**实验说明：**
　　在使用OpenNI来驱动读取kinect数据时，我们需要了解context object这个名词。查看了下OpenNI UserGuide文档，简单翻译下这个名词的意思：
　　Context是openNI中一个主要的object，它掌握了OpenNI使用过程中应用程序的全部状态，以及这些状态的prodection chains,一个应用程序有多个context，但是这些context之间不能共享信息。例如一个中间件节点不能使用另一个context的驱动节点。Context在使用前必须被立即初始化，因此此时所有嵌入的模块被下载和分析。为了释放context的内存，应用程序需调用shutdown程序。      
　　虽然翻译得不准确，但是它的大概意思就是告诉我们在驱动kinect时，需要用到context这个类，且我们需要安装一定顺序去使用，这与一些常见的库驱动差不多，比如opengl，这些都需要什么初始化啊，设置属性啊等。因此我们只需要直接去看懂他人的一个工程实例就ok了。
　　好了，本文参考Heresy的教程中的源码写的。
　　在新建好工程文件后，需要包含XnCppWrapper头文件，且需在Qt工程中设置好头文件目录和库文件目录。
　　使用OpenNI读取颜色图和深度图的步骤如下（这个是程序的核心部分）：
　　1. 定义一个Context对象，并 调用该对象的Init()方法来进行初始化。
　　2. 定义一个XnMapOutputMode格式对象，设置好分图像分辨率和帧率。
　　3. 定义颜色图和深度图的节点对象，并用其Create()方法来创建，参数为Context对象.
　　4. 设置颜色和深度图的输出模式，调用的方法是SetMapOutputMode();参数为步骤2中定义和设置好了的XnMapOutputMode对象。
　　6. 如果深度图和颜色图在一张图上显示，则必须对深度图像进行校正，校正的方法是调用深度图的如下方法：.GetAlternativeViewPointCap().SetViewPoint();
　　7. 调用context对象的StartGeneratingAll()来开启设备读取数据开关。
　　8. 调用context对象的更新数据方法，比如WaitAndupdateAll()方法。
　　9. 定义颜色图和色彩图的ImageMetaData对象，并利用对应的节点对象的方法GetMetaData()，将获取到的数据保存到对应的ImageMetaData对象中。
　　10. 如果需要将深度图转换成灰度图来显示，则需要自己将深度值转换成0~255的单通道或者多通道数据，然后直接用来显示。
　　注意如果没有设置视觉校正，则深度图的显示与颜色图的显示会出现对应不上的情况，后面的实验可以看出这2者的区别，另外对于是否需要设置镜像就要看自己的具体应用场合了。
**实验结果：**
　　下面分别分是否设置图像镜像，是否对深度图像进行校正来给出实验结果.
　　无镜像无校正：
![](http://pic002.cnblogs.com/images/2012/381513/2012092723121181.png)
　　无镜像有校正：
![](http://pic002.cnblogs.com/images/2012/381513/2012092723122611.png)
　　有镜像无校正：　　
![](http://pic002.cnblogs.com/images/2012/381513/2012092723130174.png)
　　有镜像有校正：
![](http://pic002.cnblogs.com/images/2012/381513/2012092723132052.png)
　　从有无镜像可以看出，设置镜像的效果与字面的理解是一样的，即有镜像时就相当于取镜子中的图像。有无校正可以看出，没有校正时，深度图片和颜色图片同一个物体都对应不起来，可以看下天花板上的吊灯就可以发现，没校正，2者不重合，且相差不少。有校正时效果就好多了，只是此时的深度图像显示的范围要稍小些。
**实验主要部分代码及注释(附录有工程code下载链接)：**
**首先来个最小工程，即去掉那些错误处理代码：**
**　　main.cpp:**
```cpp
#include <QtGui>
#include <XnCppWrapper.h> //包含OpenNI的头文件
using namespace xn;//使用OpenNI库中的命名空间
//全局的OpenNI object
Context g_context;
ImageGenerator g_image_generator;
DepthGenerator g_depth_generator;
//全局的Qt Object
QGraphicsPixmapItem *g_image_map;
QGraphicsPixmapItem *g_depth_map;
//CTimer类的定义
class CTimer : public QObject
{
public:
    void start() {        
        g_context.StartGeneratingAll();//开启设备读取数据的开关
        startTimer(33);//使用startTimer()启动定时器，每当时间到时会自动调用timerEvent()函数
    }
private:
    void timerEvent(QTimerEvent *) {
        g_context.WaitAndUpdateAll();//更新数据
        //颜色数据
        ImageMetaData image_map;
        g_image_generator.GetMetaData(image_map);
        //为g_image_map设置图片，图片的数据来源于外部硬件设备
        g_image_map->setPixmap(QPixmap::fromImage(QImage(image_map.Data(), image_map.XRes(),
                                                         image_map.YRes(), QImage::Format_RGB888)));
        //深度数据
        DepthMetaData depth_map;
        g_depth_generator.GetMetaData(depth_map);
        XnDepthPixel max_depth_value = depth_map.ZRes();
        QImage depth_img(depth_map.XRes(), depth_map.YRes(), QImage::Format_ARGB32);//格式为ARGB32型的
        for(unsigned int i = 0; i < depth_map.XRes(); i++)
            for(unsigned int j = 0; j < depth_map.YRes(); j++)
            {
                XnDepthPixel depth_value_ij = depth_map(i, j);//获取x，y处的坐标值
                if(depth_value_ij == 0) {
                    depth_img.setPixel(i, j, qRgba(0, 0, 0, 0));
                }//如果捕捉不到深度信息，则将其设置为0
                else {
                    float fscale = 1.0f*depth_value_ij/max_depth_value;//当前深度的比例因子
                    depth_img.setPixel(i, j, qRgba(255*(1-fscale), 0, 255*fscale, 255*(1-fscale)));
                }
            }
        g_depth_map->setPixmap(QPixmap::fromImage(depth_img));
    }
};
int  main(int argc, char **argv)
{
    QApplication app(argc, argv);
    g_context.Init();//context初始化
    g_context.SetGlobalMirror(true);//设置全局镜像,就像照镜子一样，与设置为false时的2张图片镜像
    XnMapOutputMode xmode;//定义图像的输出模式
    xmode.nXRes = 640;//x方向分辨率
    xmode.nYRes = 480;//y方向分辨率
    xmode.nFPS = 30;//帧率
    //设置颜色节点属性
    g_image_generator.Create(g_context);
    g_image_generator.SetMapOutputMode(xmode);
    //设置深度节点属性
    g_depth_generator.Create(g_context);
    g_depth_generator.SetMapOutputMode(xmode);
    //视觉校正,否则深度图和颜色图感应到的区域不能一一对应
    g_depth_generator.GetAlternativeViewPointCap().SetViewPoint(g_image_generator);
    //Qt场景设置
    QGraphicsScene scene;
    g_image_map = scene.addPixmap(QPixmap());
    g_image_map->setZValue(1);//设置为z方向上的第1层
    g_depth_map = scene.addPixmap(QPixmap());
    g_depth_map->setZValue(2);//设置为z方向上的第2层
    //Qt视图创建
    QGraphicsView view(&scene);
    view.resize(660, 500);
    //设置定时器，每隔一段时间读取kinect的颜色信息和深度信息
    CTimer timer;
    timer.start();
    view.show();
    return app.exec();
}
```
**加入错误处理部分后的完整main.cpp:**
```
```cpp
#include <QtGui>
#include <XnCppWrapper.h> //包含OpenNI的头文件
using namespace xn;//使用OpenNI库中的命名空间
//全局的OpenNI object
XnStatus g_status;
Context g_context;
ImageGenerator g_image_generator;
DepthGenerator g_depth_generator;
bool g_has_image_generator = true;
//全局的Qt Object
QGraphicsPixmapItem *g_image_map;
QGraphicsPixmapItem *g_depth_map;
//CTimer类的定义
class CTimer : public QObject
{
public:
    void start() {
        g_status = g_context.StartGeneratingAll();//开启设备读取数据的开关
        if(g_status == XN_STATUS_OK) {
            startTimer(33);//使用startTimer()启动定时器，每当时间到时会自动调用timerEvent()函数
        }
        else {
            QMessageBox::critical(NULL, "Create Data Error!", xnGetStatusString(g_status));//显示创建数据失败，该消息框没有父窗口
        }
    }
private:
    void timerEvent(QTimerEvent *) {
        g_context.WaitAndUpdateAll();//更新数据
        //颜色数据
        if(g_has_image_generator) {
            ImageMetaData image_map;
            g_image_generator.GetMetaData(image_map);
            //为g_image_map设置图片，图片的数据来源于外部硬件设备
            g_image_map->setPixmap(QPixmap::fromImage(QImage(image_map.Data(), image_map.XRes(),
                                                         image_map.YRes(), QImage::Format_RGB888)));
        }
        //深度数据
        DepthMetaData depth_map;
        g_depth_generator.GetMetaData(depth_map);
        XnDepthPixel max_depth_value = depth_map.ZRes();
        QImage depth_img(depth_map.XRes(), depth_map.YRes(), QImage::Format_ARGB32);//格式为ARGB32型的
        for(unsigned int i = 0; i < depth_map.XRes(); i++)
            for(unsigned int j = 0; j < depth_map.YRes(); j++)
            {
                XnDepthPixel depth_value_ij = depth_map(i, j);//获取x，y处的坐标值
                if(depth_value_ij == 0) {
                    depth_img.setPixel(i, j, qRgba(0, 0, 0, 0));
                }//如果捕捉不到深度信息，则将其设置为0
                else {
                    float fscale = 1.0f*depth_value_ij/max_depth_value;//当前深度的比例因子
                    depth_img.setPixel(i, j, qRgba(255*(1-fscale), 0, 255*fscale, 255*(1-fscale)));
                }
            }
        g_depth_map->setPixmap(QPixmap::fromImage(depth_img));
    }
};
int  main(int argc, char **argv)
{
    QApplication app(argc, argv);
    g_status = g_context.Init();//context初始化
    if(g_status != XN_STATUS_OK) {
        QMessageBox::critical(NULL, "Context Initial Error!", xnGetStatusString(g_status));
        return -1;
    }
   // g_context.SetGlobalMirror(true);//设置全局镜像,就像照镜子一样，与设置为false时的2张图片镜像
    XnMapOutputMode xmode;//定义图像的输出模式
    xmode.nXRes = 640;//x方向分辨率
    xmode.nYRes = 480;//y方向分辨率
    xmode.nFPS = 30;//帧率
    //设置颜色节点属性
    g_status = g_image_generator.Create(g_context);
    if(g_status != XN_STATUS_OK) {
        QMessageBox::critical(NULL, "Image map create failed", xnGetStatusString(g_status));
        g_has_image_generator = false;
    }
    if( g_has_image_generator ) {
        g_status = g_image_generator.SetMapOutputMode(xmode);
        if(g_status != XN_STATUS_OK) {
            QMessageBox::critical(NULL, "Image map output mode error!", xnGetStatusString(g_status));
            return -1;
        }
    }
    //设置深度节点属性
    g_status = g_depth_generator.Create(g_context);
    if(g_status != XN_STATUS_OK) {
        QMessageBox::critical(NULL, "Depth map create failed", xnGetStatusString(g_status));
        return -1;
    }
    g_status = g_depth_generator.SetMapOutputMode(xmode);
    if(g_status != XN_STATUS_OK) {
        QMessageBox::critical(NULL, "Depth map output mode error!", xnGetStatusString(g_status));
        return -1;
    }
    if(g_has_image_generator)//视觉校正,否则深度图和颜色图感应到的区域不能一一对应
        ;//g_depth_generator.GetAlternativeViewPointCap().SetViewPoint(g_image_generator);
    //Qt场景设置
    QGraphicsScene scene;
    g_image_map = scene.addPixmap(QPixmap());
    g_image_map->setZValue(1);//设置为z方向上的第1层
    g_depth_map = scene.addPixmap(QPixmap());
    g_depth_map->setZValue(2);//设置为z方向上的第2层
    //Qt视图创建
    QGraphicsView view(&scene);
    view.resize(660, 500);
    //设置定时器，每隔一段时间读取kinect的颜色信息和深度信息
    CTimer timer;
    timer.start();
    view.show();
    return app.exec();
}
```
**总结：**通过本次实验，了解了怎样使用OpenNI来显示kinect的颜色图像和深度图像了。
```
** 参考资料：**[http://kheresy.wordpress.com/index_of_openni_and_kinect/comment-page-5/](http://kheresy.wordpress.com/index_of_openni_and_kinect/comment-page-5/)
**附录：**[实验工程code下载](http://download.csdn.net/detail/wuweigreat/4604487)。
作者：tornadomeet     出处：http://www.cnblogs.com/tornadomeet欢迎转载或分享，但请务必声明文章出处。 （新浪微博：tornadomeet,欢迎交流！）
总结：源代码还是挺有用的，不过最后还是放弃了，还是MFC简单一些，对于只是验证算法的我们来说，渐渐的失去了方向。
**（二）：把代码写在一个单独的类里面**
**前言**
　　在上一篇文章[Kinect+OpenNI学习笔记之2(获取kinect的颜色图像和深度图像)](http://www.cnblogs.com/tornadomeet/archive/2012/09/27/2706417.html)[](http://www.cnblogs.com/tornadomeet/archive/2012/09/26/2704046.html)中,已经介绍了怎样使用OpenNI来获取Kinect的深度数据和颜色数据,并将获取到的结果在Qt中显示，不过那个代码是写在同一个cpp文件中，以后用到的时候不能讲这些显示的基本过程单独拿出来，比较麻烦。所以这节主要是将OpenNI获取图像的流程以及Qt显示这些图像的结果分开为了2个类来写，方便以后工程的直接拷贝。
　　开发环境：QtCreator2.5.1+OpenNI1.5.4.0+Qt4.8.2
**实验说明**
　　COpenNI这个类主要是初始化kinect设备，并获取深度图像和颜色图像，参加上一篇博客的初始化过程步骤，如果遇到错误，则有相应的错误处理过程。CKinectReader类是将COpenNI这个类读取到的结果显示在Qt的界面上的。因此一个类是负责与硬件Kinect打交道，一个类是负责与人（界面显示）打交道的。具体的过程见上篇文章的分析和后面的代码。
　　这里发现一个小问题，与kinect有关的工程如果改变了代码，则在每次编译前最好clean一下，因为有可能是与硬件设备相关，没有clean的工程和clean后的工程效果有时会不同。
**C/C++知识点总结:**
　　在构造函数中可以使用冒号给类中的数据成员赋值，这样的好处就是可以给常量和引用变量赋值初始化赋值的效果。
　　类的私有成员只能是类内部的函数调用，连类的对象都不能去调用私有成员变量。
　　在类的内部使用qDebug()， cout等函数输出调试时是不行的。
　　隐式数据类型转换，如果是同种类型的数据进行四则运算，则得出的结果也是那种类型，如果其中有常数类型的数据常数参与，则得出的结果会自动转换成跟常数类型相同的类型。
　　如果一个类以单独一个cpp文件出现,在使用到该类的时候,直接include该cpp文件.
**实验结果**
　　在程序中设置了镜像和视觉校正，且将kinect感应不到深度信息的地方全部显示为不透明的黑色，因此你在图中看到的黑色部分就是kinect的深度盲区。
　　效果如下：
![](http://pic002.cnblogs.com/images/2012/381513/2012092909434440.png)
**实验主要部分代码及注释（附录有工程code下载链接）：**
***copenni.cpp:***
```
```cpp
#include <XnCppWrapper.h>
#include <QtGui>
#include <iostream>
using namespace xn;
using namespace std;
class COpenNI
{
public:
    ~COpenNI() {
        context.Release();//释放空间
    }
    bool Initial() {
        //初始化
        status = context.Init();
        if(CheckError("Context initial failed!")) {
            return false;
        }
        context.SetGlobalMirror(true);//设置镜像
        //产生图片node
        status = image_generator.Create(context);
        if(CheckError("Create image generator  error!")) {
            return false;
        }
        //产生深度node
        status = depth_generator.Create(context);
        if(CheckError("Create depth generator  error!")) {
            return false;
        }
        //视角校正
        status = depth_generator.GetAlternativeViewPointCap().SetViewPoint(image_generator);
        if(CheckError("Can't set the alternative view point on depth generator")) {
            return false;
        }
        return true;
    }
    bool Start() {
        status = context.StartGeneratingAll();
        if(CheckError("Start generating error!")) {
            return false;
        }
        return true;
    }
    bool UpdateData() {
        status = context.WaitNoneUpdateAll();
        if(CheckError("Update date error!")) {
            return false;
        }
        //获取数据
        image_generator.GetMetaData(image_metadata);
        depth_generator.GetMetaData(depth_metadata);
        return true;
    }
public:
    DepthMetaData depth_metadata;
    ImageMetaData image_metadata;
private:
    //该函数返回真代表出现了错误，返回假代表正确
    bool CheckError(const char* error) {
        if(status != XN_STATUS_OK ) {
            QMessageBox::critical(NULL, error, xnGetStatusString(status));
            cerr << error << ": " << xnGetStatusString( status ) << endl;
            return true;
        }
        return false;
    }
private:
    XnStatus    status;
    Context     context;
    DepthGenerator  depth_generator;
    ImageGenerator  image_generator;
};
```
```
***ckinectreader.cpp:***
```
```cpp
#include <QtGui>
#include <QDebug>
#include <XnCppWrapper.h>
#include "copenni.cpp"  //要包含cpp文件，不能直接包含类
#include <iostream>
using namespace std;
class CKinectReader: public QObject
{
public:
    //构造函数,用构造函数中的变量给类的私有成员赋值
    CKinectReader(COpenNI &openni, QGraphicsScene &scene) : openni(openni), scene(scene) {
        test = 0.0;
    }
    ~CKinectReader() {
        scene.removeItem(image_item);
        scene.removeItem(depth_item);
        delete [] p_depth_argb;
    }
    bool Start(int interval = 33) {
        openni.Start();//因为在调用CKinectReader这个类的之前会初始化好的,所以这里直接调用Start了
        image_item = scene.addPixmap(QPixmap());
        image_item->setZValue(1);
        depth_item = scene.addPixmap(QPixmap());
        depth_item->setZValue(2);
        openni.UpdateData();
        p_depth_argb = new uchar[4*openni.depth_metadata.XRes()*openni.depth_metadata.YRes()];
        startTimer(interval);//这里是继承QObject类，因此可以调用该函数
        return true;
    }
    float test ;
private:
    COpenNI &openni;    //定义引用同时没有初始化,因为在构造函数的时候用冒号来初始化
    QGraphicsScene &scene;
    QGraphicsPixmapItem *image_item;
    QGraphicsPixmapItem *depth_item;
    uchar *p_depth_argb;
private:
    void timerEvent(QTimerEvent *) {
        openni.UpdateData();
        //这里使用const，是因为右边的函数返回的值就是const类型的
        const XnDepthPixel *p_depth_pixpel = openni.depth_metadata.Data();
        unsigned int size = openni.depth_metadata.XRes()*openni.depth_metadata.YRes();
        //找深度最大值点
        XnDepthPixel max_depth = *p_depth_pixpel;
        for(unsigned int i = 1; i < size; ++i)
            if(p_depth_pixpel[i] > max_depth )
                max_depth = p_depth_pixpel[i];
        test = max_depth;
        //将深度图像格式归一化到0~255
        int idx = 0;
        for(unsigned int i = 1; i < size; ++i) {
            //一定要使用1.0f相乘，转换成float类型，否则该工程的结果会有错误,因为这个要么是0，要么是1，0的概率要大很多
            float fscale = 1.0f*(*p_depth_pixpel)/max_depth;
            if((*p_depth_pixpel) != 0) {
                p_depth_argb[idx++] = 255*(1-fscale);    //蓝色分量
                p_depth_argb[idx++] = 0; //绿色分量
                p_depth_argb[idx++] = 255*fscale;   //红色分量，越远越红
                p_depth_argb[idx++] = 255*(1-fscale); //距离越近，越不透明
            }
            else {
                p_depth_argb[idx++] = 0;
                p_depth_argb[idx++] = 0;
                p_depth_argb[idx++] = 0;
                p_depth_argb[idx++] = 255;
            }
            ++p_depth_pixpel;//此处的++p_depth_pixpel和p_depth_pixpel++是一样的
        }
        //往item中设置图像色彩数据
        image_item->setPixmap(QPixmap::fromImage(
                              QImage(openni.image_metadata.Data(), openni.image_metadata.XRes(), openni.image_metadata.YRes(),
                              QImage::Format_RGB888)));
        //往item中设置深度数据
        depth_item->setPixmap(QPixmap::fromImage(
                              QImage(p_depth_argb, openni.depth_metadata.XRes(), openni.depth_metadata.YRes()
                              , QImage::Format_ARGB32)));
    }
};
```
```
***main.cpp:***
```
```cpp
#include <QtGui/QtGui>
#include <QDebug>
#include "ckinectreader.cpp"
int main(int argc, char **argv)
{
    COpenNI openni;
    if(!openni.Initial())//初始化返回1表示初始化成功
        return 1;
    QApplication app(argc, argv);
    QGraphicsScene scene;
    QGraphicsView view;
    view.setScene(&scene);
    view.resize(650, 540);
    view.show();
    CKinectReader kinect_reader(openni, scene);
    kinect_reader.Start();//启动，读取数据
    qDebug() << kinect_reader.test;
    return app.exec();
}
```
```
**总结：**这次实验的目的主要是将相互稍微独立的代码用单独的类来写，方便以后的代码重复利用。
**参考资料：**[http://kheresy.wordpress.com/2011/08/18/show_maps_of_openni_via_qt_graphicsview/](http://kheresy.wordpress.com/2011/08/18/show_maps_of_openni_via_qt_graphicsview/)
**附录：**[实验工程code下载](http://download.csdn.net/detail/wuweigreat/4607689)。
        作者：tornadomeet出处：http://www.cnblogs.com/tornadomeet欢迎转载或分享，但请务必声明文章出处。 （新浪微博：tornadomeet,欢迎交流！）
