# FFMPEG内存操作（二）从内存中读取数及数据格式的转换 - DoubleLi - 博客园






相关博客列表： 

[FFMPEG内存操作（一） avio_reading.c 回调读取数据到内存解析](http://blog.csdn.net/li_wen01/article/details/64132123)

[FFMPEG内存操作（二）从内存中读取数及数据格式的转换](http://blog.csdn.net/li_wen01/article/details/64904586)

[FFmpeg内存操作（三）内存转码器](http://blog.csdn.net/li_wen01/article/details/64905959)

    在雷神的《最简单的基于FFmpeg的内存读写例子（内存播放器）》中，它是设计回调函数从输入文件中读取数据。与FFMPEG 官方给出的avio_reading.c不同的是，雷神给的例子是当需要数据的时候，回调函数才去从输入文件读取数据，而avio_reading.c 则是直接全部数据读取到内存中待后面处理。

    在我的这个实例中，我是将读取到的输入文件解码成YUV420P数据格式。同时可以通过设置不同的数据格式和尺寸实现输出图像的拉伸缩小或是数据格式的装换。





**[objc]**[view plain](http://blog.csdn.net/li_wen01/article/details/64904586#)[copy](http://blog.csdn.net/li_wen01/article/details/64904586#)

[print](http://blog.csdn.net/li_wen01/article/details/64904586#)[?](http://blog.csdn.net/li_wen01/article/details/64904586#)

- /*=============================================================================    
- #     FileName: ffmpeg_mem_read.c    
- #         Desc: an example of ffmpeg read from memory 
- #       Author: licaibiao    
- #   LastChange: 2017-03-21     
- =============================================================================*/    
- #include <stdio.h>  
- 
- #define __STDC_CONSTANT_MACROS  
- #include "avcodec.h"  
- #include "avformat.h"  
- #include "swscale.h"  
- 
- //#define QUARTER_SHOW     
- 
- charchar *input_name = "cuc60anniversary_start.ts";  
- 
- FILEFILE *fp_open = NULL;  
- 
- int read_buffer(voidvoid *opaque, uint8_t *buf, int buf_size){  
- if(!feof(fp_open)){  
- int true_size=fread(buf,1,buf_size,fp_open);  
- return true_size;  
-     }else{  
- return -1;  
-     }  
- }  
- 
- int main(int argc, char* argv[])  
- {  
- AVFormatContext *pFormatCtx;  
-     AVCodecContext  *pCodecCtx;  
-     AVCodec         *pCodec;  
-     AVIOContext     *avio;  
-     AVFrame         *pFrame;  
-     AVFrame         *pFrameYUV;  
-     AVPacket        *packet;  
- 
- struct SwsContext *img_convert_ctx;  
- 
- int             videoindex;  
- int             i;  
- int             ret;  
- int             got_picture;  
- int             y_size;  
-     unsigned char   *aviobuffer;  
- FILE            *fp_yuv;  
- 
- 
- //fp_open = fopen(argv[1],"rb+");  
-     fp_open = fopen(input_name, "rb+");  
-     fp_yuv  = fopen("output.yuv", "wb+");       
- 
-     av_register_all();  
-     pFormatCtx = avformat_alloc_context();  
- 
-     aviobuffer=(unsigned charchar *)av_malloc(32768);  
-     avio = avio_alloc_context(aviobuffer, 32768,0,NULL,read_buffer,NULL,NULL);  
- 
- /* Open an input stream and read the header. */  
-     pFormatCtx->pb = avio;  
- if(avformat_open_input(&pFormatCtx,NULL,NULL,NULL)!=0){  
-         printf("Couldn't open input stream.\n");  
- return -1;  
-     }  
- 
- /* Read packets of a media file to get stream information. */  
- if(avformat_find_stream_info(pFormatCtx,NULL)<0){  
-         printf("Couldn't find stream information.\n");  
- return -1;  
-     }  
- 
-     videoindex = -1;  
- for(i=0; i<pFormatCtx->nb_streams; i++) {  
- if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){  
-             videoindex=i;  
- break;  
-         }  
-     }   
- 
- if(videoindex==-1){  
-         printf("Didn't find a video stream.\n");  
- return -1;  
-     }  
- 
-     av_dump_format(pFormatCtx, 0, input_name, 0);  
- 
- /* Find a registered decoder with a matching codec ID */  
-     pCodecCtx = pFormatCtx->streams[videoindex]->codec;  
-     pCodec    = avcodec_find_decoder(pCodecCtx->codec_id);     
- if(pCodec==NULL){  
-         printf("Codec not found.\n");  
- return -1;  
-     }  
- 
- /* Initialize the AVCodecContext to use the given AVCodec */  
- if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){  
-         printf("Could not open codec.\n");  
- return -1;  
-     }  
- 
-     pFrame    = av_frame_alloc();  
-     pFrameYUV = av_frame_alloc();  
- uint8_t *out_buffer=(uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));  
-     avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);  
- 
- /*  Allocate and return an SwsContext. */  
- /* srcW：源图像的宽 
-      * srcH：源图像的高 
-      * srcFormat：源图像的像素格式 
-      * dstW：目标图像的宽 
-      * dstH：目标图像的高 
-      * dstFormat：目标图像的像素格式 
-      * flags：设定图像拉伸使用的算法 
-     */  
- #ifndef QUARTER_SHOW  
-     img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);   
- #else  
-     img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width/2, pCodecCtx->height/2, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);   
-     printf("out frame  width = %d, height = %d \n", pCodecCtx->width/2,pCodecCtx->height/2);  
- #endif  
-     packet = (AVPacket *)av_malloc(sizeof(AVPacket));  
- while(av_read_frame(pFormatCtx, packet) >= 0){  
- if(packet->stream_index == videoindex){  
-             ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);  
- if(ret < 0){  
-                 printf("Decode Error.\n");  
- return -1;  
-             }  
- if(got_picture){  
- /* Scale the image slice in srcSlice and put the resulting scaled slice in the image in dst. 图像处理函数 */  
- /* c             the scaling context previously created with  sws_getContext() 
-                  * srcSlice      the array containing the pointers to the planes of the source slice 
-                  * srcStride     the array containing the strides for each plane of 
-                  * srcSliceY     the position in the source image of the slice to process, that is the number (counted starting from 
-                                  zero) in the image of the first row of the slice 输入图像数据的第多少列开始逐行扫描，通常设为0 
-                  * srcSliceH     the height of the source slice, that is the number of rows in the slice 为需要扫描多少行，通常为输入图像数据的高度 
-                  * dst           the array containing the pointers to the planes of the destination image 
-                  * dstStride     the array containing the strides for each plane of the destination image 
-                  */    
-                 sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);  
- 
- #ifndef QUARTER_SHOW  
- // printf("pFrameYUV->height   = %d\n ",pFrameYUV->height);  
- // printf("pFrameYUV->width    = %d\n ", pFrameYUV->width);  
- // printf("pFrameYUV->pkt_size = %d\n ",pFrameYUV->pkt_size);  
- 
-                 y_size = pCodecCtx->width*pCodecCtx->height;   
-                 fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y   
-                 fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U  
-                 fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V  
- #else  
- for(i=0; i<pCodecCtx->height/2; i++){  
-                     fwrite(pFrameYUV->data[0]+pCodecCtx->width * i ,1,pCodecCtx->width/2,fp_yuv);   
-                 }  
- for(i=0; i<pCodecCtx->height/2; i = i + 2){  
-                     fwrite(pFrameYUV->data[1]+pCodecCtx->width * i/4 ,1,pCodecCtx->width/4,fp_yuv);  
-                 }  
- 
- for(i=0; i<pCodecCtx->height/2; i = i + 2 ){               
-                     fwrite(pFrameYUV->data[2]+pCodecCtx->width * i/4 ,1,pCodecCtx->width/4,fp_yuv);   
-                 }      
- #endif               
- 
-             }  
-         }  
-         av_free_packet(packet);  
-     }  
-     sws_freeContext(img_convert_ctx);  
- 
-     fclose(fp_yuv);  
-     fclose(fp_open);  
-     av_free(out_buffer);  
-     av_free(pFrameYUV);  
-     avcodec_close(pCodecCtx);  
-     avformat_close_input(&pFormatCtx);  
- 
- return 0;  
- }  


    正常操作是将输入文件解码成YUV数据。



    当我们对sws_getContext  设置不同参数的时候，我们可以得到不同的输出数据。在这里需要注意几点：

    （1）out_buffer 的大小要跟着sws_getContext  参数的设置而改变，如果out_buffer分配得比输出数据小，会出现内存溢出问题。

    （2）sws_scale 函数中，我们一般设置从输入数据的第0行开始扫描,扫描的高度（也就是行数）一般是输入数据的高度。

    （3）得到的YUV数据是储存在pFrameYUV->data 的三个分量里，需要分开读取。



举例：

    在sws_getContext 中设置输出格式的宽和高都是输入格式的一半。正常的显示应该是图像变为了原来的1/4大小。如果我们还是按原图尺寸提取输出数据：





**[objc]**[view plain](http://blog.csdn.net/li_wen01/article/details/64904586#)[copy](http://blog.csdn.net/li_wen01/article/details/64904586#)

[print](http://blog.csdn.net/li_wen01/article/details/64904586#)[?](http://blog.csdn.net/li_wen01/article/details/64904586#)

-  y_size = pCodecCtx->width*pCodecCtx->height;   
- fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y   
- fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U  
- fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V  


得到的图像将会是：





![](http://img.blog.csdn.net/20170322095112767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlfd2VuMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    如上图，图像显示是正常的，但同时也提取了很多的无用数据，实际图像并没有缩小到原来的1/4



    所以我们需要自己对输出数据再重组：





**[objc]**[view plain](http://blog.csdn.net/li_wen01/article/details/64904586#)[copy](http://blog.csdn.net/li_wen01/article/details/64904586#)

[print](http://blog.csdn.net/li_wen01/article/details/64904586#)[?](http://blog.csdn.net/li_wen01/article/details/64904586#)

- for(i=0; i<pCodecCtx->height/2; i++){  
-     fwrite(pFrameYUV->data[0]+pCodecCtx->width * i ,1,pCodecCtx->width/2,fp_yuv);   
- }  
- for(i=0; i<pCodecCtx->height/2; i = i + 2){  
-     fwrite(pFrameYUV->data[1]+pCodecCtx->width * i/4 ,1,pCodecCtx->width/4,fp_yuv);  
- }  
- 
- for(i=0; i<pCodecCtx->height/2; i = i + 2 ){               
-     fwrite(pFrameYUV->data[2]+pCodecCtx->width * i/4 ,1,pCodecCtx->width/4,fp_yuv);   
- }   


    分别重新提取了Y U V 三个分量上的数据，得到图像如下：





![](http://img.blog.csdn.net/20170322095626119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlfd2VuMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    在读取输出数据的时候，如果发现读取的输出数据显示有问题，最好是使用UltraEdit 工具对某帧数据进行分析，先确认输出的一帧数据是否正常，然后才进行视频格式的转换。比如出现下面的这种情况



![](http://img.blog.csdn.net/20170322100310409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlfd2VuMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    从图像中大概能分析出来，Y分量数据是正常的，图像右半边的UV分量可能丢失。使用UltraEdit查看原始数据，如下图。我们可以看到从地址0xe150开始有一段是全0的数据，因此我们可以到我们的程序中去检查是否UV分量读取错误。

![](http://img.blog.csdn.net/20170322101214343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlfd2VuMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



    总结：我们直接使用sws_getContext和sws_scale 可以直接对图像进行拉伸和缩放，同时可以进行数据格式的转换，只需要设置sws_getContext 就可以了，非常简单。但是需要注意，输出的数据需要我们自己重新组合，否则读取到的输出数据很有可能出错。

from:http://blog.csdn.net/li_wen01/article/details/64904586









