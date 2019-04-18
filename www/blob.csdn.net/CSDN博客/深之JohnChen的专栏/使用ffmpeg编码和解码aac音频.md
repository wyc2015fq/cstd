# 使用ffmpeg编码和解码aac音频 - 深之JohnChen的专栏 - CSDN博客

2018年06月16日 15:07:21[byxdaz](https://me.csdn.net/byxdaz)阅读数：1547


一、aac音频编码实例

```cpp
#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
};
#endif
#endif

int flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index)
{
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
		CODEC_CAP_DELAY))
		return 0;
	while (1) 
	{
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_audio2 (fmt_ctx->streams[stream_index]->codec, &enc_pkt,
			NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame)
		{
			ret=0;
			break;
		}
		printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n",enc_pkt.size);
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
			break;
	}

	return ret;
}

int main(int argc, char* argv[])
{
	AVFormatContext* pFormatCtx;
	AVOutputFormat* fmt;
	AVStream* audio_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;

	uint8_t* frame_buf;
	AVFrame* pFrame;
	AVPacket pkt;

	int got_frame=0;
	int ret=0;
	int size=0;

	FILE *in_file=NULL;	                        //Raw PCM data
	int framenum=1000;                          //Audio frame number
	const char* out_file = "tdjm.aac";          //Output URL
	int i = 0;
	in_file= fopen("tdjm.pcm", "rb");

	av_register_all();

	//申请avformat上下文
	pFormatCtx = avformat_alloc_context();
	fmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = fmt;

	//avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	//fmt = pFormatCtx->oformat;

	//Open output URL
	if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("Failed to open output file!\n");
		return -1;
	}

	audio_st = avformat_new_stream(pFormatCtx, 0);
	if (audio_st==NULL)
	{
		return -1;
	}
	pCodecCtx = audio_st->codec;
	pCodecCtx->codec_id = fmt->audio_codec;//音频编码id
	pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;//编码类型为音频
	pCodecCtx->sample_fmt = AV_SAMPLE_FMT_S16;//采样数据的宽度
	pCodecCtx->sample_rate= 44100;			  //音频采样率	
	pCodecCtx->channel_layout=AV_CH_LAYOUT_STEREO;//声道格式（单声道、双声道）
	pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);//声道数
	pCodecCtx->bit_rate = 64000;			//平均码率

	//调试信息输出
	av_dump_format(pFormatCtx, 0, out_file, 1);
	//查找编码器
	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("Can not find encoder!\n");
		return -1;
	}
	//打开编码器
	if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
	{
		printf("Failed to open encoder!\n");
		return -1;
	}
	//申请frame
	pFrame = av_frame_alloc();
	pFrame->nb_samples= pCodecCtx->frame_size;
	pFrame->format= pCodecCtx->sample_fmt;
	//计算编码器编码能力一帧的数据大小
	size = av_samples_get_buffer_size(NULL, pCodecCtx->channels,pCodecCtx->frame_size,pCodecCtx->sample_fmt, 1);
	frame_buf = (uint8_t *)av_malloc(size);
	avcodec_fill_audio_frame(pFrame, pCodecCtx->channels, pCodecCtx->sample_fmt,(const uint8_t*)frame_buf, size, 1);
	
	//Write Header
	avformat_write_header(pFormatCtx,NULL);

	av_new_packet(&pkt,size);
        i = 0;
	while(true)
	{
		//Read PCM
		if(feof(in_file))
		{
			break;
		}
		else if (fread(frame_buf, 1, size, in_file) < size)
		{
			printf("Failed to read raw data! \n");
			break;
		}

		pFrame->data[0] = frame_buf;  //PCM Data

		pFrame->pts=i* (pCodecCtx->frame_size * 1000 / pCodecCtx->sample_rate);				//播放时间戳需要不一样，否则后面音频帧插入失败	
		got_frame=0;
		//Encode
		ret = avcodec_encode_audio2(pCodecCtx, &pkt,pFrame, &got_frame);
		if(ret < 0)
		{
			printf("Failed to encode!\n");
			return -1;
		}
		if (got_frame==1)
		{
			printf("Succeed to encode 1 frame! \tsize:%5d\n",pkt.size);
			pkt.stream_index = audio_st->index;
			ret = av_write_frame(pFormatCtx, &pkt);
			av_free_packet(&pkt);
		}
		i++;
	}
	
	//Flush Encoder
	ret = flush_encoder(pFormatCtx,0);
	if (ret < 0) 
	{
		printf("Flushing encoder failed\n");
		return -1;
	}

	//Write Trailer
	av_write_trailer(pFormatCtx);

	//Clean
	if (audio_st)
	{
		avcodec_close(audio_st->codec);
		av_free(pFrame);
		av_free(frame_buf);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	fclose(in_file);

	return 0;
}
```
二、aac音频解码实例
AAC编码只会输出float planar类型的采样，那要怎样在不支持float planar的音频输出设备（比如Windows的WaveOut）上输出呢？
对音频进行重采样。ffmepg提供swr_convert重采样。

```cpp
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"  
#include "libavutil/opt.h"  
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"  
#include "libavutil/opt.h"  
#ifdef __cplusplus
};
#endif
#endif

int m_is_first_audio_pts = 0;

int main (void)
{

	//1.注册组件
	av_register_all();
	//封装格式上下文
	AVFormatContext *pFormatCtx = avformat_alloc_context();

	//2.打开输入音频文件
	if (avformat_open_input(&pFormatCtx, "tdjm.aac", NULL, NULL) != 0) 
	{
		printf("%s", "打开输入音频文件失败");
		return -1;
	}
	//3.获取音频信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) 
	{
		printf("%s", "获取音频信息失败");
		return -1;
	}

	//音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
	int audio_stream_idx = -1;
	int i = 0;
	for (i=0; i < pFormatCtx->nb_streams; i++) 
	{
		//根据类型判断是否是音频流
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) 
		{
			audio_stream_idx = i;
			break;
		}
	}
	//4.获取解码器
	//根据索引拿到对应的流,根据流拿到解码器上下文
	AVCodecContext *pCodeCtx = pFormatCtx->streams[audio_stream_idx]->codec;
	//再根据上下文拿到编解码id，通过该id拿到解码器
	AVCodec *pCodec = avcodec_find_decoder(pCodeCtx->codec_id);
	if (pCodec == NULL) 
	{
		printf("%s", "无法解码");
		return -1;
	}
	//5.打开解码器
	if (avcodec_open2(pCodeCtx, pCodec, NULL) < 0) 
	{
		printf("%s", "编码器无法打开");
		return -1;
	}
	//编码数据
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//解压缩数据
	AVFrame *frame = av_frame_alloc();

	//frame->16bit 44100 PCM 统一音频采样格式与采样率
	SwrContext *swrCtx = swr_alloc();
	//重采样设置选项
	//输入的采样格式
	enum AVSampleFormat in_sample_fmt = pCodeCtx->sample_fmt;
	//输出的采样格式 16bit PCM
	enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	//输入的采样率
	int in_sample_rate = pCodeCtx->sample_rate;
	//输出的采样率
	int out_sample_rate = pCodeCtx->sample_rate;//如果指定输出的采样率与pCodeCtx->sample_rate相差很大时，swr_convert输出的pcm有噪音，需要使用AVAudioFifo做分包处理。
	//输入的声道布局
	uint64_t in_ch_layout = pCodeCtx->channel_layout;
	//输出的声道布局
	uint64_t out_ch_layout = AV_CH_LAYOUT_MONO;//AV_CH_LAYOUT_MONO，单通道；AV_CH_LAYOUT_STEREO,双声道

	/*
	swr_alloc_set_opts(swrCtx, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt,
		in_sample_rate, 0, NULL);
	*/

#if LIBSWRESAMPLE_VERSION_MINOR >= 17    // 根据版本不同，选用适当函数  
	/* set options */  
	av_opt_set_int(swrCtx, "in_channel_layout", in_ch_layout, 0);  
	av_opt_set_int(swrCtx, "in_sample_rate", in_sample_rate, 0);  
	av_opt_set_sample_fmt(swrCtx, "in_sample_fmt",in_sample_fmt, 0);  
	av_opt_set_int(swrCtx, "out_channel_layout",out_ch_layout, 0);  
	av_opt_set_int(swrCtx, "out_sample_rate", out_sample_rate, 0);  
	av_opt_set_sample_fmt(swrCtx, "out_sample_fmt",out_sample_fmt, 0); 
#else  
	swr_alloc_set_opts(swrCtx, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt,
		in_sample_rate, 0, NULL);
#endif
	swr_init(swrCtx);

	//获取输出的声道个数
	int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
	//存储pcm数据
	int out_size = (out_channel_nb*out_sample_rate*16)/8.0;		//1秒的数据大小
	uint8_t *out_buffer = (uint8_t *) av_malloc(out_size/2);
	FILE *fp_pcm = fopen("out.pcm", "wb");
	int ret, got_frame, framecount = 0;
	//6.一帧一帧读取压缩的音频数据AVPacket
	while (av_read_frame(pFormatCtx, packet) >= 0) 
	{
		if (packet->stream_index == audio_stream_idx) 
		{
			while(packet->size > 0)
			{
				ret = avcodec_decode_audio4(pCodeCtx, frame, &got_frame, packet);
				if (ret < 0) 
				{
					printf("%s", "解码出现错误");
					break;
				}
				//解码成功
				if (got_frame) 
				{
					printf("解码%d帧\n", framecount++);
					int dst_nb_samples = av_rescale_rnd(swr_get_delay(swrCtx, pCodeCtx->sample_rate) + frame->nb_samples,
						out_sample_rate, pCodeCtx->sample_rate, AV_ROUND_UP);
					//av_samples_alloc(&out_buffer, NULL, out_channel_nb, dst_nb_samples, out_sample_fmt, 0);
					swr_convert(swrCtx, &out_buffer, dst_nb_samples,(const uint8_t **)frame->data, frame->nb_samples);
					//swr_convert(swrCtx, &out_buffer, out_channel_nb * out_sample_rate,(const uint8_t **)frame->data, frame->nb_samples);
					//获取sample的size
					int out_buffer_size = av_samples_get_buffer_size(NULL, out_channel_nb, frame->nb_samples,
						out_sample_fmt, 1);
					//写入文件
					fwrite(out_buffer, 1, out_buffer_size, fp_pcm);
					//av_freep(&out_buffer);
				}
				// 更新解码包的信息
				packet->data += ret;
				packet->size -= ret;  
			}
		}
		av_free_packet(packet);
	}

	fclose(fp_pcm);
	av_frame_free(&frame);
	av_free(out_buffer);
	swr_free(&swrCtx);
	avcodec_close(pCodeCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}
```

使用swr_convert重采样过程中，转码如果不改变采样率，只改变格式（fmt）和通道数，PCM音频正常，但是只要改变，出来的PCM就会有杂音。

解决方法：使用AVAudioFifo做分包处理。

三、aac音频解码实例（重采样+AVAudioFifo分包）

swr_convert重采样+AVAudioFifo做分包处理，消除上面不同采样率之间转换的噪音。

```cpp
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"  
#include "libavutil/opt.h"
#include "libavutil/audio_fifo.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"  
#include "libavutil/opt.h"
#include "libavutil/audio_fifo.h"
#ifdef __cplusplus
};
#endif
#endif

int m_is_first_audio_pts = 0;

int main (void)
{

	//1.注册组件
	av_register_all();
	//封装格式上下文
	AVFormatContext *pFormatCtx = avformat_alloc_context();

	//2.打开输入音频文件
	if (avformat_open_input(&pFormatCtx, "tdjm.aac", NULL, NULL) != 0) 
	{
		printf("%s", "打开输入音频文件失败");
		return -1;
	}
	//3.获取音频信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) 
	{
		printf("%s", "获取音频信息失败");
		return -1;
	}

	//音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
	int audio_stream_idx = -1;
	int i = 0;
	for (i=0; i < pFormatCtx->nb_streams; i++) 
	{
		//根据类型判断是否是音频流
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) 
		{
			audio_stream_idx = i;
			break;
		}
	}
	//4.获取解码器
	//根据索引拿到对应的流,根据流拿到解码器上下文
	AVCodecContext *pCodeCtx = pFormatCtx->streams[audio_stream_idx]->codec;
	//再根据上下文拿到编解码id，通过该id拿到解码器
	AVCodec *pCodec = avcodec_find_decoder(pCodeCtx->codec_id);
	if (pCodec == NULL) 
	{
		printf("%s", "无法解码");
		return -1;
	}
	//5.打开解码器
	if (avcodec_open2(pCodeCtx, pCodec, NULL) < 0) 
	{
		printf("%s", "编码器无法打开");
		return -1;
	}
	//编码数据
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//解压缩数据
	AVFrame *frame = av_frame_alloc();

	//frame->16bit 44100 PCM 统一音频采样格式与采样率
	SwrContext *swrCtx = swr_alloc();
	//重采样设置选项
	//输入的采样格式
	enum AVSampleFormat in_sample_fmt = pCodeCtx->sample_fmt;
	//输出的采样格式 16bit PCM
	enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	//输入的采样率
	int in_sample_rate = pCodeCtx->sample_rate;
	//输出的采样率
	int out_sample_rate = 8000;//pCodeCtx->sample_rate;
	//输入的声道布局
	uint64_t in_ch_layout = pCodeCtx->channel_layout;
	//输出的声道布局
	uint64_t out_ch_layout = AV_CH_LAYOUT_MONO;//AV_CH_LAYOUT_MONO，单通道；AV_CH_LAYOUT_STEREO,双声道

#if LIBSWRESAMPLE_VERSION_MINOR >= 17    // 根据版本不同，选用适当函数  
	/* set options */  
	av_opt_set_int(swrCtx, "in_channel_layout", in_ch_layout, 0);  
	av_opt_set_int(swrCtx, "in_sample_rate", in_sample_rate, 0);  
	av_opt_set_sample_fmt(swrCtx, "in_sample_fmt",in_sample_fmt, 0);  
	av_opt_set_int(swrCtx, "out_channel_layout",out_ch_layout, 0);  
	av_opt_set_int(swrCtx, "out_sample_rate", out_sample_rate, 0);  
	av_opt_set_sample_fmt(swrCtx, "out_sample_fmt",out_sample_fmt, 0); 
#else  
	swr_alloc_set_opts(swrCtx, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt,
		in_sample_rate, 0, NULL);
#endif
	swr_init(swrCtx);

	//获取输出的声道个数
	int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
	int out_framesize = 1024;//参考ffmpeg中音频格式的frame_size
	/*
	各种音频格式nb_samples和frame_size其他如下：
	case AV_CODEC_ID_ADPCM_ADX:    return   32;  
	case AV_CODEC_ID_ADPCM_IMA_QT: return   64;  
	case AV_CODEC_ID_ADPCM_EA_XAS: return  128;
	case AV_CODEC_ID_ATRAC3:       return 1024 * framecount;  
	case AV_CODEC_ID_ATRAC3P:      return 2048; 
	case AV_CODEC_ID_MUSEPACK7:    return 1152;  //mp3
	*/
	//存储pcm数据
	int out_size = (out_channel_nb*out_sample_rate*16)/8.0;		//1秒的数据大小
	uint8_t *out_buffer = (uint8_t *) av_malloc(out_size/2);
	FILE *fp_pcm = fopen("out.pcm", "wb");
	int ret, got_frame, framecount = 0;
	bool bAllocAudioFifo = false;
	int readFinished = 0;
	int nb_samplesByAllocAudioFifo = 0;
	AVAudioFifo *  audiofifo = av_audio_fifo_alloc(out_sample_fmt, out_channel_nb, 1);
	//6.一帧一帧读取压缩的音频数据AVPacket
	while (av_read_frame(pFormatCtx, packet) >= 0) 
	{
		if (packet->stream_index == audio_stream_idx) 
		{
			while(packet->size > 0)
			{
				ret = avcodec_decode_audio4(pCodeCtx, frame, &got_frame, packet);
				if (ret < 0) 
				{
					printf("%s", "解码出现错误");
					break;
				}
				//解码成功
				if (got_frame) 
				{
					//printf("解码%d帧\n", framecount++);
					int dst_nb_samples = av_rescale_rnd(swr_get_delay(swrCtx, pCodeCtx->sample_rate) + frame->nb_samples,
						out_sample_rate, pCodeCtx->sample_rate, AV_ROUND_UP);
					int convert_nb_samples = swr_convert(swrCtx, &out_buffer, dst_nb_samples,
						(const uint8_t**)frame->data, frame->nb_samples);
					if(!bAllocAudioFifo)
					{
						av_audio_fifo_realloc(audiofifo, av_audio_fifo_size(audiofifo) + frame->nb_samples);
						nb_samplesByAllocAudioFifo = frame->nb_samples;
					}
					else
					{
						if(frame->nb_samples > nb_samplesByAllocAudioFifo)
						{
							av_audio_fifo_free(audiofifo);
							audiofifo = av_audio_fifo_alloc(out_sample_fmt, out_channel_nb, frame->nb_samples);
							nb_samplesByAllocAudioFifo = frame->nb_samples;
						}
					}
					av_audio_fifo_write(audiofifo, (void **)&out_buffer, convert_nb_samples);
				}
				// 更新解码包的信息
				packet->data += ret;
				packet->size -= ret;  
			}
			//read from audio_fifo
			while (av_audio_fifo_size(audiofifo) >= out_framesize){

				int frame_size = FFMIN(av_audio_fifo_size(audiofifo), out_framesize);
				AVFrame* output_frame = NULL;
				output_frame = av_frame_alloc();
				output_frame->nb_samples = frame_size;
				output_frame->channel_layout = out_ch_layout;
				output_frame->format = out_sample_fmt;
				output_frame->sample_rate = 20000;

				av_frame_get_buffer(output_frame, 0);
				av_audio_fifo_read(audiofifo, (void **)output_frame->data, frame_size);

				//写入文件
				fwrite(output_frame->data[0], 1, output_frame->linesize[0], fp_pcm);
			}
		}
		av_free_packet(packet);
	}

	//read from audio_fifo
	while (av_audio_fifo_size(audiofifo) > 0 ){

		int frame_size = FFMIN(av_audio_fifo_size(audiofifo), out_framesize);
		AVFrame* output_frame = NULL;
		output_frame = av_frame_alloc();
		output_frame->nb_samples = frame_size;
		output_frame->channel_layout = out_ch_layout;
		output_frame->format = out_sample_fmt;
		output_frame->sample_rate = 20000;

		av_frame_get_buffer(output_frame, 0);
		av_audio_fifo_read(audiofifo, (void **)output_frame->data, frame_size);

		//写入文件
		fwrite(output_frame->data[0], 1, output_frame->linesize[0], fp_pcm);
	}

	fclose(fp_pcm);
	av_frame_free(&frame);
	av_free(out_buffer);
	swr_free(&swrCtx);
	avcodec_close(pCodeCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}
```

