# ffmpeg+SDL的使用之使用SDL播放音频流 - zhuyinglong的博客 - CSDN博客





2017年09月30日 10:55:18[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：661








# 前言
首先再次说明下使用ffmpeg处理音视频流的步骤如下：

> 1：从本地或者网络位置打开音视频流；
2：获取音视频流中的包，将其转换为帧；
3：如果帧不完整，转到2；
4：对帧做一些事情；
5：转到2；




我们所能做的就是在步骤4上进行一些处理。

本项目整体流程如下：
设置SDL的音频参数 —-> 打开声音设备，播放静音 —-> ffmpeg读取音频流中数据放入buffer —->SDL调用用户设置的函数（**This function is called when the audio device needs more data.**）来获取音频数据 —-> 播放音频

注意：本项目中对音频的播放并没有做延时，但是声音是可以正常播放的。虽然程序将音频数据尽可能快的填充到声卡的缓冲区中，但是计算机的声音设备却是按照原来指定的采样率进行播放的。


# 源码

有了前篇文章[ffmpeg+SDL的使用之使用SDL播放视频流](http://blog.csdn.net/zhuyinglong2010/article/details/78140500)的技术积累，现在需要学习的只是如何使用ffmpeg将音频数据存入buffer，使用回调函数进行播放。相对来讲还是比较简单的。头文件采用和前篇博文一样的头文件ffmpegsdl.h，工程配置方式也相同，可参考上篇博文，不再赘述。

audioPlay.cpp的源码如下：



```cpp
#include <iostream>
#include "ffmpegsdl.h"
using namespace std;

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit(4 byte) audio  


static  Uint8  *audio_chunk;
static  int  audio_len;
static  Uint8  *audio_pos;

/**
*  This function is called when the audio device needs more data.
*
*  \param userdata An application-specific parameter saved in
*                  the SDL_AudioSpec structure
*  \param stream A pointer to the audio data buffer.
*  \param len    The length of that buffer in bytes.
*
*  Once the callback returns, the buffer will no longer be valid.
*  Stereo samples are stored in a LRLRLR ordering.
*
*  You can choose to avoid callbacks and use SDL_QueueAudio() instead, if
*  you like. Just open your audio device with a NULL callback.
*/
void  audio_callback(void *udata, Uint8 *stream, int len)
{
	//SDL 2.0  
	SDL_memset(stream, 0, len);
	if (audio_len == 0)
	{
		return;
	}
	len = (len>audio_len ? audio_len : len);   /*  Mix  as  much  data  as  possible  */
	/**
	 *  This takes two audio buffers of the playing audio format and mixes
	 *  them, performing addition, volume adjustment, and overflow clipping.
	 */
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

int main()
{
	char pFile[] = "..\\test.mp4";
	av_register_all();
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	//Open  
	if (avformat_open_input(&pFormatCtx, pFile, NULL, NULL) != 0) 
	{
		printf("Couldn't open input stream.\n");
		return -1;
	}
	// Retrieve stream information  
	if (avformat_find_stream_info(pFormatCtx, NULL)<0) 
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
	// Dump valid information onto standard error  
	av_dump_format(pFormatCtx, 0, pFile, false);

	AVCodec         *pCodec;
	// Find the first audio stream  
	int  audio_stream_index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &pCodec, 0);
	if (audio_stream_index == -1) 
	{
		printf("Didn't find a audio stream.\n");
		return -1;
	}

	if (pCodec == NULL) 
	{
		printf("Codec not found.\n");
		return -1;
	}
	AVCodecContext  *pCodecCtx = avcodec_alloc_context3(pCodec);
	//Fill the codec context based on the values from the supplied codec
	avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[audio_stream_index]->codecpar);
	// Open codec  
	if (avcodec_open2(pCodecCtx, pCodec, NULL)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}
	
	AVPacket* packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);

	//Out Audio Param  
	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	//nb_samples: AAC-1024 MP3-1152  
	int out_nb_samples = pCodecCtx->frame_size;
	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = 44100;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	//Out Buffer Size  
	int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);

	uint8_t* out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	AVFrame* pFrame = av_frame_alloc();
	//SDL------------------ Init  
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) 
	{
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}
	//SDL_AudioSpec 
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = pCodecCtx->sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = pCodecCtx->channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = pCodecCtx->frame_size;
	wanted_spec.callback = audio_callback;
	wanted_spec.userdata = pCodecCtx;

	if (SDL_OpenAudio(&wanted_spec, NULL)<0)
	{
		printf("can't open audio.\n");
		return -1;
	}

	//FIX:Some Codec's Context Information is missing  
	int64_t in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
	
	//Swr
	SwrContext* au_convert_ctx = swr_alloc();
	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);

	//Play  
	SDL_PauseAudio(0);
	int ret = -1;
	int index = 0;
	while (1)
	{
		ret = av_read_frame(pFormatCtx, packet);//一个包可能有多个帧
		if (ret < 0)
		{
			break;
		}

		if (packet->stream_index == audio_stream_index)
		{
			//Supply raw packet data as input to a decoder.
			ret = avcodec_send_packet(pCodecCtx, packet);
			if (ret < 0)
			{
				av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
				break;
			}

			while (ret >= 0)
			{
				//Return decoded output data from a decoder.
				ret = avcodec_receive_frame(pCodecCtx, pFrame);
				if (ret >= 0)
				{
					swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pFrame->data, pFrame->nb_samples);
					printf("index:%5d\t pts:%lld\t packet size:%d\n", index, packet->pts, packet->size);
					index++;
				}
				else if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					break;
				}
				else
				{
					av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
					goto end;
				}

			}

			while (audio_len > 0)//Wait until finish  
			{
				SDL_Delay(1);
			}

			//Set audio buffer (PCM data)  
			audio_chunk = (Uint8 *)out_buffer;
			//Audio buffer length  
			audio_len = out_buffer_size;
			audio_pos = audio_chunk;

		}
		av_packet_unref(packet);
	}

	

end:
	// Free the packet that was allocated by av_read_frame
	av_packet_free(&packet);
	swr_free(&au_convert_ctx);
	SDL_CloseAudio();//Close SDL  
	SDL_Quit();
	av_free(out_buffer);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}
```




# **工程下载**



github: [https://github.com/zhuyinglong/ffmpeg-SDL](https://github.com/zhuyinglong/ffmpeg-SDL)



# **后续**

虽然我尽力让自己正确，但避免不了有些理解不到位的误导大家，有些不清楚的地方，推荐大家还是参考官方原版一手的资料。参考资料如下：
[http://dranger.com/ffmpeg/tutorial03.html](http://dranger.com/ffmpeg/tutorial03.html)
[http://ffmpeg.org/doxygen/trunk/filtering_video_8c-example.html](http://ffmpeg.org/doxygen/trunk/filtering_video_8c-example.html)







