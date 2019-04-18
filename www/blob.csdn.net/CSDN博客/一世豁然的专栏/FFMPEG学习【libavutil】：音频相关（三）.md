# FFMPEG学习【libavutil】：音频相关（三） - 一世豁然的专栏 - CSDN博客





2017年07月29日 10:40:12[一世豁然](https://me.csdn.net/Explorer_day)阅读数：300标签：[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/Explorer_day/article/category/6289310)









一、Audio sample formats

音频采样格式列举和相关便利功能。








一）、枚举

音频采样格式


1、由样本格式描述的数据始终是本机端序列。 样本值可以由本机C类型表示，因此即使是常见的原始音频数据格式，也缺少签名的24位采样格式。

2、浮点格式基于满量程范围[-1.0,1.0]。 超出该范围的任何值都超出了完整的音量。

3、在av_samples_fill_arrays（）和FFmpeg其他位置（如libavcodec中的AVFrame）中使用的数据布局如下所示：

 对于平面样本格式，每个音频通道位于单独的数据平面中，并且linesize是单个平面的缓冲区大小（以字节为单位）。 所有数据平面的大小必须相同。 对于压缩样本格式，仅使用第一个数据平面，并且每个通道的采样交错。 在这种情况下，linesize是1平面的缓冲区大小（以字节为单位）。


|enum|[AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) { [AV_SAMPLE_FMT_NONE](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5ac5d4bb5b0fb0dc7316fb3c7a73029912) = -1, [AV_SAMPLE_FMT_U8](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5adf0f66f21492f8fd5db6debb8b3f932a), [AV_SAMPLE_FMT_S16](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5aea6132df57aebc3f76e10665395c46af), [AV_SAMPLE_FMT_S32](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5ab7ae6bb6718c07cd316778783a57a63b), [AV_SAMPLE_FMT_FLT](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5a5251d62cc3c7f18e56bc8e0c48a27903), [AV_SAMPLE_FMT_DBL](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5ae35a2a6a7593723d9fe4bcefb2876606), [AV_SAMPLE_FMT_U8P](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5a6ab0109a23af37e29ea3a7282c00d311), [AV_SAMPLE_FMT_S16P](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5a35eaaad9da207aa4e63fa02fd67fae68), [AV_SAMPLE_FMT_S32P](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5aa215d25dfbf927aa362c837fed4b15a8), [AV_SAMPLE_FMT_FLTP](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5a2d21c520e4ab21100b6440ec2d81ba1b), [AV_SAMPLE_FMT_DBLP](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5adf1348382a7c7473a425e05530989ae5), [AV_SAMPLE_FMT_S64](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5a59a56ed0ec7b70ecda41c5ae943f256e), [AV_SAMPLE_FMT_S64P](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5ae65e3ba3ffdf813b45fccab87f3588a1), [AV_SAMPLE_FMT_NB](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ggaf9a51ca15301871723577c730b5865c5a8c2ef4962274b33e2828fb9629c104fd)}|
|----|----|



*AV_SAMPLE_FMT_NB*

Number of sample formats. DO NOT USE if linking dynamically.





二）、函数


|const char *|[av_get_sample_fmt_name](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga31b9d149b2de9821a65f4f5612970838) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

返回sample_fmt的名称，如果sample_fmt无法识别，则返回NULL。





|enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5)|[av_get_sample_fmt](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga655c989b749667468e5e839e26fe63db) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

返回与名称相对应的示例格式，或者错误地返回AV_SAMPLE_FMT_NONE。





|enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5)|[av_get_alt_sample_fmt](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga8e66c3f813ea02afcaa56a8a964c749a) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[planar](http://ffmpeg.org/doxygen/trunk/libswresample_2audioconvert_8c.html#a575501d464e41fe8ddc1704a7a3d1350))|
|----|----|

返回给定样本格式的平面< - >打包替代形式，或者错误地返回AV_SAMPLE_FMT_NONE。

如果传递的sample_fmt已经处于请求的平面/打包格式，则返回的格式与输入相同。






|enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5)|[av_get_packed_sample_fmt](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga7817ec0eff4dc6fc0962f31e6d138bca) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

获取给定样品格式的打包替代形式。

如果传递的sample_fmt已经是打包格式，则返回的格式与输入相同。


返回：给定样品格式的包装替代形式或AV_SAMPLE_FMT_NONE错误。





|enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5)|[av_get_planar_sample_fmt](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga82caf838259d95cc6c4fd87633bb0e19) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

获取给定样本格式的平面替代形式。

如果传递的sample_fmt已经是平面格式，返回的格式与输入相同。


返回：给定样本格式的平面替代形式或AV_SAMPLE_FMT_NONE错误。





|char *|[av_get_sample_fmt_string](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gabdc8c2172c33fe9d36cbedaf3c5f83bb) (char *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) buf_size, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

使用sample_fmt生成与样本格式对应的字符串，如果sample_fmt为负，则生成标题。

参数：buf：缓冲区在哪里写字符串

buf_size：buf的大小

sample_fmt：打印相应信息字符串的样本格式的编号，或打印相应标题的负值。

返回：指向已填充缓冲区的指针，如果sample_fmt未知或其他错误则为NULL





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_bytes_per_sample](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga0c3c218e1dd570ad4917c69a35a6c77d) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

返回每个样本的字节数。

参数：sample_fmt：示例格式

返回：每个样本的字节数或零（如果未知的给定样本格式）





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_sample_fmt_is_planar](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga06ba8a64dc4382c422789a5d0b6bf592) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

检查样品格式是否平面。

参数：sample_fmt：样本格式进行检查

返回：1如果采样格式是平面的，如果是交错则为0





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_samples_get_buffer_size](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaa7368bc4e3a366b688e81938ed55eb06) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *linesize, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[align](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a262810794572f14d0d17ef53f17dfecc))|
|----|----|

获取给定音频参数所需的缓冲区大小。

参数：linesize：计算的linesize，可能为NULL

   channels：渠道数量

nb_samples：他在单个通道中的样本数

sample_fmt：示例格式

   align：缓冲区大小对齐（0 =默认，1 =无对齐）

返回：所需缓冲区大小或失败时的负错误代码







二、Samples manipulation

操作音频样本的函数


一）、函数


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_samples_fill_arrays](http://ffmpeg.org/doxygen/trunk/group__lavu__sampmanip.html#ga673254a62fa75dab3211caf17c73c6eb) ([uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **audio_data, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *linesize, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[align](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a262810794572f14d0d17ef53f17dfecc))|
|----|----|

填写平面数据指针，并使用样本格式sample_fmt对样本进行换行。

audio_data数组用样本数据平面的指针填充：对于平面，设置缓冲区内每个通道的数据的起始点，以便打包，仅设置整个缓冲区的起始点。


linesize指向的值设置为每个通道的平面布局的数据缓冲区的对齐大小，或者设置为用于打包布局的所有通道的缓冲区的对齐大小。


buf中的缓冲区必须足够大以包含所有样本（使用av_samples_get_buffer_size（）计算其最小大小），否则audio_data指针将指向无效数据。


查看：枚举AVSampleFormat AVSampleFormat的文档描述了数据布局。

参数：audio_data：数组将填充每个通道的指针

linesize：计算的linesize，可能为NULL

   buf：指向包含样本的缓冲区的指针

   nb_channels：渠道数量

nb_samples：单个通道中的采样数

sample_fmt：采样格式

 
  align：缓冲区大小对齐（0 =默认，1 =无对齐）

返回：>
 = 0成功或失败的负错误代码

Todo：在下一个凸块成功的情况下返回缓冲区所需的最小大小（以字节为单位）





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_samples_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__sampmanip.html#ga4db4c77f928d32c7d8854732f50b8c04) ([uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **audio_data, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *linesize, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[align](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a262810794572f14d0d17ef53f17dfecc))|
|----|----|

为nb_samples样本分配样本缓冲区，并相应填充数据指针和linesize。

分配的采样缓冲区可以通过使用av_freep（＆audio_data [0]）来释放。分配的数据将被初始化为静音。


查看：枚举AVSampleFormat AVSampleFormat的文档描述了数据布局。

参数：audio_data：数组将填充每个通道的指针

linesize：音频缓冲区的对齐大小可能为NULL

   nb_channels：音频通道数

   sample_fmt：每个通道的采样数

   align：缓冲区大小对齐（0 =默认，1
 =无对齐）

返回：> = 0成功或失败的负错误代码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_samples_alloc_array_and_samples](http://ffmpeg.org/doxygen/trunk/group__lavu__sampmanip.html#gae8b9f22f2f659d023f111390d9545273) ([uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) ***audio_data, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *linesize, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[align](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a262810794572f14d0d17ef53f17dfecc))|
|----|----|

分配数据指针数组，nb_samples样本的样本缓冲区，并填充数据指针并相应地进行排序。

这与av_samples_alloc（）相同，但也分配数据指针数组。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_samples_copy](http://ffmpeg.org/doxygen/trunk/group__lavu__sampmanip.html#ga5fdc6b1118f3ba7ad9ac9f67308b9597) ([uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **dst, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *const *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dst_offset, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) src_offset, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

将样本从src复制到dst。

参数：dst：指向数据计划的目标数组

   src：源数组指向数据计划

   dst_offset：将数据写入dst的样本中的偏移量

   src_offset：从src读取数据的样本中的偏移量

   nb_samples:要复制的样本数

   nb_channels:音频通道数

   sample_fmt：音频采样格式





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_samples_set_silence](http://ffmpeg.org/doxygen/trunk/group__lavu__sampmanip.html#ga6f287e5c4f426a8be8a8b5b8f708a05c) ([uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **audio_data, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[offset](http://ffmpeg.org/doxygen/trunk/vf__uspp_8c.html#a06b941e27d7180a9a7c906f9907682d6), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt)|
|----|----|

用静音填充音频缓冲区。

参数：audio_data:数组指针数组

   offset:在开始填充的样品中的偏移量

   nb_samples:要填充的样品数量

   nb_channels:音频通道数

   sample_fmt:音频样本格式



