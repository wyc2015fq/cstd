# 100行代码实现最简单的基于FFMPEG+SDL的视频播放器（SDL1.x） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年03月08日 23:57:08[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：164347
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)











=====================================================

最简单的基于FFmpeg的视频播放器系列文章列表：

[100行代码实现最简单的基于FFMPEG+SDL的视频播放器（SDL1.x）](http://blog.csdn.net/leixiaohua1020/article/details/8652605)

[最简单的基于FFMPEG+SDL的视频播放器 ver2 （采用SDL2.0）](http://blog.csdn.net/leixiaohua1020/article/details/38868499)

[最简单的基于FFmpeg的解码器-纯净版（不包含libavformat）](http://blog.csdn.net/leixiaohua1020/article/details/42181571)

[最简单的基于FFMPEG+SDL的视频播放器：拆分-解码器和播放器](http://blog.csdn.net/leixiaohua1020/article/details/46889389)

[最简单的基于FFMPEG的Helloworld程序](http://blog.csdn.net/leixiaohua1020/article/details/46889849)

=====================================================




## 简介

FFMPEG工程浩大，可以参考的书籍又不是很多，因此很多刚学习FFMPEG的人常常感觉到无从下手。我刚接触FFMPEG的时候也感觉不知从何学起。

因此我把自己做项目过程中实现的一个非常简单的视频播放器（大约100行代码）源代码传上来，以作备忘，同时方便新手学习FFMPEG。

该播放器虽然简单，但是几乎包含了使用FFMPEG播放一个视频所有必备的API，并且使用SDL显示解码出来的视频。

并且支持流媒体等多种视频输入，处于简单考虑，没有音频部分，同时视频播放采用直接延时40ms的方式

平台使用VC2010，使用了新版的FFMPEG类库。




**SourceForge项目主页：**

[https://sourceforge.net/projects/simplestffmpegplayer/](https://sourceforge.net/projects/simplestffmpegplayer/)




注：本文SDL采用1.x版本。另一版本采用SDL2.0，可参考：

基于FFMPEG+SDL的视频播放器 ver2 （采用SDL2.0）：[http://blog.csdn.net/leixiaohua1020/article/details/38868499](http://blog.csdn.net/leixiaohua1020/article/details/38868499)




## 流程图

没想到这篇文章中介绍的播放器挺受FFMPEG初学者的欢迎，因此再次更新两张流程图，方便大家学习。此外在源代码上添加了注释，方便理解。

该播放器解码的流程用图的方式可以表示称如下形式：

![](https://img-blog.csdn.net/20140508135954718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


SDL显示YUV图像的流程图：

![](https://img-blog.csdn.net/20140526235054515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




简单解释几句：

SDL_Surface就是使用SDL的时候弹出的那个窗口。在SDL1.x版本中，只可以创建一个SDL_Surface。

SDL_Overlay用于显示YUV数据。一个SDL_Overlay对应一帧YUV数据。

SDL_Rect用于确定SDL_Overlay显示的位置。注意：一个SDL_Overlay可以指定多个不同的SDL_Rect，这样就可以在SDL_Surface不同位置显示相同的内容。

它们的关系如下图所示：


![](https://img-blog.csdn.net/20140827123406078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下图举了个例子，指定了4个SDL_Rect，可以实现4分屏的显示。

![](https://img-blog.csdn.net/20140827124218223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## simplest_ffmpeg_player（标准版）代码

```cpp
/**
 * 最简单的基于FFmpeg的视频播放器
 * Simplest FFmpeg Player
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了视频文件的解码和显示（支持HEVC，H.264，MPEG2等）。
 * 是最简单的FFmpeg视频解码方面的教程。
 * 通过学习本例子可以了解FFmpeg的解码流程。
 * This software is a simplest video player based on FFmpeg.
 * Suitable for beginner of FFmpeg.
 */


#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "SDL/SDL.h"
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
#include <SDL/SDL.h>
#ifdef __cplusplus
};
#endif
#endif


//Full Screen
#define SHOW_FULLSCREEN 0
//Output YUV420P 
#define OUTPUT_YUV420P 0


int main(int argc, char* argv[])
{
	//FFmpeg
	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	AVPacket *packet;
	struct SwsContext *img_convert_ctx;
	//SDL
	int screen_w,screen_h;
	SDL_Surface *screen; 
	SDL_VideoInfo *vi;
	SDL_Overlay *bmp; 
	SDL_Rect rect;

	FILE *fp_yuv;
	int ret, got_picture;
	char filepath[]="bigbuckbunny_480x272.h265";

	av_register_all();
	avformat_network_init();
	
	pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
		}
	if(videoindex==-1){
		printf("Didn't find a video stream.\n");
		return -1;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL){
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
		printf("Could not open codec.\n");
		return -1;
	}
	
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	//uint8_t *out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	//avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	//SDL----------------------------
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 

	

#if SHOW_FULLSCREEN
	vi = SDL_GetVideoInfo();
	screen_w = vi->current_w;
	screen_h = vi->current_h;
	screen = SDL_SetVideoMode(screen_w, screen_h, 0,SDL_FULLSCREEN);
#else
	screen_w = pCodecCtx->width;
	screen_h = pCodecCtx->height;
	screen = SDL_SetVideoMode(screen_w, screen_h, 0,0);
#endif

	if(!screen) {  
		printf("SDL: could not set video mode - exiting:%s\n",SDL_GetError());  
		return -1;
	}

	bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, screen); 

	rect.x = 0;    
	rect.y = 0;    
	rect.w = screen_w;    
	rect.h = screen_h;  
	//SDL End------------------------


	packet=(AVPacket *)av_malloc(sizeof(AVPacket));
	//Output Information-----------------------------
	printf("------------- File Information ------------------\n");
	av_dump_format(pFormatCtx,0,filepath,0);
	printf("-------------------------------------------------\n");

#if OUTPUT_YUV420P 
    fp_yuv=fopen("output.yuv","wb+");  
#endif  

	SDL_WM_SetCaption("Simplest FFmpeg Player",NULL);

	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	//------------------------------
	while(av_read_frame(pFormatCtx, packet)>=0){
		if(packet->stream_index==videoindex){
			//Decode
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(ret < 0){
				printf("Decode Error.\n");
				return -1;
			}
			if(got_picture){
				SDL_LockYUVOverlay(bmp);
				pFrameYUV->data[0]=bmp->pixels[0];
				pFrameYUV->data[1]=bmp->pixels[2];
				pFrameYUV->data[2]=bmp->pixels[1];     
				pFrameYUV->linesize[0]=bmp->pitches[0];
				pFrameYUV->linesize[1]=bmp->pitches[2];   
				pFrameYUV->linesize[2]=bmp->pitches[1];
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, 
					pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
#if OUTPUT_YUV420P
				int y_size=pCodecCtx->width*pCodecCtx->height;  
				fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y 
				fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
				fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
#endif
				
				SDL_UnlockYUVOverlay(bmp); 

				SDL_DisplayYUVOverlay(bmp, &rect); 
				//Delay 40ms
				SDL_Delay(40);
			}
		}
		av_free_packet(packet);
	}

	//FIX: Flush Frames remained in Codec
	while (1) {
		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if (ret < 0)
			break;
		if (!got_picture)
			break;
		sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
		
		SDL_LockYUVOverlay(bmp);
		pFrameYUV->data[0]=bmp->pixels[0];
		pFrameYUV->data[1]=bmp->pixels[2];
		pFrameYUV->data[2]=bmp->pixels[1];     
		pFrameYUV->linesize[0]=bmp->pitches[0];
		pFrameYUV->linesize[1]=bmp->pitches[2];   
		pFrameYUV->linesize[2]=bmp->pitches[1];
#if OUTPUT_YUV420P
		int y_size=pCodecCtx->width*pCodecCtx->height;  
		fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y 
		fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
		fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
#endif

		SDL_UnlockYUVOverlay(bmp); 
		SDL_DisplayYUVOverlay(bmp, &rect); 
		//Delay 40ms
		SDL_Delay(40);
	}

	sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

	SDL_Quit();

	//av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}
```





1.1版之后，新添加了一个工程：simplest_ffmpeg_player_su（SU版）。


标准版在播放视频的时候，画面显示使用延时40ms的方式。这么做有两个后果：
（1）SDL弹出的窗口无法移动，一直显示是忙碌状态
（2）画面显示并不是严格的40ms一帧，因为还没有考虑解码的时间。SU（SDL Update）版在视频解码的过程中，不再使用延时40ms的方式，而是创建了一个线程，每隔40ms发送一个自定义的消息，告知主函数进行解码显示。这样做之后：
（1）SDL弹出的窗口可以移动了
（2）画面显示是严格的40ms一帧




## simplest_ffmpeg_player_su（SU版）代码

```cpp
/**
 * 最简单的基于FFmpeg的视频播放器SU(SDL升级版)
 * Simplest FFmpeg Player (SDL Update)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了视频文件的解码和显示（支持HEVC，H.264，MPEG2等）。
 * 是最简单的FFmpeg视频解码方面的教程。
 * 通过学习本例子可以了解FFmpeg的解码流程。
 * 本版本中使用SDL消息机制刷新视频画面。
 * This software is a simplest video player based on FFmpeg.
 * Suitable for beginner of FFmpeg.
 * 
 * Version:1.2
 * 
 * 备注:
 * 标准版在播放视频的时候，画面显示使用延时40ms的方式。这么做有两个后果：
 * （1）SDL弹出的窗口无法移动，一直显示是忙碌状态
 * （2）画面显示并不是严格的40ms一帧，因为还没有考虑解码的时间。
 * SU（SDL Update）版在视频解码的过程中，不再使用延时40ms的方式，而是创建了
 * 一个线程，每隔40ms发送一个自定义的消息，告知主函数进行解码显示。这样做之后：
 * （1）SDL弹出的窗口可以移动了
 * （2）画面显示是严格的40ms一帧
 * Remark:
 * Standard Version use's SDL_Delay() to control video's frame rate, it has 2
 * disadvantages:
 * (1)SDL's Screen can't be moved and always "Busy".
 * (2)Frame rate can't be accurate because it doesn't consider the time consumed 
 * by avcodec_decode_video2()
 * SU（SDL Update）Version solved 2 problems above. It create a thread to send SDL 
 * Event every 40ms to tell the main loop to decode and show video frames.
 */


#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "SDL/SDL.h"
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
#include <SDL/SDL.h>
#ifdef __cplusplus
};
#endif
#endif

//Refresh
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

int thread_exit=0;
//Thread
int sfp_refresh_thread(void *opaque)
{
	SDL_Event event;
	while (thread_exit==0) {
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		//Wait 40 ms
		SDL_Delay(40);
	}
	return 0;
}


int main(int argc, char* argv[])
{
	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	AVPacket *packet;
	struct SwsContext *img_convert_ctx;
	//SDL
	int ret, got_picture;
	int screen_w=0,screen_h=0;
	SDL_Surface *screen; 
	SDL_Overlay *bmp; 
	SDL_Rect rect;
	SDL_Thread *video_tid;
	SDL_Event event;

	char filepath[]="bigbuckbunny_480x272.h265";
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	
	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
		}
	if(videoindex==-1){
		printf("Didn't find a video stream.\n");
		return -1;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}

	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	//uint8_t *out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	//avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
//------------SDL----------------
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 

	
	screen_w = pCodecCtx->width;
	screen_h = pCodecCtx->height;
	screen = SDL_SetVideoMode(screen_w, screen_h, 0,0);

	if(!screen) {  
		printf("SDL: could not set video mode - exiting:%s\n",SDL_GetError());  
		return -1;
	}
	
	bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, screen); 
	
	rect.x = 0;    
	rect.y = 0;    
	rect.w = screen_w;    
	rect.h = screen_h;  

	packet=(AVPacket *)av_malloc(sizeof(AVPacket));

	printf("---------------File Information------------------\n");
	av_dump_format(pFormatCtx,0,filepath,0);
	printf("-------------------------------------------------\n");
	
	
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	//--------------
	video_tid = SDL_CreateThread(sfp_refresh_thread,NULL);
	//
	SDL_WM_SetCaption("Simple FFmpeg Player (SDL Update)",NULL);

	//Event Loop
	
	for (;;) {
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==SFM_REFRESH_EVENT){
			//------------------------------
			if(av_read_frame(pFormatCtx, packet)>=0){
				if(packet->stream_index==videoindex){
					ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
					if(ret < 0){
						printf("Decode Error.\n");
						return -1;
					}
					if(got_picture){

						SDL_LockYUVOverlay(bmp);
						pFrameYUV->data[0]=bmp->pixels[0];
						pFrameYUV->data[1]=bmp->pixels[2];
						pFrameYUV->data[2]=bmp->pixels[1];     
						pFrameYUV->linesize[0]=bmp->pitches[0];
						pFrameYUV->linesize[1]=bmp->pitches[2];   
						pFrameYUV->linesize[2]=bmp->pitches[1];
						sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

						SDL_UnlockYUVOverlay(bmp); 
						
						SDL_DisplayYUVOverlay(bmp, &rect); 

					}
				}
				av_free_packet(packet);
			}else{
				//Exit Thread
				thread_exit=1;
				break;
			}
		}

	}
	
	SDL_Quit();

	sws_freeContext(img_convert_ctx);

	//--------------
	//av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}
```



simplest_ffmpeg_player_su（SU版）中将simplest_ffmpeg_player（标准版）中的循环做了更改。标准版中为播放视频的循环如下代码所示。



```cpp
main(){
	//...
	while(av_read_frame(pFormatCtx, packet)>=0)
		{
			//Decode...
			SDL_Delay(40);
		}
	//...
}
```

可以看出标准版中使用SDL_Delay(40)控制视频的播放速度。这样有一些问题在前文中已经叙述。SU版定义了一个函数专门用于发送“解码和显示”的Event。




```cpp
//自定义事件
//刷新画面
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

int thread_exit=0;
//Thread
int sfp_refresh_thread(void *opaque)
{
	while (thread_exit==0) {
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		//Wait 40 ms
		SDL_Delay(40);
	}
	return 0;
}
```

主函数形式如下。使用SDL_WaitEvent()等待Event进行解码和显示。




```cpp
main(){
	//...
	SDL_Thread *video_tid = SDL_CreateThread(sfp_refresh_thread,NULL);
	//Event Loop
	SDL_Event event;
	for (;;) {
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==SFM_REFRESH_EVENT){
			//Decode...
		}

	}
	//...
}
```






## 结果



软件运行截图：

![](https://img-my.csdn.net/uploads/201303/08/1362758178_5693.JPG)

完整工程下载地址：

[http://download.csdn.net/detail/leixiaohua1020/5122959](http://download.csdn.net/detail/leixiaohua1020/5122959)





**更新（2014.5.10）==========================**





完整工程（更新版）下载地址：

[http://download.csdn.net/detail/leixiaohua1020/7319153](http://download.csdn.net/detail/leixiaohua1020/7319153)


注1：类库版本2014.5.6，已经支持HEVC以及VP9的解码，附带了这两种视频编码的码流文件。此外修改了个别变更的API函数，并且提高了一些程序的效率。

注2：新版FFmpeg类库Release下出现错误的解决方法如下：
（注：此方法适用于所有近期发布的FFmpeg类库）
VC工程属性里，linker->Optimization->References 选项，改成No(/OPT:NOREF)即可。





**更新（2014.8.25）==========================**

**simplest ffmpeg player 1.1**


版本升级至1.1，变为2个项目：

**simplest_ffmpeg_player**：标准版，FFmpeg学习的开始。
**simplest_ffmpeg_player_su**：SU（SDL Update）版，加入了简单的SDL的Event。


**simplest_ffmpeg_player（标准版）**增加了以下两个选项（当然，代码量超过了100行）

1.输出解码后的YUV420P像素数据文件

2.全屏播放

以上两项可以通过文件前面的宏进行控制：



```cpp
#define SHOW_FULLSCREEN 0
#define OUTPUT_YUV420P 0
```

另外修补了几个的函数，例如增加了SDL_Quit()等。

**simplest_ffmpeg_player_su（SU版）**具体情况在上文中已经说明。



1.1版下载地址：[http://download.csdn.net/detail/leixiaohua1020/7814403](http://download.csdn.net/detail/leixiaohua1020/7814403)

SourceForge上已经更新。




**更新（2014.10.4）==========================**


**simplest ffmpeg player 1.2**


版本升级至1.2。

1.新版本在原版本的基础上增加了“flush_decoder”功能。当av_read_frame()循环退出的时候，实际上解码器中可能还包含剩余的几帧数据。因此需要通过“flush_decoder”将这几帧数据输出。“flush_decoder”功能简而言之即直接调用avcodec_decode_video2()获得AVFrame，而不再向解码器传递AVPacket。参考代码如下：



```cpp
//FIX: Flush Frames remained in Codec
	while (1) {
		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if (ret < 0)
			break;
		if (!got_picture)
			break;
		sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
		//处理...
	}
```




具体信息参见文章：[avcodec_decode_video2()解码视频后丢帧的问题解决](http://blog.csdn.net/leixiaohua1020/article/details/19016109)

2.为了更好地适应Linux等其他操作系统，做到可以跨平台，去除掉了VC特有的一些函数。比如“#include "stdafx.h"”，“_tmain()”等等。




1.2版下载地址：[http://download.csdn.net/detail/leixiaohua1020/8001575](http://download.csdn.net/detail/leixiaohua1020/8001575)

SourceForge上已经更新。




**Linux版本=================================**

Linux下代码下载地址：

[http://download.csdn.net/detail/leixiaohua1020/7696879](http://download.csdn.net/detail/leixiaohua1020/7696879)


这个是Linux下的代码，在Ubuntu下测试可以运行，前提是安装了FFmpeg和SDL（版本1.2）。
编译命令：

`gcc simplest_ffmpeg_player.c -g -o smp.out -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lswscale`使用方法：


下列命令即可播放同一目录下的test.flv文件。


`./smp.out test.flv`


****

**更新-最终版（2015.2.12）==========================**


**simplest ffmpeg player 1 final**


这是该播放器源代码的最后一次更新，以后会把更新的重点集中在基于FFmpeg和SDL2.0的视频播放器。这次考虑到了跨平台的要求，源代码的调整幅度比较大。经过这次调整之后，源代码可以在以下平台编译通过：

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
cl simplest_ffmpeg_player.cpp /MD /link SDL.lib SDLmain.lib avcodec.lib ^
avformat.lib avutil.lib avdevice.lib avfilter.lib postproc.lib swresample.lib swscale.lib ^
/SUBSYSTEM:WINDOWS /OPT:NOREF
exit
```



> 
*MinGW：MinGW命令行下运行compile_mingw.sh即可使用MinGW的g++进行编译。编译命令如下。*




```
g++ simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.exe \
-I /usr/local/include -L /usr/local/lib \
-lmingw32 -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lswscale
```



> 
*GCC(Linux)：Linux命令行下运行compile_gcc.sh即可使用GCC进行编译。编译命令如下。*




```
gcc simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.out \
-I /usr/local/include -L /usr/local/lib -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lswscale
```



> 
*GCC(MacOS)：Mac终端下运行compile_gcc_mac.sh即可使用Mac 的GCC进行编译，Mac的GCC和Linux的GCC差别不大，但是使用SDL1.2的时候，必须加上“-framework Cocoa”参数，否则编译无法通过。编译命令如下。*




```
gcc simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.out \
-framework Cocoa -I /usr/local/include -L /usr/local/lib \
-lSDLmain -lSDL -lavformat -lavcodec -lavutil -lswscale
```


*PS：相关的编译命令已经保存到了工程文件夹中*


此外，该版本修正了在某些系统下（例如部分Ubuntu）SDL绿屏显示的问题，经过测试已经不再有绿屏现象。

CSDN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8443253](http://download.csdn.net/detail/leixiaohua1020/8443253)

SourceForge上已经更新。







## **FFMPEG相关学习资料**

SDL GUIDE 中文译本

[http://download.csdn.net/detail/leixiaohua1020/6389841](http://download.csdn.net/detail/leixiaohua1020/6389841)


ffdoc （FFMPEG的最完整教程）


[http://download.csdn.net/detail/leixiaohua1020/6377803](http://download.csdn.net/detail/leixiaohua1020/6377803)


如何用FFmpeg编写一个简单播放器


[http://download.csdn.net/detail/leixiaohua1020/6373783](http://download.csdn.net/detail/leixiaohua1020/6373783)







## 补充问题

补充1：旧版程序有一个小BUG，就是sws_getContext()之后，需要调用sws_freeContext()。否则长时间运行的话，会出现内存泄露的状况。更新版已经修复。

补充2：有人会疑惑，为什么解码后的pFrame不直接用于显示，而是调用swscale()转换之后进行显示？

如果不进行转换，而是直接调用SDL进行显示的话，会发现显示出来的图像是混乱的。关键问题在于解码后的pFrame的linesize里存储的不是图像的宽度，而是比宽度大一些的一个值。其原因目前还没有仔细调查（大概是出于性能的考虑）。例如分辨率为480x272的图像，解码后的视频的linesize[0]为512，而不是480。以第1行亮度像素（pFrame->data[0]）为例，从0-480存储的是亮度数据，而从480-512则存储的是无效的数据。因此需要使用swscale()进行转换。转换后去除了无效数据，linesize[0]变为480。就可以正常显示了。

![](https://img-blog.csdn.net/20140508163545859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










