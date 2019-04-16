# FFmpeg源代码简单分析：libavdevice的avdevice_register_all() - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年03月24日 21:49:57[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：11931
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)









=====================================================


FFmpeg的库函数源代码分析文章列表：



【架构图】

[FFmpeg源代码结构图 - 解码](http://blog.csdn.net/leixiaohua1020/article/details/44220151)

[FFmpeg源代码结构图 - 编码](http://blog.csdn.net/leixiaohua1020/article/details/44226355)



【通用】

[FFmpeg 源代码简单分析：av_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677129)


[FFmpeg 源代码简单分析：avcodec_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677265)


[FFmpeg 源代码简单分析：内存的分配和释放（av_malloc()、av_free()等）](http://blog.csdn.net/leixiaohua1020/article/details/41176777)


[FFmpeg 源代码简单分析：常见结构体的初始化和销毁（AVFormatContext，AVFrame等）](http://blog.csdn.net/leixiaohua1020/article/details/41181155)


[FFmpeg 源代码简单分析：avio_open2()](http://blog.csdn.net/leixiaohua1020/article/details/41199947)

[FFmpeg 源代码简单分析：av_find_decoder()和av_find_encoder()](http://blog.csdn.net/leixiaohua1020/article/details/44084557)


[FFmpeg 源代码简单分析：avcodec_open2()](http://blog.csdn.net/leixiaohua1020/article/details/44117891)

[FFmpeg 源代码简单分析：avcodec_close()](http://blog.csdn.net/leixiaohua1020/article/details/44206699)

【解码】
[图解FFMPEG打开媒体的函数avformat_open_input](http://blog.csdn.net/leixiaohua1020/article/details/8661601)


[FFmpeg 源代码简单分析：avformat_open_input()](http://blog.csdn.net/leixiaohua1020/article/details/44064715)


[FFmpeg 源代码简单分析：avformat_find_stream_info()](http://blog.csdn.net/leixiaohua1020/article/details/44084321)

[FFmpeg 源代码简单分析：av_read_frame()](http://blog.csdn.net/leixiaohua1020/article/details/12678577)


[FFmpeg 源代码简单分析：avcodec_decode_video2()](http://blog.csdn.net/leixiaohua1020/article/details/12679719)

[FFmpeg 源代码简单分析：avformat_close_input()](http://blog.csdn.net/leixiaohua1020/article/details/44110683)

【编码】

[FFmpeg 源代码简单分析：avformat_alloc_output_context2()](http://blog.csdn.net/leixiaohua1020/article/details/41198929)

[FFmpeg 源代码简单分析：avformat_write_header()](http://blog.csdn.net/leixiaohua1020/article/details/44116215)


[FFmpeg 源代码简单分析：avcodec_encode_video()](http://blog.csdn.net/leixiaohua1020/article/details/44206485)


[FFmpeg 源代码简单分析：av_write_frame()](http://blog.csdn.net/leixiaohua1020/article/details/44199673)


[FFmpeg 源代码简单分析：av_write_trailer()](http://blog.csdn.net/leixiaohua1020/article/details/44201645)

【其它】


[FFmpeg源代码简单分析：日志输出系统（av_log()等）](http://blog.csdn.net/leixiaohua1020/article/details/44243155)


[FFmpeg源代码简单分析：结构体成员管理系统-AVClass](http://blog.csdn.net/leixiaohua1020/article/details/44268323)


[FFmpeg源代码简单分析：结构体成员管理系统-AVOption](http://blog.csdn.net/leixiaohua1020/article/details/44279329)


[FFmpeg源代码简单分析：libswscale的sws_getContext()](http://blog.csdn.net/leixiaohua1020/article/details/44305697)


[FFmpeg源代码简单分析：libswscale的sws_scale()](http://blog.csdn.net/leixiaohua1020/article/details/44346687)


[FFmpeg源代码简单分析：libavdevice的avdevice_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/41211121)


[FFmpeg源代码简单分析：libavdevice的gdigrab](http://blog.csdn.net/leixiaohua1020/article/details/44597955)


【脚本】

[FFmpeg源代码简单分析：makefile](http://blog.csdn.net/leixiaohua1020/article/details/44556525)


[FFmpeg源代码简单分析：configure](http://blog.csdn.net/leixiaohua1020/article/details/44587465)


【H.264】

[FFmpeg的H.264解码器源代码简单分析：概述](http://blog.csdn.net/leixiaohua1020/article/details/44864509)


=====================================================


本文简单记录一下FFmpeg中libavdevice注册设备的函数avdevice_register_all()。avdevice_register_all()在编程中的使用示例可以参考文章：

《[最简单的基于FFmpeg的AVDevice例子（读取摄像头）](http://blog.csdn.net/leixiaohua1020/article/details/39702113)》


在使用libavdevice之前，必须先运行avdevice_register_all()对设备进行注册，否则就会出错。avdevice_register_all()的注册方式和av_register_all()、avcodec_register_all()这几个函数是类似的。可以参考文章：

《[FFmpeg 源代码简单分析：av_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677129)》


avdevice_register_all()代码如下。

```cpp
/*
 * 雷霄骅
 * http://blog.csdn.net/leixiaohua1020
 */

#include "config.h"
#include "avdevice.h"
//输出设备
#define REGISTER_OUTDEV(X, x)                                           \
    {                                                                   \
        extern AVOutputFormat ff_##x##_muxer;                           \
        if (CONFIG_##X##_OUTDEV)                                        \
            av_register_output_format(&ff_##x##_muxer);                 \
    }
//输入设备
#define REGISTER_INDEV(X, x)                                            \
    {                                                                   \
        extern AVInputFormat ff_##x##_demuxer;                          \
        if (CONFIG_##X##_INDEV)                                         \
            av_register_input_format(&ff_##x##_demuxer);                \
    }
//输入输出设备
#define REGISTER_INOUTDEV(X, x) REGISTER_OUTDEV(X, x); REGISTER_INDEV(X, x)

void avdevice_register_all(void)
{
    static int initialized;

    if (initialized)
        return;
    initialized = 1;

    /* devices */
    REGISTER_INOUTDEV(ALSA,             alsa);
    REGISTER_INDEV   (AVFOUNDATION,     avfoundation);
    REGISTER_INDEV   (BKTR,             bktr);
    REGISTER_OUTDEV  (CACA,             caca);
    REGISTER_OUTDEV  (DECKLINK,         decklink);
    REGISTER_INDEV   (DSHOW,            dshow);
    REGISTER_INDEV   (DV1394,           dv1394);
    REGISTER_INOUTDEV(FBDEV,            fbdev);
    REGISTER_INDEV   (GDIGRAB,          gdigrab);
    REGISTER_INDEV   (IEC61883,         iec61883);
    REGISTER_INDEV   (JACK,             jack);
    REGISTER_INDEV   (LAVFI,            lavfi);
    REGISTER_INDEV   (OPENAL,           openal);
    REGISTER_OUTDEV  (OPENGL,           opengl);
    REGISTER_INOUTDEV(OSS,              oss);
    REGISTER_INOUTDEV(PULSE,            pulse);
    REGISTER_INDEV   (QTKIT,            qtkit);
    REGISTER_OUTDEV  (SDL,              sdl);
    REGISTER_INOUTDEV(SNDIO,            sndio);
    REGISTER_INOUTDEV(V4L2,             v4l2);
//    REGISTER_INDEV   (V4L,              v4l
    REGISTER_INDEV   (VFWCAP,           vfwcap);
    REGISTER_INDEV   (X11GRAB,          x11grab);
    REGISTER_OUTDEV  (XV,               xv);

    /* external libraries */
    REGISTER_INDEV   (LIBCDIO,          libcdio);
    REGISTER_INDEV   (LIBDC1394,        libdc1394);
}
```


从代码中可以看出，avdevice_register_all()调用3个函数进行设备组建的注册：REGISTER_INDEV()，REGISTER_OUTDEV()，REGISTER_INOUTDEV()。上述3个函数实际上是预定义的3个宏：
REGISTER_INDEV()：注册输入设备。实际上调用了av_register_input_format()将输入设备注册成一个AVInputFormat。
REGISTER_OUTDEV()：注册输出设备。实际上调用了av_register_output_format()将输出设备注册成一个AVOutputFormat。
REGISTER_INOUTDEV()：注册输入设备和输出设备。实际上将上述两个宏定义合并了。




**雷霄骅leixiaohua1020@126.comhttp://blog.csdn.net/leixiaohua1020**




