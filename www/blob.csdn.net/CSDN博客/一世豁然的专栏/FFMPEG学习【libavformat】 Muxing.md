# FFMPEG学习【libavformat】:Muxing - 一世豁然的专栏 - CSDN博客





2017年06月14日 14:34:57[一世豁然](https://me.csdn.net/Explorer_day)阅读数：497








MUXER以AVPackets的形式获取编码数据，并以指定的容器格式将其写入文件或其他输出字节。




用于muxing的主要API函数是用于编写文件头的avformat_write_header（），用于写入数据包的av_write_frame（）/ av_interleaved_write_frame（）和用于完成文件的av_write_trailer（）。





在复用过程开始时，调用者必须首先调用avformat_alloc_context（）来创建多路复用上下文。 然后，呼叫者通过填充这个上下文中的各个字段来设置muxer：


1、必须设置theormat字段来选择要使用的muxer。

2、除非格式为AVFMT_NOFILE类型，否则必须将pb字段设置为从avio_open2（）或自定义IO上返回的已打开的IO上下文。

3、除非格式为AVFMT_NOSTREAMS类型，否则必须使用avformat_new_stream（）函数创建至少一个流。 呼叫者应该如已知的那样填充流编解码器参数信息，例如编解码器类型，ID和其他参数（例如，宽/高，像素或采样格式等）。 流时基应设置为调用者希望用于此流的时基（请注意，muxer实际使用的时基可能不同，后面将会介绍）。

4、建议在重新配置期间仅手动初始化AVCodecParameters中的相关字段，而不是使用avcodec_parameters_copy（）：不能保证编解码器上下文值对输入和输出格式上下文都保持有效。

5、呼叫者可以填写附加信息，例如AVFormatContext文档中描述的全局或每流元数据，章节，程序等。 这些信息是否实际存储在输出中取决于容器格式和muxer支持的内容。




当多路复用上下文完全设置时，调用者必须调用avformat_write_header（）来初始化muxer内部并写入文件头。 在此步骤中是否写入IO上下文是否依赖于muxer，但此函数始终被调用。 必须在options参数中传递任何muxer私有选项到此函数。





然后，通过重复调用av_write_frame（）或av_interleaved_write_frame（）（请参阅这些函数的文档来讨论它们之间的差异，然后将数据发送到muxer;只有一个可能与单个复用上下文一起使用，不应该混合）。 请注意，发送到muxer的数据包的时间信息必须在相应的AVStream的时基中。 该时基由muxer设置（在avformat_write_header（）步骤中），可能与调用者请求的时基不同。





一旦写入完所有的数据，调用者必须调用av_write_trailer（）来刷新任何缓冲的数据包并完成输出文件，然后关闭IO上下文（如果有的话），最后用avformat_free_context（）释放多路复用上下文。





一、数据结构


|struct|[AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html){const char * name;const char * long_name;// 格式的描述性名称，意思是更加人性化的名字。const char * mime_type；const char * extensions；// 逗号分隔的文件扩展名enum AVCodecID audio_codec；// 默认音频编解码器enum AVCodecID video_codec；// 默认视频编解码器enum AVCodecID subtitle_codec；// 默认字幕编解码器int flags；// 可以使用标志：AVFMT_NOFILE，AVFMT_NEEDNUMBER，AVFMT_GLOBALHEADER，AVFMT_NOTIMESTAMPS，AVFMT_VARIABLE_FPS，AVFMT_NODIMENSIONS，AVFMT_NOSTREAMS，AVFMT_ALLOW_FLUSH，AVFMT_TS_NONSTRICT，AVFMT_TS_NEGATIVEconst struct AVCodecTag *const * codec_tag；// 支持的codec_id-codec_tag对的列表，按“更好的选择第一”排序。const AVClass * priv_class；// AVClass为私有上下文。struct AVOutputFormat * next；int priv_data_size；// 私人数据的大小，使其可以分配在包装int(* write_header )(struct AVFormatContext *)；int(* write_packet )(struct AVFormatContext *, AVPacket *pkt)；//写一个包int(* write_trailer )(struct AVFormatContext *)；int(* interleave_packet )(struct AVFormatContext *, AVPacket *out, AVPacket *in, int flush)；// 目前只用于设置像素格式如果不是YUV420P。int(* query_codec )(enum AVCodecID id, int std_compliance)；// 测试给定的编解码器是否可以存储在此容器中。void(* get_output_timestamp )(struct AVFormatContext *s, int stream, int64_t *dts, int64_t *wall)；int(* control_message )(struct AVFormatContext *s, int type, void *data, size_t data_size)；// 允许从应用程序发送消息到设备。int(* write_uncoded_frame )(struct AVFormatContext *, int stream_index, AVFrame **frame, unsigned flags)；// 写一个未编码的AVFrame。int(* get_device_list )(struct AVFormatContext *s, struct AVDeviceInfoList *device_list)；//  返回带有属性的设备列表。int(* create_device_capabilities )(struct AVFormatContext *s, struct AVDeviceCapabilitiesQuery *caps)；// 初始化设备功能子模块。int(* free_device_capabilities )(struct AVFormatContext *s, struct AVDeviceCapabilitiesQuery *caps)；// 释放设备功能子模块。enum AVCodecID data_codec；//默认数据编解码器int(* init )(struct AVFormatContext *)；// 初始化格式void(* deinit )(struct AVFormatContext *)；// 取消初始化格式int(* check_bitstream )(struct AVFormatContext *, const AVPacket *pkt)；// 设置任何必要的比特流过滤，并提取全局头文件所需的任何额外数据。}|
|----|----|





二、宏


|#define|[AVSTREAM_INIT_IN_WRITE_HEADER](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga5396ff79e3c1ec7220765499f2efc74d)   0|
|----|----|

流参数在avformat_write_header中初始化





|#define|[AVSTREAM_INIT_IN_INIT_OUTPUT](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga5268e4fbbb73b5ed1e4083f44f565ffd)   1|
|----|----|

流参数在avformat_init_output中初始化







三、函数


|[av_warn_unused_result](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_write_header](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga18b7b10bb5b94c4842de18166bc677cb) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

分配流私有数据并将流标题写入输出媒体文件。

参数：s：媒体文件句柄必须使用avformat_alloc_context（）分配。 其字段必须设置为所需的输出格式; 其pb字段必须设置为已经打开的AVIOContext。

   options：一个充满AVFormatContext和muxer-private选项的AVDictionary。 返回时，此参数将被销毁，并替换为包含未找到的选项的dict。 可能为NULL。

返回：如果编解码器尚未在avformat_init中完全初始化，则AVSTREAM_INIT_IN_WRITE_HEADER成功，如果编解码器已经在avformat_init中被完全初始化，则AVSTREAM_INIT_IN_INIT_OUTPUT成功，否则AVERROR为失败。





|[av_warn_unused_result](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_init_output](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga169cfa28508e22e138c5b99be8517ea4) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

分配流私有数据并初始化编解码器，但不要写头。

可以在avformat_write_header之前可以使用它来在实际写入标题之前初始化流参数。 如果使用此功能，请不要将相同的选项传递给avformat_write_header。


参数：s：媒体文件句柄必须使用avformat_alloc_context（）分配。 其字段必须设置为所需的输出格式; 其pb字段必须设置为已经打开的AVIOContext。


   options：一个充满AVFormatContext和muxer-private选项的AVDictionary。 返回时，此参数将被销毁，并替换为包含未找到的选项的dict。 可能为NULL。

返回：如果编解码器要求avformat_write_header完全初始化，则AVSTREAM_INIT_IN_WRITE_HEADER成功，如果编解码器已完全初始化，则AVSTREAM_INIT_IN_INIT_OUTPUT成功完成，失败时为AVERROR。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_write_frame](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#gaa85cc1774f18f306cd20a40fc50d0b36) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090))|
|----|----|

将数据包写入输出媒体文件。

此功能将数据包直接传递到复用器，无需任何缓冲或重新排序。 如果格式要求，呼叫者负责正确交织数据包。 需要libavformat来处理交错的调用者应该调用av_interleaved_write_frame（）而不是该函数。


参数：s：媒体文件句柄

   pkt：该数据包包含要写入的数据。 请注意，与av_interleaved_write_frame（）不同，此函数不会传递给它的数据包的所有权（尽管某些muxers可能会对输入数据包进行内部引用）。

    该参数可以为NULL（在任何时候，不仅仅是在结束时），以便立即刷新muxer中缓冲的数据，以便在将数据写入输出之前将数据缓存到内部的多路复用器。

     数据包的stream_index字段必须设置为s-> stream中对应流的索引。

     必须将时间戳（pts，dts）设置为纠正流的时基中的值（除非输出格式用AVFMT_NOTIMESTAMPS标志标记，否则可以设置为AV_NOPTS_VALUE）。 传递给此函数的后续数据包的dts必须在其各自的时间基础上进行比较时严格增加（除非输出格式与AVFMT_TS_NONSTRICT标记在一起，那么它们只需要不减速）。 持续时间）也应该被设置。

返回：<0出错，= 0如果OK，1如果刷新，没有更多的数据要刷新





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_interleaved_write_frame](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga37352ed2c63493c38219d935e71db6c1) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090))|
|----|----|

将数据包写入输出媒体文件，确保正确的交错。

此函数将根据需要在内部缓冲数据包，以确保输出文件中的数据包按照增加dts的顺序正确交错。 调用者进行自己的交织应该调用av_write_frame（）而不是这个函数。


使用此函数而不是av_write_frame（）可以使muxers提前了解未来数据包的知识， 在碎片模式下，mp4 muxer的VFR内容的行为。


参数：s：媒体文件句柄

   pkt：该数据包包含要写入的数据。

    如果数据包是引用计数的，则此函数将获取该引用的所有权，并在稍后发现它合适时取消引用。 此函数返回后，调用者不得通过此引用访问数据。 如果数据包未被引用计数，则libavformat将会生成一个副本。

    此参数可以为空（在任何时候，不只是结束），以刷新交错队列。

    数据包的stream_index字段必须设置为s-> stream中对应流的索引。

    必须将时间戳（pts，dts）设置为纠正流的时基中的值（除非输出格式用AVFMT_NOTIMESTAMPS标志标记，否则可以设置为AV_NOPTS_VALUE）。 一个流中后续数据包的dts必须严格增加（除非输出格式用AVFMT_TS_NONSTRICT进行标记，那么它们只需要不减速）。 持续时间）也应该被设置。

返回：0成功，否定AVERROR出错。 即使此功能发生故障，Libavformat将始终保留释放数据包。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_write_uncoded_frame](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#gaab766dccdff1bf323a2fc604e723b441) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) stream_index, [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

将未编码的帧写入输出媒体文件。

框架必须根据容器规格正确交错; 如果没有，那么必须使用av_interleaved_write_frame（）。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_interleaved_write_uncoded_frame](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#gafe9916c601bba6827d6f9271a61260e4) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) stream_index, [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

将未编码的帧写入输出媒体文件。

如果复用器支持它，则该功能可以直接写入AVFrame结构，而无需将其编码到数据包中。 它对于使用原始视频或PCM数据的设备和类似的特殊复用器来说是非常有用的，不会将其串行化为字节流。


要测试是否可以使用给定的muxer和stream，请使用av_write_uncoded_frame_query（）。


呼叫方放弃该帧的所有权，之后不能访问该帧。


返回：> = 0成功，错误时返回负码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_write_uncoded_frame_query](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga5f6bebdc8c234a5ad9740de89acb15e6) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) stream_index)|
|----|----|

测试muxer是否支持未编码的帧。

返回：> = 0，如果未编码的帧可以写入该muxer和流，<0如果不是





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_write_trailer](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga7f14007e7dc8f481f054b21614dfec13) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

将流预告片写入输出媒体文件，并释放文件私有数据。

只能在成功调用avformat_write_header之后调用。


参数：s：媒体文件句柄

0如果OK，AVERROR_xxx出错






|[AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *|[av_guess_format](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga8795680bd7489e96eeb5aef5e615cacc) (const char *short_name, const char *filename, const char *mime_type)|
|----|----|

返回与所提供参数最匹配的已注册输出格式列表中的输出格式，如果不匹配则返回NULL。

参数：short_name：如果non-NULL检查short_name是否与注册格式的名称匹配

   filename：如果非NULL检查文件名是否以注册格式的扩展名终止

mime_type：如果非空检查mime_type是否与注册格式的MIME类型匹配





|enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)|[av_guess_codec](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#gae8a1efab53a348857f209ea51037da4c) ([AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab), const char *short_name, const char *filename, const char *mime_type, enum [AVMediaType](http://ffmpeg.org/doxygen/trunk/group__lavu__misc.html#ga9a84bba4713dfced21a1a56163be1f48)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|

根据muxer和filename来猜测编解码器ID。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_output_timestamp](http://ffmpeg.org/doxygen/trunk/group__lavf__encoding.html#ga661ee0b2f3dbcaaef5d68f2f5d75e2d4) (struct [AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) stream, int64_t *dts, int64_t *wall)|
|----|----|

获取当前输出的数据的时序信息。

“当前输出”的确切含义取决于格式。 它主要与具有内部缓冲区和/或实时工作的设备相关。


参数：s：媒体文件句柄

   stream：流媒体文件

   [out]dts：流的最后一个数据包输出的DTS，以stream time_base为单位

   [out]wall：绝对时间，该分组输出，以微秒为单位

返回：0如果OK，AVERROR（ENOSYS）如果格式不支持它注意：某些格式或设备可能不允许以原子方式测量dts和wall。



