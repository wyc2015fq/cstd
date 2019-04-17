// ffmpegdemo.cpp : �������̨Ӧ�ó������ڵ㡣
//
 
#include <stdio.h>
#include "libffmpeg.h"

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
 
#include "std/gui_c.h"

int test_decode_video2()
{
	//�ļ���ʽ������
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
	char* filepath = "input.mkv";
	filepath = "E:\\live\\test.mp4";
	FILE *fp_yuv = fopen("output.yuv", "wb+");
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) 
	{
		printf("Couldn't open input stream.\n");
		return -1;
	}
	//��ȡһ��������Ƶ���ݲ��һ��һЩ��ص���Ϣ
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) 
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
 
	//������Ƶ��������
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
 
	//�����������
	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	//���ҽ�����
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) 
	{
		printf("Codec not found.\n");
		return -1;
	}
	//�򿪽�����
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) 
	{
		printf("Could not open codec.\n");
		return -1;
	}
 
	//����AVFrame������ԭʼ��Ƶ
	pFrame = av_frame_alloc();
	//����AVFrame������yuv��Ƶ
	pFrameYUV = av_frame_alloc();

	AVPixelFormat fmt;
	fmt = AV_PIX_FMT_YUV420P;
	fmt = AV_PIX_FMT_BGR24;

	//�����ڴ棬����ͼ���ʽת��
	out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(fmt, pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, fmt, pCodecCtx->width, pCodecCtx->height, 1);
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//Output Info-----------------------------
	printf("--------------- File Information ----------------\n");
	//�ֹ����Ժ��������tbn��tbc��tbr��PAR��DAR�ĺ���
	av_dump_format(pFormatCtx, 0, filepath, 0);
	printf("-------------------------------------------------\n");
 
	//����ת��������
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
	pCodecCtx->width, pCodecCtx->height, fmt, SWS_BICUBIC, NULL, NULL, NULL);
 
	//��ȡ����
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
				//�ɹ�����һ֡
				sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameYUV->data, pFrameYUV->linesize);//ת��ͼ���ʽ
 
				if (fmt == AV_PIX_FMT_YUV420P) {
					y_size = pCodecCtx->width*pCodecCtx->height;
					fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);    //Y 
					fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
					fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V
				}
				if (fmt == AV_PIX_FMT_BGR24) {
					ShowImagePal("asdf", pCodecCtx->height, pCodecCtx->width, pFrameYUV->data[0], *pFrameYUV->linesize, 3, PixFmtMax, NULL);
					waitkey(10);
				}
				printf("Succeed to decode 1 frame!\n");

			}
			else
			{
				//δ���뵽һ֡������ʱ��βB֡���ӳ�֡���ں�����flush decoder����
			}
		}
		av_free_packet(packet);
	}
#define CODEC_CAP_DELAY 1
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