# MP4V2库与MP4AV库编译 - DoubleLi - 博客园






最近在开发一个将RTP流存储为MP4文件的功能。其中针对MP4文件读写，用到了两个开源的库。其中MP4V2用于数据的读写，MP4AV用于对其中的数据帧进行分析。




MP4V2和MP4AV都是开源项目MPEG4IP的一部分，当前MPEG4IP已经不再维护。

但是MP4V2最为其中的一部分，还有开源组织再进行更新，其项目主页：[http://code.google.com/p/mp4v2/](http://code.google.com/p/mp4v2/)，当前最新版本为mp4v2-2.0.0。



关于MP4文件的生成可以参考，MPEG4IP中MP4Creat的实现代码。



在这里将编译库过程中遇到的问题记录一下：

## MP4V2 Windows环境编译

MP4v2项目工程有windows下的工程，但是缺少几个文件，分别为platform_win32.cpp，platform_win32_impl.h，Version.rc

这几个文件可以到[http://code.google.com/p/mp4v2/](http://code.google.com/p/mp4v2/)的最新SVN库中下载到，将这几个文件更新到相应工程后，即可编译成功。



## MP4V2 Linux环境编译

MP4v2在[Linux](http://lib.csdn.net/base/linux)上编译没有遇到什么问题，直接按照工程中install文档中步骤操作即可。



## MP4AV Windows环境编译

由于MP4AV库时MPEG4IP的一部分，当前我只是在编了MPEG4IP的MP4AV的一部分，MPEG4IP在windows下的编译从网上查的资料来看，存在一些问题，但是有的网友已经给出了解决的方法，本人是按照资源：[http://download.csdn.net/detail/zhangcuizhi/1737512](http://download.csdn.net/detail/zhangcuizhi/1737512)中的文档进行操作的，MPEG4IP不能完整编译成功，但是MP4AV库可以成功编译。



## MP4AV Linux环境编译

参考MPEG4IP库中README.html，中的编译步骤执行即可。

1 执行./bootstrap ，将生成一系列的makefile

2 由于我只需要获取MP4AV库，只需要进入MP4AV工程，执行make即可

编译过程有有以下错误：

In file included from config_opts.cpp:1:
../../include/mpeg4ip.h:126: error: new declaration 'char* strcasestr(const char*, const char*)'
/usr/include/string.h:367: error: ambiguates old declaration 'const char* strcasestr(const char*, const char*)'



解决方法：

修改mpeg4ip.h从126行起，按照如下修改

修改前：
#ifdef __cplusplus
extern "C" {
#endif
char *strcasestr(const char *haystack, const char *needle);
#ifdef __cplusplus
}
#endif

修改后：
#ifdef __cplusplus
extern "C++" {
#endif
const char *strcasestr(const char *haystack, const char *needle);
#ifdef __cplusplus
}
#endif

重新编译，通过。










