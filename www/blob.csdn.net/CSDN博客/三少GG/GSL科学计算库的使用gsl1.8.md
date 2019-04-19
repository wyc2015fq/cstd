# GSL科学计算库的使用gsl1.8 - 三少GG - CSDN博客
2011年11月21日 19:45:03[三少GG](https://me.csdn.net/scut1135)阅读数：3417
# [GSL科学计算库的使用](http://www.cnblogs.com/duke/archive/2011/01/19/1939255.html)
　　粒子滤波的代码中使用了GSL库，我从网上下载的gsl-1.11-windows-binaries.zip直接解压，类似OpenCV的配置方法，附加包含目录、附加库目录、附加依赖项，就能准确使用了。
　　代码中包含库的头文件时必须的：#include <gsl/gsl_rng.h> #include <gsl/gsl_randist.h>。
      开头有这样几条语句：
```
gsl_rng_env_setup();
rng = gsl_rng_alloc( gsl_rng_mt19937 );
gsl_rng_set( rng, time(NULL) );
```
也不知道是啥意思！！
具体配置参考
[http://www.dakaren.com/index.php/archives/412.htm](http://www.dakaren.com/index.php/archives/412.htm)
”
本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/mooncircle/archive/2010/04/30/5545448.aspx
注：由上文本人在win7系统下不能实现生成lib文件，总是说lib不是系统命令，所以本人有尝试了以下方法。
由def库文件导出lib库文件
研究SIFT算法的match部分时，需要gsl库支持。在windows下安装gsl1.8后，发现安装目录lib下并没有libgsl.lib,libgslcblas.lib这两个文件，倒是有两个扩展名为def和a(linux下库文件包格式)的文件。
**在windows下其实是可以通过def导出lib格式的库文件的**。方法如下：
从**开始菜单处打开vs2005 tools下的Visual Studio 2005 命令提示，进入gsl库的lib目录下依次输入以下两条语句：lib /machine:i386 /def:libgsl.deflib /machine:i386 /def:libgslcblas.def再看lib目录下，发现有了libgsl.lib,libgslcblas.lib这两个文件。**
“
[http://blog.csdn.net/jyc1228/archive/2009/08/12/4437447.aspx](http://blog.csdn.net/jyc1228/archive/2009/08/12/4437447.aspx)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++[http://hi.baidu.com/pengjun/blog/item/2e810ff40b867f66ddc47492.html](http://hi.baidu.com/pengjun/blog/item/2e810ff40b867f66ddc47492.html)
使用GNU Scientific Library(GSL)及GSL的配置
2009年05月26日 12:45
作者：彭军
邮件：hellotim##foxmail.com
近来帮朋友编译一个SIFT匹配的程序，可是里面不仅用到了OpenCV，而且用到了GSL，但是在编译的过程中，OpenCV的配置是没有什么问题了，可是GSL一直不行，在用VC6.0进行编译的时候，总是出现类似与如下的错误：
xform.obj : error LNK2001: unresolved external symbol _gsl_rng_free
xform.obj : error LNK2001: unresolved external symbol _gsl_rng_set
xform.obj : error LNK2001: unresolved external symbol _gsl_rng_alloc
xform.obj : error LNK2001: unresolved external symbol _gsl_rng_mt19937
xform.obj : error LNK2001: unresolved external symbol _gsl_sf_choose
xform.obj : error LNK2001: unresolved external symbol _gsl_rng_uniform_int
一想既然是LINK时的错误，肯定是lib文件有关系了。看到GSL的lib文件夹下面并没有后缀名为lib的文件。但是又看到有后缀名为def的文件，我知道通过def文件是可以导出dll和lib文件的了。而且在网络上也看到可以用VS的lib命令来导出适合VS用的.lib文件，于是打开cmd窗口。
D:\Program Files\GnuWin32\lib>lib /machine:i386 /def:libgsl.def
Microsoft (R) Library Manager Version 6.00.8168
Copyright (C) Microsoft Corp 1992-1998. All rights reserved.
   Creating library libgsl.lib and object libgsl.exp
D:\Program Files\GnuWin32\lib>lib /machine:i386 /def:libgslcblas.def
Microsoft (R) Library Manager Version 6.00.8168
Copyright (C) Microsoft Corp 1992-1998. All rights reserved.
   Creating library libgslcblas.lib and object libgslcblas.exp
D:\Program Files\GnuWin32\lib>
可以看到lib文件夹下已经有了libgsl.lib和libgslcblas.lib这样的话，我们只要将这两个lib文件添加到工程Link页中就可以了，如下：
![](http://hiphotos.baidu.com/pengjun/pic/item/d4a21e30b14a68bda8018e4a.jpg)
![](http://hiphotos.baidu.com/pengjun/pic/item/55ed3812f5746075f919b84b.jpg)
当然了，要用GSL的话，也需要在VC的目录中添加GSL的include目录、lib目录和bin目录，如下：
![](http://hiphotos.baidu.com/pengjun/pic/item/37894854d7520d70574e0055.jpg)
![](http://hiphotos.baidu.com/pengjun/pic/item/7b4c267fca5dd72c29388a56.jpg)
![](http://hiphotos.baidu.com/pengjun/pic/item/0f44b58f5c2644caf11f3656.jpg)
从添加的Include目录可以看出，当你用GSL里面的头文件时，需要类似与这样的引用：
#include <gsl/gsl_rng.h>
当然，如果你在添加Include目录是添加的是：D:\Program Files\GnuWin32\include\gsl
那么就不需要前面的gsl，只要如此引用就可以了：#include <gsl_rng.h>
还有一点需要说明的是GSL中已经没有gsl_rng_free.h了，所以你需将其改为gsl_rng.h
你可以在这里下载这个经典SIFT的图像匹配程序的VC程序：
[http://download.csdn.net/source/1352136](http://download.csdn.net/source/1352136)
你可以在这里下载GSL（GNU Scientific Library）1.8：
[http://download.csdn.net/source/1352070](http://download.csdn.net/source/1352070)

