# FFmpeg 基本用法--此文转载，作为笔记 - DoubleLi - 博客园






v  容器([Container](http://lib.csdn.net/base/docker))

v  容器就是一种文件(封装)格式，比如flv、mkv、ts、mp4、rmvb、avi等。包含下面5种流以及文件头信息。

v  流(Stream)

v  是一种视频数据信息的传输方式，5种流：音频，视频，字幕，附件，数据。

v  帧(Frame)  代表一幅静止的图像，分为I帧，P帧，B帧。

v  编解码器(Codec)

v  是对视频进行压缩或者解压缩，CODEC =CODE（编码） +DECODE（解码）

v  复用/解复用(mux/demux)

v  把不同的流按照某种容器的规则放入容器，这种行为叫做复用(mux)

把不同的流从某种容器中解析出来，这种行为叫做解复用(demux)



v  码率和帧率是视频文件的最重要的基本特征，对于他们的特有设置会决定视频质量。如果我们知道码率和时长那么可以很容易计算出输出文件的大小。

v  帧率：帧率也叫帧频率，帧率是视频文件中每一秒的帧数，肉眼想看到连续移动图像至少需要15帧。

v  码率：比特率(也叫码率，数据率)是一个确定整体视频/音频质量的参数，秒为单位处理的字节数，码率和视频质量成正比，在视频文件中中比特率用bps来表达。





转码流程





v  FFmpeg的名称来自MPEG视频编码标准，前面的“FF”代表“Fast Forward”，FFmpeg是一套可以用来音视频采集、音视频格式转换，编码解码，视频截图，加水印等的开源计算机程序。可以轻易地实现多种视频格式之间的相互转换。

v  FFmpeg的用户有Google，Facebook，Youtube，VLC,优酷，爱奇艺，土豆，Mplayer，射手播放器，暴风影音，KMPlayer，QQ影音，格式工厂，狸窝视频转换器，暴风转码等。

v  FFmpeg的开发是基于[Linux](http://lib.csdn.net/base/linux)[操作系统](http://lib.csdn.net/base/operatingsystem)，但是可以在大多数操作系统中编译和使用。(在vs2010中编译不了，因为vs2010支持的是C89(不支持C99) ，ffmpeg使用的是C99，vs2013/2015可以编译)



v  FFmpeg一共包含8个库：

v  1、avcodec：编解码（最重要的库）。

v  2、avformat：封装格式处理。

v  3、avfilter：滤镜特效处理。

v  4、avdevice：各种设备的输入输出。

v  5、avutil：工具库（大部分库都需要这个库的支持）。

v  6、postproc：后加工。

v  7、swresample：音频采样数据格式转换。

v  8、swscale：视频像素数据格式转换



v  FFmpeg一共包含四个主要程序：

v  1、ffmpeg：是一个命令行工具，用来对视音频文件转换格式，也支持对电视卡实时编码；

     ffmpeg -i input.flv -c:v libx264 -c:a libfaac -b:v 800k -b:a 100k -r 25                          -ar 48000 -s 1280x720 -f flv out.flv

v  2、ffsever：是一个HTTP多媒体实时广播流服务器，支持时光平移

v  3、ffplay：是一个简单的播放器，使用ffmpeg 库解析和解码，通过SDL显示；

      ffplay input.avi

      该命令将播放当前文件夹下

    的input.avi文件

v  4、ffprobe：探测分析视音频文件

ffprobe -i input -print_format json -show_format -show_streams -show_frames



v  官网：

v  [http://ffmpeg.org/](http://ffmpeg.org/)

v  [http://ffmpeg.org/download.html](http://ffmpeg.org/download.html)

v  https://ffmpeg.zeranoe.com/builds/



v  [linux](http://lib.csdn.net/base/linux)下编译很简单 ./configure && make && make install

v  Windows下使用MinGW的gcc toolchain进行编译，没有pdb，无法进行调试

v  Windows下可以使用vs2013/2015版本编译

v  configure的时候可以指定配置，开启或关闭一些选项，启用外部的编解码库等

    ./configure --enable-shared --enable-version3 --enable-gpl --enable-nonfree --enable-libfdk_aac --enable-libmp3lame --enable-zlib --enable-libspeex --enable-libx264 --prefix=/usr/lib/buildwin64ffmpeg

    学习研究ffmpeg推荐在linux下，编译调试都方便(试了vs2015也不错，有个老外有个开源工程都集成好了)。











v  可用的bit流 ：ffmpeg –bsfs

v  可用的编解码器：ffmpeg –codecs

v  可用的解码器：ffmpeg –decoders

v  可用的编码器：ffmpeg –encoders

v  可用的过滤器：ffmpeg –filters

v  可用的视频格式：ffmpeg –formats

v  可用的声道布局：ffmpeg –layouts

v  可用的license：ffmpeg –L

v  可用的像素格式：ffmpeg –pix_fmts

v  可用的协议：ffmpeg -protocals



v  改变视频的分辨率   -s 1280x720

v  改变视频的帧率       -r 15

v  改变音频的采样率   -ar  44100

v  改变视音频的码率   -b:v 1000k -b:a 80k

v  设置输出格式          -f flv(mpegts/hls/mp4)

v  设置视音频编码格式  -c:v libx264 –c:a libfaac

v  指定视音频不转码  -c copy -c:a copy –c:v copy

v  设置处理开始时间 -ss HH:MM:SS

v  设置处理结束时间 -to HH:MM:SS

v  不要视频或者音频 –vn –an

v  设置关键帧间隔

    -force_key_frames "expr:gte(t,n_forced*1)"



ffmpeg截取视频

v  ffmpeg -i %s -ss %02d:%02d:%02d -to %02d:%02d:%02d  -c copy -f %s %s -y

v  示例：

ffmpeg –i input.flv –ss 00:10:00 –to 00:20:00 –c copy –f flv jiequ.flv –y

v  说明：

v  -i 指定输入

v  -ss 指定截取的开始时间

v  -to指定截取的结束时间

v  可以用两种方式指定时间，一种是用时分秒指定、中间用冒号分离HH:MM:SS，另一种是直接用秒数指定

v  –c copy即音视频编码采用直接复制的方式，不进行转码

v  -f flv 指定输出格式为flv，这里的flv可以替换为mp4，ts，hls等，后面接着输出文件名

v  -y即如果文件存在，直接覆盖写





ffmpeg合并视频



v  把文件名都输出到一个文件，如下放到join.txt，格式是file+空格+文件名+换行+……

v  ffmpeg -f concat -i %s -c copy -f %s %s –y

v  示例：

v  ffmpeg -f concat -i join.txt -c copy -f flv join.flv –y

v  注：只适用于相同编解码参数音视频的合并(不同的得转码可以用filter合并)



ffmpeg转封装



v  ffmpeg -i %s -c copy -f %s %s -y

v  示例：ffmpeg -i input.flv -c copy -f mp4 output.mp4 -y

    这个比较简单,直接指定不转码，输出格式和文件名就好了。

v  其中转hls有一些额外的参数可供选择

v  -hls_time 指定切片时长，默认值为2

v  -hls_list_size 指定m3u8中ts切片数量的最大值，点播可以配置一个很大的数(最大可配值INT_MAX)，即都保存，直播可以酌情配置

v  -hls_segment_filename 指定ts切片的名字

v  示例：

v  ffmpeg -i input.flv -c copy -hls_time 10 -hls_list_size 10000000 -hls_segment_filename hongyun_%d.ts -f hls hongyun.m3u8





ffmpeg转码

v  ffmpeg -i %s -c:v %s -c:a %s -ar %s -b:v %s -b:a %s -f %s %s -y

v  示例：

v  ffmpeg -i input.flv -c:v h264 -c:a libmp3lame -ar 44100 -b:v 1000k -b:a 80k -f flv output.flv –y

v  说明：

v  -c:v指定视频编码格式

v  -c:a指定音频编码格式

v  -ar 指定音频采样率

v  -b:v 指定视频码率

v  -b:a 指定音频码率

v  注：转换过程中可能会遇到一些问题，ffmpeg会给出提示信息，我们可以根据提示信息进行进一步转码

v 

v  这种情况我们加上 -bsf:v h264_mp4toannexb 应该就可以了



ffmpeg 混屏（画中画）（多屏播放）



v  ffmpeg混屏主要用到的filter中视频相关的有scale、pad、overlay；音频相关的是amix

v  ffmpeg -i rtmp://10.111.22.210/live/livestream6  -i xx.flv -i rtmp://10.111.22.210/live/livestream5 -i rtmp://10.121.22.210/live/livestream7 -filter_complex "[0:v]scale=300:200,pad=600:400:0:0[left];[1:v]scale=300:200[right];[left][right]overlay=300:0[up];[2:v]scale=300:200[down]; [up][down]overlay=0:200[downleft];[3:v]scale=300:200[downright];[downleft][downright]overlay=300:200;amix=inputs=4" -c:v h264 -c:a libfaac -f flv rtmp://192.168.11.168/live/livestream



v  命令中首先指定了4路输入；然后按一下步骤进行操作：

v  1.指定第一路流的视频([0:v])作为输入，进行比例变换(scale=300:200)、并填充视频(pad=600:400:0:0),输出为[left]；

v  2.指定第二路流的视频([1:v])为输入进行比例变换(scale=300:200)，输出为[right];

v  3.把[right]叠加到[left]上([left][right]overlay)，并指定位置，坐标为(300:0）,输出为[up]；

v  4.指定第三路流的视频([2:v]) 为输入进行比例变换(scale=300:200),输出为[down]；

v  5. 把[down] 叠加到[up] 上([up][down]overlay)并指定位置，坐标为(0:200),输出为 [downleft]；

v  6.指定第四路流的视频([3:v])为输入进行比例变换(scale=300:200)，输出为[downright];

v  7. 把[downright]叠加到[downleft]上([downleft][downright]overlay) 并指定位置,坐标为(300:200),这里我们不指定输出了(当然也可以指定)，因为滤镜链如果没有指定输入或输出，则默认使用前面的滤镜链的输出为输入，并输出给后面的滤镜链做输入。

v  8. amix=inputs=4，对音频进行混流，这里我们指定混4路音频

v  两路视频分屏的效果

![](http://img.blog.csdn.net/20170308161237531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHVsZWl5aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

ffmpeg中很多框架都是基于函数指针，有点类似于C++的基类，有几个重要的结构体，下面介绍下



几个重要的结构体



v  AVFormatContext 封装格式上下文结构体，也是统领全局的结构体，保存了视频文件封装格式相关信息。

v  AVInputFormat 每种作为输入的封装格式（例如FLV, MKV, MP4, AVI）对应一个该结构体。

v  AVOutputFormat  每种作为输出的封装格式（例如FLV, MKV, MP4, AVI）对应一个该结构体。

v  AVStream  视频文件中每个视频（音频）流对应一个该结构体。

v  AVCodecContext 编码器上下文结构体，保存了视频（音频）编解码相关信息。

v  AVCodec 每种视频（音频）编解码器(例如H.264解码器)对应一个该结构体。

v  AVPacket  存储一帧压缩编码数据。

v  AVFrame   存储一帧解码后像素（采样）数据。

v  AVIOContext    管理输入输出数据的结构体





AVFormatContext



v  AVFormatContext中几个重要的成员

v  struct AVInputFormat *iformat; 输入的封装格式(Demuxing only)

v  struct AVOutputFormat *oformat;输入的封装格式(Muxing only)

v  unsigned int nb_streams; 输入/出视频的AVStream 个数

v  AVStream **streams; 输入/出视频的AVStream []数组

v  duration ：输入视频的时长（以微秒为单位）（ Demuxing only ）

v  bit_rate ：输入视频的码率。





AVInputFormat



v  AVInputFormat中几个重要的成员

v   const char *name; 封装格式名称

v  const char *long_name ；封装格式的长名称

v  const char *extensions; 封装格式的扩展名

v  一些封装格式处理的接口函数。

v  int (*read_probe)(AVProbeData *);

v  int (*read_header)(struct AVFormatContext *);

v  int (*read_packet)(struct AVFormatContext *, AVPacket *pkt);

v  int (*read_close)(struct AVFormatContext *);

v  int (*read_seek)(struct AVFormatContext *, int stream_index, int64_t timestamp, int flags);





AVInputFormat例子



v  AVInputFormat ff_flv_demuxer = {

    .name           = "flv",

    .long_name      = NULL_IF_CONFIG_SMALL("FLV (Flash Video)"),

    .priv_data_size = sizeof(FLVContext),

    .read_probe     = flv_probe,

    .read_header    = flv_read_header,

    .read_packet    = flv_read_packet,

    .read_seek      = flv_read_seek,

    .read_close     = flv_read_close,

    .extensions     = "flv",

    .priv_class     = &flv_class,

};

AVOutputFormat



v  AVOutputFormat中几个重要的成员

v   const char *name; 封装格式名称

v  const char *long_name ；封装格式的长名称

v  const char *extensions; 封装格式的扩展名

v  一些封装格式处理的接口函数。

v  int (*write_header)(struct AVFormatContext *);

v  int (*write_packet)(struct AVFormatContext *, AVPacket *pkt);

v  int (*write_trailer)(struct AVFormatContext *);





AVOutputFormat例子



v  AVOutputFormat ff_flv_muxer = {

    .name           = "flv",

    .long_name      = NULL_IF_CONFIG_SMALL("FLV (Flash Video)"),

    .mime_type      = "video/x-flv",

    .extensions     = "flv",

    .priv_data_size = sizeof(FLVContext),

    .audio_codec    = CONFIG_LIBMP3LAME ? AV_CODEC_ID_MP3 : AV_CODEC_ID_ADPCM_SWF,

    .video_codec    = AV_CODEC_ID_FLV1,

    .write_header   = flv_write_header,

    .write_packet   = flv_write_packet,

    .write_trailer  = flv_write_trailer,

    .codec_tag      = (const AVCodecTag* const []) {                         flv_video_codec_ids, flv_audio_codec_ids, 0},

    .flags          = AVFMT_GLOBALHEADER | AVFMT_VARIABLE_FPS |

                      AVFMT_TS_NONSTRICT,

    .priv_class     = &flv_muxer_class,

};





AVStream



v  AVStream中几个重要的成员

v  int index;    /**< stream index in AVFormatContext */

v  int id;

v  AVCodecContext *codec; 该流对应的AVCodecContext

v   AVRational time_base; 该流的时基

v  AVRational r_frame_rate; 该流的帧率





AVCodecContext



v  AVCodecContext中几个重要的成员

v  const struct AVCodec  *codec;  编解码器的AVCodec

v  int width, height;  图像的宽高（只针对视频）

v  enum AVPixelFormat pix_fmt; 像素格式（只针对视频）

v  int sample_rate;   采样率（只针对音频）

v  int channels;   声道数（只针对音频）

v  enum AVSampleFormat sample_fmt; 采样格式（只针对音频）



AVCodec



v  AVCodec中几个重要的成员

v  const char *name;  编解码器名称

v  const char *long_name;  编解码器长名称

v  enum AVMediaType type;  编解码器类型

v   enum AVCodecID id;  编解码器ID

v   一些编解码的接口函数

v   int (*init)(AVCodecContext *);

v   int (*encode2)(AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame, int *got_packet_ptr);

v   int (*decode)(AVCodecContext *, void *outdata, int *outdata_size, AVPacket *avpkt);

v   int (*close)(AVCodecContext *);





AVCodec例子



v  AVCodec ff_h264_decoder = {

    .name                  = "h264",

    .long_name             = NULL_IF_CONFIG_SMALL("H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),

    .type                  = AVMEDIA_TYPE_VIDEO,

    .id                    = AV_CODEC_ID_H264,

    .priv_data_size        = sizeof(H264Context),

    .init                  = ff_h264_decode_init,

    .close                 = h264_decode_end,

    .decode                = h264_decode_frame,

    .profiles              = NULL_IF_CONFIG_SMALL(ff_h264_profiles),

    .priv_class            = &h264_class,

};





v  AVCodec ff_libx264_encoder = {

    .name             = "libx264",

    .long_name        = NULL_IF_CONFIG_SMALL("libx264 H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),

    .type             = AVMEDIA_TYPE_VIDEO,

    .id               = AV_CODEC_ID_H264,

    .priv_data_size   = sizeof(X264Context),

    .init             = X264_init,

    .encode2          = X264_frame,

    .close            = X264_close,

    .capabilities     = AV_CODEC_CAP_DELAY | AV_CODEC_CAP_AUTO_THREADS,

    .priv_class       = &x264_class,

    .defaults         = x264_defaults,

    .init_static_data = X264_init_static,

    .caps_internal    = FF_CODEC_CAP_INIT_THREADSAFE |

                        FF_CODEC_CAP_INIT_CLEANUP,

};





AVPacket



v  AVPacket中几个重要的成员

v  int64_t pts;  显示时间戳

v  int64_t dts;  解码时间戳

v  uint8_t *data;  压缩编码数据

v  int   size;  压缩编码数据大小

v  int   stream_index; 所属的AVStream

v  int   flags; 1关键帧





AVFrame



v  AVFrame中几个重要的成员

v   uint8_t *data[AV_NUM_DATA_POINTERS]; 解码后的图像像素数据（音频采样数据）。

v  int linesize[AV_NUM_DATA_POINTERS]; 对视频来说是图像中一行像素的大小；对音频来说是整个音

v  频帧的大小。

v  int width, height; 图像的宽高（只针对视频）。

v  int key_frame; 是否为关键帧（只针对视频） 。

v  enum AVPictureType pict_type; 帧类型（只针对视频） 。例如I，P，B。



AVIOContext



v  AVIOContext中几个重要的成员

v      unsigned char *buffer;   /**< Start of the buffer. */

v      int buffer_size;        /**< Maximum buffer size */

v      unsigned char *buf_ptr; /**< Current position in the buffer */

v      unsigned char *buf_end; /**< End of the data, may be less than buffer+buffer_size if the read function returned less data than requested, e.g. for streams where no more data has been received yet. */

v      void *opaque;           /**< A private pointer, passed to the read/write/seek/...  functions. */

v      int (*read_packet)(void *opaque, uint8_t *buf, int buf_size);

v      int (*write_packet)(void *opaque, uint8_t *buf, int buf_size);

v      int64_t (*seek)(void *opaque, int64_t offset, int whence);

v      int64_t pos;            /**< position in the file of the current buffer */



v  AVIOContext中几个重要的成员

v  在解码的情况下，buffer用于存储ffmpeg读入的数据。例如打开一个视频文件的时候，先把数据从硬盘读入buffer，然后在送给解码器用于解码。

v  其中opaque指向了URLContext。

  typedef struct URLContext {

      const AVClass *av_class;    /**< information for av_log(). Set by url_open(). */

      const struct URLProtocol *prot;

      void *priv_data;

      char *filename;             /**< specified URL */

      int flags;

      ……   

  } URLContext;





URLProtocol



  typedef struct URLProtocol {

      const char *name;

      int     (*url_open)( URLContext *h, const char *url, int flags);

      int     (*url_open2)(URLContext *h, const char *url, int flags, AVDictionary **options);

      int     (*url_accept)(URLContext *s, URLContext **c);

      int     (*url_handshake)(URLContext *c);

      int     (*url_read)( URLContext *h, unsigned char *buf, int size);

      int     (*url_write)(URLContext *h, const unsigned char *buf, int size);

     int64_t (*url_seek)( URLContext *h, int64_t pos, int whence);

      int     (*url_close)(URLContext *h);

      int (*url_read_pause)(URLContext *h, int pause);

      int64_t (*url_read_seek)(URLContext *h, int stream_index,

                               int64_t timestamp, int flags);

      int (*url_get_file_handle)(URLContext *h);

      int (*url_get_multi_file_handle)(URLContext *h, int **handles,

                                       int *numhandles);

      int (*url_shutdown)(URLContext *h, int flags);

      …..

  } URLProtocol;





URLProtocol例子

  const URLProtocol ff_file_protocol = {

     .name                = "file",

      .url_open            = file_open,

      .url_read            = file_read,

      .url_write           = file_write,

      .url_seek            = file_seek,

     .url_close           = file_close,

      .url_get_file_handle = file_get_handle,

      .url_check           = file_check,

      .url_delete          = file_delete,

      .url_move            = file_move,

      .priv_data_size      = sizeof(FileContext),

      .priv_data_class     = &file_class,

      .url_open_dir        = file_open_dir,

      .url_read_dir        = file_read_dir,

      .url_close_dir       = file_close_dir,

      .default_whitelist   = "file,crypto"

  };



const URLProtocol ff_tcp_protocol = {

    .name                = "tcp",

    .url_open            = tcp_open,

    .url_accept          = tcp_accept,

    .url_read            = tcp_read,

    .url_write           = tcp_write,

    .url_close           = tcp_close,

    .url_get_file_handle = tcp_get_file_handle,

    .url_shutdown        = tcp_shutdown,

    .priv_data_size      = sizeof(TCPContext),

    .flags               = URL_PROTOCOL_FLAG_NETWORK,

    .priv_data_class     = &tcp_class,

};  

v  比较不错的资源

官网：[http://ffmpeg.org/](http://ffmpeg.org/)

官网文档：[http://ffmpeg.org/documentation.html](http://ffmpeg.org/documentation.html)

官网wiki : https://trac.ffmpeg.org/wiki

雷霄骅(leixiaohua1020)的专栏(一个广院工科生的视音频技术笔记)：[http://blog.csdn.net/leixiaohua1020](http://blog.csdn.net/leixiaohua1020)

其他资源。。。

最后，缅怀雷博，感谢雷博！！！









