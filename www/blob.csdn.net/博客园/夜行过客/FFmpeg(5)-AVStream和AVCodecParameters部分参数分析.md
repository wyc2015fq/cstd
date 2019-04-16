# FFmpeg(5)-AVStream和AVCodecParameters部分参数分析 - 夜行过客 - 博客园







# [FFmpeg(5)-AVStream和AVCodecParameters部分参数分析](https://www.cnblogs.com/yongdaimi/p/9798001.html)





**一.AVStream**

AVCodecContext *codec　　　　　　　　// 已过时，使用另一个 codecpar 结构体代替。

AVRational time_base　　　　　　　　　// 时间基数。

int64_t duration　　　　　　　　　　　　// 总时长。流的总时长，该参数不可靠。

AVRational avg_frame_rate　　　　　　  // 帧率。

AVCodecParameters *codecpar;　　　　 // 包含音视频参数的结构体。很重要，可以用来获取音视频参数中的宽度、高度、采样率、编码格式等信息。

**二. AVCodecParameters **

enum AVMediaType codec_type; 　　　// 编码类型。说明这段流数据究竟是音频还是视频。

enum AVCodecID codec_id     　　　　 // 编码格式。说明这段流的编码格式，h264，MPEG4, MJPEG，etc...

uint32_t  codecTag;                   　　　  //  一般不用

int format;                                 　　　　//  格式。对于视频来说指的就是像素格式(YUV420,YUV422...)，对于音频来说，指的就是音频的采样格式。

int width, int height;                  　　　　// 视频的宽高，只有视频有

uint64_t channel_layout;             　　　// 取默认值即可

int channels;                             　　　   // 声道数

int sample_rate;                    　　　　   // 样本率

int frame_size;                            　　　 // 只针对音频，一帧音频的大小

例：打印视频的宽高、编码格式，音频的采样率等信息

```
AVFormatContext *ic = NULL;
    char path[] = "sdcard/1080.mp4";
    // char path[] = "/sdcard/qingfeng.flv";
    int ret = avformat_open_input(&ic, path, 0, 0);
    if (ret != 0) {
        LOGE("avformat_open_input() called failed: %s", av_err2str(ret));
        return env->NewStringUTF(hello.c_str());
    }
    LOGI("avformat_open_input() called success.");
    LOGI("duration is: %lld, nb_stream is: %d", ic->duration, ic->nb_streams);
    if (avformat_find_stream_info(ic, 0) >=0 ) {
        LOGI("duration is: %lld, nb_stream is: %d", ic->duration, ic->nb_streams);
    }

    /**帧率*/
    int fps = 0;
    int videoStream = 0;
    int audioStream = 1;

    for (int i = 0; i < ic->nb_streams; i++) {
        AVStream *as = ic->streams[i];
        if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            LOGI("视频数据");
            videoStream = i;
            fps = (int)r2d(as->avg_frame_rate);
            LOGI("fps = %d, width = %d, height = %d, codecid = %d, format = %d",
                 fps,
                 as->codecpar->width,
                 as->codecpar->height,
                 as->codecpar->codec_id,
                 as->codecpar->format); AVSampleFormat
        } else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            LOGI("音频数据");
            audioStream = i;
            LOGI("sample_rate = %d, channels = %d, sample_format = %d",
                 as->codecpar->sample_rate,
                 as->codecpar->channels,
                 as->codecpar->format
            );
        }
    }
```














