# 在Qt工程中使用Boost库 - fanyun的博客 - CSDN博客
2018年01月21日 19:53:08[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：548
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
在我们的C++开发中，使用优质库能极大地提高开发效率，获得很多便利。例如Boost库，Boost库是一个很强大的库，使用过的都知道它的功能强大及设计精良。那么我们能不能在Qt框架里面使用呢？答案是肯定的，Qt框架也是C++实现的啊！
 那么怎么把Boost库引入我们Qt框架的项目呢？下面我们细说：
首先，我们进入Boost官网：http://www.boost.org/，下载一个Boost库的压缩包：boost_1_52_0.tar.gz ，解压到本地，在这，
给大家推荐一个Boost开发手册：《Boost程序库完全开发指南》
使用Qt5.8新建一个工程：新建完以后在工程找到Build.pro文件 ，做如下配置：
/Workspace/project/boost_1_52_0/ 
```cpp
QT       += core
QT       -= gui
  
TARGET = boost-test
  
INCLUDEPATH += /Workspace/project/boost_1_52_0/ 
  
CONFIG   += console
CONFIG   -= app_bundle
  
TEMPLATE = app
 
SOURCES += main.cpp
```
然后，在需要引用Boost库的源文件中将Boost头文件包含进来就使用了。
下面做个测试：
```cpp
#include <boost/timer.hpp>
#include <QDebug>
  
int main(int argc, char *argv[])
{
    boost::timer tmr;
  
    QApplication a(argc, argv);
    Dialog w;
    w.show();
  
    qDebug() << tmr.elapsed() << "S" << endl;
    return a.exec();
}
```
