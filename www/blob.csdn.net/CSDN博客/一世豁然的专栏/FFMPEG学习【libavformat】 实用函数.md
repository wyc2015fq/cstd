# FFMPEG学习【libavformat】:实用函数 - 一世豁然的专栏 - CSDN博客





2017年06月16日 09:23:21[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1420








与muxing和demuxing有关的杂项效用函数（或两者）。







一、模块

一）、描述

获取将RIFF FourCCs映射到libavcodec AVCodecID的表。

这些表旨在传递给av_codec_get_id（）/ av_codec_get_tag（），如下面的代码所示：




```cpp
uint32_t tag = MKTAG('H', '2', '6', '4');
const struct AVCodecTag *table[] = { avformat_get_riff_video_tags(), 0 };
enum AVCodecID id = av_codec_get_id(table, tag);
```


二）、函数


|const struct [AVCodecTag](http://ffmpeg.org/doxygen/trunk/structAVCodecTag.html) *|[avformat_get_riff_video_tags](http://ffmpeg.org/doxygen/trunk/group__riff__fourcc.html#ga036cfb1a22e899179f932df0cfc6c415) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回：表格映射RIFF FourCCs视频到libavcodec AVCodecID。





|const struct [AVCodecTag](http://ffmpeg.org/doxygen/trunk/structAVCodecTag.html) *|[avformat_get_riff_audio_tags](http://ffmpeg.org/doxygen/trunk/group__riff__fourcc.html#ga8b9668cfd684804ebe7ab14b05aa3ba0) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回：表格映射RIFF FourCCs音频到AVCodecID。





|const struct [AVCodecTag](http://ffmpeg.org/doxygen/trunk/structAVCodecTag.html) *|[avformat_get_mov_video_tags](http://ffmpeg.org/doxygen/trunk/group__riff__fourcc.html#ga36261b12dba43e0cba5a753ecb3d0e54) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回：表格映射MOV FourCCs视频到libavcodec AVCodecID。





|const struct [AVCodecTag](http://ffmpeg.org/doxygen/trunk/structAVCodecTag.html) *|[avformat_get_mov_audio_tags](http://ffmpeg.org/doxygen/trunk/group__riff__fourcc.html#gac389e7f9cd19570bd599a61fd1884a4a) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回：表格映射MOV FourCC的音频到AVCodecID。







二、宏


|#define|[AV_FRAME_FILENAME_FLAGS_MULTIPLE](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gaa0fc59624ec1fe0dc583743678b96fb6)   1|
|----|----|

允许多个d。







三、枚举


|enum|[AVTimebaseSource](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga67f84bb8799c8da41194f046f4b970c4) { [AVFMT_TBCF_AUTO](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gga67f84bb8799c8da41194f046f4b970c4a925300170a86b606dbe4b2b2a18ad12a) = -1, [AVFMT_TBCF_DECODER](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gga67f84bb8799c8da41194f046f4b970c4ae6bd8e936313feda91adedd2501ca90b), [AVFMT_TBCF_DEMUXER](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gga67f84bb8799c8da41194f046f4b970c4a4b894ad271e07df75e1b8121455ea4ee) }|
|----|----|

应用一个比特流过滤器列表到数据包。







四、函数


|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hex_dump](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gab533a6d5d830ab90573dd035002c5bca) (FILE *f, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

发送缓冲区的一个很好的十六进制转储到指定的文件流。

参数：f：转储应发送到的文件流指针。

   buf：缓冲

   size：缓冲大小





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hex_dump_log](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga56f93dbd118ff5fe833a2e39b685974c) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *avcl, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[level](http://ffmpeg.org/doxygen/trunk/svq3_8c.html#abbb6c7c49508c1ac374683f2d1159e08), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

发送缓冲区的一个很好的十六进制转储到日志。

参数：avcl：指向任意结构的指针，第一个字段是指向AVClass结构体的指针。

   level：消息的重要性水平，较低的值表示较高的重要性。

   buf：缓冲

   size：缓冲大小





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_pkt_dump2](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga46720b0bb28ca455246805be8969ff9e) (FILE *f, const [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dump_payload, const [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *st)|
|----|----|

发送一个很好的数据包转储到指定的文件流。

参数：f：转储应发送到的文件流指针。

   pkt：数据包转储

dump_payload：如果有效载荷也必须显示，则为真。

   st：数据包所属的AVStream





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_pkt_dump_log2](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gaab9fad988c28cc1c4dcbba24a36b6113) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *avcl, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[level](http://ffmpeg.org/doxygen/trunk/svq3_8c.html#abbb6c7c49508c1ac374683f2d1159e08), const [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dump_payload, const [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *st)|
|----|----|

发送一个不错的数据包转储到日志。

参数：avcl：指向任意结构的指针，第一个字段是指向AVClass结构体的指针。

   level：消息的重要性水平，较低的值表示较高的重要性。

   pkt：转储的数据包

dump_payload：如果有效载荷也必须显示，则为真。

   st：数据包所属的AVStream





|enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)|[av_codec_get_id](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga27b89fa8286af6efb5a69e8db4033b09) (const struct [AVCodecTag](http://ffmpeg.org/doxygen/trunk/structAVCodecTag.html) *const *tags, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[tag](http://ffmpeg.org/doxygen/trunk/movenc_8c.html#a1c50fcd1195659821729f52af8f3bb7d))|
|----|----|

获取给定的编解码器标签标签的AVCodecID。

如果没有找到编解码器ID返回AV_CODEC_ID_NONE。


参数：tags：支持的codec_id-codec_tag对的列表，存储在AVInputFormat.codec_tag和AVOutputFormat.codec_tag中

   tag：编解码器标签与编解码器ID相匹配





|unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_codec_get_tag](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga126917b7ce8b4f05505098b72f4997de) (const struct [AVCodecTag](http://ffmpeg.org/doxygen/trunk/structAVCodecTag.html) *const *tags, enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[id](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a92ba2b3ed65ae251e69195d3ebe639b6))|
|----|----|

获取给定编解码器编号的编号标签。

如果没有找到编解码器标签返回0。


参数：tags：支持的codec_id-codec_tag对的列表，存储在AVInputFormat.codec_tag和AVOutputFormat.codec_tag中

   id：编解码器ID与编解码器标签相匹配





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_codec_get_tag2](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gabe1a7b6824078229c69b75c71053c035) (const struct [AVCodecTag](http://ffmpeg.org/doxygen/trunk/structAVCodecTag.html) *const *tags, enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[id](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a92ba2b3ed65ae251e69195d3ebe639b6), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[tag](http://ffmpeg.org/doxygen/trunk/movenc_8c.html#a1c50fcd1195659821729f52af8f3bb7d))|
|----|----|

获取给定编解码器编号的编解码器标签。

参数：tags：支持的codec_id - codec_tag对的列表，存储在AVInputFormat.codec_tag和AVOutputFormat.codec_tag中

   id：应在列表中搜索的编解码器ID

   tag：指向找到的标签的指针

返回：如果在标签中找不到ID，则为0，如果没有找到则为>0





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_find_default_stream_index](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gaad9737492bb66aeeec37b7c7d8d90f2a) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_index_search_timestamp](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gac4cabc0d681c34b2e2c6a87a55877f0d) ([AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *st, int64_t timestamp, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

获取特定时间戳的索引。

参数：st：时间戳属于的流

timestamp：时间戳来检索索引

   flags：如果AVSEEK_FLAG_BACKWARD，那么返回的索引将对应于<=所请求的索引的时间戳，如果向后是0，那么如果AVSEEK_FLAG_ANY寻求任何帧，则为> =否则只有关键帧

返回：<0如果没有找到这样的时间戳





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_add_index_entry](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga3ed7441cb4af0bdd65a4cf51bf35088e) ([AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *st, int64_t pos, int64_t timestamp, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[distance](http://ffmpeg.org/doxygen/trunk/nellymoserenc_8c.html#a0a5299ac77dc4fb1f9b2682d957945d2), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

将索引条目添加到排序列表中。

如果列表已经包含它，请更新该条目。


参数：timestamp：时间戳在给定流的时基





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_url_split](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga63a0579d6c2aa72470ac742bbbc1205d) (char *proto, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) proto_size, char *authorization, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) authorization_size, char *hostname, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) hostname_size, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *port_ptr, char *path, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) path_size, const char *url)|
|----|----|

将URL字符串拆分为组件。

用于存储单个组件的缓冲区的指针可以为空，以便忽略该组件。 未找到组件的缓冲区设置为空字符串。 如果未找到端口，则将其设置为负值。


参数：proto：协议的缓冲区

proto_size：原始缓冲区的大小

authorization：用于授权的缓冲区

authorization_size：授权缓冲区的大小

hostname：主机名的缓冲区

hostname_size：主机名的缓冲区大小

port_ptr：一个用于存储端口号的指针

 
  path：路径的缓冲区

path_size：路径的缓冲区的大小

 
  url：要拆分的URL





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_dump_format](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gae2645941f2dc779c307eb6314fd39f10) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *ic, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[index](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a750b5d744c39a06bfb13e6eb010e35d0), const char *url, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) is_output)|
|----|----|

打印有关输入或输出格式的详细信息，如持续时间，比特率，流，容器，程序，元数据，边数据，编解码器和时基。

参数：ic：用于分析的上下文

   index：要转储信息的流的索引

   url：要打印的URL，例如源文件或目标文件

is_output：选择指定的上下文是输入（0）还是输出（1）





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_frame_filename2](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga05f17637161cd79506f67b20ea8149c9) (char *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) buf_size, const char *path, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) number, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

在'buf'中返回'd'替换为数字的路径。

还处理'％0nd'格式，其中'n'是总位数和'％'。


参数：buf：目的缓冲区

buf_size：目的缓冲区的大小

   path：编号序列字符串

number：帧号

   flags：AV_FRAME_FILENAME_FLAGS_*

返回：如果OK，则为0，格式错误为-1





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_frame_filename](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga90e027a5d9006148413a1387c506cd40) (char *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) buf_size, const char *path, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) number)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_filename_number_test](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gac20eb319c6c46f581b5d08f5c25e763e) (const char *filename)|
|----|----|

检查文件名实际上是否是编号的序列生成器。

参数：filename：可能编号的序列字符串

返回：1如果有效的编号序列字符串，否则为0





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_sdp_create](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gaa2a7353a6bb0c8726797abd56b176af0) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *ac[], [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) n_files, char *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

为RTP会话生成SDP。

注意，这将覆盖muxer上下文中的AVStreams的id值，以获取唯一的动态有效负载类型。


参数：ac：描述RTP流的AVFormatContexts数组。 如果阵列仅由一个上下文组成，则这样的上下文可以包含多个AVStream（每个RTP流一个AVStream）。 否则，阵列中的所有上下文（每个RTP流的AVCodecContext）必须只包含一个AVStream。

n_files：AVCodecContexts的数量包含在ac中

   buf：SDP将被存储的缓冲区（必须由主叫方分配）

   size：缓冲区大小

返回：0如果OK，AVERROR_xxx出错





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_match_ext](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga8247f49e5642dd3bd20ce6ba5a7f6466) (const char *filename, const char *extensions)|
|----|----|

如果给定的文件名具有一个给定的扩展名，则返回一个正值，否则返回0。

参数：filename：文件名来检查给定的扩展名

extensions：以逗号分隔的文件扩展名列表





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_query_codec](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gaa90b4c72d1bbb298e11096d3a09ec7db) (const [AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *ofmt, enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[codec_id](http://ffmpeg.org/doxygen/trunk/rtp_8c.html#a0fd12851a78b80080e277db30411728d), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) std_compliance)|
|----|----|

测试给定容器是否可以存储编解码器。

参数：ofmt：容器检查兼容性

   codec_id：可能存储在容器中的编解码器

std_compliance：标准合规级别，FF_COMPLIANCE_ *

返回：1如果具有ID codec_id的编解码器可以存储在ofmt中，如果不能存储，则为0。 如果此信息不可用，则为负数。





|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_guess_sample_aspect_ratio](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga0f62da0e809045bb21166013c404ce43) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[format](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a6bd2bb5fcc7628a640958994a22f997c), [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *stream, [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

根据流和帧长宽比来猜测帧的采样宽高比。

由于帧长宽比由编解码器设置，而是由解复用器设置流宽比，所以这两者可能不相等。 如果要显示框架，此函数将尝试返回您应该使用的值。


基本逻辑是使用流宽度比，如果它被设置为某种理由，否则使用帧长宽比。 这样通常容易修改的容器设置可以覆盖帧中的编码值。


参数：format：该流是其一部分的格式上下文

   stream：帧是其中一部分的流

   frame：要确定具有纵横比的框架

返回：猜到（有效）sample_aspect_ratio，0/1如果不知道





|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_guess_frame_rate](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga12c049178414cc221dfafd4e7f836dea) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *stream, [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

根据容器和编解码器信息猜测帧速率。

参数：ctx：该流是其一部分的格式上下文

   stream：帧是其中一部分的流

   frame；应该确定帧速率的帧可以为NULL

返回：猜测（有效）帧率，0/1如果不知道





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_match_stream_specifier](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga7e45597834e9ef3098ddb74bc5e1550c) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *st, const char *spec)|
|----|----|

检查s中包含的流st是否与流说明符规范匹配。

请参阅规范语法文档中的“流说明符”一章。


返回：> 0如果st匹配spec; 0如果st不符合规范; 如果spec无效，则为AVERROR代码

注意：流说明符可以匹配格式中的多个流。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_queue_attached_pictures](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga501ff1b3aa357c09da90c0effb356e45) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_transfer_internal_stream_timing_info](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#gae35832b110d26ffa3e8805b3d55e8f36) (const [AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *ofmt, [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *ost, const [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *ist, enum [AVTimebaseSource](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga67f84bb8799c8da41194f046f4b970c4)[copy_tb](http://ffmpeg.org/doxygen/trunk/ffmpeg__opt_8c.html#a8e4ab3fbd5ee2de7cd51ff1a033db4b3))|
|----|----|

将内部时间信息从一个流传输到另一个流。

执行流拷贝时，此函数很有用。


参数：ofmt：ost的目标输出格式

   ost：输出流需要定时复制和调整

   ist：参考输入流来复制定时

   copy_tb：定义需要导入流编解码器时基的位置





|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_stream_get_codec_timebase](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ga64b609bbdda09ec7b29534a30656eb1a) (const [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *st)|
|----|----|

从流中获得内部编解码器时基。

参数：st：输入流从中提取时基



