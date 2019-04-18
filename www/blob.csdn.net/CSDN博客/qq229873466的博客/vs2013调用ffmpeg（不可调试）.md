# vs2013调用ffmpeg（不可调试） - qq229873466的博客 - CSDN博客

2017年09月25日 21:52:48[qq229873466](https://me.csdn.net/qq229873466)阅读数：305


环境：win10,， VS013 debug 32位。

1.下载ffmpeg（shared和dev）。[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)，编译32位工程就下载32位的；

2.然后建立vs解决方案，在VC++目录》包含文件：输入头文件路径，例如..\ffmpeg-20170921-183fd30-win32-dev\include;；

3.VC++目录》库目录：输入lib路径，例如..\ffmpeg-20170921-183fd30-win32-dev\lib;；

4.调试》环境：输入动态库路径，例如PATH=..\ffmpeg-20170921-183fd30-win32-shared\bin；

5.源文件加入avio_reading.c 测试，开头加入#define inline _inline，调试》命令参数 输入一个音频文件路径；

6.链接器》输入 ：

advapi32.lib;Secur32.lib;ws2_32.lib;avcodec.lib;avdevice.lib;avfilter.lib;avformat.lib;avutil.lib;swresample.lib;swscale.lib;；

7.C/C++》代码生成》运行库 ：/MTd；

8.编译运行。

参考工程：[http://pan.baidu.com/s/1ge7esqn](http://pan.baidu.com/s/1ge7esqn)

更具体参考：

[ffmpeg在windows下使用MinGW+msys+VS2015编译(x86或64版本，链接x264库](http://blog.csdn.net/uselym/article/details/78040225)


