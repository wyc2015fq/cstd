# FFmpeg 新旧版本编码 API 的区别 - 我的学习笔记 - CSDN博客





2018年10月14日 13:57:10[zouzhiheng](https://me.csdn.net/u011330638)阅读数：386标签：[ffmpeg																[h264](https://so.csdn.net/so/search/s.do?q=h264&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[音视频](https://blog.csdn.net/u011330638/article/category/7803889)








# 前言

FFmpeg 3.x 之前，视频编码函数为 avcodec_encode_video2，3.x 及之后的版本，avcodec_encode_video2 被弃用，取而代之的是 avcodec_send_frame() 和 avcodec_receive_packet()，下面将从 API 的使用和源码实现两个角度来分析它们的区别。

# API 的使用

## 旧版 API

下面摘抄了 ffmpeg 转码示例程序的部分代码：

```
static int encode_write_frame(AVFrame *frame, unsigned int stream_index, int *got_frame) {
    int ret;
    int got_frame_local;
    AVPacket enc_pkt;
    int (*enc_func)(AVCodecContext *, AVPacket *, const AVFrame *, int *) =
        (ifmt_ctx->streams[stream_index]->codecpar->codec_type ==
         AVMEDIA_TYPE_VIDEO) ? avcodec_encode_video2 : avcodec_encode_audio2;

    if (!got_frame)
        got_frame = &got_frame_local;

    av_log(NULL, AV_LOG_INFO, "Encoding frame\n");
    /* encode frame */
    enc_pkt.data = NULL;
    enc_pkt.size = 0;
    av_init_packet(&enc_pkt);
    ret = enc_func(stream_ctx[stream_index].enc_ctx, &enc_pkt,
            frame, got_frame);
    av_frame_free(&frame);
    if (ret < 0)
        return ret;
    if (!(*got_frame))
        return 0;

    /* prepare packet for muxing */
    enc_pkt.stream_index = stream_index;
    av_packet_rescale_ts(&enc_pkt,
                         stream_ctx[stream_index].enc_ctx->time_base,
                         ofmt_ctx->streams[stream_index]->time_base);

    av_log(NULL, AV_LOG_DEBUG, "Muxing frame\n");
    /* mux encoded frame */
    ret = av_interleaved_write_frame(ofmt_ctx, &enc_pkt);
    return ret;
}
```

## 新版 API

下面摘抄的是 ffmpeg 视频编码示例程序的部分代码：

```
static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
                   FILE *outfile)
{
    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}
```

可以看到，在使用上，新旧版本的编码 API 的主要区别有：
- 旧版 API 一个函数即可完成编码操作，编码成功后可直接使用压缩后的数据。新版 API 需要两个函数一起使用，一个 send，一个 receive，分别用于发送原始视频数据、获取编码后的数据；具体在哪里完成了编码动作，暂时未知。
- 旧版 API 一次编码动作对应 0 个或 1 个 AVFrame 和 0 个或 1 个 AVPacket。新本 API 一次编码动作对应 0 个或 1 个 AVFrame 和 0 个或多个 AVPacket。

# 源码实现

## 函数声明

### avcodec_encode_video2

```
/**
* Encode a frame of video.
*
* Takes input raw video data from frame and writes the next output packet, if
* available, to avpkt. The output packet does not necessarily contain data for
* the most recent frame, as encoders can delay and reorder input frames
* internally as needed.
*
* @param avctx     codec context
* @param avpkt     output AVPacket.
 *                  The user can supply an output buffer by setting
 *                  avpkt->data and avpkt->size prior to calling the
 *                  function, but if the size of the user-provided data is not
 *                  large enough, encoding will fail. All other AVPacket fields
 *                  will be reset by the encoder using av_init_packet(). If
 *                  avpkt->data is NULL, the encoder will allocate it.
 *                  The encoder will set avpkt->size to the size of the
 *                  output packet. The returned data (if any) belongs to the
 *                  caller, he is responsible for freeing it.
 *
 *                  If this function fails or produces no output, avpkt will be
 *                  freed using av_packet_unref().
* @param[in] frame AVFrame containing the raw video data to be encoded.
*                  May be NULL when flushing an encoder that has the
*                  AV_CODEC_CAP_DELAY capability set.
* @param[out] got_packet_ptr This field is set to 1 by libavcodec if the
*                            output packet is non-empty, and to 0 if it is
*                            empty. If the function returns an error, the
*                            packet can be assumed to be invalid, and the
*                            value of got_packet_ptr is undefined and should
*                            not be used.
* @return          0 on success, negative error code on failure
*
* @deprecated use avcodec_send_frame()/avcodec_receive_packet() instead
*/
attribute_deprecated
int avcodec_encode_video2(AVCodecContext *avctx, AVPacket *avpkt,
                          const AVFrame *frame, int *got_packet_ptr);
```

这个函数很简单，需要注意的点有 2 个：
- 编码成功后，AVPacket 不一定包含数据
- 输入的 AVFrame 可以为 NULL，用于刷新编码器，获取剩余的 AVPacket

## avcodec_send_frame

avcodec_send_frame 的声明如下：

```
/**
* Supply a raw video or audio frame to the encoder. Use avcodec_receive_packet()
* to retrieve buffered output packets.
*
* @param avctx     codec context
* @param[in] frame AVFrame containing the raw audio or video frame to be encoded.
*                  ...
*                  It can be NULL, in which case it is considered a flush
*                  packet.  This signals the end of the stream. If the encoder
*                  still has packets buffered, it will return them after this
*                  call. Once flushing mode has been entered, additional flush
*                  packets are ignored, and sending frames will return
*                  AVERROR_EOF.
*
*                  For audio:
*                  If AV_CODEC_CAP_VARIABLE_FRAME_SIZE is set, then each frame
*                  can have any number of samples.
*                  If it is not set, frame->nb_samples must be equal to
*                  avctx->frame_size for all frames except the last.
*                  The final frame may be smaller than avctx->frame_size.
*
* @return 0 on success, otherwise negative error code:
*      AVERROR(EAGAIN):   input is not accepted in the current state - user
*                         must read output with avcodec_receive_packet() (once
*                         all output is read, the packet should be resent, and
*                         the call will not fail with EAGAIN).
*      AVERROR_EOF:       the encoder has been flushed, and no new frames can
*                         be sent to it
*      AVERROR(EINVAL):   codec not opened, refcounted_frames not set, it is a
*                         decoder, or requires flush
*      AVERROR(ENOMEM):   failed to add packet to internal queue, or similar
*      other errors: legitimate decoding errors
*/
int avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame);
```

从注释中可以看出，这个函数用于发送原始的视频/音频数据给编码器编码，参数 AVFrame  同样可以为 NULL 以刷新编码器。

### avcodec_receive_packet

avcodec_receive_packet 则用于获取编码后的视频/音频数据。它的声明如下：

```
/**
* Read encoded data from the encoder.
*
* @param avctx codec context
* @param avpkt This will be set to a reference-counted packet allocated by the
*              encoder. Note that the function will always call
*              av_frame_unref(frame) before doing anything else.
* @return 0 on success, otherwise negative error code:
*      AVERROR(EAGAIN):   output is not available in the current state - user
*                         must try to send input
*      AVERROR_EOF:       the encoder has been fully flushed, and there will be
*                         no more output packets
*      AVERROR(EINVAL):   codec not opened, or it is an encoder
*      other errors: legitimate decoding errors
*/
int avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt);
```

## 函数定义

### avcodec_encode_video2

avcodec_encode_video2 的实现如下：

```
int attribute_align_arg avcodec_encode_video2(AVCodecContext *avctx,
                                              AVPacket *avpkt,
                                              const AVFrame *frame,
                                              int *got_packet_ptr)
{
    int ret;
    AVPacket user_pkt = *avpkt;
    int needs_realloc = !user_pkt.data;

    *got_packet_ptr = 0;

    if (!avctx->codec->encode2) {
        av_log(avctx, AV_LOG_ERROR, "This encoder requires using the avcodec_send_frame() API.\n");
        return AVERROR(ENOSYS);
    }

    // 获取异步编码缓存的 AVPacket
    if(CONFIG_FRAME_THREAD_ENCODER &&
       avctx->internal->frame_thread_encoder && (avctx->active_thread_type&FF_THREAD_FRAME))
        return ff_thread_video_encode_frame(avctx, avpkt, frame, got_packet_ptr);

    ... // 容错处理

	// 编码
    ret = avctx->codec->encode2(avctx, avpkt, frame, got_packet_ptr);
    av_assert0(ret <= 0);

    emms_c();

    if (avpkt->data && avpkt->data == avctx->internal->byte_buffer) {
        needs_realloc = 0;
        if (user_pkt.data) {
            if (user_pkt.size >= avpkt->size) {
                memcpy(user_pkt.data, avpkt->data, avpkt->size);
            } else {
                av_log(avctx, AV_LOG_ERROR, "Provided packet is too small, needs to be %d\n", avpkt->size);
                avpkt->size = user_pkt.size;
                ret = -1;
            }
            avpkt->buf      = user_pkt.buf;
            avpkt->data     = user_pkt.data;
        } else if (!avpkt->buf) {
            AVPacket tmp = { 0 };
            ret = av_packet_ref(&tmp, avpkt);
            av_packet_unref(avpkt);
            if (ret < 0)
                return ret;
            *avpkt = tmp;
        }
    }

    if (!ret) {
        if (!*got_packet_ptr)
            avpkt->size = 0;
        else if (!(avctx->codec->capabilities & AV_CODEC_CAP_DELAY))
            avpkt->pts = avpkt->dts = frame->pts;

        if (needs_realloc && avpkt->data) {
            ret = av_buffer_realloc(&avpkt->buf, avpkt->size + AV_INPUT_BUFFER_PADDING_SIZE);
            if (ret >= 0)
                avpkt->data = avpkt->buf->data;
        }

        avctx->frame_number++;
    }

    if (ret < 0 || !*got_packet_ptr)
        av_packet_unref(avpkt);

    return ret;
}
```

可以看到，这个函数很简单，关键代码只有一句：avctx->codec->encode2。除此之外，就是调整了一些结构体的数据而已。encode2 是 AVCodec 的函数指针，不同的编码格式对应不同的实现（参考 [FFmpeg 是如何实现多态的？](https://blog.csdn.net/leixiaohua1020/article/details/44206485)），比如编码器 ff_libx264_encoder 对应的实现函数是 X264_frame()，这里就不深入分析了。

### avcodec_send_frame

```
int attribute_align_arg avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame)
{
    if (!avcodec_is_open(avctx) || !av_codec_is_encoder(avctx->codec))
        return AVERROR(EINVAL);

    if (avctx->internal->draining)
        return AVERROR_EOF;

    if (!frame) {
        avctx->internal->draining = 1;

        if (!(avctx->codec->capabilities & AV_CODEC_CAP_DELAY))
            return 0;
    }

    if (avctx->codec->send_frame)
        return avctx->codec->send_frame(avctx, frame);

    // Emulation via old API. Do it here instead of avcodec_receive_packet, because:
    // 1. if the AVFrame is not refcounted, the copying will be much more
    //    expensive than copying the packet data
    // 2. assume few users use non-refcounted AVPackets, so usually no copy is
    //    needed

    if (avctx->internal->buffer_pkt_valid)
        return AVERROR(EAGAIN);

    return do_encode(avctx, frame, &(int){0});
}
```

avcodec_send_frame 的实现同样很简单，首先会尝试使用 AVCodec 的函数指针 send_frame 进行编码，假如对应的编码器没有实现这个函数指针，则调用 do_encode 执行旧版本的编码实现。

下面看一下 libx264 这个编码器：

```
AVCodec ff_libx264_encoder = {
    .name             = "libx264",
    .long_name        = NULL_IF_CONFIG_SMALL("libx264 H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
    .type             = AVMEDIA_TYPE_VIDEO,
    .id               = AV_CODEC_ID_H264,
    .priv_data_size   = sizeof(X264Context),
    .init             = X264_init,
    .encode2          = X264_frame,
    .close            = X264_close,
    .capabilities     = AV_CODEC_CAP_DELAY | AV_CODEC_CAP_AUTO_THREADS,
    .priv_class       = &x264_class,
    .defaults         = x264_defaults,
    .init_static_data = X264_init_static,
    .caps_internal    = FF_CODEC_CAP_INIT_THREADSAFE |
                        FF_CODEC_CAP_INIT_CLEANUP,
};
```

可以看到，这个编码器没有 send_frame 这个函数指针对应的函数实现，ff_aac_encoder 等编码器同样也没有。目前仅发现编码器 ff_hevc_nvenc_encoder 实现了 send_frame 这个函数指针，但这部分内容更接近 H265 本身而不是 FFmpeg 了，因此略过，下面直接看 do_encode。

```
static int do_encode(AVCodecContext *avctx, const AVFrame *frame, int *got_packet)
{
    int ret;
    *got_packet = 0;

    av_packet_unref(avctx->internal->buffer_pkt);
    avctx->internal->buffer_pkt_valid = 0;

	// 视频/音频编码
    if (avctx->codec_type == AVMEDIA_TYPE_VIDEO) {
        ret = avcodec_encode_video2(avctx, avctx->internal->buffer_pkt,
                                    frame, got_packet);
    } else if (avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        ret = avcodec_encode_audio2(avctx, avctx->internal->buffer_pkt,
                                    frame, got_packet);
    } else {
        ret = AVERROR(EINVAL);
    }

    if (ret >= 0 && *got_packet) {
        // Encoders must always return ref-counted buffers.
        // Side-data only packets have no data and can be not ref-counted.
        av_assert0(!avctx->internal->buffer_pkt->data || avctx->internal->buffer_pkt->buf);
        avctx->internal->buffer_pkt_valid = 1;
        ret = 0;
    } else {
        av_packet_unref(avctx->internal->buffer_pkt);
    }

    return ret;
}
```

从代码中可以看出，do_encode 的作用仅仅是判断当前的 AVCodecContext 的类型，然后再决定执行音频的编码函数还是视频的编码函数。如果是视频，则执行函数 avcodec_encode_video2；如果是音频，则执行函数 avcodec_encode_audio2。即 do_encode 最终执行的是旧版本的编码 API。

### avcodec_receive_packet

下面看 avcodec_receive_packet 的实现：

```
int attribute_align_arg avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt)
{
    av_packet_unref(avpkt);

    if (!avcodec_is_open(avctx) || !av_codec_is_encoder(avctx->codec))
        return AVERROR(EINVAL);

    if (avctx->codec->receive_packet) {
        if (avctx->internal->draining && !(avctx->codec->capabilities & AV_CODEC_CAP_DELAY))
            return AVERROR_EOF;
        return avctx->codec->receive_packet(avctx, avpkt);
    }

    // Emulation via old API.

    if (!avctx->internal->buffer_pkt_valid) {
        int got_packet;
        int ret;
        if (!avctx->internal->draining)
            return AVERROR(EAGAIN);
        ret = do_encode(avctx, NULL, &got_packet);
        if (ret < 0)
            return ret;
        if (ret >= 0 && !got_packet)
            return AVERROR_EOF;
    }

    av_packet_move_ref(avpkt, avctx->internal->buffer_pkt);
    avctx->internal->buffer_pkt_valid = 0;
    return 0;
}
```

从代码中可以看出，这个函数的实现和 avcodec_send_frame 一样，都是先判断当前编码器是否实现了 receive_packet 这个函数指针，如果实现了就直接调用该函数指针，否则调用 do_encode 这个方法。和 avcodec_send_frame 不同的是，do_encode 的第二个参数 AVFrame 为空，这是因为在执行  avcodec_send_frame  的时候，音频/视频的原始数据就已经编码成功了，因此可以传递 NULL（传递 NULL 用于刷新编码器，在上面的函数声明中有说到），直接获取  AVPacket 即可。

# 总结

新旧版本的编码 API 的主要区别是：
- 旧版本视频编码使用 avcodec_encode_video2，音频编码使用 avcodec_encode_audio2；新版本音视频编码统一使用 avcodec_send_frame 和 avcodec_receive_packet
- 旧版本 API 内部直接调用了 AVCodec 的函数指针 encode2；新版本 API 首先会判断编码器是否实现了函数指针 send_frame 和 receive_packet，如果实现了，优先使用send_frame 和 receive_packet，否则使用旧版本的 encode2
- 目前仅发现编码器 ff_hevc_nvenc_encoder 实现了新版本的 API（send_frame 和 receive_packet），libx264、AAC 等编码器依然使用了旧版本的 API（encode2）




