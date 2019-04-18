# ffmpeg开发指南 - 深之JohnChen的专栏 - CSDN博客

2012年03月03日 23:38:26[byxdaz](https://me.csdn.net/byxdaz)阅读数：29584


FFmpeg是一个集录制、转换、音/视频编码解码功能为一体的完整的开源解决方案。FFmpeg的开发是基于Linux操作系统，但是可以在大多数操作系统中编译和使用。FFmpeg支持MPEG、DivX、MPEG4、AC3、DV、FLV等40多种编码，AVI、MPEG、OGG、Matroska、ASF等90多种解码.TCPMP, VLC, MPlayer等开源播放器都用到了FFmpeg。

    一、ffmpeg介绍

ffmpeg软件包经编译过后将生成三个可执行文件，ffmpeg，ffserver，ffplay。其中ffmpeg用于对媒体文件进行处理，ffserver是一个http的流媒体服务器，ffplay是一个基于SDL的简单播放器。

ffmpeg中有五个库文件，libavcodec，libavformat，libavutil，libswscale，libpostproc，其中库libavcodec，libavformat用于对媒体文件进行处理，如格式的转换；libavutil是一个通用的小型函数库，该库中实现了CRC校验码的产生，128位整数数学，最大公约数，整数开方，整数取对数，内存分配，大端小端格式的转换等功能；libswscale，libpostproc暂时不知道何用。

   ffmpeg下载

subversion： [http://subversion.tigris.org/downloads/subversion-1.3.2.tar.gz](http://subversion.tigris.org/downloads/subversion-1.3.2.tar.gz)

tar zvxf subversion-1.3.2.tar.gz

cd subversion-1.3.2

./configure --with-apr=/usr/local/apr-httpd--with-apr-util=/usr/local/apr-util-httpd/

make 

make install

如果安装了FC6，它已经带了svn，不用装了。

ffmpeg的下载：我们就可以通过svn命令获取最新的ffmpeg，命令如下：

svn checkout svn://svn.mplayerhq.hu/ffmpeg/trunk ffmpeg

windows下编译ffmpeg源代码 

[http://blog.csdn.net/jszj/article/details/4028716](http://blog.csdn.net/jszj/article/details/4028716)

编译好的windows可用版本的下载地址（官网中可以连接到这个网站，和官方网站保持同步）：
[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)

该网站中的FFMPEG分为3个版本：Static，Shared，Dev。

前两个版本可以直接在命令行中使用，他们的区别在于：Static里面只有3个应用程序：ffmpeg.exe，ffplay.exe，ffprobe.exe，每个exe的体积都很大，相关的Dll已经被编译到exe里面去了。Shared里面除了3个应用程序：ffmpeg.exe，ffplay.exe，ffprobe.exe之外，还有一些Dll，比如说avcodec-54.dll之类的。Shared里面的exe体积很小，他们在运行的时候，到相应的Dll中调用功能。

Dev版本是用于开发的，里面包含了库文件xxx.lib以及头文件xxx.h，这个版本不包含exe文件。

  二、ffmpeg编码解码

先给出几个概念，以在后面的分析中方便理解

Container:在音视频中的容器，一般指的是一种特定的文件格式，里面指明了所包含的

音视频，字幕等相关信息

Stream:这个词有些微妙，很多地方都用到，比如TCP，SVR4系统等，其实在音视频，你

可以理解为单纯的音频数据或者视频数据等

Frames:这个概念不是很好明确的表示，指的是Stream中的一个数据单元，要真正对这

个概念有所理解，可能需要看一些音视频编码解码的理论知识

Packet:是Stream的raw数据

Codec:Coded + Decoded

其实这些概念在在FFmpeg中都有很好的体现，我们在后续分析中会慢慢看到。

基本上来说，处理视频和音频流是很容易的：

10 从video.avi文件中打开视频流video_stream

20 从视频流中读取包到帧中

30 如果这个帧还不完整，跳到20

40 对这个帧进行一些操作

50 跳回到20

在这个程序中使用ffmpeg来处理多种媒体是相当容易的，虽然很多程序可能在对帧进行操作的时候非常的复杂。因此在这篇指导中，我们将打开一个文件，读取里面的视频流，而且我们对帧的操作将是把这个帧写到一个PPM文件中。

代码：

```cpp
#include <inttypes.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" 
{
	#include "libavutil/avutil.h"
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
	#include "libavdevice/avdevice.h"
	#include "libswscale/swscale.h"
}
#endif

#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"swscale.lib")

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);
int img_convert2(AVPicture *dst, int dst_pix_fmt,AVPicture *src, int src_pix_fmt,int src_width, int src_height);

int main(int argc, char* argv[])
{
	av_register_all();

	AVFormatContext *pFormatCtx;
	
	// Open video file
	
	if(av_open_input_file(&pFormatCtx, argv[1], NULL, 0, NULL)!=0)
		return -1; // Couldn't open file

	if(av_find_stream_info(pFormatCtx)<0)	
		return -1; // Couldn't find stream information

	int i = 0;	
	int videoStream=-1;
	AVCodecContext *pCodecCtx = NULL;
	for(i=0; i < pFormatCtx->nb_streams; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if(videoStream==-1)		
		return -1; // Didn't find a video stream
	
	// Get a pointer to the codec context for the video stream
	pCodecCtx=pFormatCtx->streams[videoStream]->codec;

	AVCodec *pCodec = NULL;
	
	// Find the decoder for the video stream
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	
	if(pCodec==NULL)
	{	
		fprintf(stderr, "Unsupported codec!\n");	
		return -1; // Codec not found	
	}
	
	// Open codec	
	if(avcodec_open(pCodecCtx, pCodec)<0)	
		return -1; // Could not open codec

	AVFrame *pFrame,*pFrameRGB;	
	// Allocate video frame
	pFrame=avcodec_alloc_frame();
	pFrameRGB=avcodec_alloc_frame();	
	if(pFrameRGB==NULL)	
		return -1;

	uint8_t *buffer;	
	int numBytes;	
	// Determine required buffer size and allocate buffer
	numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
	buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,pCodecCtx->width, pCodecCtx->height);

	int frameFinished;
	
	AVPacket packet;
	
	i=0;

	while(av_read_frame(pFormatCtx, &packet)>=0)
	{	
		// Is this a packet from the video stream?		
		if(packet.stream_index==videoStream)
		{	
			// Decode video frame	
			avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,packet.data, packet.size);
			// Did we get a video frame?	
			if(frameFinished)
			{	
				// Convert the image from its native format to RGB	
				img_convert2((AVPicture *)pFrameRGB,PIX_FMT_RGB24,(AVPicture*)pFrame, pCodecCtx->pix_fmt,	
					pCodecCtx->width, pCodecCtx->height);
				
				// Save the frame to disk		
				if(++i<=100)	
					SaveFrame(pFrameRGB, pCodecCtx->width,pCodecCtx->height, i);
				
			}
			
		}
		
		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);	
	}

	// Free the RGB image	
	av_free(buffer);
	
	av_free(pFrameRGB);
	
	// Free the YUV frame	
	av_free(pFrame);
	
	// Close the codec	
	avcodec_close(pCodecCtx);

	av_close_input_file(pFormatCtx);

	return 0;
}

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
	
	FILE *pFile;
	
	char szFilename[32];
	
	int y;
	
	// Open file
	
	sprintf(szFilename, "frame%d.ppm", iFrame);
	
	pFile=fopen(szFilename, "wb");
	
	if(pFile==NULL)
		
		return;
	
	// Write header
	
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);
	
	// Write pixel data
	
	for(y=0; y<height; y++)
		
		fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
	
	// Close file
	
	fclose(pFile);
	
}

int img_convert2(AVPicture *dst, int dst_pix_fmt,				
                AVPicture *src, int src_pix_fmt,				
                int src_width, int src_height)				
{
	
    int w;	
    int h;
    SwsContext *pSwsCtx;
	w = src_width;	
    h = src_height;	
    pSwsCtx = sws_getContext(w, h, src_pix_fmt, w, h, dst_pix_fmt,SWS_BICUBIC, NULL, NULL, NULL);
	
    sws_scale(pSwsCtx,src->data, src->linesize,0, h, dst->data, dst->linesize);
	
	//这里释放掉pSwsCtx的内存
	sws_freeContext(pSwsCtx);

    return 0;	
}
```


三、ffmpeg框架代码

转载[http://blog.csdn.net/wstarx/article/details/1572393](http://blog.csdn.net/wstarx/article/details/1572393)

FFmpeg主目录下主要有libavcodec、libavformat和libavutil等子目录。其中libavcodec用于存放各个encode/decode模块，libavformat用于存放muxer/demuxer模块，libavutil用于存放内存操作等常用模块。

以flash movie的flv文件格式为例， muxer/demuxer的flvenc.c和flvdec.c文件在libavformat目录下，encode/decode的mpegvideo.c和h263de.c在libavcodec目录下。

**muxer/demuxer与encoder/decoder定义与初始化**

muxer/demuxer和encoder/decoder在FFmpeg中的实现代码里，有许多相同的地方，而二者最大的差别是muxer和demuxer分别是不同的结构AVOutputFormat与AVInputFormat，而encoder和decoder都是用的AVCodec结构。

muxer/demuxer和encoder/decoder在FFmpeg中相同的地方有：

二者都是在main()开始的av_register_all()函数内初始化的。

二者都是以链表的形式保存在全局变量中的。

muxer/demuxer是分别保存在全局变量AVOutputFormat *first_oformat与AVInputFormat *first_iformat中的。

encoder/decoder都是保存在全局变量AVCodec *first_avcodec中的。

二者都用函数指针的方式作为开放的公共接口。

demuxer开放的接口有：

**int (*read_probe)(AVProbeData *);**

**int(*read_header)(struct AVFormatContext *,AVFormatParameters *ap);**

**int (*read_packet)(struct AVFormatContext*, AVPacket *pkt);**

**int (*read_close)(struct AVFormatContext*);**

**int (*read_seek)(struct AVFormatContext *,int stream_index, int64_t timestamp, int flags);**

muxer开放的接口有：

**int (*write_header)(struct AVFormatContext *);**

**int (*write_packet)(struct AVFormatContext *, AVPacket*pkt);**

**int (*write_trailer)(struct AVFormatContext *);**

encoder/decoder的接口都是一样的，只不过二者分别只实现encoder和decoder函数：

**int (*init)(AVCodecContext *);**

**int (*encode)(AVCodecContext *, uint8_t *buf, intbuf_size, void *data);**

**int (*close)(AVCodecContext *);**

**int (*decode)(AVCodecContext *, void *outdata, int*outdata_size, uint8_t *buf, int buf_size);**

仍以flv文件为例来说明muxer/demuxer的初始化。

在libavformat/allformats.c文件的**av_register_all(void)**函数中，通过执行

**REGISTER_MUXDEMUX(FLV, flv);**

将支持flv 格式的**flv_muxer**与**flv_demuxer**变量分别注册到全局变量**first_oformat**与**first_iformat**链表的最后位置。

其中**flv_muxer**在libavformat/flvenc.c中定义如下：

**AVOutputFormat flv_muxer = {**

**"flv",**

**"flv format",**

**"video/x-flv",**

**"flv",**

**sizeof(FLVContext),**

**#ifdef CONFIG_LIBMP3LAME**

**CODEC_ID_MP3,**

**#else // CONFIG_LIBMP3LAME**

**CODEC_ID_NONE,**

**CODEC_ID_FLV1,**

**flv_write_header,**

**flv_write_packet,**

**flv_write_trailer,**

**.codec_tag= (const AVCodecTag*[]){flv_video_codec_ids,flv_audio_codec_ids, 0},**

**}**

**AVOutputFormat**结构的定义如下：

**typedef struct AVOutputFormat {**

**const char *name;**

**const char *long_name;**

**const char *mime_type;**

**const char *extensions; /**< comma separated filenameextensions */**

**/** size of private data so that it can be allocated inthe wrapper */**

**int priv_data_size;**

**/* output support */**

**enum CodecID audio_codec; /**< default audio codec */**

**enum CodecID video_codec; /**<default video codec */**

**int (*write_header)(struct AVFormatContext *);**

**int (*write_packet)(struct AVFormatContext *, AVPacket*pkt);**

**int (*write_trailer)(struct AVFormatContext *);**

**/** can use flags: AVFMT_NOFILE, AVFMT_NEEDNUMBER,AVFMT_GLOBALHEADER */**

**int flags;**

**/** currently only used to set pixel format if notYUV420P */**

**int (*set_parameters)(struct AVFormatContext *,AVFormatParameters *);**

**int (*interleave_packet)(struct AVFormatContext *,AVPacket *out, AVPacket *in, int flush);**

**/****

*** list of supported codec_id-codec_tag pairs, ordered by"better choice first"**

*** the arrays are all CODEC_ID_NONE terminated**

***/**

**const struct AVCodecTag **codec_tag;**

**/* private fields */**

**struct AVOutputFormat *next;**

**} AVOutputFormat;**

由**AVOutputFormat**结构的定义可知，**flv_muxer**变量初始化的第一、第二个成员分别为该muxer的名称与长名称，第三、第四个成员为所对应MIMIEType和后缀名，第五个成员是所对应的私有结构的大小，第六、第七个成员为所对应的音频编码和视频编码类型ID，接下来就是三个重要的接口函数，该muxer的功能也就是通过调用这三个接口实现的。

**flv_demuxer**在libavformat/flvdec.c中定义如下, 与**flv_muxer**类似，在这儿主要也是设置了5个接口函数，其中**flv_probe**接口用途是测试传入的数据段是否是符合当前文件格式，这个接口在匹配当前demuxer的时候会用到。

**AVInputFormat flv_demuxer = {**

**"flv",**

**"flv format",**

**0,**

**flv_probe,**

**flv_read_header,**

**flv_read_packet,**

**flv_read_close,**

**flv_read_seek,**

**.extensions = "flv",**

**.value = CODEC_ID_FLV1,**

**};**

在上述**av_register_all(void)**函数中通过执行libavcodec/allcodecs.c文件里的**avcodec_register_all(void)**函数来初始化全部的encoder/decoder。

因为不是每种编码方式都支持encode和decode，所以有以下三种注册方式：

**#define REGISTER_ENCODER(X,x) /**

**if(ENABLE_##X##_ENCODER)register_avcodec(&x##_encoder)**

**#define REGISTER_DECODER(X,x) /**

**if(ENABLE_##X##_DECODER)register_avcodec(&x##_decoder)**

**#define REGISTER_ENCDEC(X,x) **

**REGISTER_ENCODER(X,x);REGISTER_DECODER(X,x)**

如支持flv的**flv_encoder**和**flv_decoder**变量就分别是在libavcodec/mpegvideo.c和libavcodec/h263de.c中创建的。

**当前muxer/demuxer的匹配**

在FFmpeg的文件转换过程中，首先要做的就是根据传入文件和传出文件的后缀名匹配合适的demuxer和muxer。

匹配上的demuxer和muxer都保存在如下所示，定义在ffmpeg.c里的全局变量**file_iformat**和**file_oformat**中：

**static AVInputFormat *file_iformat;**

**static AVOutputFormat *file_oformat;**

1. demuxer匹配

在libavformat/utils.c中的**static AVInputFormat *av_probe_input_format2(AVProbeData*pd, int is_opened, int *score_max)**函数用途是根据传入的probe data数据，依次调用每个demuxer的read_probe接口，来进行该demuxer是否和传入的文件内容匹配的判断。其调用顺序如下：

**void parse_options(int argc, char **argv,const OptionDef *options)**

**static void opt_input_file(const char *filename)**

**static void opt_input_file(const char*filename)**

**int av_open_input_file(…… )**

**AVInputFormat *av_probe_input_format(AVProbeData *pd, intis_opened)**

**static AVInputFormat*av_probe_input_format2(……)**

**opt_input_file**函数是在保存在**const OptionDef options[]**数组中，用于**void parse_options(int argc, char **argv,const OptionDef *options)**中解析argv里的“-i” 参数，也就是输入文件名时调用的。

2. muxer匹配

与demuxer的匹配不同，muxer的匹配是调用**guess_format**函数，根据**main( )** 函数的argv里的输出文件后缀名来进行的。

**void parse_options(int argc, char **argv,const OptionDef *options)**

**void parse_arg_file(const char *filename)**

**static void opt_output_file(const char*filename)**

**AVOutputFormat *guess_format(const char*short_name, const char *filename,**

**const char *mime_type)**

**当前encoder/decoder的匹配**

在**main( )**函数中除了解析传入参数并初始化demuxer与muxer的**parse_options( )**函数以外，其他的功能都是在**av_encode( )**函数里完成的。

在libavcodec/utils.c中有如下二个函数。

**AVCodec *avcodec_find_encoder(enum CodecID id)**

**AVCodec *avcodec_find_decoder(enum CodecID id)**

他们的功能就是根据传入的CodecID，找到匹配的encoder和decoder。

在**av_encode( )**函数的开头，首先初始化各个**AVInputStream**和**AVOutputStream**，然后分别调用上述二个函数，并将匹配上的encoder与decoder分别保存在**AVInputStream->AVStream*st->AVCodecContext *codec->struct AVCodec *codec**与**AVOutputStream->AVStream*st->AVCodecContext *codec->struct AVCodec *codec**变量中。

**其他主要数据结构**

1. AVFormatContext

**AVFormatContext**是FFMpeg格式转换过程中实现输入和输出功能、保存相关数据的主要结构。每一个输入和输出文件，都在如下定义的指针数组全局变量中有对应的实体。

**static AVFormatContext *output_files[MAX_FILES];**

**static AVFormatContext *input_files[MAX_FILES];**

对于输入和输出，因为共用的是同一个结构体，所以需要分别对该结构中如下定义的**iformat**或**oformat**成员赋值。

**struct AVInputFormat *iformat;**

**struct AVOutputFormat *oformat;**

对一个**AVFormatContext**来说，二个成员不能同时有值，即一个**AVFormatContext**不能同时含有demuxer和muxer。

在**main( )**函数开头的**parse_options( )**函数中找到了匹配的muxer和demuxer之后，根据传入的argv参数，初始化每个输入和输出的**AVFormatContext**结构，并保存在相应的**output_files**和**input_files**指针数组中。

在**av_encode( )**函数中，**output_files**和**input_files**是作为函数参数传入后，在其他地方就没有用到了。

2. AVCodecContext

保存**AVCodec**指针和与codec相关的数据，如video的width、height，audio的sample rate等。**AVCodecContext**中的**codec_type，codec_id**二个变量对于encoder/decoder的匹配来说，最为重要。

**enum CodecType codec_type; /* see CODEC_TYPE_xxx */**

**enum CodecID codec_id; /* see CODEC_ID_xxx */**

如上所示，**codec_type**保存的是**CODEC_TYPE_VIDEO**，**CODEC_TYPE_AUDIO**等媒体类型，

**codec_id**保存的是**CODEC_ID_FLV1**，**CODEC_ID_VP6F**等编码方式。

以支持flv格式为例，在前述的**av_open_input_file(…… )** 函数中，匹配到正确的**AVInputFormat**demuxer后，通过**av_open_input_stream( )**函数中调用**AVInputFormat**的**read_header**接口来执行flvdec.c中的**flv_read_header( )**函数。在**flv_read_header( )**函数内，根据文件头中的数据，创建相应的视频或音频**AVStream**，并设置**AVStream**中**AVCodecContext**的正确的**codec_type**值。**codec_id**值是在解码过程中**flv_read_packet( )**函数执行时根据每一个packet头中的数据来设置的。

3. AVStream

**AVStream**结构保存与数据流相关的编解码器，数据段等信息。比较重要的有如下二个成员：

**AVCodecContext *codec; /**< codec context */**

**void *priv_data;**

其中**codec**指针保存的就是上节所述的encoder或decoder结构。**priv_data**指针保存的是和具体编解码流相关的数据，如下代码所示，在ASF的解码过程中，**priv_data**保存的就是**ASFStream**结构的数据。

**AVStream *st;**

**ASFStream *asf_st; **

**… …**

**st->priv_data = asf_st; **

4. AVInputStream/ AVOutputStream

根据输入和输出流的不同，前述的**AVStream**结构都是封装在**AVInputStream**和
**AVOutputStream**结构中，在**av_encode( )**函数中使用。

**AVInputStream**中还保存的有与时间有关的信息。

**AVOutputStream**中还保存有与音视频同步等相关的信息。

5. AVPacket

**AVPacket**结构定义如下，其是用于保存读取的packet数据。

**typedef struct AVPacket {**

**int64_t pts; ///< presentation time stamp in time_baseunits**

**int64_t dts; ///< decompression time stamp intime_base units**

**uint8_t *data;**

**int size;**

**int stream_index;**

**int flags;**

**int duration; ///< presentation duration in time_baseunits (0 if not available)**

**void (*destruct)(struct AVPacket *);**

**void *priv;**

**int64_t pos; ///< byte position in stream, -1 ifunknown**

**} AVPacket;**

在**av_encode( )**函数中，调用**AVInputFormat**的**(*read_packet)(struct AVFormatContext *,AVPacket *pkt);**接口，读取输入文件的一帧数据保存在当前输入**AVFormatContext**的**AVPacket**成员中。

**av_encode函数主要流程**

**av_encode( )**函数是FFMpeg中最重要的函数，编解码和输出等大部分功能都在此函数内完成，因此有必要详细描述一下这个函数的主要流程。

1. input streams initializing

2. output streams initializing

3. encoders and decoders initializing

4. set meta data information from input file if required.

5. write output files header

6. loop of handling each frame

a. read frame from input file:

b. decode frame data

c. encode new frame data

d. write new frame to output file

7. write output files trailer

8. close each encoder and decoder 


参考资料：

[http://ffmpeg.org/](http://ffmpeg.org/)

[www.**ffmpeg**.com.cn](http://www.ffmpeg.com.cn/)

[http://www.libsdl.org/](http://www.libsdl.org/)

[FFmpeg tutorial](http://blog.csdn.net/siyingruoshui/article/details/6992029)

英文原文地址:http://www.dranger.com/ffmpeg/

转载翻译博客地址:http://blog.sina.com.cn/s/blog_46dc65a90100a91b.html

文中涉及的源码打包地址:http://www.dranger.com/ffmpeg/ffmpegsource.tar.gz

[FFmpeg介绍及参数详细说明](http://blog.csdn.net/liangkaiming/article/details/5798898)

[http://blog.csdn.net/liangkaiming/article/details/5798898](http://blog.csdn.net/liangkaiming/article/details/5798898)

ffmpeg编译及使用

[http://lvzun.iteye.com/blog/706121](http://lvzun.iteye.com/blog/706121)

windows下编译ffmpeg源代码 

[http://blog.csdn.net/jszj/article/details/4028716](http://blog.csdn.net/jszj/article/details/4028716)

FFMPEG+SDL最新库文件和源文件

[http://download.csdn.net/detail/sonicx24/3740122](http://download.csdn.net/detail/sonicx24/3740122)

使[用ffmpeg+ffserver+ffplay实现视频点播](http://blog.csdn.net/qiuchangyong/article/details/6623901)

[http://blog.csdn.net/qiuchangyong/article/details/6623901](http://blog.csdn.net/qiuchangyong/article/details/6623901)

让ffmpeg支持RMVB解码

[http://hi.baidu.com/jingxshi/blog/item/7aedb3d94e4818e539012fe3.html](http://hi.baidu.com/jingxshi/blog/item/7aedb3d94e4818e539012fe3.html)

[使用ffmpeg进行摄像头捕获](http://blog.simophin.net/?p=825)

http://blog.simophin.net/?p=825&cpage=1

