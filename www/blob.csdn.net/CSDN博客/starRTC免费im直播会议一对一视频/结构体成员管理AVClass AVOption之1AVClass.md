# 结构体成员管理AVClass AVOption之1AVClass - starRTC免费im直播会议一对一视频 - CSDN博客
2017年09月05日 14:32:40[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：487
AVOption用于描述结构体中的成员变量。它最主要的作用可以概括为两个字：“赋值”。
一个AVOption结构体包含了变量名称，简短的帮助，取值等信息。
所有和AVOption有关的数据都存储在AVClass结构体中。如果一个结构体（例如AVFormatContext或者AVCodecContext）想要支持AVOption的话，它的第一个成员变量必须是一个指向AVClass结构体的指针。该AVClass中的成员变量option必须指向一个AVOption类型的静态数组。
## 何为AVOption？
AVOption是用来设置FFmpeg中变量值的结构体。特点就在于它赋值的灵活性。AVOption可以使用字符串为任何类型的变量赋值。统一使用字符串赋值。例如给int型变量qp设定值为20，通过AVOption需要传递进去一个内容为“20”的字符串。
此外，AVOption中变量的名称也使用字符串来表示。传递两个字符串（一个是变量的名称，一个是变量的值）就可以改变系统中变量的值。
对于从外部系统中调用FFmpeg的人来说，作用就很大了：从外部系统中只可以传递字符串给内部系统。比如说对于直接调用ffmpeg.exe的人来说，他们是无法修改FFmpeg内部各个变量的数值的，这种情况下只能通过输入“名称”和“值”这样的字符串，通过AVOption改变FFmpeg内部变量的值。由此可见，使用AVOption可以使FFmpeg更加适应多种多样的外部系统。如*互联网上只可以传输字符串。*
其实除了可以对FFmpeg常用结构体AVFormatContext，AVCodecContext等进行赋值之外，还可以对它们的私有数据priv_data进行赋值。例如使用libx264进行编码的时候，通过AVCodecContext的priv_data字段可以对X264Context结构体中的变量进行赋值，设置preset，profile等。使用libx265进行编码的时候，通过AVCodecContext的priv_data字段可以对libx265Context结构体中的变量进行赋值，设置preset，tune等。
## 何为AVClass？
AVClass最主要的作用就是给结构体（例如AVFormatContext等）增加AVOption功能的支持。AVClass就是AVOption和目标结构体之间的“桥梁”。AVClass要求必须声明为目标结构体的第一个变量。
AVClass中有一个option数组用于存储目标结构体的所有的AVOption。举个例子，AVFormatContext结构体，AVClass和AVOption之间的关系如下图所示。
![](file:///D:/temp/soft_data/Ynote/elesos@163.com/f8a3d2f8675c4b2e9f7f60e97bf873f1/20150315010318675.png)
图中AVFormatContext结构体的第一个变量为AVClass类型的指针av_class，它在AVFormatContext结构体初始化的时候，被赋值指向了全局静态变量av_format_context_class结构体（定义位于libavformat\options.c）。而AVClass类型的av_format_context_class结构体中的option变量指向了全局静态数组avformat_options（定义位于libavformat\options_table.h）。
static const AVClass av_format_context_class = {
    .class_name     = "AVFormatContext",
    .item_name      = format_to_name,
    .option         = avformat_options,
    .version        = LIBAVUTIL_VERSION_INT,
    .child_next     = format_child_next,
    .child_class_next = format_child_class_next,
    .category       = AV_CLASS_CATEGORY_MUXER,
    .get_category   = get_category,
};
static const AVOption avformat_options[] = {
{"avioflags", NULL, OFFSET(avio_flags), AV_OPT_TYPE_FLAGS, {.i64 = DEFAULT }, INT_MIN, INT_MAX, D|E, "avioflags"},
{"direct", "reduce buffering", 0, AV_OPT_TYPE_CONST, {.i64 = AVIO_FLAG_DIRECT }, INT_MIN, INT_MAX, D|E, "avioflags"},
{"probesize", "set probing size", OFFSET(probesize), AV_OPT_TYPE_INT64, {.i64 = 5000000 }, 32, INT64_MAX, D},
## AVOption
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/2420a9ac20ca4f139b5f78bd6324004f/code_ico.png)
- /**
-  * AVOption
-  */
- **typedef****struct** AVOption {  
- **const****char** *name;  名称。
- 
- /**
-      * short English help text
-      * @todo What about other languages?
-      */
- **const****char** *help;  简短的帮助。
- 
- /**
-      * The offset relative to the context structure where the option
-      * value is stored. It should be 0 for named constants.
-      */
- **int** offset;  选项相对结构体首部地址的偏移量（这个很重要）。
- **enum** AVOptionType type;  选项的类型。
- 
- /**
-      * the default value for scalar options
-      */
- **union** {  
-         int64_t i64;  
- **double** dbl;  
- **const****char** *str;  
- /* TODO those are unused now */
-         AVRational q;  
-     } default_val;  选项的默认值。
- **double** min;                 ///< minimum valid value for the option选项的最小值。
- **double** max;                 ///< maximum valid value for the option选项的最大值。
- 
- **int** flags;  一些标记。
- 
- /**
-      * The logical unit to which the option belongs. Non-constant
-      * options and corresponding named constants share the same
-      * unit. May be NULL.
-      */
- **const****char** *unit;  该选项所属的逻辑单元，可以为空。
- } AVOption;  
> 其中，default_val是一个union类型的变量，可以根据选项数据类型的不同，取int，double，char*，AVRational（表示分数）几种类型。
> 
## AVClass
AVClass中存储了AVOption类型的数组option，用于存储选项信息。AVClass有一个特点就是它必须位于其支持的结构体的第一个位置。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/04ba1061601e4457897eda825a7cdec2/code_ico.png)
- /**
-  * Describe the class of an AVClass context structure. That is an
-  * arbitrary struct of which the first field is a pointer to an
-  * AVClass struct (e.g. AVCodecContext, AVFormatContext etc.).
-  */
- **typedef****struct** AVClass {  
- /**
-      * The name of the class; usually it is the same name as the
-      * context structure type to which the AVClass is associated.
-      */
- **const****char*** class_name;  AVClass名称。
- 
- /**
-      * A pointer to a function which returns the name of a context
-      * instance ctx associated with the class.
-      */
- **const****char*** (*item_name)(**void*** ctx);函数,获取与AVClass相关联的结构体实例的名称。
- 
- /**
-      * a pointer to the first option specified in the class if any or NULL
-      *
-      * @see av_set_default_options()
-      */
- **const****struct** AVOption *option;  AVOption类型的数组（最重要）。
- 
- /**
-      * LIBAVUTIL_VERSION with which this structure was created.
-      * This is used to allow fields to be added without requiring major
-      * version bumps everywhere.
-      */
- 
- **int** version;  完成该AVClass的时候的LIBAVUTIL_VERSION。
- 
- /**
-      * Offset in the structure where log_level_offset is stored.
-      * 0 means there is no such variable
-      */
- **int** log_level_offset_offset;  
- 
- /**
-      * Offset in the structure where a pointer to the parent context for
-      * logging is stored. For example a decoder could pass its AVCodecContext
-      * to eval as such a parent context, which an av_log() implementation
-      * could then leverage to display the parent context.
-      * The offset can be NULL.
-      */
- **int** parent_log_context_offset;  
- 
- /**
-      * Return next AVOptions-enabled child or NULL
-      */
- **void*** (*child_next)(**void** *obj, **void** *prev);  
- 
- /**
-      * Return an AVClass corresponding to the next potential
-      * AVOptions-enabled child.
-      *
-      * The difference between child_next and this is that
-      * child_next iterates over _already existing_ objects, while
-      * child_class_next iterates over _all possible_ children.
-      */
- **const****struct** AVClass* (*child_class_next)(**const****struct** AVClass *prev);  
- 
- /**
-      * Category used for visualization (like color)
-      * This is only set if the category is equal for all objects using this class.
-      * available since version (51 << 16 | 56 << 8 | 100)
-      */
-     AVClassCategory category;  AVClass的类型，是一个类型为AVClassCategory的枚举型变量。
- 
- /**
-      * Callback to return the category.
-      * available since version (51 << 16 | 59 << 8 | 100)
-      */
-     AVClassCategory (*get_category)(**void*** ctx);  
- 
- /**
-      * Callback to return the supported/allowed ranges.
-      * available since version (52.12)
-      */
- **int** (*query_ranges)(**struct** AVOptionRanges **, **void** *obj, **const****char** *key, **int** flags);  
- } AVClass;  
下面通过具体的例子看一下AVClass这个结构体。我们看几个具体的例子：
- AVFormatContext中的AVClass
- AVCodecContext中的AVClass
- AVFrame中的AVClass
- 各种组件（libRTMP，libx264，libx265）里面特有的AVClass。
## AVFormatContext
AVFormatContext 中的AVClass定义位于libavformat\options.c中，是一个名称为av_format_context_class的静态结构体。如下所示。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/2814d9e01c6a4d6686dddf2fd5fc3146/code_ico.png)
- **static****const** AVClass av_format_context_class = {  
-     .class_name     = "AVFormatContext",  
-     .item_name      = format_to_name,  
-     .option         = avformat_options,  
-     .version        = LIBAVUTIL_VERSION_INT,  
-     .child_next     = format_child_next,  
-     .child_class_next = format_child_class_next,  
-     .category       = AV_CLASS_CATEGORY_MUXER,  
-     .get_category   = get_category,  
- };  
从源代码可以看出以下几点
（1）class_name:    该AVClass名称是“AVFormatContext”。
（2）item_name
item_name指向一个函数format_to_name()，该函数定义如下所示。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/4882ca28665f480c908b243712f6fbf3/code_ico.png)
- **static****const****char*** format_to_name(**void*** ptr)  
- {  
-     AVFormatContext* fc = (AVFormatContext*) ptr;  
- **if**(fc->iformat) **return** fc->iformat->name;  
- **else****if**(fc->oformat) **return** fc->oformat->name;  
- **else****return**"NULL";  
- }  
从函数的定义可以看出，如果AVFormatContext结构体中的AVInputFormat结构体不为空，则返回AVInputFormat的name，然后尝试返回AVOutputFormat的name，如果AVOutputFormat也为空，则返回“NULL”。
（3）option
option字段则指向一个元素个数很多的静态数组avformat_options。该数组单独定义于libavformat\options_table.h中。其中包含了AVFormatContext支持的所有的AVOption
## AVCodecContext
位于libavcodec\options.c中，是一个名称为av_codec_context_class的静态结构体。如下所示。

- **static****const** AVClass av_codec_context_class = {  
-     .class_name              = "AVCodecContext",  
-     .item_name               = context_to_name,  
-     .option                  = avcodec_options,  
-     .version                 = LIBAVUTIL_VERSION_INT,  
-     .log_level_offset_offset = offsetof(AVCodecContext, log_level_offset),  
-     .child_next              = codec_child_next,  
-     .child_class_next        = codec_child_class_next,  
-     .category                = AV_CLASS_CATEGORY_ENCODER,  
-     .get_category            = get_category,  
- };  
（1）class_name:该AVClass名称是“AVCodecContext”。
（2）item_name
item_name指向一个函数context_to_name ()，该函数定义如下所示。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/92ead9fc51f1469088683c0ee86c5356/code_ico.png)
- **static****const****char*** context_to_name(**void*** ptr) {  
-     AVCodecContext *avc= ptr;  
- 
- **if**(avc && avc->codec && avc->codec->name)  
- **return** avc->codec->name;  
- **else**
- **return**"NULL";  
- }  
从函数的定义可以看出，如果AVCodecContext中的Codec结构体不为空，则返回Codec（AVCodec类型的）的name，否则返回“NULL”。
（3）option
option字段则指向一个元素个数极多的静态数组avcodec_options。该数组单独定义于libavcodec\options_table.h中。其中包含了AVCodecContext支持的所有的AVOption
注：编码参数设置估计就是在这里
## AVFrame
位于libavcodec\options.c中，是一个名称为av_frame_class的静态结构体。如下所示。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/d708adbb518f47ada2e2b1441d9aea60/code_ico.png)
- **static****const** AVClass av_frame_class = {  
-     .class_name              = "AVFrame",  
-     .item_name               = NULL,  
-     .option                  = frame_options,  
-     .version                 = LIBAVUTIL_VERSION_INT,  
- };  
option字段则指向静态数组frame_options。frame_options定义如下所示。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/fa8c48a83c6e4cb29a6547918f42e6bd/code_ico.png)
- **static****const** AVOption frame_options[]={  
- {"best_effort_timestamp", "", FOFFSET(best_effort_timestamp), AV_OPT_TYPE_INT64, {.i64 = AV_NOPTS_VALUE }, INT64_MIN, INT64_MAX, 0},  
- {"pkt_pos", "", FOFFSET(pkt_pos), AV_OPT_TYPE_INT64, {.i64 = -1 }, INT64_MIN, INT64_MAX, 0},  
- {"pkt_size", "", FOFFSET(pkt_size), AV_OPT_TYPE_INT64, {.i64 = -1 }, INT64_MIN, INT64_MAX, 0},  
- {"sample_aspect_ratio", "", FOFFSET(sample_aspect_ratio), AV_OPT_TYPE_RATIONAL, {.dbl = 0 }, 0, INT_MAX, 0},  
- {"width", "", FOFFSET(width), AV_OPT_TYPE_INT, {.i64 = 0 }, 0, INT_MAX, 0},  
- {"height", "", FOFFSET(height), AV_OPT_TYPE_INT, {.i64 = 0 }, 0, INT_MAX, 0},  
- {"format", "", FOFFSET(format), AV_OPT_TYPE_INT, {.i64 = -1 }, 0, INT_MAX, 0},  
- {"channel_layout", "", FOFFSET(channel_layout), AV_OPT_TYPE_INT64, {.i64 = 0 }, 0, INT64_MAX, 0},  
- {"sample_rate", "", FOFFSET(sample_rate), AV_OPT_TYPE_INT, {.i64 = 0 }, 0, INT_MAX, 0},  
- {NULL},  
- };  
可以看出AVFrame的选项数组中包含了“width”，“height”这类用于视频帧的选项，以及“channel_layout”，“sample_rate”这类用于音频帧的选项。
## 各种组件特有的AVClass
除了FFmpeg中通用的AVFormatContext，AVCodecContext，AVFrame这类的结构体之外，每种特定的组件也包含自己的AVClass。
### LibRTMP
libRTMP中根据协议类型的不同定义了多种的AVClass。由于这些AVClass除了名字不一样之外，其他的字段一模一样，所以AVClass的声明写成了一个名为RTMP_CLASS的宏。
- #define RTMP_CLASS(flavor)\
- **static****const** AVClass lib ## flavor ## _class = {\  
-     .class_name = "lib" #flavor " protocol",\  
-     .item_name  = av_default_item_name,\  
-     .option     = options,\  
-     .version    = LIBAVUTIL_VERSION_INT,\  
- };  
而后定义了多种AVCLass：
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/3b4ed46500614b1ca6a5250c91cc7d9b/code_ico.png)
- RTMP_CLASS(rtmp)  
- RTMP_CLASS(rtmpt)  
- RTMP_CLASS(rtmpe)  
- RTMP_CLASS(rtmpte)  
- RTMP_CLASS(rtmps)  
这些AVClass的option字段指向的数组是一样的，如下所示。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/dc0ae840ef044a009594e7eec69c42b7/code_ico.png)
- **static****const** AVOption options[] = {  
-     {"rtmp_app", "Name of application to connect to on the RTMP server", OFFSET(app), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC|ENC},  
-     {"rtmp_buffer", "Set buffer time in milliseconds. The default is 3000.", OFFSET(client_buffer_time), AV_OPT_TYPE_STRING, {.str = "3000"}, 0, 0, DEC|ENC},  
-     {"rtmp_conn", "Append arbitrary AMF data to the Connect message", OFFSET(conn), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC|ENC},  
-     {"rtmp_flashver", "Version of the Flash plugin used to run the SWF player.", OFFSET(flashver), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC|ENC},  
-     {"rtmp_live", "Specify that the media is a live stream.", OFFSET(live), AV_OPT_TYPE_INT, {.i64 = 0}, INT_MIN, INT_MAX, DEC, "rtmp_live"},  
-     {"any", "both", 0, AV_OPT_TYPE_CONST, {.i64 = -2}, 0, 0, DEC, "rtmp_live"},  
-     {"live", "live stream", 0, AV_OPT_TYPE_CONST, {.i64 = -1}, 0, 0, DEC, "rtmp_live"},  
-     {"recorded", "recorded stream", 0, AV_OPT_TYPE_CONST, {.i64 = 0}, 0, 0, DEC, "rtmp_live"},  
-     {"rtmp_pageurl", "URL of the web page in which the media was embedded. By default no value will be sent.", OFFSET(pageurl), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC},  
-     {"rtmp_playpath", "Stream identifier to play or to publish", OFFSET(playpath), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC|ENC},  
-     {"rtmp_subscribe", "Name of live stream to subscribe to. Defaults to rtmp_playpath.", OFFSET(subscribe), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC},  
-     {"rtmp_swfurl", "URL of the SWF player. By default no value will be sent", OFFSET(swfurl), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC|ENC},  
-     {"rtmp_swfverify", "URL to player swf file, compute hash/size automatically. (unimplemented)", OFFSET(swfverify), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC},  
-     {"rtmp_tcurl", "URL of the target stream. Defaults to proto://host[:port]/app.", OFFSET(tcurl), AV_OPT_TYPE_STRING, {.str = NULL }, 0, 0, DEC|ENC},  
-     { NULL },  
- };  
### Libx264
Libx264的AVClass定义如下所示。

- **static****const** AVClass x264_class = {  
-     .class_name = "libx264",  
-     .item_name  = av_default_item_name,  
-     .option     = options,  
-     .version    = LIBAVUTIL_VERSION_INT,  
- };  
其中option字段指向的数组定义如下所示。这些option的使用频率还是比较高的。
注：x264编码参数应该通过这里设置吧？
[
- **static****const** AVOption options[] = {  
-     { "preset",        "Set the encoding preset (cf. x264 --fullhelp)",   OFFSET(preset),        AV_OPT_TYPE_STRING, { .str = "medium" }, 0, 0, VE},  
-     { "tune",          "Tune the encoding params (cf. x264 --fullhelp)",  OFFSET(tune),          AV_OPT_TYPE_STRING, { 0 }, 0, 0, VE},  
-     { "profile",       "Set profile restrictions (cf. x264 --fullhelp) ", OFFSET(profile),       AV_OPT_TYPE_STRING, { 0 }, 0, 0, VE},  
-     { "fastfirstpass", "Use fast settings when encoding first pass",      OFFSET(fastfirstpass), AV_OPT_TYPE_INT,    { .i64 = 1 }, 0, 1, VE},  
-     {"level", "Specify level (as defined by Annex A)", OFFSET(level), AV_OPT_TYPE_STRING, {.str=NULL}, 0, 0, VE},  
-     {"passlogfile", "Filename for 2 pass stats", OFFSET(stats), AV_OPT_TYPE_STRING, {.str=NULL}, 0, 0, VE},  
-     {"wpredp", "Weighted prediction for P-frames", OFFSET(wpredp), AV_OPT_TYPE_STRING, {.str=NULL}, 0, 0, VE},  
-     {"x264opts", "x264 options", OFFSET(x264opts), AV_OPT_TYPE_STRING, {.str=NULL}, 0, 0, VE},  
-     { "crf",           "Select the quality for constant quality mode",    OFFSET(crf),           AV_OPT_TYPE_FLOAT,  {.dbl = -1 }, -1, FLT_MAX, VE },  
-     { "crf_max",       "In CRF mode, prevents VBV from lowering quality beyond this point.",OFFSET(crf_max), AV_OPT_TYPE_FLOAT, {.dbl = -1 }, -1, FLT_MAX, VE },  
-     { "qp",            "Constant quantization parameter rate control method",OFFSET(cqp),        AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, INT_MAX, VE },  
-     { "aq-mode",       "AQ method",                                       OFFSET(aq_mode),       AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, INT_MAX, VE, "aq_mode"},  
-     { "none",          NULL,                              0, AV_OPT_TYPE_CONST, {.i64 = X264_AQ_NONE},         INT_MIN, INT_MAX, VE, "aq_mode" },  
-     { "variance",      "Variance AQ (complexity mask)",   0, AV_OPT_TYPE_CONST, {.i64 = X264_AQ_VARIANCE},     INT_MIN, INT_MAX, VE, "aq_mode" },  
-     { "autovariance",  "Auto-variance AQ (experimental)", 0, AV_OPT_TYPE_CONST, {.i64 = X264_AQ_AUTOVARIANCE}, INT_MIN, INT_MAX, VE, "aq_mode" },  
-     { "aq-strength",   "AQ strength. Reduces blocking and blurring in flat and textured areas.", OFFSET(aq_strength), AV_OPT_TYPE_FLOAT, {.dbl = -1}, -1, FLT_MAX, VE},  
-     { "psy",           "Use psychovisual optimizations.",                 OFFSET(psy),           AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE },  
-     { "psy-rd",        "Strength of psychovisual optimization, in <psy-rd>:<psy-trellis> format.", OFFSET(psy_rd), AV_OPT_TYPE_STRING,  {0 }, 0, 0, VE},  
-     { "rc-lookahead",  "Number of frames to look ahead for frametype and ratecontrol", OFFSET(rc_lookahead), AV_OPT_TYPE_INT, { .i64 = -1 }, -1, INT_MAX, VE },  
-     { "weightb",       "Weighted prediction for B-frames.",               OFFSET(weightb),       AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE },  
-     { "weightp",       "Weighted prediction analysis method.",            OFFSET(weightp),       AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, INT_MAX, VE, "weightp" },  
-     { "none",          NULL, 0, AV_OPT_TYPE_CONST, {.i64 = X264_WEIGHTP_NONE},   INT_MIN, INT_MAX, VE, "weightp" },  
-     { "simple",        NULL, 0, AV_OPT_TYPE_CONST, {.i64 = X264_WEIGHTP_SIMPLE}, INT_MIN, INT_MAX, VE, "weightp" },  
-     { "smart",         NULL, 0, AV_OPT_TYPE_CONST, {.i64 = X264_WEIGHTP_SMART},  INT_MIN, INT_MAX, VE, "weightp" },  
-     { "ssim",          "Calculate and print SSIM stats.",                 OFFSET(ssim),          AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE },  
-     { "intra-refresh", "Use Periodic Intra Refresh instead of IDR frames.",OFFSET(intra_refresh),AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE },  
-     { "bluray-compat", "Bluray compatibility workarounds.",               OFFSET(bluray_compat) ,AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE },  
-     { "b-bias",        "Influences how often B-frames are used",          OFFSET(b_bias),        AV_OPT_TYPE_INT,    { .i64 = INT_MIN}, INT_MIN, INT_MAX, VE },  
-     { "b-pyramid",     "Keep some B-frames as references.",               OFFSET(b_pyramid),     AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, INT_MAX, VE, "b_pyramid" },  
-     { "none",          NULL,                                  0, AV_OPT_TYPE_CONST, {.i64 = X264_B_PYRAMID_NONE},   INT_MIN, INT_MAX, VE, "b_pyramid" },  
-     { "strict",        "Strictly hierarchical pyramid",       0, AV_OPT_TYPE_CONST, {.i64 = X264_B_PYRAMID_STRICT}, INT_MIN, INT_MAX, VE, "b_pyramid" },  
-     { "normal",        "Non-strict (not Blu-ray compatible)", 0, AV_OPT_TYPE_CONST, {.i64 = X264_B_PYRAMID_NORMAL}, INT_MIN, INT_MAX, VE, "b_pyramid" },  
-     { "mixed-refs",    "One reference per partition, as opposed to one reference per macroblock", OFFSET(mixed_refs), AV_OPT_TYPE_INT, { .i64 = -1}, -1, 1, VE },  
-     { "8x8dct",        "High profile 8x8 transform.",                     OFFSET(dct8x8),        AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE},  
-     { "fast-pskip",    NULL,                                              OFFSET(fast_pskip),    AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE},  
-     { "aud",           "Use access unit delimiters.",                     OFFSET(aud),           AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE},  
-     { "mbtree",        "Use macroblock tree ratecontrol.",                OFFSET(mbtree),        AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, 1, VE},  
-     { "deblock",       "Loop filter parameters, in <alpha:beta> form.",   OFFSET(deblock),       AV_OPT_TYPE_STRING, { 0 },  0, 0, VE},  
-     { "cplxblur",      "Reduce fluctuations in QP (before curve compression)", OFFSET(cplxblur), AV_OPT_TYPE_FLOAT,  {.dbl = -1 }, -1, FLT_MAX, VE},  
-     { "partitions",    "A comma-separated list of partitions to consider. "
- "Possible values: p8x8, p4x4, b8x8, i8x8, i4x4, none, all", OFFSET(partitions), AV_OPT_TYPE_STRING, { 0 }, 0, 0, VE},  
-     { "direct-pred",   "Direct MV prediction mode",                       OFFSET(direct_pred),   AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, INT_MAX, VE, "direct-pred" },  
-     { "none",          NULL,      0,    AV_OPT_TYPE_CONST, { .i64 = X264_DIRECT_PRED_NONE },     0, 0, VE, "direct-pred" },  
-     { "spatial",       NULL,      0,    AV_OPT_TYPE_CONST, { .i64 = X264_DIRECT_PRED_SPATIAL },  0, 0, VE, "direct-pred" },  
-     { "temporal",      NULL,      0,    AV_OPT_TYPE_CONST, { .i64 = X264_DIRECT_PRED_TEMPORAL }, 0, 0, VE, "direct-pred" },  
-     { "auto",          NULL,      0,    AV_OPT_TYPE_CONST, { .i64 = X264_DIRECT_PRED_AUTO },     0, 0, VE, "direct-pred" },  
-     { "slice-max-size","Limit the size of each slice in bytes",           OFFSET(slice_max_size),AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, INT_MAX, VE },  
-     { "stats",         "Filename for 2 pass stats",                       OFFSET(stats),         AV_OPT_TYPE_STRING, { 0 },  0,       0, VE },  
-     { "nal-hrd",       "Signal HRD information (requires vbv-bufsize; "
- "cbr not allowed in .mp4)",                        OFFSET(nal_hrd),       AV_OPT_TYPE_INT,    { .i64 = -1 }, -1, INT_MAX, VE, "nal-hrd" },  
-     { "none",          NULL, 0, AV_OPT_TYPE_CONST, {.i64 = X264_NAL_HRD_NONE}, INT_MIN, INT_MAX, VE, "nal-hrd" },  
-     { "vbr",           NULL, 0, AV_OPT_TYPE_CONST, {.i64 = X264_NAL_HRD_VBR},  INT_MIN, INT_MAX, VE, "nal-hrd" },  
-     { "cbr",           NULL, 0, AV_OPT_TYPE_CONST, {.i64 = X264_NAL_HRD_CBR},  INT_MIN, INT_MAX, VE, "nal-hrd" },  
-     { "avcintra-class","AVC-Intra class 50/100/200",                      OFFSET(avcintra_class),AV_OPT_TYPE_INT,     { .i64 = -1 }, -1, 200   , VE},  
-     { "x264-params",  "Override the x264 configuration using a :-separated list of key=value parameters", OFFSET(x264_params), AV_OPT_TYPE_STRING, { 0 }, 0, 0, VE },  
-     { NULL },  
- };  
### Libx265
Libx265的AVClass定义如下所示。

- **static****const** AVClass **class** = {  
-     .class_name = "libx265",  
-     .item_name  = av_default_item_name,  
-     .option     = options,  
-     .version    = LIBAVUTIL_VERSION_INT,  
- };  
其中option字段指向的数组定义如下所示。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/eabbc1a7d28e4db7a73615cfd7b5c724/code_ico.png)
- **static****const** AVOption options[] = {  
-     { "preset",      "set the x265 preset",                                                         OFFSET(preset),    AV_OPT_TYPE_STRING, { 0 }, 0, 0, VE },  
-     { "tune",        "set the x265 tune parameter",                                                 OFFSET(tune),      AV_OPT_TYPE_STRING, { 0 }, 0, 0, VE },  
-     { "x265-params", "set the x265 configuration using a :-separated list of key=value parameters", OFFSET(x265_opts), AV_OPT_TYPE_STRING, { 0 }, 0, 0, VE },  
-     { NULL }  
- };  
## 官方代码中有关AVClass和AVOption的示例
官方代码中给出了一小段示例代码，演示了如何给一个普通的结构体添加AVOption的支持。
[cpp][view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/44268323#)![](file:///D:/temp/soft_data/Ynote/elesos@163.com/9e9af45f591749099b040c6d4daa7943/code_ico.png)
- **typedef****struct** test_struct {  
-  AVClass  ***class**;  
- **int**      int_opt;  
- **char**    str_opt;  
-     uint8_t  bin_opt;  
- **int**      bin_len;  
- } test_struct;  
- 
- **static****const** AVOption test_options[] = {  
-   { "test_int", "This is a test option of int type.", offsetof(test_struct, int_opt),  
-     AV_OPT_TYPE_INT, { .i64 = -1 }, INT_MIN, INT_MAX },  
-   { "test_str", "This is a test option of string type.", offsetof(test_struct, str_opt),  
-     AV_OPT_TYPE_STRING },  
-   { "test_bin", "This is a test option of binary type.", offsetof(test_struct, bin_opt),  
-     AV_OPT_TYPE_BINARY },  
-   { NULL },  
- };  
- 
- **static****const** AVClass test_class = {  
-     .class_name = "test class",  
-     .item_name  = av_default_item_name,  
-     .option     = test_options,  
-     .version    = LIBAVUTIL_VERSION_INT,  
- };  
## AVClass有关的API
与AVClass相关的API很少。AVFormatContext提供了一个获取当前AVClass的函数avformat_get_class()。它的代码很简单，直接返回全局静态变量av_format_context_class。定义如下所示。

- **const** AVClass *avformat_get_class(**void**)  
- {  
- **return** &av_format_context_class;  
- }  
同样，AVCodecContext也提供了一个获取当前AVClass的函数avcodec_get_class()。它直接返回静态变量av_codec_context_class。定义如下所示。
- **const** AVClass *avcodec_get_class(**void**)  
- {  
- **return** &av_codec_context_class;  
- }  
参考：
**[结构体成员管理系统-AVClass](http://blog.csdn.net/leixiaohua1020/article/details/44268323)**

