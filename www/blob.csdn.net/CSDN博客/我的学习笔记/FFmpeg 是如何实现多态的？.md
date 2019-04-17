# FFmpeg 是如何实现多态的？ - 我的学习笔记 - CSDN博客





2018年10月09日 22:32:14[zouzhiheng](https://me.csdn.net/u011330638)阅读数：102








# 前言

众所周知，FFmpeg 在解码的时候，无论输入文件是 MP4 文件还是 FLV 文件，或者其它文件格式，都能正确解封装、解码，而代码不需要针对不同的格式做出任何改变，这是面向对象中很常见的多态特性，但 FFmpeg 是用 C 语言编写的，那么它是如何使用 C 语言实现了多态特性的呢？

要解决这个问题，首先需要从函数 av_register_all 说起。

# av_register_all

av_register_all 是几乎所有 FFmpeg 程序中第一个被调用的函数，用于注册在编译 FFmpeg 时设置了 --enable 选项的封装器、解封装器、编码器、解码器等。源码如下：

```
#define REGISTER_MUXER(X, x)                                            \
    {                                                                   \
        extern AVOutputFormat ff_##x##_muxer;                           \
        if (CONFIG_##X##_MUXER)                                         \
            av_register_output_format(&ff_##x##_muxer);                 \
    }

#define REGISTER_DEMUXER(X, x)                                          \
    {                                                                   \
        extern AVInputFormat ff_##x##_demuxer;                          \
        if (CONFIG_##X##_DEMUXER)                                       \
            av_register_input_format(&ff_##x##_demuxer);                \
    }

#define REGISTER_MUXDEMUX(X, x) REGISTER_MUXER(X, x); REGISTER_DEMUXER(X, x)

static void register_all(void)
{
    // 注册编解码器
    avcodec_register_all();

    // 注册封装器、解封装器
    /* (de)muxers */
    REGISTER_MUXER   (A64,              a64);
    REGISTER_DEMUXER (AA,               aa);
    REGISTER_DEMUXER (AAC,              aac);
    REGISTER_MUXDEMUX(AC3,              ac3);
    REGISTER_MUXDEMUX(FLV,              flv);
    REGISTER_MUXDEMUX(GIF,              gif);
    ...

    /* image demuxers */
    REGISTER_DEMUXER (IMAGE_BMP_PIPE,        image_bmp_pipe);
    REGISTER_DEMUXER (IMAGE_JPEG_PIPE,       image_jpeg_pipe);
    REGISTER_DEMUXER (IMAGE_SVG_PIPE,        image_svg_pipe);
    REGISTER_DEMUXER (IMAGE_WEBP_PIPE,       image_webp_pipe);
    REGISTER_DEMUXER (IMAGE_PNG_PIPE,        image_png_pipe);
    ...

    /* external libraries */
    REGISTER_MUXER   (CHROMAPRINT,      chromaprint);
    ...
}

void av_register_all(void)
{
    static AVOnce control = AV_ONCE_INIT;

    ff_thread_once(&control, register_all);
}
```

define 里的 ## 用于拼接两个字符串，比如 REGISTER_DEMUXER(AAC, aac) ，它等效于：

```
extern AVInputFormat ff_aac_demuxer;
if(CONFIG_AAC_DEMUXER) av_register_input_format(&ff_aac_demuxer);
```

可以看出，编译 ffmpeg 时类似于 “–enable-muxer=xxx” 这样的选项在此时发挥了作用，它决定是否注册某个格式对应的(解)封装器，以便之后处理该格式的时候找到这个(解)封装器。

# av_register_input_format

av_register_input_format、av_register_output_format 源码如下：

```
/** head of registered input format linked list */
static AVInputFormat *first_iformat = NULL;
/** head of registered output format linked list */
static AVOutputFormat *first_oformat = NULL;

static AVInputFormat **last_iformat = &first_iformat;
static AVOutputFormat **last_oformat = &first_oformat;

void av_register_input_format(AVInputFormat *format)
{
    AVInputFormat **p = last_iformat;

    // Note, format could be added after the first 2 checks but that implies that *p is no longer NULL
    while(p != &format->next && !format->next && avpriv_atomic_ptr_cas((void * volatile *)p, NULL, format))
        p = &(*p)->next;

    if (!format->next)
        last_iformat = &format->next;
}

void av_register_output_format(AVOutputFormat *format)
{
    AVOutputFormat **p = last_oformat;

    // Note, format could be added after the first 2 checks but that implies that *p is no longer NULL
    while(p != &format->next && !format->next && avpriv_atomic_ptr_cas((void * volatile *)p, NULL, format))
        p = &(*p)->next;

    if (!format->next)
        last_oformat = &format->next;
}
```

从代码中可以看到，这两个注册方法会把指定的 AVInputFormat、AVOutputFormat 加到链表的尾部。

# AVInputFormat

接着看 AVInputFormat 的定义：

```
typedef struct AVInputFormat {
    /**
     * A comma separated list of short names for the format. New names
     * may be appended with a minor bump.
     */
    const char *name;
    
    /**
     * Descriptive name for the format, meant to be more human-readable
     * than name. You should use the NULL_IF_CONFIG_SMALL() macro
     * to define it.
     */
    const char *long_name;
    
    /**
     * Can use flags: AVFMT_NOFILE, AVFMT_NEEDNUMBER, AVFMT_SHOW_IDS,
     * AVFMT_GENERIC_INDEX, AVFMT_TS_DISCONT, AVFMT_NOBINSEARCH,
     * AVFMT_NOGENSEARCH, AVFMT_NO_BYTE_SEEK, AVFMT_SEEK_TO_PTS.
     */
    int flags;

    /**
     * If extensions are defined, then no probe is done. You should
     * usually not use extension format guessing because it is not
     * reliable enough
     */
    const char *extensions;
	...

    /**
     * Tell if a given file has a chance of being parsed as this format.
     * The buffer provided is guaranteed to be AVPROBE_PADDING_SIZE bytes
     * big so you do not have to check for that unless you need more.
     */
    int (*read_probe)(AVProbeData *);

    /**
     * Read the format header and initialize the AVFormatContext
     * structure. Return 0 if OK. 'avformat_new_stream' should be
     * called to create new streams.
     */
    int (*read_header)(struct AVFormatContext *);

    /**
     * Read one packet and put it in 'pkt'. pts and flags are also
     * set. 'avformat_new_stream' can be called only if the flag
     * AVFMTCTX_NOHEADER is used and only in the calling thread (not in a
     * background thread).
     * @return 0 on success, < 0 on error.
     *         When returning an error, pkt must not have been allocated
     *         or must be freed before returning
     */
    int (*read_packet)(struct AVFormatContext *, AVPacket *pkt);

    ...
} AVInputFormat;
```

可以看到，这个结构体除了 name 等变量外，还具备 read_probe、read_header 等函数指针。

以前面提到的 ff_aac_demuxer 为例，这里看一下它的实现：

```
AVInputFormat ff_aac_demuxer = {
    // 名称
    .name         = "aac",
    .long_name    = NULL_IF_CONFIG_SMALL("raw ADTS AAC (Advanced Audio Coding)"),
    // 把函数指针指向能够处理 aac 格式的函数实现
    .read_probe   = adts_aac_probe,
    .read_header  = adts_aac_read_header,
    .read_packet  = adts_aac_read_packet,
    .flags        = AVFMT_GENERIC_INDEX,
    .extensions   = "aac",
    .mime_type    = "audio/aac,audio/aacp,audio/x-aac",
    .raw_codec_id = AV_CODEC_ID_AAC,
};
```

# 总结

根据以上代码的分析，此时我们就能得出问题的答案了：

FFmpeg 之所以能够作为一个平台，无论是封装、解封装，还是编码、解码，在处理对应格式的文件/数据时，都能找到对应的库来实现，而不需要修改代码，主要就是通过结构体 + 函数指针实现的。具体实现方式是：首先设计一个结构体，然后创建该结构体的多个对象，每个对象都有着自己的成员属性及函数实现。这样就使得 FFmpeg 具备了类似于面向对象编程中的多态的效果。

PS：avcodec_register_all 也是一样的，有兴趣的可以看看 AVCodec 的声明以及 ff_libx264_encoder 等编解码器的实现。



