# FFmpeg源代码简单分析：av_find_decoder()和av_find_encoder() - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年03月06日 12:40:06[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：20267
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


本文记录FFmpeg的两个API函数：avcodec_find_encoder()和avcodec_find_decoder()。avcodec_find_encoder()用于查找FFmpeg的编码器，avcodec_find_decoder()用于查找FFmpeg的解码器。
avcodec_find_encoder()的声明位于libavcodec\avcodec.h，如下所示。

```cpp
/**
 * Find a registered encoder with a matching codec ID.
 *
 * @param id AVCodecID of the requested encoder
 * @return An encoder if one was found, NULL otherwise.
 */
AVCodec *avcodec_find_encoder(enum AVCodecID id);
```
函数的参数是一个编码器的ID，返回查找到的编码器（没有找到就返回NULL）。
avcodec_find_decoder()的声明也位于libavcodec\avcodec.h，如下所示。

```cpp
/**
 * Find a registered decoder with a matching codec ID.
 *
 * @param id AVCodecID of the requested decoder
 * @return A decoder if one was found, NULL otherwise.
 */
AVCodec *avcodec_find_decoder(enum AVCodecID id);
```

函数的参数是一个解码器的ID，返回查找到的解码器（没有找到就返回NULL）。



avcodec_find_encoder()函数最典型的例子可以参考：

[最简单的基于FFMPEG的视频编码器（YUV编码为H.264）](http://blog.csdn.net/leixiaohua1020/article/details/25430425)


avcodec_find_decoder()函数最典型的例子可以参考：

[最简单的基于FFMPEG+SDL的视频播放器 ver2 （采用SDL2.0）](http://blog.csdn.net/leixiaohua1020/article/details/38868499)

其实这两个函数的实质就是遍历AVCodec链表并且获得符合条件的元素。有关AVCodec链表的建立可以参考文章：
[ffmpeg 源代码简单分析 ： av_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677129)




## 函数调用关系图
avcodec_find_encoder()和avcodec_find_decoder()的函数调用关系图如下所示。

![](https://img-blog.csdn.net/20150305163655358)



## avcodec_find_encoder()
avcodec_find_encoder()的源代码位于libavcodec\utils.c，如下所示。

```cpp
AVCodec *avcodec_find_encoder(enum AVCodecID id)
{
    return find_encdec(id, 1);
}
```

从源代码可以看出avcodec_find_encoder()调用了一个find_encdec()，注意它的第二个参数是0。

下面我们看一下find_encdec()的定义。




## find_encdec()
find_encdec()的源代码位于libavcodec\utils.c，如下所示。

```cpp
static AVCodec *first_avcodec;

static AVCodec *find_encdec(enum AVCodecID id, int encoder)
{
    AVCodec *p, *experimental = NULL;
    p = first_avcodec;
    id= remap_deprecated_codec_id(id);
    while (p) {
        if ((encoder ? av_codec_is_encoder(p) : av_codec_is_decoder(p)) &&
            p->id == id) {
            if (p->capabilities & CODEC_CAP_EXPERIMENTAL && !experimental) {
                experimental = p;
            } else
                return p;
        }
        p = p->next;
    }
    return experimental;
}
```

find_encdec()中有一个循环，该循环会遍历AVCodec结构的链表，逐一比较输入的ID和每一个编码器的ID，直到找到ID取值相等的编码器。
在这里有几点需要注意：
（1）first_avcodec是一个全局变量，存储AVCodec链表的第一个元素。
（2）remap_deprecated_codec_id()用于将一些过时的编码器ID映射到新的编码器ID。
（3）函数的第二个参数encoder用于确定查找编码器还是解码器。当该值为1的时候，用于查找编码器，此时会调用av_codec_is_encoder()判断AVCodec是否为编码器；当该值为0的时候，用于查找解码器，此时会调用av_codec_is_decoder()判断AVCodec是否为解码器。


### av_codec_is_encoder()
av_codec_is_encoder()是一个判断AVCodec是否为编码器的函数。如果是编码器，返回非0值，否则返回0。

```cpp
/**
 * @return a non-zero number if codec is an encoder, zero otherwise
 */
int av_codec_is_encoder(const AVCodec *codec);
```
av_codec_is_encoder()源代码很简单，如下所示。

```cpp
int av_codec_is_encoder(const AVCodec *codec)
{
    return codec && (codec->encode_sub || codec->encode2);
}
```

从源代码可以看出，av_codec_is_encoder()判断了一下AVCodec是否包含了encode2()或者encode_sub()接口函数。




### av_codec_is_decoder()
av_codec_is_decoder()是一个判断AVCodec是否为解码器的函数。如果是解码器，返回非0值，否则返回0。

```cpp
/**
 * @return a non-zero number if codec is a decoder, zero otherwise
 */
int av_codec_is_decoder(const AVCodec *codec);
```
av_codec_is_decoder()源代码也很简单，如下所示。

```cpp
int av_codec_is_decoder(const AVCodec *codec)
{
    return codec && codec->decode;
}
```
从源代码可以看出，av_codec_is_decoder()判断了一下AVCodec是否包含了decode()接口函数。


## avcodec_find_decoder()
avcodec_find_decoder()的源代码位于libavcodec\utils.c，如下所示。

```cpp
AVCodec *avcodec_find_decoder(enum AVCodecID id)
{
    return find_encdec(id, 0);
}
```
可以看出avcodec_find_decoder()同样调用了find_encdec()，只是第2个参数设置为0。因此不再详细分析。




**雷霄骅leixiaohua1020@126.comhttp://blog.csdn.net/leixiaohua1020**



