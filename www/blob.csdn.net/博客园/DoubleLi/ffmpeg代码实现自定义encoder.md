# ffmpeg代码实现自定义encoder - DoubleLi - 博客园






### 1、概述

本文主要讲述如何用ffmpeg代码实现自己的encoder。

### 2、代码




**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46360851#)[copy](http://blog.csdn.net/dancing_night/article/details/46360851#)



- /*  
- *本程序主要实现一个自己的encoder并加入到encoder链中去,供api调用 
- *作者：缪国凯（MK）  
- *821486004@qq.com  
- *2015-6-4  
- */   
- 
- #include "stdafx.h"  
- 
- #ifdef __cplusplus  
- extern "C"  
- {  
- #endif  
- #include <libavformat/avformat.h>  
- #include <libavcodec/avcodec.h>  
- #include <libavutil/pixdesc.h>  
- #  
- #ifdef __cplusplus  
- };  
- #endif  
- 
- #pragma comment(lib, "avcodec.lib")  
- #pragma comment(lib, "avformat.lib")  
- #pragma comment(lib, "avutil.lib")  
- //#pragma comment(lib, "avdevice.lib")  
- //#pragma comment(lib, "avfilter.lib")  
- //#pragma comment(lib, "postproc.lib")  
- //#pragma comment(lib, "swresample.lib")  
- //#pragma comment(lib, "swscale.lib")  
- 
- static av_cold int mk_encode_init(AVCodecContext *avctx)  
- {  
- const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(avctx->pix_fmt);  
- 
-     avctx->coded_frame = av_frame_alloc();  
-     avctx->coded_frame->pict_type = AV_PICTURE_TYPE_I;  
-     avctx->bits_per_coded_sample = av_get_bits_per_pixel(desc);  
- if(!avctx->codec_tag)  
-         avctx->codec_tag = avcodec_pix_fmt_to_codec_tag(avctx->pix_fmt);  
- return 0;  
- }  
- 
- static int mk_encode(AVCodecContext *avctx, AVPacket *pkt,  
- const AVFrame *frame, int *got_packet)  
- {  
- int ret = avpicture_get_size(avctx->pix_fmt, avctx->width, avctx->height);  
- 
- if (ret < 0)  
- return ret;  
- 
- if (pkt->data == NULL && pkt->size == 0)  
-     {  
-         av_new_packet(pkt,ret);  
-         pkt->size = ret;  
-     }  
- 
- //  if ((ret = ff_alloc_packet2(avctx, pkt, ret)) < 0)  
- //      return ret;  
- 
- if ((ret = avpicture_layout((const AVPicture *)frame, avctx->pix_fmt, avctx->width,  
-         avctx->height, pkt->data, pkt->size)) < 0)  
- return ret;  
- 
- //  if(avctx->codec_tag == AV_RL32("yuv2") && ret > 0 &&  
- //      avctx->pix_fmt   == AV_PIX_FMT_YUYV422)   
- //  {  
- //          int x;  
- //          for(x = 1; x < avctx->height*avctx->width*2; x += 2)  
- //              pkt->data[x] ^= 0x80;  
- //  }  
-     pkt->flags |= AV_PKT_FLAG_KEY;  
-     *got_packet = 1;  
- return 0;  
- }  
- 
- static av_cold int mk_close(AVCodecContext *avctx)  
- {  
-     av_frame_free(&avctx->coded_frame);  
- return 0;  
- }  
- 
- AVCodec ff_mkvideo_encoder = {  
- /*.name           = */"mkvideo",  
- /*.long_name      = */"mk video",  
- /*.type           = */AVMEDIA_TYPE_VIDEO,  
- /*.id             = */AV_CODEC_ID_MKVIDEO,  
- /*.capabilities = */0,  
- /*.supported_framerates = */NULL,  
- /*.pix_fmts     = */NULL,  
- /*.supported_samplerates = */NULL,  
- /*.sample_fmts  = */NULL,  
- /*.channel_layouts = */NULL,  
- /*.max_lowres       = */0,  
- /*.priv_class       = */NULL,  
- /*.profiles     = */NULL,  
- /*.priv_data_size   = */0,  
- /*.next         = */NULL,  
- /*.init_thread_copy = */NULL,  
- /*.update_thread_context = */NULL,  
- /*.defaults     = */NULL,  
- /*.init_static_data = */NULL,     
- /*.init           = */mk_encode_init,  
- /*.encode_sub       = */NULL,  
- /*.encode2        = */mk_encode,  
- /*.decode           = */NULL,  
- /*.close          = */mk_close,  
- };  
- 
- void help()  
- {  
-     printf("**********************************************\n");  
-     printf("Usage:\n");  
-     printf("    MyMuxer [inputfile] [outputfile] \n");  
-     printf("\n");  
-     printf("Examples: \n");  
-     printf("    MyMuxer a.avi a.yuv \n");  
-     printf("**********************************************\n");    
- }  
- 
- int _tmain(int argc, _TCHAR* argv[])  
- {  
- if(argc < 3 || (!strcmp(argv[1],"--help")))  
-     {  
-         help();  
- return 0;  
-     }  
- 
-     av_register_all();  
-     avcodec_register(&ff_mkvideo_encoder);  
- 
-     AVFormatContext *in_fxt = NULL, *out_fxt = NULL;  
-     AVStream *out_stream = NULL;  
- int video_index = -1;  
- 
- if (avformat_open_input(&in_fxt, argv[1], NULL, NULL) < 0)  
-     {  
-         printf("can not open the input file context!\n");  
- goto end;  
-     }  
- if (avformat_find_stream_info(in_fxt, NULL) < 0)  
-     {  
-         printf("can not find the stream info!\n");  
- goto end;  
-     }  
- 
- if(avformat_alloc_output_context2(&out_fxt, NULL, NULL, argv[2]) < 0)  
-     {  
-         printf("can not alloc output context!\n");  
- goto end;  
-     }  
- 
- for (int i = 0; i < in_fxt->nb_streams; i++)  
-     {  
- if (in_fxt->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)  
-         {  
- //open decoder  
- if(0 > avcodec_open2(in_fxt->streams[i]->codec, avcodec_find_decoder(in_fxt->streams[i]->codec->codec_id), NULL))  
-             {  
-                 printf("can not find or open decoder!\n");  
- goto end;  
-             }  
-             video_index = i;  
- //new stream  
-             out_stream = avformat_new_stream(out_fxt, NULL);  
- if (!out_stream)  
-             {  
-                 printf("can not new stream for output!\n");  
- goto end;  
-             }  
- //set codec context param  
-             out_stream->codec->codec = avcodec_find_encoder(/*out_fxt->oformat->video_codec*/AV_CODEC_ID_MKVIDEO);  
-             out_stream->codec->height = in_fxt->streams[i]->codec->height;  
-             out_stream->codec->width = in_fxt->streams[i]->codec->width;  
- 
-             out_stream->codec->time_base = in_fxt->streams[i]->time_base;  
- //out_stream->codec->time_base.den = 25;  
- 
-             out_stream->codec->sample_aspect_ratio = in_fxt->streams[i]->codec->sample_aspect_ratio;           
-             out_stream->codec->pix_fmt = in_fxt->streams[i]->codec->pix_fmt;  
- 
-             out_stream->avg_frame_rate.den = out_stream->codec->time_base.num;  
-             out_stream->avg_frame_rate.num = out_stream->codec->time_base.den;  
- if (!out_stream->codec->codec)  
-             {  
-                 printf("can not find the encoder!\n");  
- goto end;  
-             }  
- if ((avcodec_open2(out_stream->codec, out_stream->codec->codec, NULL)) < 0)  
-             {  
-                 printf("can not open the encoder\n");  
- goto end;  
-             }  
- if (out_fxt->oformat->flags & AVFMT_GLOBALHEADER)  
-                 out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;  
- break;  
-         }  
-     }  
- 
- if (-1 == video_index)  
-     {  
-         printf("found no video stream in input file!\n");  
- goto end;  
-     }  
- 
- if (!(out_fxt->oformat->flags & AVFMT_NOFILE))  
-     {  
- if(avio_open(&out_fxt->pb, argv[2], AVIO_FLAG_WRITE) < 0)  
-         {  
-             printf("can not open output file handle!\n");  
- goto end;  
-         }  
-     }  
- 
- if(avformat_write_header(out_fxt, NULL) < 0)  
-     {  
-         printf("can not write the header of the output file!\n");  
- goto end;  
-     }  
- 
-     AVPacket pkt_in, pkt_out;  
-     AVFrame *frame;  
-     frame = av_frame_alloc();  
-     av_init_packet(&pkt_in);  
-     av_init_packet(&pkt_out);  
- int got_frame, got_picture;  
- int i = 0, frame_index = 0;  
- while(1)  
-     {  
-         got_frame = -1;  
-         got_picture = -1;  
- if (av_read_frame(in_fxt, &pkt_in) < 0)  
-         {  
- break;  
-         }  
- if (avcodec_decode_video2(in_fxt->streams[video_index]->codec, frame, &got_frame, &pkt_in) < 0)  
-         {  
-             printf("can not decoder a frame");  
- break;  
-         }  
-         av_free_packet(&pkt_in);  
- 
- if (got_frame)  
-         {  
-             frame->pts = i++;  
-             pkt_out.data = NULL;//主要这里必须自己初始化，或者必须置为null，不然ff_alloc_packet2函数会报错  
-             pkt_out.size = 0;  
- if (avcodec_encode_video2(out_stream->codec, &pkt_out, frame, &got_picture) < 0)  
-             {  
-                 printf("can not encode a frame!\n");  
- break;  
-             }  
- 
- if (got_picture)  
-             {  
-                 printf("Succeed to encode frame: %5d\tsize:%5d\n",frame_index,pkt_out.size);  
-                 pkt_out.stream_index = out_stream->index;  
-                 frame_index++;  
-                 av_write_frame(out_fxt, &pkt_out);  
-                 av_free_packet(&pkt_out);  
-             }  
-         }  
-     }  
-     av_frame_free(&frame);  
- 
-     av_write_trailer(out_fxt);  
- 
- //clean  
-     avcodec_close(out_stream->codec);  
-     avcodec_close(out_fxt->streams[video_index]->codec);  
- end:  
-     avformat_close_input(&in_fxt);  
- 
- if (out_fxt && !(out_fxt->oformat->flags & AVFMT_NOFILE))  
-     {  
-         avio_close(out_fxt->pb);  
-     }  
-     avformat_free_context(out_fxt);  
- return 0;  
- }  



### 3、解释

原理和前面的自定义的muxer、demuxer一样，在这里就不多说了。

from:http://blog.csdn.net/dancing_night/article/details/46360851









