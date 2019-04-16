# FFmpeg(3)-AVFormatContext 结构体内容分析 - 夜行过客 - 博客园







# [FFmpeg(3)-AVFormatContext 结构体内容分析](https://www.cnblogs.com/yongdaimi/p/9796799.html)





AVIOContext *pb                          IO Context,。自定义一些读写格式或者从内存当中读时用到此成员变量。

char filename[1024];                   文件名。它会把打开的文件名存下来(经常会用到断开重连的操作)

unsigned int nb_streams            流的数量。

AVStream **streams                   流。一般存放的是音频或者是视频数据的信息，正常情况下0是视频下标，1是音频下标；

int64_t duration                          总长度。表示我们整个媒体文件的长度，以AV_TIME_BASE这个时间基数为单位，1000000，实际上它是一个微秒值，可以除以1000得到毫秒值，除以1000000得到秒值。要注意的是，这个值可能无法获取到。

int64 bit_rate                              比特率

void avformat_close_input(AVFormatContext **s) 关闭输入的上下文














