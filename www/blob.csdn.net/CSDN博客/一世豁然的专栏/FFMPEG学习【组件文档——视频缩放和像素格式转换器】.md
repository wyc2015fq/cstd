# FFMPEG学习【组件文档——视频缩放和像素格式转换器】 - 一世豁然的专栏 - CSDN博客





2017年08月30日 11:07:57[一世豁然](https://me.csdn.net/Explorer_day)阅读数：392








一、描述

FFmpeg rescaler为libswscale库图像转换实用程序提供了一个高级界面。 特别地，它允许执行图像重新缩放和像素格式转换。








二、缩放选项

视频缩放器支持以下命名选项。





可以通过在FFmpeg工具中指定-option值来设置选项。 对于编程使用，可以在SwsContext选项或libavutil / opt.h API中显式设置它们。


sws_flags


设置缩放标记。这也用于设置缩放算法。只能选择一个算法。默认值为“bicubic”。





它接受以下值：



“fast_bilinear”

选择快速双线性缩放算法。



‘bilinear’

选择双线性缩放算法。



‘bicubic’

选择双三次缩放算​​法。



‘experimental’

选择实验缩放算法。



‘neighbor’

选择最近邻调整算法。



‘area’

选择平均面积重定标算法。



“bicublin”

选择亮度分量的双三次缩放算​​法，色度分量双线性。



‘gauss’

选择高斯分类算法。



‘sinc’

选择sinc重新缩放算法。



‘lanczos’

选择Lanczos重新缩放算法。



‘spline’

选择自然双三次样条重调整算法。



“print_info”

启用打印/调试日志记录。



“accurate_rnd”

启用精确的舍入。



“full_chroma_int”

启用全色度插值。



“full_chroma_inp”

选择完整色度输入。



“bitexact对”

启用bitexact输出








srcw

设置源宽度。



SRCH

设置源高度。



dstw

设定目的地宽度。



DSTH

设定目的地高度。



src_format

设置源像素格式（必须表示为整数）。



dst_format

设置目标像素格式（必须表示为整数）。



src_range

选择源范围。



dst_range

选择目的地范围。



param0，param1

设置缩放算法参数。 指定的值是某些缩放算法的特定值，而被其他算法忽略。 指定的值是浮点数值。








sws_dither

设置抖动算法。 接受以下值之一。 默认值为“auto”。



'auto'

自动选择



'none'

没有抖动



“bayer”

拜耳抖动



“ed”

误差扩散抖动



“a_dither”

算术抖动，基于加法



“x_dither”

算术抖动，基于xor（更随机/更少的明显的图案，a_dither）。








ALPHABLEND

将Alpha混合设置为在输入为alpha但不输出时使用。 默认值为“none”。



“uniform_color”

混合均匀的背景颜色



‘checkerboard’

混合到棋盘上



'none'

没有混合




