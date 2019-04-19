# FFMpeg中apiexample.c例子分析——解码分析 - maopig的专栏 - CSDN博客
2011年06月18日 19:01:00[maopig](https://me.csdn.net/maopig)阅读数：1435标签：[video																[buffer																[codec																[input																[null																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)
# FFMpeg中apiexample.c例子分析——解码分析					收藏
![](http://hi.csdn.net/attachment/201007/1/0_1277953769f73z.gif)
我们直接从video_decode_example()函数开始讲，该函数实现了如何去解码一个视频文件，以.mpeg文
件为例。
　　（1）将缓存的末尾清0，从而确保读操作不会越界导致破坏mpeg流。
uint8_t
inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
memset(inbuf +
INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);
　　　　可见，我们所分配的缓存，除了存放数据外，最后部分还预留了一小段空间。
　　（2）avcodec_find_decoder()函数用于查找与codec ID相匹配的已注册的解码器。
　　（3）avcodec_alloc_context()函数用于分配一个AVCodecContext并设置默认值，
如果失败返回NULL，并可用av_free()进行释放。
　　（4）avcodec_alloc_frame()函数用于分配一个AVFrame并设置默认值，如果失败返回NULL，
并可用av_free()进行释放。
　　（5）avcodec_open()函数用给定的AVCodec来初始化AVCodecContext。
　　　　对于一些编解码器，像msmpeg4和mpeg4，其宽度和高度必须要初始化，因为这些信息在码流（bitstream）中是没有的。avcodec_open()会帮我们把宽度和高度设置好。
　　（6）打开文件fopen。
　　（7）循环解码。
来源：([http://blog.sina.com.cn/s/blog_4e87ac6e0100a60g.html](http://blog.sina.com.cn/s/blog_4e87ac6e0100a60g.html)
)
 - FFMpeg中apiexample.c例子分析——解码分析_eNsEmpire_新浪博客
　　（7.1）读取文件fread(inbuf,
 1, INBUF_SIZE, f );
　　　　注意1：一些编解码器是基于流的（像mpegvideo，mpegaudio），这是使用它们的唯一一种方法，因为在解析它之前，你不知道压缩数据的大小。
　　　　但是，一些其他编解码器（像msmpeg4，mpeg4）是基于帧的，所以对于某一帧，你必须调用这些编解码器来处理所有数据。你也必须在初始化这些编解
码器前，先把宽度和高度设置了。
　　　　注意2：一些编解码器允许原始参数（像帧大小，采样率）在任何一帧被改变。我们虽然已对此做了处理，但你也
要重视起来。
　　　　这儿，我们用一个基于流的解码器（mpeg1video），所以我们把数据输到解码器，看它是否可以解码一帧。
　　（7.2）avcodec_decode_video()函数用于解码一个视频帧,从inbuf_ptr到picture,所采用的编解码器是c。
　　（7.3）写入文件。
　　（8）解码延时的帧，再写入文件。
　　　　一些解码器，像MPEG，传输I帧和P帧时，都会有一帧的延时。所以我们必须要获取视频的最后一帧数据。
　　（9）释放资源。
fclose(f);
avcodec_close(c);
av_free(c);
av_free(picture);
