# FFmpeg(2)-avformat_open_input()函数详解并示例打开mp4文件 - 夜行过客 - 博客园







# [FFmpeg(2)-avformat_open_input()函数详解并示例打开mp4文件](https://www.cnblogs.com/yongdaimi/p/9796349.html)





**一. 解封装**

pts 是显示的时间 dts是解码的时间， 这个时间是用来做同步。
- av_register_all(), 注册所有的格式。包括解封装格式和加封装格式。
- avformat_network_init(), 用于初始化网络。FFmpeg本身也支持解封装RTSP的数据,如果要解封装网络数据格式，则可调用该函数。
- avformat_open_input()。打开一个文件并解析。可解析的内容包括：视频流、音频流、视频流参数、音频流参数、视频帧索引。
- avformat_find_stream_info()，查找格式和索引。有些早期格式它的索引并没有放到头当中，需要你到后面探测，就会用到此函数。
- av_find_best_stream(), 当视频被解封装出来后，需要分开处理音频和视频，需要找到对应的音频流和视频流
- 解封装后涉及到的结构体：

AVFormatContext: 封装的上下文

AVStream : 存放的是音频流或视频流的参数信息

AVPacket: 针对于具体的解封装完后的一个一个的数据包。

av_read_frame() 用于读取一个AVPacket,AVPacket里面包含了这个包的pts,dts,还有这个包的stream index(它是音频还是视频),是否是关键帧，AVPacket把h264数据的间隔符去掉了。

**二.avformat_open_input()函数详解及mp4文件打开示例**

2.1 avformat_open_input函数详解

该函数用于打开一个输入的封装器。在调用该函数之前，须确保av_register_all()和avformat_network_init()已调用。

参数说明：

AVFormatContext **ps, 格式化的上下文。要注意，如果传入的是一个AVFormatContext*的指针，则该空间须自己手动清理，若传入的指针为空，则FFmpeg会内部自己创建。

const char *url, 传入的地址。支持http,RTSP,以及普通的本地文件。地址最终会存入到AVFormatContext结构体当中。

AVInputFormat *fmt, 指定输入的封装格式。一般传NULL，由FFmpeg自行探测。

AVDictionary **options, 其它参数设置。它是一个字典，用于参数传递，不传则写NULL。参见：libavformat/options_table.h,其中包含了它支持的参数设置。

2.2 mp4文件打开示例

```
extern "C" JNIEXPORT jstring
JNICALL
Java_com_yuneec_yongdaimi_ff_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    hello += avcodec_configuration();

    // 初始化解封装
    av_register_all();
    // 初始化网络
    avformat_network_init();
    // 打开文件
    AVFormatContext *ic = NULL;
    char path[] = "sdcard/1080.mp4";
    int ret = avformat_open_input(&ic, path, 0, 0);
    if (ret == 0) {
        LOGI("avformat_open_input() called success.");
    } else {
        LOGE("avformat_open_input() called failed: %s", av_err2str(ret));
    }
    return env->NewStringUTF(hello.c_str());
}
```

PS：

1.这里用到了avformat_open_input()函数，需要包含其对应的头文件： `#include <libavformat/avformat.h>` ；

2.代码中还用到了 `av_err2str()` 这个函数，因此CMakeLists.txt中还需要包含它所对应的 `libavutil.so` 库，相关的CMakeLists.txt如下：

```
# avcodec
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FF}/libavcodec.so)

# avformat
add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${FF}/libavformat.so)

# avutil
add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FF}/libavutil.so)
```

另外，不要忘记添加链接：

```
target_link_libraries( # Specifies the target library.
                       native-lib
                       avcodec
                       avformat
                       avutil

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
```














