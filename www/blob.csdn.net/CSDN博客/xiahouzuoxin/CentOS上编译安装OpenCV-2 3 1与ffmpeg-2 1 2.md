# CentOS上编译安装OpenCV-2.3.1与ffmpeg-2.1.2 - xiahouzuoxin - CSDN博客





2014年07月30日 12:33:22[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4170
个人分类：[Image/Audio/ML...](https://blog.csdn.net/xiahouzuoxin/article/category/1203619)

所属专栏：[OpenCV学习一步步](https://blog.csdn.net/column/details/zx-opencv.html)









已测试环境：
- CentOS 6.3 32bit
- CentOS 6.5 64bit

曾经在CentOS 6.3 32bit安装过OpenCV，参见[CentOS 6.3中安装OpenCV2.3.1](http://blog.csdn.net/xiahouzuoxin/article/details/9831263)，如今换了64bit系统，大刀阔斧，重新来一遍。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E4%B8%8A%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85OpenCV-2.3.1%E4%B8%8Effmpeg-2.1.2.md#%E6%A3%80%E6%9F%A5%E5%B9%B6%E5%AE%89%E8%A3%85%E7%9B%B8%E5%85%B3%E7%A8%8B%E5%BA%8F%E7%A1%AE%E4%BF%9Dgtk%E5%AE%89%E8%A3%85%E6%88%90%E5%8A%9F%E5%90%A6%E5%88%99%E6%97%A0%E6%B3%95%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87)检查并安装相关程序，确保gtk安装成功，否则无法显示图片

yum install gcc-c++              # g++编译
yum install gtk-devel            # 反正是gtk神马的，不一定就是gtk-devel，可以使用*gtk-devel*匹配
yum install gimp-devel
yum install gimp-devel-tools
yum install gimp-help-browser
yum install zlib-devel
yum install libtiff-devel        # tiff格式图片支持
yum install libjpeg-devel        # jpeg图片支持
yum install libpng-devel         # png图片支持
yum install gstreamer-devel      # 处理MP3、Ogg、MPEG1、MPEG2、AVI等格式的多媒体数据
yum install libavc1394-devel     # 1394音频接口的控制命令集
yum install libraw1394-devel
yum install libdc1394-devel
yum install jasper-devel
yum install jasper-utils
yum install cmake                # 方便编译构建OpenCV的工具
yum install swig
yum install python               # python环境支持
yum install libtool
yum install nasm                 # 汇编工具


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E4%B8%8A%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85OpenCV-2.3.1%E4%B8%8Effmpeg-2.1.2.md#%E5%AE%89%E8%A3%85ffmpeg212)安装ffmpeg2.1.2

ffmpeg没有现成的用于CentOS的rpm包，因此需要使用源码编译安装，

./configure--enable-shared–disable-yasm
make
make install


此时，ffmpeg默认安装到/usr/local下面.

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E4%B8%8A%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85OpenCV-2.3.1%E4%B8%8Effmpeg-2.1.2.md#%E5%AE%89%E8%A3%85opencv-231)安装OpenCV-2.3.1
- 
使用vi打开CMakeLists.txt文件，找到下面一行后将其注释(#)

set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS${CMAKE_CONFIGURATION_TYPES} )
# set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS${CMAKE_CONFIGURATION_TYPES} )


- 
使用vi打开CMakeLists.txt文件，搜索关键字ffmpeg，找到对应位置，

if(EXISTS /usr/include/ffmpeg/libavformat/avformat.hOR HAVE_FFMPEG_SWSCALE)


确认上面的目录是/usr/include，如果在安装ffmpeg时，修改了路径，则作相应的修改，现在我们可以不动。

- 
到opencv源代码主目录写下，依次执行

mkdir release
cd release
cmake ../
make
make install


等待安装结束，默认安装到位置/usr/local/。


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E4%B8%8A%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85OpenCV-2.3.1%E4%B8%8Effmpeg-2.1.2.md#%E6%B5%8B%E8%AF%95)测试

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


编写Makefile，

SRC=DisplayImage.cpp
LIB=-L/usr/local/lib/
INC=-I/usr/local/include/opencv/
Test:$(SRC)
   g++ -o Test $(LIB)$(INC) -lopencv_core -lopencv_highgui -lopencv_ml $^


执行make，没有错误。继续运行，./Test {ImagePathName}，出现“不能链接到动态库”的错误，大致如下：

error while loading shared libraries: libopencv_core.so.2.3: cannot open shared object file: No such file or directory


解决办法：

cd /etc/ld.so.conf.d/   # 该文件夹下存放了各种动态链接库的路径信息
vim usr-lib.conf


添加如下信息（opencv的lib所在路径）

/usr/local/lib


保存文件后执行ldconfig命令即可。

重新运行，没问题，收工。

![result](https://github.com/xiahouzuoxin/notes/raw/master/images/CentOS%E4%B8%8A%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85OpenCV-2.3.1%E4%B8%8Effmpeg-2.1.2/result.png)



