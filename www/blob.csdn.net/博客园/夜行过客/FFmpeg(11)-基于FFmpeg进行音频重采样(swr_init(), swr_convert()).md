# FFmpeg(11)-基于FFmpeg进行音频重采样(swr_init(), swr_convert()) - 夜行过客 - 博客园







# [FFmpeg(11)-基于FFmpeg进行音频重采样(swr_init(), swr_convert())](https://www.cnblogs.com/yongdaimi/p/9830268.html)





**一.包含头文件和库文件**

修改CMakeLists

```
# swresample
add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FF}/libswresample.so)
```

更新target_link_libraries

```
target_link_libraries( # Specifies the target library.
                       native-lib
                       avcodec
                       avformat
                       avutil
                       swscale
                       swresample

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
```

在代码中包含头文件 #include <libswresample/swresample.h>

**二.相关函数说明**

a)

SwrContext *swr_alloc(void);  // 分配重采样的上下文。

SwrContext *swr_alloc_set_opts(struct SwrContext *s, int64_t out_ch_layout, AVSampleFormat out_sample_fmt, int out_sample_rate

, int64_t in_ch_layout, AVSampleFormat in_sample_fmt, int in_sample_rate, int log_offset, void *log_ctx

);

参数1：重采样上下文

参数2：输出的layout, 如：5.1声道…

参数3：输出的样本格式。Float, S16, S24

参数4：输出的样本率。可以不变。

参数5：输入的layout。

参数6：输入的样本格式。

参数7：输入的样本率。

参数8，参数9，日志，不用管，可直接传0

针对音频的播放速度，可以通过样本率的改变而改变。



int swr_init(struct SwrContext *s);                       // 初始化上下文

void swr_free(struct SwrContext **s);                  // 释放上下文空间。



b) 

swr_convert()

针对每一帧音频的处理。把一帧帧的音频作相应的重采样

int swr_convert(struct SwrContext *s, uint8_t **out, int out_count, const uint8_t **in, int in_count);

参数1：音频重采样的上下文

参数2：输出的指针。传递的输出的数组

参数3：输出的样本数量，不是字节数。单通道的样本数量。

参数4：输入的数组，AVFrame解码出来的DATA

参数5：输入的单通道的样本数量。

三.示例代码

```
// 初始化像素格式转换上下文
    SwsContext *vctx = NULL;
    int outWidth = 1280;
    int outHeight = 720;
    char *rgb = new char[1920*1080*4];
    char *pcm = new char[48000*4*2];

    // 初始化音频重采样上下文
    SwrContext *actx = swr_alloc();
    actx = swr_alloc_set_opts(
            actx,
            av_get_default_channel_layout(2),
            AV_SAMPLE_FMT_S16,
            ac->sample_rate,
            av_get_default_channel_layout(ac->channels),
            ac->sample_fmt,
            ac->sample_rate,
            0, 0
    );

    ret = swr_init(actx);
    if (ret != 0) {
        LOGE("swr_init failed");
    } else {
        LOGI("swr_init success");
    }

    for (;;) {

        if (getNowMs() - start >= 3000) {
            LOGI("now decoder fps is: %d", frameCount / 3);
            start = getNowMs();
            frameCount = 0;
        }
        int ret = av_read_frame(ic, packet);
        if (ret != 0) {
            LOGE("读取到结尾处");
            int pos = 20 * r2d(ic->streams[videoStream]->time_base);
            // 改变播放进度
            av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
            continue;
        }

//        LOGI("Read a Packet. streamIndex=%d, size=%d, pts=%lld, flag=%d",
//             packet->stream_index,
//             packet->size,
//             packet->pts,
//             packet->flags
//        );

        AVCodecContext *cc = vc;
        if (packet->stream_index == audioStream) cc = ac;
        // 发送到线程中去解码(将packet写入到解码队列当中去)
        ret = avcodec_send_packet(cc, packet);
        // 清理
        int p = packet->pts;
        av_packet_unref(packet);
        if (ret != 0) {
            // LOGE("avcodec_send_packet failed.");
            continue;
        }

        for(;;) {
            // 从已解码成功的数据当中取出一个frame, 要注意send一次,receive不一定是一次
            ret = avcodec_receive_frame(cc, frame);
            if (ret != 0) {
                break;
            }
            if (cc == vc) {
                frameCount++;
                vctx = sws_getCachedContext(
                        vctx,
                        frame->width,
                        frame->height,
                        (AVPixelFormat)frame->format,
                        outWidth,
                        outHeight,
                        AV_PIX_FMT_RGBA,
                        SWS_FAST_BILINEAR,
                        0, 0, 0
                );
                if (!vctx) {
                    LOGE("sws_getCachedContext failed!");
                } else {
                    // 开始像素格式转换
                    uint8_t  *data[AV_NUM_DATA_POINTERS] = {0};
                    data[0] = (uint8_t *)rgb;
                    int lines[AV_NUM_DATA_POINTERS] = {0};
                    lines[0] = outWidth * 4;
                    int h = sws_scale(
                            vctx,
                            (const uint8_t **)frame->data,
                            frame->linesize,
                            0,
                            frame->height,
                            data, lines
                    );
                    LOGI("sws_scale = %d", h);
                }
            } else { // 音频部分
                uint8_t *out[2] = {0};
                out[0] = (uint8_t *)pcm;
                // 音频重采样
                int len = swr_convert(
                        actx,
                        out,
                        frame->nb_samples,
                        (const uint8_t **)frame->data,
                        frame->nb_samples
                );
                LOGI("swr_convert = %d", len);
            }
            // LOGI("Receive a frame.........");
        }
    }

    delete rgb;
    delete pcm;
```














