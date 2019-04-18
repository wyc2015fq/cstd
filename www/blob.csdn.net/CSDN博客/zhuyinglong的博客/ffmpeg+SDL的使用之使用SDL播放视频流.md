# ffmpeg+SDL的使用之使用SDL播放视频流 - zhuyinglong的博客 - CSDN博客





2017年09月30日 10:54:50[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：532








# 前言
首先再次说明下使用ffmpeg处理音视频流的步骤如下：

> 1：从本地或者网络位置打开音视频流；
2：获取音视频流中的包，将其转换为帧；
3：如果帧不完整，转到2；
4：对帧做一些事情；
5：转到2；


我们所能做的就是在步骤4上进行一些处理。本项目所做的事流程如下：

首先，找到包中的视频流。

然后，解码成帧。

最后，将原始帧转换成YUV420P格式使用SDL进行覆盖显示。

注意：本项目中对视频的播放并没有做延时，解码转换出来直接播放，所以播放速度较快。



# **流媒体相关知识科普**

YUV和RGB一样也是一种颜色空间。其原理是把亮度与色度分离，研究证明,人眼对亮度的敏感超过色度。利用这个原理，可以把色度信息减少一点，人眼也无法查觉这一点。所以使用该颜色空间可以有效降低带宽。简单的来说，现在视频流一般采用的视频编码格式为H264（工程中test.mp4视频就是），该格式是经过压缩编码后的，是不能直接被显示出来的。必须经过解码到未压缩的格式，如YUV，才可以播放。事实上现在很多视频流的格式已经是YUV格式或者很容易被转换成YUV格式了。SDL支持多种格式的YUV覆盖，下面程序使用较为通用的一种YUV420P。使用VLC播放器对工程中的test.mp4视频文件编解码信息进行查看如下图：

![](https://img-blog.csdn.net/20170930154052179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中，MP4是一种视频的封装格式，其余的还有MKV，AVI等。我们一般所说的视频都包括音频，所以准确的来说，MP4格式用来对编码之后的H264（视频压缩格式）和AAC（音频压缩格式）进行封装，使用播放器播放的时候需要将视频和音频分离出来，该过程叫解复用。然后对视音频分别进行解码，视频一般解码为YUV，音频一般解码为PCM。播放器将解码后的音视频数据经过音视频同步之后，分别输送到声卡和网卡驱动。这就是我们平时从点开一个视频文件到播放出来所经历的步骤。


**源码**


有了前一篇博文[ffmpeg+SDL的使用之获取视频帧将其存储为PPM格式图片](http://blog.csdn.net/zhuyinglong2010/article/details/78136863)的技术积累，现在需要学习的新知识就是如何使用SDL将视频显示出来。SDL库的使用相对来讲还是比较简单**。**头文件采用和前篇博文一样的头文件ffmpegsdl.h，工程配置方式也相同。

videoPlay.cpp的源码如下：

```cpp
#include <iostream>
#include "ffmpegsdl.h"
using namespace std;

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

	AVCodec* pDec = NULL;
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
	AVCodecContext* pDecCtx = avcodec_alloc_context3(pDec);
	if (!pDecCtx)
	{
		return AVERROR(ENOMEM);
	}

	//Fill the codec context based on the values from the supplied codec
	avcodec_parameters_to_context(pDecCtx, pFormatCtx->streams[video_stream_index]->codecpar);

	//Initialize the AVCodecContext to use the given AVCodec.
	if (avcodec_open2(pDecCtx, pDec, NULL) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
		return -1;
	}


	AVPacket* packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);
	//Allocate an AVFrame and set its fields to default values.
	AVFrame *pFrame = av_frame_alloc();
	AVFrame *pFrameYUV = av_frame_alloc();

	// Determine required buffer size and allocate buffer
	int byteSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P,
		pDecCtx->width,
		pDecCtx->height,
		1
	);
	uint8_t* buffer = (uint8_t *)av_malloc(byteSize);

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,
		buffer, AV_PIX_FMT_YUV420P,
		pDecCtx->width,
		pDecCtx->height,
		1
	);

	int screen_w = pDecCtx->width / 2;
	int screen_h = pDecCtx->height / 2;
	// initialize SWS context for software scaling
	SwsContext *pSws_ctx = sws_getContext(pDecCtx->width,
		pDecCtx->height,
		pDecCtx->pix_fmt,
		screen_w,
		screen_h,
		AV_PIX_FMT_YUV420P,
		SWS_BICUBIC,
		NULL,
		NULL,
		NULL
	);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	//SDL 2.0 Support for multiple windows  
	SDL_Window* screen = SDL_CreateWindow("SDL",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h,
		SDL_WINDOW_OPENGL
	);

	if (!screen)
	{
		printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
		return -1;
	}

	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
	//IYUV: Y + U + V  (3 planes)  
	SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer,
		SDL_PIXELFORMAT_IYUV,
		SDL_TEXTUREACCESS_STREAMING,
		screen_w,
		screen_h
	);
	SDL_Rect sdlRect;
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = screen_w;
	sdlRect.h = screen_h;

	int ret = -1;
	SDL_Event event;

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
					// Convert the image from its native format to YUV
					sws_scale(pSws_ctx,
						(const uint8_t* const *)pFrame->data,
						pFrame->linesize,
						0,
						pDecCtx->height,
						pFrameYUV->data,
						pFrameYUV->linesize
					);

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

				SDL_UpdateYUVTexture(sdlTexture, &sdlRect,
					pFrameYUV->data[0], pFrameYUV->linesize[0],
					pFrameYUV->data[1], pFrameYUV->linesize[1],
					pFrameYUV->data[2], pFrameYUV->linesize[2]
				);
				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
				SDL_RenderPresent(sdlRenderer);
			}

		}
		// Wipe the packet.		
		av_packet_unref(packet);
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			SDL_Quit();
			break;
		default:
			break;
		}
	}
end:
	// Free the packet that was allocated by av_read_frame
	av_packet_free(&packet);
	//Free the swscaler context swsContext
	sws_freeContext(pSws_ctx);
	//Free the codec context and everything associated with it and write NULL to the provided pointer.
	avcodec_free_context(&pDecCtx);
	// Close the video file
	avformat_close_input(&pFormatCtx);
	//Free the frame and any dynamically allocated objects in it
	av_frame_free(&pFrame);
	av_frame_free(&pFrameYUV);
	// Free the RGB image
	av_free(buffer);
	return 0;
}
```


# **工程下载**

github: [https://github.com/zhuyinglong/ffmpeg-SDL](https://github.com/zhuyinglong/ffmpeg-SDL)



# **后续**

虽然我尽力让自己正确，但避免不了有些理解不到位的误导大家，有些不清楚的地方，推荐大家还是参考官方原版一手的资料。这也是为什么我的代码注释很少的原因，只是帮大家将流程打通。大家将我的工程下载之后，使用source Insight可以很方便的看到所有函数的官方原版注释，那是最准确的。


参考资料如下：
[http://dranger.com/ffmpeg/tutorial02.html](http://dranger.com/ffmpeg/tutorial02.html)
[http://ffmpeg.org/doxygen/trunk/filtering_video_8c-example.html](http://ffmpeg.org/doxygen/trunk/filtering_video_8c-example.html)




