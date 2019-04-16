# 最简单的基于FFmpeg的视频编码器-更新版（YUV编码为HEVC(H.265)） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年10月04日 14:12:45[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：46480
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)











=====================================================


最简单的基于FFmpeg的视频编码器文章列表：


[最简单的基于FFMPEG的视频编码器（YUV编码为H.264）](http://blog.csdn.net/leixiaohua1020/article/details/25430425)


[最简单的基于FFmpeg的视频编码器-更新版（YUV编码为HEVC(H.265)）](http://blog.csdn.net/leixiaohua1020/article/details/39770947)


[最简单的基于FFmpeg的编码器-纯净版（不包含libavformat）](http://blog.csdn.net/leixiaohua1020/article/details/42181271)


=====================================================


前一阵子做过一个基于FFmpeg的视频编码器的例子：
[最简单的基于FFMPEG的视频编码器（YUV编码为H.264）](http://blog.csdn.net/leixiaohua1020/article/details/25430425)
在该例子中，可以将YUV像素数据（YUV420P）编码为H.264码流。因为如今FFmpeg已经实现了对libx265的支持，因此对上述编码H.264的例子进行了升级，使之变成编码H.265（HEVC）的例子。

比较早的FFmpeg的类库（大约几个月以前的版本，我这里编译时间是2014.05.06）对H.265的编码支持有问题。开始调试的时候，以为是自己的代码有问题，几经修改也没有找到解决方法。最终发现是类库本身的问题，更换新版本的类库（我这里编译时间是2014.09.16）后问题解决。




## 流程

下面附上一张FFmpeg编码视频的流程图。通过该流程，不仅可以编码H.264/H.265的码流，而且可以编码MPEG4/MPEG2/VP9/VP8等多种码流。实际上使用FFmpeg编码视频的方式都是一样的。图中蓝色背景的函数是实际输出数据的函数。浅绿色的函数是视频编码的函数。




![](https://img-blog.csdn.net/20141004022957438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


简单介绍一下流程中各个函数的意义（上一篇YUV编码为H.264的文章中已经写过一遍，这里复制粘贴一下）：

> av_register_all()：注册FFmpeg所有编解码器。
avformat_alloc_output_context2()：初始化输出码流的AVFormatContext。
avio_open()：打开输出文件。
av_new_stream()：创建输出码流的AVStream。
avcodec_find_encoder()：查找编码器。
avcodec_open2()：打开编码器。
avformat_write_header()：写文件头（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）。
avcodec_encode_video2()：编码一帧视频。即将AVFrame（存储YUV像素数据）编码为AVPacket（存储H.264等格式的码流数据）。
av_write_frame()：将编码后的视频码流写入文件。
flush_encoder()：输入的像素数据读取完成后调用此函数。用于输出编码器中剩余的AVPacket。
av_write_trailer()：写文件尾（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）。



## 代码
下面直接贴上代码

```cpp
/**
 * 最简单的基于FFmpeg的视频编码器
 * Simplest FFmpeg Video Encoder
 * 
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 * 
 * 本程序实现了YUV像素数据编码为视频码流（HEVC(H.265)，H264，MPEG2，VP8等等）。
 * 是最简单的FFmpeg视频编码方面的教程。
 * 通过学习本例子可以了解FFmpeg的编码流程。
 * This software encode YUV420P data to HEVC(H.265) bitstream (or
 * H.264, MPEG2, VP8 etc.).
 * It's the simplest video encoding software based on FFmpeg. 
 * Suitable for beginner of FFmpeg 
 */

#include <stdio.h>

extern "C"
{
#include "libavutil\opt.h"
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
		if (!got_frame){
			ret=0;
			break;
		}
		printf("Succeed to encode 1 frame! 编码成功1帧！\n");
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
	AVStream* video_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;

	uint8_t* picture_buf;
	AVFrame* picture;
	int size;

	//FILE *in_file = fopen("src01_480x272.yuv", "rb");	//Input YUV data 视频YUV源文件 
	FILE *in_file = fopen("ds_480x272.yuv", "rb");	//Input YUV data 视频YUV源文件 
	int in_w=480,in_h=272;//宽高	
	//Frames to encode
	int framenum=100;
	//const char* out_file = "src01.h264";	//Output Filepath 输出文件路径
	//const char* out_file = "src01.ts";
	//const char* out_file = "src01.hevc";
	const char* out_file = "ds.hevc";

	av_register_all();
	//Method1 方法1.组合使用几个函数
	pFormatCtx = avformat_alloc_context();
	//Guess Format 猜格式
	fmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = fmt;
	
	//Method 2 方法2.更加自动化一些
	//avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	//fmt = pFormatCtx->oformat;


	//Output Format 注意输出路径
	if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("Failed to open output file! 输出文件打开失败");
		return -1;
	}

	video_st = avformat_new_stream(pFormatCtx, 0);
	video_st->time_base.num = 1; 
	video_st->time_base.den = 25;  

	if (video_st==NULL)
	{
		return -1;
	}
	//Param that must set
	pCodecCtx = video_st->codec;
	//pCodecCtx->codec_id =AV_CODEC_ID_HEVC;
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
	//pCodecCtx->qcompress = 0.6;
	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;

	//Optional Param
	pCodecCtx->max_b_frames=3;

	// Set Option
	AVDictionary *param = 0;
	//H.264
	if(pCodecCtx->codec_id == AV_CODEC_ID_H264) {
		av_dict_set(?m, "preset", "slow", 0);
		av_dict_set(?m, "tune", "zerolatency", 0);
	}
	//H.265
	if(pCodecCtx->codec_id == AV_CODEC_ID_H265){
		av_dict_set(?m, "x265-params", "qp=20", 0);
		av_dict_set(?m, "preset", "ultrafast", 0);
		av_dict_set(?m, "tune", "zero-latency", 0);
	}

	//Dump Information 输出格式信息
	av_dump_format(pFormatCtx, 0, out_file, 1);

	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec){
		printf("Can not find encoder! 没有找到合适的编码器！\n");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec,?m) < 0){
		printf("Failed to open encoder! 编码器打开失败！\n");
		return -1;
	}
	


	picture = avcodec_alloc_frame();
	size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(size);
	avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

	//Write File Header 写文件头
	avformat_write_header(pFormatCtx,NULL);

	AVPacket pkt;
	int y_size = pCodecCtx->width * pCodecCtx->height;
	av_new_packet(&pkt,y_size*3);

	for (int i=0; i<framenum; i++){
		//Read YUV 读入YUV
		if (fread(picture_buf, 1, y_size*3/2, in_file) < 0){
			printf("Failed to read YUV data! 文件读取错误\n");
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
		//Encode 编码
		int ret = avcodec_encode_video2(pCodecCtx, &pkt,picture, &got_picture);
		if(ret < 0){
			printf("Failed to encode! 编码错误！\n");
			return -1;
		}
		if (got_picture==1){
			printf("Succeed to encode 1 frame! 编码成功1帧！\n");
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

	//Write file trailer 写文件尾
	av_write_trailer(pFormatCtx);

	//Clean 清理
	if (video_st){
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



## 结果

软件运行截图（受限于文件体积，原始YUV帧数只有100帧）：

![](https://img-blog.csdn.net/20141004023001510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这次换了个有趣点的YUV序列。之前总是看YUV标准测试序列都已经看烦了，这次换个电视剧里的序列相对更加生动一些。YUV序列如下图所示。

![](https://img-blog.csdn.net/20141004022836031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
编码后的HEVC（H.265）码流：

![](https://img-blog.csdn.net/20141004023009824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 下载




**Simplest ffmpeg video encoder**

****

**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestffmpegvideoencoder/](https://sourceforge.net/projects/simplestffmpegvideoencoder/)

Github：[https://github.com/leixiaohua1020/simplest_ffmpeg_video_encoder](https://github.com/leixiaohua1020/simplest_ffmpeg_video_encoder)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_encoder](http://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_encoder)
[](https://sourceforge.net/projects/simplestffmpegvideoencoder/)

CSDN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8001515](http://download.csdn.net/detail/leixiaohua1020/8001515)

本程序实现了YUV像素数据编码为视频码流（H.265，H264，MPEG2，VP8等等）。是最简单的FFmpeg视频编码方面的教程。
它包含以下两个子项目：
simplest_ffmpeg_video_encoder：最简单的基于FFmpeg的视频编码器。使用libavcodec和libavformat编码并且封装视频。
simplest_ffmpeg_video_encoder_pure：最简单的基于FFmpeg的视频编码器-纯净版。仅使用libavcodec编码视频，不使用libavformat。


**更新-1.1 (2015.1.03)=========================================**

增加了《最简单的基于FFmpeg的编码器-纯净版（不包含libavformat）》中的simplest_ffmpeg_video_encoder_pure工程。

CSDN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8322003](http://download.csdn.net/detail/leixiaohua1020/8322003)


**更新-1.2 (2015.2.13)=========================================**



这次考虑到了跨平台的要求，调整了源代码。经过这次调整之后，源代码可以在以下平台编译通过：

> 
*VC++：打开sln文件即可编译，无需配置。*


> 
*cl.exe：打开compile_cl.bat即可命令行下使用cl.exe进行编译，注意可能需要按照VC的安装路径调整脚本里面的参数。编译命令如下。*




```
::VS2010 Environment
call "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
::include
@set INCLUDE=include;%INCLUDE%
::lib
@set LIB=lib;%LIB%
::compile and link
cl simplest_ffmpeg_video_encoder.cpp /link avcodec.lib avformat.lib avutil.lib ^
avdevice.lib avfilter.lib postproc.lib swresample.lib swscale.lib /OPT:NOREF
```



> 
*MinGW：MinGW命令行下运行compile_mingw.sh即可使用MinGW的g++进行编译。编译命令如下。*




```
g++ simplest_ffmpeg_video_encoder.cpp -g -o simplest_ffmpeg_video_encoder.exe \
-I /usr/local/include -L /usr/local/lib \
-lavformat -lavcodec -lavutil
```



> 
*GCC：Linux或者MacOS命令行下运行compile_gcc.sh即可使用GCC进行编译。编译命令如下。*




```
gcc simplest_ffmpeg_video_encoder.cpp -g -o simplest_ffmpeg_video_encoder.out \
-I /usr/local/include -L /usr/local/lib -lavformat -lavcodec -lavutil
```



PS：相关的编译命令已经保存到了工程文件夹中


CSDN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8444967](http://download.csdn.net/detail/leixiaohua1020/8444967)

SourceForge上已经更新。








