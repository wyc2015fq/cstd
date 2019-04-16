# 最简单的基于FFMPEG+SDL的音频播放器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年08月29日 17:11:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：130









FFMPEG工程浩大，可以参考的书籍又不是很多，因此很多刚学习FFMPEG的人常常感觉到无从下手。

在此我把自己做项目过程中实现的一个非常简单的音频播放器（大约200行代码）源代码传上来，以作备忘，同时方便新手学习FFMPEG。

该播放器虽然简单，但是几乎包含了使用FFMPEG播放一个音频所有必备的API，并且使用SDL输出解码出来的音频。

并且支持流媒体等多种音频输入。
程序使用了新的FFMPEG类库，和早期版本的FFMPEG类库的API函数略有不同。

平台使用VC2010



注意：

1.程序输出的解码后PCM音频数据可以使用Audition打开播放

2.m4a,aac文件可以直接播放。mp3文件需要调整SDL音频帧大小为4608（默认是4096），否则播放会不流畅

3.也可以播放视频中的音频



贴上程序代码：



```cpp
//
//FFMPEG+SDL音频解码程序
//雷霄骅
//中国传媒大学/数字电视技术
//leixiaohua1020@126.com
//
//
#include <stdlib.h>
#include <string.h>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
//SDL
#include "sdl/SDL.h"
#include "sdl/SDL_thread.h"
};
#include "decoder.h"
//#include "wave.h"

//#define _WAVE_

//全局变量---------------------
	static  Uint8  *audio_chunk; 
	static  Uint32  audio_len; 
	static  Uint8  *audio_pos; 
//-----------------
	/*  The audio function callback takes the following parameters: 
	stream: A pointer to the audio buffer to be filled 
	len: The length (in bytes) of the audio buffer (这是固定的4096？)
	回调函数
	注意：mp3为什么播放不顺畅？
	len=4096;audio_len=4608;两个相差512！为了这512，还得再调用一次回调函数。。。
	m4a,aac就不存在此问题(都是4096)！
	*/ 
	void  fill_audio(void *udata,Uint8 *stream,int len){ 
		/*  Only  play  if  we  have  data  left  */ 
	if(audio_len==0) 
			return; 
		/*  Mix  as  much  data  as  possible  */ 
	len=(len>audio_len?audio_len:len); 
	SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);
	audio_pos += len; 
	audio_len -= len; 
	} 
//-----------------


int decode_audio(char* no_use)
{
	AVFormatContext	*pFormatCtx;
	int				i, audioStream;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;

	char url[300]={0};
	strcpy(url,no_use);
	//Register all available file formats and codecs
	av_register_all();

	//支持网络流输入
	avformat_network_init();
	//初始化
	pFormatCtx = avformat_alloc_context();
	//有参数avdic
	//if(avformat_open_input(&pFormatCtx,url,NULL,&avdic)!=0){
	if(avformat_open_input(&pFormatCtx,url,NULL,NULL)!=0){
		printf("Couldn't open file.\n");
		return -1;
	}
	
	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
	// Dump valid information onto standard error
	av_dump_format(pFormatCtx, 0, url, false);

	// Find the first audio stream
	audioStream=-1;
	for(i=0; i < pFormatCtx->nb_streams; i++)
		//原为codec_type==CODEC_TYPE_AUDIO
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			audioStream=i;
			break;
		}

	if(audioStream==-1)
	{
		printf("Didn't find a audio stream.\n");
		return -1;
	}

	// Get a pointer to the codec context for the audio stream
	pCodecCtx=pFormatCtx->streams[audioStream]->codec;

	// Find the decoder for the audio stream
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}

	// Open codec
	if(avcodec_open(pCodecCtx, pCodec)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}

	/********* For output file ******************/
	FILE *pFile;
#ifdef _WAVE_
	pFile=fopen("output.wav", "wb");
	fseek(pFile, 44, SEEK_SET); //预留文件头的位置
#else
	pFile=fopen("output.pcm", "wb");
#endif

	// Open the time stamp file
	FILE *pTSFile;
	pTSFile=fopen("audio_time_stamp.txt", "wb");
	if(pTSFile==NULL)
	{
		printf("Could not open output file.\n");
		return -1;
	}
	fprintf(pTSFile, "Time Base: %d/%d\n", pCodecCtx->time_base.num, pCodecCtx->time_base.den);

	/*** Write audio into file ******/
	//把结构体改为指针
	AVPacket *packet=(AVPacket *)malloc(sizeof(AVPacket));
	av_init_packet(packet);

	//音频和视频解码更加统一！
	//新加
	AVFrame	*pFrame;
	pFrame=avcodec_alloc_frame();

	//---------SDL--------------------------------------
	//初始化
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		exit(1);
	}

	//结构体，包含PCM数据的相关信息
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = pCodecCtx->sample_rate; 
	wanted_spec.format = AUDIO_S16SYS; 
	wanted_spec.channels = pCodecCtx->channels; 
	wanted_spec.silence = 0; 
	wanted_spec.samples = 1024; //播放AAC，M4a，缓冲区的大小
	//wanted_spec.samples = 1152; //播放MP3，WMA时候用
	wanted_spec.callback = fill_audio; 
	wanted_spec.userdata = pCodecCtx; 

	if (SDL_OpenAudio(&wanted_spec, NULL)<0)//步骤（2）打开音频设备 
	{ 
		printf("can't open audio.\n"); 
		return 0; 
	} 
	//-----------------------------------------------------
	printf("比特率 %3d\n", pFormatCtx->bit_rate);
	printf("解码器名称 %s\n", pCodecCtx->codec->long_name);
	printf("time_base  %d \n", pCodecCtx->time_base);
	printf("声道数  %d \n", pCodecCtx->channels);
	printf("sample per second  %d \n", pCodecCtx->sample_rate);
	//新版不再需要
//	short decompressed_audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
//	int decompressed_audio_buf_size;
	uint32_t ret,len = 0;
	int got_picture;
	int index = 0;
	while(av_read_frame(pFormatCtx, packet)>=0)
	{
		if(packet->stream_index==audioStream)
		{
			//decompressed_audio_buf_size = (AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2;
			//原为avcodec_decode_audio2
				//ret = avcodec_decode_audio4( pCodecCtx, decompressed_audio_buf,
				//&decompressed_audio_buf_size, packet.data, packet.size );
			//改为
			ret = avcodec_decode_audio4( pCodecCtx, pFrame,
				&got_picture, packet);
			if ( ret < 0 ) // if error len = -1
			{
                printf("Error in decoding audio frame.\n");
                exit(0);
            }
			if ( got_picture > 0 )
			{
#if 1
				printf("index %3d\n", index);
				printf("pts %5d\n", packet->pts);
				printf("dts %5d\n", packet->dts);
				printf("packet_size %5d\n", packet->size);
				
				//printf("test %s\n", rtmp->m_inChunkSize);
#endif
				//直接写入
				//注意：数据是data【0】，长度是linesize【0】
#if 1
				fwrite(pFrame->data[0], 1, pFrame->linesize[0], pFile);
				//fwrite(pFrame, 1, got_picture, pFile);
				//len+=got_picture;
				index++;
				//fprintf(pTSFile, "%4d,%5d,%8d\n", index, decompressed_audio_buf_size, packet.pts);
#endif
			}
#if 1
			//---------------------------------------
			//printf("begin....\n"); 
			//设置音频数据缓冲,PCM数据
			audio_chunk = (Uint8*) pFrame->data[0]; 
			//设置音频数据长度
			audio_len = pFrame->linesize[0];
			//audio_len = 4096;
			//播放mp3的时候改为audio_len = 4096
			//则会比较流畅，但是声音会变调！MP3一帧长度4608
			//使用一次回调函数（4096字节缓冲）播放不完，所以还要使用一次回调函数，导致播放缓慢。。。
			//设置初始播放位置
			audio_pos = audio_chunk;
			//回放音频数据 
			SDL_PauseAudio(0);
			//printf("don't close, audio playing...\n"); 
			while(audio_len>0)//等待直到音频数据播放完毕! 
				SDL_Delay(1); 
			//---------------------------------------
#endif
		}
		// Free the packet that was allocated by av_read_frame
		//已改
		av_free_packet(packet);
	}
	//printf("The length of PCM data is %d bytes.\n", len);

#ifdef _WAVE_
	fseek(pFile, 0, SEEK_SET);
	struct WAVE_HEADER wh;

	memcpy(wh.header.RiffID, "RIFF", 4);
	wh.header.RiffSize = 36 + len;
	memcpy(wh.header.RiffFormat, "WAVE", 4);

	memcpy(wh.format.FmtID, "fmt ", 4);
	wh.format.FmtSize = 16;
	wh.format.wavFormat.FormatTag = 1;
	wh.format.wavFormat.Channels = pCodecCtx->channels;
	wh.format.wavFormat.SamplesRate = pCodecCtx->sample_rate;
	wh.format.wavFormat.BitsPerSample = 16;
	calformat(wh.format.wavFormat); //Calculate AvgBytesRate and BlockAlign

	memcpy(wh.data.DataID, "data", 4);
	wh.data.DataSize = len;

	fwrite(&wh, 1, sizeof(wh), pFile);
#endif
	SDL_CloseAudio();//关闭音频设备 
	// Close file
	fclose(pFile);
	// Close the codec
	avcodec_close(pCodecCtx);
	// Close the video file
	av_close_input_file(pFormatCtx);

	return 0;
}
```



程序会打印每一帧的信息

运行截图：

![](https://img-blog.csdn.net/20130829171426343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


完整工程下载地址：

[http://download.csdn.net/detail/leixiaohua1020/6033893](http://download.csdn.net/detail/leixiaohua1020/6033893)

完整工程（更新版）下载地址：

[http://download.csdn.net/detail/leixiaohua1020/7319225](http://download.csdn.net/detail/leixiaohua1020/7319225)

新版本中使用了最新版本的FFMPEG类库（2014.5.7）。FFMPEG在新版本中的音频解码方面发生了比较大的变化。如果将旧版的主程序和新版的类库组合使用的话，会出现听到的都是杂音这一现象。经过研究发现，新版中avcodec_decode_audio4()解码后输出的音频采样数据格式为AV_SAMPLE_FMT_FLTP（float, planar）而不再是AV_SAMPLE_FMT_S16（signed 16 bits）。因此无法直接使用SDL进行播放。

最后的解决方法是使用SwrContext对音频采样数据进行转换之后，再进行输出播放，问题就可以得到解决了。转换方面的代码如下示例：



```cpp
//输出音频数据大小，一定小于输出内存。
int out_linesize;
//输出内存大小
int out_buffer_size=av_samples_get_buffer_size(&out_linesize, pCodecCtx->channels,pCodecCtx->frame_size,pCodecCtx->sample_fmt, 1);
uint8_t *out_buffer=new uint8_t[out_buffer_size];
...
au_convert_ctx = swr_alloc();
au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,
	pCodecCtx->channel_layout,pCodecCtx->sample_fmt , pCodecCtx->sample_rate,0, NULL);
swr_init(au_convert_ctx);

while(av_read_frame(pFormatCtx, packet)>=0){
	......
	swr_convert(au_convert_ctx,&out_buffer, out_linesize,(const uint8_t **)pFrame->data , pFrame->nb_samples);

	......
}
```







**FFMPEG相关学习资料：**



# SDL GUIDE 中文译本

[http://download.csdn.net/detail/leixiaohua1020/6389841](http://download.csdn.net/detail/leixiaohua1020/6389841)


ffdoc （FFMPEG的最完整教程）

[http://download.csdn.net/detail/leixiaohua1020/6377803](http://download.csdn.net/detail/leixiaohua1020/6377803)

如何用FFmpeg编写一个简单播放器

[http://download.csdn.net/detail/leixiaohua1020/6373783](http://download.csdn.net/detail/leixiaohua1020/6373783)

====================

在SourceForge上传了一个版本：

[https://sourceforge.net/projects/simplestffmpegaudioplayer/](https://sourceforge.net/projects/simplestffmpegaudioplayer/)




