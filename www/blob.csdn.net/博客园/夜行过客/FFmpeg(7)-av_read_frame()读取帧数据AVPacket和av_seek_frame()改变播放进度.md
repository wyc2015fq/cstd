# FFmpeg(7)-av_read_frame()读取帧数据AVPacket和av_seek_frame()改变播放进度 - 夜行过客 - 博客园







# [FFmpeg(7)-av_read_frame()读取帧数据AVPacket和av_seek_frame()改变播放进度](https://www.cnblogs.com/yongdaimi/p/9800264.html)





**一.av_read_frame()**

该函数用于读取具体的音/视频帧数据
int av_read_frame(AVFormatContext *s, AVPacket *pkt);
参数说明：

AVFormatContext *s  　　// 文件格式上下文

AVPacket *pkt  　　　　 // 这个值不能传NULL，必须是一个空间，

　　　　　　　　　　　　// 返回值：return 0 is OK, <0 on error or end of file



**二.AVPacket**

2.1 结构体常见参数说明

AVBufferRef buf; 　　　 // 指向一个buf空间，该空间是用来存储引用计数的。有可能多个AVPacket指向的都是这个buf的空间。因此要在每次拷贝的时候对其进行+1，删除的时候对其进行-1.

int64_t pts 　　 　　　 // 显示时间。是一个long long 的类型。

int64_t des 　　　 // 解码时间。与上面一致，单位也是long long 类型。

uint8_t *data; int size// 指向的是AVBufferRef里面再分配的空间。该空间无法手动删除，因为是在动态库中创建的，需要调用FFmpeg提供的接口来删除。



2.2 相关函数说明

AVPacket *av_packet_alloc(void) 　　　　　　　// AVPacket空间的创建和初始化。创建一个AVPacket对象，它会在堆上面申请空间，因此还需要去手动释放。

AVPacket *av_packet_clone(const AVPacket *src); // AVPacket空间的复制。创建并应用引用计数， 同时分配空间并增加引用计数。

int av_packet_ref(AVPacket *dst, const AVPacket *src); // 手动对AVPacket的引用计数+1

int av_packet_unref(AVPacket *pkt);　　// 手动对AVPacket的引用计数-1

void av_packet_free(AVPacket **pkt); // 清空对象并减引用计数

void av_init_packet(AVPacket *pkt); // 初始化的一个接口，如果只是在栈中或者是new了一个对象，可以利用此函数进行初始化。

int av_packet_from_data(AVPacket *pkt, uint8_t *data, int size); // 分配空间的函数。如果创建了一个AVPacket,但还需要分配空间的话，可以调用该函数来分配空间。

int av_copy_packet(AVPacket *dst, const AVPacket *src);  // 早期函数，已过期。不推荐使用。





**三.av_seek_frame()**

该函数可以将音/视频seek到指定的位置。

参数说明：

AVFormatContext *s // 封装格式上下文

int streamIndex　　　 // 流的索引。默认值为-1，因为媒体文件中可能既包含视频又包含音频，可以通过streamIndex来指定究竟是以视频还是音频来移。

int64_t timestamp. // 时间戳。你要移动到哪个时间位置。

int flag 　　　　　 // 标识位。表示我们移动的策略(究竟是向前移，还是向后移)。



PS:av_seek_frame flag

#define AVSEEK_FLAG_BACKGROUND 1              ///<<Seek Background 往后移,

#define AVSEEK_FALG_BYTE 　　　　　　　 ///<<<seeking based on position in bytes让时间戳
变成一个byte, 按照文件的大小位置跳到那个位置

#define AVSEEK_FLAG_ANY 　　　　　　　 ///<<<seek to any frame, even non-keyframes // 移动到任意帧的位置，不去找前面的关键帧，

#define AVSEEK_FLAG_FRAME 　　　　　　 ///<<<seeking based on frame number // 找关键帧，一般与AVSEEK_FLAG_BACKGROUND一起使用





**四.代码示例**

```
// 读取一帧数据
    AVPacket *packet = av_packet_alloc();
    for (;;) {
        int ret = av_read_frame(ic, packet);
        if (ret != 0) {
            LOGI("读取到结尾处");
            int pos = 20 * r2d(ic->streams[videoStream]->time_base);
            // 改变播放进度
            av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
            continue;
        }
        LOGI("streamIndex=%d, size=%d, pts=%lld, flag=%d",
             packet->stream_index,
             packet->size,
             packet->pts,
             packet->flags
        );
        av_packet_unref(packet);
    }
```
















