# F2C用法 - 工作笔记 - CSDN博客





2012年02月01日 09:23:27[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7255









f2c是一个能将Fortran程序转换成c程序的软件和库，其官网为[http://www.netlib.org/f2c/](http://www.netlib.org/f2c/)。

官网中的资源如下：

f2c.1：f2c程序使用手册

f2c.pdf：介绍了转换的原理

libf2c.zip（combined libf77, libi77, with several makefile variants）：利用命令行nmake -f makefile.vc编译，得到一个vcf2c.lib。

mswin：下载win32下的命令行程序f2c.exe，用这个程序来转换文件。

src：没有下载，估计是f2c.exe的源代码

readme：没仔细看，瞄了一眼，仿佛是介绍如何编译f2c.exe和libf2c。

使用的大体过程：

1）用f2c.exe将f程序转成c程序

2）先加上f2c.h头文件的引用和链接vcf2c.lib库，编译链接c程序

可参考[http://blog.chinaunix.net/u/6140/showart_313600.html](http://blog.chinaunix.net/u/6140/showart_313600.html)和[http://www.stat.rice.edu/~helpdesk/howto/Fortran-Dll-SPlus-R.txt](http://www.stat.rice.edu/~helpdesk/howto/Fortran-Dll-SPlus-R.txt)



[f2c](http://www.netlib.org/f2c/libf2c.zip)[完](http://www.netlib.org/f2c/libf2c.zip)整源代码(126KB)

[f2c的mswin平台命令行](http://f2c/mswin/)(131KB)

[f2c使用说明](http://www.netlib.org/f2c/f2c.1)

[f2c更新记录](http://www.netlib.org/f2c/changes)



其实f2c的使用方法非常简单：
**f2c [	option ... ] file ...**
一般option取默认即可，如 **f2c romform.f** 。

需要注意的是，fortran的文件后缀必须是.f或者.F，.for是不认的，而且严格执行72列的限制。




