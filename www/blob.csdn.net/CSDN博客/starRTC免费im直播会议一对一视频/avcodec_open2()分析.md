# avcodec_open2()分析 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月30日 10:16:51[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：40
该函数用于初始化一个视音频编解码器的AVCodecContext。
int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
各个参数的含义：
avctx：需要初始化的AVCodecContext。
codec：输入的AVCodec
options：一些选项。例如使用libx264编码的时候，“preset”，“tune”等都可以通过该参数设置。
函数调用关系图
avcodec_open2()
avcodec_open2()的源代码量是非常长的，但是它的调用关系非常简单——它只调用了一个关键的函数，即AVCodec的init()，后文将会对这个函数进行分析。
我们可以简单梳理一下avcodec_open2()所做的工作，如下所列：
（1）为各种结构体分配内存（通过各种av_malloc()实现）。
（2）将输入的AVDictionary形式的选项设置到AVCodecContext。
（3）其他一些零零碎碎的检查，比如说检查编解码器是否处于“实验”阶段。
（4）如果是编码器，检查输入参数是否符合编码器的要求
（5）调用AVCodec的init()初始化具体的解码器。
我们分析一下第4步和第5步。
检查输入参数是否符合编码器要求
在这里简单分析一下第4步，即“检查输入参数是否符合编码器的要求”。这一步中检查了很多的参数，在这里我们随便选一个参数pix_fmts（像素格式）看一下，如下所示。
- //检查像素格式
- if (avctx->codec->pix_fmts) {
- for (i = 0; avctx->codec->pix_fmts[i] != AV_PIX_FMT_NONE; i++)
- if (avctx->pix_fmt == avctx->codec->pix_fmts[i])
- break;
- if (avctx->codec->pix_fmts[i] == AV_PIX_FMT_NONE
- && !((avctx->codec_id == AV_CODEC_ID_MJPEG || avctx->codec_id == AV_CODEC_ID_LJPEG)
- && avctx->strict_std_compliance <= FF_COMPLIANCE_UNOFFICIAL)) {
- char buf[128];
- snprintf(buf, sizeof(buf), "%d", avctx->pix_fmt);
- av_log(avctx, AV_LOG_ERROR, "Specified pixel format %s is invalid or not supported\n",
- (char *)av_x_if_null(av_get_pix_fmt_name(avctx->pix_fmt), buf));
- ret = AVERROR(EINVAL);
- goto free_and_end;
- }
- if (avctx->codec->pix_fmts[i] == AV_PIX_FMT_YUVJ420P ||
- avctx->codec->pix_fmts[i] == AV_PIX_FMT_YUVJ411P ||
- avctx->codec->pix_fmts[i] == AV_PIX_FMT_YUVJ422P ||
- avctx->codec->pix_fmts[i] == AV_PIX_FMT_YUVJ440P ||
- avctx->codec->pix_fmts[i] == AV_PIX_FMT_YUVJ444P)
- avctx->color_range = AVCOL_RANGE_JPEG;
- }
可以看出，该代码首先进入了一个for()循环，将AVCodecContext中设定的pix_fmt与编码器AVCodec中的pix_fmts数组中的元素逐一比较。
先简单介绍一下AVCodec中的pix_fmts数组。AVCodec中的pix_fmts数组存储了该种编码器支持的像素格式，并且规定以AV_PIX_FMT_NONE（AV_PIX_FMT_NONE取值为-1）为结尾。例如，libx264的pix_fmts数组的定义位于libavcodec\libx264.c，如下所示。
- staticconstenum AVPixelFormat pix_fmts_8bit[] = {
- AV_PIX_FMT_YUV420P,
- AV_PIX_FMT_YUVJ420P,
- AV_PIX_FMT_YUV422P,
- AV_PIX_FMT_YUVJ422P,
- AV_PIX_FMT_YUV444P,
- AV_PIX_FMT_YUVJ444P,
- AV_PIX_FMT_NV12,
- AV_PIX_FMT_NV16,
- AV_PIX_FMT_NONE
- };
从pix_fmts_8bit的定义可以看出libx264主要支持的是以YUV为主的像素格式。
现在回到“检查输入pix_fmt是否符合编码器的要求”的那段代码。如果for()循环从AVCodec->pix_fmts数组中找到了符合AVCodecContext->pix_fmt的像素格式，或者完成了AVCodec->pix_fmts数组的遍历，都会跳出循环。如果发现AVCodec->pix_fmts数组中索引为i的元素是AV_PIX_FMT_NONE（即最后一个元素，取值为-1）的时候，就认为没有找到合适的像素格式，并且最终提示错误信息。
AVCodec->init()
avcodec_open2()中最关键的一步就是调用AVCodec的init()方法初始化具体的编码器。AVCodec的init()是一个函数指针，指向具体编解码器中的初始化函数。这里我们以libx264为例，看一下它对应的AVCodec的定义。libx264对应的AVCodec的定义位于libavcodec\libx264.c，如下所示。
- AVCodec ff_libx264_encoder = {
- .name = "libx264",
- .long_name = NULL_IF_CONFIG_SMALL("libx264 H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
- .type = AVMEDIA_TYPE_VIDEO,
- .id = AV_CODEC_ID_H264,
- .priv_data_size = sizeof(X264Context),
- .init = X264_init,
- .encode2 = X264_frame,
- .close = X264_close,
- .capabilities = CODEC_CAP_DELAY | CODEC_CAP_AUTO_THREADS,
- .priv_class = &x264_class,
- .defaults = x264_defaults,
- .init_static_data = X264_init_static,
- };
可以看出在ff_libx264_encoder中init()指向X264_init()。X264_init()的定义同样位于libavcodec\libx264.c，
X264_init()的代码以后研究X264的时候再进行细节的分析，在这里简单记录一下它做的两项工作：
（1）设置X264Context的参数。X264Context主要完成了libx264和FFmpeg对接的功能。可以看出代码主要在设置一个params结构体变量，该变量的类型即是x264中存储参数的结构体x264_param_t。
（2）调用libx264的API进行编码器的初始化工作。例如调用x264_param_default()设置默认参数，调用x264_param_apply_profile()设置profile，调用x264_encoder_open()打开编码器等等。
X264Context的定义，位于libavcodec\libx264.c，
