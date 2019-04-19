# Android原生(Native)C开发之六：libpng移植笔记 - xqhrs232的专栏 - CSDN博客
2010年03月02日 11:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：931标签：[android																[c																[存储																[linker																[network																[compiler](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=network&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址：：[http://www.csharpwin.com/dotnetspace/4096r1455.shtml](http://www.csharpwin.com/dotnetspace/4096r1455.shtml)
libpng（[http://www.libpng.org/](http://www.libpng.org/)）是读取PNG（Portable Network Graphic Format）文件的最官方的函数库，所以在介绍libpng之前，有必要先介绍下PNG文件。
PNG是20世纪90年代中期开始开发的图像文件存储格式，其目的是企图替代GIF和TIFF文件格式，同时增加一些GIF文件格式所不具备的特性。流式网络图形格式(Portable Network Graphic Format，PNG)名称来源于非官方的“PNG's Not GIF”，是一种位图文件(bitmap file)存储格式，读成“ping”。PNG用来存储灰度图像时，灰度图像的深度可多到16位，存储彩色图像时，彩色图像的深度可多到48位，并且还可存储多到16位的α通道数据。PNG使用从LZ77派生的无损数据压缩算法。
PNG用了无损数据压缩算法，用的压缩库上是上一篇介绍的zlib，编译libpng前，要先编译zlib（请参见上一篇：[Android原生(Native)C开发之五：zlib移植笔记](http://www.csharpwin.com/dotnetspace/4095r6563.shtml)）。
先从sourceforge下载libpng的最新版本：1.2.33，请选择without config script版本：[bz2格式](http://prdownloads.sourceforge.net/libpng/libpng-1.2.33-no-config.tar.bz2?download)，[zip格式](http://prdownloads.sourceforge.net/libpng/lpng1233.zip?download)，这个版本没有configure文件，因为基本上不需要改动libpng的设置，就能编译通过。
将下载的源码包解开，得到时一个名为 libpng-1.2.33 的目录，将 scripts/makefile.gcc 复制到 libpng-1.2.33/Makefile，将此文件的 8 - 27 行改成如下即可（粗体为增加部分）：
...
ZLIBINC = ../zlib-1.2.3
ZLIBLIB = ../zlib-1.2.3
# Compiler, linker, lib and other tools
CC = arm-none-linux-gnueabi-gcc
LD = $(CC)
AR_RC = arm-none-linux-gnueabi-ar rcs
RANLIB = arm-none-linux-gnueabi-ranlib
RM_F = rm -f
CDEBUG = -g -DPNG_DEBUG=5
LDDEBUG =
CRELEASE = -O2
LDRELEASE = -s -static
#CFLAGS = -W -Wall $(CDEBUG)
CFLAGS = -W -Wall $(CRELEASE)
#LDFLAGS = $(LDDEBUG)
LDFLAGS = $(LDRELEASE)
LIBS = -lz -lm
...
在Cygwin或Linux命令行方式进入libpng-1.2.33目录，输入: make 即可成功编译libpng，得到时的静态库libpng.a及可执行文件 pngtest。
用 emulator -noskin 命令启动 Android 模拟器，将pngtest上传到模拟器，即可测试pngtest:
$adb shell mkdir /dev/sample
$adb push pngtest /dev/sample
$adb push pngtest.png /dev/sample
$adb shell chmod 777 /dev/sample/pngtest
$adb shell
#cd /dev/sample
#./pngtest pngtest.png
Testing libpng version 1.2.33
   with zlib   version 1.2.3
 libpng version 1.2.33 - October 31, 2008
   Copyright (c) 1998-2008 Glenn Randers-Pehrson
   Copyright (c) 1996-1997 Andreas Dilger
   Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
 library (10233): libpng version 1.2.33 - October 31, 2008
 pngtest (10233): libpng version 1.2.33 - October 31, 2008
 sizeof(png_struct)=984, sizeof(png_info)=288
Testing pngtest.png:
 Pass 0: rwrwrwrwrwrwrwrwrw
 Pass 1: rwrwrwrwrwrwrwrwrw
 Pass 2: rwrwrwrwrwrwrwrw
 Pass 3: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
 Pass 4: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
 Pass 5: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
         rwrwrwrw
 Pass 6: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
         rwrwrwrwrw
 PASS (9782 zero samples)
 Filter 0 was used 21 times
 Filter 1 was used 15 times
 Filter 2 was used 52 times
 Filter 3 was used 10 times
 Filter 4 was used 33 times
 tIME = 7 Jun 1996 17:58:08 +0000
libpng passes test
其中"$"为cygwin的命令提示符，"#"为 adb shell的命令提示符，测试如下图所示：
![](http://writeblog.csdn.net/upload/s7.jpg)
