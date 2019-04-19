# QT显示图片的四种方法 - xqhrs232的专栏 - CSDN博客
2017年10月11日 10:28:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：41402
原文地址::[http://www.cnblogs.com/s_agapo/archive/2012/03/13/2394176.html](http://www.cnblogs.com/s_agapo/archive/2012/03/13/2394176.html)
相关文章
1、[Qt：设置背景图片的三种方式（setAutoFillBackground）](http://blog.csdn.net/liuligui5200/article/details/47612403)----[http://blog.csdn.net/liuligui5200/article/details/47612403](http://blog.csdn.net/liuligui5200/article/details/47612403)
2、[Qt中为按钮添加图片的几种方法](http://blog.csdn.net/sz_silence06/article/details/51077992)----[http://blog.csdn.net/SZ_Silence06/article/details/51077992](http://blog.csdn.net/SZ_Silence06/article/details/51077992)
QT显示图片的方法有很多
大致为
label上显示
直接画出来
容器显示
**1---------------显示gif图片（label上显示）**
[http://archive.cnblogs.com/a/2053701/](http://archive.cnblogs.com/a/2053701/)
在QT中要显示GIF图片,不能通过单单的添加部件来完成.
还需要手动的编写程序.
工具:QT Creator
新建一个工程,我们先在designer中,添加一个QLabel部件.
如下图:
![](http://pic002.cnblogs.com/images/2011/115749/2011052217340871.png)
将QLabel拉成适当大小.
在类cpp函数中添加如下程序:
```
#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QMovie>
Widget::Widget(QWidget *parent) :
QWidget(parent),
ui(new Ui::Widget)
{
ui->setupUi(this);
QMovie *movie =new QMovie("D:/Project/Qt/testclass/2.gif");
ui->label->setMovie(movie);
movie->start();
}
Widget::~Widget()
{
delete ui;
}
```
如下图:
![](http://pic002.cnblogs.com/images/2011/115749/2011052217364021.png)
这里要注意QMovie中的路径名:"D:/Project/Qt/testclass/2.gif" 这里的路径斜杠和WINDOWS下是相反的.WINDOWS下默认是反斜杠.
编译,运行就没有问题,就会看到GIF文件在播放了.
如下图:
![](http://pic002.cnblogs.com/images/2011/115749/2011052217414063.png)
当文档GIF图片显示:
```
#include <QtGui/QApplication>
#include <QLabel>
#include <QMovie>
int main(int argc,char*argv[])
{
QApplication app(argc,argv);
QLabel *label =new QLabel();
QMovie *movie =new QMovie("D:/Project/Qt/firstQT/2.gif");
label->setMovie(movie);
movie->start();
label->show();
return app.exec();
}
```
**2-------------------------label上显示图片------------------**
把你的label.png放到工程目录顶层，直接
QPixmap pixmap("label.png");
 ui->title_label->setPixmap(pixmap);
ui->title_label->show();
---
可以直接：
label->setPixmap(QPixmap("./pic.jpg"));  
或者：
QImage *image= new QImage("./pic.jpg");  
label->setPixmap(QPixmap::fromImage(image));
再或者在中途换某个图像的话：
 QImage *image= new QImage("./pic1.jpg");  
label->setPixmap(QPixmap::fromImage(image));
...........
image->load("./pic2.jpg");  
**3----------直接画出图片-------------------------**
voidlogindlg::paintEvent(QPaintEvent*)
{QPainterpainter(this);QPixmappix;pix.load("D:/QT/login/login/images/delta.png");painter.drawPixmap(0,0,100,33,pix);//painter.drawLine(0,0,100,100);}

**4-----------程序启动时的图片**
QApplication app(argc, argv);
     QSplashScreen *splash =new QSplashScreen;
     splash->setPixmap(QPixmap(":/images/splash.png"));//设置图片
     splash->show();//显示图片
     Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
     splash->showMessage(QObject::tr("Setting
 up the main window..."),topRight, Qt::white);//显示信息
     MainWindow mainWin;
     splash->showMessage(QObject::tr("Loading
 modules..."),
                         topRight, Qt::white);//显示信息
     loadModules();
     splash->showMessage(QObject::tr("Establishing
 connections..."),
                         topRight, Qt::white);//显示信息
     establishConnections();
     mainWin.show();
     splash->finish(&mainWin);//图片一直显示到mainWin加载完成
delete splash;//释放空间，节省内存
return app.exec();
来自: [http://hi.baidu.com/sdink/blog/item/879e7013027a7fcaa7ef3f83.html](http://hi.baidu.com/sdink/blog/item/879e7013027a7fcaa7ef3f83.html)
首先你得加载一张能显示透明的图片，jpg格式肯定是不行的，一般都是png
还有不同的部件加载图片的方法也不太相同，比如：
QLabel加载图片：
C/C++ code
    QString strPath=imagePath.value(day);  //图片路径
    QPixmap pix(strPath);
    dayLabel->setPixmap(pix);
QPushButton加载图片：
C/C++ code
  button->setIcon(QIcon("toolbutton.png")); 
   button->setIconSize(QSize(48, 48));
其中setIconSize函数是用来截取图片的显示区域，如果没有该函数，该图片是被缩放的放到图片上
用调色板加载图片：
C/C++ code
      QPalette p = palette(); 
       p.setBrush(QPalette::Button, QBrush(QPixmap("toolbutton.png"))); 
       setPalette(p);
另外实现按钮的透明：
C/C++ code
    button->setFlat(true);
还有就是用绘制事件函数了：
C/C++ code
    QPixmap arrayImage("/home/image/array.png"); //图片路径
    QRect arrayRect(0,0,50,50); //截取图片区域
    QPainter painter；
    painter.drawPixmap(QPoint(100,100),arrayImage,arrayRect); //打印图片
//=============================================================================================================================
备注::
1>这里要注意QMovie中的路径名:"D:/Project/Qt/testclass/2.gif" 这里的路径斜杠和WINDOWS下是相反的.WINDOWS下默认是反斜杠.
