# vc2005编译ffmpeg以及ffplay - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月25日 21:35:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：33
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)










**ffmpeg编译过程：**
 1 [http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)
 下载官方提供的源码，win32库和dll。
 2 新建vc2005 console空工程，把ffmpeg.h,ffmpeg.c,cmdutils.c,cmdutils.h,cmdutils_common_opts.h,
 ffmpeg_filter.c,ffmpeg_opt.c加到工程。
 3 http://drv.nu/ffmpeg/doxygen/config_8h-source.html
 从这里生成 config.h
 4 从mingw下复制stdint.h和inttypes.h

 5 把.h里的inline换成__inline
 或

```cpp
#if defined(WIN32) && !defined(__cplusplus)
#define inline __inline
#endif
```

或
 project->[setting]->[c/c++]->Preprocessor definitions:编辑框里输入inline=__inline

 6 找不到colorspace.h等一些文件，从源码里找到复制过来。

7 有几个系统文件找不到，注释掉config.h相关定义，如



```cpp
//#define HAVE_SYS_RESOURCE_H 1
```



另外，需要用到socket，修改：

```cpp
#define HAVE_WINSOCK2_H 1
 #define HAVE_STRUCT_ADDRINFO 1
```

7 print_all_libs_info 里找不到什么东西，就注释掉。


 8参考 http://www.cnblogs.com/zyl910/archive/2012/08/08/c99int.html
 在config.h里面加上
 #define PRIu64 "I64u"
 #define PRId64 "I64d"

 9
 //#define AV_TIME_BASE_Q (AVRational){1, AV_TIME_BASE}

改成

#define AV_TIME_BASE_Q _AVRational(1, AV_TIME_BASE)





再加：



```cpp
AVRational _AVRational(int num, int den) {
AVRational r = {num, den};
return r;
}
```



或者，在用到AV_TIME_BASE_Q 的地方，手写AVRational r = {1,AV_TIME_BASE };，再用上这个r.

 最麻烦就是const OptionDef options[] 这个数组的初值处理，主要是里面的函数，参考ffmpeg_vc5的代码，把每一行改成这样：{ "f", HAS_ARG | OPT_STRING | OPT_OFFSET, { (void*)OFFSET(format) },
 就是把原来函数的地方{.fun = xxx}改成 {（void*）xxx}。编译通过了，常规的一些输入参数都好用。复杂的还没试。

 最后会提示几个链接问题，手动加上const AVPixFmtDescriptor av_pix_fmt_descriptors[PIX_FMT_NB];
 然后，在config.h里加：

```cpp
#define AVCONV_DATADIR "c:/ffmpeg1010/"


#define snprintf _snprintf
#define lrint(f) (f>=0?(int32_t)(f+(double)0.5):(int32_t)(f-(double)0.4))
#define lrintf(f) (f>=0?(int32_t)(f+(float)0.5):(int32_t)(f-(float)0.4))
#define llrintf(f) (f>=0?(int64_t)(f+(double)0.5):(int64_t)(f-(double)0.4))
#define llrint(f) (f>=0?(int64_t)(f+(float)0.5):(int64_t)(f-(float)0.4))


#define log2(x) (log(x) * 1.44269504088896340736)
```

这个AVCONV_DATADIR 是随便写的，暂时还没用上。

 到此，基本就完全成功了。编译出来，运行ffmpeg -i rtsp://xxx/test.264 out.mp4
 保存的质量非常好。



**ffplay编译过程**

ffplay编译比ffmpeg简单的多，新建工程，加上代码，稍改几行就OK了。然后ffplay rtsp，出了一个黑窗口，播放效果不错。再新生成一个mfc工程，给sdl指定一个窗口，视频就在指定窗口播放了。不过这时有个问题，拖动窗口时，视频就花掉了。估计sdl需要一些参数，以后有空再研究了。



源码下载：[http://download.csdn.net/detail/sxcong/4672795](http://download.csdn.net/detail/sxcong/4672795)



原文地址：[http://blog.csdn.net/sxcong/article/details/8097594](http://blog.csdn.net/sxcong/article/details/8097594)




