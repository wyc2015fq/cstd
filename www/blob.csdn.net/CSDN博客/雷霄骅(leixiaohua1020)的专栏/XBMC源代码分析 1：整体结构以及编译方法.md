# XBMC源代码分析 1：整体结构以及编译方法 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月31日 00:17:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：44









XBMC（全称是XBOX Media Center）是一个开源的媒体中心软件。XBMC最初为Xbox而开发，可以运行在Linux、OSX、Windows、Android4.0系统。我自己下载了一个然后体验了一下，感觉确实不错，和Windows自带的媒体中心差不多。

XBMC项目首页：[http://xbmc.org/](http://xbmc.org/)

![](https://img-blog.csdn.net/20131221203519140)

XBMC差不多是我接触到的开源多媒体项目中体积最大的了。但是它的编译方法却出乎意料的简单。我按照它的Wiki上面说的步骤去做，非常顺利的完成了编译，没有遇到任何错误，赞一个。

**下面简述一下它的编译方法。**

**前提条件**：

1.Visual C++ 2010

2.Microsoft DirectX SDK （August 2009 或更晚的版本）

3.Git

4.JRE

**编译**：

注意：需要下载很多东西，所以需要联网

1.使用Git下载源代码。Git地址：git://github.com/xbmc/xbmc.git

2.运行DownloadBuildDeps.bat （所在目录 project\BuildDependencies）：下载编译项目所需要的依赖项

3.运行DownloadMingwBuildEnv.bat （所在目录 project\BuildDependencies） ：下载编译ffmpeg库所需要的依赖项

4.运行buildmingwlibs.bat （所在目录 project\Win32BuildSetup）： 编译ffmpeg库

 5.以下二选一。一般情况下选第二个就可以了。

（1）BuildSetup.bat （所在目录 project\Win32BuildSetup）：只有需要直接编译一个打包文件的时候，才推荐使用该批处理。

（2）extract_git_rev.bat ： 如果是为了调试，并且使用 VC++ 2010 进行编译，推荐使用该批处理。

6.打开project\VS2010Express\XBMC for Windows.sln，就可以编译了。



**下面对XBMC源代码进行一个整体分析：**

源代码的目录结构如下图所示。我把其中比较主要的文件夹下面标记了一条红线。

![](https://img-blog.csdn.net/20131223185850031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这几个主要文件夹的作用如下（其他文件夹就不再细说了）：

> 
addons：附加元件。比如说XBMC的皮肤文件，屏幕保护文件，可视化效果文件等等。

docs：文档。

language：语言文件。

project：项目工程文件。

xbmc：源代码

lib：调用的各个库。比如说libavcodec。




XBMC项目解决方案的目录如下图所示。可以看出项目工程数量是极其巨大的。

其中名字为“XBMC”的工程是主程序。

ImageLib_XXX是图片处理的工程。

libXBMC_XXX是完成XBMC各种功能的工程。

visXXX是各种可视化效果的工程。

![](https://img-blog.csdn.net/20131223191046859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



我们来看一下“XBMC”工程的目录。该工程下源文件的数量也是十分庞大的。不同功能的类被放到了不同的文件夹中，显得还是比较井然有续的：

其中“core”文件夹中存放核心的类

“addon”文件夹中存放和addon相关的类

“music”文件夹中存放和音乐功能相关的类

“video”文件夹中存放和影视功能相关的类

“settings”文件夹中存放和设置功能相关的类

此处不一一例举



![](https://img-blog.csdn.net/20131223191053421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






