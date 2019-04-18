# FFMPEG学习【libavcodec】：实用函数（二） - 一世豁然的专栏 - CSDN博客





2017年06月09日 15:43:29[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2056








一、函数


|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_set_dimensions](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga4e17541c4f237465b50668d279192da8) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[width](http://ffmpeg.org/doxygen/trunk/tests_2utils_8c.html#a2474a5474cbff19523a51eb1de01cda4), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[height](http://ffmpeg.org/doxygen/trunk/tests_2utils_8c.html#ad12fc34ce789bce6c8a05d8a17138534))|
|----|----|



|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02) size_t|[av_get_codec_tag_string](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaf91030a62b10ff4446fcf35c1a8eca8d) (char *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), size_t buf_size, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) codec_tag)|
|----|----|

在buf中放置一个表示codec标签的字符串。

参数：buf：缓冲区放置编解码器标签

  size：缓冲字节大小

codec_tag： 编解码器标签进行分配

返回：如果有足够的空间可用，将生成的字符串的长度，不包括尾随的null





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_string](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga6d4056568b5ab73d2e55800d9a5caa66) (char *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) buf_size, [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *enc, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[encode](http://ffmpeg.org/doxygen/trunk/encode__video_8c.html#a56739f6d2e501c2388279fcde2078ac6))|
|----|----|



|const char *|[av_get_profile_name](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga0dc4a8003d0818a235bde2d8fbe81e11) (const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[profile](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a41cdb4a229a3d71837d607124f8a07a4))|
|----|----|

返回指定配置文件的名称（如果有）。

参数：codec：搜索给定配置文件的编解码器

  profile：请求名称的配置文件值

返回：找到配置文件的名称，否则返回空。





|const char *|[avcodec_profile_name](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga7f9d1f2fbf0978fc1ddc2fe196f29fa4) (enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[codec_id](http://ffmpeg.org/doxygen/trunk/rtp_8c.html#a0fd12851a78b80080e277db30411728d), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[profile](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a41cdb4a229a3d71837d607124f8a07a4))|
|----|----|

返回指定配置文件的名称（如果有）。

参数：codec_id：请求的配置文件所属的编解码器的ID

  profile：请求名称的配置文件值

返回：找到配置文件的名称，否则为空。

注意：不同于av_get_profile_name（），它搜索特定解码器或编码器实现支持的配置文件列表，该功能将从AVCodecDescriptor中搜索配置文件列表





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_default_execute](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga9ccd730ba6e891b6c61da68ab138c63f) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(*[func](http://ffmpeg.org/doxygen/trunk/checkasm_8c.html#af406187a441c51e4589704fae05f15fe))([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[c2](http://ffmpeg.org/doxygen/trunk/murmur3_8c.html#a3b64fdbd8e90e5f4782ca75d197e9e5c), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *arg2), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[arg](http://ffmpeg.org/doxygen/trunk/libavcodec_2jacosubdec_8c.html#ac3a73711fd7f8a50db1c4b62e41cf852), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *ret, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_default_execute2](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga365b89d7f5b6dc9f930f07ec4078a611) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(*[func](http://ffmpeg.org/doxygen/trunk/checkasm_8c.html#af406187a441c51e4589704fae05f15fe))([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[c2](http://ffmpeg.org/doxygen/trunk/murmur3_8c.html#a3b64fdbd8e90e5f4782ca75d197e9e5c), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *arg2, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[arg](http://ffmpeg.org/doxygen/trunk/libavcodec_2jacosubdec_8c.html#ac3a73711fd7f8a50db1c4b62e41cf852), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *ret, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2))|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_fill_audio_frame](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga66b44aad56fa22a78fb8916eb8cc0516) ([AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) buf_size, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[align](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a262810794572f14d0d17ef53f17dfecc))|
|----|----|

填充AVFrame音频数据并对指针进行linesize。

缓冲区buf必须是预分配的缓冲区，其大小足够容纳指定的样本量。 填充的AVFrame数据指针将指向此缓冲区。


如果需要平面音频，则分配AVFrame扩展数据通道指针。


参数：frame：必须在调用该函数之前设置AVFrame frame-> nb_samples。 此函数填充frame-> data，frame-> extended_data，frame-> linesize [0]。

  nb_channels：管道数量

sample_fmt：采样格式

  buf：缓冲区用于帧数据

buf_size：缓冲大小

  align：平面尺寸样本对齐（0 =默认）

返回：> = 0成功，否则失败返回错误代码

在下一个libavutil碰撞时返回存储样品所需的大小（以字节为单位）






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_flush_buffers](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaf60b0e076f822abcb2700eb601d352a6) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx)|
|----|----|

重置内部解码器状态/刷新内部缓冲区。

应该调用比如：在寻找或切换到不同的流时。


注意：当未计数的帧被使用（即，avctx-> refcounted_frames为0）时，这使得先前从解码器返回的帧无效。 当使用refcounted帧时，解码器只是释放它可能在内部保留的任何引用，但调用者的引用仍然有效。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_bits_per_sample](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga0717e7440fdb123f708082f66cd9b41d) (enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[codec_id](http://ffmpeg.org/doxygen/trunk/rtp_8c.html#a0fd12851a78b80080e277db30411728d))|
|----|----|

每个样本返回编解码器位。

参数：[in]codec_id：编解码器


返回：每个样本的位数或零，如果给定的编解码器未知。





|enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)|[av_get_pcm_codec](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gafa5775a0bd6a6ab58011e8492156eb6e) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5)[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) be)|
|----|----|

返回与采样格式相关联的PCM编解码器。

参数：be：字符串，0为小，1为大，-1（或任何其他）为本机

返回：AV_CODEC_ID_PCM_* 或 AV_CODEC_ID_NONE





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_exact_bits_per_sample](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gadddbcd146ffaf53016bac730c631f7b3) (enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[codec_id](http://ffmpeg.org/doxygen/trunk/rtp_8c.html#a0fd12851a78b80080e277db30411728d))|
|----|----|

每个样本返回编解码器位。

如果每个样本的位完全正确，而不是近似，则返回非零。


参数：[in]codec_id：编解码器

返回：每个样本的位数或零，如果给定的编解码器未知。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_audio_frame_duration](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga3266a8c3df0790c62259f91afcde45a9) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) frame_bytes)|
|----|----|

返回音频帧持续时间。

参数：avctx：编解码上下文

frame_bytes：帧大小，如果未知，则为0

返回：如果已知，样本中的帧持续时间。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_audio_frame_duration2](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga1f4495d5092c1f58283ff9a16352b4e1) ([AVCodecParameters](http://ffmpeg.org/doxygen/trunk/structAVCodecParameters.html) *par, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) frame_bytes)|
|----|----|

此功能与av_get_audio_frame_duration（）相同，但它与AVCodecParameters而不是AVCodecContext配合使用。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_register_bitstream_filter](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga6d8f7eec28df88d590eb3b3d865cf0ca) ([AVBitStreamFilter](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilter.html) *bsf)|
|----|----|

注册比特流筛选器

过滤器可以通过av_bitstream_filter_next（）访问应用程序代码，也可以使用av_bitstream_filter_init（）直接初始化。






|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[AVBitStreamFilterContext](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilterContext.html) *|[av_bitstream_filter_init](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga511d9ec0593e70035813492a7bb1d390) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

给定比特流过滤器名称创建并初始化比特流过滤器上下文。

必须使用av_bitstream_filter_close（）来释放返回的上下文。


参数：name：比特流过滤器的名称

返回：如果找到匹配的过滤器并成功初始化了比特流过滤器上下文，否则为NULL





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bitstream_filter_filter](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga8aa30391ba53516321c69ec0cea34e39) ([AVBitStreamFilterContext](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilterContext.html) *bsfc, [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *avctx, const char *[args](http://ffmpeg.org/doxygen/trunk/avisynth__c_8h.html#a68026e8546814670dd0cd181ed2308f9), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **poutbuf, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *poutbuf_size, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) buf_size, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) keyframe)|
|----|----|

过滤比特流

此功能可使用大小为buf_size的缓冲区buf进行过滤，并将过滤的缓冲区放在poutbuf指向的缓冲区中。


输出缓冲区必须由调用者释放。


参数：bsfc：由av_bitstream_filter_init（）创建的比特流过滤器上下文

avctx:过滤器访问的AVCodecContext可能为NULL。 如果指定，则必须指向数据包发送到的输出流的编码器上下文。

  args:指定过滤器配置的参数可以为NULL

poutbuf:指针被更新为指向经过滤的缓冲区

poutbuf_size:指针以字节更新为已过滤的缓冲区大小

  buf:缓冲区包含要过滤的数据

  buf_size:buf大小（以字节为单位）

keyframe：如果缓冲区过滤器对应于关键帧分组数据，则设置为非零

返回：> = 0，否则为错误代码

如果返回值为正，则输出缓冲区被分配并在*
 poutbuf中可用，并且与输入缓冲区不同。


如果返回值为0，则输出缓冲区不被分配，并且应该被视为与输入缓冲区相同，或者在设置了*
 poutbuf的情况下，它指向输入缓冲区（不一定在其起始地址）。 一个特殊情况是如果* poutbuf设置为NULL，并且* poutbuf_size设置为0，表示应该丢弃数据包。






|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_bitstream_filter_close](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga294d399ec25bf1fd3c92ccd085654c76) ([AVBitStreamFilterContext](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilterContext.html) *bsf)|
|----|----|

释放比特流过滤器上下文。

参数：bsf：使用av_bitstream_filter_init（）创建的比特流过滤器上下文可以为NULL





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[AVBitStreamFilter](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilter.html) *|[av_bitstream_filter_next](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gab03098c0e4139e972dfbc96a6485ce67) (const [AVBitStreamFilter](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilter.html) *f)|
|----|----|

如果f为NULL，则返回第一个注册的比特流过滤器，如果f不为空，则在f之后返回下一个注册的比特流过滤器，如果f为最后一个则返回NULL。

该函数可用于遍历所有注册的比特流过滤器。






|const [AVBitStreamFilter](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilter.html) *|[av_bsf_get_by_name](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gae491493190b45698ebd43db28c4e8fe9) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

返回：具有指定名称的比特流筛选器，如果不存在此类比特流筛选器，则为NULL。





|const [AVBitStreamFilter](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilter.html) *|[av_bsf_next](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga7437884db8ee577d121a94069420c199) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **opaque)|
|----|----|

迭代所有注册的比特流过滤器。

参数：opaque：一个指针，其中libavcodec将存储迭代状态。 必须指向NULL才能开始迭代。

返回：下一个注册的比特流过滤器或迭代结束时为NULL





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_alloc](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga7da65af303e20c9546e15ec266b182c1) (const [AVBitStreamFilter](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilter.html) *[filter](http://ffmpeg.org/doxygen/trunk/vf__yadif_8c.html#a4aac1797de0c2fcdc5b2b2b3191d252d), [AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) **[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

为给定的比特流过滤器分配上下文。

调用者必须按照文档中所述填写上下文参数，然后在将任何数据发送到过滤器之前调用av_bsf_init（）。


参数：filter：为其分配实例的过滤器。

  ctx：指向新分配的上下文的指针的指针。 过滤完成后，必须先释放av_bsf_free（）。

返回：0成功，失败时返回负ERROR代码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_init](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga242529d54013acf87e94273d298a5ff2) ([AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

在设置完所有参数和选项后，准备使用过滤器。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_send_packet](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaada9ea8f08d3dcf23c14564dbc88992c) ([AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090))|
|----|----|

提交数据包进行过滤。

发送每个数据包后，必须通过重复调用av_bsf_receive_packet（）才能完全排除过滤器，直到它返回AVERROR（EAGAIN）或AVERROR_EOF。


参数：pkt：包过滤。 比特流过滤器将获取数据包的所有权并重置pkt的内容。 如果发生错误，pkt不会被触摸。 该参数可以是NULL，其指示流的结束（即，不再发送分组）。 这将导致过滤器输出其内部缓冲的任何数据包。

返回：0成功，否则返回AVERROR出错。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_receive_packet](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga7fffb6c87b91250956e7a2367af56b38) ([AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090))|
|----|----|

检索已过滤的数据包。

参数：[out]pkt：这个结构将被填充过滤的数据包的内容。 它由调用者拥有，当不再需要时，必须使用av_packet_unref（）来释放它。 当调用此函数时，此参数应为“干净”（即，使用av_packet_alloc（）新分配或与av_packet_unref（））不相关。 如果此函数成功返回，则pkt的内容将被返回的数据完全覆盖。 失败时，pkt没有被触动。

返回：0成功。 如果需要将更多的数据包发送到过滤器（使用av_bsf_send_packet（））获得更多的输出，则AVERROR（EAGAIN）。 如果过滤器没有进一步的输出，则为AVERROR_EOF。 如果发生错误，则另一个负AVERROR值。

注意：一个输入数据包可能会导致多个输出数据包，因此在使用av_bsf_send_packet（）发送数据包后，需要重复调用该函数，直到它停止返回0.滤波器也可以输出比发送给它的数据包更少的数据包， 所以这个功能可以在av_bsf_send_packet（）调用成功后立即返回AVERROR（EAGAIN）。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_bsf_free](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga08d53431e76355f88e27763b1940df4f) ([AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) **[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

释放比特流过滤器上下文和与之相关的一切; 将NULL写入提供的指针。





|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[av_bsf_get_class](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaa0f3b8733cad7ddfeb277d8d323de257) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

获取AVClass AVBSFContext。

它可以与AV_OPT_SEARCH_FAKE_OBJ结合使用来检查选项。






|[AVBSFList](http://ffmpeg.org/doxygen/trunk/structAVBSFList.html) *|[av_bsf_list_alloc](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga02cc7758126d74276d9dc835eb423853) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配空列表的比特流过滤器。

该列表必须稍后由av_bsf_list_free（）释放，或由av_bsf_list_finalize（）完成。


返回：指向AVBSFList成功时，在发生故障时为NULL





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_bsf_list_free](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga0e144e330612e37a23acf056db4ac220) ([AVBSFList](http://ffmpeg.org/doxygen/trunk/structAVBSFList.html) **lst)|
|----|----|

释放比特流过滤器的列表。

参数：lst：指向av_bsf_list_alloc（）返回的指针





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_list_append](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga51caad27662682e124796bbff590cab8) ([AVBSFList](http://ffmpeg.org/doxygen/trunk/structAVBSFList.html) *lst, [AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) *bsf)|
|----|----|

将比特流筛选器附加到比特流筛选器列表。

参数：lst：列表附加到

  bsf：过滤上下文要附加

返回：> = 0成功，否则返回AVERROR在失败的情况下






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_list_append2](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaf9085d5cf405b89e2129dc6b2ef54aeb) ([AVBSFList](http://ffmpeg.org/doxygen/trunk/structAVBSFList.html) *lst, const char *bsf_name, [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

构造新的比特流过滤器上下文，因为它是名称和选项，并将其附加到比特流筛选器列表。

参数：lst：列表附加到

bsf_name：比特流过滤器的名称

  options：位流过滤器的选项可以设置为NULL

返回：> = 0成功，否则返回AVERROR在失败的情况下






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_list_finalize](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaa4f71b6265b76b23abd418b62ffaff2d) ([AVBSFList](http://ffmpeg.org/doxygen/trunk/structAVBSFList.html) **lst, [AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) **bsf)|
|----|----|

完成比特流筛选器列表。

该函数将AVBSFList转换为单个AVBSFContext，因此可将整个比特流过滤器链视为由av_bsf_alloc（）最新分配的单个过滤器。 如果调用成功，则AVBSFList结构被释放，并且lst将被设置为NULL。 在失败的情况下，调用者负责通过av_bsf_list_free（）释放结构，


参数：lst：要转换的过滤器列表结构

  [out]bsf：指针被设置为新创建的表示比特流过滤器链的AVBSFContext结构

返回：> = 0成功，否则返回AVERROR在失败的情况下





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_list_parse_str](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gacc7bbd4bb41b36893f6ad971fd95f7fb) (const char *str, [AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) **bsf)|
|----|----|

解析字符串描述比特流筛选器列表，并创建描述比特流筛选器的整个链的单个AVBSFContext。

导致AVBSFContext可以被视为由av_bsf_alloc（）最新分配的任何其他AVBSFContext。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_bsf_get_null_filter](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gad7ef51525e9607c2d12c75f3bcbc1388) ([AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html) **bsf)|
|----|----|

获取null /直通比特流过滤器。

参数：[out]bsf：指针被设置为直通比特流过滤器的新实例





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_fast_padded_malloc](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga04a23a2336412504052d5c4e0c7716ee) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), size_t min_size)|
|----|----|

与av_fast_malloc有相同的行为但缓冲区附加AV_INPUT_BUFFER_PADDING_SIZE在结尾将始终为0。

此外，整个缓冲区将在初始化之后和之后调整大小为0初始化，以便不会出现未初始化的数据。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_fast_padded_mallocz](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaea5550dc9271f356631f7002772158d8) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), size_t min_size)|
|----|----|

与av_fast_padded_malloc相同的行为，除了缓冲区将在调用后始终为0初始化。





|unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_xiphlacing](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga144411ccbce77847ee9ce10d7fe5361c) (unsigned char *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) v)|
|----|----|

将缓冲区的长度编码到缓冲区。

由xiph编解码器使用。


参数：s：缓冲区写入; 必须至少（v / 255 + 1）个字节长

  v：extradata的大小（以字节为单位）

返回：写入缓冲区的字节数。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_log_missing_feature](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga8df73458e977b4a0f9d8e4f1eeae47f1) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *avc, const char *feature, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) want_sample)|
|----|----|

记录关于缺少的功能的通用警告消息。

此函数仅用于FFmpeg（libavcodec，libavformat等）内部使用，通常不会被应用程序使用。


参数：avc：指向一个任意结构的指针，第一个字段是一个指向AVClass结构体的指针

feature：包含缺少的功能的名称的字符串

want_sample：指示是否需要展示此功能的样品。 如果want_sample不为零，则会向日志消息中添加额外的语句，该消息告诉用户如何将样本报告给开发邮件列表。

已废弃：请改用avpriv_report_missing_feature（）。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_log_ask_for_sample](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga90d24e73ca8633fc9e42c4bf2faf7923) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *avc, const char *msg,...) [av_printf_format](http://ffmpeg.org/doxygen/trunk/rtpproto_8c.html#a592f4a5e13fd709e6e0bc93a3131612d)(2|
|----|----|

记录一个通用的警告消息，要求一个样本。

此函数仅用于FFmpeg（libavcodec，libavformat等）内部使用，通常不会被应用程序使用。


参数：[in]avc：指向一个任意结构的指针，第一个字段是一个指向AVClass结构体的指针

  [in]msg：包含可选消息的字符串，如果没有消息，则为NULL





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_register_hwaccel](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga1e7e3fd30164d0af5139885404fd2407) ([AVHWAccel](http://ffmpeg.org/doxygen/trunk/structAVHWAccel.html) *hwaccel)|
|----|----|

注册硬件加速器hwaccel。





|[AVHWAccel](http://ffmpeg.org/doxygen/trunk/structAVHWAccel.html) *|[av_hwaccel_next](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gacd85ab0da8585474814590a81d011d5c) (const [AVHWAccel](http://ffmpeg.org/doxygen/trunk/structAVHWAccel.html) *hwaccel)|
|----|----|

如果hwaccel为NULL，则返回第一个注册的硬件加速器，如果hwaccel为非NULL，则在hwaccel之后返回下一个已注册的硬件加速器，如果hwaccel为最后一个则返回NULL。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_lockmgr_register](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gae120ec60cdf49cf0f793eff3ec731bcc) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(*[cb](http://ffmpeg.org/doxygen/trunk/vf__geq_8c.html#afc61272a374024bcd86d8859783d3d52))([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **[mutex](http://ffmpeg.org/doxygen/trunk/w32pthreads_8h.html#a8647861982974418fd65cf3dca69c54e), enum [AVLockOp](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga7a013315e444069ec9630ddf3edce3f8)[op](http://ffmpeg.org/doxygen/trunk/libavcodec_2anm_8c.html#ac14d0b5f175e1c1c06f33693d8567695)))|
|----|----|

注册一个用户提供的锁管理器，支持AVLockOp指定的操作。

函数的“mutex”参数指向一个（void *），其中lockmgr应该存储/获取指向分配的互斥体的用户的指针。 它在AV_LOCK_CREATE时为NULL，并且等于最后一次调用所有其他操作的值。 如果锁管理器无法执行操作，那么它应该使互斥体处于与调用时相同的状态，并返回非零值。 但是，当使用AV_LOCK_DESTROY调用时，互斥量将始终被假定已被成功销毁。 如果av_lockmgr_register成功，它将返回一个非负值，如果它失败，它将返回一个负值，并销毁所有互斥量并注销所有回调。 av_lockmgr_register不是线程安全的，它必须在使用使用锁的任何调用之前从单个线程调用。


参数：cb：用户定义回调。 av_lockmgr_register调用此回调和以前注册的回调。 回调将用于创建多个互斥体，每个互斥体必须由其自己的底层锁定机制支持（即不要使用单个静态对象来实现您的锁管理器）。 如果cb设置为NULL，则lockmgr将被注销。





|enum [AVMediaType](http://ffmpeg.org/doxygen/trunk/group__lavu__misc.html#ga9a84bba4713dfced21a1a56163be1f48)|[avcodec_get_type](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga9680ab906f1206deafc521ed54927375) (enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[codec_id](http://ffmpeg.org/doxygen/trunk/rtp_8c.html#a0fd12851a78b80080e277db30411728d))|
|----|----|

获取给定编解码器的类型。





|const char *|[avcodec_get_name](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga2016a52e94f867ebe5113bdf448e182d) (enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[id](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a92ba2b3ed65ae251e69195d3ebe639b6))|
|----|----|

获取编解码器的名称。

返回：标识编解码器的静态字符串; 从不NULL





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_is_open](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga906dda732e79eac12067c6d7ea19b630) ([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

返回：如果s是打开的（即，avcodec_open2（）被调用，没有相应的avcodec_close（）），则为正值，否则为0。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_codec_is_encoder](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga2b665824e4d9144f8d4f6c01e3e85aa3) (const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec)|
|----|----|

返回：如果编解码器是编码器，则为非零，否则为零





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_codec_is_decoder](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga6dc18eef1afca3610644a52565cf8a31) (const [AVCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html) *codec)|
|----|----|

返回：如果编解码器是解码器，则为非零，否则为零





|const [AVCodecDescriptor](http://ffmpeg.org/doxygen/trunk/structAVCodecDescriptor.html) *|[avcodec_descriptor_get](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gac09f8ddc2d4b36c5a85c6befba0d0888) (enum [AVCodecID](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)[id](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a92ba2b3ed65ae251e69195d3ebe639b6))|
|----|----|

返回：给定编解码器ID的描述符，如果没有描述符，则为NULL。





|const [AVCodecDescriptor](http://ffmpeg.org/doxygen/trunk/structAVCodecDescriptor.html) *|[avcodec_descriptor_next](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga909ef5261eafb1be1871f718353632bd) (const [AVCodecDescriptor](http://ffmpeg.org/doxygen/trunk/structAVCodecDescriptor.html) *prev)|
|----|----|

迭代所有libavcodec已知的编解码器描述符。

参数：prev：先前的描述符。 NULL表示获取第一个描述符。

返回：下一个描述符或若是最后一个描述符后，返回NULL





|const [AVCodecDescriptor](http://ffmpeg.org/doxygen/trunk/structAVCodecDescriptor.html) *|[avcodec_descriptor_get_by_name](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga199f854b030f24bd31d251f9d36f1392) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

返回：具有给定名称的编解码器描述符，如果不存在这样的描述符，则为NULL。





|[AVCPBProperties](http://ffmpeg.org/doxygen/trunk/structAVCPBProperties.html) *|[av_cpb_properties_alloc](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga8686e21fb057cdbd5d5078c9822a726d) (size_t *[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

分配CPB属性结构，并将其字段初始化为默认值。

参数：size：如果非NULL，则分配的结构体的大小将在此处写入。 这对于将其嵌入在侧面数据中是有用的。

返回：新分配的结构或失败返回NULL



