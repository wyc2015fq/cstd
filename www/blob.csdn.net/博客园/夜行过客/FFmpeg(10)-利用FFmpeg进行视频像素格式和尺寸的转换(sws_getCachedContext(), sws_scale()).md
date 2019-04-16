# FFmpeg(10)-利用FFmpeg进行视频像素格式和尺寸的转换(sws_getCachedContext(), sws_scale()) - 夜行过客 - 博客园







# [FFmpeg(10)-利用FFmpeg进行视频像素格式和尺寸的转换(sws_getCachedContext(), sws_scale())](https://www.cnblogs.com/yongdaimi/p/9829290.html)





**一.包含头文件和库文件**

像素格式的相关函数包含在 libswscale.so 库中，CMakeLists需要做下列改动：

```
# swscale
add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${FF}/libswscale.so)
```

target_link_libraries

```
target_link_libraries( # Specifies the target library.
                       native-lib
                       avcodec
                       avformat
                       avutil
                       swscale

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
```

同时在代码中包含头文件 #include <libswscale/swscale.h>

**二. 相关函数说明**

a)

sws_getContext()                                                        // 像素格式转换的上下文。会创建新的空间

struct SwsContext *sws_getCachedContext();           // 函数名补全。会根据传入的上下文到缓冲里面去找。

参数说明：
- 第一参数可以传NULL，默认会开辟一块新的空间。
- srcW,srcH, srcFormat， 原始数据的宽高和原始像素格式(YUV420)，
- dstW,dstH,dstFormat;   目标宽，目标高，目标的像素格式(这里的宽高可能是手机屏幕分辨率，RGBA8888)，这里不仅仅包含了尺寸的转换和像素格式的转换
- flag 提供了一系列的算法，快速线性，差值，矩阵，不同的算法性能也不同，快速线性算法性能相对较高。只针对尺寸的变换。对像素格式转换无此问题
- 
#define SWS_FAST_BILINEAR 1

#define SWS_BILINEAR 2

#define SWS_BICUBIC 4

#define SWS_X      8

#define SWS_POINT 0x10

#define SWS_AREA 0x20

#define SWS_BICUBLIN 0x40


后面还有两个参数是做过滤器用的，一般用不到，传NULL，最后一个参数是跟flag算法相关，也可以传NULL。

b)

 int sws_scale();

具体每一帧数据的处理。

struct SwsContext *c                   // 像素格式转换的上下文

uint8_t *const srcSlice[]                // src的Slice，对应的具体数据的数组，是指针数组，也就是二维数组，YUV420P(三行数据)

const int srcStride[]                      // linesize, 一行对应的大小。

int srcSliceY                                 // 用不到，直接传0即可。

int srcSliceH                                // 图像的高度。

uint8_t *const dst[]                      // 目标的地址。也是一个指针数组。

const int dstStride[]                     // 输入的linesize

**三, 相关代码参考**

```
// 初始化像素格式转换上下文
    SwsContext *vctx = NULL;
    int outWidth = 1280;
    int outHeight = 720;
    char *rgb = new char[1920*1080*4];

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
            }
            // LOGI("Receive a frame.........");
        }
    }
```














