# 最简单的基于FFMPEG的视频编码器（YUV编码为H.264） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年05月12日 00:42:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：63
个人分类：[FFMPEG																[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)](https://blog.csdn.net/leixiaohua1020/article/category/1360795)









本文介绍一个最简单的基于FFMPEG的视频编码器。该编码器实现了YUV420P的像素数据编码为H.264的压缩编码数据。编码器代码十分简单，但是每一行代码都很重要，适合好好研究一下。弄清楚了本代码也就基本弄清楚了FFMPEG的编码流程。目前我虽然已经调通了程序，但是还是有些地方没有完全搞明白，需要下一步继续探究然后补充内容。

本程序使用最新版的类库（编译时间为2014.5.6），开发平台为VC2010。所有的配置都已经做好，只需要运行就可以了。

下面直接上代码：



```cpp
/* 
 *最简单的基于FFmpeg的视频编码器
 *Simplest FFmpeg Video Encoder
 *
 *雷霄骅 Lei Xiaohua
 *leixiaohua1020@126.com
 *中国传媒大学/数字电视技术
 *Communication University of China / Digital TV Technology
 *http://blog.csdn.net/leixiaohua1020
 *
 *本程序实现了YUV像素数据编码为视频码流（H264，MPEG2，VP8等等）。
 *是最简单的FFmpeg视频编码方面的教程。
 *通过学习本例子可以了解FFmpeg的编码流程。
 *This software encode YUV420P data to H.264 bitstream.
 *It's the simplest video encoding software based on FFmpeg. 
 *Suitable for beginner of FFmpeg 
 */

#include "stdafx.h"

extern "C"
{
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
};


int flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index)
{
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
		CODEC_CAP_DELAY))
		return 0;
	while (1) {
		printf("Flushing stream #%u encoder\n", stream_index);
		//ret = encode_write_frame(NULL, stream_index, &got_frame);
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2 (fmt_ctx->streams[stream_index]->codec, &enc_pkt,
			NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame)
		{ret=0;break;}
		printf("编码成功1帧！\n");
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
			break;
	}
	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	AVFormatContext* pFormatCtx;
	AVOutputFormat* fmt;
	AVStream* video_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;

	uint8_t* picture_buf;
	AVFrame* picture;
	int size;

	FILE *in_file = fopen("src01_480x272.yuv", "rb");	//视频YUV源文件 
	int in_w=480,in_h=272;//宽高	
	int framenum=50;
	const char* out_file = "src01.h264";					//输出文件路径

	av_register_all();
	//方法1.组合使用几个函数
	pFormatCtx = avformat_alloc_context();
	//猜格式
	fmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = fmt;
	
	//方法2.更加自动化一些
	//avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	//fmt = pFormatCtx->oformat;


	//注意输出路径
	if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("输出文件打开失败");
		return -1;
	}

	video_st = av_new_stream(pFormatCtx, 0);
	if (video_st==NULL)
	{
		return -1;
	}
	pCodecCtx = video_st->codec;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = PIX_FMT_YUV420P;
	pCodecCtx->width = in_w;  
	pCodecCtx->height = in_h;
	pCodecCtx->time_base.num = 1;  
	pCodecCtx->time_base.den = 25;  
	pCodecCtx->bit_rate = 400000;  
	pCodecCtx->gop_size=250;
	//H264
	//pCodecCtx->me_range = 16;
	//pCodecCtx->max_qdiff = 4;
	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;
	//pCodecCtx->qcompress = 0.6;
	//输出格式信息
	av_dump_format(pFormatCtx, 0, out_file, 1);

	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("没有找到合适的编码器！\n");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
	{
		printf("编码器打开失败！\n");
		return -1;
	}
	picture = avcodec_alloc_frame();
	size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(size);
	avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

	//写文件头
	avformat_write_header(pFormatCtx,NULL);

	AVPacket pkt;
	int y_size = pCodecCtx->width * pCodecCtx->height;
	av_new_packet(&pkt,y_size*3);

	for (int i=0; i<framenum; i++){
		//读入YUV
		if (fread(picture_buf, 1, y_size*3/2, in_file) < 0)
		{
			printf("文件读取错误\n");
			return -1;
		}else if(feof(in_file)){
			break;
		}
		picture->data[0] = picture_buf;  // 亮度Y
		picture->data[1] = picture_buf+ y_size;  // U 
		picture->data[2] = picture_buf+ y_size*5/4; // V
		//PTS
		picture->pts=i;
		int got_picture=0;
		//编码
		int ret = avcodec_encode_video2(pCodecCtx, &pkt,picture, &got_picture);
		if(ret < 0)
		{
			printf("编码错误！\n");
			return -1;
		}
		if (got_picture==1)
		{
			printf("编码成功1帧！\n");
			pkt.stream_index = video_st->index;
			ret = av_write_frame(pFormatCtx, &pkt);
			av_free_packet(&pkt);
		}
	}
	//Flush Encoder
	int ret = flush_encoder(pFormatCtx,0);
	if (ret < 0) {
		printf("Flushing encoder failed\n");
		return -1;
	}

	//写文件尾
	av_write_trailer(pFormatCtx);

	//清理
	if (video_st)
	{
		avcodec_close(video_st->codec);
		av_free(picture);
		av_free(picture_buf);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	fclose(in_file);

	return 0;
}
```





软件运行截图（受限于文件体积，原始YUV帧数很少）：

![](https://img-blog.csdn.net/20140512004021078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

编码前的YUV序列：

![](https://img-blog.csdn.net/20140512003916546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

编码后的H.264码流：

![](https://img-blog.csdn.net/20140512003920687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下载地址：

[http://download.csdn.net/detail/leixiaohua1020/7324115](http://download.csdn.net/detail/leixiaohua1020/7324115)



修正记录：

之前发现编码后的H.264码流与YUV输入的帧数不同。经过观察对比其他程序后发现需要调用flush_encoder()将编码器中剩余的视频帧输出。已经将该问题修正。

下载地址（修正后）：

[http://download.csdn.net/detail/leixiaohua1020/7466649](http://download.csdn.net/detail/leixiaohua1020/7466649)

SourceForge上已经更新。



SourceForge项目地址：

[https://sourceforge.net/projects/simplestffmpegvideoencoder/](https://sourceforge.net/projects/simplestffmpegvideoencoder/)





