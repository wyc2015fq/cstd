# FFMPEG学习【libavcodec】：编解码器：硬件加速器桥：XvMC - 一世豁然的专栏 - CSDN博客





2017年05月10日 10:56:09[一世豁然](https://me.csdn.net/Explorer_day)阅读数：681








一、文件

[xvmc.h](http://ffmpeg.org/doxygen/trunk/xvmc_8h.html)


公共libavcodec XvMC头文件。











二、数据结构

struct  xvmc_pix_fmt{

int xvmc_id；

该字段包含特殊常数值AV_XVMC_ID。

它被用作应用程序正确使用API的测试，并且没有由像素例程导致的损坏。

应用程序 - 在初始化时设置

libavcodec - 未改变




short * data_blocks；

指向由XvMCCreateBlocks（）分配的块数组的指针。

数组必须被XvMCDestroyBlocks（）释放。 每组64个值表示差分像素信息（以MoCo模式）或IDCT的系数的一个数据块。



应用程序 - 在初始化时设置指针

libavcodec - 将系数/像素数据填充到阵列中




XvMCMacroBlock * mv_blocks；

指向由XvMCCreateMacroBlocks（）分配的宏块描述数组，并由XvMCDestroyMacroBlocks（）释放。



应用程序 - 在初始化时设置指针

libavcodec - 将描述数据填充到数组中




int allocated_mv_blocks；

可以存储在mv_blocks数组中的宏块描述数。



应用程序 - 在初始化时设置

libavcodec - 未改变




int allocated_data_blocks；

可以在data_blocks数组中一次存储的块数。



应用程序 - 在初始化时设置

libavcodec - 未改变




int idct；

表明硬件会将data_block解释为IDCT系数，并对它们执行IDCT。



应用程序 - 在初始化时设置

libavcodec - 未改变




int unsigned_intra；

在MoCo模式中，它表示帧内宏块被假定为无符号格式; 与XVMC_INTRA_UNSIGNED标志相同。



应用程序 - 在初始化时设置

libavcodec - 未改变




XvMCSurface * p_surface；

指向XvMCCreateSurface（）分配的表面的指针。

它必须在应用程序退出时被XvMCDestroySurface（）释放。 它识别视频硬件上的帧及其状态。



应用程序 - 在初始化时设置

libavcodec - 未改变




XvMCSurface * p_past_surface；

在调用ff_draw_horiz_band（）之前由解码器设置，被XvMCRenderSurface函数需要。

指向表面的指针用作过去的参考



应用程序 - 未改变

libavcodec - 设置




XvMCSurface * p_future_surface；

指向表面用作未来参考。



应用程序 - 未改变

libavcodec - 设置




unsigned int picture_structure；

顶部/底部字段或帧。



应用程序 - 未改变

libavcodec - 设置




unsigned int flags；

XVMC_SECOND_FIELD - 序列中的第1或第2个字段。



应用程序 - 未改变

libavcodec - 设置




int start_mv_blocks_num；

已经传递到硬件的mv_blocks数组中的宏块描述数。



应用程序 - 在get_buffer（）上调零。 一个成功的ff_draw_horiz_band（）可以用fill_mb_block_num或者二者来增加它们。

libavcodec - 未改变




int filled_mv_blocks_num；

mv_blocks数组中的新宏块描述数（在start_mv_blocks_num之后）由libavcodec填充并必须传递到硬件。



应用程序 - 在get_buffer（）或ff_draw_horiz_band（）成功后将其置零。

libavcodec - 每个存储的MB之一增加



应用程序 - 在get_buffer（）或ff_draw_horiz_band（）成功后将其置零。

libavcodec - 每个存储的MB之一增加




int next_free_data_block_num；

下一个空闲数据块的数量; 一个数据块由data_blocks数组中的64个短值组成。

已经通过将它们的位置放置在相应的块描述结构字段中，这些字段是mv_blocks数组的一部分，已经声明了此前的所有块。



应用程序 - 在在get_buffer（）上调零。 成功的ff_draw_horiz_band（）可能与start_mb_blocks_num一起归零。

libavcodec - 每个解码的宏块将其包含的编码块的数量增加。

}











三、宏

#define AV_XVMC_ID   0x1DC711C0







