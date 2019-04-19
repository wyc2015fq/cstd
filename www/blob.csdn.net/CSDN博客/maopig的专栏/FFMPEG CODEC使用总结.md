# FFMPEG CODEC使用总结 - maopig的专栏 - CSDN博客
2011年09月07日 23:02:28[maopig](https://me.csdn.net/maopig)阅读数：6241
分类： [视频编解码技术](http://blog.csdn.net/mybmw320i/article/category/683738)2010-07-15 10:29
283人阅读[评论](http://blog.csdn.net/mybmw320i/article/details/5736493#comments)(0)[收藏]()[举报](http://blog.csdn.net/mybmw320i/article/details/5736493#report)
ffmpeg里提供了很多的encoder，decoder，详见avcodec.h里的枚举变量CodecID。
宏定义
#define REGISTER_ENCODER(X,x) { /
          extern AVCodec x##_encoder; /
          if(##X##_ENCODER)  avcodec_register(&x##_encoder); }
#define REGISTER_DECODER(X,x) { /
     extern AVCodec x##_decoder; /
           if(##CODEC_ID_##X##)  avcodec_register(&x##_decoder); }
比如：
 REGISTER_DECODER (H264, h264);
就相当于extern AVCodec h264_decoder;
            if(CODEC_ID_H264)
            avcodec_register(&h264_decoder);
而这三行代码的实际作用就是：
           AVCodec codec;
           codec = &h264_decoder;
配置好codec之后，
定义全局变量AVCodecContext *avctx;
然后avctx->codec = codec;
后续就可以调用通用的API：
                     avctx->codec->init         初始化
                     avctx->codec->decode  解码
                  或avctx->codec->encode  编码
                     avctx->codec->close     关闭
实现编码或解码功能
codec是一个结构体，定义如下：
typedef struct AVCodec {
    const char *name;
    enum CodecType type;
    enum CodecID id;
    int priv_data_size;
    int (*init)(AVCodecContext *);    //初始化函数
    int (*encode)(AVCodecContext *, uint8_t *buf, int buf_size, void *data);
//编码函数
    int (*close)(AVCodecContext *); //关闭函数
    int (*decode)(AVCodecContext *, void *outdata, int *outdata_size, AVPacket *avpkt);
//解码函数
对于不同的codec，这三个API对应的函数不同。
   对于CODEC_ID_H264，h264_decoder的定义为
AVCodec h264_decoder = {
    "h264",
    CODEC_TYPE_VIDEO,
    CODEC_ID_H264,
    sizeof(H264Context),
    decode_init,
    NULL,
    decode_end,
    decode_frame,
 }
对于CODEC_ID_H263，h263_decoder的定义如下:
 AVCodec h263_decoder = {
    "h263",
    CODEC_TYPE_VIDEO,
    CODEC_ID_H263,
    sizeof(MpegEncContext),
    ff_h263_decode_init,
    NULL,
    ff_h263_decode_end,
    ff_h263_decode_frame,
    CODEC_CAP_DRAW_HORIZ_BAND | CODEC_CAP_DR1 | CODEC_CAP_TRUNCATED | CODEC_CAP_DELAY,
  //  .flush= ff_mpeg_flush,
  //  .long_name= NULL_IF_CONFIG_SMALL("H.263 / H.263-1996, H.263+ / H.263-1998 / H.263 version 2"),
  //  .pix_fmts= ff_hwaccel_pixfmt_list_420,
};
对于CODEC_ID_MPEG4，mpeg4_decoder的定义如下:
AVCodec mpeg4_decoder = {
    "mpeg4",
    CODEC_TYPE_VIDEO,
    CODEC_ID_MPEG4,
    sizeof(MpegEncContext),
    ff_h263_decode_init,
    NULL,
    ff_h263_decode_end,
    ff_h263_decode_frame,
    CODEC_CAP_DRAW_HORIZ_BAND | CODEC_CAP_DR1 | CODEC_CAP_TRUNCATED | CODEC_CAP_DELAY,
//    .flush= ff_mpeg_flush,
//    .long_name= NULL_IF_CONFIG_SMALL("MPEG-4 part 2"),
//    .pix_fmts= ff_hwaccel_pixfmt_list_420,
};
