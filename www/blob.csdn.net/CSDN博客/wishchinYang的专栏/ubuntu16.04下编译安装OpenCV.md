# ubuntu16.04下编译安装OpenCV - wishchinYang的专栏 - CSDN博客
2014年10月03日 11:31:01[wishchin](https://me.csdn.net/wishchin)阅读数：9063
一： 预先配置
        为使OpenCV的安装在编译时更完备，预先安装好所有的开发平台；
二：编译OpenCV
       在OpenCV官网下载UNIX的源码包；
       安装一下软件：  
```python
sudo apt-get install build-essential libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg62-dev libtiff4-dev cmake libswscale-dev libjasper-dev
```
       编译源码：转移到源码文件夹
```cpp
cmake .
    make
    sudo make install
详细编译：
cd ~/opencv-2.4.10
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ~/Tools/opencv-2.4.10
sudo make
sudo make install
```
三：配置Library
      打开/etc/ld.so.conf.d/opencv.conf，在末尾加入/usr/local/lib ;
      编辑添加目录：     
```python
sudo ldconfig
```
编辑/etc/bash.bashrc，加入
```python
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
   export PKG_CONFIG_PATH
```
四：测试安装
       打开Python，输入：
```cpp
import cv2.cv  as cv
```
       或者：将opencv源码包下的example/c拷贝出来，执行./build_all.sh
       编译完后试一个人脸检测的程序：
```
./facedetect --cascade="/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml" --scale=1.5 lena.jpg
```
Tips：
       1.关于Bash编译方法，参照opencv源码下example/c/build_all.sh
       2.有些路径的OpenCV注意大小写，如example/c下facedetect运行时的路径
五：官方的OpenCV编译安装指南
[http://www.samontab.com/web/2012/06/installing-opencv-2-4-1-ubuntu-12-04-lts/](http://www.samontab.com/web/2012/06/installing-opencv-2-4-1-ubuntu-12-04-lts/)
