# CentOS 下 Codeblocks 的 安装 + 汉化 以及 基本使用介绍 - Soul Joy Hub - CSDN博客

2016年06月13日 09:56:19[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：440


                Codeblocks 安装

注：在root用户下运行下列命令 

1.安装gcc,需要c和c++两部分,默认安装下,CentOS不安装编译器的,在终端输入以下命令即可 

yum install gcc 

yum install gcc-c++ 

2.安装gtk2-devel,因为默认已经安装了正式产品需要的支持库,但是没有安装开发所需要的文档. 

yum install gtk2* 

3、安装wxGTK,首先从地址http://sourceforge.net/projects/wxwindows/files/wxAll/2.8.9/wxWidgets-2.8.9.tar.gz/download下载wxGTK安装包，解压之后，进入目录后执行命令 

./configure --prefix=/usr --enable-xrc --enable-monolithic --enable-unicode 

 make 

 make install 

4、安装codeblocks，下载codeblocks安装包，下载地址：http://www.codeblocks.org/downloads/ 

这里有两种方式进行安装： 

第一种方式是直接下载rpm包，可以直接进行安装，但是rpm安装形式，没有默认安装可视设计wxsmith。 

第二种方式是下载源代码包，后缀为.tar.gz(.tar.bz2)，下载之后，解压缩，进入目录，执行命令 

./configure --prefix=/usr --with-contrib-plugins=all（如果只是./configure那默认情况是不会安装可视设计wssmith，如果不需要图形界面开发的话可以直接使用第一种安装方式） 

make 

make install 

5.安装 DOS 窗口 ：yum install xterm 

6.安装 调试器：yum install gdb 

# codeblocks 汉化

汉化包下载地址：
[http://pan.baidu.com/share/link?shareid=162389&uk=503806257](http://pan.baidu.com/share/link?shareid=162389&uk=503806257)

进入文件夹位置：

cd /usr/share/codeblocks

新建文件夹：

mkdir locale

进入locale文件夹：

cd locale

复制zh_CN文件夹进来

cp -R /......../ . （zh_CN文件夹的绝对路径，)注意：后面的空格点都不能省略。文件名字不能改！！！

![](http://images.cnitblog.com/blog/709693/201501/242337531252784.png)

![](http://images.cnitblog.com/blog/709693/201501/242337588753274.png)

![](http://images.cnitblog.com/blog/709693/201501/242337596563146.png)

重启 codeblocks ，完成


# Codeblocks 基本调试使用

![](http://images.cnitblog.com/blog/709693/201501/242338009381105.png)

![](http://images.cnitblog.com/blog/709693/201501/242338047354265.png)

![](http://images.cnitblog.com/blog/709693/201501/242338070475639.png)

## 编码提示

![](http://images.cnitblog.com/blog/709693/201501/242338086726168.png)

## 设置断点 

在需要设置断点处，右击左边行号，Add breakpoint，则出现一个红色的点(可以同时设置多个，前提是不能在debug的运行模式下)。


![](http://images.cnitblog.com/blog/709693/201501/242338091107768.png)

## 调试运行

 Debug->Start(F8) 

## **查看变量 **

Debug->Debugging windows->Watches

其中，在空白区域右击Add watch，即可出现对话框

![](http://images.cnitblog.com/blog/709693/201501/242338100785697.png)

## 单步执行 

Next Line：F7 Next Instruction(指令)：Alt＋F7 

Step into：Shift＋F7 

Step out：Shift＋Ctrl＋F7 
Run to cursor：F4可实现从断点A运行到断点B的功能。

![](http://images.cnitblog.com/blog/709693/201501/242338104389740.png)

