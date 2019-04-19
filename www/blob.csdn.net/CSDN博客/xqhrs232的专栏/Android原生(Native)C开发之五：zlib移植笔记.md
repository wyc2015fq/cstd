# Android原生(Native)C开发之五：zlib移植笔记 - xqhrs232的专栏 - CSDN博客
2010年03月02日 11:20:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1060标签：[android																[压缩、解压缩																[c																[dictionary																[makefile																[wince](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=dictionary&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=压缩、解压缩&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址：：[http://www.csharpwin.com/dotnetspace/4095r6563.shtml](http://www.csharpwin.com/dotnetspace/4095r6563.shtml)
![](http://writeblog.csdn.net/upload/s6.jpg)
[zlib(http://www.zlib.net/)](http://www.zlib.net/)是一套非常流行的且开源的压缩、解压缩库，由Jean-loup Gailly与Mark Adler所开发，初版0.9版在1995年5月1日发表。zlib使用DEFLATE演算法，最初是为libpng函式库所写的，后来普遍为许多软体所使用。
zlib当前版本是 1.2.3，其移植性非常好，本人已经在Windows、Linux、WinCE、Symbian等平台成功地移植了zlib，当然也包括今天要移植的平台：Android。
首先，从zlib 的官方网站下载zlib的源代码：[zlib123.zip](http://www.zlib.net/zlib123.zip)，解压后得到一个目录zlib-1.2.3，zib已经提供好了一个Makefile，改动几行（19、20、27、36、37）就可以动手编译了，改动如下：
......
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -O2
......
LDFLAGS=libz.a -s -static
......
AR = arm-none-linux-gnueabi-ar rc
RANLIB = arm-none-linux-gnueabi-ranlib
......
进入zlib-1.2.3目录，在命令行输入：make 即可。
最后会生成两个可Android可执行文件： example 、minigzip 。
运行Android模拟器， 进入命令行，将两个可执行文件 push 至Android模拟器，再执行，命令如下：
$adb push example /dev/sample/example
$adb push minigzip /dev/sample/minigzip
$adb shell chmod 777 /dev/sample/*
$adb shell
#cd /dev/sample
#./example
zlib version 1.2.3 = 0x1230, compile flags = 0x55
uncompress(): hello, hello!
gzread(): hello, hello!
gzgets() after gzseek:  hello!
inflate(): hello, hello!
large_inflate(): OK
after inflateSync(): hello, hello!
inflate with dictionary: hello, hello!
#ls -l
ls -l
-rwxrwxrwx root     root       513228 2008-11-10 04:18 minigzip
-rwxrwxrwx root     root       517640 2008-11-10 04:18 example
-rwxrwxrwx root     root           31 2008-11-10 04:28 
foo.gz
#exit
$adb pull /dev/sample/foo.gz d:/foo.gz
其中，“$”是Linux或Cygwin命令行的提示符，#是Android命令行的提示符。
最后，用 ls -l 命令查看当前目录，会发现一个 foo.gz的文件，这个文件就是example生成的，退出adb shell，用命令： adb pull /dev/sample/foo.gz d:/foo.gz 可将模拟器上生成的文件 foo.gz pull至D盘根目录，用winrar或7zip可查看这个文件的内容，正是代码中所写的“hello, hello! ”！
至此，zlib移植Android平台运行成功！
