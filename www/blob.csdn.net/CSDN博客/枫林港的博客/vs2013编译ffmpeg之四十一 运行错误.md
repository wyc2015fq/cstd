# vs2013编译ffmpeg之四十一 运行错误 - 枫林港的博客 - CSDN博客





2018年07月12日 23:43:51[枫林港](https://me.csdn.net/u013601132)阅读数：59








# 运行错误

## 错误一

运行ffplay_g.exe的时候会弹出下面的框： 
![这里写图片描述](https://img-blog.csdn.net/20180707005334575?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
或者缺少GetNumaNodeProcessorMaskEx也是一样。

在link的时候加`-VERBOSE`就可以看到每个链接的符号出自哪里，在link的打印里面有：

> 
​      Found __imp__InterlockedCompareExchange64@20

​        Referenced in x265-static.lib(wavefront.obj)

​        Referenced in x265-static.lib(threadpool.obj)

​        Loaded kernel32.lib(KERNEL32.dll)


所以x265编译有问题。后来在网络上找到了解决办法：编译x265的wavefront.c、threadpool.c这两个.c文件的时候加上`_WIN32_WINNT=0x0501`。

## 错误二

ffplay运行的时候遇到错误，缺少GetNumaNodeProcessorMaskEx、WakeAllConditionVariable等，需要在x265的common、encoder工程的C/C++ –>Preprocessor –> Preprocessor Definition里面加上下面两个定义，需要在工程属性里面加，而不是针对单个文件：

> 
_WIN32_WINNT=0x0501

_WIN32


从代码里面搜索缺少的符号，就可以发现需要定义的宏。

运行ffmpeg-3.3的vs工程，出现错误“无法定位程序输入点setthreadgroupaffinity于动态链接库KERNEL32.dll上”。因为下载了最新的x265的代码编译生成了新的x265的库，也是同样的解决办法，在x265的common、encoder工程的C/C++ –>Preprocessor –> Preprocessor Definition里面加上下面两个定义并重新编译x265：

> 
_WIN32_WINNT=0x0501

_WIN32


## 错误三

ffmpeg-3.0开始多了个check_options函数，里面有av_assert0，在debug模式下，用MSVC跑ffmpeg会导致程序出现assert。ffmpeg-2.6.2版本都没有这个函数，ffplayer、ffprobe也没有此函数。根据options数组里面的timestamp定义，肯定会导致assert，所以直接将check_options的调用去掉。

## 0xc000007b错误

win7+VS2013+ffmpeg-3.3编译出来的32位ffmpeg在其他win7、winxp下都跑不起来，报下面的错误： 
![这里写图片描述](https://img-blog.csdn.net/20180707005436953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在网络上搜索0xc000007b，都是说directX有问题，重新安装directX9C、用DirectX Repair V3.5 (Enhanced Edition)修复都不能解决问题。

后来用远程调试，在VS2013里面的Modules窗口里面可以看到加载的dll，用本地调试和远程调试对比就可以看到加载到了哪个dll出了问题。首先看到的是加载OpenCL.dll出了问题，这个库是Intel显卡的驱动，安装opencl_runtime_16.1.1_x64_setup.msi可以解决这个问题。

然后还缺vcomp120d.dll，这个库没有查到是在哪个安装包里面，网上说在vcredist.exe里面，但安装了VS2013对应的vcredist.exe版本，还是没有这个dll。后来发现有vcomp120.dll（可能是DirectX Repair V3.5 (Enhanced Edition)修复带来的），vcomp120d.dll应该是vcomp120.dll的debug版本，所以直接将vcomp120.dll拷贝并修改文件名为vcomp120d.dll就可以解决问题。

OpenCL.dll、vcomp120d.dll也可以拷贝没有问题的win7电脑C:\Windows\SysWOW64目录下面的，并放到有问题的win7的C:\Windows\SysWOW64下面。放在ffmpeg.exe同级目录不能解决这个问题。





