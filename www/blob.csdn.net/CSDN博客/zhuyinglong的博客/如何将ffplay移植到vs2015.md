# 如何将ffplay移植到vs2015 - zhuyinglong的博客 - CSDN博客





2017年10月16日 17:59:08[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：291








# 前言

在ffmpeg+SDL系列博客中介绍的是使用ffmpeg单独提取音频，单独提取视频，使用SDL单独进行播放的，这并不能称为一个播放器。播放器最主要的就是做一个音视频同步。ffmpeg源码中会带有ffplay的源码，ffplay是基于FFMPEG+SDL的开源播放器。看懂了ffplay，FFMPEG的一些常用接口用法就可以了解了。最主要的是其中有音视频同步算法。开发播放器就可以就可以参考ffplay的写法。

下面介绍如何在ffplay移植到vs2015中。和之前一样ffmpeg版本为3.3.3。原版的ffplay是C语言写的，改扩展名为cpp后需要修正一些cpp关键字参数及一些C语言转C++的强制类型转换，不过基本上花点时间就能解决，没什么难度。下面以C语言版本为例进行移植说明。C++版本步骤与之类似。




# 移植

将ffplay移植到vs2015的步骤还是还是比较简单的。

首先，从ffmpeg中拷贝如下5个文件，如下图：

![](https://img-blog.csdn.net/20171016175923528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

说明：

1、ThirdParty文件夹为ffmpeg和SDL2的头文件、lib引导库。

2、config.h在\compat\avisynth\avs目录下。


然后，新建win32控制台程序空工程，将上述5个文件添加到建立的win32控制台程序空工程中。

再然后，配置工程属性，如下图：

![](https://img-blog.csdn.net/20171016180412344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后，做一些兼容性修改。

具体修改的地方大家可以下载本人修改后的工程使用文件对比工具与原版进行对比，看下具体修改的地方。




# 下载

修改好的vs2015工程（C语言版）下载地址如下：

[https://github.com/zhuyinglong/ffplay](https://github.com/zhuyinglong/ffplay)

修改好的vs2015工程（C++语言版）下载地址如下：


[https://github.com/zhuyinglong/ffplaycore](https://github.com/zhuyinglong/ffplaycore)




