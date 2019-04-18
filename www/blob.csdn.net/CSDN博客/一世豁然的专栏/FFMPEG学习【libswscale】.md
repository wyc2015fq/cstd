# FFMPEG学习【libswscale】 - 一世豁然的专栏 - CSDN博客





2017年07月30日 11:17:03[一世豁然](https://me.csdn.net/Explorer_day)阅读数：463








颜色转换和缩放库。

返回LIBSWSCALE_VERSION_INT常量。








一、数据结构


|struct|[SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html){double * coeff；// 指向系数列表的指针int length；//向量中的系数数量}|
|----|----|



|struct|[SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html){SwsVector * lumH；SwsVector * lumV；SwsVector * chrH；SwsVector * chrV；}|
|----|----|








二、宏


|#define|[SWS_FAST_BILINEAR](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga6110064d9edfbec77ca5c3279cb75c31)   1|
|----|----|
|| |
|#define|[SWS_BILINEAR](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga26b45a2c6df7c1637feab34fbb21a9bb)   2|
|| |
|#define|[SWS_BICUBIC](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga02a4ee9d3c158e842941873d2cf83b9f)   4|
|| |
|#define|[SWS_X](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga5a8a563c525f7a71ef33368a782157a0)   8|
|| |
|#define|[SWS_POINT](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaae31eb0722e375c12d4fe7a028694491)   0x10|
|| |
|#define|[SWS_AREA](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga702a05689a18ccd170e64fb664ebca57)   0x20|
|| |
|#define|[SWS_BICUBLIN](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga867d1e9ac9ad1eafc33cc3e94041b091)   0x40|
|| |
|#define|[SWS_GAUSS](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga6f8f44a4d92bc09eed63984bc3acf94a)   0x80|
|| |
|#define|[SWS_SINC](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga783824f7ce78aff026e9f71f97be00be)   0x100|
|| |
|#define|[SWS_LANCZOS](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaae77dad7fc203836ce365378b35da167)   0x200|
|| |
|#define|[SWS_SPLINE](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaff798131b2051020f5539baddeeb4e12)   0x400|
|| |
|#define|[SWS_SRC_V_CHR_DROP_MASK](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga3f5d541ffe1c4bf8a216c54f0de8eb11)   0x30000|
|| |
|#define|[SWS_SRC_V_CHR_DROP_SHIFT](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga77331b2b4b9648ca47a3ca307a2151c5)   16|
|| |
|#define|[SWS_PARAM_DEFAULT](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gad4a209577e10d655c2495349a96bb611)   123456|
|| |
|#define|[SWS_PRINT_INFO](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga4cc80734dcb486c315ca2061363c6ede)   0x1000|
|| |
|#define|[SWS_FULL_CHR_H_INT](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga4af3bbc6d873caa5ac0bef06f534e3c6)   0x2000|
|| |
|#define|[SWS_FULL_CHR_H_INP](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga4b086de7216852cfc2e40f576285fc97)   0x4000|
|| |
|#define|[SWS_DIRECT_BGR](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga2d1c4cc56bf45e13ee8ce7e3bc199397)   0x8000|
|| |
|#define|[SWS_ACCURATE_RND](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga2f63481f9ac7c35ea5ee7075f15c040e)   0x40000|
|| |
|#define|[SWS_BITEXACT](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga0042bbf5ffe1952e9794a0056792f89f)   0x80000|
|| |
|#define|[SWS_ERROR_DIFFUSION](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga8ba8294c18e0dae9b7883aff1b5f7e89)   0x800000|
|| |
|#define|[SWS_MAX_REDUCE_CUTOFF](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gacc8f46b6954cae7d891a041793e2e751)   0.002|
|| |
|#define|[SWS_CS_ITU709](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gab5d1f9b6f50c155496b358d348ed6f36)   1|
|| |
|#define|[SWS_CS_FCC](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaae1a28de2fd6fdd21a082bcc10ce6cb9)   4|
|| |
|#define|[SWS_CS_ITU601](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaf9de144a406b66c68bd29cdcaefaeee5)   5|
|| |
|#define|[SWS_CS_ITU624](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gab3795e86c9f475f206a187d937c7f4fa)   5|
|| |
|#define|[SWS_CS_SMPTE170M](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gac7287919a348578d8452c42c03d65042)   5|
|| |
|#define|[SWS_CS_SMPTE240M](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga8a066eae69a34d9d60c73c40628094e4)   7|
|| |
|#define|[SWS_CS_DEFAULT](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga1867c2c57f3c0d18fa72592e30e7d3c2)   5|
|| |
|#define|[SWS_CS_BT2020](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga0c8f55463ce3bd8904dedd7e73f29050)   9|








三、函数


|unsigned|[swscale_version](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gac879a3ada8130d32ba02a9265bfef253) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|



|const char *|[swscale_configuration](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaaee057c84b44975856aa26e7bb0decce) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libswscale构建时配置。





|const char *|[swscale_license](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga985798b99929665a4dcecf661d45de72) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libswscale许可证。





|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *|[sws_getCoefficients](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gac271c2aa3dd5569c59f018eaf914bbc0) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) colorspace)|
|----|----|

返回指向适用于sws setColorspace Details（）的给定颜色空间的yuv < - > rgb系数的指针。

参数：colorspace：其中一个SWS_CS_ *宏。 如果无效，则使用SWS_CS_DEFAULT。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[sws_isSupportedInput](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gab5e631b602510bd3fa3a5a7d509e573d) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)[pix_fmt](http://ffmpeg.org/doxygen/trunk/hwcontext__videotoolbox_8c.html#a7331e302ea7bb48e0b605a069576e135))|
|----|----|

如果pix_fmt是支持的输入格式，则返回正值，否则返回0。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[sws_isSupportedOutput](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga6885dc8ec36e8f2e701924263aa92cb0) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)[pix_fmt](http://ffmpeg.org/doxygen/trunk/hwcontext__videotoolbox_8c.html#a7331e302ea7bb48e0b605a069576e135))|
|----|----|

如果pix_fmt是支持的输出格式，则返回正值，否则返回0。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[sws_isSupportedEndiannessConversion](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga67f865ce22ae986ea84f4d7ce60ef182) (enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)[pix_fmt](http://ffmpeg.org/doxygen/trunk/hwcontext__videotoolbox_8c.html#a7331e302ea7bb48e0b605a069576e135))|
|----|----|

参数：pix_fmt：像素格式

返回：如果支持pix_fmt的字节顺序转换，则为正值，否则为0。





|struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *|[sws_alloc_context](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gab97d02dc20945fd013ba49afb8bd1099) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配一个空的SwsContext。

他必须填写并传递给sws_init_context（）。 要填写，请参阅AVOptions，options.c和sws_setColorspaceDetails（）。






|[av_warn_unused_result](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[sws_init_context](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga2120f2a4fb3aa075cb0aff06eb5b55cb) (struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *sws_context, [SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *srcFilter, [SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *dstFilter)|
|----|----|

初始化swscaler上下文sws_context。

返回：成功为零或正值，误差为负值





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_freeContext](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gad3af0ca76f071dbe0173444db9882932) (struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *swsContext)|
|----|----|

释放swscaler上下文swsContext。

如果swsContext为NULL，则不做任何操作。






|struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *|[sws_getContext](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaf360d1a9e0e60f906f74d7d44f9abfdd) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcW, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcH, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) srcFormat, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dstW, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dstH, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) dstFormat, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4), [SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *srcFilter, [SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *dstFilter, const double *param)|
|----|----|

分配并返回SwsContext。

您需要使用sws_scale（）执行缩放/转换操作。


参数：srcW：源图像的宽度

srcH：源图像的高度

srcFormat：源图像格式

dstW：目的地图像的宽度

dstW：目的地图像的高度

dstFormat：目的地图像格式

 
  flags：指定用于重新缩放的哪种算法和选项

param：调整使用的缩放器的额外参数对于SWS_BICUBIC参数[0]和[1]调整基函数的形状，param [0]调谐f（1）和param [1] f'（1）对于SWS_GAUSS参数[0] 调整指数并因此调节截止频率对于SWS_LANCZOS参数[0]调整窗口函数的宽度

返回：指向分配的上下文的指针，如果出现错误则为NULL

注意：这个函数将在写入更清晰的替代方法后被删除





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[sws_scale](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gae531c9754c9205d90ad6800015046d74) (struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *const srcSlice[], const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcStride[], [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcSliceY, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcSliceH, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *const dst[], const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dstStride[])|
|----|----|

缩放srcSlice中的图像切片，并将结果缩放的切片放在dst中的图像中。

切片是图像中连续行的序列。


必须按顺序提供切片，无论是以顶部还是底部的顺序。 如果以不间断的顺序提供切片，则该函数的行为是未定义的。


参数：c：以前使用sws_getContext（）创建的缩放上下文

srcSlice：该数组包含指向源切片平面的指针

srcStride：该阵列包含源图像的每个平面的步幅

srcSliceY：要处理的片的源图像中的位置，即片的第一行的图像中的数（从零开始计数）

srcSliceH：源切片的高度，即切片中的行数

   dst：该数组包含指向目标图像平面的指针

dstStride：该阵列包含目标图像的每个平面的步幅

返回：输出切片的高度

缩放srcSlice中的图像切片，并将结果缩放的切片放在dst中的图像中。


假设平面YUV处于YUV顺序而不是YVU。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[sws_setColorspaceDetails](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga541bdffa8149f5f9203664f955faa040) (struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) inv_table[4], [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcRange, const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[table](http://ffmpeg.org/doxygen/trunk/ircamdec_8c.html#a5398a60b56e50317884acf50a256b249)[4], [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dstRange, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) brightness, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) contrast, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) saturation)|
|----|----|

参数：dstRange：指示输出的while-black范围的标志（1 = jpeg / 0 = mpeg）
srcRange：指示输入的while-black范围的标志（1 = jpeg / 0 = mpeg）

   table：描述输出yuv空间的yuv2rgb系数，通常为ff_yuv2rgb_coeffs [x]

   inv_table：描述输入yuv空间的yuv2rgb系数，通常为ff_yuv2rgb_coeffs [x]

brightness：16.16定点亮度校正

brightness：16.16定点对比度校正

saturation：16.16定点饱和度校正

返回：-1如果不支持。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[sws_getColorspaceDetails](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga098dc4b69a97b616fc35791aec249ff1) (struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) **inv_table, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *srcRange, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) **[table](http://ffmpeg.org/doxygen/trunk/ircamdec_8c.html#a5398a60b56e50317884acf50a256b249), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *dstRange, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *brightness, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *contrast, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *saturation)|
|----|----|

返回：-1如果不支持。

|[SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *|[sws_allocVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga7c50ae0b2facbff7afcf88233c94d363) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[length](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a4b0c8e796f240df86333df9dd6ff4361))|
|----|----|

分配并返回具有长度系数的未初始化向量。





|[SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *|[sws_getGaussianVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga25e4916da07d6d4fcfc4417f6c246204) (double variance, double quality)|
|----|----|

返回用于过滤质量的归一化高斯曲线= 3是高品质，较低质量。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_scaleVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gaf79fd6cabde06a0f2d118eb373bf83c6) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), double scalar)|
|----|----|

将a的所有系数缩放为标量值。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_normalizeVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gab40ce33fa0a432f1ba57dda9f740380d) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), double [height](http://ffmpeg.org/doxygen/trunk/tests_2utils_8c.html#ad12fc34ce789bce6c8a05d8a17138534))|
|----|----|

缩放a的所有系数，使其总和等于高度。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *|[sws_getConstVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga507d1691a576b9b04043142bd455830f) (double [c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[length](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a4b0c8e796f240df86333df9dd6ff4361))|
|----|----|

分配并返回具有长度系数的向量，全部具有相同的值c。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *|[sws_getIdentityVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga90a796031eccf4f5891baf20d42e41ae) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配并返回一个只有一个系数的值，值为1.0。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_convVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga90a7634854479a5a172c54939bd07101) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072))|
|----|----|



|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_addVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga18063bec21c8b2ff9ff9edd611a7f01f) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072))|
|----|----|



|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_subVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga0124c4e8cb7405220d671794c4351f70) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072))|
|----|----|



|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_shiftVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga3c303b7b0c40ca2d3df6c3467869e26b) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[shift](http://ffmpeg.org/doxygen/trunk/tests_2swresample_8c.html#a78dd9f7c9440136f1bd89fab33ad2fb2))|
|----|----|



|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *|[sws_cloneVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga596eb7b52058efe735e35ac36929ca46) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912))|
|----|----|



|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_printVec2](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga8b005054bbaed15d803d966186de2ee4) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *log_ctx, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) log_level)|
|----|----|

用av_log（）打印向量的文本表示，如果log_level <= av_log_level。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_freeVec](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga5745e8384365fd5c6b0a3241d6ab767b) ([SwsVector](http://ffmpeg.org/doxygen/trunk/structSwsVector.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912))|
|----|----|



|[SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *|[sws_getDefaultFilter](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gab885b56d729331538917edae65d557dd) (float lumaGBlur, float chromaGBlur, float lumaSharpen, float chromaSharpen, float chromaHShift, float chromaVShift, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) verbose)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_freeFilter](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga17b762761f7a53d596841ac0f2d4a2b5) ([SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *[filter](http://ffmpeg.org/doxygen/trunk/vf__yadif_8c.html#a4aac1797de0c2fcdc5b2b2b3191d252d))|
|----|----|



|struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *|[sws_getCachedContext](http://ffmpeg.org/doxygen/trunk/group__libsws.html#gadffa09f208a3eba7fa3a6b1f74ab77f7) (struct [SwsContext](http://ffmpeg.org/doxygen/trunk/structSwsContext.html) *context, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcW, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcH, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) srcFormat, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dstW, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dstH, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) dstFormat, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4), [SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *srcFilter, [SwsFilter](http://ffmpeg.org/doxygen/trunk/structSwsFilter.html) *dstFilter, const double *param)|
|----|----|

检查上下文是否可以重用，否则重新分配一个上下文。

如果上下文为NULL，则只需调用sws_getContext（）即可获取新的上下文。 否则，检查参数是否已经保存在上下文中。 如果是这样，返回当前上下文。 否则，释放上下文并使用新参数获取新的上下文。


要警告srcFilter和dstFilter不被检查，它们被认为保持不变。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_convertPalette8ToPacked32](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga2a140989dfed29dd91065352b6a52840) (const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) num_pixels, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[palette](http://ffmpeg.org/doxygen/trunk/v4l_8c.html#ad7cfd19025208c4b93648dc94d007eb2))|
|----|----|

将8位调色框转换为颜色深度为32位的帧。

输出框将具有与调色板相同的打包格式。


参数：src：源帧缓冲区

   dst：目的帧缓冲区

num_pixels：要转换的像素数

   palette：数组具有[256]条目，必须匹配src的颜色排列（RGB或BGR）





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[sws_convertPalette8ToPacked24](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga3c3d094eb840e10fbc3a37c20a6c8585) (const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) num_pixels, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[palette](http://ffmpeg.org/doxygen/trunk/v4l_8c.html#ad7cfd19025208c4b93648dc94d007eb2))|
|----|----|

将8位调色框转换为颜色深度为24位的帧。

使用调色板格式“ABCD”，目标帧的格式为“ABC”。


参数：src：源帧缓冲区

   dst：目的帧缓冲区

num_pixels：要转换的像素数

   palette：数组具有[256]条目，必须匹配src的颜色排列（RGB或BGR）





|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[sws_get_class](http://ffmpeg.org/doxygen/trunk/group__libsws.html#ga14ed8a7188230d908f9f28ae9f7ba459) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

获取swsContext的AVClass。

它可以与AV_OPT_SEARCH_FAKE_OBJ结合使用来检查选项。


查看：[av_opt_find()](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gae31ae7fb20113b00108d0ecf53f25664).



