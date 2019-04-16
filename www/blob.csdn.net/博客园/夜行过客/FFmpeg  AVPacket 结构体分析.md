# FFmpeg: AVPacket 结构体分析 - 夜行过客 - 博客园







# [FFmpeg: AVPacket 结构体分析](https://www.cnblogs.com/yongdaimi/p/10654519.html)





**AVPacket**是FFmpeg中很重要的一个数据结构，它保存了**解封装之后，解码之前的数据**（注意：仍然是压缩后的数据）和关于这些数据的一些附加信息，如**显示时间戳（pts）**、**解码时间戳（dts）**、**数据时长**，**所在媒体流的索引**等。对于视频来说，AVPacket通常包含一个压缩的Frame，而音频（Audio）则有可能包含多个压缩的Frame。并且，一个Packet有可能是空的，不包含任何压缩数据，只含有side data（side data，容器提供的关于Packet的一些附加信息。例如，在编码结束的时候更新一些流的参数）。AVPacket的大小是公共的ABI（public ABI）一部分，这样的结构体在FFmpeg很少，由此也可见AVPacket的重要性。它可以被分配在栈空间上（可以使用语句AVPacket packet; 在栈空间定义一个Packet ），并且除非libavcodec 和 libavformat有很大的改动，不然不会在AVPacket中添加新的字段。

**

```
/**
 * 该结构用来存储压缩数据. 这是典型的解复合器的输出然后塞入解码器，或者是接收编码器的输出然后塞入复合器
 *
 * 解复合器 --> AVPacket --> 解码器 --> YUV/RGB --> 编码器 --> AVPacket -->复合器
 *
 * 对视频而言, 它通常包含一个压缩帧. 对音频而言，则它可能包含多个压缩帧. 
 * 编码器允许输出一个空包, 没有压缩数据, 仅包含侧数据。
 * （例如在编码结束时更新一些流参数）
 *
 * AVPacket是FFmpeg少数几个结构之一, 其大小是公共ABI的一部分.
 *
 * 数据所有权的语义取决于buf域.
 * 如果被设置, 分组数据动态分配，且永远有效，直到一个叫av_packet_unref()减少引用计数为0时才被释放
 *
 * 如果buf域没有被设置，那么av_packet_ref()将做一个复制而不会增加引用计数
 *
 * 附加数据始终由av_malloc()分配内存, 由av_packet_ref()进行拷贝，由av_packet_unref()执行释放.
 *
 * @浏览 av_packet_ref()
 * @浏览 av_packet_unref()
 */
typedef struct AVPacket 
{
    // 用来管理data指针引用的数据缓存的
    // 为NULL时，那么数据包是不计数的
    AVBufferRef *buf;
 
    // 显示时间戳, 对应时间戳AVStream->time_base单元; 这个时间点, 解压缩的数据包将被提交给用户
    // 如果时间不被存储在文件里, 则可以写成AV_NOPTS_VALUE
    // pts必须大于或等于dts, 因为显示不能在解压缩之前被发生, 除非有人想查看十六进制存储。
    // 某些格式误用了这个名词dts或者是pts/cts那是意味着别的意思, 所以时间戳必须在被存储到AVPacket之前转换成真正的PTS/DTS。
    int64_t pts;
 
    // 解码时间戳, 对应时间戳AVStream->time_base单元; 这个时间点, 数据包被解码
    // 如果时间不被存储在文件里, 则可以写成AV_NOPTS_VALUE
    int64_t dts;
 
    // 存储的数据,指向一个缓存,这是AVPacket实际的数据
    uint8_t *data;
 
    // 数据的大小
    int size;
    
    // 标识该AVPacket所属的音频/视频流的索引
    int stream_index;
    
    // 一个AV_PKT_FLAG标识值, 最低为置1表示关键帧
    int flags;
 
    // 容器可以提供的附加数据
    // 包可以包含几种AVPacketSideDataType类型的侧信息
    AVPacketSideData *side_data;
 
    // 附加信息元素
    int side_data_elems;
 
    // 数据的时长,以所属媒体流的时间基准为单位
    int64_t duration;
 
    // 该数据在媒体流中的字节偏移量
    int64_t pos;
 
    // 该字段不再使用
#if FF_API_CONVERGENCE_DURATION
    attribute_deprecated
    int64_t convergence_duration;
#endif
 
} AVPacket;
```

重要的变量有以下几个

uint8_t ***data**：压缩编码的数据。例如对于H.264来说。1个AVPacket的data通常对应一个NAL。注意：在这里只是对应，而不是一模一样。他们之间有微小的差别：使用FFMPEG类库分离出多媒体文件中的H.264码流因此在使用FFMPEG进行视音频处理的时候，常常可以将得到的AVPacket的data数据直接写成文件，从而得到视音频的码流文件。

int  ** size**：data的大小

int64_t **pts**：显示时间戳

int64_t **dts**：解码时间戳

int   **stream_index**：标识该AVPacket所属的视频/音频流。
int  flags:  标识，可籍于此标识判断是否是关键帧(https://www.jianshu.com/p/85398957d726)



参考链接：

1.[AVPacket](https://blog.csdn.net/davidsguo008/article/details/72628675)

2.[FFMPEG结构体分析：AVPacket](https://blog.csdn.net/leixiaohua1020/article/details/14215755)














