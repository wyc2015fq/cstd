# FFMpeg框架代码阅读 - u013366022的专栏 - CSDN博客
2015年04月14日 22:37:27[slitaz](https://me.csdn.net/u013366022)阅读数：300
**简介**
FFmpeg是一个集录制、转换、音/视频编码解码功能为一体的完整的开源解决方案。FFmpeg的开发是基于Linux操作系统，但是可以在大多数操作系统中编译和使用。FFmpeg支持MPEG、DivX、MPEG4、AC3、DV、FLV等40多种编码，AVI、MPEG、OGG、Matroska、ASF等90多种解码.
TCPMP, VLC, MPlayer等开源播放器都用到了FFmpeg。
FFmpeg主目录下主要有libavcodec、libavformat和libavutil等子目录。其中libavcodec用于存放各个encode/decode模块，libavformat用于存放muxer/demuxer模块，libavutil用于存放内存操作等常用模块。
以flash movie的flv文件格式为例， muxer/demuxer的flvenc.c和flvdec.c文件在libavformat目录下，encode/decode的mpegvideo.c和h263de.c在libavcodec目录下。
**muxer/demuxer与encoder/decoder定义与初始化**
muxer/demuxer和encoder/decoder在FFmpeg中的实现代码里，有许多相同的地方，而二者最大的差别是muxer和demuxer分别是不同的结构AVOutputFormat与AVInputFormat，而encoder和decoder都是用的AVCodec结构。
muxer/demuxer和encoder/decoder在FFmpeg中相同的地方有：
l二者都是在main()开始的av_register_all()函数内初始化的。
l二者都是以链表的形式保存在全局变量中的。
muxer/demuxer是分别保存在全局变量AVOutputFormat *first_oformat与AVInputFormat *first_iformat中的。
encoder/decoder都是保存在全局变量AVCodec *first_avcodec中的。
l二者都用函数指针的方式作为开放的公共接口。
demuxer开放的接口有：
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- int (*read_probe)(AVProbeData *);  
- int(*read_header)(struct AVFormatContext *, AVFormatParameters *ap);  
- int (*read_packet)(struct AVFormatContext *, AVPacket *pkt);  
- int (*read_close)(struct AVFormatContext *);  
- int (*read_seek)(struct AVFormatContext *, int stream_index, int64_t timestamp, int flags);  
muxer开放的接口有：
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- int (*write_header)(struct AVFormatContext *);  
- int (*write_packet)(struct AVFormatContext *, AVPacket *pkt);  
- int (*write_trailer)(struct AVFormatContext *);  
encoder/decoder的接口都是一样的，只不过二者分别只实现encoder和decoder函数：int (*init)(AVCodecContext *);
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- int (*encode)(AVCodecContext *, uint8_t *buf, int buf_size, void *data);  
- int (*close)(AVCodecContext *);  
- int (*decode)(AVCodecContext *, void *outdata, int *outdata_size, uint8_t *buf, int buf_size);  
仍以flv文件为例来说明muxer/demuxer的初始化。
在libavformat/allformats.c文件的**av_register_all(void)**函数中，通过执行
**REGISTER_MUXDEMUX(FLV, flv);**
将支持flv 格式的**flv_muxer**与**flv_demuxer**变量分别注册到全局变量**first_oformat**与**first_iformat**链表的最后位置。
其中**flv_muxer**在libavformat/flvenc.c中定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- AVOutputFormat flv_muxer = {  
- "flv",  
- "flv format",  
- "video/x-flv",  
- "flv",  
- sizeof(FLVContext),  
- #ifdef CONFIG_LIBMP3LAME
-     CODEC_ID_MP3,  
- #else // CONFIG_LIBMP3LAME
-     CODEC_ID_NONE,  
-     CODEC_ID_FLV1,  
-     flv_write_header,  
-     flv_write_packet,  
-     flv_write_trailer,  
-     .codec_tag= (const AVCodecTag*[]){flv_video_codec_ids, flv_audio_codec_ids, 0},  
- }  
**AVOutputFormat**结构的定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- typedefstruct AVOutputFormat {  
- constchar *name;  
- constchar *long_name;  
- constchar *mime_type;  
- constchar *extensions; /**< comma separated filename extensions */
- /** size of private data so that it can be allocated in the wrapper */
- int priv_data_size;  
- /* output support */
- enum CodecID audio_codec; /**< default audio codec */
- enum CodecID video_codec; /**< default video codec */
- int (*write_header)(struct AVFormatContext *);  
- int (*write_packet)(struct AVFormatContext *, AVPacket *pkt);  
- int (*write_trailer)(struct AVFormatContext *);  
- /** can use flags: AVFMT_NOFILE, AVFMT_NEEDNUMBER, AVFMT_GLOBALHEADER */
- int flags;  
- /** currently only used to set pixel format if not YUV420P */
- int (*set_parameters)(struct AVFormatContext *, AVFormatParameters *);  
- int (*interleave_packet)(struct AVFormatContext *, AVPacket *out, AVPacket *in, int flush);  
- 
- /**
-      * list of supported codec_id-codec_tag pairs, ordered by "better choice first"
-      * the arrays are all CODEC_ID_NONE terminated
-      */
- conststruct AVCodecTag **codec_tag;  
- /* private fields */
- struct AVOutputFormat *next;  
- } AVOutputFormat;  
由**AVOutputFormat**结构的定义可知，**flv_muxer**变量初始化的第一、第二个成员分别为该muxer的名称与长名称，第三、第四个成员为所对应MIMIE Type和后缀名，第五个成员是所对应的私有结构的大小，第六、第七个成员为所对应的音频编码和视频编码类型ID，接下来就是三个重要的接口函数，该muxer的功能也就是通过调用这三个接口实现的。
**flv_demuxer**在libavformat/flvdec.c中定义如下, 与**flv_muxer**类似，在这儿主要也是设置了5个接口函数，其中**flv_probe**接口用途是测试传入的数据段是否是符合当前文件格式，这个接口在匹配当前demuxer的时候会用到。
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- AVInputFormat flv_demuxer = {  
- "flv",  
- "flv format",  
-     0,  
-     flv_probe,  
-     flv_read_header,  
-     flv_read_packet,  
-     flv_read_close,  
-     flv_read_seek,  
-     .extensions = "flv",  
-     .value = CODEC_ID_FLV1,  
- };  
在上述**av_register_all(void)**函数中通过执行libavcodec/allcodecs.c文件里的**avcodec_register_all(void)**函数来初始化全部的encoder/decoder。
因为不是每种编码方式都支持encode和decode，所以有以下三种注册方式：
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- #define REGISTER_ENCODER(X,x) /
- if(ENABLE_##X##_ENCODER) register_avcodec(&x##_encoder)  
- #define REGISTER_DECODER(X,x) /
- if(ENABLE_##X##_DECODER) register_avcodec(&x##_decoder)  
- #define REGISTER_ENCDEC(X,x)    REGISTER_ENCODER(X,x); REGISTER_DECODER(X,x)
如支持flv的**flv_encoder**和**flv_decoder**变量就分别是在libavcodec/mpegvideo.c和libavcodec/h263de.c中创建的。
**当前muxer/demuxer的匹配**
在FFmpeg的文件转换过程中，首先要做的就是根据传入文件和传出文件的后缀名匹配合适的demuxer和muxer。
匹配上的demuxer和muxer都保存在如下所示，定义在ffmpeg.c里的全局变量**file_iformat**和**file_oformat**中：
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- static AVInputFormat *file_iformat;  
- static AVOutputFormat *file_oformat;  
1.demuxer匹配
在libavformat/utils.c中的**static AVInputFormat *av_probe_input_format2(AVProbeData *pd, int is_opened, int *score_max)**函数用途是根据传入的probe data数据，依次调用每个demuxer的read_probe接口，来进行该demuxer是否和传入的文件内容匹配的判断。其调用顺序如下：
**void parse_options(int argc, char **argv, const OptionDef *options)**
**static void opt_input_file(const char *filename)**
**static void opt_input_file(const char *filename)**
**int av_open_input_file(…… )**
**                                     AVInputFormat *av_probe_input_format(AVProbeData *pd, int is_opened)**
**static AVInputFormat *av_probe_input_format2(……)**
**opt_input_file**函数是在保存在**const OptionDef options[]**数组中，用于**void parse_options(int argc, char **argv, const OptionDef *options)**中解析argv里的“-i” 参数，也就是输入文件名时调用的。
2.muxer匹配
与demuxer的匹配不同，muxer的匹配是调用**guess_format**函数，根据**main( )** 函数的argv里的输出文件后缀名来进行的。
**void parse_options(int argc, char **argv, const OptionDef *options)**
**void parse_arg_file(const char *filename)**
**static void opt_output_file(const char *filename)**
**AVOutputFormat *guess_format(const char *short_name, const char *filename,****const char *mime_type)**
**当前encoder/decoder的匹配**
在**main( )**函数中除了解析传入参数并初始化demuxer与muxer的**parse_options( )**函数以外，其他的功能都是在**av_encode( )**函数里完成的。
在libavcodec/utils.c中有如下二个函数。
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- AVCodec *avcodec_find_encoder(enum CodecID id)  
- AVCodec *avcodec_find_decoder(enum CodecID id)  
他们的功能就是根据传入的CodecID，找到匹配的encoder和decoder。
在**av_encode( )**函数的开头，首先初始化各个**AVInputStream**和**AVOutputStream**，然后分别调用上述二个函数，并将匹配上的encoder与decoder分别保存在**AVInputStream->AVStream *st->AVCodecContext *codec->struct AVCodec *codec**与**AVOutputStream->AVStream *st->AVCodecContext *codec->struct AVCodec *codec**变量中。
**其他主要数据结构**
1.AVFormatContext
**AVFormatContext**是FFMpeg格式转换过程中实现输入和输出功能、保存相关数据的主要结构。每一个输入和输出文件，都在如下定义的指针数组全局变量中有对应的实体。
**static AVFormatContext *output_files[MAX_FILES];**
**static AVFormatContext *input_files[MAX_FILES];**
对于输入和输出，因为共用的是同一个结构体，所以需要分别对该结构中如下定义的**iformat**或**oformat**成员赋值。
**struct AVInputFormat *iformat;**
**        struct AVOutputFormat *oformat;**
对一个**AVFormatContext**来说，二个成员不能同时有值，即一个**AVFormatContext**不能同时含有demuxer和muxer。
在**main( )**函数开头的**parse_options( )**函数中找到了匹配的muxer和demuxer之后，根据传入的argv参数，初始化每个输入和输出的**AVFormatContext**结构，并保存在相应的**output_files**和**input_files**指针数组中。
在**av_encode( )**函数中，**output_files**和**input_files**是作为函数参数传入后，在其他地方就没有用到了。
2.AVCodecContext
保存**AVCodec**指针和与codec相关的数据，如video的width、height，audio的sample rate等。**AVCodecContext**中的**codec_type，codec_id**二个变量对于encoder/decoder的匹配来说，最为重要。
**enum CodecType codec_type; /* see CODEC_TYPE_xxx */**
**              enum CodecID codec_id; /* see CODEC_ID_xxx */**
如上所示，**codec_type**保存的是**CODEC_TYPE_VIDEO**，**CODEC_TYPE_AUDIO**等媒体类型，
**codec_id**保存的是**CODEC_ID_FLV1**，**CODEC_ID_VP6F**等编码方式。
以支持flv格式为例，在前述的**av_open_input_file(…… )** 函数中，匹配到正确的**AVInputFormat** demuxer后，通过**av_open_input_stream( )**函数中调用**AVInputFormat**的**read_header**接口来执行flvdec.c中的**flv_read_header( )**函数。在**flv_read_header( )**函数内，根据文件头中的数据，创建相应的视频或音频**AVStream**，并设置**AVStream**中**AVCodecContext**的正确的**codec_type**值。**codec_id**值是在解码过程中**flv_read_packet( )**函数执行时根据每一个packet头中的数据来设置的。
3.AVStream
**AVStream**结构保存与数据流相关的编解码器，数据段等信息。比较重要的有如下二个成员：
**AVCodecContext *codec; /**< codec context */**
**                 void *priv_data;**
其中**codec**指针保存的就是上节所述的encoder或decoder结构。**priv_data**指针保存的是和具体编解码流相关的数据，如下代码所示，在ASF的解码过程中，**priv_data**保存的就是**ASFStream**结构的数据。
**AVStream *st;**
**ASFStream *asf_st;   **
**     … …**
**st->priv_data = asf_st;**
4.AVInputStream/ AVOutputStream
根据输入和输出流的不同，前述的**AVStream**结构都是封装在**AVInputStream**和 **AVOutputStream**结构中，在**av_encode( )**函数中使用。
**AVInputStream**中还保存的有与时间有关的信息。
**AVOutputStream**中还保存有与音视频同步等相关的信息。
5.AVPacket
**AVPacket**结构定义如下，其是用于保存读取的packet数据。
**[cpp]**[view
 plain](http://blog.csdn.net/czc1009/article/details/14643577#)[copy](http://blog.csdn.net/czc1009/article/details/14643577#)
- typedefstruct AVPacket {  
-     int64_t pts;            ///< presentation time stamp in time_base units
-     int64_t dts;            ///< decompression time stamp in time_base units
-     uint8_t *data;  
- int   size;  
- int   stream_index;  
- int   flags;  
- int   duration;        ///< presentation duration in time_base units (0 if not available)
- void (*destruct)(struct AVPacket *);  
- void *priv;  
-     int64_t pos;           ///< byte position in stream, -1 if unknown
- } AVPacket;  
在**av_encode( )**函数中，调用**AVInputFormat**的**(*read_packet)(struct AVFormatContext *, AVPacket *pkt);**接口，读取输入文件的一帧数据保存在当前输入**AVFormatContext**的**AVPacket**成员中。
