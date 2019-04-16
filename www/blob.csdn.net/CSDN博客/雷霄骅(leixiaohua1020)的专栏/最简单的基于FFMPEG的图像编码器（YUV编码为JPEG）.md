# 最简单的基于FFMPEG的图像编码器（YUV编码为JPEG） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年05月09日 00:25:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：81
个人分类：[FFMPEG																[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)](https://blog.csdn.net/leixiaohua1020/article/category/1360795)









伴随着毕业论文的完成，这两天终于腾出了空闲，又有时间搞搞FFMPEG的研究了。想着之前一直搞的都是FFMPEG解码方面的工作，很少涉及到FFMPEG编码方面的东西，于是打算研究一下FFMPEG的编码。在网上看了一些例子，发现要不然是难度略微有些大，要不然就是类库比较陈旧，于是就决定自己做一个编码方面的例子，方便以后学习。

本文的编码器实现了YUV420P的数据编码为JPEG图片。本着简单的原则，代码基本上精简到了极限。使用了2014年5月6号编译的最新的FFMPEG类库。

程序很简单，打开工程后直接运行即可将YUV数据编码为JPEG。本程序十分灵活，可以根据需要修改成编码各种图像格式的编码器，比如PNG，GIF等等。

平台使用VC2010。

下面直接上代码。比较重要的地方都有注释。



```cpp
/* 
 *最简单的基于FFmpeg的图像编码器
 *Simplest FFmpeg Picture Encoder
 *
 *雷霄骅 Lei Xiaohua
 *leixiaohua1020@126.com
 *中国传媒大学/数字电视技术
 *Communication University of China / Digital TV Technology
 *http://blog.csdn.net/leixiaohua1020
 *
 *本程序实现了YUV420P像素数据编码为JPEG图片。是最简单的FFmpeg编码方面的教程。
 *通过学习本例子可以了解FFmpeg的编码流程。
 *This software encode YUV420P data to JPEG format file.It's the simplest encode software based on FFmpeg.
 *Suitable for beginner of FFmpeg
 */

#include "stdafx.h"

extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>
};

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

	FILE *in_file = fopen("cuc_view_480x272.yuv", "rb");	//视频YUV源文件 
	int in_w=480,in_h=272;									//宽高
	const char* out_file = "cuc_view_encode.jpg";					//输出文件路径

	av_register_all();

	//方法1.组合使用几个函数
	pFormatCtx = avformat_alloc_context();
	//猜格式。用MJPEG编码
	fmt = av_guess_format("mjpeg", NULL, NULL);
	pFormatCtx->oformat = fmt;
	//注意：输出路径
	if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("输出文件打开失败");
		return -1;
	}

	//方法2.更加自动化一些
	//avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	//fmt = pFormatCtx->oformat;

	video_st = av_new_stream(pFormatCtx, 0);
	if (video_st==NULL)
	{
		return -1;
	}
	pCodecCtx = video_st->codec;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = PIX_FMT_YUVJ420P;

	pCodecCtx->width = in_w;  
	pCodecCtx->height = in_h;

	pCodecCtx->time_base.num = 1;  
	pCodecCtx->time_base.den = 25;   
	//输出格式信息
	av_dump_format(pFormatCtx, 0, out_file, 1);

	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("没有找到合适的编码器！");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
	{
		printf("编码器打开失败！");
		return -1;
	}
	picture = avcodec_alloc_frame();
	size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(size);
	if (!picture_buf)
	{
		return -1;
	}
	avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

	//写文件头
	avformat_write_header(pFormatCtx,NULL);

	AVPacket pkt;
	int y_size = pCodecCtx->width * pCodecCtx->height;
	av_new_packet(&pkt,y_size*3);
	//读入YUV
	if (fread(picture_buf, 1, y_size*3/2, in_file) < 0)
	{
		printf("文件读取错误");
		return -1;
	}
	picture->data[0] = picture_buf;  // 亮度Y
	picture->data[1] = picture_buf+ y_size;  // U 
	picture->data[2] = picture_buf+ y_size*5/4; // V
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
		pkt.stream_index = video_st->index;
		ret = av_write_frame(pFormatCtx, &pkt);
	}

	av_free_packet(&pkt);
	//写文件尾
	av_write_trailer(pFormatCtx);

	printf("编码成功！\n");

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


编码前的YUV420P数据：



![](https://img-blog.csdn.net/20140509002339531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

编码后的JPEG：

![](https://img-blog.csdn.net/20140509002355765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

完整工程下载地址：

[http://download.csdn.net/detail/leixiaohua1020/7319265](http://download.csdn.net/detail/leixiaohua1020/7319265)

SourceForge项目地址：

[https://sourceforge.net/projects/simplestffmpegpictureencoder/](https://sourceforge.net/projects/simplestffmpegpictureencoder/)





