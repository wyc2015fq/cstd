# 【ffmpeg重温】 - 三少GG - CSDN博客
2011年09月22日 18:02:35[三少GG](https://me.csdn.net/scut1135)阅读数：2381
什么是muxer/demuxer?浏览次数：2099次 悬赏分：10 | 解决时间：2007-8-1 09:59 | 提问者：lxafyk如题
最佳答案：
**muxer是合并将视频文件、音频文件和字幕文件合并为某一个视频格式。如，可将a.avi, a.mp3, a.srt用muxer合并为mkv格式的视频文件。demuxer是拆分这些文件的。**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## ffmpeg解码顺序及相关
fmpeg解码的顺序大致如下:
1.初始化解码器:avcodec_init();
2.注册所有格式和解码器:av_register();
3.打开文件:av_open_input_file(filename);
4.取出数据流信息:av_find_stream_info();
5.判断数据流是否为视频,并提取器编码信息:AVCodec->codec_type ==CODE_ID_VIDEO;
6.根据编码信息寻找相应的解码器:avcodec_find_decoder(AVCodecContext->codec_id);
7.打开视频解码器:avcodec_open();
8.为视频帧.........
avcodec_init主要包括dsp_static_init函数，而dsp_static_init函数主要负责静态查找表结构的初始化。在baseline中，主要是初始化ff_cropTbl。这个表很有意思，a= ff_cropTble[i]实现的功能实际上是a = i<0 ? 0 :(i>255 ? 255 : i);这个表的大小是2*MAX_NEG_CROP+256,当MAX_NEG_CROP定义为1024时，实际上这个表可将-1024至1024的任意数转化为0-255的数。这样设计的实质是用查找表方式来减少所需执行的指令数量，是用空间上的代价来换取速度的提高，这是一种典型的优化方式。
avcodec_register_all主要包括register_avcodec(&h264_decoder)，这个函数主要负责登记所需的编解码器，在ffmpeg中包括各种编解码器，要将它们登记在一个编解码器列表中，在本设计中，该列表只包括h264解码器。
video_decode_example分以下几个函数：
avcodec_find_decoder：在登记的编解码器列表中找到h264解码器（通过CodecID)；
avcodec_alloc_context:主要包括avcodec_get_context_defaults2函数。它负责初始化AVCodecContext中的某些函数指针（get_buffer, release_buffer)及某些参数设置（thread_count)；
avcodec_alloc_frame:主要包括avcodec_get_frame_default函数。主要是分配AVFrame的空间，初始化一些属性（如key_frame=1)，并返回指向AVFrame的指针；
avcodec_open：为H264Context（它是avctx->priv_data)分配空间(122712字节），并清0;设置AVCodecContext结构体中的codec，codec_id,frame_number，并利用avctx->codec->init(decode_init)来初始化解码器。
   decode_init：包括以下几个函数
    首先设置s->chroma_qscale_table和s->picture_structure，s->avctx。
    common_init： 设置s->width, height,codec_id。然后ff_h264_pred_init函数，再设置h->dequant_coeff_pps，再初始化pps.scaling_matrix4的值。ff_h264_pred_init：初始化帧内预测函数的指针，包括9+3种4*4块，4+3种8*8色度块和 4+3种16*16块.
    decode_init_vlc: 建立CAVLC相关的表格（包括chroma_dc_coeff_token, coeff_token,total_zeros,run_before)。主要通过init_vlc函数实现。init_vlc：建立某个CAVLC的表格，主要通过build_table实现。
   decode_init结束；
avcodec_open结束；
打开图像文件；
进入无限循环体for(;;)
读入INBUF_SIZE大小的数据，若返回为0，刚break
avcodec_decode_video函数；（下面单独讲此函数）
如果解码完一帧图像，则执行yuv_output，其实就是将yuv数据写到.yuv文件里
循环体结尾
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[ffmpeg分析系列之一(注册该注册的)](http://hi.baidu.com/monkeymeetfox/blog/item/5b95eec7932b2645b319a8b7.html)
**hello，各位好，本人是一名嵌入式软件工程师，目前正使用ffmpeg开发一款嵌入式多媒体播放器，《ffmpeg分析》系列博文是本人在阅读ffmpeg源代码时所做的笔记，希望对各位有点帮助。分析过程结合下面的例程：**
**[http://dranger.com/ffmpeg/tutorial05.c](http://dranger.com/ffmpeg/tutorial05.c)**
**一. 调用av_register_all函数注册所有的格式和编码解码器.**
**1.1 先调用avcodec_register_all函数注册所有的编码解码器.**
**1. 下面列出与H264相关的注册:**
```
// 注册硬件加速器
     REGISTER_HWACCEL (H264_DXVA2, h264_dxva2);
     REGISTER_HWACCEL (H264_VAAPI, h264_vaapi);
    // 注册解码器
     REGISTER_DECODER (H264, h264);
     REGISTER_DECODER (H264_VDPAU, h264_vdpau);
    // 注册编码器
     REGISTER_ENCODER (LIBX264, libx264);
    // 注册分析器
     REGISTER_PARSER (H264, h264);
    // 注册位流分离器
     REGISTER_BSF (H264_MP4TOANNEXB, h264_mp4toannexb);
```
**2. 下面列出注册宏:**
```
#define REGISTER_HWACCEL(X,x){
 \
          extern AVHWAccel x##_hwaccel; \
          if(CONFIG_##X##_HWACCEL)
 av_register_hwaccel(&x##_hwaccel);}
#define REGISTER_ENCODER(X,x){ \
          extern AVCodec x##_encoder; \
          if(CONFIG_##X##_ENCODER)
 avcodec_register(&x##_encoder);}
#define REGISTER_DECODER(X,x){ \
          extern AVCodec x##_decoder; \
          if(CONFIG_##X##_DECODER)
 avcodec_register(&x##_decoder);}
#define REGISTER_ENCDEC(X,x) REGISTER_ENCODER(X,x);
 REGISTER_DECODER(X,x)
#define REGISTER_PARSER(X,x){ \
          extern AVCodecParser x##_parser; \
          if(CONFIG_##X##_PARSER)
 av_register_codec_parser(&x##_parser);}
#define REGISTER_BSF(X,x){ \
          extern AVBitStreamFilter x##_bsf; \
          if(CONFIG_##X##_BSF)
 av_register_bitstream_filter(&x##_bsf);}
```
**3. 分析一下注册函数, 以avcodec_register函数为例:**
```
void avcodec_register(AVCodec*codec)
{
     AVCodec **p;
     avcodec_init();
     p = &first_avcodec;
    while (*p!=NULL)
 p =&(*p)->next;
    *p = codec;
     codec->next=NULL;
}
```
**   可以看到avcodec_register函数把输入的AVCodec连成一个链表, 其它注册函数与之类似, 就不多言了.**
**4. 上面调用了avcodec_init函数:**
```
void avcodec_init(void)
{
    static int initialized= 0;
     if (initialized!= 0)
        return;
     initialized = 1;
     dsputil_static_init ();
}
```
**   这个函数只会真正执行一次.**
**5. 上面调用了dsputil_static_init函数:**
```
av_coldvoid dsputil_static_init(void)
{
    int i;
     for(i=0;i<256;i++)
 ff_cropTbl[i + MAX_NEG_CROP]= i;
    for(i=0;i<MAX_NEG_CROP;i++){
         ff_cropTbl[i]= 0;
         ff_cropTbl[i 
+ MAX_NEG_CROP + 256]= 255;
    }
     for(i=0;i<512;i++){
         ff_squareTbl[i]=(i- 256)*(i-
 256);
    }
     for(i=0; i<64; i++)
 inv_zigzag_direct16[ff_zigzag_direct[i]]= i+1;
}
```
**   可以看到, 它初始化了一些静态数据.**
**1.2 注册所有的格式和外部库及协议.**
**1. 下面列出与H264相关的注册:**
```
// 注册分离器和混合器
     REGISTER_MUXDEMUX (H264, h264);
    // 注册文件协议
     REGISTER_PROTOCOL (FILE,file);
```
**2. 下面列出注册宏:**
```
#define REGISTER_MUXER(X,x){
 \
    extern AVOutputFormat x##_muxer; \
    if(CONFIG_##X##_MUXER)
 av_register_output_format(&x##_muxer);}
#define REGISTER_DEMUXER(X,x){ \
    extern AVInputFormat x##_demuxer; \
    if(CONFIG_##X##_DEMUXER)
 av_register_input_format(&x##_demuxer);}
#define REGISTER_MUXDEMUX(X,x) REGISTER_MUXER(X,x);
 REGISTER_DEMUXER(X,x)
#define REGISTER_PROTOCOL(X,x){ \
    extern URLProtocol x##_protocol; \
    if(CONFIG_##X##_PROTOCOL)
 av_register_protocol(&x##_protocol);}
```
**   这些注册函数与avcodec_register函数类似, 就不多言了.**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1. [http://blog.csdn.net/menuconfig/article/category/314119](http://blog.csdn.net/menuconfig/article/category/314119)    【2011.12.22】
2. [http://helloxchen.itpub.net/category/42725/66651](http://helloxchen.itpub.net/category/42725/66651)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[FFMpeg框架代码阅读](http://hi.baidu.com/%C0%E4%D1%AA%B1%F9%B7%E5/blog/item/1a920adc4282c63c5882dde6.html)1. 简介
FFmpeg是一个集录制、转换、音/视频编码解码功能为一体的完整的开源解决方案。FFmpeg的
开发是基于Linux操作系统，但是可以在大多数操作系统中编译和使用。FFmpeg支持MPEG、
DivX、MPEG4、AC3、DV、FLV等40多种编码，AVI、MPEG、OGG、Matroska、ASF等90多种解码.
TCPMP, VLC, MPlayer等开源播放器都用到了FFmpeg。
FFmpeg主目录下主要有libavcodec、libavformat和libavutil等子目录。其中libavcodec用
于存放各个encode/decode模块，libavformat用于存放muxer/demuxer模块，libavutil用于
存放内存操作等辅助性模块。
以flash movie的flv文件格式为例， muxer/demuxer的flvenc.c和flvdec.c文件在
libavformat目录下，encode/decode的mpegvideo.c和h263de.c在libavcodec目录下。
2. muxer/demuxer与encoder/decoder定义与初始化
muxer/demuxer和encoder/decoder在FFmpeg中的实现代码里，有许多相同的地方，而二者最
大的差别是muxer 和demuxer分别是不同的结构AVOutputFormat与AVInputFormat，而encoder
和decoder都是用的AVCodec 结构。
muxer/demuxer和encoder/decoder在FFmpeg中相同的地方有：
   二者都是在main()开始的av_register_all()函数内初始化的
   二者都是以链表的形式保存在全局变量中的
       muxer/demuxer是分别保存在全局变量AVOutputFormat *first_oformat与
       AVInputFormat *first_iformat中的。
       encoder/decoder都是保存在全局变量AVCodec *first_avcodec中的。
   二者都用函数指针的方式作为开放的公共接口
demuxer开放的接口有：
   int (*read_probe)(AVProbeData *);
   int (*read_header)(struct AVFormatContext *, AVFormatParameters *ap);
   int (*read_packet)(struct AVFormatContext *, AVPacket *pkt);
   int (*read_close)(struct AVFormatContext *);
   int (*read_seek)(struct AVFormatContext *, int stream_index, int64_t timestamp, int flags);
muxer开放的接口有：
   int (*write_header)(struct AVFormatContext *);
   int (*write_packet)(struct AVFormatContext *, AVPacket *pkt);
   int (*write_trailer)(struct AVFormatContext *);
encoder/decoder的接口是一样的，只不过二者分别只实现encoder和decoder函数：
   int (*init)(AVCodecContext *);
   int (*encode)(AVCodecContext *, uint8_t *buf, int buf_size, void *data);
   int (*close)(AVCodecContext *);
   int (*decode)(AVCodecContext *, void *outdata, int *outdata_size, uint8_t *buf, int buf_size);
仍以flv文件为例来说明muxer/demuxer的初始化。
在libavformat\allformats.c文件的av_register_all(void)函数中，通过执行
REGISTER_MUXDEMUX(FLV, flv);
将支持flv 格式的flv_muxer与flv_demuxer变量分别注册到全局变量first_oformat与first_iformat链表的最后位置。
其中flv_muxer在libavformat\flvenc.c中定义如下：
AVOutputFormat flv_muxer = {
    "flv",
    "flv format",
    "video/x-flv",
    "flv",
    sizeof(FLVContext),
#ifdef CONFIG_LIBMP3LAME
    CODEC_ID_MP3,
#else // CONFIG_LIBMP3LAME
    CODEC_ID_NONE,
    CODEC_ID_FLV1,
    flv_write_header,
    flv_write_packet,
    flv_write_trailer,
    .codec_tag= (const AVCodecTag*[]){flv_video_codec_ids, flv_audio_codec_ids, 0},
}
AVOutputFormat结构的定义如下：
typedef struct AVOutputFormat {
    const char *name;
    const char *long_name;
    const char *mime_type;
    const char *extensions; /**< comma separated filename extensions */
    /** size of private data so that it can be allocated in the wrapper */
    int priv_data_size;
    /* output support */
    enum CodecID audio_codec; /**< default audio codec */
    enum CodecID video_codec; /**< default video codec */
    int (*write_header)(struct AVFormatContext *);
    int (*write_packet)(struct AVFormatContext *, AVPacket *pkt);
    int (*write_trailer)(struct AVFormatContext *);
    /** can use flags: AVFMT_NOFILE, AVFMT_NEEDNUMBER, AVFMT_GLOBALHEADER */
    int flags;
    /** currently only used to set pixel format if not YUV420P */
    int (*set_parameters)(struct AVFormatContext *, AVFormatParameters *);
    int (*interleave_packet)(struct AVFormatContext *, AVPacket *out, AVPacket *in, int flush);
    /**
     *list of supported codec_id-codec_tag pairs,ordered by "better choice first"
     * the arrays are all CODEC_ID_NONE terminated
     */
    const struct AVCodecTag **codec_tag;
    /* private fields */
    struct AVOutputFormat *next;
} AVOutputFormat;
由AVOutputFormat结构的定义可知，flv_muxer变量初始化的第一、二个成员分别为该muxer
的名称与长名称，第三、第四个成员为所对应MIMIE Type和后缀名，第五个成员是所对应的
私有结构的大小，第六、第七个成员为所对应的音频编码和视频编码类型ID，接下来就是三
个重要的接口函数，该 muxer的功能也就是通过调用这三个接口实现的。
flv_demuxer在libavformat\flvdec.c中定义如下, 与flv_muxer类似，在这儿主要也是设置
了5个接口函数，其中flv_probe接口用途是测试传入的数据段是否是符合当前文件格式，这
个接口在匹配当前demuxer时会用到。
AVInputFormat flv_demuxer = {
    "flv",
    "flv format",
    0,
    flv_probe,
    flv_read_header,
    flv_read_packet,
    flv_read_close,
    flv_read_seek,
    .extensions = "flv",
    .value = CODEC_ID_FLV1,
};
在上述av_register_all(void)函数中通过执行libavcodec\allcodecs.c文件里的
avcodec_register_all(void)函数来初始化全部的encoder/decoder。
因为不是每种编码方式都支持encode和decode，所以有以下三种注册方式：
#define REGISTER_ENCODER(X,x) \
   if(ENABLE_##X##_ENCODER) register_avcodec(&x##_encoder)
#define REGISTER_DECODER(X,x) \
   if(ENABLE_##X##_DECODER) register_avcodec(&x##_decoder)
#define REGISTER_ENCDEC(X,x) REGISTER_ENCODER(X,x); REGISTER_DECODER(X,x)
如支持flv的flv_encoder和flv_decoder变量就分别是在libavcodec\mpegvideo.c和
libavcodec\h263de.c中创建的。
3. 当前muxer/demuxer的匹配
在FFmpeg的文件转换过程中，首**先要做的就是根据传入文件和传出文件的后缀名[FIXME]匹配合适的demuxer和muxer。匹配上的demuxer和muxer都保存在如下所示**，定义在ffmpeg.c里的
全局变量file_iformat和file_oformat中：
   static AVInputFormat *file_iformat;
   static AVOutputFormat *file_oformat;
3.1 demuxer匹配
**在libavformat\utils.c中的static AVInputFormat *av_probe_input_format2(AVProbeData *pd, int is_opened, int *score_max)函数用途是根据传入的probe data数据，依次调用每个demuxer的read_probe接口，来进行该demuxer是否和传入的文件内容匹配的判断。其调用顺序如下：void parse_options(int argc, char **argv, const OptionDef *options,void (* parse_arg_function)(const char *));   static void opt_input_file(const char *filename)       int av_open_input_file(…… )           AVInputFormat *av_probe_input_format(AVProbeData *pd,int is_opened)               static AVInputFormat *av_probe_input_format2(……)**
opt_input_file函数是在保存在const OptionDef options[]数组中，用于
void parse_options(int argc, char **argv, const OptionDef *options)中解析argv里的
“-i” 参数，也就是输入文件名时调用的。
3.2 muxer匹配
**与demuxer的匹配不同，muxer的匹配是调用guess_format函数**，根据main() 函数的argv里的
输出文件后缀名来进行的。
void parse_options(int argc, char **argv, const OptionDef *options, 
           void (* parse_arg_function)(const char *));
   void parse_arg_file(const char *filename)
       static void opt_output_file(const char *filename)
           AVOutputFormat *guess_format(const char *short_name, 
                           const char *filename,
                           const char *mime_type)
3.3 当前encoder/decoder的匹配
在main()函数中除了解析传入参数并初始化demuxer与muxer的parse_options( )函数以外，
其他的功能都是在av_encode( )函数里完成的。
在libavcodec\utils.c中有如下二个函数:
   AVCodec *avcodec_find_encoder(enum CodecID id)
   AVCodec *avcodec_find_decoder(enum CodecID id)
他们的功能就是根据传入的CodecID，找到匹配的encoder和decoder。
在av_encode( )函数的开头，首先初始化各个AVInputStream和AVOutputStream，然后分别调
用上述二个函数，并将匹配上的encoder与decoder分别保存在:
AVInputStream->AVStream *st->AVCodecContext *codec->struct AVCodec *codec与
AVOutputStream->AVStream *st->AVCodecContext *codec->struct AVCodec *codec变量。
4. 其他主要数据结构
4.1 AVFormatContext
AVFormatContext是FFMpeg格式转换过程中实现输入和输出功能、保存相关数据的主要结构。
每一个输入和输出文件，都在如下定义的指针数组全局变量中有对应的实体。
   static AVFormatContext *output_files[MAX_FILES];
   static AVFormatContext *input_files[MAX_FILES];
对于输入和输出，因为共用的是同一个结构体，所以需要分别对该结构中如下定义的iformat
或oformat成员赋值。
   struct AVInputFormat *iformat;
   struct AVOutputFormat *oformat;
对一个AVFormatContext来说，这二个成员不能同时有值，即一个AVFormatContext不能同时
含有demuxer和muxer。在main( )函数开头的parse_options( )函数中找到了匹配的muxer和
demuxer之后，根据传入的argv参数，初始化每个输入和输出的AVFormatContext结构，并保
存在相应的output_files和input_files指针数组中。在av_encode( )函数中，output_files
和input_files是作为函数参数传入后，在其他地方就没有用到了。
4.2 AVCodecContext
保存AVCodec指针和与codec相关数据，如video的width、height，audio的sample rate等。
AVCodecContext中的codec_type，codec_id二个变量对于encoder/decoder的匹配来说，最为
重要。
   enum CodecType codec_type;    /* see CODEC_TYPE_xxx */
   enum CodecID codec_id;        /* see CODEC_ID_xxx */
如上所示，codec_type保存的是CODEC_TYPE_VIDEO，CODEC_TYPE_AUDIO等媒体类型，
codec_id保存的是CODEC_ID_FLV1，CODEC_ID_VP6F等编码方式。
**以支持flv格式为例，在前述的av_open_input_file(…… ) 函数中，匹配到正确的AVInputFormat demuxer后，通过av_open_input_stream( )函数中调用AVInputFormat的read_header接口来执行flvdec.c中的flv_read_header( )函数。在flv_read_header( )函数内，根据文件头中的数据，创建相应的视频或音频AVStream，并设置AVStream中AVCodecContext的正确的codec_type值。codec_id值是在解码过程中flv_read_packet( )函数执行时根据每一个packet头中的数据来设置的。**
4.3 AVStream
AVStream结构保存与数据流相关的编解码器，数据段等信息。比较重要的有如下二个成员：
   AVCodecContext *codec; /**< codec context */
   void *priv_data;
其中codec指针保存的就是上节所述的encoder或decoder结构。priv_data指针保存的是和具
体编解码流相关的数据，如下代码所示，在ASF的解码过程中，priv_data保存的就是
ASFStream结构的数据。
   AVStream *st;
   ASFStream *asf_st; 
   … …
   st->priv_data = asf_st;
4.4 AVInputStream/ AVOutputStream
根据输入和输出流的不同，前述的AVStream结构都是封装在AVInputStream和AVOutputStream
结构中，在av_encode( )函数中使用。AVInputStream中还保存的有与时间有关的信息。
AVOutputStream中还保存有与音视频同步等相关的信息。
4.5 AVPacket
AVPacket结构定义如下，其是用于保存读取的packet数据。
typedef struct AVPacket {
    int64_t pts;            ///< presentation time stamp in time_base units
    int64_t dts;            ///< decompression time stamp in time_base units
    uint8_t *data;
    int   size;
    int   stream_index;
    int   flags;
    int   duration;        ///< presentation duration in time_base units (0 if not available)
    void (*destruct)(struct AVPacket *);
    void *priv;
    int64_t pos;           ///< byte position in stream, -1 if unknown
} AVPacket;
在av_encode()函数中，调用AVInputFormat的
(*read_packet)(struct AVFormatContext *, AVPacket *pkt)接口，读取输入文件的一帧数
据保存在当前输入AVFormatContext的AVPacket成员中。
5. av_encode函数主要流程
av_encode()函数是FFMpeg中最重要的函数，编解码和输出等大部分功能都在此函数完成，
因此有必要详细描述一下这个函数的主要流程。
1).input streams initializing
2).output streams initializing
3).encoders and decoders initializing
4).set meta data information from input file if required.
5).write output files header
6).loop of handling each frame
   a.read frame from input file:
   b.decode frame data
   c.encode new frame data
   d.write new frame to output file
7).write output files trailer
8).close each encoder and decoder
