# ffmpeg视频编解码实例 - 深之JohnChen的专栏 - CSDN博客

2018年06月12日 12:29:48[byxdaz](https://me.csdn.net/byxdaz)阅读数：1833



视频解码

```cpp
// ffmpegdemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32

//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};
#else

//Linux...
#ifdef __cplusplus
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#endif

int main()
{
	//文件格式上下文
	AVFormatContext	*pFormatCtx;
	int		i = 0, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame, *pFrameYUV;
	unsigned char *out_buffer;
	AVPacket *packet;

	int y_size;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
	char filepath[] = "input.mkv";
	FILE *fp_yuv = fopen("output.yuv", "wb+");
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) 
	{
		printf("Couldn't open input stream.\n");
		return -1;
	}
	//读取一部分视音频数据并且获得一些相关的信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) 
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}

	//查找视频编码索引
	videoindex = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}

	if (videoindex == -1)
	{
		printf("Didn't find a video stream.\n");
		return -1;
	}

	//编解码上下文
	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	//查找解码器
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) 
	{
		printf("Codec not found.\n");
		return -1;
	}
	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) 
	{
		printf("Could not open codec.\n");
		return -1;
	}

	//申请AVFrame，用于原始视频
	pFrame = av_frame_alloc();
	//申请AVFrame，用于yuv视频
	pFrameYUV = av_frame_alloc();
	//分配内存，用于图像格式转换
	out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//Output Info-----------------------------
	printf("--------------- File Information ----------------\n");
	//手工调试函数，输出tbn、tbc、tbr、PAR、DAR的含义
	av_dump_format(pFormatCtx, 0, filepath, 0);
	printf("-------------------------------------------------\n");

	//申请转换上下文
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
	pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	//读取数据
	while (av_read_frame(pFormatCtx, packet) >= 0) 
	{
		if (packet->stream_index == videoindex) 
		{
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if (ret < 0) 
			{
				printf("Decode Error.\n");
				return -1;
			}

			if (got_picture >= 1) 
			{
				//成功解码一帧
				sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameYUV->data, pFrameYUV->linesize);//转换图像格式

				y_size = pCodecCtx->width*pCodecCtx->height;
				fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);    //Y 
				fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
				fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V
				printf("Succeed to decode 1 frame!\n");
			}
			else
			{
				//未解码到一帧，可能时结尾B帧或延迟帧，在后面做flush decoder处理
			}
		}
		av_free_packet(packet);
	}

	//flush decoder
	//FIX: Flush Frames remained in Codec
	while (true) 
	{
		if (!(pCodec->capabilities & CODEC_CAP_DELAY))
			return 0;

		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if (ret < 0)
		{
			break;
		}
		if (!got_picture)
		{
			break;
		}

		sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
			pFrameYUV->data, pFrameYUV->linesize);

		int y_size = pCodecCtx->width*pCodecCtx->height;
		fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);    //Y 
		fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
		fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V
		printf("Flush Decoder: Succeed to decode 1 frame!\n");
	}

	sws_freeContext(img_convert_ctx);
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	fclose(fp_yuv);

    return 0;
}
```

视频编码

```cpp
// ffmpegdemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32

//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};
#else

//Linux...
#ifdef __cplusplus
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#endif

int flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index) 
{
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &CODEC_CAP_DELAY))
	{
		return 0;
	}
	while (true) 
	{
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
		{
			break;
		}
		if (!got_frame)
		{
			ret = 0;
			break;
		}
		printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt.size);
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
		{
			break;
		}
	}

	return ret;
}

int main()
{
	AVFormatContext* pFormatCtx;
	AVOutputFormat* pOutputFmt;
	AVStream* video_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;
	AVPacket pkt;
	uint8_t* picture_buf;
	AVFrame* pFrame;
	int picture_size;
	int y_size;
	int framecnt = 0;
	int in_w = 640, in_h = 272;					//视频图像宽高
	const char* out_file = "output.h264";

	int i, j;
	int num;
	int index_y, index_u, index_v;
	uint8_t *y_, *u_, *v_, *in;

	int got_picture = 0;
	int ret;

	av_register_all();
	//alloc avformat context
	pFormatCtx = avformat_alloc_context();
	//猜测类型,返回输出类型。
	pOutputFmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = pOutputFmt;
	//打开FFmpeg的输入输出文件,成功之后创建的AVIOContext结构体。
	if (avio_open2(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE,NULL,NULL) < 0) 
	{
		//Failed to open output file
		return -1;
	}

	//除了以下方法，另外还可以使用avcodec_find_encoder_by_name()来获取AVCodec  
	pCodec = avcodec_find_encoder(pOutputFmt->video_codec);//获取编码器  
	if (!pCodec) 
	{
		//cannot find encoder  
		return -1;
	}

	pCodecCtx = avcodec_alloc_context3(pCodec);//申请AVCodecContext，并初始化。  
	if (!pCodecCtx) 
	{
		//failed get AVCodecContext  
		return -1;
	}

	FILE * in_file = fopen("640-272.yuv", "rb");   //打开原始yuv数据文件

	/* 创建输出码流的AVStream */
	video_st = avformat_new_stream(pFormatCtx, 0);
	video_st->time_base.num = 1;
	video_st->time_base.den = 25;
	if (video_st == NULL) 
	{
		return -1;
	}
	//Param that must set  
	pCodecCtx = video_st->codec;
	//pCodecCtx->codec_id =AV_CODEC_ID_HEVC;  //H265
	pCodecCtx->codec_id = AV_CODEC_ID_H264;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
	pCodecCtx->width = in_w;
	pCodecCtx->height = in_h;
	pCodecCtx->b_frame_strategy = true;
	/*
	码率
	bit_rate/-bt tolerance 设置视频码率容忍度kbit/s （固定误差）
	rc_max_rate/-maxrate bitrate设置最大视频码率容忍度 （可变误差）
	rc_min_rate/-minrate bitreate 设置最小视频码率容忍度（可变误差）
	rc_buffer_size/-bufsize size 设置码率控制缓冲区大小

	如何设置固定码率编码 ?
	bit_rate是平均码率，不一定能控制住
	c->bit_rate = 400000;
	c->rc_max_rate = 400000;
	c->rc_min_rate = 400000;
	提示  [libx264 @ 00c70be0] VBV maxrate specified, but no bufsize, ignored
	再设置  c->rc_buffer_size = 200000;  即可。如此控制后编码质量明显差了。
	*/
	pCodecCtx->bit_rate = 400000;				//采样码率越大，目标文件越大
	//pCodecCtx->bit_rate_tolerance = 8000000; // 码率误差，允许的误差越大，视频越小
	//两个I帧之间的间隔
	pCodecCtx->gop_size = 15;

	//编码帧率，每秒多少帧。下面表示1秒25帧
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;

	//最小的量化因子
	pCodecCtx->qmin = 10;
	//最大的量化因子
	pCodecCtx->qmax = 30;

	//最大B帧数
	pCodecCtx->max_b_frames = 3;

	// Set Option  
	AVDictionary *param = 0;
	//H.264  
	if (pCodecCtx->codec_id == AV_CODEC_ID_H264) 
	{
		/*
		preset的参数主要调节编码速度和质量的平衡，有ultrafast、superfast、veryfast、faster、fast、medium、slow、slower、veryslow、placebo这10个选项，从快到慢。
		*/
		av_dict_set(¶m, "preset", "fast", 0);
		/* 
		tune的参数主要配合视频类型和视觉优化的参数。
		tune的值有： film：  电影、真人类型；
		animation：  动画；
		grain：      需要保留大量的grain时用；
		stillimage：  静态图像编码时使用；
		psnr：      为提高psnr做了优化的参数；
		ssim：      为提高ssim做了优化的参数；
		fastdecode： 可以快速解码的参数；
		zerolatency：零延迟，用在需要非常低的延迟的情况下，比如电视电话会议的编码。
		*/
		av_dict_set(¶m, "tune", "zerolatency", 0);
		/*
		画质,分别是baseline, extended, main, high
		1、Baseline Profile：基本画质。支持I/P 帧，只支持无交错（Progressive）和CAVLC；
		2、Extended profile：进阶画质。支持I/P/B/SP/SI 帧，只支持无交错（Progressive）和CAVLC；(用的少)
		3、Main profile：主流画质。提供I/P/B 帧，支持无交错（Progressive）和交错（Interlaced）， 也支持CAVLC 和CABAC 的支持；
		4、High profile：高级画质。在main Profile 的基础上增加了8x8内部预测、自定义量化、 无损视频编码和更多的YUV 格式；
　　	H.264 Baseline profile、Extended profile和Main profile都是针对8位样本数据、4:2:0格式(YUV)的视频序列。在相同配置情况下，
		High profile（HP）可以比Main profile（MP）降低10%的码率。 根据应用领域的不同，Baseline profile多应用于实时通信领域，
		Main profile多应用于流媒体领域，High profile则多应用于广电和存储领域。
		*/
		//av_dict_set(¶m, "profile", "main", 0);  
	}
	else if (pCodecCtx->codec_id == AV_CODEC_ID_H265 || pCodecCtx->codec_id == AV_CODEC_ID_HEVC)
	{
		av_dict_set(¶m, "preset", "fast", 0);
		av_dict_set(¶m, "tune", "zerolatency", 0);
	}

	//Output Info-----------------------------
	printf("--------------- out_file Information ----------------\n");
	//手工调试函数，输出tbn、tbc、tbr、PAR、DAR的含义
	av_dump_format(pFormatCtx, 0, out_file, 1);	//最后一个参数，如果是输出文件时，该值为1；如果是输入文件时，该值为0
	printf("-----------------------------------------------------\n");

	/* 查找编码器 */
	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec) 
	{
		printf("Can not find encoder! \n");
		return -1;
	}

	/* 打开编码器 */
	if (avcodec_open2(pCodecCtx, pCodec,¶m) < 0) 
	{
		printf("Failed to open encoder! \n");
		return -1;
	}

	pFrame = av_frame_alloc();
	picture_size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(picture_size);
	avpicture_fill((AVPicture *)pFrame, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

	//Write File Header  
	/* 写文件头（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS） */
	avformat_write_header(pFormatCtx, NULL);

	/* Allocate the payload of a packet and initialize its fields with default values.  */
	av_new_packet(&pkt, picture_size);

	j = 1;
	y_size = pCodecCtx->width * pCodecCtx->height;
	unsigned int uiReadSize = 0;
	while (true)
	{
		//Read raw YUV data
		if (feof(in_file))	//文件结束
		{
			break;
		}
		else
		{
			uiReadSize = fread(picture_buf, 1, y_size * 3 / 2, in_file);
			if (uiReadSize <= 0)
			{
				printf("Failed to read raw data! \n");
				break;
			}
			else if (uiReadSize < y_size * 3 / 2)
			{
				//读取数据不满足一帧YUV数据，表示数据读取完
				break;
			}
		}

		pFrame->data[0] = picture_buf;					 // Y  
		pFrame->data[1] = picture_buf + y_size;			 // U   
		pFrame->data[2] = picture_buf + y_size * 5 / 4;  // V  

		//PTS  
		pFrame->pts = j*(video_st->time_base.den) / ((video_st->time_base.num) * 25);

		//Encode  
		/* 编码一帧视频。即将AVFrame（存储YUV像素数据）编码为AVPacket（存储H.264等格式的码流数据） */
		int ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);
		if (ret < 0)
		{
			printf("Failed to encode! \n");
			return -1;
		}
		if (got_picture == 1)
		{
			printf("Succeed to encode frame: %5d\tsize:%5d\n",framecnt,pkt.size);  
			framecnt++;
			pkt.stream_index = video_st->index;

			/* 将编码后的视频码流写入文件 */
			ret = av_write_frame(pFormatCtx, &pkt);

			av_free_packet(&pkt);
		}

		j++;
	}

	//Flush Encoder  
	/* 输入的像素数据读取完成后调用此函数。用于输出编码器中剩余的AVPacket */
	ret = flush_encoder(pFormatCtx, 0);
	if (ret < 0) 
	{
		printf("Flushing encoder failed\n");
		return -1;
	}

	//Write file trailer  
	/* 写文件尾（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS） */
	av_write_trailer(pFormatCtx);

	//Clean  
	if (video_st) 
	{
		avcodec_close(video_st->codec);
		av_free(pFrame);
		av_free(picture_buf);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);
	av_free_packet(&pkt);

	fclose(in_file);

	return 0;
}
```

代码下载：[点击打开链接](https://download.csdn.net/download/byxdaz/10473843)

注意：
1、vs编译工具使用最新的ffmpeg库，ffmpeg调用avcodec库直接崩溃在avcodec_register_all处，不调用avcodec_register_all直接调用av_frame_alloc，也会崩溃。
解决方法：
工程属性：链接器-优化-引用的值由“是 (/OPT:REF)”改为“否 (/OPT:NOREF)”

2、ffmpeg视频编码参数，参考[https://blog.csdn.net/byxdaz/article/details/80663718](https://blog.csdn.net/byxdaz/article/details/80663718)

[](https://blog.csdn.net/byxdaz/article/details/80663718)
参考资料：
https://blog.csdn.net/leixiaohua1020/article/details/25430425/

