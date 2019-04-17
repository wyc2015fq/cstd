# FFMPEG学习【libavformat】:Demuxing - 一世豁然的专栏 - CSDN博客





2017年06月14日 09:21:34[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1518








Demuxers读取一个媒体文件并将其分割成数据块（数据包）。





分组包含属于单个基本流的一个或多个编码帧。 在lavf API中，此过程由用于打开文件的avformat_open_input（）函数表示，av_read_frame（）用于读取单个数据包，最后是avformat_close_input（），该文件执行清理。








一、数据结构


|struct|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html){const char * name；// 用逗号分隔的格式的短名单。const char * long_name；//格式的描述性名称，意思是更加人性化的名字。int flags；//可以使用标志：AVFMT_NOFILE，AVFMT_NEEDNUMBER，AVFMT_SHOW_IDS，AVFMT_GENERIC_INDEX，AVFMT_TS_DISCONT，AVFMT_NOBINSEARCH，AVFMT_NOGENSEARCH，AVFMT_NO_BYTE_SEEK，AVFMT_SEEK_TO_PTS。const char * extensions；// 如果定义了扩展名，则不会进行任何探针。const struct AVCodecTag *const * codec_tag；const AVClass * priv_class；// AVClass为私有上下文。const char * mime_type；// 逗号分隔的MIME类型列表。struct AVInputFormat * next；int raw_codec_id；// 原始解码器在这里存储他们的编解码器ID。int priv_data_size;// 私有数据的大小可以在包装器中分配。int(* read_probe )(AVProbeData *)；// 告诉某个给定的文件是否有机会被解析为这种格式。int(* read_header )(struct AVFormatContext *)；// 读取格式头并初始化AVFormatContext结构。int(* read_packet )(struct AVFormatContext *, AVPacket *pkt)；// 读取一个数据包并将其放在“pkt”中。int(* read_close )(struct AVFormatContext *)；//关闭流。int(* read_seek )(struct AVFormatContext *, int stream_index, int64_t timestamp, int flags)；// 寻求相对于流组件stream_index中的帧的给定时间戳。int64_t(* read_timestamp )(struct AVFormatContext *s, int stream_index, int64_t *pos, int64_t pos_limit)；// 以stream [stream_index] .time_base为单位获取下一个时间戳。int(* read_play )(struct AVFormatContext *)；// 开始/恢复播放 - 只有使用基于网络的格式（RTSP）才有意义。int(* read_pause )(struct AVFormatContext *)；// 暂停播放 - 只有使用基于网络的格式（RTSP）才有意义。int(* read_seek2 )(struct AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags)；// 寻求时间戳ts。int(* get_device_list )(struct AVFormatContext *s, struct AVDeviceInfoList *device_list)；// 返回带有属性的设备列表。int(* create_device_capabilities )(struct AVFormatContext *s, struct AVDeviceCapabilitiesQuery *caps)；// 初始化设备功能子模块。int(* free_device_capabilities )(struct AVFormatContext *s, struct AVDeviceCapabilitiesQuery *caps)；// 免费设备功能子模块。}|
|----|----|








二、函数


|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *|[av_find_input_format](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga7d2f532c6653c2419b17956712fdf3da) (const char *short_name)|
|----|----|

根据输入格式的短名称查找AVInputFormat。





|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *|[av_probe_input_format](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gad8a1268228fff3aeae0aeaa99a767a24) ([AVProbeData](http://ffmpeg.org/doxygen/trunk/structAVProbeData.html) *pd, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) is_opened)|
|----|----|

猜测文件格式。

参数：pd：要探测的数据

is_opened：文件是否已经打开; 确定是否探测具有或不具有AVFMT_NOFILE的分离器。





|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *|[av_probe_input_format2](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gae8b938f6e7c3741dd27a6c171e72f33d) ([AVProbeData](http://ffmpeg.org/doxygen/trunk/structAVProbeData.html) *pd, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) is_opened, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *score_max)|
|----|----|

猜测文件格式。

参数：pd：要探测的数据

is_opened：文件是否已经打开; 确定是否探测具有或不具有AVFMT_NOFILE的分离器。

score_max：探测分数越大，需要接受检测，该变量随后被设置为实际检测分数。 如果分数<= AVPROBE_SCORE_MAX / 4，建议用较大的探针缓冲区重试。





|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *|[av_probe_input_format3](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga7e4318244afd6c5fe55cec75d27258b2) ([AVProbeData](http://ffmpeg.org/doxygen/trunk/structAVProbeData.html) *pd, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) is_opened, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *score_ret)|
|----|----|

猜测文件格式。

参数：is_opened：文件是否已经打开; 确定是否探测具有或不具有AVFMT_NOFILE的分离器。

score_ret：得分最好的检测。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_probe_input_buffer2](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gafba896fafa5947fefeb47360f0bb0237) ([AVIOContext](http://ffmpeg.org/doxygen/trunk/structAVIOContext.html) *pb, [AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) **[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab), const char *url, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *logctx, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[offset](http://ffmpeg.org/doxygen/trunk/vf__uspp_8c.html#a06b941e27d7180a9a7c906f9907682d6), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) max_probe_size)|
|----|----|

探测一个字节流来确定输入格式。

每次探头返回的分数太低时，探头缓冲区大小会增加，并进行另一次尝试。 当达到最大探针尺寸时，返回具有最高分数的输入格式。


参数：pd：用来探测的比特流

   fmt：输入格式放在这里

   url：流url

   logctx：日志上下文

   offset：在字节流内的偏移量进行探测

max_probe_size：最大探针缓冲区大小（默认为零）

返回：在成功的情况下的分数，对应于最大分数的负值为AVPROBE_SCORE_MAX AVERROR代码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_probe_input_buffer](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga85cab6fb4038a82d90b4adea8c253943) ([AVIOContext](http://ffmpeg.org/doxygen/trunk/structAVIOContext.html) *pb, [AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) **[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab), const char *url, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *logctx, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[offset](http://ffmpeg.org/doxygen/trunk/vf__uspp_8c.html#a06b941e27d7180a9a7c906f9907682d6), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) max_probe_size)|
|----|----|

像av_probe_input_buffer2（），但成功返回0。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_open_input](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga31d601155e9035d5b0e7efedc894ee49) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) **ps, const char *url, [AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab), [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

打开输入流并读取标题。

编解码器未打开。 流必须用avformat_close_input（）关闭。


参数：ps：指向用户提供的AVFormatContext（由avformat_alloc_context分配）。 可能是指向NULL的指针，在这种情况下，AVFormatContext由该函数分配并写入ps。 请注意，用户提供的AVFormatContext将在失败时被释放。

   url：要打开的流的URL。

   fmt：如果非NULL，则此参数强制使用特定的输入格式。 否则格式将自动检测。

   options：一个充满AVFormatContext和demuxer-private选项的字典。 返回时，此参数将被销毁，并替换为包含未找到的选项的dict。 可能为NULL。

返回：返回0成功，失败返回负的AVERROR

注意：如果要使用自定义IO，则预分配格式上下文并设置其pb字段。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_demuxer_open](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga71091e146791c4d181635485447c2830) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *ic)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_find_stream_info](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gad42172e27cddafb81096939783b157bb) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *ic, [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

读取媒体文件的数据包以获取流信息。

这对于没有诸如MPEG的标题的文件格式很有用。 该功能还可以在MPEG-2重复帧模式的情况下计算真实的帧率。 此功能不会更改逻辑文件位置; 检查的分组可以被缓冲以供稍后处理。


参数：ic：媒体文件句柄

   options：如果非NULL，则ic.nb_streams指向字典的长数组指针，其中第i个成员包含对应于第i个流的编解码器选项。 返回时，每个字典将填充未找到的选项。

返回：> = 0如果OK，AVERROR_xxx出错

注意：这个功能不能保证打开所有的编解码器，所以返回时非空的选项是完全正常的行为。

提示：让用户以某种方式决定需要什么信息，以免我们浪费时间获取用户不需要的东西。





|[AVProgram](http://ffmpeg.org/doxygen/trunk/structAVProgram.html) *|[av_find_program_from_stream](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga9edb9ac206ed05dc5baa9d168f31de48) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *ic, [AVProgram](http://ffmpeg.org/doxygen/trunk/structAVProgram.html) *last, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

查找属于给定流的程序。

参数：ic：媒体文件句柄

   last：最后找到的程序，搜索将在此程序后启动，或者从开始，如果为NULL

   s：流索引

返回：属于s的下一个程序，如果没有程序被找到，则为NULL，或者最后一个程序不在ic程序中。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_program_add_stream_index](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gaa59b7800da8ec11457c1e5b3fc295a20) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *ac, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) progid, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) idx)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_find_best_stream](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gaa6fa468c922ff5c60a6021dcac09aff9) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *ic, enum [AVMediaType](http://ffmpeg.org/doxygen/trunk/group__lavu__misc.html#ga9a84bba4713dfced21a1a56163be1f48)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) wanted_stream_nb, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) related_stream, [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) **decoder_ret, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

找到文件中的“最佳”流。

最好的流是根据各种启发式确定的，最有可能是用户期望的。 如果解码器参数不为NULL，则av_find_best_stream将找到流的编解码器的默认解码器; 不能找到解码器的流被忽略。


参数：ic：媒体文件句柄

   type：流类型：视频，音频，字幕等

wanted_stream_nb：用户请求的流号码，或-1表示自动选择

related_stream：尝试找到与此相关的流（例如，在同一程序中），如果没有，则返回-1

decoder_ret：如果非NULL，则返回所选流的解码器

   flags：标志; 目前没有定义

返回：在成功的情况下为非负流数，AVERROR_STREAM_NOT_FOUND如果没有可以找到请求类型的流，则AVERROR_DECODER_NOT_FOUND如果发现流，但没有解码器

注意：如果av_find_best_stream成功返回并且decode_ret不为NULL，则* decode_ret保证被设置为有效的AVCodec。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_read_frame](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga4fdb3084415a82e3810de6ee60e46a61) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090))|
|----|----|

返回流的下一帧。

此函数返回存储在文件中的内容，并且不会验证解码器有什么有效帧。 它会将存储在文件中的内容分成帧，并为每个调用返回一个。 它不会在有效帧之间省略无效数据，以便为解码器提供可能进行解码的最大信息。


如果pkt-> buf为NULL，则数据包有效直到下一个av_read_frame（）或直到avformat_close_input（）。 否则数据包无限期有效。 在这两种情况下，当不再需要数据包时，必须使用av_packet_unref来释放数据包。 对于视频，数据包只包含一帧。 对于音频，如果每个帧具有已知的固定大小（例如PCM或ADPCM数据），则它包含整数帧。 如果音频帧具有可变大小（例如，MPEG音频），则它包含一帧。


pkt-> pts，pkt-> dts和pkt->持续时间始终设置为在AVStream.time_base单位中修正值（如果格式无法提供）。 如果视频格式有B帧，pkt-> pts可以是AV_NOPTS_VALUE，所以如果不解压缩有效载荷，最好依靠pkt-> dts。


返回：0表示成功，<0表示错误或者文件的尾部





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_seek_frame](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gaa23f7619d8d4ea0857065d9979c75ac8) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) stream_index, int64_t timestamp, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

以时间戳寻求关键帧。

参数：s：媒体文件句柄

stream_index：如果stream_index为（-1），则选择默认流，并将时间戳从AV_TIME_BASE单位自动转换为流特定的time_base。

timestamp：AVStream.time_base单位中的时间戳记，或者如果没有指定流，则以AV_TIME_BASE为单位。

   flags：选择方向和寻找模式的标志

返回：>=0表示成功





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_seek_file](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga3b40fc8d2fda6992ae6ea2567d71ba30) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

寻求时间戳ts。

要完成这一工作，所以可以成功地显示所有活动流的点将最接近ts并在最小/最大值范围内。 活动流都是具有AVStream.discard <AVDISCARD_ALL的流。


如果标志包含AVSEEK_FLAG_BYTE，则所有时间戳都是以字节为单位，并且是文件位置（所有分离器可能不支持）。 如果标志包含AVSEEK_FLAG_FRAME，则所有时间戳都与stream_index的流中的帧相同（所有分离器可能不支持）。 否则所有时间戳都是以stream_index选择的流或者stream_index为-1，以AV_TIME_BASE为单位。 如果标志包含AVSEEK_FLAG_ANY，则非关键帧将被视为关键帧（所有分离器可能不支持此关键帧）。 如果标志包含AVSEEK_FLAG_BACKWARD，则会被忽略。


参数：s：媒体文件句柄

stream_index：用作时基参考的流的索引

min_ts：最小可接受的时间戳

   ts：目标时间戳

max_ts：最大可接受的时间戳

   flags：标志

返回：> = 0成功，否则为错误代码

注意：这是新的API API的一部分，它仍在建设中。 因此，不要使用它。 它可能随时改变，不要指望ABI兼容性！





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_flush](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gaa03a82c5fd4fe3af312d229ca94cd6f3) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

丢弃所有内部缓冲的数据。

当处理字节流中的不连续性时，这可能很有用。 通常只能使用可以重新同步的格式。 这包括无标题格式，如MPEG-TS / TS，但也应该使用NUT，Ogg和有限的方式AVI。


调用此功能时，流的集合，检测到的持续时间，流参数和编解码器不会改变。 如果你想要一个完整的重置，最好打开一个新的AVFormatContext。


这不刷新AVIOContext（s-> pb）。 如有必要，请在调用此函数之前调用avio_flush（s-> pb）。


参数：s：媒体文件句柄

返回：> = 0成功，否则为错误代码






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_read_play](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga7494bb63a59e79e9fe88eb1682d4d7b3) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

开始播放基于网络的流





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_read_pause](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#ga27db687592d99f25ccf81a3b3ee8da9c) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

暂停基于网络的流





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avformat_close_input](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gae804b99aec044690162b8b9b110236a4) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) **[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

关闭打开的输入AVFormatContext。

释放它及其所有内容，并将*设置为NULL。








三、打开媒体文件

打开文件所需的最少信息是其URL，它传递给avformat_open_input（），如以下代码所示：




```cpp
const char    *url = "file:in.mp3";
AVFormatContext *s = NULL;
int ret = avformat_open_input(&s, url, NULL, NULL);
if (ret < 0)
    abort();
```


上述代码尝试分配AVFormatContext，打开指定的文件（自动检测格式）并读取头文件，将存储在其中的信息导出到s中。 某些格式没有标题或不存储足够的信息，因此建议您调用avformat_find_stream_info（）函数，该函数尝试读取和解码几个帧以找到缺少的信息。




在某些情况下，您可能希望使用avformat_alloc_context（）自己预先分配AVFormatContext，并在将其转换为avformat_open_input（）之前对其进行一些调整。 一种情况是当您想要使用自定义函数来读取输入数据而不是lavf内部I / O层。 要做到这一点，使用avio_alloc_context（）创建自己的AVIOContext，将读取回调传递给它。 然后将AVFormatContext的pb字段设置为新创建的AVIOContext。





由于打开的文件的格式通常在avformat_open_input（）返回之后才知道，所以不可能在预先分配的上下文中设置demuxer的私有选项。 相反，应该将选项传递给包含在AVDictionary中的avformat_open_input（）：




```cpp
AVDictionary *options = NULL;
av_dict_set(&options, "video_size", "640x480", 0);
av_dict_set(&options, "pixel_format", "rgb24", 0);
if (avformat_open_input(&s, url, NULL, &options) < 0)
    abort();
av_dict_free(&options);
```


该代码将私有选项'video_size'和'pixel_format'传递给分解器。 它们将是必需的。 原始视频分解器，因为它不知道如何解释原始视频数据否则。 如果格式与原始视频不同，则这些选项将不会被解码器识别，因此不会被应用。 然后在选项字典中返回这些无法识别的选项（已识别的选项已被使用）。 呼叫程序可以根据需要处理这些未被识别的选项，例如



```cpp
AVDictionaryEntry *e;
if (e = av_dict_get(options, "", NULL, AV_DICT_IGNORE_SUFFIX)) {
    fprintf(stderr, "Option %s not recognized by the demuxer.\n", e->key);
    abort();
}
```


读完文件后，您必须使用avformat_close_input（）关闭文件。 它将释放与文件相关的所有内容。







四、从打开的文件读取

从打开的AVFormatContext读取数据是通过重复调用av_read_frame（）来完成的。 每个呼叫如果成功，将返回包含由AVPacket.stream_index标识的一个AVStream的编码数据的AVPacket。 如果呼叫者希望解码数据，则该数据包可以直接传递到libavcodec解码函数avcodec_send_packet（）或avcodec_decode_subtitle2（）。





如果已知AVPacket.pts，AVPacket.dts和AVPacket.duration定时信息将被设置。 如果流不提供它们，也可能未设置它们（即，对于pts / dts为AV_NOPTS_VALUE，持续时间为0）。 时间信息将以AVStream.time_base为单位，即必须乘以时基将其转换为秒。





如果在返回的数据包上设置了AVPacket.buf，则动态分配数据包，用户可以无限期地保留该数据包。 否则，如果AVPacket.buf为NULL，则分组数据由解复用器内部某处的静态存储器支持，并且数据包仅在下一个av_read_frame（）调用或关闭文件之前有效。 如果调用者需要更长的生命周期，av_dup_packet（）将会生成一个av_malloc（）的副本。 在这两种情况下，当不再需要数据包时，必须使用av_packet_unref（）来释放数据包。




