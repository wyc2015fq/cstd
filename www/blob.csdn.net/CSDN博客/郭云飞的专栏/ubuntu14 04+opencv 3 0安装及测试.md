
# ubuntu14.04+opencv 3.0安装及测试 - 郭云飞的专栏 - CSDN博客


2016年07月06日 18:45:29[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：211个人分类：[计算机应用																](https://blog.csdn.net/guoyunfei20/article/category/1848145)[Halcon经验分享																](https://blog.csdn.net/guoyunfei20/article/category/1837131)[
							](https://blog.csdn.net/guoyunfei20/article/category/1848145)



# ubuntu14.04+opencv 3.0安装及测试
时间 2015-05-02 12:47:50[博客园精华区
](http://www.tuicool.com/sites/bAzeYv)
*原文*[
http://www.cnblogs.com/llxrl/p/4471831.html](http://www.tuicool.com/articles/hit/nYJrYra)
主题[
](http://www.tuicool.com/topics/11020052)OpenCV[ ](http://www.tuicool.com/topics/11020052)Ubuntu[
](http://www.tuicool.com/topics/11200002)
本文记录ubuntu下安装opencv过程，步骤来自[
opencv官网](http://docs.opencv.org/doc/tutorials/introduction/linux_install/linux_install.html?highlight=install)
## 1、安装opencv所需的库（编译器、必须库、可选库）
转载请说明[
http://www.cnblogs.com/llxrl/p/4471831.html](http://www.cnblogs.com/llxrl/p/4471831.html)
GCC 4.4.x or later
CMake 2.6 or higher
Git
GTK+2.x or higher, including headers (libgtk2.0-dev)
pkg-config
Python 2.6 or later and Numpy 1.5 or later with developer packages (python-dev, python-numpy)
ffmpeg or libav development packages: libavcodec-dev, libavformat-dev, libswscale-dev
[optional] libtbb2 libtbb-dev
[optional] libdc1394 2.x
[optional] libjpeg-dev, libpng-dev, libtiff-dev, libjasper-dev, libdc1394-22-dev

```python
1
```
```python
[compiler]
```
```python
su
```
```python
do
```
```python
apt-
```
```python
get
```
```python
install
```
```python
build-
```
```python
essential
```
```python
2
```
```python
[required]
```
```python
sudo
```
```python
apt-
```
```python
get
```
```python
install
```
```python
cmake git libgtk2
```
```python
.
```
```python
0
```
```python
-dev
```
```python
pkg-config
```
```python
libavcodec-dev libavformat-dev libswscale-
```
```python
dev
```
```python
3
```
```python
[optional]
```
```python
sudo
```
```python
apt-
```
```python
get
```
```python
install
```
```python
python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-
```
```python
22
```
```python
-dev
```
## 2、从官网下载最新opencv源码（2.4以上）
[http://sourceforge.net/projects/opencvlibrary/](http://sourceforge.net/projects/opencvlibrary/)
或者github
## 3、编译opencv
将opencv放至任意目录，解压
unzip opencv-3.0.0-rc1.zip
创建编译目录，编译
```python
cd ~/opencv-
```
```python
3.0
```
```python
.
```
```python
0
```
```python
-
```
```python
rc1
```
```python
mkdir
```
```python
release
cd release
cmake
```
```python
-D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/
```
```python
local ..
```
```python
make
```
```python
su
```
```python
do
```
```python
make
```
```python
install
```
## 4、测试opencv
## 1) 创建工作目录
mkdir~/opencv-lena
cd ~/opencv-lena
gedit DisplayImage.cpp
## 2) 编辑如下代码
\#include <stdio.h>\#include<opencv2/opencv.hpp>usingnamespacecv;intmain(intargc,char**argv )
{if( argc !=2){printf("usage: DisplayImage.out <Image_Path>\n");return-1;}Mat image;image= imread( argv[1],1);if( !image.data ){printf("No image data \n");return-1;}namedWindow("Display Image", WINDOW_AUTOSIZE );imshow("Display Image", image);waitKey(0);return0;
}
## 3) 创建CMake编译文件
gedit CMakeLists.txt写入如下内容
cmake_minimum_required(VERSION2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
add_executable( DisplayImage DisplayImage.cpp)
target_link_libraries( DisplayImage${OpenCV_LIBS})
## 4) 编译
cd ~/opencv-lena
cmake .make
## 5) 执行
此时opencv-lena文件夹中已经产生了可执行文件DisplayImage，下载lena.jpg放在opencv-lena下，运行
```python
.
```
```python
/DisplayImage lena.jpg
```
## 6) 结果
![](http://img0.tuicool.com/3URbimn.jpg%21web)



