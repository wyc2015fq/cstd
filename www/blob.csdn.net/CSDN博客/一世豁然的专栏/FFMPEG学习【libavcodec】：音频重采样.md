# FFMPEG学习【libavcodec】：音频重采样 - 一世豁然的专栏 - CSDN博客





2017年06月08日 15:21:06[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1820








一、函数

废弃：使用 libswresample代替

|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[ReSampleContext](http://ffmpeg.org/doxygen/trunk/structReSampleContext.html) *|[av_audio_resample_init](http://ffmpeg.org/doxygen/trunk/group__lavc__resample.html#ga6837453b6819cf2f4d26c2d320b1d696) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) output_channels, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) input_channels, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) output_rate, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) input_rate, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt_out, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt_in, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) filter_length, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) log2_phase_count, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[linear](http://ffmpeg.org/doxygen/trunk/interplayacm_8c.html#a552efe53b811f3855f53f59129cf34af), double cutoff)|
|----|----|

初始化音频重采样上下文

参数：output_channels：输出通道数

input_channels：输入通道数

output_rate：输出采样率

  input_rate：输入采样率

sample_fmt_out：请求输出示例格式

sample_fmt_in：输入样品格式

filter_length：滤波器组中每个FIR滤波器的长度相对于截止频率

log2_phase_count：log2的多相滤波器组中的条目数

 
 linear：如果1，则所使用的FIR滤波器将在最接近的2之间进行线性内插，如果0将最接近使用

 
 cutoff：截止频率，1.0对应于输出采样率的一半

返回：分配的ReSampleContext，如果发生错误则为NULL





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[audio_resample](http://ffmpeg.org/doxygen/trunk/group__lavc__resample.html#ga4a2391f7d72287f478ae9dfc0ae373f4) ([ReSampleContext](http://ffmpeg.org/doxygen/trunk/structReSampleContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), short *output, short *input, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples)|
|----|----|



|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[audio_resample_close](http://ffmpeg.org/doxygen/trunk/group__lavc__resample.html#ga06447648458074fba375b9cc069664e0) ([ReSampleContext](http://ffmpeg.org/doxygen/trunk/structReSampleContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

自由重新采样上下文。

参数：s：前一个使用av_audio_resample_init（）创建的重新抽样上下文的非NULL指针





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02) struct [AVResampleContext](http://ffmpeg.org/doxygen/trunk/structAVResampleContext.html) *|[av_resample_init](http://ffmpeg.org/doxygen/trunk/group__lavc__resample.html#ga9f11d551d458bb904e1078c65121fe03) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) out_rate, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) in_rate, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) filter_length, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) log2_phase_count, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[linear](http://ffmpeg.org/doxygen/trunk/interplayacm_8c.html#a552efe53b811f3855f53f59129cf34af), double cutoff)|
|----|----|

初始化音频重新采样器。

注意，如果任一速率不是一个整数，那么简单地将这两个速率缩放到一起就可以了。


参数：filter_length：滤波器组中每个FIR滤波器的长度相对于截止频率

log2_phase_count：log2的多相滤波器组中的条目数

  linear：如果1，则所使用的FIR滤波器将在最接近的2之间被线性内插，如果0将被使用最接近的

  cutoff：截止频率，1.0对应于输出采样率的一半







|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_resample](http://ffmpeg.org/doxygen/trunk/group__lavc__resample.html#gaced5571eafa8e23a25b96e1755f8f65c) (struct [AVResampleContext](http://ffmpeg.org/doxygen/trunk/structAVResampleContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), short *dst, short *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *consumed, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) src_size, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dst_size, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) update_ctx)|
|----|----|

使用先前配置的上下文重新采样样本数组。

参数：src：一系列未消耗的样品

consumed：已经在这里返回已经消耗的src的样本数

src_size：未使用样品的数量

  dst_size：dst中可用样本的空间量

update_ctx：如果这是0，那么上下文将不会被修改，这样几个通道可以用相同的上下文重新采样。

返回：如果发生错误，以dst或-1写入的样本数







|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_resample_compensate](http://ffmpeg.org/doxygen/trunk/group__lavc__resample.html#gadf6acc7cdbbf2fe8d6a7990a22662c0b) (struct [AVResampleContext](http://ffmpeg.org/doxygen/trunk/structAVResampleContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) sample_delta, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) compensation_distance)|
|----|----|

补偿采样/时间戳漂移。

补偿通过更改重新采样器参数来完成，因此不会发生可听到的点击或类似的失真


参数：sample_delta：应在其上进行补偿的输出样本中的距离

compensation_distance：应输出的输出样本数量较少

示例：av_resample_compensate（c，10，500）而不是510样本，只输出500个样本


注意，由于四舍五入，实际补偿可能略有不同，特别是如果补偿距离大，init中使用的in_rate小









|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_resample_close](http://ffmpeg.org/doxygen/trunk/group__lavc__resample.html#ga2901688d58c5a64b97436288b301ad18) (struct [AVResampleContext](http://ffmpeg.org/doxygen/trunk/structAVResampleContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|







