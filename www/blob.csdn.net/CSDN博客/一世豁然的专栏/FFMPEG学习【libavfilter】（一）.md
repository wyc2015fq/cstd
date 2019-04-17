# FFMPEG学习【libavfilter】（一） - 一世豁然的专栏 - CSDN博客





2017年06月10日 14:14:23[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2607








此模块用于获取流的属性。





一、Buffer sink accessors模块

一）、函数


|enum [AVMediaType](http://ffmpeg.org/doxygen/trunk/group__lavu__misc.html#ga9a84bba4713dfced21a1a56163be1f48)|[av_buffersink_get_type](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#ga1eb8bbf583ffb7cc29aaa1944b1e699c) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_buffersink_get_time_base](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#gabc82f65ec7f4fa47c5216260639258a1) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersink_get_format](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#ga402ddbef6f7347869725696846ac81eb) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_buffersink_get_frame_rate](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#ga55614fd28de2fa05b04f427390061d5b) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersink_get_w](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#gac8c86515d2ef56090395dfd74854c835) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersink_get_h](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#ga955ecf3680e71e10429d7500343be25c) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_buffersink_get_sample_aspect_ratio](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#gaa38ee33e1c7f6f7cb190bd2330e5f848) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersink_get_channels](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#gace78881c41bf449527826b95d21279a2) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|uint64_t|[av_buffersink_get_channel_layout](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#ga87e21bf198fd932c30cc3cdc6b16bff1) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersink_get_sample_rate](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#ga2af714e82f48759551acdbc4488ded4a) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|



|[AVBufferRef](http://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *|[av_buffersink_get_hw_frames_ctx](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersink__accessors.html#gaa1415790bfe3dacb5af1c60e9eda3714) (const [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|





二）、Buffer source API模块

1、头文件

[buffersrc.h](http://ffmpeg.org/doxygen/trunk/buffersrc_8h.html)





2、数据结构


|struct|[AVBufferSrcParameters](http://ffmpeg.org/doxygen/trunk/structAVBufferSrcParameters.html){int format；//  视频：像素格式，值对应于枚举AVPixelFormat音频：采样格式，值对应于枚举AVSampleFormatAVRational time_base；//  时间基准用于输入帧上的时间戳。int width；//  仅视频，输入框的显示尺寸。int height；AVRational sample_aspect_ratio；//  仅视频，样本（像素）宽高比。AVRational frame_rate；//  仅视频，输入视频的帧速率。AVBufferRef * hw_frames_ctx；//  只有hwaccel像素格式的视频。int sample_rate；//  仅音频，每秒钟采样的音频采样率。uint64_t channel_layout；//  仅音频，音频通道布局。}|
|----|----|

此结构包含描述将传递到此过滤器的帧的参数。




3、枚举


|enum|{ [AV_BUFFERSRC_FLAG_NO_CHECK_FORMAT](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#gga3d01b665f7e924f5f97fa41f5879c7f7a6efcf61145ec6d60d3a773fcd0797872) = 1, [AV_BUFFERSRC_FLAG_PUSH](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#gga3d01b665f7e924f5f97fa41f5879c7f7a361be31bba170d5ec5bac19ce962ceb5) = 4, [AV_BUFFERSRC_FLAG_KEEP_REF](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#gga3d01b665f7e924f5f97fa41f5879c7f7a24758c8f4deed09b04b1be6c2e66a14e) = 8 }|
|----|----|


4、函数


|unsigned|[av_buffersrc_get_nb_failed_requests](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#ga996e96007a0fda870549ac3c4e1e0967) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *buffer_src)|
|----|----|

获取失败的请求数。

失败的请求是在调用request_frame方法时，而缓冲区中没有帧。 添加帧时，该号码将重置。






|[AVBufferSrcParameters](http://ffmpeg.org/doxygen/trunk/structAVBufferSrcParameters.html) *|[av_buffersrc_parameters_alloc](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#gaae82d4f8a69757ce01421dd3167861a5) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配一个新的AVBufferSrcParameters实例。

它应该由av_free（）的调用者释放。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersrc_parameters_set](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#ga398cd2a84f8b4a588197ab9d90135048) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [AVBufferSrcParameters](http://ffmpeg.org/doxygen/trunk/structAVBufferSrcParameters.html) *param)|
|----|----|

使用提供的参数初始化buffersrc或abuffersrc过滤器。

此函数可能会被多次调用，后面的调用将覆盖以前的调用。 一些参数也可以通过AVOptions进行设置，然后使用最后使用的方法优先。


参数：ctx：bufferrc或abuffersrc过滤器的一个实例

    param：流参数。 后来传递给此过滤器的帧必须符合这些参数。 param中的所有分配的字段仍然由调用者拥有，libavfilter将在必要时进行内部副本或引用。

返回：0成功，失败时出现负ERROR代码。





|[av_warn_unused_result](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersrc_write_frame](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#ga786409c3f3910a0ab5fa6b23b4e7399b) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

将一个帧添加到缓冲区源。

参数：ctx：bufferrc过滤器的一个实例

    frame：帧被添加。 如果帧被引用计数，则此函数将对其进行新的引用。 否则将复制帧数据。

返回：0成功，失败时出现负ERROR代码。






|[av_warn_unused_result](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersrc_add_frame](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#ga8fc71cb48c1ad1aa78b48f87daa4cf19) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

将一个帧添加到缓冲区源。

参数：ctx：bufferrc过滤器的一个实例

    frame：帧被添加。 如果帧被引用计数，则此函数将获取参考的所有权并重置帧。 否则将复制帧数据。 如果此函数返回错误，则不会触摸输入框。

返回：0成功，失败时出现负ERROR代码。






|[av_warn_unused_result](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffersrc_add_frame_flags](http://ffmpeg.org/doxygen/trunk/group__lavfi__buffersrc.html#ga73ed90c3c3407f36e54d65f91faaaed9) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *buffer_src, [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

将一个帧添加到缓冲区源。

默认情况下，如果帧被引用计数，则该函数将获取引用的所有权并重置帧。 这可以使用标志来控制。


如果此函数返回错误，则不会触摸输入框。


参数：buffer_src：指向缓冲区源上下文的指针

    frame：一个帧，或者为NULL标记EOF

    flags：AV_BUFFERSRC_FLAG_ *的一个组合

返回：0成功，失败时出现负ERROR代码。





二、头文件

1、[avfilter.h](http://ffmpeg.org/doxygen/trunk/avfilter_8h.html)




2、[version.h](http://ffmpeg.org/doxygen/trunk/libavfilter_2version_8h.html)







三、数据结构


|1、struct|[AVFilter](http://ffmpeg.org/doxygen/trunk/structAVFilter.html){const char * name；//过滤器名称。const char * description；//过滤器说明。const AVFilterPad * inputs；//输入列表，由零元素终止。const AVFilterPad * outputs；//输出列表，由零元素终止。const AVClass * priv_class；//私有数据类，用于声明过滤器私有AVOptions。int flags；//AVFILTER_FLAG_ *的组合。int(* init )(AVFilterContext *ctx)；//过滤初始化函数。int(* init_dict )(AVFilterContext *ctx, AVDictionary **options)；//应该通过想要将AVOptions的字典传递给在init中分配的嵌套上下文的过滤器来设置而不是init。void(* uninit )(AVFilterContext *ctx)；//过滤器在其输入和输出上支持的查询格式。int priv_size；//要为过滤器分配的私有数据的大小int flags_internal；//avfilter的附加标志仅供内部使用。struct AVFilter * next；// 由过滤器注册系统使用。int(* process_command )(AVFilterContext *, const char *cmd, const char *arg, char *res, int res_len, int flags)；// 使过滤器实例处理一个命令。int(* init_opaque )(AVFilterContext *ctx, void *opaque)；//过滤初始化函数，替代init（）回调。int(* activate )(AVFilterContext *ctx);//过滤器激活函数。}|
|----|----|

过滤器定义。





|2、struct|[AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html){const AVClass * av_class；//需要av_log（）和过滤常用选项const AVFilter * filter；// AVFilter的一个实例char * name；//此过滤器实例的名称AVFilterPad * input_pads；//数组输入板AVFilterLink ** inputs；//指向输入链接的指针数组unsigned nb_inputs；//输入板数AVFilterPad * output_pads；//输出板阵列unsigned nb_outputs；//输出板数量void * priv；//过滤器使用的私人数据struct AVFilterGraph * graph；//filtergraph this filter belongs toint thread_type；//允许/使用的多线程类型。AVFilterInternal * internal；//libavfilter内部使用的不透明结构。struct AVFilterCommand * command_queue；char * enable_str；//启用表达式字符串void * enable；// 解析的表达式（AVExpr *）double * var_values；//启用表达式的变量值int is_disabled；//从最后的表达式评估启用状态AVBufferRef * hw_device_ctx；//对于将创建硬件框架的过滤器，设置过滤器应在其中创建的设备。int nb_threads；//此过滤器实例允许的最大线程数。unsigned ready；//过滤器的就绪状态}|
|----|----|

一个过滤器的实例。





|3、struct|[AVFilterLink](http://ffmpeg.org/doxygen/trunk/structAVFilterLink.html){AVFilterContext * src；//源滤镜AVFilterPad * srcpad；//源过滤器上的输出垫AVFilterContext * dst；//目的过滤器AVFilterPad * dstpad；//目标过滤器上的输入板enum AVMediaType type；//过滤介质类型int w；//建议图像宽度int h；//建议图像高度AVRational sample_aspect_ratio；// 建议采样宽高比uint64_t channel_layout；//当前缓冲区的通道布局（请参阅libavutil / channel_layout.h）int sample_rate；//每秒钟的采样int format；//建议媒体格式AVRational time_base；//定义通过此链接的帧/样本的PTS使用的时基。AVFilterFormats * in_formats；//分别由输入和输出过滤器支持的格式和通道布局列表。AVFilterFormats * out_formats；AVFilterFormats * in_samplerates；// 用于自动协商的频道布局和采样率列表。AVFilterFormats * out_samplerates；struct AVFilterChannelLayouts * in_channel_layouts；struct AVFilterChannelLayouts * out_channel_layouts；int request_samples；//仅音频，目的地过滤器将其设置为非零值，以请求具有给定数量样本的缓冲区应发送给它。enum AVFilterLink:: { ... }  init_state；//链接属性（维度等）的初始化阶段struct AVFilterGraph * graph；//图表所属的过滤器。int64_t current_pts；// 链接的当前时间戳，由链接time_base单位的最新帧定义。int64_t current_pts_us；//由AV_TIME_BASE单位的最新帧定义的链接的当前时间戳。int age_index；//年龄数组中的索引AVRational frame_rate；// 链路上流的帧速率，如果未知或变量，则为1/0;如果保留为0/0，将自动从源过滤器的第一个输入复制（如果存在）。AVFrame * partial_buf；//缓冲液部分填充样品以实现固定/最小尺寸。int partial_buf_size；//要分配的部分缓冲区的大小。int min_samples；// 一次过滤的最小样品数量。int max_samples；// 一次过滤的最大样本数。int channels;//频道数量unsigned flags；// 链接处理标志int64_t frame_count_in；//通过链接发送的过去帧数。int64_t frame_count_out；void * frame_pool；// 指向FFFramePool结构体的指针。int frame_wanted_out；// 如果此过滤器的输出上当前需要一个帧，则为真。AVBufferRef * hw_frames_ctx；// 对于hwaccel像素格式，这应该是对描述帧的AVHWFramesContext的引用。char reserved [0xF000];//内部结构成员}|
|----|----|

两个过滤器之间的链接。





|4、struct|[AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html){const AVClass * av_class；AVFilterContext ** filters；unsigned nb_filters；char * scale_sws_opts；// 用于自动插入缩放滤镜的sws选项int thread_type；// 此图中允许使用过滤器的多线程类型。int nb_threads；//  该图中过滤器使用的最大线程数。AVFilterGraphInternal * internal；// libavfilter内部使用不透明对象。void * opaque；//  不透明的用户数据。avfilter_execute_func * execute；// 调用者可以在分配图形之后并在添加任何过滤器之前立即设置此回调，以提供自定义多线程实现。char * aresample_swr_opts；//  用于自动插入的样本过滤器的swr选项，仅通过AVOptions访问AVFilterLink ** sink_links；// 私人领域。int sink_links_count；unsigned disable_auto_convert；}|
|----|----|



|5、struct|[AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html){char * name；//列表中该输入/输出的唯一名称AVFilterContext * filter_ctx；// 与此输入/输出相关联的过滤器上下文int pad_idx；//  用于连接的filt_ctx垫的索引struct AVFilterInOut * next；//  列表中的下一个输入/输入，如果是最后一个则为NULL}|
|----|----|

过滤器链的输入/输出的链表。







四、宏


|#define|[AVFILTER_FLAG_DYNAMIC_INPUTS](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gae6ed6c10a03508829bdf17560e3e10e5)   (1 << 0)|
|----|----|

滤波器输入的数量不是仅由AVFilter.inputs决定的。





|#define|[AVFILTER_FLAG_DYNAMIC_OUTPUTS](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga1c5c173875bdbbbbaad943a5d951846d)   (1 << 1)|
|----|----|

滤波器输出的数量不是仅由AVFilter.inputs决定的。





|#define|[AVFILTER_FLAG_SLICE_THREADS](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gacad757479a5b22c367dd37408a9844fd)   (1 << 2)|
|----|----|

该过滤器通过将帧分割成多个部分并同时处理来支持多线程。





|#define|[AVFILTER_FLAG_SUPPORT_TIMELINE_GENERIC](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga00c4b8da7d5d89fa4ddc5dcdf72bffca)   (1 << 16)|
|----|----|

一些过滤器支持通用的“启用”表达式选项，可用于在时间轴中启用或禁用过滤器。





|#define|[AVFILTER_FLAG_SUPPORT_TIMELINE_INTERNAL](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gae225339ad292450a5832296f58f0d424)   (1 << 17)|
|----|----|

与AVFILTER_FLAG_SUPPORT_TIMELINE_GENERIC相同，只是当启用表达式为假时，过滤器将会像往常一样调用其filter_frame（）回调函数。





|#define|[AVFILTER_FLAG_SUPPORT_TIMELINE](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gaa7c47b597777253aaa41dd6c7b41a8a2)   ([AVFILTER_FLAG_SUPPORT_TIMELINE_GENERIC](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga00c4b8da7d5d89fa4ddc5dcdf72bffca) | [AVFILTER_FLAG_SUPPORT_TIMELINE_INTERNAL](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gae225339ad292450a5832296f58f0d424))|
|----|----|

用于测试滤镜是否支持或没有时间轴功能（内部或一般）的方便面罩。



|| |
|----|----|
|#define|[AVFILTER_THREAD_SLICE](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga872969bce62dd528ffb91a1096d9f99a)   (1 << 0)|

同时处理帧的多个部分。





|#define|[AVFILTER_CMD_FLAG_ONE](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gace41bae000b621fc8804a93ce9f2d6e9)   1|
|----|----|


停止一次过滤器了解命令（例如对于target = all），快速过滤器将自动受益。





|#define|[AVFILTER_CMD_FLAG_FAST](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gae7b6836b786ef62d5a5d58941e948d74)   2|
|----|----|

只有当它的快速执行命令（像在hw中支持对比度调整的视频输出）




五、类型定义


|typedef [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(|[avfilter_action_func](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga206af7b1d9892c2a46850e4e2b76f491) )([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[arg](http://ffmpeg.org/doxygen/trunk/libavcodec_2jacosubdec_8c.html#ac3a73711fd7f8a50db1c4b62e41cf852), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) jobnr, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_jobs)|
|----|----|

传递给AVFilterGraph :: execute回调的函数指针可以多次执行，可能并行执行。





|typedef [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(|[avfilter_execute_func](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gadad885bdec73d83d5d1633cddee2ae08) )([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [avfilter_action_func](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga206af7b1d9892c2a46850e4e2b76f491) *[func](http://ffmpeg.org/doxygen/trunk/checkasm_8c.html#af406187a441c51e4589704fae05f15fe), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[arg](http://ffmpeg.org/doxygen/trunk/libavcodec_2jacosubdec_8c.html#ac3a73711fd7f8a50db1c4b62e41cf852), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *ret, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_jobs)|
|----|----|

执行多个作业的功能，可能并行。




六、枚举


|enum|{ [AVFILTER_AUTO_CONVERT_ALL](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ggade4ace78b27f5ceccade6e2e29404c48a385b5a02cfa6899d888c996309cb82ed) = 0, [AVFILTER_AUTO_CONVERT_NONE](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ggade4ace78b27f5ceccade6e2e29404c48a065142be87c3afee01ef498654fde0b4) = -1 }|
|----|----|







