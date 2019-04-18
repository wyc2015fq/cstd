# FFMPEG学习【libavutil】：数据结构（二） - 一世豁然的专栏 - CSDN博客





2017年07月21日 10:06:56[一世豁然](https://me.csdn.net/Explorer_day)阅读数：461








一、AVFrame

AVFrame是引用计数的原始多媒体数据的抽象。





一）、模块

描述其他帧属性的标志。



|#define|[AV_FRAME_FLAG_CORRUPT](https://ffmpeg.org/doxygen/trunk/group__lavu__frame__flags.html#gadddbce4ec0cc2ad4298cf6f266f97f6a)   (1 << 0)|
|----|----|

帧数据可能被破坏，例如，

由于解码错误。






|#define|[AV_FRAME_FLAG_DISCARD](https://ffmpeg.org/doxygen/trunk/group__lavu__frame__flags.html#gaa0c693c70705b18c1c1346f520b6f9cc)   (1 << 2)|
|----|----|

标记需要解码的帧的标志，但不应该被输出。







二）、数据结构

**AVFrameSideData**


结构以保存AVFrame的边数据。


sizeof（AVFrameSideData）不是公共ABI的一部分，所以新的字段可能会添加到最后一个小的凹凸。



|struct|[AVFrameSideData](https://ffmpeg.org/doxygen/trunk/structAVFrameSideData.html){enum AVFrameSideDataType type；uint8_t * data；int size；AVDictionary * metadata；AVBufferRef * buf；}|
|----|----|





**AVFrame**

该结构描述了解码（原始）音频或视频数据。


AVFrame必须使用av_frame_alloc（）分配。 请注意，这只会分配AVFrame本身，数据的缓冲区必须通过其他方式进行管理（见下文）。 AVFrame必须与av_frame_free（）一起释放。


AVFrame通常被分配一次，然后重复使用多次以保存不同的数据（例如，单个AVFrame来保存从解码器接收的帧）。 在这种情况下，av_frame_unref（）将释放框架中的任何引用，并将其重新设置为原始的清洁状态，然后再次重新使用。


AVFrame描述的数据通常通过AVBuffer API进行引用。 基础缓冲区引用存储在AVFrame.buf / AVFrame.extended_buf中。 如果设置了至少一个引用，则AVFrame被认为是引用计数，即如果AVFrame.buf [0]！= NULL。 在这种情况下，每个单个数据平面必须包含在AVFrame.buf或AVFrame.extended_buf中的缓冲区之一中。 所有数据可能有一个缓冲区，或者每个平面有一个单独的缓冲区，或两者之间的任何内容。


sizeof（AVFrame）不是公共ABI的一部分，所以新的字段可能会被添加到最后一个小的碰撞。


可以通过AVOptions访问字段，使用的名称字符串与通过AVOptions可访问的字段的C结构字段名称相匹配。 用于AVFrame的AVClass可以从avcodec_get_frame_class（）获取，



|struct|[AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html){uint8_t * data [AV_NUM_DATA_POINTERS]；// 指向图片/频道平面的指针。int linesize [AV_NUM_DATA_POINTERS]// 对于视频，每个图片行的字节大小。uint8_t ** extended_data;//指向数据平面/通道的指针。int nb_samples;// 此框描述的音频样本数（每个通道）int format;// 帧的格式，-1如果未知或未设置值对应于视频帧的枚举AVPixelFormat，枚举AVSampleFormat为音频）int key_frame;// 1 - >关键帧，0->非关键帧enum AVPictureType pict_type;//帧的图片类型。AVRational sample_aspect_ratio；//视频帧的采样宽高比，如果未知/未指定，则为0/1。int64_t pts;// 以time_base为单位表示时间戳（应向用户显示帧的时间）。attribute_deprecated int64_t pkt_pts;//从AVPacket复制的PTS被解码以产生此帧。int64_t pkt_dts;// 从AVPacket复制的DTS触发返回此帧。int coded_picture_number;//图片编号以比特比顺序int display_picture_number;// 图片编号显示顺序int quality;// 质量（1（好）和FF_LAMBDA_MAX（差））void * opaque;//为用户的一些私人数据attribute_deprecated uint64_t error [AV_NUM_DATA_POINTERS];int repeat_pict;// 当解码时，这表示图片必须延迟多少。int interlaced_frame;// 图片的内容是交错的。int top_field_first;// 如果内容是隔行扫描，则首先显示顶部字段。int palette_has_changed;//告诉用户应用程序调色板已从前一帧更改。int64_t reordered_opaque;//重新排序的不透明64位（通常是整数或双精度浮点PTS，但可以是任何东西）。int sample_rate;//音频数据的采样率。uint64_t channel_layout;// 音频数据的通道布局。AVBufferRef * buf [AV_NUM_DATA_POINTERS];// AVBuffer引用支持此框架的数据。AVBufferRef ** extended_buf;// 对于需要超过AV_NUM_DATA_POINTERS AVBufferRef指针的平面音频，此数组将保存所有不能适合AVFrame.buf的引用。int nb_extended_buf;// extended_buf中的元素数。AVFrameSideData ** side_data;int nb_side_data;int flags;//帧标志，AV_FRAME_FLAGS的组合。enum AVColorRange color_range;// MPEG与JPEG YUV范围。enum AVColorPrimaries color_primaries;enum AVColorTransferCharacteristic color_trc;enum AVColorSpace colorspace;//YUV颜色空间类型。enum AVChromaLocation chroma_location;int64_t best_effort_timestamp;// 帧时间戳使用各种启发式估计，在流时基int64_t pkt_pos;// 从已经输入到解码器的最后一个AVPacket重新排序posint64_t pkt_duration;// 相应分组的持续时间，以AVStream-> time_base单位表示，如果未知，则为0。AVDictionary * metadata;//元数据int decode_error_flags；//解码帧的错误标志，如果解码器产生帧，则将其设置为FF_DECODE_ERROR_xxx标志的组合，但在解码期间存在错误。int channels;// 音频通道数，仅用于音频。int pkt_size;//包含压缩帧的相应分组的大小。attribute_deprecated int8_t * qscale_table;// QP表。attribute_deprecated int qstride;// QP店步伐。attribute_deprecated int qscale_type;AVBufferRef * qp_table_buf;AVBufferRef * hw_frames_ctx;//对于hwaccel格式的帧，这应该是对描述帧的AVHWFramesContext的引用。AVBufferRef * opaque_ref;// AVBufferRef由API用户免费使用。}|
|----|----|

视频尺寸

视频帧。


视频帧的编码维度（以像素为单位），即包含一些明确定义值的矩形的大小。


注意：用于显示/呈现的框架部分进一步受到“裁剪”矩形的限制。



```cpp
int 	width
int 	height
```


裁剪



视频帧。 从帧的顶部/底部/左/右边界丢弃的像素数，以获得用于呈现的帧的子矩形。




```cpp
size_t 	crop_top
size_t 	crop_bottom
size_t 	crop_left
size_t 	crop_right
```





三）、枚举


|enum|[AVFrameSideDataType](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gae01fa7e427274293aacdf2adc17076bc) { [AV_FRAME_DATA_PANSCAN](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca3de91a03b453c956b7274a2392d9a437), [AV_FRAME_DATA_A53_CC](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca4848fd3952b4c2e1c1b85acb628c78a6), [AV_FRAME_DATA_STEREO3D](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bcacec681d6146e8bd4c61b2e1cc88354a8), [AV_FRAME_DATA_MATRIXENCODING](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bcaa69ea5c11a5431bfff9e565714dafe41), [AV_FRAME_DATA_DOWNMIX_INFO](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca71819257ea0bedb7e0740a0ce60e404a), [AV_FRAME_DATA_REPLAYGAIN](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca66f46a9df9fcb8cf212e0693465794e1), [AV_FRAME_DATA_DISPLAYMATRIX](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca3e01b882b6fa04724076067d6d9f8b69),[AV_FRAME_DATA_AFD](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca9aabf5c0b8abc7064bfcc59485e54aea), [AV_FRAME_DATA_MOTION_VECTORS](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bcaf50d0ed0a9a4517341c221be03a06c42), [AV_FRAME_DATA_SKIP_SAMPLES](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca6b0b1ee4315f322922710f65d02a146b), [AV_FRAME_DATA_AUDIO_SERVICE_TYPE](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca51023c847159dcc08776f9dcee5e0c90), [AV_FRAME_DATA_MASTERING_DISPLAY_METADATA](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca6e305f92e325fbd219527452048f16cb), [AV_FRAME_DATA_GOP_TIMECODE](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca03982b828a1069c895d16cda4b3e9fd4), [AV_FRAME_DATA_SPHERICAL](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca12806c5fd4b37dca88312c61965f33e8), [AV_FRAME_DATA_CONTENT_LIGHT_LEVEL](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ggae01fa7e427274293aacdf2adc17076bca311f0afd2dded92ef68ae00f7711aa2d)}|
|----|----|



|enum|[AVActiveFormatDescription](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga612627d8f03d52317992979dfa151d76) { [AV_AFD_SAME](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gga612627d8f03d52317992979dfa151d76a0e9a49946d70eb6ca77cf937ed1a342c) = 8, [AV_AFD_4_3](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gga612627d8f03d52317992979dfa151d76ab118e63239595dceb50deea7351dc1cb) = 9, [AV_AFD_16_9](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gga612627d8f03d52317992979dfa151d76ad8f8850e69a05765363a4a833adc97d8) = 10, [AV_AFD_14_9](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gga612627d8f03d52317992979dfa151d76a0da920de1ba63c73fabe1b9878477a13) = 11, [AV_AFD_4_3_SP_14_9](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gga612627d8f03d52317992979dfa151d76a65edb00603a0d7fdb37cecac94b62cc5) = 13, [AV_AFD_16_9_SP_14_9](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gga612627d8f03d52317992979dfa151d76a488ce7ad4833c96497000dd03b8f0b55) = 14, [AV_AFD_SP_4_3](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gga612627d8f03d52317992979dfa151d76aac53fd979d24b22c0673d1a0bc59e304) = 15 }|
|----|----|





四）、函数


|int64_t|[av_frame_get_best_effort_timestamp](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gacb9d9082c10e625e51a68caceabd4458) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

某些AVFrame字段的访问器。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_best_effort_timestamp](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga8c4a32b4b0314432050a4bbe29171070) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), int64_t [val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|int64_t|[av_frame_get_pkt_duration](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga877a6a0703b74e7e27f196313e160d60) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_pkt_duration](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gaebd80bc8675289737641235473bdc19d) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), int64_t [val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|int64_t|[av_frame_get_pkt_pos](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga2f09c3e454ca78320a8f3be17d0473c1) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_pkt_pos](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga6c137c59cc65afdf2dea1fef903a69e1) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), int64_t [val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|int64_t|[av_frame_get_channel_layout](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gacefc94a29e6741998e408363c8531909) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_channel_layout](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gad695986e6e97256de64bb49b0c11a882) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), int64_t [val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_get_channels](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gac252f01dc52f96b5faf9466ecbe5c150) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_channels](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gae499cd896ae97d2ef97fb4b9d76535c3) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|
|| |



|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_get_sample_rate](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga3a7aec7d00a18357af066da81b17cfcd) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_sample_rate](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga2bc09011d642d2f7b60a683d2daf8cd4) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|[AVDictionary](https://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *|[av_frame_get_metadata](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga4a42eca8e01c59b2cef5507c959e663b) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_metadata](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gace774db7b47a1c708a23f4f24fe85a8d) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [AVDictionary](https://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_get_decode_error_flags](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gac72c0e153987ef0e3febc26452bb3861) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_decode_error_flags](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga0060b8c9949b28790013039e70d830f2) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_get_pkt_size](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga03e2090721bb5d66f292090c7e7e2524) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_pkt_size](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga12742a052df91eeb22a0062cf3da0504) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|[AVDictionary](https://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **|[avpriv_frame_get_metadatap](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gae3797588a2664e4d1cc137b7aaf67b1c) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|int8_t *|[av_frame_get_qp_table](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga3072a9eafc1dba9df289a9aac2dcb083) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *f, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[stride](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#adec60f1216ca1b900f8d776c5b6190e0), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[type](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|



|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_set_qp_table](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga4021d1fbc36d31bcaa986aa6217624c5) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *f, [AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[stride](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#adec60f1216ca1b900f8d776c5b6190e0), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[type](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|
|| |



|enum [AVColorSpace](https://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#aff71a069509a1ad3ff54d53a1c894c85)|[av_frame_get_colorspace](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga03224acf06cbd666f3878a2c35b3f704) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_colorspace](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga43e7dd7b76a6d76811e7f33557fd48e6) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), enum [AVColorSpace](https://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#aff71a069509a1ad3ff54d53a1c894c85)[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|enum [AVColorRange](https://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a3da0bf691418bc22c4bcbe6583ad589a)|[av_frame_get_color_range](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga829e2f3cb3edcc04d76fa9eac37392cd) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_set_color_range](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga862e9789b46539b3edcf0ae4cb106f95) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), enum [AVColorRange](https://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a3da0bf691418bc22c4bcbe6583ad589a)[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|



|const char *|[av_get_colorspace_name](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gaf0e077dd848dd61ee4ac89ef4d3547cc) (enum [AVColorSpace](https://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#aff71a069509a1ad3ff54d53a1c894c85)[val](https://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b))|
|----|----|

获取颜色空间的名称。

返回：标识颜色空间的静态字符串; 可以为NULL。





|[AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *|[av_frame_alloc](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gac700017c5270c79c1e1befdeeb008b2f) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVFrame并将其字段设置为默认值。

结果必须使用av_frame_free（）被释放。


返回：AVFrame充满默认值或失败时为NULL。

注意：这只分配AVFrame本身，而不是数据缓冲区。 那些必须通过其他方式分配，例如 使用av_frame_get_buffer（）或手动。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_free](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga979d73f3228814aee56aeca0636e37cc) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) **[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

释放帧和其中任何动态分配的对象，例如

extended_data。 如果框架被引用计数，则首先将其未引用。


参数：frame：框架被释放。 指针将设置为NULL。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_ref](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga88b0ecbc4eb3453eef3fbefa3bddeb7c) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *dst, const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

设置对源帧描述的数据的新引用。

将框架属性从src复制到dst，并从src为每个AVBufferRef创建一个新的引用。


如果src未引用计数，则会分配新的缓冲区并复制数据。


警告：在调用此函数之前，dst必须已经被av_frame_unref（dst）未被引用或新分配了av_frame_alloc（），否则将会发生未定义的行为。

返回：0成功，否定AVERROR出错





|[AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *|[av_frame_clone](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga46d6d32f6482a3e9c19203db5877105b) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

创建引用与src相同的数据的新框架。

这是av_frame_alloc（）+ av_frame_ref（）的快捷方式。


返回：新创建的AVFrame成功，NULL出错。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_unref](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga0a2b687f9c1c5ed0089b01fd61227108) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

取消引用帧引用的所有缓冲区，并重置框架域。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_move_ref](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga709e62bc2917ffd84c5c0f4e1dfc48f7) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *dst, [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

将src中包含的所有内容移动到dst并重置src。

警告：dst不是未引用的，而是直接覆盖而不读取或取消分配其内容。 在调用此函数之前，请手动调用av_frame_unref（dst），以确保没有内存泄漏。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_get_buffer](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga6b1acbfa82c79bf7fd78d868572f0ceb) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[align](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a262810794572f14d0d17ef53f17dfecc))|
|----|----|

为音频或视频数据分配新的缓冲区。

在调用此函数之前，必须在框架上设置以下字段：


1、格式（视频的像素格式，音频采样格式）

2、视频的宽度和高度

3、音频的nb_samples和channel_layout

此函数将填充AVFrame.data和AVFrame.buf数组，并在必要时分配并填充AVFrame.extended_data和AVFrame.extended_buf。 对于平面格式，将为每个平面分配一个缓冲区。


警告：如果帧已经被分配，调用此函数将泄漏内存。 此外，在某些情况下可能会发生未定义的行为。

参数：frame：在其中存储新缓冲区的帧。

   align：所需缓冲区大小对齐

返回：0成功，否定AVERROR出错。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_is_writable](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga3ba755bada5c3c8883361ef43fb5fb7a) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

检查帧数据是否可写。

返回：如果帧数据是可写的，则为正值（如果且仅当每个底层缓冲区只有一个引用，即存储在此帧中的引用）时，则该值为真。 否则返回0。

如果返回1，答案是有效的，直到av_buffer_ref（）被调用到任何一个底层的AVBufferRefs（例如通过av_frame_ref（）或直接）。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_make_writable](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gadd5417c06f5a6b419b0dbd8f0ff363fd) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

确保帧数据是可写的，如果可能，避免数据复制。

如果帧是可写的，则不执行任何操作，如果不是，则分配新的缓冲区并复制数据。


返回：0成功，否定AVERROR出错。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_copy](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gaec4e92f6e1e75ffaf76e07586fb0c9ed) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *dst, const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

将帧数据从src复制到dst。

此函数不分配任何东西，dst必须已经初始化并分配与src相同的参数。


该功能仅复制帧数据（即数据/扩展数据数组的内容），而不是任何其他属性。


返回：> = 0成功，错误的AVERROR为负。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_frame_copy_props](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gab9b275b114ace0db95c5796bc71f3012) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *dst, const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

仅将src中的“元数据”字段复制到dst。

用于此功能的元数据是不影响缓冲区中数据布局的那些字段。 例如。 pts，采样率（音频）或采样宽高比（视频），但不是宽/高或通道布局。 侧面数据也被复制。






|[AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *|[av_frame_get_plane_buffer](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga7fb7f99dabe10fa5c7c078f4f5f173c5) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[plane](https://ffmpeg.org/doxygen/trunk/avisynth__c_8h.html#a331df3ad3da3df2efdb58c21eedfe6a0))|
|----|----|

获取存储给定数据平面的缓冲区引用。

参数：plane：在frame-> extended_data中感兴趣的数据平面的索引。

返回：包含平面的缓冲区引用，如果输入框无效则为NULL。





|[AVFrameSideData](https://ffmpeg.org/doxygen/trunk/structAVFrameSideData.html) *|[av_frame_new_side_data](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gab4caf9361b00f9b2ab57c222dce3fec4) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), enum [AVFrameSideDataType](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gae01fa7e427274293aacdf2adc17076bc)[type](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

将一个新的边数据添加到一个框架。

参数：frame：应该添加侧面数据的框架

   type：添加侧数据的类型

   size：边数据的大小

返回：新添加的侧面数据成功，NULL出错





|[AVFrameSideData](https://ffmpeg.org/doxygen/trunk/structAVFrameSideData.html) *|[av_frame_get_side_data](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gadec0efb470b1eead6a979333d9deca0c) (const [AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), enum [AVFrameSideDataType](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gae01fa7e427274293aacdf2adc17076bc)[type](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|

返回：指向成功的给定类型的边数据的指针，如果在该帧中没有这种类型的边数据，则为NULL。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_frame_remove_side_data](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga132d6c01d0a21e5b48b96cd7c988de91) ([AVFrame](https://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](https://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209), enum [AVFrameSideDataType](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gae01fa7e427274293aacdf2adc17076bc)[type](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|

如果提供的类型的边数据存在于帧中，则将其从帧中取出并将其移除。





|const char *|[av_frame_side_data_name](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga5cdb93858965e5afd6591792ffb72d99) (enum [AVFrameSideDataType](https://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#gae01fa7e427274293aacdf2adc17076bc)[type](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|

返回：一个标识边数据类型的字符串
            


