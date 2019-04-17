# vs2015+ffmpeg开发环境配置【转】 - zp704393004的专栏 - CSDN博客





2018年06月23日 09:30:02[原来未知](https://me.csdn.net/zp704393004)阅读数：926








﻿﻿





本文转载自：[http://blog.csdn.net/hustlx/article/details/51014307](http://blog.csdn.net/hustlx/article/details/51014307)


# 1.在[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/) 下载最新的ffmpeg的dev版和share版，注意下载的时候严格区分x64和x86，取决于你vs2015调试的解决方案平台用什么，如果安装了错误的版本将会出现一堆无法解析的错误。

# 包含三个版本：Static、Shared以及Dev• Static   --- 包含3个应用程序：ffmpeg.exe , ffplay.exe , ffprobe.exe，体积都很大，相关的DLL已经被编译到exe里面去了。• Shared --- 除了ffmpeg.exe , ffplay.exe , ffprobe.exe之外还有一些DLL，exe体积很小，在运行时到相应的DLL中调用功能。• Dev      --- 开发者（developer）版本，里面包含了库文件xxx.lib以及头文件xxx.h，这个版本不含exe文件    把dev版本与share版本都下下来解压，dev版本文件夹中的Include和lib目录整个儿复制到VS项目目录下。接着将share版本文件夹中bin目录下对应的所有dll复制到项目路径下，如下图所示：

![](https://img-blog.csdn.net/20160330144756638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 2.在工程属性》C/C++》常规》附加包含目录：$(ProjectDir)include

比如在我的项目中：C:\Users\yckj\Documents\Visual Studio2015\Projects\watermarkUI\include

![](https://img-blog.csdn.net/20160330145114166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 点击应用，

# 在工程属性》链接器》常规》附加库目录：$(ProjectDir)lib。

C:\Users\yckj\Documents\Visual Studio2015\Projects\watermarkUI\lib

点击应用，

![](https://img-blog.csdn.net/20160330145335231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#    最后点击确定

#  3，在要用到ffmpeg库的头文件中加入：




**[cpp]**[view plain](http://blog.csdn.net/hustlx/article/details/51014307#)[copy](http://blog.csdn.net/hustlx/article/details/51014307#)



- extern "C"  
- {  
- #include "include\libavcodec\avcodec.h"  
- #include "include\libavformat\avformat.h"  
- #include "include\libavutil\channel_layout.h"  
- #include "include\libavutil\common.h"  
- #include "include\libavutil\imgutils.h"  
- #include "include\libswscale\swscale.h"   
- #include "include\libavutil\imgutils.h"      
- #include "include\libavutil\opt.h"         
- #include "include\libavutil\mathematics.h"      
- #include "include\libavutil\samplefmt.h"   
- };  
- #pragma comment(lib, "avcodec.lib")  
- #pragma comment(lib, "avformat.lib")  
- #pragma comment(lib, "avdevice.lib")  
- #pragma comment(lib, "avfilter.lib")  
- #pragma comment(lib, "avutil.lib")  
- #pragma comment(lib, "postproc.lib")  
- #pragma comment(lib, "swresample.lib")  
- #pragma comment(lib, "swscale.lib")  


这样你的ffmpeg库就应该配置成功了。






