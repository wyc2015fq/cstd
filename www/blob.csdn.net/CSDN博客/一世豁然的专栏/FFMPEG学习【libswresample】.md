# FFMPEG学习【libswresample】 - 一世豁然的专栏 - CSDN博客





2017年07月29日 16:42:59[一世豁然](https://me.csdn.net/Explorer_day)阅读数：3172








音频重采样，采样格式转换和混合库。


与lswr的交互是通过SwrContext完成的，SwrContext被分配给swr_alloc（）或swr_alloc_set_opts（）。 它是不透明的，所以所有参数必须使用AVOptions API设置。


为了使用lswr，你需要做的第一件事就是分配SwrContext。 这可以使用swr_alloc（）或swr_alloc_set_opts（）来完成。 如果您使用前者，则必须通过AVOptions API设置选项。 后一个函数提供了相同的功能，但它允许您在同一语句中设置一些常用选项。


例如，以下代码将设置从平面浮动样本格式到交织的带符号16位整数的转换，从48kHz到44.1kHz的下采样，以及从5.1声道到立体声的下混合（使用默认混合矩阵）。 这是使用swr_alloc（）函数。




```cpp
SwrContext *swr = swr_alloc();
av_opt_set_channel_layout(swr, "in_channel_layout",  AV_CH_LAYOUT_5POINT1, 0);
av_opt_set_channel_layout(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO,  0);
av_opt_set_int(swr, "in_sample_rate",     48000,                0);
av_opt_set_int(swr, "out_sample_rate",    44100,                0);
av_opt_set_sample_fmt(swr, "in_sample_fmt",  AV_SAMPLE_FMT_FLTP, 0);
av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);
```


同样的工作也可以使用swr_alloc_set_opts（）：



```cpp
SwrContext *swr = swr_alloc_set_opts(NULL,  // we're allocating a new context
                      AV_CH_LAYOUT_STEREO,  // out_ch_layout
                      AV_SAMPLE_FMT_S16,    // out_sample_fmt
                      44100,                // out_sample_rate
                      AV_CH_LAYOUT_5POINT1, // in_ch_layout
                      AV_SAMPLE_FMT_FLTP,   // in_sample_fmt
                      48000,                // in_sample_rate
                      0,                    // log_offset
                      NULL);                // log_ctx
```


一旦设置了所有值，它必须用swr_init（）初始化。 如果需要更改转换参数，可以使用AVOptions来更改参数，如上面第一个例子所述; 或者使用swr_alloc_set_opts（），但是第一个参数是分配的上下文。 您必须再次调用swr_init（）。

转换本身通过重复调用swr_convert（）来完成。 请注意，如果提供的输出空间不足或采样率转换完成后，样本可能会在swr中缓冲，这需要“未来”样本。 可以随时通过使用swr_convert（）（in_count可以设置为0）来检索不需要将来输入的样本。 在转换结束时，可以通过调用具有NULL in和in incount的swr_convert（）来刷新重采样缓冲区。


在转换过程中使用的样本可以使用libavutil示例操作API进行管理，包括以下示例中使用的av_samples_alloc（）函数。


输入和输出之间的延迟可以随时通过使用swr_get_delay（）找到。


以下代码演示了假设上面的参数和调用者定义的函数get_input（）和handle_output（））的转换循环：




```cpp
uint8_t **input;
int in_samples;
while (get_input(&input, &in_samples)) {
    uint8_t *output;
    int out_samples = av_rescale_rnd(swr_get_delay(swr, 48000) +
                                     in_samples, 44100, 48000, AV_ROUND_UP);
    av_samples_alloc(&output, NULL, 2, out_samples,
                     AV_SAMPLE_FMT_S16, 0);
    out_samples = swr_convert(swr, &output, out_samples,
                                     input, in_samples);
    handle_output(output, out_samples);
    av_freep(&output);
}
```


转换完成后，必须使用swr_free（）释放转换上下文和与之相关的所有内容。 也可以使用swr_close（）函数，但它主要是为了与libavresample兼容，不需要调用。

如果在swr_free（）之前没有完全刷新数据，则不会有内存泄漏。





一、函数


|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[swr_get_class](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga6ca0298348b382cf0fe67d7f339a17f3) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

获取 SwrContext的AVClass。

它可以与AV_OPT_SEARCH_FAKE_OBJ结合使用来检查选项。


查看：[av_opt_find()](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gae31ae7fb20113b00108d0ecf53f25664).

返回：SwrContext的AVClass







二、选项常数

这些常量用于lswr的AVOptions接口。



|enum|[SwrDitherType](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga387e613b19e5269a46d9ff1a5ee3fcd4) { [SWR_DITHER_NONE](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4afacc17a82581598f762a8036ba559836) = 0, [SWR_DITHER_RECTANGULAR](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4a0fec2dd28b1357b50e9807ca6ecee83a), [SWR_DITHER_TRIANGULAR](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4ae4635dc355dc680eeb211e7af17ddfd2), [SWR_DITHER_TRIANGULAR_HIGHPASS](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4a91276f2e437aa9d0d8bdffabe24f5532), [SWR_DITHER_NS](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4ae6858a54f5b2e1bffa9aaef20b2593c0) = 64, [SWR_DITHER_NS_LIPSHITZ](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4a6f6072071262f7f05a52057cfe48df87), [SWR_DITHER_NS_F_WEIGHTED](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4ae197d7fc5592652aaa8edf1067c56af9), [SWR_DITHER_NS_MODIFIED_E_WEIGHTED](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4a60eeb8ca5de47916a81f2c3a6534eed4), [SWR_DITHER_NS_IMPROVED_E_WEIGHTED](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4ad13d2ff2831d463eb46ca4c46dfec7b0), [SWR_DITHER_NS_SHIBATA](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4ae3aba4bf43c2210a148b11c0215b5d24), [SWR_DITHER_NS_LOW_SHIBATA](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4af275c21089abbf78a6623544f34d76f9), [SWR_DITHER_NS_HIGH_SHIBATA](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4ac569828a706ccf22a8d5810c973c8f17), [SWR_DITHER_NB](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga387e613b19e5269a46d9ff1a5ee3fcd4aacbdf2f91bb14cf390116492115fb8ea)}|
|----|----|

抖动算法。





|enum|[SwrEngine](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga87f9e023bbe780d3ccf17dfc7abed580) { [SWR_ENGINE_SWR](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga87f9e023bbe780d3ccf17dfc7abed580a11652db5af0c279d79deba1c577c9729), [SWR_ENGINE_SOXR](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga87f9e023bbe780d3ccf17dfc7abed580a2933f5b0b96829095a04a8acd19b27e7), [SWR_ENGINE_NB](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga87f9e023bbe780d3ccf17dfc7abed580a865af2e70ebb85b3cafb93f1854dbef9) }|
|----|----|

重新采样发动机。





|enum|[SwrFilterType](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga2176b2a3a8b809788f6e7ccdc238b6be) { [SWR_FILTER_TYPE_CUBIC](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga2176b2a3a8b809788f6e7ccdc238b6bea1a6413b9888bac851e2ee4f991f01d0c), [SWR_FILTER_TYPE_BLACKMAN_NUTTALL](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga2176b2a3a8b809788f6e7ccdc238b6bea17f5dd4512b4abc2285905293a81cf55), [SWR_FILTER_TYPE_KAISER](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gga2176b2a3a8b809788f6e7ccdc238b6bea8f492a5dc94b6e41a37f190c5492bf3f) }|
|----|----|

重新采样过滤器类型。





|#define|[SWR_FLAG_RESAMPLE](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga2226aca7f49137f3f29b6f731a985916)   1|
|----|----|

强制重采样即使相等采样率。










三、SwrContext构造函数

|struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *|[swr_alloc](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaf58c4ff10f73d74bdab8e5aa7193147c) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配SwrContext


如果使用此函数，则需要在调用swr_init（）之前设置参数（手动或使用swr_alloc_set_opts（））。


参数：[swr_alloc_set_opts()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gad7f29022726c871aadf935281a9813bc), [swr_init()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gae173e8ed91717700471a1dcd06f00f67), [swr_free()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga818f7d78b1ad7d8d5b70de374b668c34)

返回：错误为NULL，否则分配上下文




|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_init](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gae173e8ed91717700471a1dcd06f00f67) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

用户参数设置后初始化上下文。


注意：必须使用AVOption API配置上下文。

查看：[av_opt_set_int()](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#ga3adf7185c21cc080890a5ec02c2e56b2)，[av_opt_set_dict()](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaf1a88e8f152d7835f111b1ef8813fb2e)

参数：s：Swr上下文进行初始化

返回：发生故障时出现AVERROR错误代码。




|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_is_initialized](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gab11b02705e97404b6d74b18b2fcfdfcf) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

检查swr上下文是否已初始化。


参数：s：Swr上下文检查

查看：[swr_init()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gae173e8ed91717700471a1dcd06f00f67)

返回：如果初始化为正，则为0，如果未初始化则为0




|| |
|----|----|
|struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *|[swr_alloc_set_opts](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gad7f29022726c871aadf935281a9813bc) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), int64_t out_ch_layout, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) out_sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) out_sample_rate, int64_t in_ch_layout, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) in_sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) in_sample_rate, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) log_offset, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *log_ctx)|

如果需要，分配SwrContext并设置/重置常见参数。


此函数不需要使用swr_alloc（）分配s。 另一方面，swr_alloc（）可以使用swr_alloc_set_opts（）在分配的上下文上设置参数。


参数：s：现有Swr上下文（如果可用），如果不可用则为NULL

out_ch_layout：输出通道布局（AV_CH_LAYOUT_ *）

out_sample_fmt：输出采样格式（AV_SAMPLE_FMT_ *）。

   out_sample_rate：输出采样率（频率（Hz））

in_ch_layout：输入通道布局（AV_CH_LAYOUT_
 *）

in_sample_fmt：输入样品格式（AV_SAMPLE_FMT_ *）。

in_sample_rate：输入采样率（频率（Hz））

log_offset：记录级别偏移

log_ctx：父记录上下文，可以为NULL

查看：[swr_init()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gae173e8ed91717700471a1dcd06f00f67), [swr_free()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga818f7d78b1ad7d8d5b70de374b668c34)

返回：错误为NULL，否则分配上下文




四、SwrContext析构函数

|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[swr_free](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga818f7d78b1ad7d8d5b70de374b668c34) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) **[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

释放给定的SwrContext并将指针设置为NULL。


参数：s：指向Swr上下文的指针的指针




|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[swr_close](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaa4bf1048740dfc08d68aba9f1b4db22e) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

关闭上下文以使swr_is_initialized（）返回0。


可以通过运行swr_init（）将上下文恢复使用，也可以在不使用swr_close（）的情况下使用swr_init（）。 此功能主要用于简化用户尝试支持libavresample和libswresample的用例。


参数：s：Swr上下文关闭










五、核心转换函数

|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_convert](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaa5bb6cab830146efa8c760fa66ee582a) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **[out](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae26fe5221615fb0a46f7688b1c268f69), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) out_count, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **[in](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#a374bb0bba03cab664bd3187c3b5d09dd), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) in_count)|
|----|----|

转换音频


in和in_count可以设置为0，以最后刷新最后几个样本。


如果提供比输出空间更多的输入，则输入将被缓冲。 您可以通过使用swr_get_out_samples（）来检索给定数量的输入样本所需的输出样本数的上限来避免此缓冲。 转换将直接运行，无需复制即可。


参数：s：分配Swr上下文，并设置参数

   out：输出缓冲区，只有在打包音频的情况下才需要设置第一个

out_count：每个通道样品中可用于输出的空间量

   in：输入缓冲区，只有在打包音频的情况下才需要设置第一个

   in_count：在一个通道中可用的输入样本数

返回：每个通道的采样数量，误差的负值







|int64_t|[swr_next_pts](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaad9811626c6c01a241f6bf46aff27333) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), int64_t [pts](http://ffmpeg.org/doxygen/trunk/transcode__aac_8c.html#a4fbb704fb96c36a78a36eee010306ce7))|
|----|----|

将下一个时间戳从输入转换为输出时间戳为1 /（in_sample_rate * out_sample_rate）单位。


注意：有两种略有不同的行为模式。

1、当不使用自动时间戳补偿时，（min_compensation> = FLT_MAX）在这种情况下时间戳将通过延迟补偿

2、当使用自动时间戳补偿时，（min_compensation <FLT_MAX）在这种情况下，输出时间戳将匹配输出样本号。 有关两种补偿方式，请参阅ffmpeg-resampler（1）。

参数：s：初始化Swr上下文

   pts：时间戳为下一个输入样本，INT64_MIN如果未知

查看：swr_set_compensation（），swr_drop_output（）和swr_inject_silence（）是内部用于时间戳补偿的函数。

返回：下一个输出样本的输出时间戳










六、低级选项设置函数

这些功能提供了设置AVOption API无法实现的低级选项的方法。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_set_compensation](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gab7f21690522b85d7757e13fa9853d4d8) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) sample_delta, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) compensation_distance)|
|----|----|

激活重采样补偿（“软”补偿）。


在swr_next_pts（）中需要时，内部调用此函数。


参数：s：分配Swr上下文。 如果未初始化，或未设置SWR_FLAG_RESAMPLE，则会使用标志集调用swr_init（）。

sample_delta：每个样本PTS的delta

compensation_distance：要补偿的样品数量

返回：> = 0成功，AVERROR错误代码如果：

1、s为null

2、compensation_distance小于0，

3、compensation_distance是0，但是sample_delta不是，

4、补偿不支持重采样器，或

5、调用时，swr_init（）失败。







|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_set_channel_mapping](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga1b528050b5f5b704aecbfbc257a17ef0) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *channel_map)|
|----|----|

设置一个定制的输入通道映射。


参数：s：分配的Swr上下文，尚未初始化

channel_map：定制输入通道映射（通道索引数组，静音通道为-1）

返回：> = 0成功，否则AVERROR错误代码失败。




|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_build_matrix](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga0405fcd08440b3fc0165a00969582dc3) (uint64_t in_layout, uint64_t out_layout, double center_mix_level, double surround_mix_level, double lfe_mix_level, double rematrix_maxval, double rematrix_volume, double *matrix, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[stride](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#adec60f1216ca1b900f8d776c5b6190e0), enum [AVMatrixEncoding](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabd576ea664e3ac5d31f3362064fe13e6) matrix_encoding, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *log_ctx)|
|----|----|

生成通道混合矩阵。


此函数是用于构建默认混合矩阵的libswresample内部使用的函数。 它被公开，作为构建自定义矩阵的效用函数。


参数：in_layout：输入通道布局

out_layout：输出通道布局

center_mix_level：混合水平为中心通道

surround_mix_level：环绕声道的混合电平

   ife_mix_level：低频效果通道的混合电平

rematrix_maxval：如果为1.0，系数将被归一化以防止溢出。 如果INT_MAX，系数将不会被归一化。

matrix：混合系数; 矩阵[i + stride * o]是输出通道o中输入通道i的权重。

 
  stride：矩阵阵列中相邻输入通道之间的距离

matrix_encoding：矩阵立体声下混模式（例如dplii）

log_ctx：父记录上下文，可以为NULL

返回：0成功，否则ERROR代码失败






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_set_matrix](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaf9064d65b1e654dc78f7d6bcd2e8c747) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), const double *matrix, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[stride](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#adec60f1216ca1b900f8d776c5b6190e0))|
|----|----|

设置一个定制的混音矩阵。


参数：s：分配的Swr上下文，尚未初始化

   matrix：混音系数; 矩阵[i + stride * o]是输出通道o中输入通道i的权重

   stride：在矩阵的行之间偏移

返回：> = 0成功，否则AVERROR错误代码失败。










七、样品处理函数

|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_drop_output](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga43d7ba4507891fc25aacbbea5cdfb065) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2))|
|----|----|

丢弃指定数量的输出样本。


如果需要“硬”补偿，则此函数与swr_inject_silence（）一起由swr_next_pts（）调用。


参数：s：分配Swr上下文

   count：要丢弃的样品数量

返回：> = 0，否则为ERROR代码




|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_inject_silence](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga4733ed4779ed3e30379de2a6ef8de837) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2))|
|----|----|

注入指定数量的沉默样本。


如果需要“硬”补偿，此函数与swr_drop_output（）一起被swr_next_pts（）调用。


参数：s：分配Swr上下文

   count：要丢弃的样品数量

返回：> = 0，否则为ERROR代码




|int64_t|[swr_get_delay](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga5121a5a7890a2d23b72dc871dd0ebb06) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), int64_t base)|
|----|----|

获取下一个输入样本相对于下一个输出样本将遇到的延迟。


如果提供了比可用的输出空间更多的输入，Swresample可以缓冲数据，而且采样率之间的转换也需要延迟。 此函数返回所有此类延迟的总和。 输入或输出采样率的精确延迟不一定是整数值。 特别是当以较大的值进行下采样时，输出采样率可能是用于表示延迟的不良选择，类似于上采样和输入采样率。


参数：s：swr上下文

   base：返回延迟的时基：

1、f设置为1，返回的延迟是以秒为单位

2、如果设置为1000，返回的延迟是毫秒

3、如果设置为输入采样率，则返回的延迟在输入采样中

4、如果将其设置为输出采样率，则返回的延迟在输出采样中

5、如果它是in_sample_rate和out_sample_rate的最不常见的倍数，那么将返回一个确切的无舍弃的延迟

返回：延迟1 /基本单位。




|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_get_out_samples](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga97a8d5f6abe3bcdfb6072412f17285a4) (struct [SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) in_samples)|
|----|----|

查找下一个swr_convert调用将输出的样本数的上限，如果使用in_samples的输入样本调用。


这取决于内部状态，任何改变内部状态（如进一步的swr_convert（）调用）都可能会改变样本数swr_get_out_samples（）返回相同数量的输入样本数。


参数：in_samples：输入样本数。

注意：任何调用swr_inject_silence（），swr_convert（），swr_next_pts（）或swr_set_compensation（）都会使此限制无效

   建议将正确的可用缓冲区大小传递给所有函数，如swr_convert（），即使swr_get_out_samples（）表示较少使用。

返回：下一个swr_convert将输出的样本数量的上限或用于指示错误的负值










八、配置访问器

|unsigned|[swresample_version](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaed39c2153a1e26b4ef656ec8c765a2e3) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回LIBSWRESAMPLE_VERSION_INT常量。


这对于检查构建时libswresample是否与运行时libswresample具有相同的版本是有用的。


返回：未签名的int型版本




|const char *|[swresample_configuration](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gad602fde7a824753d5adc57e7f1372e27) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回swr构建时配置。

返回：构建时间./configure标志



|const char *|[swresample_license](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga29c5657b18632e206a99d1ae27225499) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回swr许可证。


返回：libswresample的许可证在建立时确定













九、AVFrame基础API

|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_convert_frame](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gac482028c01d95580106183aa84b0930c) ([SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *swr, [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *output, const [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *input)|
|----|----|

转换输入AVFrame中的样本并将其写入输出AVFrame。


输入和输出AVFrames必须具有channel_layout，sample_rate和格式设置。


如果输出的AVFrame没有分配数据指针，那么将使用av_frame_get_buffer（）来设置nb_samples字段来分配帧。


输出的AVFrame可以为空，或者比所需的分配样本少。 在这种情况下，未写入输出的任何剩余样本将被添加到内部FIFO缓冲区，以便在下次调用此函数或swr_convert（）时返回。


如果转换采样率，内部重采样延迟缓冲器中可能存在数据。 swr_get_delay（）告诉剩余样本的数量。 要将此数据作为输出，请调用此函数或使用NULL输入调用此函数或swr_convert（）。


如果SwrContext配置与输出和输入AVFrame设置不匹配，转换不会发生，并且取决于哪个AVFrame不匹配AVERROR_OUTPUT_CHANGED，则返回AVERROR_INPUT_CHANGED或它们的按位OR的结果。


查看：swr_delay()，[swr_convert()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaa5bb6cab830146efa8c760fa66ee582a)，[swr_get_delay()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga5121a5a7890a2d23b72dc871dd0ebb06)

参数：swr：音频重新采样上下文

   output：输出AVFrame

   input：输入AVFrame

返回：0成功，失败或不匹配配置错误。




|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[swr_config_frame](http://ffmpeg.org/doxygen/trunk/group__lswr.html#ga73ade2cf6934765cf518c6c917b77c76) ([SwrContext](http://ffmpeg.org/doxygen/trunk/structSwrContext.html) *swr, const [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[out](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae26fe5221615fb0a46f7688b1c268f69), const [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[in](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#a374bb0bba03cab664bd3187c3b5d09dd))|
|----|----|

使用AVFrames提供的信息配置或重新配置SwrContext。


即使故障，原始重采样上下文也被重置。 如果上下文打开，该函数将在内部调用swr_close（）。


查看：[swr_close()](http://ffmpeg.org/doxygen/trunk/group__lswr.html#gaa4bf1048740dfc08d68aba9f1b4db22e);

参数：swr：音频重新采样上下文

  out：输出AVFrame

  in：输入AVFrame

返回：0成功，失败时出错。



