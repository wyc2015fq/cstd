# FFMPEG学习【libavformat】:核心函数 - 一世豁然的专栏 - CSDN博客





2017年06月15日 09:28:48[一世豁然](https://me.csdn.net/Explorer_day)阅读数：775








查询libavformat功能，分配核心结构等函数







一、函数


|unsigned|[avformat_version](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gaf2d4e0fee66cef99dea2153933e6b907) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回LIBAVFORMAT_VERSION_INT常量。





|const char *|[avformat_configuration](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga5f48b0398d77d8eeb45d6c649e4e3404) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavformat构建时配置。





|const char *|[avformat_license](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga160fef784175409f945c70e8931accc8) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavformat许可证。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_register_all](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga917265caec45ef5a0646356ed1a507e3) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

初始化libavformat并注册所有的muxers，demuxers和protocol。

如果不调用此功能，则可以准确选择要支持的格式。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_register_input_format](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga896f319308627b5b66fd41a4b1d53ae6) ([AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *[format](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a6bd2bb5fcc7628a640958994a22f997c))|
|----|----|
|| |



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_register_output_format](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gabf0629358bc40c7874932ef6f4dda6b8) ([AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *[format](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a6bd2bb5fcc7628a640958994a22f997c))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_network_init](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga84542023693d61e8564c5d457979c932) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

进行网络组件的全局初始化。

这是可选的，但建议，因为它避免了隐含地为每个会话进行设置的开销。


如果在某些主要版本碰撞中使用网络协议，则调用此功能将变得强制。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avformat_network_deinit](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga245f2875f80ce67ec3d1e0f54dacf2c4) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

撤消由avformat_network_init完成的初始化。





|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *|[av_iformat_next](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga64ff7d30d32639e69fe57f99bb48f680) (const [AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *f)|
|----|----|

如果f为NULL，则返回第一个注册的输入格式，如果f为非NULL，则在f之后返回下一个注册的输入格式，如果f为最后一个则返回NULL。





|[AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *|[av_oformat_next](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga8fe016b9e8df67a5ffde855f2f891d71) (const [AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *f)|
|----|----|

如果f为NULL，则返回第一个注册的输出格式，如果f为非NULL，则在f之后返回下一个注册的输出格式，如果f为最后一个则返回NULL。





|[AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *|[avformat_alloc_context](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gac7a91abf2f59648d995894711f070f62) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVFormatContext。

avformat_free_context（）可用于释放上下文以及框架内的所有内容。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avformat_free_context](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gac2990b13b68e831a408fce8e1d0d6445) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

释放AVFormatContext及其所有流。

参数：s：释放上下文





|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[avformat_get_class](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga6030789ce395ef543306bbe933619749) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

获取AVCormatContext的AVClass。

它可以与AV_OPT_SEARCH_FAKE_OBJ结合使用来检查选项。






|[AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *|[avformat_new_stream](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gadcb0fd3e507d9b58fe78f61f8ad39827) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

向媒体文件添加新的流。

当分解时，它由read_header（）中的解分配器调用。 如果标志AVFMTCTX_NOHEADER在s.ctx_flags中设置，那么也可以在read_packet（）中调用它。


当复用时，应由用户调用avformat_write_header（）之前。


用户需要调用avcodec_close（）和avformat_free_context（）才能清除avformat_new_stream（）的分配。


参数：s：媒体文件句柄

  c：如果不为空，则将新对应的AVCodecContext初始化为使用此编解码器。 这是需要的。 编解码器特定默认设置，因此如果已知编解码器应该提供。

返回：新创建的流或NULL错误。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_stream_add_side_data](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga5c8d715704c153cb645982444063cc2b) ([AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *st, enum [AVPacketSideDataType](http://ffmpeg.org/doxygen/trunk/group__lavc__packet.html#ga9a80bfcacc586b483a973272800edb97)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), size_t [size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

将现有阵列作为流侧数据进行包装。

参数：st：流

  type：侧信息类型

  data：边数据数组。 必须使用av_malloc（）函数族分配。 数据的所有权转移到st。

  size：侧信息大小。

返回：成功为零，失败时出现负ERROR代码。 在失败时，流不变，数据仍然由呼叫者拥有。





|[uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *|[av_stream_new_side_data](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gae324697cedd36e7b47a1e142dc24b805) ([AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *stream, enum [AVPacketSideDataType](http://ffmpeg.org/doxygen/trunk/group__lavc__packet.html#ga9a80bfcacc586b483a973272800edb97)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

从流分配新信息。

参数：stream：流

  type：所需侧信息类型

  size：侧信息大小

返回：指向新鲜分配的数据，否则为NULL





|[uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *|[av_stream_get_side_data](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#ga2c3404067d7e9bc7b8443e3f58cf8e65) (const [AVStream](http://ffmpeg.org/doxygen/trunk/structAVStream.html) *stream, enum [AVPacketSideDataType](http://ffmpeg.org/doxygen/trunk/group__lavc__packet.html#ga9a80bfcacc586b483a973272800edb97)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

从流中获取边信息。

参数：stream：流

  type：所需侧信息类型

  size： 用于存储的信息大小的指针（可选）

返回：指向数据的指针，否则为NULL





|[AVProgram](http://ffmpeg.org/doxygen/trunk/structAVProgram.html) *|[av_new_program](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gab31f7c7c99dcadead38e8e83e0fdb828) ([AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[id](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a92ba2b3ed65ae251e69195d3ebe639b6))|
|----|----|




