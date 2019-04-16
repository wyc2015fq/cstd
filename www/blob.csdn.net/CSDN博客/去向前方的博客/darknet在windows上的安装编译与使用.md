# darknet在windows上的安装编译与使用 - 去向前方的博客 - CSDN博客





2018年07月13日 15:48:41[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：7208








# 目录



- [目录](#目录)
- [前言](#前言)- [1，操作系统环境](#1操作系统环境)
- [2，下载darknet](#2下载darknet)
- [3，下载安装Cygwin](#3下载安装cygwin)
- [4，编译darknet](#4编译darknet)
- [5，使用darknet](#5使用darknet)





# 前言

本篇文章参考这篇： 
[http://www.cnblogs.com/jackkwok/p/9045890.html](http://www.cnblogs.com/jackkwok/p/9045890.html)

但部分细节随着时间发生了变化。 

 darknet在windows上的安装编译与使用
darknet（[https://github.com/pjreddie/darknet](https://github.com/pjreddie/darknet)）实现了YOLO网络模型，能快速准确识别多达9000种物体。但其在windows系统上的安装却并非一帆风顺，本文进行总结，以免掉坑。

## 1，操作系统环境

Windows7 64位操作系统，无gpu

## 2，下载darknet

在网站[https://github.com/pjreddie/darknet](https://github.com/pjreddie/darknet)上，点击“clone or download”，然后选择“download ZIP”，下载为文件darknet-master.zip。解压到“e:\darknet-master”目录。

## 3，下载安装Cygwin

参照以下文章“windows 安装cygwin教程”进行安装。

[windows cygwin教程](https://blog.csdn.net/lvsehaiyang1993/article/details/81027399)

重点：

²        寻找映像文件时，要选择国内快速站点。比如下文推荐的sohu镜像（[http://mirrors.sohu.com/cygwin/](http://mirrors.sohu.com/cygwin/)）。笔者就是采用它，安装非常快速。

[https://blog.csdn.net/u011393033/article/details/51766876](https://blog.csdn.net/u011393033/article/details/51766876)

²        选择组件时，要加上devel目录下的make组件，否则无法执行make命令。

²        记得添加windows环境变量，最好将cygwin\bin放到path开头。

## 4，编译darknet

进入Cygwin64 Terminal，目录转移到e：\darknet-master，然后执行make。 
![这里写图片描述](https://img-blog.csdn.net/20180713153442765?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
结束后，会产生darknet.exe、libdarknet.a、libdarknet.so文件。

注意：
- 
编译darknet源码不能采用mingw，要采用cygwin。笔者首先安装的是mingw组件包，结果在编译时出现大量头文件不存心的错误。

- 
执行make前要先删除已存在的obj目录。由于笔者曾经采用过mingw进行编译，虽然失败未生成libdarknet.so文件，但obj目录下生成不少.o目标文件，此时如果直接用cygwin执行make，会出现大量莫名错误。如“对‘_XXX’未定义的引用”、/usr/lib/gcc/x86_64-pc-cygwin/6.4.0/../../../../x86_64-pc-cygwin/bin/ld: i386 架构于输入文件 libdarknet.a(shortcut_layer.o) 与 i386:x86-64 输出不兼容

- 
Examples\Go.c 文件需要修改，要加上**“#include **


## 5，使用darknet

Darknet的使用可参考如下官方网址：[https://pjreddie.com/darknet/](https://pjreddie.com/darknet/)

笔者测试的是最新的yolo V3模型，具体参见： 
[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)
输入如下命令： 

darknet detect cfg/yolov3.cfg yolov3.weights data/eagle.jpg

其中“eagle.jpg”如下图所示。YoloV3虽然花费时间11秒久了点，但对图中的鸟实现了完美识别。 
![这里写图片描述](https://img-blog.csdn.net/20180713154536582?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
打开生成的predictions.png文件如下，至此Yolo在windows上的应用取得成功。 
![这里写图片描述](https://img-blog.csdn.net/20180713154658467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







