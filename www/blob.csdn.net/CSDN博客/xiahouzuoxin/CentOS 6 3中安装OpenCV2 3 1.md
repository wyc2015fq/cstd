# CentOS 6.3中安装OpenCV2.3.1 - xiahouzuoxin - CSDN博客





2013年08月08日 09:45:34[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2368
个人分类：[Image/Audio/ML...](https://blog.csdn.net/xiahouzuoxin/article/category/1203619)









下面为自己测试可用的OpenCV在Linux下的安装步骤

## **1、检查并安装相关程序，确保gtk安装成功，否则无法显示图片**

yum install gcc-c++ 

yuminstall gtk-devel.i686

yum installgimp-devel.i686

yuminstall gimp-devel-tools.i686

yum installgimp-help-browser.i686

yuminstall zlib-devel.i686

yum installlibtiff-devel.i686

yum installlibjpeg-devel.i686

yuminstall libpng-devel.i686

yum installgstreamer-devel.i686

yum installlibavc1394-devel.i686

yuminstall libraw1394-devel.i686

yum installlibdc1394-devel.i686

yum installjasper-devel.i686

yum install jasper-utils.i686

yum installcmake

yum installswig

yum install python

yum –y install libtool

yum installnasm.i686

## **2、安装ffmpeg**

./configure --enable-shared –disable-yasm

make 

make install

此时，ffmpeg默认安装到/usr/local下面



## **3、安装OpenCV**

使用vi打开CMakeLists.txt文件，

（1）找到

set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS${CMAKE_CONFIGURATION_TYPES} )

后将其注释（添加#号）。

（2）搜索关键字ffmpeg，找到对应位置，修改路径为（这与前面ffmpeg安装路径有关）

if(EXISTS /usr/include/ffmpeg/libavformat/avformat.hOR HAVE_FFMPEG_SWSCALE)

（3）到opencv源代码主目录写下，依次执行

mkdir release

cd release

cmake ../

make

make install

等待安装结束，默认安装到位置/usr/local/。



## **4、测试**



```cpp
#include <cv.h>
#include <highgui.h>
 
using namespace cv;
int main( int argc, char** argv )
{
  Mat image;
  image = imread(argv[1], 1 );
 
  if( argc != 2 ||!image.data )
    {
      printf("No image data \n" );
      return -1;
    }
  namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
  imshow("Display Image", image );
  waitKey(0);
 
  return 0;
}
```




编写makefile文件如下，

SRC=DisplayImage.cpp

LIB=-L/usr/local/lib/

INC=-I/usr/local/include/opencv/

Test:$(SRC)

   g++ -o Test $(LIB)$(INC) -lopencv_core -lopencv_highgui -lopencv_ml $^



执行make，没有错误。继续运行，./Test {ImagePathName}，出现“不能链接到动态库”的错误。

解决办法：

cd /etc/ld.so.conf.d/ 
  （该文件夹下存放了各种动态链接库的路径信息）

新建一个文件，vi usr-lib.conf，添加如下信息（opencv的lib所在路径）

/usr/local/lib

关闭文件后执行ldconfig命令即可。



重新运行，没问题，收工。









**参考链接：**

     1）[http://opencv.willowgarage.com/wiki/InstallGuide_Linux](http://opencv.willowgarage.com/wiki/InstallGuide_Linux)


       2）[https://sites.google.com/a/asu.edu/wireless-video-sensor/how-to-1/how-to-install-ffmpeg-opencv-under-fedora](https://sites.google.com/a/asu.edu/wireless-video-sensor/how-to-1/how-to-install-ffmpeg-opencv-under-fedora)


       3）[http://www.360doc.com/content/11/0726/10/1217721_135892686.shtml](http://www.360doc.com/content/11/0726/10/1217721_135892686.shtml)


       4）[http://www.samontab.com/web/2010/04/installing-opencv-2-1-in-ubuntu/](http://www.samontab.com/web/2010/04/installing-opencv-2-1-in-ubuntu/)


       5）[http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=4670](http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=4670)


       6）[http://www.cppblog.com/fengqing888/archive/2011/08/12/153530.html](http://www.cppblog.com/fengqing888/archive/2011/08/12/153530.html)


       7) [http://my.oschina.net/douzhenjia/blog/32683](http://my.oschina.net/douzhenjia/blog/32683)




