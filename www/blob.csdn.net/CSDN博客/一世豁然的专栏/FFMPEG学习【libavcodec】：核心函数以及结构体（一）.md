# FFMPEG学习【libavcodec】：核心函数以及结构体（一） - 一世豁然的专栏 - CSDN博客





2017年06月08日 14:22:18[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1149








                
一、模块

1、AVPacket

使用AVPacket的类型和功能。



2、AVHWAccel



3、AVPicture

使用AVPicture的函数。





二、数据结构

1、struct   AVCodecDescriptor{

enum AVCodecID id；

enum AVMediaType type



const char * name

该描述符描述的编解码器的名称。



const char * long_name

此编解码器的更具描述性的名称。



int props

编解码器属性，AV_CODEC_PROP_ *flag的组合。



const char *const * mime_types

与编解码器相关联的MIME类型。



const struct AVProfile * profiles

如果非NULL，则为此编解码器识别的配置文件数组。

}

该结构描述了由AVCodecID描述的单个编解码器的属性。



2、struct   RcOverride{

int start_frame;

int end_frame;



int qscale;



float quality_factor;

}



3、struct   AVPanScan{

int id；



int width 

宽度和高度在1/16像素



int height;



int16_t position [3][2];

左上角的位置在1/16像素中最多可达3个字段/帧

}

平扫扫描区域。

这指定应显示的区域。 注意，一帧可能有多个这样的区域。



4、struct   AVCPBProperties{

int max_bitrate;

流的最大比特率，以比特/秒为单位。



int min_bitrate;

流的最小比特率，以比特/秒为单位。



int avg_bitrate

流的平均比特率，以位/秒为单位。



int buffer_size;

应用速率控制的缓冲区的大小，以位为单位。



uint64_t vbv_delay;

在27MHz时钟的周期内，接收到与该结构相关联的分组的时间之间的延迟和应当被解码的时间。

}

该结构描述了编码比特流的比特率特性。

它大致对应于用于H.264 / HEVC的用于MPEG-2或HRD参数的VBV参数的子集。



5、struct   AVCodecContext{

const AVClass * av_class

  关于av_log的结构信息


int log_level_offset


enum  AVMediaType codec_type


const struct AVCodec * codec


attribute_deprecated char codec_name [32]


enum  AVCodecID codec_id


unsigned int codec_tag

 fourcc（LSB优先，所以“ABCD” - >（'D'<< 24）+（'C'<< 16）+（'B'<< 8）+'A'）。更多...


attribute_deprecated unsigned int stream_codec_tag


void * priv_data


struct AVCodecInternal * internal

 用于内部数据的专用上下文。更多...


void * opaque

 用户的私人数据可用于携带应用程序特定的东西。更多...


int64_t bit_rate

 平均比特率更多...


int bit_rate_tolerance

 允许比特流从引用偏离的位数。更多...


int global_quality

 编码解码器的全局质量，每帧不能更改。更多...


int compression_level


int标志

 AV_CODEC_FLAG_ *。更多...


int flags2

 AV_CODEC_FLAG2_ *。更多...


uint8_t * extradata

  一些编解码器需要/可以使用像Huffman表这样的extradata。更多...


int extradata_size


AVRational time_base

 这是表示帧时间戳的基本时间单位（以秒为单位）。更多...


int ticks_per_frame

 对于某些编解码器，时基比帧速率更接近现场速率。更多...


int延迟

 编解码延迟更多...


int width

  图片宽度/高度。更多...


int高度


int coded_width

 比特流宽度/高度可以不同于宽度/高度，例如更多...


int coded_height


int gop_size

 一组图片中的图片数量，或0个用于intra_only更多...


enum AVPixelFormat pix_fmt

  像素格式，请参阅AV_PIX_FMT_xxx。更多...


attribute_deprecated int me_method

 此选项什么也不做。更多...


void（* draw_horiz_band）（struct AVCodecContext * s，const AVFrame * src，int offset [AV_NUM_DATA_POINTERS]，int y，int type，int height）

 如果非NULL，则libavcodec解码器调用“draw_horiz_band”来绘制水平带。更多...


枚举AVPixelFormat（* get_format）（struct AVCodecContext * s，const enum AVPixelFormat * fmt）

 回调协商pixelFormat更多...


int max_b_frames

 非B帧之间的最大B帧数注意：输出将相对于输入延迟max_b_frames + 1。更多...


float b_quant_factor

 IP和B帧之间的qscale因子如果> 0，则将使用最后一个P帧量化器（q = lastp_q * factor + offset）。更多...


attribute_deprecated int rc_strategy


attribute_deprecated int b_frame_strategy


float b_quant_offset

 IP和B帧之间的qscale偏移量更多...


int has_b_frames

  解码器中帧重排序缓冲区的大小。更多...


attribute_deprecated int mpeg_quant


float i_quant_factor

 P和I帧之间的qscale因子如果> 0，则将使用最后一个P帧量化器（q = lastp_q * factor + offset）。更多...


float i_quant_offset

 P和I帧之间的qscale偏移更多...


float lumi_masking

 亮度屏蔽（0-> disabled）更多...


float temporal_cplx_masking

 临时复杂掩蔽（0->禁用）更多...


float spatial_cplx_masking

  空间复杂度掩蔽（0-> disabled）更多...


float p_masking

 p块屏蔽（0-> disabled）更多...


float dark_masking

 黑暗掩蔽（0->禁用）更多...


int slice_count

 切片数更多...


attribute_deprecated int prediction_method


int * slice_offset

 帧偏移量以字节为单位更多...


AVRational sample_aspect_ratio

  样本宽高比（0如果未知）这是像素的宽度除以像素的高度。更多...


int me_cmp

  运动估计比较功能更多...


int me_sub_cmp

 子像素运动估计比较功能更多...


int mb_cmp

 宏块比较功能（不支持）更多...


int ildct_cmp

 隔行DCT比较功能更多...


int dia_size

 ME钻石尺寸和形状。更多...


int last_predictor_count

 以前的MV预测值（2a + 1×2a + 1平方）的数量更多...


attribute_deprecated int pre_me


int me_pre_cmp

  运动估计预制比较功能更多...


int pre_dia_size

 ME预制钻石尺寸和形状。更多...


int me_subpel_quality

  子弹ME质量更多...


attribute_deprecated int dtg_active_format

 DTG有效格式信息（仅在DVB MPEG-2传输流中使用的附加宽高比信息）0如果未设置。更多...


int me_range

 子单位最大运动估计搜索范围如果0则无限制。

attribute_deprecated int intra_quant_bias


attribute_deprecated int inter_quant_bias


int slice_flags

 切片标志


int mb_decision

  宏块决策模式


uint16_t * intra_matrix

 定制内部量化矩阵


uint16_t * inter_matrix

 定制互量化矩阵



}

主要的外部API结构。



6、struct   AVProfile{

int profile；



const char * name；

配置文件的简称

}



7、struct   AVCodec{

const char * name;

编解码器实现的名称。



const char * long_name

 编解码器的描述性名称，意思是比名字更人性化。



enum AVMediaType type



enum AVCodecID id



int capabilities

 编解码器功能。



const AVRational * supported_framerates

 数组支持的帧速率，或NULL（如果有的话）数组被{0,0}终止



enum AVPixelFormat * pix_fmts

 支持的像素格式的数组，如果未知，则为NULL，数组以-1



const int * supported_samplerates

 支持的音频采样器阵列，如果未知，则为NULL，数组终止于0



enum AVSampleFormat * sample_fmts

 数组支持的样本格式，或NULL如果未知，数组将被-1终止



const uint64_t * channel_layouts

 支持通道布局数组，如果未知，则返回NULL。



uint8_t max_lowres

 解码器最低支持的最小值。



const AVClass * priv_class

 AVClass为私有上下文。



const AVProfile * profiles

 数组识别的配置文件，如果未知，则为NULL，数组由{FF_PROFILE_UNKNOWN}终止



int priv_data_size



struct AVCodec * next



const AVCodecDefault * defaults

 专用编解码器默认值。



void(* init_static_data )(struct AVCodec *codec)

 初始化编解码器静态数据，从avcodec_register（）调用。



int(* init )(AVCodecContext *)



int(* encode_sub )(AVCodecContext *, uint8_t *buf, int buf_size, const struct AVSubtitle *sub)



int(* encode2 )(AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame, int *got_packet_ptr)

 将数据编码到AVPacket。



int(* decode )(AVCodecContext *, void *outdata, int *outdata_size, AVPacket *avpkt)



int(* close )(AVCodecContext *)



int(* send_frame )(AVCodecContext *avctx, const AVFrame *frame)

使用解耦的数据包/帧数据流编码API。



int(* receive_packet )(AVCodecContext *avctx, AVPacket *avpkt)



int(* receive_frame )(AVCodecContext *avctx, AVFrame *frame)

 使用解耦的数据包/帧数据流解码API。



void(* flush )(AVCodecContext *)

flush缓冲区



int caps_internal

 内部编解码器功能。



const char * bsfs

仅解码，在解码之前应用于分组的比特流筛选器的逗号分隔列表



帧级线程支持功能

int(* init_thread_copy )(AVCodecContext *)

如果已定义，则在创建线程上下文时调用它。



int(* update_thread_context )(AVCodecContext *dst, const AVCodecContext *src)

 将必要的上下文变量从以前的线程上下文复制到当前的。

}



8、struct   AVSubtitleRect{

int x；

pict的左上角，当pict未设置时未定义



int y；

pict的左上角，当pict未设置时未定义



int w；

pict的宽度，未设置pict时未定义



int h；

高度的图像，未设置pict时未定义



int nb_colors

pict中的颜色数量，未设置pict时未定义



attribute_deprecated AVPicture pict



uint8_t * data [4]；

data + linesize为此字幕的位图。



int linesize [4]；

enum AVSubtitleType type；



char * text;

0终止普通UTF-8文本



char * ass;

0端接ASS / SSA兼容事件线。



int  flag;

}



9、struct   AVSubtitle{

uint16_t format；



uint32_t start_display_time；



uint32_t end_display_time；



unsigned num_rects；



AVSubtitleRect ** rects



int64_t pts；

与数据包相同，在AV_TIME_BASE中。

}



10、struct   AVCodecParameters{

enum AVMediaType codec_type;

编码数据的一般类型。



enum AVCodecID codec_id;

编码数据的特定类型（使用的编解码器）。



uint32_t codec_tag;

关于编解码器的附加信息（对应于AVI FOURCC）。



uint8_t * extradata;

初始化解码器所需的额外二进制数据，依赖于编解码器。



int extradata_size;

扩展数据库内容的大小（以字节为单位）。



int format;

int64_t bit_rate；

编码数据的平均比特率（以每秒位数表示）。



int bits_per_coded_sample

编码字中每个样本的位数。



int bits_per_raw_sample

这是每个输出样本中有效位的数量。



int profile

流遵循的编解码器特定比特流限制。



int level



int width

仅仅视频有用



int height

AVRational sample_aspect_ratio

仅仅视频有用



enum AVFieldOrder field_order

仅仅视频有用



enum AVColorRange color_range

仅仅视频有用



enum AVColorPrimaries color_primaries



enum AVColorTransferCharacteristic color_trc



enum AVColorSpace color_space



enum AVChromaLocation chroma_location



int video_delay

仅仅视频有用



uint64_t channel_layout

仅仅音频有用



int channels

仅仅音频有用



int sample_rate

仅仅音频有用



int block_align

仅仅音频有用



int frame_size

仅仅音频有用

int initial_padding

仅仅音频有用



int trailing_padding

仅仅音频有用



int seek_preroll

仅仅音频有用


}







三、宏

#define AV_CODEC_PROP_INTRA_ONLY（1 << 0）


编解码器仅使用内部压缩。





#define AV_CODEC_PROP_LOSSY（1 << 1）


编解码器支持有损压缩。





#define AV_CODEC_PROP_LOSSLESS（1 << 2）


编解码器支持无损压缩。





＃define AV_CODEC_PROP_REORDER（1 << 3）


编解码器支持帧重排序。





#define AV_CODEC_PROP_BITMAP_SUB（1 << 16）


字幕编解码器是基于位图的可以从AVSubtitleRect-> pict字段读取的解码AVSubtitle数据。





#define AV_CODEC_PROP_TEXT_SUB（1 << 17）


字幕编解码器是基于文本的。





#define FF_INPUT_BUFFER_PADDING_SIZE 32




#define FF_MIN_BUFFER_SIZE 16384




#define FF_MAX_B_FRAMES 16





#define AV_CODEC_FLAG_UNALIGNED（1 << 0）


允许解码器产生不符合CPU要求的数据平面的帧





#define AV_CODEC_FLAG_QSCALE（1 << 1）


使用固定的qscale。





#define AV_CODEC_FLAG_4MV（1 << 2）


H.263每MB允许/高级预测4 MV。





#define AV_CODEC_FLAG_OUTPUT_CORRUPT（1 << 3）


输出即使那些可能已损坏的帧。





#define AV_CODEC_FLAG_QPEL（1 << 4）


使用qpel MC。





#define AV_CODEC_FLAG_PASS1（1 << 9）


在第一遍模式下使用内部2通速率控制。





#define AV_CODEC_FLAG_PASS2（1 << 10）


在二次通过模式下使用内部2通速率控制。





#define AV_CODEC_FLAG_LOOP_FILTER（1 << 11）


环路滤波器。





#define AV_CODEC_FLAG_GRAY（1 << 13）


仅解码/编码灰度。





#define AV_CODEC_FLAG_PSNR（1 << 15）


编码时将设置错误[？]变量。





#define AV_CODEC_FLAG_TRUNCATED（1 << 16）


输入比特流可能在随机位置而不是仅在帧边界被截断。





#define AV_CODEC_FLAG_INTERLACED_DCT（1 << 18）


使用隔行DCT。





#define AV_CODEC_FLAG_LOW_DELAY（1 << 19）


强制低延迟





#define AV_CODEC_FLAG_GLOBAL_HEADER（1 << 22）


将全局头文件放在extradata中，而不是每个关键帧。





#define AV_CODEC_FLAG_BITEXACT（1 << 23）


只使用bitexact的东西（除了（I）DCT）。





#define AV_CODEC_FLAG_AC_PRED（1 << 24）

 H.263高级帧内编码/ MPEG-4交流预测。





#define AV_CODEC_FLAG_INTERLACED_ME（1 << 29）

 隔行运动估计





#define AV_CODEC_FLAG_CLOSED_GOP（1U << 31）





#define AV_CODEC_FLAG2_FAST（1 << 0）

 允许不符合规范的加速技巧。





#define AV_CODEC_FLAG2_NO_OUTPUT（1 << 2）

 跳过比特流编码。





#define AV_CODEC_FLAG2_LOCAL_HEADER（1 << 3）

 在每个关键帧而不是在extradata中放置全局头。





#define AV_CODEC_FLAG2_DROP_FRAME_TIMECODE（1 << 13）

 时间码采用丢帧格式。





#define AV_CODEC_FLAG2_CHUNKS（1 << 15）

 输入比特流可能在分组边界而不是仅在帧边界被截断。





#define AV_CODEC_FLAG2_IGNORE_CROP（1 << 16）

 丢弃SPS的裁剪信息。





#define AV_CODEC_FLAG2_SHOW_ALL（1 << 22）

 在第一个关键帧之前显示所有帧。





#define AV_CODEC_FLAG2_EXPORT_MVS（1 << 28）

 通过帧侧数据导出运动矢量。





#define AV_CODEC_FLAG2_SKIP_MANUAL（1 << 29）

 不要跳过样本并将跳过信息导出为边框数据。





#define AV_CODEC_FLAG2_RO_FLUSH_NOOP（1 << 30）

 不要重置ASS ReadRrder字段（字幕解码）





#define AV_CODEC_CAP_DRAW_HORIZ_BAND（1 << 0）

 解码器可以使用draw_horiz_band回调。





#define AV_CODEC_CAP_DR1（1 << 1）

 Codec使用get_buffer（）分配缓冲区并支持自定义分配器。





#define AV_CODEC_CAP_TRUNCATED   (1 << 3)





#define AV_CODEC_CAP_DELAY（1 << 5）

 编码器或解码器需要在结尾使用NULL输入进行刷新，以提供完整和正确的输出。





#define AV_CODEC_CAP_SMALL_LAST_FRAME（1 << 6）

 编码器可以馈送一个较小尺寸的最终帧。





#define AV_CODEC_CAP_HWACCEL_VDPAU（1 << 7）

 编解码器可以导出HW解码数据（VDPAU）。





#define AV_CODEC_CAP_SUBFRAMES（1 << 8）

 编解码器可以每个AVPacket输出多个帧通常，分离器一次返回一个帧，不做的分离器连接到解析器，将其返回到合适的帧中。





#define AV_CODEC_CAP_EXPERIMENTAL（1 << 9）

 编解码器是实验性的，因此避免使用非实验编码器。





#define AV_CODEC_CAP_CHANNEL_CONF（1 << 10）

 编解码器应该填写通道配置和采样而不是容器。





#define AV_CODEC_CAP_FRAME_THREADS（1 << 12）

 编解码器支持帧级多线程。





#define AV_CODEC_CAP_SLICE_THREADS（1 << 13）

 编解码器支持基于片段（或基于分区的）多线程。





#define AV_CODEC_CAP_PARAM_CHANGE（1 << 14）

 编解码器支持任何时候更改的参数。





#define AV_CODEC_CAP_AUTO_THREADS（1 << 15）

 Codec支持avctx-> thread_count == 0（auto）。





#define AV_CODEC_CAP_VARIABLE_FRAME_SIZE（1 << 16）

 音频编码器支持在每个呼叫中​​接收不同数量的采样。





#define AV_CODEC_CAP_AVOID_PROBING（1 << 17）

 解码器不是探测的首选。





#define AV_CODEC_CAP_INTRA_ONLY 0x40000000

 编解码器仅在内部。





#define AV_CODEC_CAP_LOSSLESS 0x80000000

 编解码器是无损的。





#define CODEC_FLAG_UNALIGNED AV_CODEC_FLAG_UNALIGNED

 允许解码器产生不符合CPU要求的数据平面的帧





#define CODEC_FLAG_QSCALE   AV_CODEC_FLAG_QSCALE



#define CODEC_FLAG_4MV   AV_CODEC_FLAG_4MV



#define CODEC_FLAG_OUTPUT_CORRUPT   AV_CODEC_FLAG_OUTPUT_CORRUPT



#define CODEC_FLAG_QPEL   AV_CODEC_FLAG_QPEL



#define CODEC_FLAG_GMC   0x0020



#define CODEC_FLAG_MV0   0x0040



#define CODEC_FLAG_INPUT_PRESERVED   0x0100




#define CODEC_FLAG_PASS1   AV_CODEC_FLAG_PASS1



#define CODEC_FLAG_PASS2   AV_CODEC_FLAG_PASS2



#define CODEC_FLAG_GRAY   AV_CODEC_FLAG_GRAY



#define CODEC_FLAG_EMU_EDGE   0x4000



#define CODEC_FLAG_PSNR   AV_CODEC_FLAG_PSNR



#define CODEC_FLAG_TRUNCATED   AV_CODEC_FLAG_TRUNCATED



#define CODEC_FLAG_NORMALIZE_AQP   0x00020000


#define CODEC_FLAG_INTERLACED_DCT   AV_CODEC_FLAG_INTERLACED_DCT


#define CODEC_FLAG_LOW_DELAY   AV_CODEC_FLAG_LOW_DELAY


#define CODEC_FLAG_GLOBAL_HEADER   AV_CODEC_FLAG_GLOBAL_HEADER


#define CODEC_FLAG_BITEXACT   AV_CODEC_FLAG_BITEXACT


#define CODEC_FLAG_AC_PRED   AV_CODEC_FLAG_AC_PRED


#define CODEC_FLAG_LOOP_FILTER   AV_CODEC_FLAG_LOOP_FILTER


#define CODEC_FLAG_INTERLACED_ME   AV_CODEC_FLAG_INTERLACED_ME


#define CODEC_FLAG_CLOSED_GOP   AV_CODEC_FLAG_CLOSED_GOP


#define CODEC_FLAG2_FAST   AV_CODEC_FLAG2_FAST


#define CODEC_FLAG2_NO_OUTPUT   AV_CODEC_FLAG2_NO_OUTPUT


#define CODEC_FLAG2_LOCAL_HEADER   AV_CODEC_FLAG2_LOCAL_HEADER


#define CODEC_FLAG2_DROP_FRAME_TIMECODE   AV_CODEC_FLAG2_DROP_FRAME_TIMECODE


#define CODEC_FLAG2_IGNORE_CROP   AV_CODEC_FLAG2_IGNORE_CROP


#define CODEC_FLAG2_CHUNKS   AV_CODEC_FLAG2_CHUNKS


#define CODEC_FLAG2_SHOW_ALL   AV_CODEC_FLAG2_SHOW_ALL


#define CODEC_FLAG2_EXPORT_MVS   AV_CODEC_FLAG2_EXPORT_MVS


#define CODEC_FLAG2_SKIP_MANUAL   AV_CODEC_FLAG2_SKIP_MANUAL





#define CODEC_CAP_DRAW_HORIZ_BAND AV_CODEC_CAP_DRAW_HORIZ_BAND

 解码器可以使用draw_horiz_band回调





#define CODEC_CAP_DR1 AV_CODEC_CAP_DR1

 Codec使用get_buffer（）分配缓冲区并支持自定义分配器。





#define CODEC_CAP_TRUNCATED AV_CODEC_CAP_TRUNCATED





#define CODEC_CAP_DELAY AV_CODEC_CAP_DELAY

 编码器或解码器需要在结尾使用NULL输入进行刷新，以提供完整和正确的输出。





#define CODEC_CAP_SMALL_LAST_FRAME AV_CODEC_CAP_SMALL_LAST_FRAME

 编码器可以馈送一个较小尺寸的最终帧。





#define CODEC_CAP_HWACCEL_VDPAU AV_CODEC_CAP_HWACCEL_VDPAU

 编解码器可以导出HW解码数据（VDPAU）。





#define CODEC_CAP_SUBFRAMES AV_CODEC_CAP_SUBFRAMES

 编解码器可以每个AVPacket输出多个帧通常，分离器一次返回一个帧，不做的分离器连接到解析器，将其返回到合适的帧中。





#define CODEC_CAP_EXPERIMENTAL AV_CODEC_CAP_EXPERIMENTAL

 编解码器是实验性的，因此避免使用非实验编码器。





#define CODEC_CAP_CHANNEL_CONF AV_CODEC_CAP_CHANNEL_CONF

 编解码器应该填写通道配置和采样而不是容器。





#define CODEC_CAP_NEG_LINESIZES 0x0800





#define CODEC_CAP_FRAME_THREADS AV_CODEC_CAP_FRAME_THREADS

 编解码器支持帧级多线程。





#define CODEC_CAP_SLICE_THREADS AV_CODEC_CAP_SLICE_THREADS

 编解码器支持基于片段（或基于分区的）多线程。





#define CODEC_CAP_PARAM_CHANGE AV_CODEC_CAP_PARAM_CHANGE

 编解码器支持任何时候更改的参数。





#define CODEC_CAP_AUTO_THREADS AV_CODEC_CAP_AUTO_THREADS

 Codec支持avctx-> thread_count == 0（auto）。





#define CODEC_CAP_VARIABLE_FRAME_SIZE AV_CODEC_CAP_VARIABLE_FRAME_SIZE

 音频编码器支持在每个呼叫中​​接收不同数量的采样。





#define CODEC_CAP_INTRA_ONLY AV_CODEC_CAP_INTRA_ONLY

 编解码器仅在内部。





#define CODEC_CAP_LOSSLESS AV_CODEC_CAP_LOSSLESS

 编解码器是无损的。





#define HWACCEL_CODEC_CAP_EXPERIMENTAL 0x0200

 HWAccel是实验性的，因此避免了非实验编解码器。





#define MB_TYPE_INTRA4x4   0x0001


#define MB_TYPE_INTRA16x16   0x0002


#define MB_TYPE_INTRA_PCM   0x0004


#define MB_TYPE_16x16   0x0008


#define MB_TYPE_16x8   0x0010


#define MB_TYPE_8x16   0x0020


#define MB_TYPE_8x8   0x0040


#define MB_TYPE_INTERLACED   0x0080


#define MB_TYPE_DIRECT2   0x0100


#define MB_TYPE_ACPRED   0x0200


#define MB_TYPE_GMC   0x0400


#define MB_TYPE_SKIP   0x0800


#define MB_TYPE_P0L0   0x1000


#define MB_TYPE_P1L0   0x2000


#define MB_TYPE_P0L1   0x4000


#define MB_TYPE_P1L1   0x8000


#define MB_TYPE_L0   (MB_TYPE_P0L0 | MB_TYPE_P1L0)


#define MB_TYPE_L1   (MB_TYPE_P0L1 | MB_TYPE_P1L1)


#define MB_TYPE_L0L1   (MB_TYPE_L0 | MB_TYPE_L1)


#define MB_TYPE_QUANT   0x00010000


#define MB_TYPE_CBP   0x00020000


#define FF_QSCALE_TYPE_MPEG1   0


#define FF_QSCALE_TYPE_MPEG2   1


#define FF_QSCALE_TYPE_H264   2


#define FF_QSCALE_TYPE_VP56   3


#define AV_GET_BUFFER_FLAG_REF   (1 << 0)
解码器将保留对帧的引用，并且可以稍后重用它。


#define AV_SUBTITLE_FLAG_FORCED   0x00000001








四、枚举


enum  [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gaadca229ad2c20e060a14fec08a5cc7ce)AVCodecID { 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea48761c50b102343e2648c3e28a94e7ab)AV_CODEC_ID_NONE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf019b13f4891b36ae579cd7bc96d1f78)AV_CODEC_ID_MPEG1VIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf614abb36f7c5d0c42ed522c1d6c99f0)AV_CODEC_ID_MPEG2VIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea11cf9a49627f70f6ab33d3bf93752ae4)AV_CODEC_ID_H261, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac3d126631b15729d01d427b642c3bc9e)AV_CODEC_ID_H263, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8565d2953c28245356f94c3fde6480ea)AV_CODEC_ID_RV10, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab0bbb03eef201676cd723df390cdc795)AV_CODEC_ID_RV20, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadaaf3a9acfa91bd739c50299f6779409)AV_CODEC_ID_MJPEG, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa9d4e4b5bcc781a464a6099c26879d87)AV_CODEC_ID_MJPEGB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabcfdae1f16ef5a193eebc9358d4c944d)AV_CODEC_ID_LJPEG, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadfde4827ec22ff5303b888d0f8f5ad2e)AV_CODEC_ID_SP5X, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceacb0e7018507b17ee90adb93fc65a7ee8)AV_CODEC_ID_JPEGLS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead95dfdf68b462db508b45fb693a4c09e)AV_CODEC_ID_MPEG4, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1b88854d7b37174f45df205456d7c1cc)AV_CODEC_ID_RAWVIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea075f56399fdfd9f09a6bd58fd9f74b46)AV_CODEC_ID_MSMPEG4V1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5791eefbde4ab67a4c786c5f06371a61)AV_CODEC_ID_MSMPEG4V2, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf2c51d3f6f70b731f3b316d352e0fc62)AV_CODEC_ID_MSMPEG4V3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea828ab11de849f8fb98d1d3bf9788a5cb)AV_CODEC_ID_WMV1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5362e286d0d453bee3940514f3f29641)AV_CODEC_ID_WMV2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf223d4b1dc9e6ed1b550d02a35c2542c)AV_CODEC_ID_H263P, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae91678afc0c9bbaf38681c6c1c5441fa)AV_CODEC_ID_H263I, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea130d77683c690fdeeb9de1e9409d06d4)AV_CODEC_ID_FLV1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead6962d77136d74ee2ff0b8c9015b3cca)AV_CODEC_ID_SVQ1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea35e8d1b643beef5a57fffbd2b3f44050)AV_CODEC_ID_SVQ3, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa10d44ae477c03846836534fadd73b06)AV_CODEC_ID_DVVIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2240162eb5e79d7e0b0e4678704276c6)AV_CODEC_ID_HUFFYUV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea09bdb43a318848e4a3f2436d121b9ec1)AV_CODEC_ID_CYUV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3d4c051c8b18a3c18750676a308c10be)AV_CODEC_ID_H264, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea82163fef605d01abf8632cd723551062)AV_CODEC_ID_INDEO3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea11920474f1c6dada14ac2cfd36f67601)AV_CODEC_ID_VP3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8c3ef0a1eec3cf22756a1323989ff4d8)AV_CODEC_ID_THEORA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadadb9fb777ad4b2b7e06106ca2d97ebb)AV_CODEC_ID_ASV1, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae6946708181cca9b487479b47f78d7f0)AV_CODEC_ID_ASV2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea610509eb997372e2ea71cb6b4dfb553f)AV_CODEC_ID_FFV1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafb4cfd6c985bd71551197b8b6e49e88d)AV_CODEC_ID_4XM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7f855d1c2c04aab20a6cd0d94fe66c52)AV_CODEC_ID_VCR1, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5be35dda3c6b97d6ee09ce0b121a7c47)AV_CODEC_ID_CLJR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea985c039dc1f95273c5abe5de81e09738)AV_CODEC_ID_MDEC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaeb0858785fab79aa09b7347a513477b9)AV_CODEC_ID_ROQ, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7558a7a217008c0fe793423231671325)AV_CODEC_ID_INTERPLAY_VIDEO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea07428ed0f786b688e6c0bc3e205e0f35)AV_CODEC_ID_XAN_WC3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea37e35c26824829e4ab7b41652fc77e1e)AV_CODEC_ID_XAN_WC4, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2e83b6080158a68a1e2486e342d2c36d)AV_CODEC_ID_RPZA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5872ef870db6ecee94f18da1a0f18979)AV_CODEC_ID_CINEPAK, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea32ab953bccf3b838c7f98cb501140a1f)AV_CODEC_ID_WS_VQA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4a5c958de80fb367f0dfe5a3b8902e29)AV_CODEC_ID_MSRLE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf7cbec610a4b659549983a23d2aa416f)AV_CODEC_ID_MSVIDEO1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea06053505e009178c15151d209f8e4c81)AV_CODEC_ID_IDCIN, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0dba75c225159195b003b226d920398b)AV_CODEC_ID_8BPS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaac568d9f1ca9f8ab8cb2f38a6dfdaab0)AV_CODEC_ID_SMC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8f07f59bc63ae0fd62f026322b663fb3)AV_CODEC_ID_FLIC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea92af537484e1864240903787f0788e52)AV_CODEC_ID_TRUEMOTION1, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8be7b1f2d9331d7078e9a656b7568dd3)AV_CODEC_ID_VMDVIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf657b6dfcb6ca7f36ceced3557ea4327)AV_CODEC_ID_MSZH, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea506ef7fbaa9c05f411009bf39fd80a79)AV_CODEC_ID_ZLIB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2372807239c429f6313bc79d33bab472)AV_CODEC_ID_QTRLE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5e6a2bcf37a0248b847b5250e716eafd)AV_CODEC_ID_TSCC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6917fb5fd2ed7db4290a807d4cadb8d6)AV_CODEC_ID_ULTI, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead44bc5c8fb068adf11284846382504cf)AV_CODEC_ID_QDRAW, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea38eb43042ca48fb4b1d39ba5ebdd3ff6)AV_CODEC_ID_VIXL, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceacd840f1de66e837c1aba7544bc0564ff)AV_CODEC_ID_QPEG, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea13651c88d4a774a1012b40f6087b5fb9)AV_CODEC_ID_PNG, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab3621092974c8cff284604d55f75a786)AV_CODEC_ID_PPM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea76ea8dd8b2cef999ab9652c87219d764)AV_CODEC_ID_PBM, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea46d3681388e8806a1926cc0bb633759f)AV_CODEC_ID_PGM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaaf73f1f5476aa4cb4a106d1d9bc6ad5b)AV_CODEC_ID_PGMYUV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7096f796b28213e016f407ebe2b23b67)AV_CODEC_ID_PAM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea026b9f64393c4d53d5940620d5a7f7c8)AV_CODEC_ID_FFVHUFF, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa4566a412c746c9267e2461262d396fd)AV_CODEC_ID_RV30, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9ff341616ebac6400d57194606de5593)AV_CODEC_ID_RV40, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7765c5c02c05ea1a96ef1ca35dad068f)AV_CODEC_ID_VC1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadb3fab61116754cc08f3168dd26ff776)AV_CODEC_ID_WMV3, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae32415a39c6511d5c5ed343731dfe3d8)AV_CODEC_ID_LOCO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac7c694cd26c87c8b94b0aa0408d16fc6)AV_CODEC_ID_WNV1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaad78457cd0512dec024c50ded9255d33)AV_CODEC_ID_AASC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea74f06f8ccfcf8e07a2cba62ac83f8b68)AV_CODEC_ID_INDEO2, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf7796b986db33449a592c3ae1e7f2e9b)AV_CODEC_ID_FRAPS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabc11317c3e8c99c1b41b86b0ca61878b)AV_CODEC_ID_TRUEMOTION2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabb04ef46d4490540a3a0243aaa919fc9)AV_CODEC_ID_BMP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6518b5819aa46b335de4d8b920fd5b67)AV_CODEC_ID_CSCD, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa6fef7fb585f95d263612657cda29573)AV_CODEC_ID_MMVIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae991bfabdf946218e8d1a70f90dcedc3)AV_CODEC_ID_ZMBV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4b48cce6b5f054407a9fb07fcacc81fe)AV_CODEC_ID_AVS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea201ece382d697009aa13d77a482d8365)AV_CODEC_ID_SMACKVIDEO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9b64d2d7aa60ac493e3f6b738dbe8655)AV_CODEC_ID_NUV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9d198cf724eb3cd182110ca085a527c4)AV_CODEC_ID_KMVC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaee192c6a3ea6b52b881e2ed971a781fc)AV_CODEC_ID_FLASHSV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8b076e20f47fa72362f22ce00b9f976a)AV_CODEC_ID_CAVS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4ff1478f2b0f3bc728f6cce39d44b9f0)AV_CODEC_ID_JPEG2000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea526f6bc0af65684f67389236b911c771)AV_CODEC_ID_VMNC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabbb5f61cec6b4083489462ae1a39bc17)AV_CODEC_ID_VP5, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0f14d3ea3598ac4a1d4a9d73e01b56b0)AV_CODEC_ID_VP6, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea14aac7455a40185b56f48bfac4647552)AV_CODEC_ID_VP6F, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea45e6ff6972ee9e4bffe817a38d2beb70)AV_CODEC_ID_TARGA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea29590a30babd163dfeebabfb0248925e)AV_CODEC_ID_DSICINVIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8b6c63bedfe10e32e2d15a9b612a6ef5)AV_CODEC_ID_TIERTEXSEQVIDEO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab287c8cb5ab084d156b3f8c11fa698d6)AV_CODEC_ID_TIFF, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea35d8b1cf0066b5a9da9e86e49b0c3e45)AV_CODEC_ID_GIF, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf2e6523927efc9db7ed6e3dcaddfddf2)AV_CODEC_ID_DXA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead36347b6ae291fc3cc95f84aecf8035a)AV_CODEC_ID_DNXHD, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaed29a65f7c6b258316a921f3d3208e10)AV_CODEC_ID_THP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6c72c01a979f8558551509cfac3085f9)AV_CODEC_ID_SGI, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac370116b025c58e06681d8d1c16ae711)AV_CODEC_ID_C93, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0e5fee8b7f3755568e85dd87cef88ed6)AV_CODEC_ID_BETHSOFTVID, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2c3dd83dedaeb8bcdb9286d786f4fd79)AV_CODEC_ID_PTX, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea82308080bbcb8e07b9992e813dfa8540)AV_CODEC_ID_TXD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8abead4b8a87bf4a142b7db7e50cef8e)AV_CODEC_ID_VP6A, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafa26899e6acd71441007e6c35461cfc3)AV_CODEC_ID_AMV, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4b23025c7bfcd9117c3e0eca77a20c19)AV_CODEC_ID_VB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea828ac6d0106288cc1980ae96ef1d3c2e)AV_CODEC_ID_PCX, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5d9faecd012c9a2d47e40e0a268fd2c6)AV_CODEC_ID_SUNRAST, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea24bd7701f087bbad4cbf78438849de57)AV_CODEC_ID_INDEO4, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea16f9edf141d19e55734dbfcd3857ce31)AV_CODEC_ID_INDEO5, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae1ebc21e053a0fbe49029f63b926bb6c)AV_CODEC_ID_MIMIC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa06c988fc66d8618cc6c2ed149dcbe52)AV_CODEC_ID_RL2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea091eca04b0aa5eb2981e74ce78f32da7)AV_CODEC_ID_ESCAPE124, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea00d5e8ca007d62fe704bb22b2be7b556)AV_CODEC_ID_DIRAC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab5ba6986b47894bb8611455d14b1f250)AV_CODEC_ID_BFI, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea12d0508cd1403f8272d98de4f11b4cef)AV_CODEC_ID_CMV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea26b515ca6810bfefc17beadcff33f1fb)AV_CODEC_ID_MOTIONPIXELS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea28ae488f549794d4c6ffedd0fff83d07)AV_CODEC_ID_TGV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9ea7b9d85dbfc68ef53a648e34c1fc86)AV_CODEC_ID_TGQ, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea93e25f9e318041d53c0655d36d9623f7)AV_CODEC_ID_TQI, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2eeaceb777ebeece34d0af283831b65e)AV_CODEC_ID_AURA, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0c1a938764967517ce7420374accc2f0)AV_CODEC_ID_AURA2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea915a50b4a0ed015dba9dd325d27a439a)AV_CODEC_ID_V210X, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9895ea0a1e6fc140566731c3f7c5f870)AV_CODEC_ID_TMV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea11b45c130aea73e9ab73c4cdfb176803)AV_CODEC_ID_V210, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf20137ebc6d50438f1159ac66dfa4a9a)AV_CODEC_ID_DPX, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4be68436a8e9c3ba69919ca487c9b824)AV_CODEC_ID_MAD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4b2e274845edd73ca8025ed70a91ac79)AV_CODEC_ID_FRWU, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea730fd43cf81636bbf3a2c11c6c8559fd)AV_CODEC_ID_FLASHSV2, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea76fbc3c96f42a427d3ae1eeb6e1a8aaf)AV_CODEC_ID_CDGRAPHICS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2400153f986f90a4db379bd407e225b3)AV_CODEC_ID_R210, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaec305465c9a43a418d3ce7f93d60d053)AV_CODEC_ID_ANM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea73758a7f9fcc3fd1cb7533a39a0beb6c)AV_CODEC_ID_BINKVIDEO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea054e8c530a2f650eaeece8baf0c9f13b)AV_CODEC_ID_IFF_ILBM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8725c4ddc5aa0f42e413da8dd86be79c)AV_CODEC_ID_KGV1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0b66da3bdc5f4daae18c10f0ce6c766e)AV_CODEC_ID_YOP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea77772369e1d305824b08b53a8d5c56b6)AV_CODEC_ID_VP8, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2f8f94b86674549af61994639abdb234)AV_CODEC_ID_PICTOR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead3d14bb39e556f703f7b90a2e5e84a8e)AV_CODEC_ID_ANSI, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaae4ee474148248dee829a60d1adf8dfe)AV_CODEC_ID_A64_MULTI, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7762bd7c923e6836024782b28b875026)AV_CODEC_ID_A64_MULTI5, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadfd3b6add5f4c1dad10b85ef2f474efd)AV_CODEC_ID_R10K, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea56d2a49a2f28921cc8d2ee762e4f5522)AV_CODEC_ID_MXPEG, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5455d25d346fb985f104ac97a4b8d01c)AV_CODEC_ID_LAGARITH, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab140360bd655d8add22265dbe45b581c)AV_CODEC_ID_PRORES, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9f3b79a952d3cdcf484aa41e9c64571e)AV_CODEC_ID_JV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea039667ef2581a7bca1533e9476b730fe)AV_CODEC_ID_DFA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7de7e6cabd99e6215c1a4fdfca196198)AV_CODEC_ID_WMV3IMAGE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5e1b7f778aed1ba72262d708d602d74b)AV_CODEC_ID_VC1IMAGE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3313f443c4f36800b279818ed9525f07)AV_CODEC_ID_UTVIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0927738ad0f3790364ce57694f97b65f)AV_CODEC_ID_BMV_VIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea80099cd3307b3b848f201c5d98e47854)AV_CODEC_ID_VBLE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf4d36f7aa9f2ef3d5f8d334353aae852)AV_CODEC_ID_DXTORY, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3bbc391cc2b0bb220d5bf950cdfc4f1f)AV_CODEC_ID_V410, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab2da2aa56366ac85b26151a7ca75349b)AV_CODEC_ID_XWD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9c43fea9a5b9e3da54b3810cc9e8ecbf)AV_CODEC_ID_CDXL, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea74fad79b79fd7fabf35cbef967da2603)AV_CODEC_ID_XBM, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf1697273c05e4605444f0f41396739b3)AV_CODEC_ID_ZEROCODEC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf7c4f69a8ef661c53fae35cd5d8a98ab)AV_CODEC_ID_MSS1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1167130b3a74649f117126749366f97a)AV_CODEC_ID_MSA1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceacc64eb7d1b978ad2be7f90a44d5acaa1)AV_CODEC_ID_TSCC2, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5c7cad52b4d46786518e30d855a2b47d)AV_CODEC_ID_MTS2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea277ddc6d640dd4b319587350f3b5a444)AV_CODEC_ID_CLLC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea95ef4e077346c2f43c7568b11b1ae07d)AV_CODEC_ID_MSS2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaedab7fb3a1647fbdc114ad0447105c72)AV_CODEC_ID_VP9, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3fc5c34c4edafe3bd2c71e996c83b04a)AV_CODEC_ID_AIC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5947fac43d6817b691a4bc1f64f20075)AV_CODEC_ID_ESCAPE130, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaab57a3cf30313f49eb77c24a748eebc0)AV_CODEC_ID_G2M, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac9eb9136ca924db98aa71306a1530b41)AV_CODEC_ID_WEBP, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabeeba7d448dd4f8f931d05612e0c44e4)AV_CODEC_ID_HNM4_VIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa3e19a45b81bb602a4a37a4221274a20)AV_CODEC_ID_HEVC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabb3a243c38cdf7bfd380e4c3c8752ae4)AV_CODEC_ID_FIC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab45fa4be4865749b3f7f3f68bfb7a766)AV_CODEC_ID_ALIAS_PIX, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9502fa122a011d55e1057d519fb75290)AV_CODEC_ID_BRENDER_PIX, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0ea3a7e6b37e673948fae7e13d2b72d0)AV_CODEC_ID_PAF_VIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabc76b0cc02be5952124697b98ef2c4df)AV_CODEC_ID_EXR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3fc7f7a6861e3744f32e38a3c9d15c20)AV_CODEC_ID_VP7, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8ee1775ee286c830aba02cefce37bfe2)AV_CODEC_ID_SANM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea59d47b3316a9fb14203a22d5c169e3cb)AV_CODEC_ID_SGIRLE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea627fb03cc76110503962e8d46f91c539)AV_CODEC_ID_MVC1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac4b21d1f470cf86f8e1aa2416fcb2461)AV_CODEC_ID_MVC2, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea990f7448c1a5b8a354dc3affa1aeb529)AV_CODEC_ID_HQX, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf48e02bddcd438ebb17430aa3f50d2cc)AV_CODEC_ID_TDSC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf85fe24fa0b0717a4ebb0e31609f7932)AV_CODEC_ID_HQ_HQA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabc357acdbb191aaba9a9abb719077d6a)AV_CODEC_ID_HAP, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafa5e95379b848be170304379a0c03d38)AV_CODEC_ID_DDS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea92977658e8bfc929a90cc6f9cd824cfb)AV_CODEC_ID_DXV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaccf0a68b505c41b381aa294091dea16f)AV_CODEC_ID_SCREENPRESSO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0164ebe70733d122c0638c0d99ba6771)AV_CODEC_ID_RSCC, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea14ea269e75a39cfbc002a9d830b4bcd6)AV_CODEC_ID_Y41P = 0x8000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4100c41cb36ea3761ef6b8663374e7ba)AV_CODEC_ID_AVRP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf4af8b770c8a89602b25313db541f021)AV_CODEC_ID_012V, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaea40e943e4002267e20c0dda6498a2e9)AV_CODEC_ID_AVUI, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac1c65ebb53d915a490495df173475a4d)AV_CODEC_ID_AYUV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafe04f75e6fdfceac4db3c8f7b4726778)AV_CODEC_ID_TARGA_Y216, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea835d96992d9a3673c4d76459aff9f1de)AV_CODEC_ID_V308, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2825b4322257b5292cc71ad196acbfc9)AV_CODEC_ID_V408, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea11de9ead44f7b3dd11043c54fb56a155)AV_CODEC_ID_YUV4, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafc44dbe94c7f665dd2cc95302b561edf)AV_CODEC_ID_AVRN, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4da1273a192bfd82b21853ebde414e0d)AV_CODEC_ID_CPIA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8325d089e284d10840e7105a8c28e4a4)AV_CODEC_ID_XFACE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea253b7f23251a791e6f1d95a86dbab0cd)AV_CODEC_ID_SNOW, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaafdf8cbfcb390ea0bf0a0c5738d46e11)AV_CODEC_ID_SMVJPEG, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea21362c401630027757e02c301e44ff4f)AV_CODEC_ID_APNG, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea041dec660061c8e655aafaa4c26dbe24)AV_CODEC_ID_DAALA, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea428b05855c78a30bd14ba8f5e0a4fe77)AV_CODEC_ID_CFHD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0cfed23b6a60682058b7c2996a136e8f)AV_CODEC_ID_TRUEMOTION2RT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3c1e06a25d7c4bcbaf4f46ec482da30e)AV_CODEC_ID_M101, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadc5eb9fc477a61c8ac99fa0e0d68cdab)AV_CODEC_ID_MAGICYUV, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea766513ce79b765aebff49c19aca7243d)AV_CODEC_ID_SHEERVIDEO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2031beb2ac3629ce701dbe15fc76f067)AV_CODEC_ID_YLC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2312ef9d5a9e4af8aaa981ce94716d6b)AV_CODEC_ID_PSD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea325c44b4865bcdd11d95f02dd5d50d9b)AV_CODEC_ID_PIXLET, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9fed38bd1048f2fd8ce1e14b874f1b83)AV_CODEC_ID_SPEEDHQ, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa14777c888eea45741247845ea0dd337)AV_CODEC_ID_FMVC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8bdbce85582d7562d32b032b3e963ec8)AV_CODEC_ID_SCPR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea38aa741a36cb0359269b24ded2086f62)AV_CODEC_ID_CLEARVIDEO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea15bd0567cbf67b50cbeddf3a9f72b8e4)AV_CODEC_ID_XPM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6dbe55d3a303780cfdb3627384e4141d)AV_CODEC_ID_AV1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea601366c273a91fb8ab566a07e466af58)AV_CODEC_ID_BITPACKED, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac0b1196d97790c93570d9c04ebd3a00c)AV_CODEC_ID_MSCC, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea767a1b1471362b0983e192687d07e231)AV_CODEC_ID_SRGC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceada05c0c119445e113fb61c4bde03f9b0)AV_CODEC_ID_SVG, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaba4df23b76d13f138375ea29ff7c32d5)AV_CODEC_ID_FIRST_AUDIO =
 0x10000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea29c8a6ec93ad1a77e50f2b48efa80204)AV_CODEC_ID_PCM_S16LE =
 0x10000, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5144fb79599a0c61ef1e66ef72b6f04c)AV_CODEC_ID_PCM_S16BE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf74e44549c9bac344123cede6a0871ca)AV_CODEC_ID_PCM_U16LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8d7e94ed46d290528877c6a8949a756c)AV_CODEC_ID_PCM_U16BE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea689ee50aa42c2ae666da10615ff78e10)AV_CODEC_ID_PCM_S8, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1e9ff6fe2442af342b2b42a1a0b2abc8)AV_CODEC_ID_PCM_U8, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaffd38a41156e08564e5c6e88dbd06cb2)AV_CODEC_ID_PCM_MULAW, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea794b01f8eb94f196d5ebb9f4af1ae0c2)AV_CODEC_ID_PCM_ALAW, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6384417ba463d7aeb03015797cff3349)AV_CODEC_ID_PCM_S32LE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead316c7d2fd05155d39d862a23fda97a1)AV_CODEC_ID_PCM_S32BE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea71143037f27cc95fd5b5b8245cc6d8b9)AV_CODEC_ID_PCM_U32LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf9d8f80b2f0c516669ea9d6f18c69ebd)AV_CODEC_ID_PCM_U32BE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea55f60c53c2cc765877092c125ee172be)AV_CODEC_ID_PCM_S24LE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0513a388236f95e61e73b943e725d4a8)AV_CODEC_ID_PCM_S24BE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae4523044929927b24cbb846463584a7c)AV_CODEC_ID_PCM_U24LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea56e4ec447eb34b4af671fd5c8e36ddb4)AV_CODEC_ID_PCM_U24BE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5e3d1cd48a286bd5edc38741da6b23fd)AV_CODEC_ID_PCM_S24DAUD, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae0a844cb0082a09c193fb4c1904f7d9c)AV_CODEC_ID_PCM_ZORK, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae0ca330183ef218ea83f3b5af72ddaf0)AV_CODEC_ID_PCM_S16LE_PLANAR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea265ac8e5e9471978b053db920e7d4f0d)AV_CODEC_ID_PCM_DVD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2e3b263e55bf4774f5a3a1d7b0261df4)AV_CODEC_ID_PCM_F32BE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0178ff664c08bd74594545ba0c5efacb)AV_CODEC_ID_PCM_F32LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabbe3519b849a2acd3e8366bc1173f221)AV_CODEC_ID_PCM_F64BE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7a06659945e1b8973c6f4712d4910ba1)AV_CODEC_ID_PCM_F64LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea315e9b3d0083e30639b4f54b70bd2cf0)AV_CODEC_ID_PCM_BLURAY, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9ed5e23d0459943273f246db85efe1e7)AV_CODEC_ID_PCM_LXF, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1a5a69765d6da8731924db0835b005b3)AV_CODEC_ID_S302M, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf836e7c461fa60e1cb3281ed65b9b82f)AV_CODEC_ID_PCM_S8_PLANAR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea50556a2dfa9f65156a10df83c7b47913)AV_CODEC_ID_PCM_S24LE_PLANAR, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2a630809b19f2e75b76df46492dffa85)AV_CODEC_ID_PCM_S32LE_PLANAR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1c6bda9ea60ec36565f3a3127ee59642)AV_CODEC_ID_PCM_S16BE_PLANAR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa6b7ebb6caae7f0b539e3b299a7a2937)AV_CODEC_ID_PCM_S64LE =
 0x10800, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea11fe7c8c18c05d994c3d85cb14c7507f)AV_CODEC_ID_PCM_S64BE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2c728a538cc65576413619c108e42e3a)AV_CODEC_ID_PCM_F16LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea006833a0a53b52dc07637629c6eff899)AV_CODEC_ID_PCM_F24LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa238badb45f42286d03179686f2a6768)AV_CODEC_ID_ADPCM_IMA_QT =
 0x11000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadad7dc9e33bcb7b38d886ad11ac788b7)AV_CODEC_ID_ADPCM_IMA_WAV, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9eac85b06e51a78b3b958bbd2eb928c2)AV_CODEC_ID_ADPCM_IMA_DK3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea99d61cb6935952b96a0818c594f1a059)AV_CODEC_ID_ADPCM_IMA_DK4, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1afc35bd273602a567d6517bbd61a361)AV_CODEC_ID_ADPCM_IMA_WS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaaf14b7fa6293d5d501f4aa264075f77f)AV_CODEC_ID_ADPCM_IMA_SMJPEG, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6f63c9df5fee26f639f330cd99554467)AV_CODEC_ID_ADPCM_MS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1f5474f0e9d61e51a3b6b4012563bda5)AV_CODEC_ID_ADPCM_4XM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea56d59fbacf5a6db70d8d69dd1ffc4c10)AV_CODEC_ID_ADPCM_XA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6e64ec6fce1310b9dda915e55303048a)AV_CODEC_ID_ADPCM_ADX, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa2e9c307fb128dfd9dff221d33edf7ed)AV_CODEC_ID_ADPCM_EA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaaa43507bca8296df2333437245fb41b7)AV_CODEC_ID_ADPCM_G726, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea16e195418bb8ed99d9bb9114fa820dbc)AV_CODEC_ID_ADPCM_CT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea059036a462f51cb2acb903ff7a5efaa7)AV_CODEC_ID_ADPCM_SWF, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5ee9dac88143bbc5037d460ca57809e1)AV_CODEC_ID_ADPCM_YAMAHA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea755c93b48368752b8e9a3b4a612d7b63)AV_CODEC_ID_ADPCM_SBPRO_4, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea60821a311fe4b9e4a3712b9326a30426)AV_CODEC_ID_ADPCM_SBPRO_3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2f32069b72c70f733b154d21f7780628)AV_CODEC_ID_ADPCM_SBPRO_2, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead3c9bf4b6642ff74aac8fa1e0bc8a382)AV_CODEC_ID_ADPCM_THP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8c90d0b288a4225e2793f1e81fc23f7e)AV_CODEC_ID_ADPCM_IMA_AMV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1d0fc213ef4d78eb824e1bb8a3e8ca1a)AV_CODEC_ID_ADPCM_EA_R1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae34786ef207bc8ac5df3e06102eec8aa)AV_CODEC_ID_ADPCM_EA_R3, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea637ad77f2e57915f4f6ce9cb96856f04)AV_CODEC_ID_ADPCM_EA_R2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea43adf2d362fae8b03d8444f9d688d667)AV_CODEC_ID_ADPCM_IMA_EA_SEAD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea21efa561a1fbc658b5a706889962c4ba)AV_CODEC_ID_ADPCM_IMA_EA_EACS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8d0413f2c5e41c675aa4a73d72f0c2a1)AV_CODEC_ID_ADPCM_EA_XAS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea76b2e71d17fb89d9d253824dd3e004c1)AV_CODEC_ID_ADPCM_EA_MAXIS_XA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea66aedf10fb5aa729f0ccda2718112ab7)AV_CODEC_ID_ADPCM_IMA_ISS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2f60ee03608000dd48e21ea699d1e52f)AV_CODEC_ID_ADPCM_G722, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea807da007dc43b0f69465f706d49be0cd)AV_CODEC_ID_ADPCM_IMA_APC, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae92fb10e08a5db53ef8f69092647c0e3)AV_CODEC_ID_ADPCM_VIMA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2d956c88a5eee1033ab7dc21ed0fe1a4)AV_CODEC_ID_VIMA =
 AV_CODEC_ID_ADPCM_VIMA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead90606cb664accfda87cf51d52426ff8)AV_CODEC_ID_ADPCM_AFC =
 0x11800, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea214f26a6d8d51346d65bd562ecfb00bc)AV_CODEC_ID_ADPCM_IMA_OKI, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3347194650b39d040e80a6c3d826c31a)AV_CODEC_ID_ADPCM_DTK, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaea2f4918a45f88ef44750872b337a37b)AV_CODEC_ID_ADPCM_IMA_RAD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea65cdc46b3d1c5f110d51b4b3e465800b)AV_CODEC_ID_ADPCM_G726LE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadbafa944453e24e85d18cb7e5c4d7654)AV_CODEC_ID_ADPCM_THP_LE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea55ad60fce2eff6ab1061a9566a64f3eb)AV_CODEC_ID_ADPCM_PSX, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac28d73212b64e6bb36ce9484f9b70f51)AV_CODEC_ID_ADPCM_AICA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9ff821515b3ee090715f1f478382cfe6)AV_CODEC_ID_ADPCM_IMA_DAT4, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab02a19d9d062a7d3056cd11cad08ae67)AV_CODEC_ID_ADPCM_MTAF, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea88d9e41adf1df5445e3068512177b582)AV_CODEC_ID_AMR_NB =
 0x12000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac62343eecae501ff795fe264ce19d965)AV_CODEC_ID_AMR_WB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead80651a95ce787bfdfc7a259702e9e7b)AV_CODEC_ID_RA_144 =
 0x13000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea64b4d0a85fa2d177c6d72a04b2ab07c3)AV_CODEC_ID_RA_288, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9cda6e938236d38cc2c1ff6630ea2cca)AV_CODEC_ID_ROQ_DPCM =
 0x14000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5541c6cd43d40afbc11b184e723428d1)AV_CODEC_ID_INTERPLAY_DPCM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab2fea28c2a9de52f61125417506a16e3)AV_CODEC_ID_XAN_DPCM, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6e1a1749fcc819a8b7c612331e4644f0)AV_CODEC_ID_SOL_DPCM, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea62ce049ad66c5c14b2728fdf3f1ccd76)AV_CODEC_ID_SDX2_DPCM =
 0x14800, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea90ffce5aedda33269b22221916cd8487)AV_CODEC_ID_MP2 =
 0x15000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaed6f7acd6cf30c059f2e32515bd19bc4)AV_CODEC_ID_MP3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaadba4b5a087e2b6bf2acdd1b5aebb988)AV_CODEC_ID_AAC, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac87e37dcf0240ae87c9004d10a14f1a0)AV_CODEC_ID_AC3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa3379ed720d45daac872b2f483f545de)AV_CODEC_ID_DTS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0fac27af40d22e02bf926ecd4f7bc03a)AV_CODEC_ID_VORBIS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea91aff3d42b6b8fc68dfe079a33a64949)AV_CODEC_ID_DVAUDIO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea83358ccfd89744d275b120a9d1eb61de)AV_CODEC_ID_WMAV1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea92c919ae422d479ba7cf8e0bb1c6100f)AV_CODEC_ID_WMAV2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceacc6e781037605bd089f1ebbef0c28874)AV_CODEC_ID_MACE3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceacf3d90862beaeae4505a33d4ac26e669)AV_CODEC_ID_MACE6, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2632ab8921d2b14854f58fb2e089702a)AV_CODEC_ID_VMDAUDIO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceabab880efbebbd91d3817debef9e4dc00)AV_CODEC_ID_FLAC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadee0fb3fcc014a90fe9bf929abf89c1d)AV_CODEC_ID_MP3ADU, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaca01aa1c3bfa9df5acbd6083564f9f69)AV_CODEC_ID_MP3ON4, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea07b816eceeeeae08be3b2de916023733)AV_CODEC_ID_SHORTEN, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea81f7bb2a5a480533df028153b1f664c6)AV_CODEC_ID_ALAC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6a0b07cf8812e6c8cca139a29ef95576)AV_CODEC_ID_WESTWOOD_SND1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2c43eedcf1b16ddb37fdfdbd2bd530bf)AV_CODEC_ID_GSM, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead2cbe7fc8799aad9f808e81723e4452f)AV_CODEC_ID_QDM2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa4652899df5ea4061a1cb7f94fa1d758)AV_CODEC_ID_COOK, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8cf29d90e993fef9e034ed1e48987c59)AV_CODEC_ID_TRUESPEECH, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea94bc512e0c73486d18f23b8e27915409)AV_CODEC_ID_TTA, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead5871fc95fa45eed7b2984bb02c41450)AV_CODEC_ID_SMACKAUDIO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac5a616b6750e42680e2ab71c7db317d1)AV_CODEC_ID_QCELP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae6f1c1bbb4e389366f56cfb8cc77d240)AV_CODEC_ID_WAVPACK, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea80750dc22eb3922c8fdd801c79efb3c2)AV_CODEC_ID_DSICINAUDIO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5d940160a3a8f6de5d01ddfc70d16d1e)AV_CODEC_ID_IMC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea07a62d6db7f2714190885aff266be9c1)AV_CODEC_ID_MUSEPACK7, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead7c47629784f27b6747d581fada0392e)AV_CODEC_ID_MLP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea70345884116384cc881f1ae7fa8eba2a)AV_CODEC_ID_GSM_MS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaac96d7eb6ee2d7a2effe3a0d59154c08)AV_CODEC_ID_ATRAC3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1d72097be6b2c093e68e8f207ce60925)AV_CODEC_ID_VOXWARE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea40c7bc93234f6eb8b5ead2b5d887aa4a)AV_CODEC_ID_APE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab8c68089cc8eaf757bbec50cc5795be1)AV_CODEC_ID_NELLYMOSER, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea909556505cd304b3ae2e9131e8e370fe)AV_CODEC_ID_MUSEPACK8, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae6753ab38dc444ec81089ad18f962cbb)AV_CODEC_ID_SPEEX, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3b3a0eff07db4a47cdb88c1180b3ca66)AV_CODEC_ID_WMAVOICE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafc80a116ff2921f47e3eb20fc9f070da)AV_CODEC_ID_WMAPRO, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8105401278b4938d0f3aaabbac1d3242)AV_CODEC_ID_WMALOSSLESS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea64ed32c01a8725f27d9f3f9c12216c4c)AV_CODEC_ID_ATRAC3P, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea14c0f2031427f1065ae8c18095e4384f)AV_CODEC_ID_EAC3, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea39c05013b9bbca74c12ecc637de96120)AV_CODEC_ID_SIPR, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea585f305d3aab40bdfc9688f9367b04c0)AV_CODEC_ID_MP1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae4690ed32add965639acf9b69f9a63cf)AV_CODEC_ID_TWINVQ, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab89325573cab37c7fe7f6003d0134f7e)AV_CODEC_ID_TRUEHD, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3084d85d4882836e9d6c5b224a8f68bd)AV_CODEC_ID_MP4ALS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0b54eff8cf6853b4535d94b49d958dd8)AV_CODEC_ID_ATRAC1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae0961449f790c225a6de634b54ab1440)AV_CODEC_ID_BINKAUDIO_RDFT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6d39b78dfb3e66c9dc21ff91d7ae8c7f)AV_CODEC_ID_BINKAUDIO_DCT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf1c53c6ecbd416cccbb3a258a2d7290d)AV_CODEC_ID_AAC_LATM, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf1fd3864a6fd0e82163fd85a0cadbbb0)AV_CODEC_ID_QDMC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2dda47258355317e1da81c6b557841d0)AV_CODEC_ID_CELT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea02394b74bf5e5a9d1597a950ac1c05ff)AV_CODEC_ID_G723_1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1a6ba806315bfaee42f10782c1240256)AV_CODEC_ID_G729, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2d3d266cf76593642f046220b42acac0)AV_CODEC_ID_8SVX_EXP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead41445ff5e48c95d4fcd1668d05d15c7)AV_CODEC_ID_8SVX_FIB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea84bdcd6fe01b1938a48a6fe112f5f4e4)AV_CODEC_ID_BMV_AUDIO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5b59fada853e4e77f2063a37c8cc4eb5)AV_CODEC_ID_RALF, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea14d17ab2af94745aac3a348ef0a4877a)AV_CODEC_ID_IAC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaeb2f1e1f975d68c286c52c197727456c)AV_CODEC_ID_ILBC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf41ed5e8d7cf14b25f02f5738d9ab117)AV_CODEC_ID_OPUS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea879fe5474d71622c1bcb82672ffcb64b)AV_CODEC_ID_COMFORT_NOISE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea982bf30618f91ef2d22c10803db59316)AV_CODEC_ID_TAK, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaffefa07da5af291cda932c5aa9a00ba3)AV_CODEC_ID_METASOUND, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaba6cecfdc1f7085eff9ea390bca1f82f)AV_CODEC_ID_PAF_AUDIO, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab306aea23035ba05e8c2aab7674301ad)AV_CODEC_ID_ON2AVC, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceadbd952bfdd9a4ab164b20f14e9620813)AV_CODEC_ID_DSS_SP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2b9d67b8afbc3cc569ca41677b416e3e)AV_CODEC_ID_FFWAVESYNTH =
 0x15800, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cead3beaef180e8f602253a42400e32fe2b)AV_CODEC_ID_SONIC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaba5211214dd50732d4173ee25a569987)AV_CODEC_ID_SONIC_LS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea49aaafd1aea907d4d90ec18246b7cfc1)AV_CODEC_ID_EVRC, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3f95fd7f9fbc835cc702a83ab3780a52)AV_CODEC_ID_SMV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1de0fb02093b1ee70f232acfb13f768e)AV_CODEC_ID_DSD_LSBF, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa3b6cc33789cb8064a7af9910227a125)AV_CODEC_ID_DSD_MSBF, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea495b15f5692100855aab7405c0504af3)AV_CODEC_ID_DSD_LSBF_PLANAR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3051f62762f8a8d495e0d29eeefa1c9c)AV_CODEC_ID_DSD_MSBF_PLANAR, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea29d9517407ed90d0ffd6c235f7529649)AV_CODEC_ID_4GV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0ce47dae0a321045e09b1101d529275b)AV_CODEC_ID_INTERPLAY_ACM, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2b080674c70f0d41bf3429b84fe84826)AV_CODEC_ID_XMA1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3a859113bb7c33aa1dcd0e1fc8383e83)AV_CODEC_ID_XMA2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceab7020b9cd1bb1845c80d93e0cff92479)AV_CODEC_ID_DST, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea8fab24a8737c7d60c9e1b70e706054fa)AV_CODEC_ID_ATRAC3AL, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea939677eb956ad777a6d26f2887e5ef27)AV_CODEC_ID_ATRAC3PAL, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0109122e90fee33da49638c017f5c4b1)AV_CODEC_ID_FIRST_SUBTITLE =
 0x17000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2d41739284f633efadccd4b43e906937)AV_CODEC_ID_DVD_SUBTITLE =
 0x17000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea1a3f67e556cc61335c6f001ae3c7069f)AV_CODEC_ID_DVB_SUBTITLE, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3b738e90e2e06c46df79d6b82f2f096b)AV_CODEC_ID_TEXT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea30dc7b340d566342595dd6f060f1e897)AV_CODEC_ID_XSUB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea579f9d41a5ac5adab90245824d61453e)AV_CODEC_ID_SSA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafef6fb0a89aaa9a74c24129f380da853)AV_CODEC_ID_MOV_TEXT, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea146a68962f8721fc709e19f5671844d7)AV_CODEC_ID_HDMV_PGS_SUBTITLE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceacefff4578427aea42f7bae17f6a5a0a4)AV_CODEC_ID_DVB_TELETEXT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3916aa80b7ce1e4039ca2295b01a4ba0)AV_CODEC_ID_SRT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceafb2ec174f73e61c7563c8017e8fc316b)AV_CODEC_ID_MICRODVD =
 0x17800, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea844a7eeed17163929fb56ee00eddd1d2)AV_CODEC_ID_EIA_608, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea29efe025a4398998dbfe762cf0bbd900)AV_CODEC_ID_JACOSUB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea28e2d0cdc8c36d277f8adce808f7dd16)AV_CODEC_ID_SAMI, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea55663844c2d62417ed219aa128132506)AV_CODEC_ID_REALTEXT, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea4544ff4443c41d7426059cb2ef199125)AV_CODEC_ID_STL, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7b6c6b49e41933c1f83207b9653b8c7b)AV_CODEC_ID_SUBVIEWER1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaad683f0e2cd34f5c9f6c2ff813061e71)AV_CODEC_ID_SUBVIEWER, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5b0708bf7c0276316672f513b4dded18)AV_CODEC_ID_SUBRIP, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea82b722838aa5158de9313be99930618a)AV_CODEC_ID_WEBVTT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea160f5b01bf93dbc3352af1c0a001cefd)AV_CODEC_ID_MPL2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea9321895a9caaedf1afcce047ffe21606)AV_CODEC_ID_VPLAYER, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea28212eff72ec33ced01dc2e20f742644)AV_CODEC_ID_PJS, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea98e0491591ea805a932711598edb38cb)AV_CODEC_ID_ASS, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea517a3cdbbcfc75fd1e967d270be8f3bc)AV_CODEC_ID_HDMV_TEXT_SUBTITLE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea5ef7de38a63273a9916425ed4f544017)AV_CODEC_ID_FIRST_UNKNOWN =
 0x18000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea7396b9ed6459c928cb4cee2be5328717)AV_CODEC_ID_TTF =
 0x18000, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea61295522c813f773c5b42f589af3335a)AV_CODEC_ID_SCTE_35, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaf1bc9ee1663866899f8ec983e6006336)AV_CODEC_ID_BINTEXT =
 0x18800, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaeff8d0d9cb485afc066e699aadfe57e4)AV_CODEC_ID_XBIN, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea6bfc8ce0bac2bd4c04a99cbb80468396)AV_CODEC_ID_IDF, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceac81efb9f2076c74c59ac2c9855767879)AV_CODEC_ID_OTF, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea44cfc6da6c78153c2def0bd9fab6d84c)AV_CODEC_ID_SMPTE_KLV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaab7e9e9c1df689648e2f33b9bbeff23c)AV_CODEC_ID_DVD_NAV, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea12ae549c4f144325b3d0bf54f2b18c3d)AV_CODEC_ID_TIMED_ID3, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa5cef5ab33a218c7ceccb713171a8632)AV_CODEC_ID_BIN_DATA, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea2eaaed0131f13de4d83b77342040bbd1)AV_CODEC_ID_PROBE =
 0x19000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea0453ba9b391ba005191484449eb23290)AV_CODEC_ID_MPEG2TS =
 0x20000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceae298685b3ec27fc40d978250dc35a6c8)AV_CODEC_ID_MPEG4SYSTEMS =
 0x20001, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7ceaa555182b7354f5793eb863dece259cd8)AV_CODEC_ID_FFMETADATA = 0x21000, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggaadca229ad2c20e060a14fec08a5cc7cea3eb24d18d2fb3bf64e695e77562bd49c)AV_CODEC_ID_WRAPPED_AVFRAME =
 0x21001 

}
识别比特流的语法和语义。






enum  [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gadcff0a5957a99f6fdf1eec9d5a3fdfaf)AVAudioServiceType { 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafa909e448624c46b32506f20acaeb59219)AV_AUDIO_SERVICE_TYPE_MAIN =
 0, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafa89cc3ca931cc01de8b8b498492133f58)AV_AUDIO_SERVICE_TYPE_EFFECTS =
 1, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafae1d0587828c1f7efdb0ac449946448fc)AV_AUDIO_SERVICE_TYPE_VISUALLY_IMPAIRED =
 2, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafabd1590db13ebdc6f1c53fd5cd97ff3ca)AV_AUDIO_SERVICE_TYPE_HEARING_IMPAIRED =
 3, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafa94e109ad671c99029eb89b61a840050c)AV_AUDIO_SERVICE_TYPE_DIALOGUE =
 4, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafac279dfd2816ee38406920c0c4718bd90)AV_AUDIO_SERVICE_TYPE_COMMENTARY =
 5, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafa49e2c0c319dfdd3b5756e58900c8a88c)AV_AUDIO_SERVICE_TYPE_EMERGENCY =
 6, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafa0c000c46073046780a3daa1e8f84e207)AV_AUDIO_SERVICE_TYPE_VOICE_OVER =
 7, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafab4290d124e49f795ef1ba709fc60233c)AV_AUDIO_SERVICE_TYPE_KARAOKE =
 8, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ggadcff0a5957a99f6fdf1eec9d5a3fdfafac562760f8958fb519327590d42e91b0e)AV_AUDIO_SERVICE_TYPE_NB

}



enum  [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga957ef25581a6b3c98589817b1cbed02c)AVFieldOrder { 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga957ef25581a6b3c98589817b1cbed02ca886b3191488ef874bf3e94b3d9cec713)AV_FIELD_UNKNOWN, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga957ef25581a6b3c98589817b1cbed02ca9165cfa953463411e28670a9d06af2e2)AV_FIELD_PROGRESSIVE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga957ef25581a6b3c98589817b1cbed02ca236b46495cd9254b4030d36fc6f27f23)AV_FIELD_TT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga957ef25581a6b3c98589817b1cbed02ca7bf84c3d1e2714ddee42b3e94826fca4)AV_FIELD_BB, 
[](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga957ef25581a6b3c98589817b1cbed02cae1efa8b010ca5331f1f845104e7c0667)AV_FIELD_TB, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga957ef25581a6b3c98589817b1cbed02caba81a12143ed2cea06438739a7caaafb)AV_FIELD_BT

}



enum  [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#ga828218dcb8874ab2c5af8d486c365421)AVSubtitleType { [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga828218dcb8874ab2c5af8d486c365421a996f4241a1f619428260744d394c5caa)SUBTITLE_NONE, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga828218dcb8874ab2c5af8d486c365421a0e577aa36b52d777c6ec5bef8625dde4)SUBTITLE_BITMAP, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga828218dcb8874ab2c5af8d486c365421a911079085bcf785c5f1f06e1e0d651c2)SUBTITLE_TEXT, [](http://ffmpeg.org/doxygen/trunk/group__lavc__core.html#gga828218dcb8874ab2c5af8d486c365421a3fd131173f80938f3d30f99977aa455a)SUBTITLE_ASS }



