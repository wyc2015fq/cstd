# ffmpeg+SDL的使用之获取视频帧将其存储为PPM格式图片 - zhuyinglong的博客 - CSDN博客





2017年09月29日 19:50:40[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：314








# **前言**



FFMPEG(Fast Forward Moving Pictures Experts Group)是音视频的分离，转换，编码解码及流媒体的开源完全解决方案。关于FFMPEG的库的介绍网上有很多资料，如果只是看不使用，收获并不会有多大。下面只介绍下面程序中会使用到的四个库且只介绍其中程序中会使用到的功能。
libavformat：用于存放muxer/demuxer模块，对音频视频格式的解析;用于各种音视频封装格式的生成和解析，包括获取解码所需信息以生成解码上下文结构和读取音视频帧等功能；

libavcodec：用于存放各个encode/decode模块，CODEC其实是COder/DECoder的缩写，也就是编码解码器；用于各种类型声音/图像编解码；

libswscale：用于视频场景比例缩放、色彩映射转换；

libavutil：工具库，包含一些公共的工具函数；

要使用ffmpeg对流进行处理，不仅需要学习相应的ffmpeg接口，还需要具有一定的流媒体相关的知识。下文所提到的例子并没有多少的代码量，但是想要一下子看懂还是有些难度的。本人在学习该方面知识的时候，下载了雷神的代码，也运行成功了，但是将其看懂理解着实费了不少功夫，主要是流媒体相关知识储备不够。这篇文章并不对流媒体相关知识进行讲解。


下载了新版本的ffmpeg3.3.3发现有些接口已经改变，直接使用雷神博客中的源码，编译器不将SDL检查关闭的话是不能编译编译通过的。关闭方法找到项目属性页如下图所示位置，将SDL检查是改为否，即可编译通过。
![](https://img-blog.csdn.net/20170929195509582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

但是既然版本库更新就有它的理由，对于有代码洁癖的人这是不能容忍的，所以本项目基于3.3.3版本库的接口对雷神博客中的源码进行改写。

ffmpeg相关库的下载及配置方法请参见以下博客：

> 
[在Visual Studio中编写基于ffmpeg的helloworld程序](http://blog.csdn.net/zhuyinglong2010/article/details/77968718)




# **原理介绍**

从原理上来讲，使用ffmpeg处理音视频流的步骤还是比较简单的，如下：

1：从本地或者网络位置打开音视频流；

2：获取音视频流中的包，将其转换为帧；

3：如果帧不完整，转到2；

4：对帧做一些事情；

5：转到2；

我们所能做的就是在步骤4上进行一些处理，该程序所做的就是将帧转换成RGB存储方式，然后存入PPM文件。为什么是PPM格式而不是其他图像格式，因为该图像格式存储数据的方式比较简单，并不需要做多少处理。关于PPM文件格式详细介绍，参见以下博客：[](http://blog.csdn.net/zhuyinglong2010/article/details/78078397)

> 
[PPM图像格式说明](http://blog.csdn.net/zhuyinglong2010/article/details/78078397)


# **建立工程**

## **1、新建一个空的WIN32控制台程序，添加cpp源文件。**

说明：以下截图中包含其余两个在修改中的工程videoPlay、audioPlay及SDL库相关东西，本项目并没有用到可忽略。

工程根目录结构如下图所示：

![](https://img-blog.csdn.net/20170929202412436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中dll文件中为所需ffmpeg的动态库，如下图所示：

![](https://img-blog.csdn.net/20170929202621519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中lib文件中为ffmpeg的动态库的引导库，如下图所示：

![](https://img-blog.csdn.net/20170929202803328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中include文件中为ffmpeg的头文件及ffmpegsdl.h，如下图所示：


![](https://img-blog.csdn.net/20170929202745865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## 2、项目属性配置



头文件及lib引导库目录配置如下图：

![](https://img-blog.csdn.net/20170929204001197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


dll库目录配置如下图：

![](https://img-blog.csdn.net/20170929204009324?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该dll库目录可不用配置，直接将dll拷贝到编译生成的exe同目录的Debug目录下亦可。

# **源码**



由于后续要学习SDL，所以就自己创建了一个新的头文件ffmpegsdl.h，为了使用方便，将ffmpeg和SDL库的所有头文件、lib等包含进来了，实际项目并不需要那么多，可根据需要进行裁剪。源码如下：



```cpp
#include <iostream>
#include <string>
#include <sstream> //for stringstream
#include <fstream> //for file 
#include "ffmpegsdl.h"

using namespace std;

//将帧保存为PPM图像
void SaveFrame2PPM(AVFrame *pFrame, int width, int height, int iFrame);

//int转string
string num2string(int& num);


int main()
{
	// Register all formats and codecs
	av_register_all();

	// Initalizing to NULL prevents segfaults!
	AVFormatContext   *pFormatCtx = NULL;
	// Open video file
	if (avformat_open_input(&pFormatCtx, "..\\test.mp4", NULL, NULL) != 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
		return -1; // Couldn't open file
	}
	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, "1=========================>test.mp4", 0);
	// Retrieve stream information
	if (avformat_find_stream_info(pFormatCtx, NULL)<0)
	{
		av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
		return -1; // Couldn't find stream information
	}
	// 对比上次的Dump可以看出通过avformat_find_stream_info函数往AVFormatContext中填充了流的信息
	av_dump_format(pFormatCtx, 0, "2=========================>test.mp4", 0);

	AVCodec           *pDec = NULL;
	/* select the video stream and find the decoder*/
	int video_stream_index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &pDec, 0);
	if (video_stream_index < 0) 
	{
		av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
		return -1;
	}

	//Note that we must not use the AVCodecContext from the video stream directly! 
	//So we have to use avcodec_copy_context() to copy the context to a new location (after allocating memory for it, of course).	
	/* create decoding context */
	AVCodecContext    *pDecCtx = avcodec_alloc_context3(pDec);
	if (!pDecCtx)
	{
		return AVERROR(ENOMEM);
	}

	//Fill the codec context based on the values from the supplied codec
	avcodec_parameters_to_context(pDecCtx, pFormatCtx->streams[video_stream_index]->codecpar);

	//Initialize the AVCodecContext to use the given AVCodec.
	if (avcodec_open2(pDecCtx, pDec, NULL) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
		return -1;
	}


	AVPacket* packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);
	//Allocate an AVFrame and set its fields to default values.
	AVFrame *pFrame = av_frame_alloc();
	AVFrame *pFrameRGB = av_frame_alloc();

	// Determine required buffer size and allocate buffer
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pDecCtx->width,pDecCtx->height,1);
	uint8_t* buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize,
		buffer,AV_PIX_FMT_RGB24,
		pDecCtx->width,
		pDecCtx->height,
		1
	);
	// initialize SWS context for software scaling
	SwsContext *sws_ctx = sws_getContext(pDecCtx->width,
		pDecCtx->height,
		pDecCtx->pix_fmt,
		pDecCtx->width,
		pDecCtx->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);


	int ret = -1;
	int count = 0;
	while (1) 
	{
		/**
		 *Technically a packet can contain partial frames or other bits of data, 
		 *but ffmpeg's parser ensures that the packets we get contain either complete or multiple frames.
		 */
		ret = av_read_frame(pFormatCtx, packet);
		if (ret < 0)
		{
			break;
		}

		if (packet->stream_index == video_stream_index) 
		{
			//Supply raw packet data as input to a decoder.
			ret = avcodec_send_packet(pDecCtx, packet);
			if (ret < 0) 
			{
				av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
				break;
			}

			while (ret >= 0)
			{
				//Return decoded output data from a decoder.
				ret = avcodec_receive_frame(pDecCtx, pFrame);//一个包可能有多个帧
				if (ret >= 0)
				{
					// Convert the image from its native format to RGB
					sws_scale(sws_ctx, (const uint8_t* const *)pFrame->data,
						pFrame->linesize, 0, pDecCtx->height,
						pFrameRGB->data, pFrameRGB->linesize);
					if(count < 5 )
					{
						count++;
						SaveFrame2PPM(pFrameRGB, pDecCtx->width, pDecCtx->height, count);
					}
						
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
		}
		// Wipe the packet.		
		av_packet_unref(packet);
	}
end:
	// Free the packet that was allocated by av_read_frame
	av_packet_free(&packet);
	//Free the swscaler context swsContext
	sws_freeContext(sws_ctx);
	//Free the codec context and everything associated with it and write NULL to the provided pointer.
	avcodec_free_context(&pDecCtx);
	// Close the video file
	avformat_close_input(&pFormatCtx);
	//Free the frame and any dynamically allocated objects in it
	av_frame_free(&pFrame);
	av_frame_free(&pFrameRGB);
	// Free the RGB image
	av_free(buffer);
	return 0;
}

void SaveFrame2PPM(AVFrame *pFrame, int width, int height, int iFrame)
{
	string strFile = "frame" + num2string(iFrame)+ ".ppm";
	//采用追加模式，以二进制写入到文件  
	ofstream fout(strFile.c_str(), ios_base::out | ios_base::binary);
	if (!fout.is_open())
	{
		cerr << "Can't open " << strFile << endl;
		exit(EXIT_FAILURE);
	}
	
	// Write PPM header
	fout << "P6" << endl;
	fout << width << " " << height << endl;
	fout << "255" << endl;
	

	// Write pixel data
	for (int y = 0; y<height; y++)
	{
		fout.write((const char*)(pFrame->data[0] + y*pFrame->linesize[0]), width * 3);
	}
	fout.close();
	fout.clear();
	cout << strFile << " write!"<<endl;
	
}

//ostringstream对象用来进行格式化的输出，常用于将各种类型转换为string类型
//ostringstream只支持<<操作符
string num2string(int& num)
{
	ostringstream oss;  //创建一个格式化输出流
	oss << num;             //把值传递如流中
	return oss.str();
}
```

**工程下载**
github: [https://github.com/zhuyinglong/ffmpeg-SDL](https://github.com/zhuyinglong/ffmpeg-SDL)


# **后续**

虽然我尽力让自己正确，但避免不了有些理解不到位的误导大家，有些不清楚的地方，推荐大家还是参考官方原版一手的资料。参考资料如下：
[http://dranger.com/ffmpeg/tutorial01.html](http://dranger.com/ffmpeg/tutorial01.html)
[http://ffmpeg.org/doxygen/trunk/filtering_video_8c-example.html](http://ffmpeg.org/doxygen/trunk/filtering_video_8c-example.html)






