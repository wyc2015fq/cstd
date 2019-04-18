# FFMPEG学习【libavcodec】：实用函数（一） - 一世豁然的专栏 - CSDN博客





2017年06月09日 10:56:41[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1318








一、模块

一）、pixel formats模块


|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avcodec_get_chroma_sub_sample](http://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html#ga8734c0195b87efe1a060ca101505c7b8) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)[pix_fmt](http://ffmpeg.org/doxygen/trunk/hwcontext__videotoolbox_8c.html#a7331e302ea7bb48e0b605a069576e135), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *h_shift, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *v_shift)|
|----|----|

实用函数从像素格式AVPixFmtDescriptor访问log2_chroma_w log2_chroma_h。

此函数声明pix_fmt有效。 请查看av_pix_fmt_get_chroma_sub_sample，返回失败代码，并在无效的pix_fmts的情况下继续。


参数：[in]pix_fmt：像素格式

  [out]h_shift:存储log2_chroma_w

  [out]v_shift:存储log2_chroma_h





|unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_pix_fmt_to_codec_tag](http://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html#ga9bb07f91a7c87cba19a44ebdcf5bde63) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)[pix_fmt](http://ffmpeg.org/doxygen/trunk/hwcontext__videotoolbox_8c.html#a7331e302ea7bb48e0b605a069576e135))|
|----|----|

返回表示与像素格式pix_fmt相关联的4CC代码的值，如果没有找到关联的4CC代码，则返回0。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avcodec_get_pix_fmt_loss](http://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html#ga838136cb54100d6afcb2f7e0377566ff) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) dst_pix_fmt, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) src_pix_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) has_alpha)|
|----|----|

已废弃：查看[av_get_pix_fmt_loss()](http://ffmpeg.org/doxygen/trunk/pixdesc_8c.html#a69ba6ef97543b19a15236a513e56dffe)




|enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)|[avcodec_find_best_pix_fmt_of_list](http://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html#ga9e74b43a3433ccfe836814f0a6371aa0) (const enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) *pix_fmt_list, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) src_pix_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) has_alpha, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *loss_ptr)|
|----|----|

找到最佳的像素格式转换为给定某些的源像素格式。

当从一种像素格式转换到另一种像素格式时，可能会发生信息丢失。 例如，当从RGB24转换为GRAY时，颜色信息将丢失。 同样，从某些格式转换成其他格式时也会发生其他损失。 avcodec_find_best_pix_fmt_of_2（）搜索哪些给定的像素格式应该被用来承受最少的损失。 由它选择一个的像素格式由pix_fmt_list参数决定。


参数：[in]pix_fmt_list：AV_PIX_FMT_NONE终止的像素格式数组可供选择

  [in]src_pix_fmt：源像素格式

  [in]has_alpha：是否使用源像素格式alpha通道。

  [out]loss_ptr：组合标志通知您将会发生什么样的损失。

返回：要转换的最佳像素格式或如果没有找到，则为-1。





|enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)|[avcodec_find_best_pix_fmt_of_2](http://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html#ga209703f647b87bd3f35ed97eb534fd21) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) dst_pix_fmt1, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) dst_pix_fmt2, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) src_pix_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) has_alpha, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *loss_ptr)|
|----|----|

已废弃：查看[av_find_best_pix_fmt_of_2()](http://ffmpeg.org/doxygen/trunk/pixdesc_8c.html#a875f1e7a15d882d4cd9d38dde093939b)




|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02) enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)|[avcodec_find_best_pix_fmt2](http://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html#gafaa40af15ce2d1ab4adca12b827ee85f) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) dst_pix_fmt1, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) dst_pix_fmt2, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) src_pix_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) has_alpha, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *loss_ptr)|
|----|----|



|enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)|[avcodec_default_get_format](http://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html#ga2d16136bf8844295b2f8ea17c386d833) (struct [AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), const enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) *[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab))|
|----|----|





二）、FFT函数模块

1、头文件

[avfft.h](http://ffmpeg.org/doxygen/trunk/avfft_8h.html)





2、数据结构


|struct|[FFTComplex](http://ffmpeg.org/doxygen/trunk/structFFTComplex.html)|
|----|----|


3、类型定义


|typedef float|[FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96)|
|----|----|


4、函数


|[FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *|[av_fft_init](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga6c6b88ddaa9aee5b0ec4e2d09a78e286) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nbits, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[inverse](http://ffmpeg.org/doxygen/trunk/asfcrypt_8c.html#a1daab37d5201e2de0ffa163447a51c0f))|
|----|----|

建立一个复合的FFT。

参数：nbits：log2的输入数组的长度

  inverse：如果0执行正向变换，如果1执行反向





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_fft_permute](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga4d8f70cf57c4f969c99af653b8a82f15) ([FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [FFTComplex](http://ffmpeg.org/doxygen/trunk/structFFTComplex.html) *z)|
|----|----|

在调用ff_fft_calc（）之前，需要进行排列。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_fft_calc](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga4fe79338f98704178512503f87a4af4c) ([FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [FFTComplex](http://ffmpeg.org/doxygen/trunk/structFFTComplex.html) *z)|
|----|----|

使用av_fft_init（）中定义的参数进行复合FFT。

输入数据必须先排列。 没有1.0 / sqrt（n）标准化。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_fft_end](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga9dd6f59f2d27b37fc047d9dc452c6ba5) ([FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|



|[FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *|[av_mdct_init](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga92059089cd3aa014ca07e18677a05f95) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nbits, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[inverse](http://ffmpeg.org/doxygen/trunk/asfcrypt_8c.html#a1daab37d5201e2de0ffa163447a51c0f), double scale)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_imdct_calc](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga356840c227e60047afffbbb34ded5477) ([FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *output, const [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *input)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_imdct_half](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga3597ad2ed7a46ac1bf040c7d9ccefabc) ([FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *output, const [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *input)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_mdct_calc](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gac59dbae80d532741eb8af797b3756eae) ([FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *output, const [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *input)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_mdct_end](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gad7543db3d701ce7c0ce789c10d52aa76) ([FFTContext](http://ffmpeg.org/doxygen/trunk/structFFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|



|[RDFTContext](http://ffmpeg.org/doxygen/trunk/structRDFTContext.html) *|[av_rdft_init](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga5259358e6ddb8f7d02513a3a3bcd3e54) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nbits, enum [RDFTransformType](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga58a704747daba49afa3611cea218a24b) trans)|
|----|----|

建立一个真正的FFT。

参数：nbits：log2的输入数组的长度

trans：转换的类型





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_rdft_calc](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga745b8757713a5a0f0347eddcc6ccc9a4) ([RDFTContext](http://ffmpeg.org/doxygen/trunk/structRDFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1))|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_rdft_end](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga9d0be7a2253417ab4bb53a08bb70dfce) ([RDFTContext](http://ffmpeg.org/doxygen/trunk/structRDFTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|



|[DCTContext](http://ffmpeg.org/doxygen/trunk/structDCTContext.html) *|[av_dct_init](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga64a4f6938a7f836305672ef6a7d23d6c) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nbits, enum [DCTTransformType](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaeeabd062332b246d19bed36e041d2df6)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|

设置DCT。

参数：nbits：DCT-II，DCT-III和DST-I（1 << nbits）+ 1用于DCT-I的输入阵列的大小（1 << nbits）

  type：转换的类型





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_dct_calc](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#ga11f6dd46c458176ef8440c7a15bea7d0) ([DCTContext](http://ffmpeg.org/doxygen/trunk/structDCTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [FFTSample](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaa306dc16df543b25d9910debc3f76b96) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1))|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_dct_end](http://ffmpeg.org/doxygen/trunk/group__lavc__fft.html#gaf529d0ebe6f05c419d4b995322993517) ([DCTContext](http://ffmpeg.org/doxygen/trunk/structDCTContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|








二、数据结构


|struct|[AVBitStreamFilterContext](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilterContext.html)|
|----|----|





|struct|[AVBSFContext](http://ffmpeg.org/doxygen/trunk/structAVBSFContext.html)|
|----|----|

比特流滤波器状态。





|struct|[AVBitStreamFilter](http://ffmpeg.org/doxygen/trunk/structAVBitStreamFilter.html)|
|----|----|








三、宏


|enum|[AVLockOp](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#ga7a013315e444069ec9630ddf3edce3f8) { [AV_LOCK_CREATE](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gga7a013315e444069ec9630ddf3edce3f8a486a4a2d7a97554c2e073536c038465d), [AV_LOCK_OBTAIN](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gga7a013315e444069ec9630ddf3edce3f8af684bbd8575f2686f42346f5e43be673), [AV_LOCK_RELEASE](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gga7a013315e444069ec9630ddf3edce3f8a7c470cd815cdef3b16a977e9a8c89c69), [AV_LOCK_DESTROY](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gga7a013315e444069ec9630ddf3edce3f8aee9ae55e2efe16565998e473c2a18935) }|
|----|----|

lockmgr使用的锁定操作。






