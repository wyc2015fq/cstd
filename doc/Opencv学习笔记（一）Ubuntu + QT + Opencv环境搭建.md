# Opencv学习笔记（一）Ubuntu + QT + Opencv环境搭建

2012年03月03日 16:53:08

crzy_sparrow

阅读数：12824

更多

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									

​    **最近，想好好地整下opencv里出现的算法，打算沿着牛人杨现同学的opencv系列笔记学习，感觉写得真的很不错，很大程度上激发了我学习opencv的热情，这里表示感谢哈。杨大牛的博客地址如下：**

​         http://blog.csdn.net/yang_xian521

​    **要好好学习，一个不错的开发环境是必须的，于是今天趁着空闲搭建了下ubuntu +QT + opencv的开发环境。具体步骤如下：**

​    **1)终端下运行apt-cache search opencv，显示如下：（root权限哦）**
 harpia - Image Processing/Computer Vision Automatic Prgm. Tool
 libcv-dev - development files for libcv
 libcv2.1 - computer vision library
 libcvaux-dev - development files for libcvaux
 libcvaux2.1 - computer vision extension library
 libhighgui-dev - development files for libhighgui
 libhighgui2.1 - computer vision GUI library
 opencv-doc - OpenCV documentation and examples
 python-opencv - Python bindings for the computer vision library

​    **2）把这些包都安装了：apt-get installlibcv-dev libcv2.1 libcvaux-dev libcvaux2.1 libhighgui-dev libhighgui2.1  opencv-doc**

​    **3）对qt安装目录下的mkspecs/default/qmake.conf修改，我的QT-SDK安装目录是/opt/QTSDK，qmake.conf在/opt/QtSDK/Desktop/Qt/4.8.0/gcc/mkspecs/default下具体修改如下：添加**

QMAKE_INCDIR = /usr/include/opencv

QMAKE_LIBS = -lcvaux -lcv -lcxcore -lhighgui -lml



**测试代码：**

```cpp
#include <cv.h>
#include <highgui.h>
int main()
{
IplImage *img = 0;
img = cvLoadImage("tong.bmp",-1);    //tong.bmp在所建工程的目录下
cvNamedWindow("tong", 1);
cvShowImage("tong", img);
cvWaitKey(0);
return 0;
}
```

 **和同学出去吃饭啦，回来继续搞起，加油加油！**



**更新下：附上编译安装opencv2.3.1的方法（ubuntu+qt+opencv2.3.1）：**

**http://code.google.com/p/qt-opencv-multithreaded/wiki/Documentation**

**附上学习资料电子书一本：**

**http://ishare.iask.sina.com.cn/f/16863133.html**

**其源代码：http://ishare.iask.sina.com.cn/f/22616566.html**

**好吧，加油学习，以后也会写写opencv学习心得～～**