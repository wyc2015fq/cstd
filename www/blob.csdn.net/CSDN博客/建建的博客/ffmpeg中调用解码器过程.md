# ffmpeg中调用解码器过程 - 建建的博客 - CSDN博客
2017年06月06日 11:17:56[纪建](https://me.csdn.net/u013898698)阅读数：847
                
该文将以X264编码器为例
所有编码器和解码器都是在avcodec_register_all()函数中注册的。从中可以找到视频的H264解码器和X264编码
REGISTER_DECODER(H264,              h264);
REGISTER_ENCODER(LIBX264,           libx264);
#define REGISTER_DECODER(X, x)                                          \
     {                                                                   \
         extern AVCodec ff_##x##_decoder;                                \
         if (CONFIG_##X##_DECODER)                                       \
             avcodec_register(&ff_##x##_decoder);                        \
     }
#define REGISTER_ENCODER(X, x)                                          \
     {                                                                   \
         extern AVCodec ff_##x##_encoder;                                \
         if (CONFIG_##X##_ENCODER)                                       \
             avcodec_register(&ff_##x##_encoder);                        \
     }
注册的过程发生在avcodec_register(AVCodec *codec)函数中，实际上就是向全局链表last_avcodec中加入libx264_encoder、h264_decoder特定的编解码器，输入参数AVCodec是一个结构体，可以理解为编解码器的基类，其中不仅包含了名称，id等属性，而且包含了如下函数指针，让每个具体的编解码器扩展类实现。
 int (*init)(AVCodecContext *);
    int (*encode_sub)(AVCodecContext *, uint8_t *buf, int buf_size,
                       const struct AVSubtitle *sub);
     /**
      * Encode data to an AVPacket.
      *
      * @param      avctx          codec context
      * @param      avpkt          output AVPacket (may contain a user-provided buffer)
      * @param[in]  frame          AVFrame containing the raw data to be encoded
      * @param[out] got_packet_ptr encoder sets to 0 or 1 to indicate that a
      *                            non-empty packet was returned in avpkt.
      * @return 0 on success, negative error code on failure
      */
     int (*encode2)(AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame, int *got_packet_ptr);
     int (*decode)(AVCodecContext *, void *outdata, int *outdata_size, AVPacket *avpkt);
     int (*close)(AVCodecContext *);
     /**
      * Flush buffers.
      * Will be called when seeking
      */
     void (*flush)(AVCodecContext *);
AVCodec ff_libx264_encoder = {
     .name             = "libx264",
     .long_name        = NULL_IF_CONFIG_SMALL("libx264 H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
     .type             = AVMEDIA_TYPE_VIDEO,
     .id               = AV_CODEC_ID_H264,
     .priv_data_size   = sizeof(X264Context),
     .init             = X264_init,
     .encode2          = X264_frame,
     .close            = X264_close,
     .capabilities     = CODEC_CAP_DELAY | CODEC_CAP_AUTO_THREADS,
     .priv_class       = &x264_class,
     .defaults         = x264_defaults,
     .init_static_data = X264_init_static,
 };
这里具体对来自AVCodec的属性和方法进行赋值。其中
  .init = X264_init,
     .encode2 = X264_frame,
     .close = X264_close,
将函数指针指向了具体函数，这三个函数将使用libx264静态库中提供的API，也就是X264的主要接口函数进行具体实现。
上面看到的X264Context封装了X264所需要的上下文管理数据，
复制代码
typedef struct X264Context {
     AVClass        *class;
     x264_param_t    params;
     x264_t         *enc;
     x264_picture_t  pic;
     uint8_t        *sei;
     int             sei_size;
     char *preset;
     char *tune;
     char *profile;
     char *level;
     int fastfirstpass;
     char *wpredp;
     char *x264opts;
     float crf;
     float crf_max;
     int cqp;
     int aq_mode;
     float aq_strength;
     char *psy_rd;
     int psy;
     int rc_lookahead;
     int weightp;
     int weightb;
     int ssim;
     int intra_refresh;
     int bluray_compat;
     int b_bias;
     int b_pyramid;
     int mixed_refs;
     int dct8x8;
     int fast_pskip;
     int aud;
     int mbtree;
     char *deblock;
     float cplxblur;
     char *partitions;
     int direct_pred;
     int slice_max_size;
     char *stats;
     int nal_hrd;
     char *x264_params;
 } X264Context;
它属于结构体AVCodecContext的void *priv_data变量，定义了每种编解码器私有的上下文属性，AVCodecContext也类似上下文基类一样。可以用类图来表示大概的编解码器组合。
编解码器打开操作是在transcode_init() -> init_input_stream() -> avcodec_open2()完成的，对具体的编解码器进行初始化。例如X264_init()
具体的编码操作是在transcode() -> transcode_step() -> reap_filters() -> do_video_out() 或 do_audio_out() -> avcodec_encode_video2() 或 avcodec_encode_audio2()。然后通过函数指针调用特定的编解码器。例如X264_frame()。
最后通过avcodec_close()关闭编解码器。例如X264_close()
