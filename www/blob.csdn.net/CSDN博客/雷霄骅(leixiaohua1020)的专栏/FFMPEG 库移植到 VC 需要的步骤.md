# FFMPEG 库移植到 VC 需要的步骤 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月15日 12:27:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：77









在VC下使用FFMPEG编译好的库，不仅仅是把.h，.lib，.dll拷贝到工程中就行了，还需要做以下几步。（此方法适用于自己使用MinGW编译的库，也同样适用于从网上下载的编译好的库，例如[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)）。

（1）像其他额外库一样，设置VC的Include路径为你c:\msys\local\include,设置VClib路径为次c:\msys\local\bin，增加操作系统的一个Path c:\msys\local\bin（这一步好像不是必须的）。


 （2）将mingw安装目录下的include的inttypes.h，stdint.h，_mingw.h三个文件拷到你ffmpeg库的目录下的include

 (3)在_mingw.h文件的结尾处(在#endif 一行之前)添加了一行：
 #define __restrict__

 (4)把所有long long改成了__int64，如果是直接在vs2008下编译，则这个修改应该是不需要的(这步我没有遇到)

 （5）
 #ifdef __cplusplus

 #include "stdio.h"
 #include "stdlib.h"
 #include "string.h"
 #include "SDL/SDL.h"
 //#include "windows.h"

 extern "C"
 {
 #include "ffmpeg/avutil.h"
 #include "ffmpeg/avcodec.h"
 #include "ffmpeg/avformat.h"
 }

 #endif

 #pragma comment(lib,"avutil.lib")
 #pragma comment(lib,"avcodec.lib")
 #pragma comment(lib,"avformat.lib")

 （6）如果遇到error C3861: 'UINT64_C': identifier not found
 在common.h里加入定义如下：
 #ifndef INT64_C
 #define INT64_C(c) (c ## LL)
 #define UINT64_C(c) (c ## ULL)

#endif



原文地址：[http://blog.sina.com.cn/s/blog_6bcde18101011v2h.html](http://blog.sina.com.cn/s/blog_6bcde18101011v2h.html)




