# 使用VS2017 编写Linux系统上的Opencv程序 - DoubleLi - 博客园








# 背景

之前写图像算法的程序都是在window10下使用VS编写，VS这个IDE结合“[ImageWatch.vsix](http://download.csdn.net/download/u010677365/10249681)“插件，用于调试opencv相关的图像算法程序十分方便。后因项目需要，需将相关程序移植到Linux系统，才开始研究如何使用VS编写Linux上的Opencv程序。

# 一、Ubuntu安装相关库

## 0/先把ubuntu镜像源换成清华的

这样安装下载的速度会快些。

## 1/下载opencv cmake安装包

新建opencv文件夹，去opencv官网下载源文件，[https://opencv.org/opencv-3-4.html](https://opencv.org/opencv-3-4.html)，放入路径中； 
新建cmaker文件夹，去Cmaker官网下载安装包，[https://cmake.org/download/](https://cmake.org/download/)，放入路径中。

## 2/安装Cmake

因为可能需要联合编译，最好用最新版本，要图简单就直接sudo apt-get install cmake 。控制台输入：

```
./bootstrap
make -j3
sudo make install
```
- 1
- 2
- 3

## 3/安装依赖库

解压opencv安装包，并新建build文件夹，然后在build文件夹路径下安装依赖项，控制台输入：
`sudo apt-get install build-essential libgtk2.0-dev libjpeg-dev libtiff5-dev libjasper-dev libopenexr-dev cmake python-dev python-numpy python-tk libtbb-dev libeigen2-dev yasm libfaac-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev libqt4-dev libqt4-opengl-dev sphinx-common texlive-latex-extra libv4l-dev libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev`- 1

## 4/安装opencv

配置cmake（简单的话可以cmake-gui ..然后依次点Configure和Generate按钮生成Configure文件），控制台输入：
`sudo cmake -D CMAKE_BUILD_TYPE=RELEASE -D WITH_EIGEN=OFF -D WITH_TBB=ON -D WITH_V4L=ON -D CMAKE_INSTALL_PREFIX=/usr/local/opencv/340 ..`- 1

编译

```bash
sudo make -j3
```
- 1

安装

```bash
sudo make install -j3
```
- 1

## 5/配置lib库路径

在 /etc/ld.so.conf.d/ 路径下新建了 opencv.conf 文件(如已有该文件就不用新建了)，
`sudo gedit /etc/ld.so.conf.d/opencv.conf`- 1

打开该文件加入下面的内容： 
/usr/local/opencv/340/lib 
然后
`sudo ldconfig /etc/ld.so.conf`- 1

## 6/环境变量

对所有用户有效修改/etc/profile文件 
对个人有效则修改~/.bashrc文件

```bash
sudo gedit /etc/bash.bashrc
```
- 1

在文件末尾加上 
export PKG_CONFIG_PATH=/usr/local/opencv/340/lib/pkgconfig 
export LD_LIBRARY_PATH=/usr/local/opencv/340/lib 
保存之后，再使用命令

```bash
source /etc/bash.bashrc
```
- 1

## 7/更新

```bash
sudo updatedb
```
- 1

## 8/检查opencv安装情况

pkg-config –modversion opencv 
pkg-config –cflags opencv 
pkg-config –libs opencv

## 9/小程序测试

### 使用opencv测试代码

cd到opencv-3.4.0/smaples/cpp/example_cmake目录下

```
cmake .
make
./opencv_example
```
- 1
- 2
- 3

即可看到打开了摄像头，在左上角有一个hello opencv 即表示配置成功.

### 自己编写代码测试

自己建立文件夹，写个简单的test1.cpp，然后控制台输入：

```
g++ test1.cpp -o test1 `pkg-config opencv --cflags --libs` -std=c++11
./test1
```
- 1
- 2

## 10/安装错了，就得卸载了重新安装

进入到build文件夹，控制台输入：

```
make uninstall
cd ..
sudo rm -r release
sudo rm -r /usr/local/include/opencv2 /usr/local/include/opencv /usr/include/opencv /usr/include/opencv2 /usr/local/share/opencv /usr/local/share/OpenCV /usr/share/opencv /usr/share/OpenCV /usr/local/bin/opencv* /usr/local/lib/libopencv
```
- 1
- 2
- 3
- 4

# 二、VS新建Linux项目

Visual Studio 从2015开始，就开始支持Linux编程，VS2017更不用说（只要安装的时候注意安装跨平台Linux的模块即可）。相关的说明可上官网查看：[https://blogs.msdn.microsoft.com/vcblog/2016/03/30/visual-c-for-linux-development/](https://blogs.msdn.microsoft.com/vcblog/2016/03/30/visual-c-for-linux-development/)
打开VS，文件》新建》项目，选择Visual C++》跨平台》Linux》控制台应用程序。 
![新建Linux项目](http://img.blog.csdn.net/20180211144034906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY3NzM2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
VS会自动帮你写好一个main.cpp，此时直接按F5就能运行调试，然后需要你输入Linux系统的IP地址（可在Shell输入 ifconfig -a查询），Boot用户名，密码。 
![摘取自MSDN](http://img.blog.csdn.net/20180211144549549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY3NzM2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击确定后，就能直接运行了。 
这个时候在Linux系统的/home/下会新建projects文件夹，其中就有VS的新建项目文件夹。

# 三、VS项目链接Linux的头文件和外接opencv库

参考：[http://www.cnblogs.com/dongc/p/6599461.html](http://www.cnblogs.com/dongc/p/6599461.html)；[https://www.cnblogs.com/reskai/p/7135586.html](https://www.cnblogs.com/reskai/p/7135586.html)
主要都是项目属性中设置相关参数。

## 1/链接头文件

由于是在双系统下编程，头文件的作用如下：
- 在Windows系统下需要头文件来进行代码提示
- 在Linux系统下需要程序链接头文件

### 1-1 头文件用于代码提示

首先将Linux下相关的头文件复制到VS的Linux项目路径下，既可以直接鼠标点击复制，也可直接cmd下运行（前提system32下有pscp.exe）
`pscp -r 用户名@UbuntuIP地址:/usr/include .`- 1

然后将/usr/local/opencv/340/include（与你之前安装opencv的设置有关）下的文件也拷贝到VS的工程目录下。 
然后在项目属性中设置VC++目录》包含目录 
![包含目录主要用于代码提示](http://img.blog.csdn.net/20180211152729490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY3NzM2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 1-2 头文件用于程序运行

程序是在Linux上运行的，也需要头文件，这里的头文件路径应是Linux系统上的绝对路径。在项目属性中设置 C/C++ 》附加包含目录，这里主要填入opencv安装后的头文件目录，我这里是 
/usr/local/opencv/340/include 
/usr/local/opencv/340/include/opencv 
/usr/local/opencv/340/include/opencv2 
具体的路径根据你安装opencv确定。 
C++编译器选为g++ 
![附加包含目录才用于Linux程序运行](http://img.blog.csdn.net/20180211153841655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY3NzM2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2/链接外部opencv库

### 2-1 设置“附加”库目录

链接外部opencv库，之前我一直在VC++目录下设置 库目录，后来发现根本没用！ 
后来发现，凡是需要远程在Linux系统上运行的，都必须设置在 “附加“里面。故，**Linux的库目录也必须设置在“附加库目录”里面，里面填写的是opencv相关库在Linux下的绝对路径。 
![跟Linux相关都药设置“附加*”](http://img.blog.csdn.net/20180211154430964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY3NzM2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里我填写 
/usr/local/opencv/340/lib

### 2-2 设置库依赖项

这里跟windows下设置类似，在连接器》输入》库依赖项中，填写可能用到的opencv库名称（注意这里的名称是去掉“lib”前缀和”.a”/”.so”尾缀的库名称）。 
![这里写图片描述](http://img.blog.csdn.net/20180211155029799?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY3NzM2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我这里填入：

```
opencv_calib3d
opencv_core
opencv_dnn
opencv_features2d
opencv_flann
opencv_highgui
opencv_imgcodecs
opencv_imgproc
opencv_ml
opencv_objdetect
opencv_photo
opencv_shape
opencv_stitching
opencv_superres
opencv_video
opencv_videoio
opencv_videostab
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17

## 3/小程序测试

随便在main.cpp中写个小程序测试一下：

```cpp
#include <cstdio>
#include <iostream>  
#include <opencv2/opencv.hpp>  

using namespace std;
using namespace cv;

int main()
{
    printf("hello from UbuntuTestified!\n");
    Mat srcImage = imread("11.jpg");
    imshow("testImg2", srcImage);

    waitKey(5000);

    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17

按F5生成一下……怎么没反应？别急，VS是在Windows系统，生成的程序是在Linux系统，自然不能在windows下显示效果（但文字显示可以）。 
直接到Linux系统下，该程序的生成路径下(…\bin\release或debug)打开，记得把命名为”11.jpg”的图片也放入该路径下。 
这样程序就能正常运行了。 
![Linux下运行opencv程序效果](http://img.blog.csdn.net/2018021116001710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY3NzM2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



版权声明：本文为博主原创文章，未经博主允许不得转载。 http://blog.csdn.net/u010677365/article/details/79310617









