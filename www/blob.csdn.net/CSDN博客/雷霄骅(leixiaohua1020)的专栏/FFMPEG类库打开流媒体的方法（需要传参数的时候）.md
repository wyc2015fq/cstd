# FFMPEG类库打开流媒体的方法（需要传参数的时候） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月06日 20:07:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：32
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)










使用ffmpeg类库进行开发的时候，打开流媒体（或本地文件）的函数是avformat_open_input()。

其中打开网络流的话，前面要加上函数avformat_network_init()。

一般情况下，只要传入流媒体的url就可以了。但是在打开某些流媒体的时候，可能需要附加一些参数。

例如在播放中央人民广播电台的声音信号的时候，其url为“rtsp://mms.cnr.cn/cnr003?MzE5MTg0IzEjIzI5NjgwOQ==”

如果直接进行打开是不会成功的，我们可以使用ffplay做一下实验：


`ffplay rtsp://mms.cnr.cn/cnr003?MzE5MTg0IzEjIzI5NjgwOQ==`
会出现错误：



Invalid data found when processing input

这时候我们需要指定其传输方式为TCP，需要将命令改为如下形式：


`ffplay -rtsp_transport tcp rtsp://mms.cnr.cn/cnr003?MzE5MTg0IzEjIzI5NjgwOQ==`
附加了参数以后，发现就可以正常播放了。



此外还可以附加一些参数，比如


`ffplay -rtsp_transport tcp -max_delay 5000000 rtsp://mms.cnr.cn/cnr003?MzE5MTg0IzEjIzI5NjgwOQ==`

 在使用FFMPEG类库进行编程的时候，如何将这些附加的参数传递给avformat_open_input()呢？经过研究后发现，可以通过AVDictionary把参数传给avformat_open_input()。



看一下avformat_open_input()的定义：



```cpp
/**
 * Open an input stream and read the header. The codecs are not opened.
 * The stream must be closed with av_close_input_file().
 *
 * @param ps Pointer to user-supplied AVFormatContext (allocated by avformat_alloc_context).
 *           May be a pointer to NULL, in which case an AVFormatContext is allocated by this
 *           function and written into ps.
 *           Note that a user-supplied AVFormatContext will be freed on failure.
 * @param filename Name of the stream to open.
 * @param fmt If non-NULL, this parameter forces a specific input format.
 *            Otherwise the format is autodetected.
 * @param options  A dictionary filled with AVFormatContext and demuxer-private options.
 *                 On return this parameter will be destroyed and replaced with a dict containing
 *                 options that were not found. May be NULL.
 *
 * @return 0 on success, a negative AVERROR on failure.
 *
 * @note If you want to use custom IO, preallocate the format context and set its pb field.
 */
int avformat_open_input(AVFormatContext **ps, const char *filename, AVInputFormat *fmt, AVDictionary **options);
```


 可以看出avformat_open_input()的第4个参数是一个AVDictionary类型的参数。这个参数就是传入的附加参数。



设置AVDictionary的时候会用到av_dict_set()。

下面看看把命令


`ffplay -rtsp_transport tcp -max_delay 5000000 rtsp://mms.cnr.cn/cnr003?MzE5MTg0IzEjIzI5NjgwOQ==`


转化为代码实现的方式：



```cpp
AVFormatContext	*pFormatCtx;
pFormatCtx = avformat_alloc_context();
...代码略
AVDictionary *avdic=NULL;
char option_key[]="rtsp_transport";
char option_value[]="tcp";
av_dict_set(&avdic,option_key,option_value,0);
char option_key2[]="max_delay";
char option_value2[]="5000000";
av_dict_set(&avdic,option_key2,option_value2,0);
char url[]="rtsp://mms.cnr.cn/cnr003?MzE5MTg0IzEjIzI5NjgwOQ==";

avformat_open_input(&pFormatCtx,url,NULL,&avdic);
```












