# 编译OpenCV遇到Qmake问题 - wishchinYang的专栏 - CSDN博客
2015年04月02日 16:16:36[wishchin](https://me.csdn.net/wishchin)阅读数：1677
1、Ubuntu安装OpenCv，出现：qmake: could not exec '/usr/lib/x86_64-linux-gnu/qt4/bin/qmake': No such file or directory
必须安装QT：
sudo apt-get install qt-sdk
2、又出现 
下列软件包有未满足的依赖关系：
 unity-control-center : 依赖: libcheese-gtk23 (>= 3.4.0) 但是它将不会被安装
                        依赖: libcheese7 (>= 3.0.1) 但是它将不会被安装
3、于是：
先安装一个aptitude, 即在终端输入：
sudo apt-get install aptitude
安装完之后，使用此工具安装qt-sdk可用。即在终端输入：
sudo aptitude install qt-sdk
接受推荐方案：删除了125个包，心疼！
