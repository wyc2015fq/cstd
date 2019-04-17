# ffmpeg avpicture_fill的一些使用 - DoubleLi - 博客园









标签： [ffmpeg](http://www.csdn.net/tag/ffmpeg)[avpicture_fill](http://www.csdn.net/tag/avpicture_fill)

2013-05-17 10:03 4713人阅读 [评论](http://blog.csdn.net/u010144106/article/details/8938334#comments)(1) 收藏[举报](http://blog.csdn.net/u010144106/article/details/8938334#report)



![](http://static.blog.csdn.net/images/category_icon.jpg) 分类：

ffmpeg（3） ![](http://static.blog.csdn.net/images/arrow_triangle%20_down.jpg)


这个FFMPEG我没找到详细的中文教程，只有将自己的使用心得记录。

1 int avpicture_fill(AVPicture *picture, uint8_t *ptr,

                   int pix_fmt, int width, int height);

这个函数的使用本质上是为已经分配的空间的结构体AVPicture挂上一段用于保存数据的空间，这个结构体中有一个指针数组data[4]，挂在这个数组里。一般我们这么使用：

1） pFrameRGB=avcodec_alloc_frame();


2） numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);

    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

3） avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,pCodecCtx->width, pCodecCtx-         >height);


以上就是为pFrameRGB挂上buffer。这个buffer是用于存缓冲数据的。

好，现在让我们来看一下tutorials里常出现的pFrame为什么不用fill空间。主要是下面这句：

avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,packet.data, packet.size);

这里根据我的瞎猜，很可能是pFrame已经挂上了packet.data，所以就不用fill了。

2 现在开始谈ffmpeg里的一些设备上下文，关于设备上下文，回顾一下它的定义：

Windows操作系统统一管理着诸如显示，打印等操作，将它们看作是一个个的设备，每一个设备都有一个复杂的数据结构来维护。所谓设备上下文就是指这个数据结构。然而，我们不能直接和这些设备上下文打交道，只能通过引用标识它的句柄(实际上是一个整数)，让Windows去做相应的处理。



ffmpeg的两个设备上下文AVFormatContext和AVCodecContext，实际上是可以直接对其进行操作的。

AVFormatContext是FFMpeg格式转换过程中实现输入和输出功能、保存相关数据的主要结构。

AVCodecContext保存AVCodec指针和与codec相关的数据，如video的width、height，audio的sample rate等。

这里一些重要的数据结构可以参看：

[http://www.sudu.cn/info/html/edu/20070101/287256.html](http://www.sudu.cn/info/html/edu/20070101/287256.html)

实际上AVCodecContext里的stream结构体中包含了一个AVCodecContext。所以码流对应的codec可以从这里取出。



3 用SDL_SetVideoMode创建一个且仅有一个的screen surface，相当于屏幕了，所有东西都往上画



4 struct SwsContext *sws_getContext

(int srcW, int srcH, int srcFormat, int dstW, int dstH, int dstFormat, int 

flags,SwsFilter *srcFilter, SwsFilter *dstFilter, double *param);

获取用于转换在上下文，这里转换的概念包括视频的大小，像素格式。但这里只是获取了DC未开始转换



5 int sws_scale

(struct SwsContext *context, uint8_t* src[], int srcStride[], int srcSliceY,

 int srcSliceH, uint8_t* dst[], int dstStride[]);

开始向目标转换，转换后可以用于显示，但转换之前必须指定dst的地址和行数，这样才知道数据以何种方式copy到dst中去。



6 关于声音，由于计算机方面知识比较单薄，故复习了一下队列多线程互斥体条件变量等概念，耽误了不少时间。

1）首先要给wanted_spec设置一堆声音播放的参数，其中最重要的就是控制播放速度的采样率和回调函数，接着fill给spec，这个spec猜测就是声卡。

2）初始化队列，开始播放声音同时开启callback。这些都是关于声音解码器的工作，在读入文件info后做完。 

3）接下来是读码流了，若是声音的码流，则执行packet_queue_put(&audioq, &packet)，将直接将包放入队   列，同时发送条件变量通知packet_queue_get已经有数据了

4）现在来分析一直在进行中的回调函数audio_callback，这里面会调用audio_decode_frame，进入后是一个无限循环，顺序应是先执行packet_queue_get，待得到数据后再执行avcodec_decode_audio2解码

5）在packet_queue_get中将先上锁，等待packet_queue_put的通知（其间将解锁，睡眠，得到通知再上锁），后再解锁，上锁解锁是为了防止put和get同时操作队列










