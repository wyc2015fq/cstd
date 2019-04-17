# FFMPEG学习【libavcodec】：解码：帧解析 - 一世豁然的专栏 - CSDN博客





2017年05月02日 16:19:54[一世豁然](https://me.csdn.net/Explorer_day)阅读数：574








一、数据结构



```cpp
struct  	AVCodecParserContext
struct  	AVCodecParser
```







二、枚举



```cpp
enum  	AVPictureStructure {
```

```cpp
AV_PICTURE_STRUCTURE_UNKNOWN,
```

```cpp
AV_PICTURE_STRUCTURE_TOP_FIELD,
```

```cpp
AV_PICTURE_STRUCTURE_BOTTOM_FIELD,
```

```cpp
AV_PICTURE_STRUCTURE_FRAME
```

```cpp
}
```



三、函数

1、AVCodecParser * av_parser_next（const AVCodecParser * c）




2、void av_register_codec_parser（AVCodecParser * parser）




3、AVCodecParserContext * av_parser_init (int codec_id)




4、int av_parser_parse2 (AVCodecParserContext *s, AVCodecContext *avctx, uint8_t **poutbuf, int *poutbuf_size, const uint8_t *buf, int buf_size, int64_t pts, int64_t dts, int64_t pos)

注释：解析一个数据包。

参数：s-解析上下文

    avctx-编解码上下文

    poutbuf-设置为指向解析缓冲区的指针，如果尚未完成则设置为NULL。

    poutbuf_size-设置为解析缓冲区的大小，如果尚未完成则设置为零。

    buf-输入缓冲

    buf_size-无填充的缓冲区大小（以字节为单位）。 即 假定完整缓冲区大小为buf_size + AV_INPUT_BUFFER_PADDING_SIZE。 要发送EOF信号，应该为设置为0（这样可以输出最后一帧）。

    pts-输入显示时间戳。

    dts-输入解码时间戳

    pos-在流中输入字节位置。

返回：使用的输入比特流的字节数。




5、int av_parser_change (AVCodecParserContext *s, AVCodecContext *avctx, uint8_t **poutbuf, int *poutbuf_size, const uint8_t *buf, int buf_size, int keyframe)

返回：如果输出缓冲区是输入的子集，则返回0，返回1，如果输出缓冲区被分配，且必须被释放。

废弃：使用AVBitStreamFilter




6、void av_parser_close (AVCodecParserContext *s)





