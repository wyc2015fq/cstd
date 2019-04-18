# FFmpeg filter - 深之JohnChen的专栏 - CSDN博客

2018年06月10日 17:08:44[byxdaz](https://me.csdn.net/byxdaz)阅读数：723


一、FFmpeg filter简介

FFmpeg中的libavfilter提供了一整套的基于filter的机制。filter本身是一个插件的形式，可以快速的组装需要的效果。
比如下面的filter，可以实现视频的水平镜像效果。
ffplay.exe sample.rmvb -vf hflip

FFmpeg定义的libavcodec接口已经成为在编解码领域的事实上的行业标准。但音视频filter并没有类似的标准，多个不同的多媒体项目（比如MPlayer、Xine、GStreamer等）都实现了自定义的filter系统。为了统一filter库API接口，FFmpeg提出了参考DirectDraw实现了高质量、高效、灵活的音视频filter接口。详细的文档资料可以参考[FFmpeg filter](http://ffmpeg.org/ffmpeg-filters.html)。

filter的分类

按照处理数据的类型，通常多媒体的filter分为：

·                     音频filter

·                     视频filter

·                     字幕filter

FFmpeg中filter分为：

·                     source filter （只有输出）

·                     audio filter

·                     video filter

·                     Multimedia filter

·                     sink filter （只有输入）

除了source和sink filter，其他filter都至少有一个输入、至少一个输出。

介绍了这么多，下面也是一个例子，使用filter实现宽高减半显示：

ffplay.exe sample.rmvb -vf scale=iw/2:ih/2

FFmpeg filter可以认为是一些预定义的范式，可以实现类似积木的多种功能的自由组合。每个filter都有固定数目的输入和输出，而且实际使用中不允许有空悬的输入输出端。使用文本描述时我们可以通过标识符指定输入和输出端口，将不同filter串联起来，构成更复杂的filter。这就形成了嵌套的filter。当然每个filter可以通过ffmpeg/ffplay命令行实现，但通常filter更方便。

ffmpeg.exe、ffplay.exe能够通过filter处理原始的音视频数据。ffmpeg将filtergraph分为simple filtergraph和complex filtergraph。通常simple filtergraph只有一个输入和输出，ffmpeg命令行中使用-vf、-af识别，基本原理图如下：

```
_________                        ______________
    |         |                      |              |
    | decoded |                      | encoded data |
    | frames  |\                   _ | packets      |
    |_________| \                  /||______________|
                 \   __________   /
      simple     _\||          | /  encoder
      filtergraph   | filtered |/
                    | frames   |
                    |__________|
```

complex filtergraph，通常是具有多个输入输出文件，并有多条执行路径；ffmpeg命令行中使用-lavfi、-filter_complex，基本原理图如下：

```
_________
|         |
| input 0 |\                    __________
|_________| \                  |          |
             \   _________    /| output 0 |
              \ |         |  / |__________|
 _________     \| complex | /
|         |     |         |/
| input 1 |---->| filter  |\
|_________|     |         | \   __________
               /| graph   |  \ |          |
              / |         |   \| output 1 |
 _________   /  |_________|    |__________|
|         | /
| input 2 |/
```

FFmpeg中filter包含三个层次，filter->filterchain->filtergraph。具体可以参考下图：

![filter syntax](https://images2015.cnblogs.com/blog/410527/201608/410527-20160831125254840-1910065677.jpg)

filter是ffmpeg的libavfilter提供的基础单元。在同一个线性链中的filter使用逗号分隔，在不同线性链中的filter使用分号隔开，比如下面的例子：

ffmpeg -i INPUT -vf "split[main][tmp]; [tmp] crop=iw:ih/2:0:0, vflip [flip]; [main][flip]overlay=0:H/2" OUTPUT

这里crop、vflip处于同一个线性链，split、overlay位于另一个线性链。二者连接通过命名的label实现（位于中括号中的是label的名字）。在上例中split filter有两个输出，依次命名为[main]和[tmp]；[tmp]作为crop filter输入，之后通过vflip filter输出[flip]；overlay的输入是[main]和[flilp]。如果filter需要输入参数，多个参数使用冒号分割。
对于没有音频、视频输入的filter称为source filter，没有音频、视频输出的filter称为sink filter。

经典的filter

FFmpeg支持的所有filter可以通过[filters](http://ffmpeg.org/ffmpeg-filters.htm)查看。
这里选几个相对经典的filter。

音频filter

adelay filter
实现不同声道的延时处理。使用参数如下adelay=1500|0|500，这个例子中实现第一个声道的延迟1.5s，第三个声道延迟0.5s，第二个声道不做调整。

aecho filter
实现回声效果，具体参考http://ffmpeg.org/ffmpeg-filters.html#aecho。

amerge filter
将多个音频流合并成一个多声道音频流。具体参考http://ffmpeg.org/ffmpeg-filters.html#amerge-1。

ashowinfo filter
显示每一个audioframe的信息，比如时间戳、位置、采样格式、采样率、采样点数等。具体参考http://ffmpeg.org/ffmpeg-filters.html#ashowinfo。

panfilter
特定声道处理，比如立体声变为单声道，或者通过特定参数修改声道或交换声道。主要有两大类：
混音处理，比如下面的例子pan=1c|c0=0.9*c0+0.1*c1，实现立体声到单声道的变换；
声道变换，比如5.1声道顺序调整，pan="5.1| c0=c1 | c1=c0| c2=c2 | c3=c3 | c4=c4 | c5=c5"。

silencedetect和silenceremove filter
根据特定参数检测静音和移除静音。

volume和volumedetect filter
这两个filter分别实现音量调整和音量检测。

audio source filter
aevalsrc filter按照特定表达式生成音频信号。
anullsrc filter生成特定的原始音频数据，用于模板或测试。
anoisesrc filter生成噪声音频信号。
sine filter生成正弦波音频信号。

audio sink filter
abuffersink filter和anullsink filter，这些filter只是用于特定情况下结束filterchain。

视频filter

blend和tblend filter
将两帧视频合并为一帧。具体参数参考http://ffmpeg.org/ffmpeg-filters.html#blend_002c-tblend。

crop filter
按照特定分辨率裁剪输入视频，具体参数参考http://ffmpeg.org/ffmpeg-filters.html#crop。

drawbox、drawgrid、drawtext filter
绘制box（对话框）、grid（表格）、text（文本）。

edgedetect filter
边缘检测filter。

fps filter
按照指定帧率输出视频帧（丢帧或者复制）。具体参考http://ffmpeg.org/ffmpeg-filters.html#fps-1。

hflip、vflip filter
水平和垂直镜像。

histogram filter
生成每帧的各颜色分量的直方图。

noise filter
在输入视频帧中添加白噪声。

overlay filter
视频叠加。具体参考http://ffmpeg.org/ffmpeg-filters.html#overlay-1。

pad filter
视频边界填充。具体参考http://ffmpeg.org/ffmpeg-filters.html#pad-1。

rotate filter
视频任意角度旋转。具体参考http://ffmpeg.org/ffmpeg-filters.html#rotate。

scale filter
使用libswscale库完成视频缩放的filter。

showinfo filter
显示视频帧的参数信息，比如时间戳、采样格式、帧类型等。

subtitles filter
使用libass库绘制subtitle（字幕）。

thumbnail filter
提取缩略图的filter。

transpose filter
图像转置的filter。参数参考http://ffmpeg.org/ffmpeg-filters.html#transpose。

source filter
主要有cellatuo、coreimagesrc、mptestsrc、life等filter，具体效果建议参考ffmpeg用户手册。

source sink
主要有buffersink、nullsink两个filter。

多媒体filter

ahistogram filter
将音频转化为视频输出，并显示为音量的直方图。

concat filter
将音频流、视频流拼接成一个。具体参考http://ffmpeg.org/ffmpeg-filters.html#concat。

metadata、ametadata filter
操作metadata信息。

setpts、asetpts filter
改变输入音频帧或视频帧的pts。

showfreqs、showspectrum、showspertrumpic、showvolume、showwaves filter
将输入音频转换为视频显示，并显示频谱、音量等信息

split、asplit filter
将输入切分为多个相同的输出。

source filter
主要是movie、amovie filter。从movie容器中读取音频或者视频帧。

为ffmpeg添加自定义滤镜
https://blog.csdn.net/xiaojun111111/article/details/50849182

二、过滤器使用流程和经常函数

基本的过滤器使用流程是:

     解码后的画面--->buffer过滤器---->其他过滤器---->buffersink过滤器--->处理完的画面

所有的过滤器形成了过滤器链,一定要的两个过滤器是buffer过滤器和buffersink过滤器,前者的作用是将解码后的画面加载到过滤器链中,后者的作用是将处理好的画面从过滤器链中读取出来。

过滤器相关的结构体:

AVFilterGraph: 管理所有的过滤器图像

AVFilterContext: 过滤器上下文

AVFilter: 过滤器

流程中的关键函数如下所示：

avfilter_register_all()：注册所有AVFilter。
avfilter_graph_alloc()：为FilterGraph分配内存。
avfilter_graph_create_filter()：创建并向FilterGraph中添加一个Filter。
avfilter_graph_parse_ptr()：将一串通过字符串描述的Graph添加到FilterGraph中。参数3，4结构中可以指定需要连接输入过滤器、输出过滤器。

avfilter_link()：连接过滤器。连接输入输出过滤器。比如：

```cpp
avfilter_link(filter_buffer_ctx, 0,filter_yadif_ctx, 0);

avfilter_link(filter_yadif_ctx, 0,filter_buffersink_ctx, 0);
```

avfilter_graph_config()：检查FilterGraph的配置。

av_buffersrc_add_frame()：向FilterGraph中加入一个AVFrame。

av_buffersink_get_frame()：从FilterGraph中取出一个AVFrame。

三、实例

1、  ffmeg 反交错

参考：https://blog.csdn.net/crazyman2010/article/details/42913055

2、最简单的基于FFmpeg的AVfilter的例子

```cpp
// test_avfilter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/**
* 最简单的基于FFmpeg的AVFilter例子 - 纯净版
* Simplest FFmpeg AVfilter Example - Pure
*
* 雷霄骅 Lei Xiaohua
* leixiaohua1020@126.com
* 中国传媒大学/数字电视技术
* Communication University of China / Digital TV Technology
* http://blog.csdn.net/leixiaohua1020
*
* 本程序使用FFmpeg的AVfilter实现了YUV像素数据的滤镜处理功能。
* 可以给YUV数据添加各种特效功能。
* 是最简单的FFmpeg的AVFilter方面的教程。
* 适合FFmpeg的初学者。
*
* This software uses FFmpeg's AVFilter to process YUV raw data.
* It can add many excellent effect to YUV data.
* It's the simplest example based on FFmpeg's AVFilter.
* Suitable for beginner of FFmpeg
*
*/
#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
#define snprintf _snprintf
//Windows
extern "C"
{
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#endif

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

int main(int argc, char* argv[])
{
	getchar();

	int ret;
	AVFrame *frame_in;
	AVFrame *frame_out;
	unsigned char *frame_buffer_in;
	unsigned char *frame_buffer_out;

	AVFilterContext *buffersink_ctx;
	AVFilterContext *buffersrc_ctx;
	AVFilterGraph *filter_graph;
	static int video_stream_index = -1;

	//Input YUV
	FILE *fp_in = fopen("trans.yuv", "rb+");
	if (fp_in == NULL) {
		printf("Error open input file.\n");
		return -1;
	}
	int in_width = 1588;
	int in_height = 900;

	//Output YUV
	FILE *fp_out = fopen("output.yuv", "wb+");
	if (fp_out == NULL) {
		printf("Error open output file.\n");
		return -1;
	}

	//const char *filter_descr = "lutyuv='u=128:v=128'";
	//const char *filter_descr = "boxblur";
	//const char *filter_descr = "hflip";
	//const char *filter_descr = "hue='h=60:s=-3'";
	//const char *filter_descr = "crop=2/3*in_w:2/3*in_h";
	//const char *filter_descr = "drawbox=x=100:y=100:w=100:h=100:color=pink@0.5";
	const char *filter_descr = "drawtext=fontfile=arial.ttf:fontcolor=red:fontsize=30:text='Lei Xiaohua':x=50:y=50";

	avfilter_register_all();

	char args[512];
	AVFilter *buffersrc = avfilter_get_by_name("buffer");
	AVFilter *buffersink = avfilter_get_by_name("buffersink");//老版本ffmpeg使用 avfilter_get_by_name("ffbuffersink")
	AVFilterInOut *outputs = avfilter_inout_alloc();
	AVFilterInOut *inputs = avfilter_inout_alloc();
	enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };
	AVBufferSinkParams *buffersink_params;

	filter_graph = avfilter_graph_alloc();

	/* buffer video source: the decoded frames from the decoder will be inserted here. */
	snprintf(args, sizeof(args),
		"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
		in_width, in_height, AV_PIX_FMT_YUV420P,
		1, 25, 1, 1);

	ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
		args, NULL, filter_graph);
	if (ret < 0) {
		printf("Cannot create buffer source\n");
		return ret;
	}

	/* buffer video sink: to terminate the filter chain. */
	buffersink_params = av_buffersink_params_alloc();
	buffersink_params->pixel_fmts = pix_fmts;
	ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
		NULL, buffersink_params, filter_graph);
	av_free(buffersink_params);
	if (ret < 0) {
		printf("Cannot create buffer sink\n");
		return ret;
	}

	/* Endpoints for the filter graph. */
	outputs->name = av_strdup("in");
	outputs->filter_ctx = buffersrc_ctx;
	outputs->pad_idx = 0;
	outputs->next = NULL;

	inputs->name = av_strdup("out");
	inputs->filter_ctx = buffersink_ctx;
	inputs->pad_idx = 0;
	inputs->next = NULL;

	if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr,
		&inputs, &outputs, NULL)) < 0)
		return ret;

	if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
		return ret;

	frame_in = av_frame_alloc();
	frame_buffer_in = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width, in_height, 1));
	av_image_fill_arrays(frame_in->data, frame_in->linesize, frame_buffer_in,
		AV_PIX_FMT_YUV420P, in_width, in_height, 1);

	frame_out = av_frame_alloc();
	frame_buffer_out = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width, in_height, 1));
	av_image_fill_arrays(frame_out->data, frame_out->linesize, frame_buffer_out,
		AV_PIX_FMT_YUV420P, in_width, in_height, 1);

	frame_in->width = in_width;
	frame_in->height = in_height;
	frame_in->format = AV_PIX_FMT_YUV420P;

	while (1) {

		if (fread(frame_buffer_in, 1, in_width*in_height * 3 / 2, fp_in) != in_width*in_height * 3 / 2) {
			break;
		}
		//input Y,U,V
		frame_in->data[0] = frame_buffer_in;
		frame_in->data[1] = frame_buffer_in + in_width*in_height;
		frame_in->data[2] = frame_buffer_in + in_width*in_height * 5 / 4;

		if (av_buffersrc_add_frame(buffersrc_ctx, frame_in) < 0) {
			printf("Error while add frame.\n");
			break;
		}

		/* pull filtered pictures from the filtergraph */
		ret = av_buffersink_get_frame(buffersink_ctx, frame_out);
		if (ret < 0)
			break;

		//output Y,U,V
		if (frame_out->format == AV_PIX_FMT_YUV420P) {
			for (int i = 0; i<frame_out->height; i++) {
				fwrite(frame_out->data[0] + frame_out->linesize[0] * i, 1, frame_out->width, fp_out);
			}
			for (int i = 0; i<frame_out->height / 2; i++) {
				fwrite(frame_out->data[1] + frame_out->linesize[1] * i, 1, frame_out->width / 2, fp_out);
			}
			for (int i = 0; i<frame_out->height / 2; i++) {
				fwrite(frame_out->data[2] + frame_out->linesize[2] * i, 1, frame_out->width / 2, fp_out);
			}
		}
		printf("Process 1 frame!\n");
		av_frame_unref(frame_out);
	}

	fclose(fp_in);
	fclose(fp_out);

	av_frame_free(&frame_in);
	av_frame_free(&frame_out);
	avfilter_graph_free(&filter_graph);

	return 0;
}
```

3、最简单的基于FFmpeg的AVfilter例子（水印叠加）

https://blog.csdn.net/leixiaohua1020/article/details/29368911

源代码：https://github.com/leixiaohua1020/simplest_ffmpeg_video_filter

 四、注意事项

1、 显示文字乱码问题，文字需要是utf-8格式，字体也必须存在。

2、 调用avfilter_graph_parse_ptr解析字幕水印时返回-22，ffmpeg报错No suchfilter: 'drawtext'，这是由于编译的时候没有开启FreeType字体引擎库，编译的时候./configure--enable-libfreetype。

3、 调用avfilter_graph_parse_ptr解析参数时返回-2，drawtext=fontfile=arial.ttf:fontcolor=red:fontsize=30:text='LeiXiaohua':x=50:y=50里面的arial.ttf是字体文件，要保证字体文件在程序可以找到的目录，冒号在里面是分隔符，路径中不能带冒号。

4、 编译完ffmpeg后可能没有libpostproc.so这个库，编译的时候加入开启这个库的编译选项--enable-postproc。

参考资料：

[https://www.cnblogs.com/tocy/p/ffmpeg-filter-intro.html](https://www.cnblogs.com/tocy/p/ffmpeg-filter-intro.html)

[https://blog.csdn.net/shixin_0125/article/details/78400252](https://blog.csdn.net/shixin_0125/article/details/78400252)

