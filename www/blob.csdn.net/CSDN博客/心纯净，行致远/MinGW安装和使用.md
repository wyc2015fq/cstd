# MinGW安装和使用 - 心纯净，行致远 - CSDN博客





2018年04月02日 20:43:54[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：201











目录[(?)](https://blog.csdn.net/xhhjin/article/details/8449251)[[-]](https://blog.csdn.net/xhhjin/article/details/8449251#)
- [一MinGW安装](https://blog.csdn.net/xhhjin/article/details/8449251#t0)- [1选择安装版本Repository Catalogues](https://blog.csdn.net/xhhjin/article/details/8449251#t1)
- [2设置安装目录](https://blog.csdn.net/xhhjin/article/details/8449251#t2)
- [3选择安装组件](https://blog.csdn.net/xhhjin/article/details/8449251#t3)
- [4等待下载并安装完成](https://blog.csdn.net/xhhjin/article/details/8449251#t4)
- [5设置MinGW环境变量](https://blog.csdn.net/xhhjin/article/details/8449251#t5)
- [6查看GCC版本信息](https://blog.csdn.net/xhhjin/article/details/8449251#t6)

- [二MinGW使用](https://blog.csdn.net/xhhjin/article/details/8449251#t7)- [1测试编译C](https://blog.csdn.net/xhhjin/article/details/8449251#t8)
- [2编译test](https://blog.csdn.net/xhhjin/article/details/8449251#t9)
- [3执行](https://blog.csdn.net/xhhjin/article/details/8449251#t10)





MinGW全称Minimalist GNU For Windows，是个精简的Windows平台C/C++、ADA及Fortran编译器，相比Cygwin而言，体积要小很多，使用较为方便。MinGW提供了一套完整的开源编译工具集，以适合Windows平台应用开发，且不依赖任何第三方C运行时库。

MinGW包括：
- 一套集成编译器，包括C、C++、ADA语言和Fortran语言编译器
- 用于生成Windows二进制文件的GNU工具的（编译器、链接器和档案管理器）
- 用于Windows平台安装和部署MinGW和MSYS的命令行安装器（mingw-get）
- 用于命令行安装器的GUI打包器（mingw-get-inst）

注:本文测试环境为Windows 7 SP1+MinGW 20120426。

## 一、MinGW安装

MinGW官方网址：[http://www.mingw.org/](http://www.mingw.org/)

下载地址：[http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/](http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/)

下载后只是个安装器，需要在线下载真正的安装内容，所以速度不会快。具体步骤如下：

### 1、选择安装版本（Repository Catalogues）

有两个选择，一个是当前打包版本，另一个是下载最新版本。只要你的安装器是从官方下载的，选择前者即可，结果一般就是最新版本。

### 2、设置安装目录

默认安装到C:\MinGW

### 3、选择安装组件

这个根据大家需要选择安装组件（C编译器、C++编译器、Fortran编译器、ObjC编译器、Ada编译器等），一般选择C/C++编译器即可，看各位用途确定，组件列表中还有MSYS小系统等可供选择。

### 4、等待下载并安装完成

### 5、设置MinGW环境变量

依次鼠标点击桌面“我的电脑”->选择左侧的“高级系统设置”，选择“高级”->“环境变量”，然后再Path里增加;C:\MinGW\bin声明。点击确定。

小提示：设置完成后，Windows似乎不会自动更新环境变量，除非重启机子，那么我们可以通过在下面的命令行中，胡乱设置一下PATH（比如SET PATH=C:\），然后退出命令行，那么系统环境变量就会被强制刷新。

注意：箭头指的两个分号一定要加上并且分号一定要在英文输入模式下输入


![](https://img-blog.csdn.net/20140706193030656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdtaW5nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


原文可以直接查看是否安装成功，不过我是重启了一下才成功的，重启之后按如下操作：

在CMD中输入gcc -v，看看有没有安装成功，如果出现一下画面说明安装成功![](https://img-blog.csdn.net/20140706193244453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdtaW5nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 6、查看GCC版本信息

开始运行，输入CMD，回车并运行，输入gcc -v查看编译器版本。如下图：

![](https://img-my.csdn.net/uploads/201212/28/1356707507_5809.jpg)


## 二、MinGW使用

### 1、测试编译C++

测试文件test.cpp，代码如下（下面为C代码，只是使用G++来编译而已）




|`1`|`#include <stdio.h>`|
|----|----|
|`2`||
|----|----|
|`3`|`int``main() {`|
|----|----|
|`4`|`    ``printf``(``"Hello World!"``);`|
|----|----|
|`5`|`        ``return``0;`|
|----|----|
|`6`|`}`|
|----|----|



2、编译test

编译命令如下：

g++ test.cpp -o test

输出结果为：test.exe

### 3、执行

输入test回车，结果如下：

![](https://img-my.csdn.net/uploads/201212/28/1356707389_9840.jpg)


参考资料：[http://www.mingw.org/wiki/InstallationHOWTOforMinGW](http://www.mingw.org/wiki/InstallationHOWTOforMinGW)

文章来自：[http://www.metsky.com/archives/588.html](http://www.metsky.com/archives/588.html)





