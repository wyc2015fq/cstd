# FFMPEG学习【组件文档——音频重采样器】 - 一世豁然的专栏 - CSDN博客





2017年08月30日 14:27:45[一世豁然](https://me.csdn.net/Explorer_day)阅读数：429








一、描述

FFmpeg重采样器为libswresample库音频重采样实用程序提供了一个高级接口。 特别地，它允许执行音频重采样，音频通道布局再匹配，以及转换音频格式和打包布局。








二、重采样器选项

音频重采样器支持以下命名选项。





可以通过在FFmpeg工具中指定-option值来设置选项，通过在SwrContext选项中显式设置值或使用libavutil / opt.h API进行编程使用，可以通过在issample过滤器中使用option = value来设置选项。





ich，in_channel_count

设置输入通道的数量。 默认值为0.如果设置相应的通道布局in_channel_layout，则不设置此值。



och，out_channel_count

设置输出通道的数量。 默认值为0.如果设置相应的通道布局out_channel_layout，则不设置此值。



uch，used_channel_count

设置使用的输入通道数。 默认值为0.此选项仅用于特殊重新映射。



isr，in_sample_rate

设置输入采样率。 默认值为0。



osr，out_sample_rate

设置输出采样率。 默认值为0。



isf，in_sample_fmt

指定输入样本格式。 默认设置为none。



osf，out_sample_fmt

指定输出样本格式。 默认设置为none。



tsf，internal_sample_fmt

设置内部样品格式。 默认值为none。 如果没有明确设置，这将自动选择。





icl，in_channel_layout

ocl，out_channel_layout

设置输入/输出通道布局。



有关所需语法，请参阅ffmpeg-utils（1）手册中的（ffmpeg-utils）通道布局部分。





clev，center_mix_level

设置中心组合级别。 它是以deciBel表示的值，必须在[-32,32]的间隔。



slev，surround_mix_level

设置环绕声混合音量。 它是以deciBel表示的值，必须在[-32,32]的间隔。



lfe_mix_level

将LFE组合设置为非LFE级别。 当有LFE输入但没有LFE输出时使用。 它是以deciBel表示的值，必须在[-32,32]的间隔。



rmvol，rematrix_volume

设置rematrix体积。 默认值为1.0。



rematrix_maxval

设置重新输出的最大输出值。 这可以用于防止削波，从而防止音量减小。 值为1.0可防止剪辑。



flags，swr_flags

设置转换器使用的标志。 默认值为0。


它支持以下各个标志：



res

强制重采样，即使输入和输出采样率匹配，该标志也强制使用采样。





dither_scale

设置抖动刻度。 默认值为1。





dither_method

设置抖动方法。 默认值为0。





支持的值：


‘rectangular’

选择矩形抖动



‘triangular’

选择三角形抖动



“triangular_hp”

选择高通三角波



“lipshitz”

选择Lipshitz噪声整形抖动。



‘shibata’

选择Shibata噪声整形抖动。



“low_shibata”

选择低柴田噪声整形抖动。



“high_shibata”

选择高柴田噪声整形抖动。



“f_weighted”

选择f加权噪声整形抖动



“modified_e_weighted”

选择修改电子加权噪声整形抖动



“improved_e_weighted”

选择改进的电子加权噪声整形抖动





重采样

设置重采样引擎 默认值为swr。



支持的值：



“SWR”

选择本地SW Resampler; 在这种情况下，过滤器选项精度和偏差不适用。



“soxr”

选择SoX重新采样器（如果可用）; 补偿和过滤器选项filter_size，phase_shift，exact_rational，filter_type＆kaiser_beta在这种情况下不适用。








filter_size

仅对于swr，设置重采样过滤器大小，默认值为32。



phase_shift

仅对于swr，设置重采样相移，默认值为10，必须在[0,30]的间隔。



linear_interp

启用时使用线性插补（默认）。 如果要在exact_rational失败时保留速度而不是质量，请禁用它。



exact_rational

仅对于swr，启用时，请尝试根据输入和输出采样率使用精确的phase_count。 然而，如果它大于1 << phase_shift，则phase_count将为1 << phase_shift作为回退。 默认启用。





cutoff

设置截止频率（swr：6dB点; soxr：0dB点）比;必须是0到1之间的浮点值。默认值为0.97，swr为0.97，而soxr为0.91（采样率为44100，保留整个音频带为20kHz）。



precision

仅对于soxr，将重新采样信号的位精度。默认值为20（适用于目标位深度为16），使SoX的“高品质”;值28为SoX的“非常高品质”。



cheby

仅对于soxr，选择通配带无滚动（Chebyshev）和“非理性”比率的更高精度近似。默认值为0。



async

对于swr，简单的1参数音频同步到使用拉伸，挤压，填充和修剪的时间戳。将其设置为1将允许填充和修剪，较大的值表示数据可能每秒拉伸或挤压的样本中的最大数量。默认值为0，因此不应用补偿使样本与音频时间戳匹配。



first_pts

对于swr，假设第一个pts应该是这个值。时间单位为1 /采样率。这允许在流开始时进行填充/修整。默认情况下，没有对第一帧的预期位置进行假设，因此不进行任何填充或修整。例如，如果音频流在视频流之后启动，或者由于编码器延迟而修剪任何具有负值的样本，则可以将其设置为0以静音开始。



min_comp

仅对于swr，设置时间戳和音频数据之间的最小差异（以秒为单位），以触发拉伸/挤压/填充或修剪数据以使其与时间戳相匹配。默认是拉伸/挤压/填充和修剪被禁用（min_comp = FLT_MAX）。



min_hard_comp

仅对于swr，设置时间戳和音频数据之间的最小差异（以秒为单位），以触发添加/删除样本以使其与时间戳相匹配。该选项有效地是在硬（修剪/填充）和软（挤压/拉伸）补偿之间进行选择的阈值。请注意，默认情况下，所有补偿都通过min_comp禁用。默认值为0.1。



comp_duration

仅对于swr，设置拉伸/挤压数据以使其与时间戳相匹配的持续时间（以秒为单位）。必须是非负双浮点值，默认值为1.0。



max_soft_comp

仅对于swr，设置数据被拉伸/挤压的最大因子，使其与时间戳相匹配。必须是非负双浮点值，默认值为0。





matrix_encoding

选择矩阵立体声编码。



它接受以下值：



'none'

选择无



‘dolby’

选择杜比



“DPLII”

选择杜比定向逻辑II



默认值为none。








FILTER_TYPE

仅对于swr，请选择重新采样过滤器类型。 这只会影响重采样操作。



它接受以下值：



‘cubic’

选择立方体



“blackman_nuttall”

选择Blackman Nuttall窗口sinc



‘kaiser’

选择Kaiser windowed sinc





kaiser_beta

仅对于swr，设置Kaiser窗口beta值。 间隔[2,16]中必须是双浮点数，默认值为9。



output_sample_bits

仅对于swr，设置用于抖动的已使用输出采样位数。 必须是间隔[0,64]中的整数，默认值为0，这意味着它不被使用。




