# 在windows使用vs2008编译live555 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月16日 16:55:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：68
个人分类：[Live555](https://blog.csdn.net/leixiaohua1020/article/category/1362936)










１．live555源代码简介
liveMedia项目的源代码包括四个基本的库，各种测试代码以及IVE555 Media Server。

四个基本的库分别是UsageEnvironment&TaskScheduler，groupsock，liveMedia，BasicUsageEnvironment。

UsageEnvironment 和TaskScheduler类用于事件的调度，实现异步读取事件的句柄的设置以及错误信息的输出。另外，还有一个HashTable类定义了一个通用的 hash表，其它代码要用到这个表。这些都是抽象类，在应用程序中基于这些类实现自己的子类。

groupsock类是对网络接口的封装，用于收发数据包。正如名字本身，Groupsock主要是面向多播数据的收发的，它也同时支持单播数据的收发。Groupsock定义了两个构造函数
 Groupsock(UsageEnvironment& env, struct in_addr const& groupAddr,
 Port port, u_int8_t ttl);
 Groupsock(UsageEnvironment& env, struct in_addr const& groupAddr,
 struct in_addr const& sourceFilterAddr,
 Port port);
前 者是用于SIM（source-independent multicast）组，后者用于SSM（source-specific multicast）组。groupsock库中的Helper例程提供了读写socket等函数，并且屏蔽了不同的操作系统之间的区别，这是在 GroupsockHelper.cpp文件中实现的。

liveMedia库中有一系列类，基类是Medium，这些类针对不同的流媒体类型和编码。

各种测试代码在testProgram目录下，比如openRTSP等，这些代码有助于理解liveMedia的应用。

LIVE555 Media Server是一个纯粹的RTSP服务器。支持多种格式的媒体文件:

 * TS流文件，扩展名ts。
 * PS流文件，扩展名mpg。
 * MPEG-4视频基本流文件，扩展名m4e。
 * MP3文件，扩展名mp3。
 * WAV文件(PCM)，扩展名wav。
 * AMR音频文件，扩展名.amr。
 * AAC文件，ADTS格式，扩展名aac。
２．在ｗｉｎｄｏｗｓ下编译ｌｉｖｅ５５５
　　(1).下载live555,http://www.live555.com/
 (2). 编辑 win32config,TOOLS32=C:\Program Files\Microsoft Visual Studio 9.0\VC
 即保证这个路径是你的Vs2008 路径。
 (3). 编辑"LINK_OPTS_0 = $(linkdebug) msvcirt.lib" in win32config to "LINK_OPTS_0 = $(linkdebug) msvcrt.lib"，由于编译器所要LINK的运行库不一样了
 (4). 修改groupsock/makefile.head,替换"INCLUDES = -Iinclude -I../UsageEnvironment/include" 为"INCLUDES = -Iinclude -I../UsageEnvironment/include -DNO_STRSTREAM".
 (5). 运行genWindowsMakefiles.cmd 生成VS能够编译的*.mak文件
 (6). 将以下内容保存为live/compile.bat

```
call "C:\Program Files\Microsoft Visual Studio 9\VC\vcvarsall.bat"
cd liveMedia
nmake /B -f liveMedia.mak
cd ../groupsock
nmake /B -f groupsock.mak
cd ../UsageEnvironment
nmake /B -f UsageEnvironment.mak
cd ../BasicUsageEnvironment
nmake /B -f BasicUsageEnvironment.mak
cd ../testProgs
nmake /B -f testProgs.mak
cd ../mediaServer
nmake /B -f mediaServer.mak
```



有关这一点来说，建议把vs2008的编译环境加入到环境变量中，那么以后需用命令行编译程序都可行了，可以参考http://blog.chinaunix.net/u3/94873/showart_1907792.html的前部分设置VS2008的环境设置。
 (7). 在命令行下运行compile.bat,就会看到很多编译过程出现在CMD中了。

以上的编译并不是DEBUG模式，要调试时，应先在win32config加入一行"NODEBUG=1"

进行编译后，可以在要调试的程序路径下输入如：C:\works\VCCode\video\live555-latest\live\testProgs>devenv openRTSP.exe,这样就会把相关的调试环境搭建起来进行调试了。



原文地址：[http://blog.chinaunix.net/uid-506080-id-2110629.html](http://blog.chinaunix.net/uid-506080-id-2110629.html)




