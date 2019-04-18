# FFMPEG学习【libavcodec】：编解码器：硬件加速器桥：VDPAU解码器和渲染器 - 一世豁然的专栏 - CSDN博客





2017年05月10日 09:17:57[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1761








VDPAU硬件加速有两个模块。


1、VDPAU解码。

2、VDPAU演示。




VDPAU解码模块使用FFmpeg解析机制解析所有头，并使用VDPAU进行实际解码。


根据当前的实现，实际的解码和渲染（API调用）作为VDPAU演示（vo_vdpau.c）模块的一部分完成。








一、文件

[vdpau.h](http://www.ffmpeg.org/doxygen/trunk/vdpau_8h.html)


公共libavcodec VDPAU头文件。








二、数据结构

union  AVVDPAUPictureInfo{


VdpPictureInfoH264 h264；

VdpPictureInfoMPEG1Or2 mpeg；

VdpPictureInfoVC1 vc1；

VdpPictureInfoMPEG4Part2 mpeg4；

}




struct  AVVDPAUContext{


VdpDecoder decoder；

VDPAU解码器手柄。

被用户设置。




VdpDecoderRender * render；

VDPAU解码器渲染回调。

被用户设置。




attribute_deprecated union AVVDPAUPictureInfo info；

VDPAU图片信息。

被libavcodex设置。




attribute_deprecated int bitstream_buffers_allocated；

分配比特率缓冲表的大小。

被libavcodex设置。





attribute_deprecated int bitstream_buffers_used

比特流缓冲表中有用的比特流缓冲器。

被libavcodex设置。





attribute_deprecated  VdpBitstreamBuffer * bitstream_buffers；

比特流缓冲区表。

被libavcodex设置。





AVVDPAU_Render2 render2；

}

该结构用于在libavcodec库和客户端视频应用程序之间共享数据。





struct  vdpau_render_state{

VdpVideoSurface surface；

用作渲染表面，从未改变。




int state；

保存FF_VDPAU_STATE_ *值。




union AVVDPAUPictureInfo info；

所有支持的编解码器的图像参数信息。




int bitstream_buffers_allocated；

描述压缩视频数据的大小/位置。

释放比特流缓存时设置为0。




int digits_buffers_used;



VdpBitstreamBuffer * bitstream_buffers;


用户负责使用av_freep（）释放此缓冲区。

}


该结构用作FFmpeg解码器（vd_）和显示（vo_）模块之间的回调。


这用于定义包含在FFmpeg解码器和其客户端之间传递的表面，图像参数，比特流信息等的视频帧。








三、宏

#define FF_VDPAU_STATE_USED_FOR_RENDER   1


videoSurface用于渲染。





#define FF_VDPAU_STATE_USED_FOR_REFERENCE   2


被参数/预测需要

编解码器操作此宏。








四、类型定义

typedef int(* AVVDPAU_Render2 )(struct AVCodecContext *, struct AVFrame *, const VdpPictureInfo *, uint32_t, const VdpBitstreamBuffer *）


|||
|----|----|





五、函数

AVVDPAUContext * av_alloc_vdpaucontext (void)；


AVVDPAUContext的分配函数。


解析：允许扩展结构而不会中断API / ABI。





AVVDPAU_Render2 av_vdpau_hwaccel_get_render2 (const AVVDPAUContext *)；





void av_vdpau_hwaccel_set_render2 (AVVDPAUContext *, AVVDPAU_Render2)；





int av_vdpau_bind_context (AVCodecContext *avctx, VdpDevice device, VdpGetProcAddress *get_proc_address, unsigned flags)；


解析：将VDPAU设备与编解码器上下文相关联以进行硬件加速。


这个函数的意思是从get_format（）编解码器回调或更早版本调用。 也可以在avcodec_flush_buffers（）之后调用底层VDPAU设备中间流（例如从非透明显示抢占恢复）。


注意：如果此函数成功完成，get_format（）必须返回AV_PIX_FMT_VDPAU。

参数：avctx - 解调其调用get_format（）回调的上下文。

    device - VDPAU设备手柄用于硬件加速。

    get_proc_address - VDPAU设备驱动。

    flags - 零个或多个OR'AV_HWACCEL_FLAG_ *标志

返回：0表示成功，AVERROR代码表示失败。




int av_vdpau_get_surface_parameters (AVCodecContext *avctx, VdpChromaType *type, uint32_t *width, uint32_t *height)；


解析：获取参数以使用VDPAU硬件解码加速为编解码器环境创建足够的VDPAU视频表面。


注意：如果上下文未使用av_vdpau_bind_context（）成功绑定到VDPAU设备，行为是未定义的。

参数：avctx - 用于解码流的编解码器上下文

    type - 存储空间用于VDPAU视频表面色度类型（或NULL忽略）。

    width - 存储空间为VDPAU视频表面像素宽度（或NULL忽略）。

    height - 存储空间为VDPAU视频表面像素高度（或NULL忽略）。

返回：0成功，失败时出现负ERROR代码。




AVVDPAUContext * av_vdpau_alloc_context (void)；


解析：分配AVVDPAUContext。


返回：新分配的AVVDPAUContext或NULL失败。




attribute_deprecated int av_vdpau_get_profile (AVCodecContext *avctx, VdpDecoderProfile *profile)；


获取应用于初始化VDPAU解码器的解码器配置文件。


应该从AVCodecContext.get_format（）回调调用。


已废弃:请改用av_vdpau_bind_context（）。

参数：avctx -  用于解码流的编解码器上下文。

    profile - 一个指针，其结果将被写入成功。 如果此函数返回错误，则配置文件的内容将不会被定义。

返回：0成功（非负），否定AVERROR错误码表示失败。



