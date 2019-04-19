# 如何编译syncml-ctoolkit开源软件包 - 2019跟着小虎玩着去软考 - CSDN博客
2010年03月22日 15:31:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：2589
**如何编译syncml-ctoolkit开源软件包**
# 一.SyncML开发的源代码，从何处取得：
[http://sourceforge.net/project/showfiles.php?group_id=58216](http://sourceforge.net/project/showfiles.php?group_id=58216)
下载：
1. syncml-ctoolkitC语言软件开发包
2. toolkit-manual 开发包说明手册
# 二.如何在WindowsXP上编译SyncML：
## 1.下载MinGW 5.1.4开发包，进行安装：
**可以到如下网站下载：****[http://sourceforge.net/project/showfiles.php?group_id=2435](http://sourceforge.net/project/showfiles.php?group_id=2435)**
**或者去：****[http://www.mingw.org/](http://www.mingw.org/)**
**1.mingw32-make.tar.gz**
**2.mingw32-make-3.80.0-3.tar.gz**
## 2. 下载MSYS-1.0.11软件，进行安装：
**该软件是模拟****UNIX****操作系统环境，提供一个****Shell****的操作界面终端；**
**1.MSYS-1.0.11-20090120-dll.tar.gz**
**2.MSYS-1.0.10.exe**
## 3．拷贝一份af_irda.h头文件到C:/MinGW/include目录下
**在网络上找一份头文件****af_irda.h****（可以在****Google****网站，直接搜索“****af_irda.h****”该字符串），放置于如下位置：****C:/MinGW/include**
# 三.如何进行编译：
## 1.如何编译库：
A．运行msys虚拟的UNIX环境，文件在：C:/msys/1.0/msys.bat；
B．进入工作目录，也就是存放代码目录下syncml_win/toolkit_4_4/src/bld/win
如：cd D:/Project/syncml_win/toolkit_4_4/src/bld/win
注意：UNIX环境，所以应该输入“/”，而不是Windows上“/”；
C．make all即可
Make的其他几个参数是：sml，xpt，clean分别是编译sml,xpt动态链接库，以及清除所有的；
## 2.如何编译Sample示例：
A．先编译生成sml.dll，iobex.dll，xpt.dll，xptawsp.dll，xpthttp.dll，xptobex.dll，xptwsp.dll等
B．进入Sample例子目录syncml_win/toolkit_4_4/src/examples：
C．分别编译sender,receiver客户端和服务器程序；
使用的命令：make all(clean，new)三个命令
详细参看Makefile脚本文件；
# 四．如何执行，看Demo例子的运行效果：
## 1．首先运行服务器：./receiver.exe &(&表示程序后台运行)
## 2．然后运行客户端例子：./sender.exe
