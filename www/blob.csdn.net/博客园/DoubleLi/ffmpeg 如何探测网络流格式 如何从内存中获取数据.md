# ffmpeg 如何探测网络流格式/如何从内存中获取数据 - DoubleLi - 博客园






一般ffmpeg都是直接从文件中读取或者从网络流中读取，比如rtp://xx.xx.xx.xx:xxxx。

事实上也支持从内存中获取。

函数avio_alloc_context()实现该功能。



**[html]**[view plain](http://blog.csdn.net/rootusers/article/details/42551935#)[copy](http://blog.csdn.net/rootusers/article/details/42551935#)

[print](http://blog.csdn.net/rootusers/article/details/42551935#)[?](http://blog.csdn.net/rootusers/article/details/42551935#)

- AVIOContext *avio_alloc_context(  
-                   unsigned char *buffer,  
-                   int buffer_size,  
-                   int write_flag,  
-                   void *opaque,  
-                   int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),//重写该函数，指定从内存中读取的方法，将buf_size字节大小的数据保存到buf  
-                   int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),//对应的这是写内存的函数  
-                   int64_t (*seek)(void *opaque, int64_t offset, int whence));  


对于探测网络媒体流个格式，也可以用此种方法，先接收数据，然后探测。



下面贴出代码：





**[cpp]**[view plain](http://blog.csdn.net/rootusers/article/details/42551935#)[copy](http://blog.csdn.net/rootusers/article/details/42551935#)

[print](http://blog.csdn.net/rootusers/article/details/42551935#)[?](http://blog.csdn.net/rootusers/article/details/42551935#)

- /* 
- *author tongli 
- */  
- extern "C"{  
- #include "libavformat/avformat.h"  
- #include "libavcodec/avcodec.h"  
- #include "libavutil/avutil.h"  
- }  
- #define BUF_SIZE 4096*500  
- 
- FILE* file;  
- //实现read_packet函数，从文件中读取模拟的是从内存中获取，同样可以实现为接收网络流  
- int read_packet(void *opaque, uint8_t *buf, int buf_size)  
- {  
- int n = 0;  
- if (!feof(file)){  
-         n = fread(buf, 1, buf_size, file);  
- return n;  
-     }else  
- return -1;  
- }  
- 
- int main(int argc, char** argv)  
- {  
-     file = fopen("2.mp4", "rb");  
- if (file == NULL)  
- return -1;  
-     av_register_all();  
-     AVIOContext* pb = NULL;  
-     AVInputFormat* piFmt = NULL;  
-     AVInputFormat* pFmt = NULL;  
- 
-     uint8_t* buf = (uint8_t*)av_mallocz(sizeof(uint8_t)* BUF_SIZE);  
- 
-     pb = avio_alloc_context(buf, BUF_SIZE, 0, NULL, read_packet, NULL, NULL);  
- if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0)//探测从内存中获取到的媒体流的格式  
-     {  
-         fprintf(stderr, "probe format failed\n");  
- return -1;   
-     }  
- else{  
-         fprintf(stdout, "format:%s[%s]\n", piFmt->name, piFmt->long_name);  
-     }  
- return 0;  
- }  



下面实现一个简单的例子，从内存中读取，然后播放。









**[cpp]**[view plain](http://blog.csdn.net/rootusers/article/details/42551935#)[copy](http://blog.csdn.net/rootusers/article/details/42551935#)

[print](http://blog.csdn.net/rootusers/article/details/42551935#)[?](http://blog.csdn.net/rootusers/article/details/42551935#)

- <pre name="code" class="cpp">/* 
- *author tongli 
- */  
- #include <stdio.h>  
- #include <direct.h>  
- #include <io.h>  
- extern "C"{  
- #include "libavformat/avformat.h"  
- #include "libavcodec/avcodec.h"  
- #include "libavutil/avutil.h"  
- #include "libswscale/swscale.h"  
- #include "libavformat/avio.h"  
- #include "sdl/SDL.h"  
- }  
- #define BUF_SIZE 4096 * 500  
- FILE* file;  
- 
- int read_data(void *opaque, uint8_t *buf, int buf_size)  
- {  
- int n = 0;  
- if (!feof(file)){  
-         n = fread(buf, 1, buf_size, file);  
- return n;  
-     }  
- else  
- return -1;  
- }  
- 
- int main(int argc, char* argv[])  
- {  
-     av_register_all();  
- //file = fopen("h2.dat", "rb");  
-     file = fopen("3.mp4", "rb+");  
- if (file == NULL)  
- return -1;  
-     AVFormatContext *pFormatCtx;  
- int             i, videoindex;  
-     AVCodecContext  *pCodecCtx;  
-     AVCodec         *pCodec;  
-     AVIOContext* pb = NULL;  
-     AVInputFormat* piFmt = NULL;  
- 
-     uint8_t* buf = (uint8_t*)av_mallocz(sizeof(uint8_t)* BUF_SIZE);  
- 
-     pb = avio_alloc_context(buf, BUF_SIZE, 0, NULL, read_data, NULL, NULL);  
- if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0)  
-     {  
-         fprintf(stderr, "probe format failed\n");  
- return -1;  
-     }  
- else{  
-         fprintf(stdout, "format:%s[%s]\n", piFmt->name, piFmt->long_name);  
-     }  
-     pFormatCtx = avformat_alloc_context();  
-     pFormatCtx->pb = pb;  
- 
- if (avformat_open_input(&pFormatCtx, "", piFmt, NULL) != 0){//iformat，priv_data赋值，pb, nbstreams,streams为null  
-         printf("Couldn't open input stream.（无法打开输入流）\n");  
- return -1;  
-     }  
- if (avformat_find_stream_info(pFormatCtx, NULL)<0)//nbstreams,streams赋值， pb还是为null  
-     {  
-         printf("Couldn't find stream information.（无法获取流信息）\n");  
- return -1;  
-     }  
-     videoindex = -1;  
- for (i = 0; i<pFormatCtx->nb_streams; i++) //一般情况下，一个媒体只有两个流，视频和音频流即streams[0],stream[1]  
- if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)  
-     {//找到视频流  
-         videoindex = i;//在nb_streams视频流的索引  
- break;  
-     }  
- if (videoindex == -1)  
-     {  
-         printf("Didn't find a video stream.（没有找到视频流）\n");  
- return -1;  
-     }  
-     pCodecCtx = pFormatCtx->streams[videoindex]->codec;  
-     pCodec = avcodec_find_decoder(pCodecCtx->codec_id);  
- if (pCodec == NULL)  
-     {  
-         printf("Codec not found.（没有找到解码器）\n");  
- return -1;  
-     }  
- if (avcodec_open2(pCodecCtx, pCodec, NULL)<0)  
-     {  
-         printf("Could not open codec.（无法打开解码器）\n");  
- return -1;  
-     }  
-     AVFrame *pFrame, *pFrameYUV;  
-     pFrame = av_frame_alloc();  
-     pFrameYUV = av_frame_alloc();  
- 
-     uint8_t *out_buffer;  
-     out_buffer = new uint8_t[avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height)];  
- 
-     avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);  
- //------------SDL----------------  
- if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
-         printf("Could not initialize SDL - %s\n", SDL_GetError());  
- return -1;  
-     }  
-     SDL_Surface *screen;  
-     screen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 0, 0);  
- if (!screen) {  
-         printf("SDL: could not set video mode - exiting\n");  
- return -1;  
-     }  
-     SDL_Overlay *bmp;  
-     bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height, SDL_YV12_OVERLAY, screen);  
-     SDL_Rect rect;  
- //---------------  
- int ret, got_picture;  
- int y_size = pCodecCtx->width * pCodecCtx->height;  
- 
-     AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));  
-     av_new_packet(packet, y_size);  
- 
- struct SwsContext *img_convert_ctx;  
-     img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,  
-         AV_PIX_FMT_YUVJ420P/*pCodecCtx->pix_fmt*/, pCodecCtx->width, pCodecCtx->height,  
-         PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);  
- //------------------------------  
- while (av_read_frame(pFormatCtx, packet) >= 0)  
-     {  
- if (packet->stream_index == videoindex)  
-         {  
-             ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);  
- if (ret < 0)  
-             {  
-                 printf("Decode Error.（解码错误）\n");  
- return -1;  
-             }  
- if (got_picture)  
-             {  
-                 sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);  
- 
-                 SDL_LockYUVOverlay(bmp);  
-                 bmp->pixels[0] = pFrameYUV->data[0];  
-                 bmp->pixels[2] = pFrameYUV->data[1];  
-                 bmp->pixels[1] = pFrameYUV->data[2];  
-                 bmp->pitches[0] = pFrameYUV->linesize[0];  
-                 bmp->pitches[2] = pFrameYUV->linesize[1];  
-                 bmp->pitches[1] = pFrameYUV->linesize[2];  
-                 SDL_UnlockYUVOverlay(bmp);  
-                 rect.x = 0;  
-                 rect.y = 0;  
-                 rect.w = pCodecCtx->width;  
-                 rect.h = pCodecCtx->height;  
-                 SDL_DisplayYUVOverlay(bmp, &rect);  
- //延时40ms  
-                 SDL_Delay(40);  
-             }  
-         }  
-         av_free_packet(packet);  
-     }  
-     sws_freeContext(img_convert_ctx);  
- 
- delete[] out_buffer;  
-     av_free(pFrameYUV);  
-     avcodec_close(pCodecCtx);  
-     avformat_close_input(&pFormatCtx);  
- 
- return 0;  
- }  
- 










