# ffmpeg视频小结



## FFMPEG视频解码流程

  1. 注册所有容器格式和CODEC: av_register_all()
  2. 打开文件:av_open_input_file()
  3. 从文件中提取流信息:av_find_stream_info()
  4. 穷举所有的流，查找其中种类为CODEC_TYPE_VIDEO
  5. 查找对应的解码器:avcodec_find_decoder()
  6. 打开编解码器:avcodec_open()
  7. 为解码帧分配内存:avcodec_alloc_frame()
  8. 不停地从码流中提取出帧数据:av_read_frame()
  9. 判断帧的类型，对于视频帧调用:avcodec_decode_video()
  10. 解码完后，释放解码器:avcodec_close()
  11. 关闭输入文件:avformat_close_input_file()

 

## FFMPEG视频编码流程

1、av_register_all()：注册FFmpeg所有编解码器。

2、avformat_alloc_output_context2()：初始化输出码流的AVFormatContext。

3、avio_open()：打开输出文件。

4、av_new_stream()：创建输出码流的AVStream。

5、avcodec_find_encoder()：查找编码器。

6、avcodec_open2()：打开编码器。

7、avformat_write_header()：写文件头（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）。

8、不停地从码流中提取出YUV数据，进行编码。

avcodec_encode_video2()：编码一帧视频。即将AVFrame（存储YUV像素数据）编码为AVPacket（存储H.264等格式的码流数据）。

av_write_frame()：将编码后的视频码流写入文件。

9、flush_encoder()：输入的像素数据读取完成后调用此函数。用于输出编码器中剩余的AVPacket。

10、av_write_trailer()：写文件尾（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）。

 

## YUV，RGB各种图像原始数据之间的转换

FFMPEG中的swscale提供了视频原始数据（YUV420，YUV422，YUV444，RGB24...）之间的转换，分辨率变换等操作。swscale主要用于在2个AVFrame之间进行转换。

在转换时还要用到AVFrame这个结构体。

```cpp
typedef struct AVFrame  
{  
#define AV_NUM_DATA_POINTERS 8  
    uint8_t *   data [AV_NUM_DATA_POINTERS]; //指向图像数据  
    int linesize [AV_NUM_DATA_POINTERS]; //行的长度  
    int width; //图像的宽  
    int height; //图像的高  
    int format;  //图像格式  
     ……  
}AVFrame;
```

### 1.初始化pFrameYUV的时候，设定想要转换的格式：

```cpp
AVFrame *pFrame,*pFrameYUV;  
pFrame=avcodec_alloc_frame();  
pFrameYUV=avcodec_alloc_frame();  
uint8_t *out_buffer;  
//计算这个格式的图片，需要多少字节来存储  
int bytes_num = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
//申请空间来存放图片数据。包含源数据和目标数据  
out_buffer=(uint8_t*)av_malloc(bytes_num);   
//把av_malloc得到的内存和AVFrame关联起来。  
avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height); 
```

 

只需要把PIX_FMT_***改了就可以了

### 2.在sws_getContext()中更改想要转换的格式：

```cpp
img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
```

 

也是把PIX_FMT_***改了就可以了

最后，如果想将转换后的原始数据存成文件，只需要将pFrameYUV的data指针指向的数据写入文件就可以了。

例如，保存YUV420P格式的数据，用以下代码：

```cpp
fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),1,output);  
fwrite(pFrameYUV->data[1],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);  
fwrite(pFrameYUV->data[2],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);
```

保存RGB24格式的数据，用以下代码：

```cpp
fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height)*3,1,output);  
```

保存UYVY格式的数据，用以下代码：

```cpp
fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),2,output);  
```

 

在FFMPEG中，图像原始数据包括两种：planar和packed。planar就是将几个分量分开存，比如YUV420中，data[0]专门存Y，data[1]专门存U，data[2]专门存V。而packed则是打包存，所有数据都存在data[0]中。具体哪个格式是planar，哪个格式是packed，可以查看pixfmt.h文件。注：有些格式名称后面是LE或BE，分别对应little-endian或big-endian。另外名字后面有P的是planar格式。

YUV420P转换成YUYV422完整转换实例参考：

https://blog.csdn.net/handsomehong/article/details/72796635

 

## ffmpeg结构体介绍

### AVStream

存储每一个视频/音频流信息的结构体 AVStream重要的变量如下

```
int index：标识该视频/音频流
AVCodecContext *codec：指向该视频/音频流的AVCodecContext（它们是一一对应的关系）
AVRational time_base：时基。通过该值可以把PTS，DTS转化为真正的时间。FFMPEG其他结构体中也有这个字段，但是根据我的经验，只有AVStream中的time_base是可用的。PTS*time_base=真正的时间
AVDictionary *metadata：元数据信息
AVRational avg_frame_rate：帧率（注：对视频来说，这个挺重要的）
AVPacket attached_pic：附带的图片。比如说一些MP3，AAC音频文件附带的专辑封面。
int64_t duration：该视频/音频流长度
```



### AVIOContext

FFMPEG管理输入输出数据的结构体里面中有以下几个变量比较重要：

```
unsigned char *buffer：缓存开始位置
int buffer_size：缓存大小（默认32768）
unsigned char *buf_ptr：当前指针读取到的位置
unsigned char *buf_end：缓存结束的位置
void *opaque：URLContext结构体
```

在解码的情况下，buffer用于存储ffmpeg读入的数据。例如打开一个视频文件的时候，先把数据从硬盘读入buffer，然后在送给解码器用于解码。

其中opaque指向了URLContext。注意，这个结构体并不在FFMPEG提供的头文件中，而是在FFMPEG的源代码中。URLContext结构体中还有一个结构体URLProtocol。在这个结构体中，除了一些回调函数接口之外，有一个变量constchar *name，该变量存储了协议的名称。每一种输入协议都对应这样一个结构体。

### AVCodecContext

挑一些关键的变量来看看（这里只考虑解码）。

```
enum AVMediaType codec_type：编解码器的类型（视频，音频...）
struct AVCodec *codec：采用的解码器AVCodec（H.264,MPEG2...）
int bit_rate：平均比特率
uint8_t *extradata; int extradata_size：针对特定编码器包含的附加信息（例如对于H.264解码器来说，存储SPS，PPS等）
AVRational time_base：根据该参数，可以把PTS转化为实际的时间（单位为秒s）
int width, height：如果是视频的话，代表宽和高
int refs：运动估计参考帧的个数（H.264的话会有多帧，MPEG2这类的一般就没有了）
int sample_rate：采样率（音频）
int channels：声道数（音频）
enum AVSampleFormat sample_fmt：采样格式
int profile：型（H.264里面就有，其他编码标准应该也有）
int level：级（和profile差不太多）
```

在这里需要注意：AVCodecContext中很多的参数是编码的时候使用的，而不是解码的时候使用的。

### AVFormatContext:

在使用FFMPEG进行开发的时候，AVFormatContext是一个贯穿始终的数据结构，很多函数都要用到它作为参数。它是FFMPEG解封装（flv，mp4，rmvb，avi）功能的结构体。下面看几个主要变量的作用（在这里考虑解码的情况）：

```
struct AVInputFormat *iformat：输入数据的封装格式
AVIOContext *pb：输入数据的缓存
unsigned int nb_streams：视音频流的个数
AVStream **streams：视音频流
char filename[1024]：文件名
int64_t duration：时长（单位：微秒ms，转换为秒需要除以1000000）
int bit_rate：比特率（单位bps，转换为kbps需要除以1000）
AVDictionary *metadata：元数据
```

视频的时长可以转换成HH:MM:SS的形式，示例代码如下：

```
AVFormatContext *pFormatCtx;
CString timelong;
//duration是以微秒为单位
//转换成hh:mm:ss形式
int tns, thh, tmm, tss;
tns = (pFormatCtx->duration)/1000000;
thh = tns / 3600;
tmm = (tns % 3600) / 60;
tss = (tns % 60);
timelong.Format("%02d:%02d:%02d",thh,tmm,tss);
```

### AVFrame

AVFrame结构体一般用于存储原始数据（即非压缩数据，例如对视频来说是YUV，RGB，对音频来说是PCM），此外还包含了一些相关的信息。比如说，解码的时候存储了宏块类型表，QP表，运动矢量表等数据。编码的时候也存储了相关的数据。因此在使用FFMPEG进行码流分析的时候，AVFrame是一个很重要的结构体。
下面看几个主要变量的作用（在这里考虑解码的情况）

```
uint8_t *data[AV_NUM_DATA_POINTERS]：解码后原始数据（对视频来说是YUV，RGB，对音频来说是PCM）
int linesize[AV_NUM_DATA_POINTERS]：data中“一行”数据的大小。注意：未必等于图像的宽，一般大于图像的宽。
int width, height：视频帧宽和高（1920x1080,1280x720...）
int nb_samples：音频的一个AVFrame中可能包含多个音频帧，在此标记包含了几个
int format：解码后原始数据类型（YUV420，YUV422，RGB24...）
int key_frame：是否是关键帧
enum AVPictureType pict_type：帧类型（I,B,P...）
AVRational sample_aspect_ratio：宽高比（16:9，4:3...）
int64_t pts：显示时间戳
int coded_picture_number：编码帧序号
int display_picture_number：显示帧序号
int8_t *qscale_table：QP表
uint8_t *mbskip_table：跳过宏块表
int16_t (*motion_val[2])[2]：运动矢量表
uint32_t *mb_type：宏块类型表
short\*dct_coeff：DCT系数，这个没有提取过
int8_t \*ref_index[2]：运动估计参考帧列表（貌似H.264这种比较新的标准才会涉及到多参考帧）
int interlaced_frame：是否是隔行扫描
uint8_t motion_subsample_log2：一个宏块中的运动矢量采样个数，取log的
```

### AVPacket

存储压缩编码数据相关信息的结构体 在AVPacket结构体中，重要的变量有以下几个

```
uint8_t *data：压缩编码的数据接收数据流就是由这个变量接收。
例如对于H.264来说。1个AVPacket的data通常对应一个NAL。
注意：在这里只是对应，而不是一模一样。他们之间有微小的差别：使用FFMPEG类库分离出多媒体文件中的H.264码流
因此在使用FFMPEG进行视音频处理的时候，常常可以将得到的AVPacket的data数据直接写成文件，从而得到视音频的码流文件。
int size：data的大小
int64_t pts：显示时间戳
int64_t dts：解码时间戳
int stream_index：标识该AVPacket所属的视频/音频流。
```

### AVCodec

存储编解码器信息的结构体 主要的几个变量

```
const char *name：编解码器的名字，比较短
const char *long_name：编解码器的名字，全称，比较长
enum AVMediaType type：指明了类型，是视频，音频，还是字幕
enum AVCodecID id：ID，不重复
const AVRational *supported_framerates：支持的帧率（仅视频）
const enum AVPixelFormat *pix_fmts：支持的像素格式（仅视频）
const int *supported_samplerates：支持的采样率（仅音频）
const enum AVSampleFormat *sample_fmts：支持的采样格式（仅音频）
const uint64_t *channel_layouts：支持的声道数（仅音频）
int priv_data_size：私有数据的大小
```

每一个编解码器对应一个该结构体，查看一下ffmpeg的源代码，我们可以看一下H.264解码器的结构体如下所示（h264.c）：

```
AVCodec ff_h264_decoder = {
.name           = "h264",
.type           = AVMEDIA_TYPE_VIDEO,
.id            = CODEC_ID_H264,
.priv_data_size = sizeof(H264Context),
.init           = ff_h264_decode_init,
.close          = ff_h264_decode_end,
.decode         = decode_frame,
.capabilities   = CODEC_CAP_DR1 | CODEC_CAP_DELAY |
CODEC_CAP_SLICE_THREADS | CODEC_CAP_FRAME_THREADS,
.flush          = flush_dpb,
.long_name =NULL_IF_CONFIG_SMALL("H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
.init_thread_copy =ONLY_IF_THREADS_ENABLED(decode_init_thread_copy),
.update_thread_context =ONLY_IF_THREADS_ENABLED(decode_update_thread_context),
.profiles = NULL_IF_CONFIG_SMALL(profiles),
.priv_class    = &h264_class,
};
```

AVPixelFormat定义如下

```c
enum AVPixelFormat {
AV_PIX_FMT_NONE = -1,
AV_PIX_FMT_YUV420P, ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per2x2 Y samples)
AV_PIX_FMT_YUYV422, ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
AV_PIX_FMT_RGB24, ///< packed RGB 8:8:8, 24bpp, RGBRGB...
AV_PIX_FMT_BGR24, ///< packed RGB 8:8:8, 24bpp, BGRBGR...
AV_PIX_FMT_YUV422P, ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per2x1 Y samples)
AV_PIX_FMT_YUV444P, ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per1x1 Y samples)
AV_PIX_FMT_YUV410P, ///< planar YUV 4:1:0, 9bpp, (1 Cr & Cb sample per4x4 Y samples)
AV_PIX_FMT_YUV411P, ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per4x1 Y samples)
AV_PIX_FMT_GRAY8, ///< Y , 8bpp
AV_PIX_FMT_MONOWHITE, ///< Y , 1bpp, 0 is white, 1 is black,
in each byte pixels are ordered from the msb to the lsb
AV_PIX_FMT_MONOBLACK, ///< Y ,1bpp, 0 is black, 1 is white,
in each byte pixels are ordered from the msb to the lsb
AV_PIX_FMT_PAL8, ///< 8 bit with PIX_FMT_RGB32 palette
AV_PIX_FMT_YUVJ420P, ///< planar YUV 4:2:0, 12bpp, full scale (JPEG),
deprecated in favor of PIX_FMT_YUV420P and setting color_range
AV_PIX_FMT_YUVJ422P, ///< planar YUV 4:2:2, 16bpp, full scale (JPEG),
deprecated in favor of PIX_FMT_YUV422P and setting color_range
AV_PIX_FMT_YUVJ444P, ///< planar YUV 4:4:4, 24bpp, full scale (JPEG),
deprecated in favor of PIX_FMT_YUV444P and setting color_range
AV_PIX_FMT_XVMC_MPEG2_MC,///< XVideo Motion Acceleration via common packetpassing
AV_PIX_FMT_XVMC_MPEG2_IDCT,
... 
}
```

AVDictionary

------

```c
#define AV_DICT_MATCH_CASE      1
#define AV_DICT_IGNORE_SUFFIX   2
#define AV_DICT_DONT_STRDUP_KEY 4   
#define AV_DICT_DONT_STRDUP_VAL 8  
#define AV_DICT_DONT_OVERWRITE 16   
#define AV_DICT_APPEND         32  
typedef struct {
   char *key;
   char *value;
} AVDictionaryEntry;
struct AVDictionary {
   int count;
   AVDictionaryEntry *elems;
};
```

 

从字面上看这是一个键值对, 更宏观一些则为键值对数组, 嗯, 动态数组

这就能构建我们通常所说的字典?

这个数据结构有4个操作, 读(查找)/写(创建)/拷贝, 释放

 

里面有几个操作标志, 记录下

```
AV_DICT_MATCH_CASE     查找时是否比较大小, 不比较大小则全部转化为大写来比较

AV_DICT_IGNORE_SUFFIX  查找时是否忽略后缀, 这里的后缀指字典里key 超过查找key 部分
                       如果设置了改参数则, 查找"abc", 可能会找到"abcde" 这样的key

AV_DICT_DONT_STRDUP_KEY 设置key时, 是否把key 字符串复制一份

AV_DICT_DONT_STRDUP_VAL 设置value时, 是否把value 字符串复制一份

AV_DICT_DONT_OVERWRITE  写入时不要覆盖原来存在的key(存在key 则不设置), 否则把原来的key/value 释放重建

AV_DICT_APPEND    如果已经存在key/value, 把将新的value 值连到旧的后面
```

 

 

## ffmpeg源码跟踪之时间基转换，时间戳比较

AV_TIME_BASE

ffmpeg中的内部计时单位（时间基），ffmepg中的所有时间都是于它为一个单位，比如AVStream中的duration即以为着这个流的长度为duration个AV_TIME_BASE。AV_TIME_BASE定义为：

\#define         AV_TIME_BASE   1000000

 

AV_TIME_BASE_Q

ffmpeg内部时间基的分数表示，实际上它是AV_TIME_BASE的倒数。从它的定义能很清楚的看到这点：

\#define         AV_TIME_BASE_Q   (AVRational){1, AV_TIME_BASE}

AVRounding定义

AVRounding { 
  AV_ROUND_ZERO = 0, AV_ROUND_INF = 1, AV_ROUND_DOWN = 2, AV_ROUND_UP= 3, 
  AV_ROUND_NEAR_INF = 5, AV_ROUND_PASS_MINMAX = 8192 
}

我们转换时间基的时候用到av_rescale_q，av_rescale_q_rnd，av_compare_ts这些函数。

函数av_rescale_rnd其实就是返回(a * b)/c，只是加了一些舍入方案。

av_rescale_q(pts, timebase1, timebase2)，其实就是按照下面的公式计算了一下， 公式如下：  

x = pts * (timebase1.num / timebase1.den )*(timebase2.den / timebase2.num);

这个x就是转换后的时间戳。

int64_t av_rescale_q_rnd(int64_t a, AVRational bq, AVRational cq,

​                         enum AVRounding) av_const;

此函数主要用于对于不同时间戳的转换。跟av_rescale_q一样，只是增加AVRounding标记，做越界处理。

 

FFMPEG的很多结构中有AVRationaltime_base;这样的一个成员，它是AVRational结构的

typedef struct AVRational{

   int num; ///<numerator

   int den; ///<denominator

} AVRational;

AVRational这个结构标识一个分数，num为分数，den为分母。

实际上time_base的意思就是时间的刻度：

如（1,25），那么时间刻度就是1/25

（1,9000），那么时间刻度就是1/90000

那么，在刻度为1/25的体系下的time=5，转换成在刻度为1/90000体系下的时间time为(5*1/25)/(1/90000)= 3600*5=18000

ffmpeg中做pts计算时，存在大量这种转换在以下结构中都有。

AVCodecContext：编解码上下文。

AVStream:文件或其它容器中的某一条流。

如果由某个解码器产生固定帧率的码流

AVCodecContext中的AVRational根据帧率来设定，如25帧，那么num = 1，den=25

AVStream中的time_base一般根据其采样频率设定，如（1，90000）

在某些场景下涉及到PTS的计算时，就涉及到两个Time的转换，以及到底取哪里的time_base进行转换：

场景1：编码器产生的帧，直接存入某个容器的AVStream中，那么此时packet的Time要从AVCodecContext的time转换成目标AVStream的time

场景2：从一种容器中demux出来的源AVStream的frame，存入另一个容器中某个目的AVStream。

​           此时的时间刻度应该从源AVStream的time，转换成目的AVStream timebase下的时间。

其实，问题的关键还是要理解，不同的场景下取到的数据帧的time是相对哪个时间体系的。

demux出来的帧的time：是相对于源AVStream的timebase

编码器出来的帧的time：是相对于源AVCodecContext的timebase

mux存入文件等容器的time：是相对于目的AVStream的timebase

这里的time指pts。

转码时:

在解码之前需要进行如下转换:

packet.pts = av_rescale_q_rnd(packet.pts,  

​                    ic->streams[videoindex]->time_base,  

​                    ic->streams[videoindex]->codec->time_base,  

​                    (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));  


在编码后写到file之前需要进行如下转换:

pkt.pts = av_rescale_q_rnd(pkt.pts,  

​                            oc->streams[videoindex]->codec->time_base,  

​                            oc->streams[videoindex]->time_base,  

​                            (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));  

 

 

时间戳比较av_compare_ts函数

```cpp
int av_compare_ts(int64_t ts_a, AVRational tb_a, int64_t ts_b, AVRational tb_b)  
{  
    int64_t a = tb_a.num * (int64_t)tb_b.den;  
    int64_t b = tb_b.num * (int64_t)tb_a.den;  
    if ((FFABS(ts_a)|a|FFABS(ts_b)|b) <= INT_MAX)  
        return (ts_a*a > ts_b*b) - (ts_a*a < ts_b*b);  
    if (av_rescale_rnd(ts_a, a, b, AV_ROUND_DOWN) < ts_b)  
        return -1;  
    if (av_rescale_rnd(ts_b, b, a, AV_ROUND_DOWN) < ts_a)  
        return 1;  
    return 0;  
}  
```

## FFmpeg中的时间戳（DTS和PTS）

FFmpeg里有两种时间戳：DTS（Decoding TimeStamp）和PTS（Presentation Time Stamp）。 顾名思义，前者是解码的时间，后者是显示的时间。要仔细理解这两个概念，需要先了解FFmpeg中的packet和frame的概念。

FFmpeg中用AVPacket结构体来描述解码前或编码后的压缩包，用AVFrame结构体来描述解码后或编码前的信号帧。对于视频来说，AVFrame就是视频的一帧图像。这帧图像什么时候显示给用户，就取决于它的PTS。DTS是AVPacket里的一个成员，表示这个压缩包应该什么时候被解码。如果视频里各帧的编码是按输入顺序（也就是显示顺序）依次进行的，那么解码和显示时间应该是一致的。可事实上，在大多数编解码标准（如H.264或HEVC）中，编码顺序和输入顺序并不一致。 于是才会需要PTS和DTS这两种不同的时间戳。

DTS主要用于视频的解码,在解码阶段使用.PTS主要用于视频的同步和输出.在显示的时候使用.在没有B frame的情况下.DTS和PTS的输出顺序是一样的.

I frame 的解码不依赖于任何的其它的帧.而p frame的解码则依赖于其前面的I frame或者P frame.B frame的解码则依赖于其前的最近的一个I frame或者P frame 及其后的最近的一个P frame.

 

### 编码过程中pts计算方法

av_rescale_q用于计算Packet的PTS。av_rescale_q的返回值是一个很大的整数，且每次计算的结果间隔很大。

不同于avcodec_encode_video改变AVCodecContext *avctx的pts（小整数，且间隔小）。

av_rescale_q(a,b,c)是用来把时间戳从一个时基调整到另外一个时基时候用的函数。它基本的动作是计算a*b/c，但是这个函数还是必需的，因为直接计算会有溢出的情况发生。AV_TIME_BASE_Q是AV_TIME_BASE作为分母后的版本。它们是很不相同的：AV_TIME_BASE * time_in_seconds = avcodec_timestamp而AV_TIME_BASE_Q * avcodec_timestamp= time_in_seconds（注意AV_TIME_BASE_Q实际上是一个AVRational对象，所以你必需使用avcodec中特定的q函数来处理它）。

 

H264/90000,代表时钟频率必须是90000

 

背景知识:

(一个AAC原始帧包含一段时间内1024个采样及相关数据)

\1. 视频时间戳

pts = inc++ *(1000/fps); 其中inc是一个静态的，初始值为0，每次打完时间戳inc加1.

在ffmpeg，中的代码为

pkt.pts= m_nVideoTimeStamp++ *(m_VCtx->time_base.num * 1000 / m_VCtx->time_base.den);

\2. 音频时间戳

pts = inc++ * (frame_size * 1000 /sample_rate)

在ffmpeg中的代码为

pkt.pts= m_nAudioTimeStamp++ *(m_ACtx->frame_size * 1000 / m_ACtx->sample_rate);

 

# ffmpeg解码H264缺少帧的解决办法

当使用avcodec_decode_video2时，如果第三个参数的值为1，则表示完成一帧的解码，如果为0，表示没有解码完成。此时需要计算未解码的帧数，以便再次调用avcodec_decode_video2函数。当解码成功一帧时返回，如果没有解码，则累加。在最后将之前未解码的数据再次解码。

出现未解码完成的原因：B帧或延迟帧（一些时间戳在后的数据帧）不立即解码，在缓存中。

代码如下：

```cpp
int  getFrame(unsigned char** yuvBuffer, unsigned char** rgbBuffer, int* size, int* width, int* height)  
{  
    int got_picture = 0;    // 找到帧标志  
    int len = 0;  
    AVPacket avpkt;  
    av_init_packet(&avpkt);  
    //int frame = 0;  
    // av_read_fram返回下一帧，发生错误或文件结束返回<0  
    while (av_read_frame(m_fmtctx, &avpkt) >= 0)  
    {  
        // 解码视频流  
        if (avpkt.stream_index == m_videoStream)  
        {  
            len = avcodec_decode_video2(m_avctx, m_picture, &got_picture, &avpkt);  
            if (len < 0)  
            {  
                debug("error while decoding frame.\n");  
                return -1;  
            }  
            if (got_picture)  
            {  
                m_picWidth  = m_avctx->width;  
                m_picHeight = m_avctx->height;  
                // 传出原始数据指针，由于内部已经申请了，不用再开辟数据  
                if (yuvBuffer != NULL)  
                {  
                    *yuvBuffer = m_picture->data[0];  
                    if (size != NULL)  
                    {  
                        *size = len; // to check  
                    }  
                }  
                if (rgbBuffer != NULL)  
                {  
                    *rgbBuffer = convertToRgb();  
                    if (size != NULL)  
                    {  
                        *size = m_picWidth * m_picHeight * 3; // 上面指定了rgb24，所以是w*h*3  
                    }  
                }  
                //printf("frame fmt: %d\n", m_picture->format);  
                if (width != NULL)  
                {  
                    *width = m_picWidth;  
                }  
                if (height != NULL)  
                {  
                    *height = m_picHeight;  
                }  
                //printf("bit_rate: %d width: %d height:%d\n", m_avctx->bit_rate, m_avctx->width, m_avctx->height);  
                return 1;  
            } // end of got picture  
            else  
            {  
                m_skippedFrame++;  
                //debug("skipped count: %d\n", m_skippedFrame);  
            }  
        } // end of video stream  
        av_free_packet(&avpkt);  
    } // end of read frame  
    return 0;  
}  
上面的函数已经统计了缓存起来的帧总数，下面根据m_skippedFrame的值再调用avcodec_decode_video2解码。
int  getSkippedFrame(unsigned char** yuvBuffer, unsigned char** rgbBuffer, int* size, int* width, int* height)  
{  
    int got_picture = 0;    // 找到帧标志  
    int len = 0;  
    AVPacket avpkt;  
    memset(&avpkt, '\0', sizeof(AVPacket));  
    av_init_packet(&avpkt);  
    // 是否还有缓存的帧  
    while (m_skippedFrame-- > 0)  
    {  
        // 注：avpkt要清空data和size，否则无法解码  
        avpkt.data = NULL;  
        avpkt.size = 0;  
        // 解码视频流  
        len = avcodec_decode_video2(m_avctx, m_picture, &got_picture, &avpkt);  
        if (len < 0)  
        {  
            debug("error while decoding frame.\n");  
            return -1;  
        }  
        if (got_picture)  
        {  
            // 传出原始数据指针，由于内部已经申请了，不用再开辟数据  
            if (yuvBuffer != NULL)  
            {  
                *yuvBuffer = m_picture->data[0];  
            }  
            if (rgbBuffer != NULL)  
            {  
                *rgbBuffer = convertToRgb();  
            }  
            //printf("frame fmt: %d\n", m_picture->format);  
            if (size != NULL)  
            {  
                *size = len;  
            }  
            m_picWidth  = m_avctx->width;  
            m_picHeight = m_avctx->height;  
            if (width != NULL)  
            {  
                *width = m_picWidth;  
            }  
            if (height != NULL)  
            {  
                *height = m_picHeight;  
            }  
            //printf("bit_rate: %d width: %d height:%d\n", m_avctx->bit_rate, m_avctx->width, m_avctx->height);  
            return 1;  
        } // end of got picture  
        av_packet_unref(&avpkt);  
    } // end of read frame  
    return 0;  
}  

```

注意，在调用avcodec_decode_video2对缓存的帧解码时，一定要将avpkt.data置为NULL，并将avpkt.size置为0，否则会解码不成功。在调用av_init_packet前，也要手工对AVPacket结构体进行清零操作。在解码函数avcodec_decode_video2注释中有如下说明：

 \* @note Codecs which have theCODEC_CAP_DELAY capability set have a delay
 \* between input and output, these need to be fed withavpkt->data=NULL,
 \* avpkt->size=0 at the end to return the remaining frames.

# FFmpeg 获取H264流中的sps pps

H.264的SPS和PPS串，包含了初始化H.264解码器所需要的信息参数，包括编码所用的profile，level，图像的宽和高，deblock滤波器等。

（1）avcC的数据结构对应sps和pps流，avcC的数据结构：

```cpp
aligned(8) class AVCDecoderConfigurationRecord {     
 unsigned int(8) configurationVersion = 1;     
 unsigned int(8) AVCProfileIndication;     
 unsigned int(8) profile_compatibility;     
 unsigned int(8) AVCLevelIndication;     
 bit(6) reserved = '111111'b;     
 unsigned int(2) lengthSizeMinusOne;     
 bit(3) reserved = '111'b;     
 unsigned int(5) numOfSequenceParameterSets;     
for (i=0; i< numOfSequenceParameterSetsispan>    
 unsigned int(16) sequenceParameterSetLength ;     
 bit(8*sequenceParameterSetLength) sequenceParameterSetNALUnit;     
 }     
 unsigned int(8) numOfPictureParameterSets;     
for (i=0; i< numOfPictureParameterSetsispan>    
 unsigned int(16) pictureParameterSetLength;     
 bit(8*pictureParameterSetLength) pictureParameterSetNALUnit;     
 }     
}     
```

(2) FFmpeg 如何获取sps和pps

方法一：分析字节流数据。

ffmpeg获取sps和pps非常简单。avcC数据结构对应于AVFormatContext->streams[H264Index]->codec->extradata。

```cpp
if ((ret = avformat_open_input(&ic, InputFileName, NULL, NULL)) < 0)  
{  
    xprintf->Trace(0,"******** Decode avformat_open_input() Function result=%d",ret);  
    return ret;  
}  
if ((ret = avformat_find_stream_info(ic, NULL)) < 0)   
{    
    xprintf->Trace(0,"******** Decode avformat_find_stream_info() Function result=%d ",ret);  
    avformat_close_input(&ic);    
    return ret;    
}    
for (int i=0;i<ic->streams[0]->codec->extradata_size;i++)  
{  
    printf("%x ",ic->streams[0]->codec->extradata[i]);  
}  
//分析字节流数据，解析上面的avcC数据结构
```

 

方法二：使用ffmpeg的流过滤器获取sps及pps

流过滤器"h264_mp4toannexb",在av_register_all()函数中会被注册。

```cpp
int ParseH264ExtraDataInMp4(int stream_id)  
{  
    uint8_t *dummy = NULL;  
    int dummy_size;  
    AVBitStreamFilterContext* bsfc =  av_bitstream_filter_init("h264_mp4toannexb");  
    if(bsfc == NULL)  
    {  
        return -1;  
    }  
    av_bitstream_filter_filter(  
            bsfc, format_ctx_->streams[stream_id]->codec, NULL, &dummy, &dummy_size, NULL, 0, 0);  
  av_bitstream_filter_close(bsfc);  
}
```

 

**关于ffmpeg封装h264为mp4/flv/mkv的问题**

在使用FFMPEG的类库进行编程的过程中，可以直接输出解复用之后的的视频数据码流。只需要在每次调用av_read_frame()之后将得到的视频的AVPacket存为本地文件即可。

经试验，在分离MPEG2码流的时候，直接存储AVPacket即可。

在分离H.264码流的时候，直接存储AVPacket后的文件可能是不能播放的。如果视音频复用格式是TS（MPEG2 Transport Stream），直接存储后的文件是可以播放的。复用格式是FLV，MP4则不行。

 

对于h264文件来说，h264有两种封装，
    一种是annexb模式，传统模式，有startcode，SPS和PPS是在ES中。
     一种是mp4模式，一般mp4、flv、mkv都没有startcode，SPS和PPS以及其它信息被封装在container中，每一个frame前面是使用NALU长度（固定字节，通常为4字节，大端模式）分隔NAL。
很多解码器只支持annexb这种模式，因此需要将mp4做转换：在ffmpeg中用h264_mp4toannexb_filter可以做转换。

代码：

```cpp
FILE *fp=fopen("test.264","ab");
AVCodecContext *pCodecCtx=... 
unsigned char *dummy=NULL;   //输入的指针  
int dummy_len;  
AVBitStreamFilterContext* bsfc =  av_bitstream_filter_init("h264_mp4toannexb");    
av_bitstream_filter_filter(bsfc, pCodecCtx, NULL, &dummy, &dummy_len, NULL, 0, 0);  
fwrite(pCodecCtx->extradata,pCodecCtx-->extradata_size,1,fp);  
av_bitstream_filter_close(bsfc);    
free(dummy);  
```

 

ffmpeg读取mp4中的H264数据，并不能直接得到NALU，文件中也没有储存0x00000001的分隔符。packet中的数据起始处没有分隔符(0x00000001), 也不是0x65、0x67、0x68、0x41等字节，所以可以肯定这不是标准的nalu。其实，前4个字0x000032ce表示的是nalu的长度，从第5个字节开始才是nalu的数据。所以直接将前4个字节替换为0x00000001即可得到标准的nalu数据。

 

**关于ffmpeg封装aac为mp4音频的问题**

FFmpeg官方文档对aac_adtstoasc的说明：

1）将AAC编码器编码后的原始码流（ADTS头 + ES流）封装为MP4或者FLV或者MOV等格式时，需要先将ADTS头转换为MPEG-4AudioSpecficConfig （将音频相关编解码参数提取出来），并将原始码流中的ADTS头去掉（只剩下ES流）。

2）相反，从MP4或者FLV或者MOV等格式文件中解封装出AAC码流（只有ES流）时，需要在解析出的AAC码流前添加ADTS头（含音频相关编解码参数）。

 

| ADTS AAC    |        |             |        |      |             |        |
| ----------- | ------ | ----------- | ------ | ---- | ----------- | ------ |
| ADTS_header | AAC ES | ADTS_header | AAC ES | ...  | ADTS_header | AAC ES |





 

MP4 container requires AV_CODEC_FLAG_GLOBAL_HEADER, which means all stream should containsstream data only, and other data is provided by setting AVCodecContext.extradata. Because MP4 has its own way oftransporting meta information (here transport info), writing that transportprefix before each frame will make the data unreadable.



//ofmt_ctx is AVFormatContext

//enc_ctx is the AVCodecContext of thecurrent stream

if (ofmt_ctx->oformat->flags &AVFMT_GLOBALHEADER)

   enc_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

Without them the encodermay add the metadatas in the data which is sent to the container. For AACit is the ADTS header, for H264 it is SPS and PPS data.

通过“aac_adtstoasc”的bitstream filter做转换，将在音频es之前增加adts头。

代码参考这篇文章：<https://blog.csdn.net/leixiaohua1020/article/details/39802913>

 

**ffmpeg中新的BitStreamFilter（h264_mp4toannexb、aac_adtstoasc等）的使用方法**

FFMPEG转码器在封装成FLV或者MP4的时候，会用到h264_mp4toannexb、aac_adtstoasc这两个filter。老版本的bsf使用需要特别注意，一个不小心，就造成的内存泄漏，不容易察觉。 

1、初始化并申请空间

```cpp
AVBSFContext *bsf_ctx;
AVBitStreamFilter *filter =av_bsf_get_by_name("aac_adtstoasc");
if(!filter)
{
   av_log(NULL,AV_LOG_ERROR,"Unkonw bitstream filter");
}
int ret = av_bsf_alloc(filter,&bsf_ctx);
```

 

2、使用av_bsf_send_packet和av_bsf_receive_packet处理数据，

```cpp
AVPacket pkt;
...
av_read_frame(ifmt_ctx, &pkt);
//把pkt数据推送到filter中去
ret = av_bsf_send_packet(bsf_ctx,&pkt);
if(ret < 0)
   //推送失败，做异常处理
//获取处理后的数据，用同一个pkt
ret = av_bsf_receive_packet(bsf_ctx,&pkt);
if(ret < 0)
   //读取失败，做异常处理

```

 

3、释放

av_bsf_free(&bsf_ctx);

**视音频同步**

音视频同步方法：选择一个参考时钟，参考时钟上的时间是线性递增的，生成数据流时依据参考时钟上的时间给每个数据块

都打上时间戳（一般包括开始时间和结束时间）。在播放时，读取数据块上的时间戳，同时参考当前参考时钟上的时间来安

排播放。数据流不会发生参考关系。

 步骤:
1, 首先分离器分解为音频和视频数据流

2，输出以前进行时间戳比较，相同则是同步的，直接输出。

3，不同的则经过同步函数进行调整之后再输出

decoder 可以根据frame rate 计算每个frame 的间隔时间，只要知道第一个frame的pts，后面的pts就可以根据frame rate计算出来。

PTS：presentation time stamp 显示时间戳

<https://blog.csdn.net/wh8_2011/article/details/52252024>

 

**使用mp4v2将H264+AAC合成mp4文件**

<https://www.cnblogs.com/lidabo/p/3832634.html>

 

 

参考资料：

<http://ffmpeg.org/>

<https://blog.csdn.net/column/details/ffmpeg-devel.html>

<https://www.cnblogs.com/lidabo/p/3832634.html>