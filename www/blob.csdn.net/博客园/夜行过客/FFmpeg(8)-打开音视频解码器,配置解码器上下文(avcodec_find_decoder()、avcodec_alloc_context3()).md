# FFmpeg(8)-打开音视频解码器,配置解码器上下文(avcodec_find_decoder()、avcodec_alloc_context3()) - 夜行过客 - 博客园







# [FFmpeg(8)-打开音视频解码器,配置解码器上下文(avcodec_find_decoder()、avcodec_alloc_context3())](https://www.cnblogs.com/yongdaimi/p/9804699.html)





**一.avcodec_find_decoder**

获取解码器。在使用之前必须保证所用到的解码器已经注册，最简单的就是调用**avcodec_register_all**() 函数，就像之前注册解封装器的时候，也要注册一下。。

AVCodec *avcodec_find_decoder(enum AVCodecID id); 

// 查找解码器，第一种方法就是直接通过ID号查找，这个ID号从哪里获取呢？就像刚才我们解封装之后，你可以发现我们的AVStream里面其实是有一个codecID, 那个ID号就是我们要用到的解码器的ID号。当然如果本身知道格式的ID号，也可以直接传进去(一般我们用h264,那这个codecID就是28)。找到这个解码器，然后返回到AVCodec当中去。AVCodec当中存放的是解码器格式的配置信息，并不代表最终要处理的解码器。

AVCodec *avcodec_find_decoder_by_name(const char name); 

// 除了通过解码器的ID号来查找解码器，还可能通过名字打开解码器。例：avcodec_find_decoder_by_name(“h264_mediacodec”);  // 用Android里面自带的解码模块)



**二.AVCodecContext 解码器上下文**

AVCodecContext *avcode_alloc_context3(const AVCodec *codec);       // 申请AVCodecContext空间。需要传递一个编码器，也可以不传，但不会包含编码器。

void avcodec_free_context(AVCodecContext **avctx);                          // 清理并AVCodecContext空间。

int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);

// 打开视频解码器。如果在 avcode_alloc_context3 的时候没有传解码器，则在此需要进行传递，后面的options是可选参数。参见：libavcodec/options_table.h。

AVCodecContext 的常用参数：

int thread_count;　// 用于配置解码线程数

time_base             // 时间基数。

三.**avcodec_parameters_to_context**

avcodec_parameters_to_context(codec, p)。该函数用于将流里面的参数，也就是AVStream里面的参数直接复制到AVCodecContext的上下文当中。



**四. 打开音视频解码器示例**

```
// 注册解码器
avcodec_register_all();
AVCodec *vc = avcodec_find_decoder(ic->streams[videoStream]->codecpar->codec_id); // 软解
    // vc = avcodec_find_decoder_by_name("h264_mediacodec"); // 硬解
    if (!vc) {
        LOGE("avcodec_find_decoder[videoStream] failure");
        return env->NewStringUTF(hello.c_str());
    }
    // 配置解码器
    AVCodecContext *vct = avcodec_alloc_context3(vc);
    avcodec_parameters_to_context(vct, ic->streams[videoStream]->codecpar);
    vct->thread_count = 1;
    // 打开解码器
    int re = avcodec_open2(vct, vc, 0);
    if (re != 0) {
        LOGE("avcodec_open2 failure");
        return env->NewStringUTF(hello.c_str());
    }
```














