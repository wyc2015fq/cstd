# FFmpeg(9)-解码器解码代码演示(FFmpeg调用MediaCodec实现硬解码、多线程解码、及音视频解码性能测试) - 夜行过客 - 博客园







# [FFmpeg(9)-解码器解码代码演示(FFmpeg调用MediaCodec实现硬解码、多线程解码、及音视频解码性能测试)](https://www.cnblogs.com/yongdaimi/p/9810379.html)





**一.AVFrame**

用来存放解码后的数据。

【相关函数】

AVFrame *frame = av_frame_alloc();                       // 空间分配，分配一个空间并初始化。

void av_frame_free(AVFrame **frame);                   // 空间释放。两种释放方式，一种是将引用计数-1,

int av_frame_ref(AVFrame *dst, const AVFrame *src); // 引用计数增加1。比如要在多线程访问的时候复制到另外一边，就可以利用引用计数的变化。

AVFrame *av_frame_clone(const AVFrame *src);      // 复制。也是重新创建一个空间，然后引用计数加+1。与AVPacket的区别在于：AVFrame的复制开销更大。1920*1080p的视频，一帧可能就有几MB，一秒钟可能就有几百MB，所以做一帧画面的内存复制可能都耗费到毫秒级别，不像AVPacket可能只有微秒级别，会影响帧率。所以在它的空间复制上一定要慎重，所以我们一般用引用计数的方式来做。

void av_frame_unref(AVFrame *frame);                   // 直接把对象的引用计数-1.



【结构体包含的内容】

uint8_t *data[AV_NUM_DATA,POINTERS]                // 存放的数据。

int linesize[AV_NUM_DATA,POINTERS]                    // 对于视频就是一行数据的大小。对于音频就是一个通道数据的大小。

int width, int height, int nb_samples                         // 视频部分， 音频相关(单通道的样本数量)

int64_ t pts                                                             // 实际这一帧的pts。

int64_t pkt_dts                                                       // 对应包当中的dts。

int sample_rate;                                                      // 样本率

uint64_t channel_layout;                                         // 通道类型

int channels;                                                          // 通道数量

int format;                                                              // 像素格式。区分音频和视频。视频的话就是AVPixelFormat，音频的话就是AVSampleFormat

**二.解码器解码代码演示**

```
// 初始化解封装
    av_register_all();
    // 注册解码器
    avcodec_register_all();

    // 初始化网络
    avformat_network_init();
    // 打开文件
    AVFormatContext *ic = NULL;
    char path[] = "sdcard/1080.mp4";
    // char path[] = "/sdcard/qingfeng.flv";
    int ret = avformat_open_input(&ic, path, 0, 0);
    if (ret != 0) {
        LOGE("avformat_open_input() called failed: %s", av_err2str(ret));
        return env->NewStringUTF(hello.c_str());
    }
    LOGI("avformat_open_input(): File open success.");
    LOGI("File duration is: %lld, nb_stream is: %d", ic->duration, ic->nb_streams);
    if (avformat_find_stream_info(ic, 0) >=0 ) {
        LOGI("File duration is: %lld, nb_stream is: %d", ic->duration, ic->nb_streams);
    }

    /**帧率*/
    int fps = 0;
    /*视频流索引*/
    int videoStream = 0;
    /*音频流索引*/
    int audioStream = 1;

    // 遍历获得音/视频流索引
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
                 as->codecpar->format);
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

    // 也可以利用av_find_best_stream()函数来查找音视频流索引
    // audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    // LOGI("av_find_best_stream, audio index is: %d", audioStream);


    // 查找视频解码器
    AVCodec *vCodec = avcodec_find_decoder(ic->streams[videoStream]->codecpar->codec_id); // 软解
    // vCodec = avcodec_find_decoder_by_name("h264_mediacodec"); // 硬解
    if (!vCodec) {
        LOGE("avcodec_find_decoder() failed. can not found video decoder.");
        return env->NewStringUTF(hello.c_str());
    }
    // 配置解码器上下文
    AVCodecContext *vc = avcodec_alloc_context3(vCodec);
    // 将AVStream里面的参数复制到上下文当中
    avcodec_parameters_to_context(vc, ic->streams[videoStream]->codecpar);
    vc->thread_count = 8;
    // 打开解码器
    ret = avcodec_open2(vc, vCodec, 0);
    if (ret != 0) {
        LOGE("avcodec_open2() failed. can not open video decoder, line is: %d", __LINE__);
        return env->NewStringUTF(hello.c_str());
    }

    // 查找音频解码器
    AVCodec *aCodec = avcodec_find_decoder(ic->streams[audioStream]->codecpar->codec_id); // 软解
    // aCodec= avcodec_find_decoder_by_name("h264_mediacodec"); // 硬解
    if (!aCodec) {
        LOGE("avcodec_find_decoder() failed. can not found audio decoder.");
        return env->NewStringUTF(hello.c_str());
    }
    // 配置解码器上下文
    AVCodecContext *ac = avcodec_alloc_context3(aCodec);
    // 将AVStream里面的参数复制到上下文当中
    avcodec_parameters_to_context(ac, ic->streams[audioStream]->codecpar);
    ac->thread_count = 8;
    // 打开解码器
    ret = avcodec_open2(ac, aCodec, 0);
    if (ret != 0) {
        LOGE("avcodec_open2() failed. can not open audio decoder");
        return env->NewStringUTF(hello.c_str());
    }

    // 读取帧数据
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    int64_t start = getNowMs();
    int frameCount = 0;

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
            }
            // LOGI("Receive a frame.........");
        }
    }

    // 关闭上下文
    avformat_close_input(&ic);
    return env->NewStringUTF(hello.c_str());
```

另外：硬件解码器需要进行注册，即需要把Java虚拟机的环境传递给FFmpeg，因此还需要添加下列代码，否则解码器无法打开。

```
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res)
{
    av_jni_set_java_vm(vm,0);
    return JNI_VERSION_1_4;
}
```

av_jni_set_java_vm(vm,0); 包含在 <libavcodec/jni.h> 头文件当中，不要忘记包含该头文件。














