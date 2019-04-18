# FFMPEG学习【libavcodec】：核心函数以及结构体（二） - 一世豁然的专栏 - CSDN博客





2017年06月08日 14:54:22[一世豁然](https://me.csdn.net/Explorer_day)阅读数：721








一、函数


|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_codec_get_pkt_timebase](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga99afa816f3fc9251ecf54ad305d43be3) (const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_codec_set_pkt_timebase](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga89c6c7cf5717165b8751aa84bf397557) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|const [AVCodecDescriptor](http://ffmpeg.org/doxygen/trunk/structAVCodecDescriptor.html) *|[av_codec_get_codec_descriptor](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gad7c67883918a8a68710b6f89f461c37f) (const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_codec_set_codec_descriptor](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gad07ba397392e00cb2560cc9a39a60745) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, const [AVCodecDescriptor](http://ffmpeg.org/doxygen/trunk/structAVCodecDescriptor.html) *[desc](http://ffmpeg.org/doxygen/trunk/qsv_8c.html#a710bce51374aba96ab04912897666c35))|
|----|----|



|unsigned|[av_codec_get_codec_properties](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga32597f790d2e3051e982b332654fdf94) (const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_codec_get_lowres](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gae52334a4870627285d40a77be95dbe85) (const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_codec_set_lowres](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaf4af025ede1f451152486b8f2805f544) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_codec_get_seek_preroll](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga1065b464eb473666a6758d4d7089385e) (const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_codec_set_seek_preroll](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga7b1bfa942a5074882680616694e93c59) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|uint16_t *|[av_codec_get_chroma_intra_matrix](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga5425dce8ae0a9f8ab8ac09b131b083f6) (const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_codec_set_chroma_intra_matrix](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga5e57eb44d2173147901d8415cb85f4d7) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, uint16_t *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_codec_get_max_lowres](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gafda0a87bc29a129cd12e858b4609cccd) (const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec)|
|----|----|



|[AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *|[av_codec_next](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga15369ce1a486dd26a14a043c2b4d7e76) (const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

如果c为NULL，则返回第一个注册的编解码器，如果c为非NULL，则在c之后返回下一个已注册的编解码器，如果c为最后一个，则返回NULL。





|unsigned|[avcodec_version](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gadf72cccead0dc2575adcea2dc3e1ae7c) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回LIBAVCODEC_VERSION_INT常量。





|const char *|[avcodec_configuration](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaba6aaa26147862a94d660a7b21ecd156) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavcodec构建时配置。





|const char *|[avcodec_license](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gae594db5f65482502088cba133ff96352) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavcodec许可证。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_register](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga882513208349dbcea177626d9fd7ed92) ([AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec)|
|----|----|

注册编解码器并初始化libavcodec。

警告：此函数或avcodec_register_all（）必须在任何其他libavcodec函数之前调用。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_register_all](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaf1a2bb4e7c7611c131bb6212bf0fa639) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

注册在配置时启用的所有编解码器，解析器和比特流筛选器。

如果您不调用此函数，您可以使用单独的注册功能来精确选择要支持的格式。






|[AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *|[avcodec_alloc_context3](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gae80afec6f26df6607eaacf39b561c315) (const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec)|
|----|----|

分配AVCodecContext并将其字段设置为默认值。

结果应该使用avcodec_free_context（）来释放。


参数：codec：如果非NULL，则分配专用数据并初始化给定编解码器的默认值。 然后使用不同的编解码器调用avcodec_open2（）是非法的。 如果为NULL，则编解码器特定的默认值将不会被初始化，这可能导致次优的默认设置（这对于编码器，例如libx264）很重要。

返回：AVCodecContext填入默认值或失败时为NULL。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_free_context](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaf869d0829ed607cec3a4a02a1c7026b3) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) **avctx)|
|----|----|

释放编解码器上下文和与之相关联的所有内容，并将NULL写入提供的指针。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_get_context_defaults3](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga1dd4bf43e6a4ec8b8d76bd3673e5e73a) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec)|
|----|----|

遗弃：不应该使用此函数，因为不支持多次打开和打开编解码器上下文。 应为每个新用途分配新的编解码器上下文。





|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[avcodec_get_class](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gae8ab0faa9a07901c48e07e5feee82626) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

获得AVCodecContext AVClass。

它可以与AV_OPT_SEARCH_FAKE_OBJ结合使用来检查选项。






|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[avcodec_get_frame_class](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gad55dfc8e9e168e498ab053b38ba6dec8) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

获取AVClass AVFrame。


它可以与AV_OPT_SEARCH_FAKE_OBJ结合使用来检查选项。






|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[avcodec_get_subtitle_rect_class](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga4a9d3dc85f704795638b7be320fbc357) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

获取AVSubtitleRect的AVClass。

它可以与AV_OPT_SEARCH_FAKE_OBJ结合使用来检查选项。






|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_copy_context](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gae381631ba4fb14f4124575d9ceacb87e) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *dest, const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

将源AVCodecContext的设置复制到目标AVCodecContext中。

所得到的目标编解码器上下文将被打开，即您需要调用avcodec_open2（），然后才能使用此AVCodecContext对视频/音频数据进行解码/编码。


参数：dest：目标编解码器上下文，应使用avcodec_alloc_context3（NULL）初始化，否则将初始化

  src：源编解码器上下文

返回：AVERROR（）发生错误（例如内存分配错误），0成功





|[AVCodecParameters](http://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html) *|[avcodec_parameters_alloc](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga647755ab2252e93221bb345f3d5e414f) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配一个新的AVCodecParameters并将其字段设置为默认值（未知/无效/ 0）。

返回的结构必须与avcodec_parameters_free（）一起释放。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_parameters_free](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga950c8da55b8112077e640b6a0cb8cf36) ([AVCodecParameters](http://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html) **par)|
|----|----|

释放一个AVCodecParameters实例以及与之相关联的所有内容，并向提供的指针写入NULL。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_parameters_copy](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga6d02e640ccc12c783841ce51d09b9fa7) ([AVCodecParameters](http://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html) *dst, const [AVCodecParameters](http://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

将src的内容复制到dst。

dst中的任何分配的字段被释放，并被src中相应字段的新分配的副本替换。


返回：成功返回零，失败返回负值






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_parameters_from_context](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga0c7058f764778615e7978a1821ab3cfe) ([AVCodecParameters](http://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html) *par, const [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *codec)|
|----|----|

根据提供的编解码器上下文中的值填写参数结构。

par中任何分配的字段都被释放，并被编码解码器中相应字段的重复替换。


返回：成功返回零，失败返回负值






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_parameters_to_context](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gac7b282f51540ca7a99416a3ba6ee0d16) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *codec, const [AVCodecParameters](http://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html) *par)|
|----|----|

根据提供的编解码器参数的值填写编解码器上下文。

二编解码器中具有par中对应字段的任何已分配字段将被释放，并替换为参数中相应字段的重复。 编解码器中没有对方的参数中的字段未被触发。


返回：成功返回零，失败返回负值






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_open2](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga11f785a188d7d9df71621001465b0f1d) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec, [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

初始化AVCodecContext以使用给定的AVCodec。

在使用此函数之前，上下文必须使用avcodec_alloc_context3（）分配。


函数avcodec_find_decoder_by_name（），avcodec_find_encoder_by_name（），avcodec_find_decoder（）和avcodec_find_encoder（）提供了一种检索编解码器的简单方法。


警告：这个功能不是线程安全的！

注意：在使用解码例程（如avcodec_receive_frame（））之前，始终调用此函数。

参数：avctx：初始化的上下文。

codec：打开这个上下文的编解码器。 如果非空编解码器先前传递给avcodec_alloc_context3（）或此上下文，则该参数必须为NULL或等于先前传递的编解码器。

  options：填写AVCodecContext和编解码器私有选项的字典。 返回时，此对象将填充未找到的选项。

返回：成功返回零，失败返回负值





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_close](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaf4daa92361efb3523ef5afeb0b54077f) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|

关闭给定的AVCodecContext并释放所有与之相关联的数据（而不是AVCodecContext本身）。

在尚未打开的AVCodecContext上调用此函数将使用非NULL编解码器释放avcodec_alloc_context3（）中分配的编解码器特定数据。 随后的调用什么都不做。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avsubtitle_free](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaa2c3e02a761d9fc0c5c9b2340408c332) ([AVSubtitle](http://ffmpeg.org/doxygen/trunk/structAVSubtitle.html) *sub)|
|----|----|

释放给定字幕结构中所有分配的数据。

参数：sub：用来释放的AVSubtitle



