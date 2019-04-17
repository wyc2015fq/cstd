# FFmpeg 源码分析 - avcodec_send_packet 和 avcodec_receive_frame - 我的学习笔记 - CSDN博客





2018年10月09日 22:57:45[zouzhiheng](https://me.csdn.net/u011330638)阅读数：1123








# 使用

从 FFmpeg 3.x 开始，avcodec_decode_video2 就被废弃了，取而代之的是 avcodec_send_packet 和 avcodec_receive_frame。使用方法很简单，可以查看 ffmpeg 源码文件夹 ffmpeg-< version>/doc/example/decode_video.c 下的代码，这里摘抄关键部分如下：

```
static void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt,
                   const char *filename)
{
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", dec_ctx->frame_number);
    }
}
```

# avcodec_decode_video2

在开始分析 avcodec_send_packet 和 avcodec_receive_frame 之前，先看一下 avcodec_decode_video2 的源码实现：

```
int attribute_align_arg avcodec_decode_video2(AVCodecContext *avctx, AVFrame *picture,
                                              int *got_picture_ptr,
                                              const AVPacket *avpkt)
{
    return compat_decode(avctx, picture, got_picture_ptr, avpkt);
}
```

```
static int compat_decode(AVCodecContext *avctx, AVFrame *frame,
                         int *got_frame, const AVPacket *pkt)
{
    AVCodecInternal *avci = avctx->internal;
    int ret = 0;

    av_assert0(avci->compat_decode_consumed == 0);

    if (avci->draining_done && pkt && pkt->size != 0) {
        av_log(avctx, AV_LOG_WARNING, "Got unexpected packet after EOF\n");
        avcodec_flush_buffers(avctx);
    }

    *got_frame = 0;
    avci->compat_decode = 1;

    ... // 容错处理

    if (!avci->compat_decode_partial_size) {
        ret = avcodec_send_packet(avctx, pkt);
        ...
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(avctx, frame);
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                ret = 0;
            goto finish;
        }

        if (frame != avci->compat_decode_frame) {
            if (!avctx->refcounted_frames) {
                ret = unrefcount_frame(avci, frame);
                if (ret < 0)
                    goto finish;
            }

            *got_frame = 1;
            frame = avci->compat_decode_frame;
        } else {
            ...
        }

        if (avci->draining || (!avctx->codec->bsfs && avci->compat_decode_consumed < pkt->size))
            break;
    }

finish:
    if (ret == 0) {
        /* if there are any bsfs then assume full packet is always consumed */
        if (avctx->codec->bsfs)
            ret = pkt->size;
        else
            ret = FFMIN(avci->compat_decode_consumed, pkt->size);
    }
    avci->compat_decode_consumed = 0;
    avci->compat_decode_partial_size = (ret >= 0) ? pkt->size - ret : 0;

    return ret;
}
```

可以看到，avcodec_decode_video2 这个过时的函数，在新版本中最终还是通过 avcodec_send_packet() 和 avcodec_receive_frame() 完成的。

# avcodec_send_packet

下面看 avcodec_send_packet，关键的地方写了注释

```
int attribute_align_arg avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt)
{
    AVCodecInternal *avci = avctx->internal;
    int ret;

    // 检查 AVCodecContext 是否已打开，并且 AVCodec 是否为解码器
    if (!avcodec_is_open(avctx) || !av_codec_is_decoder(avctx->codec))
        return AVERROR(EINVAL);

    if (avctx->internal->draining)
        return AVERROR_EOF;

    if (avpkt && !avpkt->size && avpkt->data)
        return AVERROR(EINVAL);

    // 初始化 avci 的 DecodeFilterContext 等成员
    ret = bsfs_init(avctx);
    if (ret < 0)
        return ret;

    av_packet_unref(avci->buffer_pkt);
    if (avpkt && (avpkt->data || avpkt->side_data_elems)) {
        ret = av_packet_ref(avci->buffer_pkt, avpkt);
        if (ret < 0)
            return ret;
    }

    // 把 AVPacket 的数据传给 avci->filter.bsfs[0]
    ret = av_bsf_send_packet(avci->filter.bsfs[0], avci->buffer_pkt);
    if (ret < 0) {
        av_packet_unref(avci->buffer_pkt);
        return ret;
    }

    if (!avci->buffer_frame->buf[0]) {
        // 解码
        ret = decode_receive_frame_internal(avctx, avci->buffer_frame);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
            return ret;
    }

    return 0;
}
```

可以看到，avcodec_send_packet 的关键是 decode_receive_frame_internal 这个函数。DecodeFilterContext、av_bsf_send_packet 等结构体或函数主要用于存储 AVPacket。

## av_bsf_send_packet

av_bsf_send_packet 的逻辑很简单，定义如下：

```
int av_bsf_send_packet(AVBSFContext *ctx, AVPacket *pkt)
{
    if (!pkt || (!pkt->data && !pkt->side_data_elems)) {
        ctx->internal->eof = 1;
        return 0;
    }

    if (ctx->internal->eof) {
        av_log(ctx, AV_LOG_ERROR, "A non-NULL packet sent after an EOF.\n");
        return AVERROR(EINVAL);
    }

    if (ctx->internal->buffer_pkt->data ||
        ctx->internal->buffer_pkt->side_data_elems)
        return AVERROR(EAGAIN);

    av_packet_move_ref(ctx->internal->buffer_pkt, pkt);

    return 0;
}
```

```
void av_packet_move_ref(AVPacket *dst, AVPacket *src)
{
    *dst = *src;
    av_init_packet(src);
    src->data = NULL;
    src->size = 0;
}
```

可以看到，av_bsf_send_packet 的作用是把传过来的 AVPacket 放到指定的 AVBSFContext 内部，并将该 AVPacket 置为空。

## decode_receive_frame_internal

decode_receive_frame_internal 是解码的关键函数，定义如下：

```
static int decode_receive_frame_internal(AVCodecContext *avctx, AVFrame *frame)
{
    AVCodecInternal *avci = avctx->internal;
    int ret;

    av_assert0(!frame->buf[0]);

	// 解码的关键函数
    if (avctx->codec->receive_frame)
        ret = avctx->codec->receive_frame(avctx, frame);
    else
        ret = decode_simple_receive_frame(avctx, frame);

    if (ret == AVERROR_EOF)
        avci->draining_done = 1;

    return ret;
}
```

可以看到，decode_receive_frame_internal 首先会判断 AVCodec 是否存在 receive_frame 这个函数指针，如果存在，就使用该指针对应的函数实现进行解码，否则调用 decode_simple_receive_frame 解码。

以 H264 格式为例，对应的解码器为 ff_h264_decoder：

```
AVCodec ff_h264_decoder = {
    .name                  = "h264",
    .long_name             = NULL_IF_CONFIG_SMALL("H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
    .type                  = AVMEDIA_TYPE_VIDEO,
    .id                    = AV_CODEC_ID_H264,
    .priv_data_size        = sizeof(H264Context),
    .init                  = h264_decode_init,
    .close                 = h264_decode_end,
    .decode                = h264_decode_frame,
    .capabilities          = /*AV_CODEC_CAP_DRAW_HORIZ_BAND |*/ AV_CODEC_CAP_DR1 |
                             AV_CODEC_CAP_DELAY | AV_CODEC_CAP_SLICE_THREADS |
                             AV_CODEC_CAP_FRAME_THREADS,
    .caps_internal         = FF_CODEC_CAP_INIT_THREADSAFE | FF_CODEC_CAP_EXPORTS_CROPPING,
    .flush                 = flush_dpb,
    .init_thread_copy      = ONLY_IF_THREADS_ENABLED(decode_init_thread_copy),
    .update_thread_context = ONLY_IF_THREADS_ENABLED(ff_h264_update_thread_context),
    .profiles              = NULL_IF_CONFIG_SMALL(ff_h264_profiles),
    .priv_class            = &h264_class,
};
```

没有发现函数指针 receive_frame，因此这里直接看 decode_simple_receive_frame：

```
static int decode_simple_receive_frame(AVCodecContext *avctx, AVFrame *frame)
{
    int ret;

    // 某些解码器可能会消耗部分数据包而不返回任何输出，因此需要在循环中调用此函数
    // 直到它返回EAGAIN
    while (!frame->buf[0]) {
        ret = decode_simple_internal(avctx, frame);
        if (ret < 0)
            return ret;
    }

    return 0;
}
```

可以看到，它直接调用 decode_simple_internal 这个函数：

```
/*
* The core of the receive_frame_wrapper for the decoders implementing
* the simple API.
* 某些解码器可能会消耗部分数据包而不返回任何输出，因此需要在循环中调用此函数，直到它返回EAGAIN
**/
static int decode_simple_internal(AVCodecContext *avctx, AVFrame *frame)
{
    AVCodecInternal   *avci = avctx->internal;
    DecodeSimpleContext *ds = &avci->ds;
    AVPacket           *pkt = ds->in_pkt;
    // copy to ensure we do not change pkt
    AVPacket tmp;
    int got_frame, actual_got_frame, did_split;
    int ret;

    if (!pkt->data && !avci->draining) {
        av_packet_unref(pkt);
        // 获取在执行 av_bsf_send_packet 时缓存的  AVPacket
        ret = ff_decode_get_packet(avctx, pkt);
        if (ret < 0 && ret != AVERROR_EOF)
            return ret;
    }

    ...

    got_frame = 0;

    if (HAVE_THREADS && avctx->active_thread_type & FF_THREAD_FRAME) {
        // 获取异步解码缓存的 AVFrame
        ret = ff_thread_decode_frame(avctx, frame, &got_frame, &tmp);
    } else {
        // 解码的关键函数
        ret = avctx->codec->decode(avctx, frame, &got_frame, &tmp);

        ...
    }
    
    ... // 设置 AVFrame 的成员变量，如 best_effort_timestamp、format、channel 等

    avci->compat_decode_consumed += ret;

    // 如果这个 AVPacket 已被消耗完，则释放内存，否则调整指针、成员变量
    if (ret >= pkt->size || ret < 0) {
        av_packet_unref(pkt);
    } else {
        int consumed = ret;

        pkt->data                += consumed;
        pkt->size                -= consumed;
        avci->last_pkt_props->size -= consumed; // See extract_packet_props() comment.
        pkt->pts                  = AV_NOPTS_VALUE;
        pkt->dts                  = AV_NOPTS_VALUE;
        avci->last_pkt_props->pts = AV_NOPTS_VALUE;
        avci->last_pkt_props->dts = AV_NOPTS_VALUE;
    }

    if (got_frame)
        av_assert0(frame->buf[0]);

    return ret < 0 ? ret : 0;
}
```

这个函数其实很长，但最关键的只有 avctx->codec->decode 这一句代码，decode 是 结构体 AVCodec 的函数指针，不同的编码格式对应不同的函数实现，以 H264 为例，对应的解码器为 ff_h264_decoder (上面把它的声明贴出来了)，函数实现是 h264_decode_frame。

## h264_decode_frame

这里简单看一下 h264_decode_frame：

```
static int h264_decode_frame(AVCodecContext *avctx, void *data,
                             int *got_frame, AVPacket *avpkt)
{
    const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    H264Context *h     = avctx->priv_data;
    AVFrame *pict      = data;
    int buf_index;
    int ret;

    h->flags = avctx->flags;
    h->setup_finished = 0;
    h->nb_slice_ctx_queued = 0;

    ff_h264_unref_picture(h, &h->last_pic_for_ec);

    /* end of stream, output what is still in the buffers */
    // 直接返回依然在缓存中的数据
    if (buf_size == 0)
        return send_next_delayed_frame(h, pict, got_frame, 0);

    if (h->is_avc && av_packet_get_side_data(avpkt, AV_PKT_DATA_NEW_EXTRADATA, NULL)) {
        int side_size;
        uint8_t *side = av_packet_get_side_data(avpkt, AV_PKT_DATA_NEW_EXTRADATA, &side_size);
        // 解码 SPS、PPS
        if (is_extra(side, side_size))
            ff_h264_decode_extradata(side, side_size,
                                     &h->ps, &h->is_avc, &h->nal_length_size,
                                     avctx->err_recognition, avctx);
    }
    if(h->is_avc && buf_size >= 9 && buf[0]==1 && buf[2]==0 && (buf[4]&0xFC)==0xFC && (buf[5]&0x1F) && buf[8]==0x67){
        if (is_extra(buf, buf_size))
            return ff_h264_decode_extradata(buf, buf_size,
                                            &h->ps, &h->is_avc, &h->nal_length_size,
                                            avctx->err_recognition, avctx);
    }

	// 解码 NAL Unit
    buf_index = decode_nal_units(h, buf, buf_size);
    if (buf_index < 0)
        return AVERROR_INVALIDDATA;

    ... 

    av_assert0(pict->buf[0] || !*got_frame);

    ff_h264_unref_picture(h, &h->last_pic_for_ec);

    return get_consumed_bytes(buf_index, buf_size);
}
```

可以看到，这个函数大致可以分为 2 个步骤：

1) 判断当前是否已到达文件的末尾，如果是，则返回解码后依然存在于缓存中的数据

2) 否则根据 NAL 的类型进行解码，如果是 SPS、PPS 数据，则调用 ff_h264_decode_extradata，否则调用 decode_nal_units
其中函数 is_extra 用于判断数据类型是否为 SPS、PPS：

```
static int is_extra(const uint8_t *buf, int buf_size)
{
    int cnt= buf[5]&0x1f;
    const uint8_t *p= buf+6;
    while(cnt--){
        int nalsize= AV_RB16(p) + 2;
        if(nalsize > buf_size - (p-buf) || (p[2] & 0x9F) != 7)
            return 0;
        p += nalsize;
    }
    cnt = *(p++);
    if(!cnt)
        return 0;
    while(cnt--){
        int nalsize= AV_RB16(p) + 2;
        if(nalsize > buf_size - (p-buf) || (p[2] & 0x9F) != 8)
            return 0;
        p += nalsize;
    }
    return 1;
}
```

从代码中可以看出，这个函数判在进行断的时候，有两个很关键的数字为 7 和 8，而这两个数字在文章 [音视频开发基础概述 - PCM、YUV、H264、常用软件介绍](https://blog.csdn.net/u011330638/article/details/81107312) 中介绍 H264 格式的部分说过，它们分别表示该 NAL Unit 的类型为 SPS、PPS。

ff_h264_decode_extradata 和 decode_nal_units 这两个函数继续分析下去可以发现很多 H264 相关的知识，这里就不继续了，有兴趣的可以自行研究。

# avcodec_receive_frame

```
int attribute_align_arg avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame)
{
    AVCodecInternal *avci = avctx->internal;
    int ret;

    av_frame_unref(frame);

    if (!avcodec_is_open(avctx) || !av_codec_is_decoder(avctx->codec))
        return AVERROR(EINVAL);

    ret = bsfs_init(avctx);
    if (ret < 0)
        return ret;

    // 如果存在缓存，则直接获取
    // 这个条件判断在函数 avcodec_send_packet 里也有
    if (avci->buffer_frame->buf[0]) {
        av_frame_move_ref(frame, avci->buffer_frame);
    } else {
        // 否则进行解码
        ret = decode_receive_frame_internal(avctx, frame);
        if (ret < 0)
            return ret;
    }

    ...

    avctx->frame_number++;

    return 0;
}
```

可以看到，这个函数的逻辑很简单，它首先会尝试从缓存中直接获取 AVFrame，如果不存在，则调用 decode_receive_frame_internal 进行解码。

# 总结

函数 avcodec_decode_video2 已经过时了，现在推荐使用的是 avcodec_send_packet() 和 avcodec_receive_frame()，而在新版本中，avcodec_decode_video2 也是通过这两个新的 API 完成解码功能的。

avcodec_send_packet 和 avcodec_receive_frame 内部的关键实现都是 decode_receive_frame_internal，最终调用的是 AVCodec 的两个函数指针之一：receive_frame 或 decode，但目前 ff_h264_decoder、ff_aac_decoder 等解码器依然只实现了 decode 这个函数。即 avcodec_send_packet 和 avcodec_receive_frame 最终调用的是 AVCodec 的函数指针 decode，对应不同的编码格式，decode 有不同的实现，以 ff_h264_decoder 为例，decode 对应的是 h264_decode_frame，它会根据 NAL Unit 的类型进行解码。




