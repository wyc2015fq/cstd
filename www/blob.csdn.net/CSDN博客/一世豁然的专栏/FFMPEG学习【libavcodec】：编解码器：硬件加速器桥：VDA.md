# FFMPEG学习【libavcodec】：编解码器：硬件加速器桥：VDA - 一世豁然的专栏 - CSDN博客





2017年05月09日 20:45:09[一世豁然](https://me.csdn.net/Explorer_day)阅读数：826








一、文件

[vda.h](http://www.ffmpeg.org/doxygen/trunk/vda_8h.html)


公共libavcodec VDA头文件。








二、数据结构

struct  vda_context{


VDADecoder decoder；

VDA解码器对象。

编码：未使用。

解码：通过libavcodec设置/取消。




CVPixelBufferRef cv_buffer；

包含当前图像数据的Core Video像素缓冲区。

编码：未使用。

编码：通过libavcodec设置。通过用户取消。




int use_sync_decoding；

在同步模式下使用硬件解码器。

编码：未使用。

解码：通过用户设置。




int width；

帧宽度。

编码：未使用。

解码：通过用户设置/取消。




int height

帧高度



编码：未使用。

解码：通过用户设置/取消。





int format；

帧格式。



编码：未使用。

解码：通过用户设置/取消。





OSType cv_pix_fmt_type；

输出图像缓冲区的像素格式。



编码：未使用。

解码：通过用户设置/取消。





uint8_t * priv_bitstream；

未使用



编码：未使用。

解码：通过用户设置/取消。





int priv_bitstream_size;

未使用；




int priv_allocated_size；

未使用




int use_ref_buffer；

使用av_buffer来管理缓冲区。

当标志设置时，解码器返回的CVPixelBuffers将自动释放，因此必要时必须保留它们。 不设置此标志可能会导致内存泄漏。



编码：未使用。

解码：通过用户设置。

}

该结构用于向VDA FFmpeg HWAccel实现提供必要的配置和数据。





struct  AVVDAContext{


VDADecoder decoder;

VDA解码器对象。

由调用者创建和释放。




VDADecoderOutputCallback output_callback;

必须传递给VDADecoderCreate的输出回调。




OSType cv_pix_fmt_type;

CVPixelBuffer格式VDA将用于解码帧的类型; 由调用者设置。

}

该结构体保存了用于初始化VDA解码的调用者和libavcodec之间需要传递的所有信息。


它的大小不是公共ABI的一部分，它必须使用av_vda_alloc_context（）分配，并与av_free（）一起释放。








三、函数

int ff_vda_create_decoder (struct vda_context *vda_ctx, uint8_t *extradata, int extradata_size)


解析：创建视频解码器。




int ff_vda_destroy_decoder (struct vda_context *vda_ctx)


销毁视频解码器。





AVVDAContext * av_vda_alloc_context (void)


解析：分配并初始化VDA上下文。


当调用者选择AV_PIX_FMT_VDA格式时，该函数应从get_format（）回调中调用。 然后，主叫方必须创建将用于VDA加速解码的解码器对象（使用由libavcodec提供的输出回调）。


当使用VDA解码完成时，调用者必须摧毁解码器对象，并使用av_free（）释放VDA上下文。


返回：新分配的上下文，失败时为NULL。





int av_vda_default_init (AVCodecContext *avctx)


解析：这是一个便捷的功能，可以使用内部实现创建和设置VDA上下文。


参数avctx - 相应的编解码器上下文。

返回：> = 0成功，失败时出现负ERROR码




int av_vda_default_init2（AVCodecContext * avctx，AVVDAContext * vdactx）


解析：这是一个便捷的功能，可以使用内部实现创建和设置VDA上下文。


参数： avctx - 相应的编解码器上下文

     vdactx  - 用来使用的VDA上下文

返回：> = 0成功，失败时出现负ERROR码





void av_vda_default_free（AVCodecContext * avctx）


解析：必须调用此函数来释放使用av_vda_default_init（）初始化的VDA上下文。


参数： avctx - 相应的编解码器上下文




