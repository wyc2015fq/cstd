# 把自定义的demuxer加入ffmpeg源码 - DoubleLi - 博客园






.简介：把上一篇文章中的demuxer加入ffmpeg源码中去，使可以用命令行方式调用自定义的demuxer

### 第一步：

在libavformat目录下新建mkdemuxer.c和mkdemuxer.h，代码如下：

mkdemuxer.c：





**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46348515#)[copy](http://blog.csdn.net/dancing_night/article/details/46348515#)



- /*  
- *实现一个自己的demuxer并加入到demuxer链中去 
- *作者：缪国凯（MK）  
- *821486004@qq.com  
- *2015-6-3  
- */   
- 
- #include "mkdemuxer.h"  
- 
- typedef struct MKVideoDemuxerContext {  
- const AVClass *pclass;     /**< Class for private options. */  
- int width, height;        /**< Integers describing video size, set by a private option. */  
- char *pixel_format;       /**< Set by a private option. */  
-     AVRational framerate;     /**< AVRational describing framerate, set by a private option. */  
- } MKVideoDemuxerContext;  
- 
- int mkvideo_read_header(AVFormatContext *ctx)  
- {  
-     MKVideoDemuxerContext *s = ctx->priv_data;  
- enum AVPixelFormat pix_fmt;  
-     AVStream *st;  
- 
-     st = avformat_new_stream(ctx, NULL);  
- if (!st)  
- return AVERROR(ENOMEM);  
- 
-     st->codec->codec_type = AVMEDIA_TYPE_VIDEO;  
- 
-     st->codec->codec_id = ctx->iformat->raw_codec_id;  
- 
- //这里就简单的直接赋值为420p  
-     pix_fmt = AV_PIX_FMT_YUV420P;  
- 
-     st->time_base.num = s->framerate.den;  
-     st->time_base.den = s->framerate.num;  
-     st->pts_wrap_bits = 64;  
- 
- 
-     st->codec->width  = s->width;  
-     st->codec->height = s->height;  
-     st->codec->pix_fmt = pix_fmt;  
- 
-     AVRational tmpRa;  
-     tmpRa.den = 1;  
-     tmpRa.num = 8;  
-     st->codec->bit_rate = av_rescale_q(avpicture_get_size(st->codec->pix_fmt, s->width, s->height),  
-         tmpRa, st->time_base);  
- 
- return 0;  
- }  
- 
- int mkvideo_read_packet(AVFormatContext *s, AVPacket *pkt)  
- {  
- int packet_size, ret, width, height;  
-     AVStream *st = s->streams[0];  
- 
-     width = st->codec->width;  
-     height = st->codec->height;  
- 
-     packet_size = avpicture_get_size(st->codec->pix_fmt, width, height);  
- if (packet_size < 0)  
- return -1;  
- 
-     ret = av_get_packet(s->pb, pkt, packet_size);  
-     pkt->pts = pkt->dts = pkt->pos / packet_size;  
- 
-     pkt->stream_index = 0;  
- if (ret < 0)  
- return ret;  
- 
- return 0;  
- }  
- 
- #define OFFSET(x) offsetof(MKVideoDemuxerContext, x)  
- #define DEC AV_OPT_FLAG_DECODING_PARAM  
- 
- static const AVOption mk_options[] =   
- {  
-     { "video_size", "set frame size", OFFSET(width), AV_OPT_TYPE_IMAGE_SIZE, {.str = NULL}, 0, 0, DEC },  
-     { "pixel_format", "set pixel format", OFFSET(pixel_format), AV_OPT_TYPE_STRING, {.str = "yuv420p"}, 0, 0, DEC },  
-     { "framerate", "set frame rate", OFFSET(framerate), AV_OPT_TYPE_VIDEO_RATE, {.str = "25"}, 0, 0, DEC },  
-     { NULL },  
- };  
- 
- static const AVClass mk_demuxer_class = {  
-     .class_name = "mk video demuxer",  
-     .item_name  = av_default_item_name,  
-     .option     = mk_options,  
-     .version    = LIBAVUTIL_VERSION_INT,  
- };  
- 
- AVInputFormat ff_mk_demuxer = {  
-     .name           = "mk",  
-     .long_name      = NULL_IF_CONFIG_SMALL("MK Video Container"),  
-     .flags          = AVFMT_GENERIC_INDEX,  
-     .extensions     = "mk",   
-     .priv_class     = &mk_demuxer_class,      
-     .raw_codec_id   = AV_CODEC_ID_RAWVIDEO,  
-     .priv_data_size = sizeof(MKVideoDemuxerContext),  
- 
-     .read_header    = mkvideo_read_header,  
-     .read_packet    = mkvideo_read_packet,  
- };  








mkdemuxer.h：





**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46348515#)[copy](http://blog.csdn.net/dancing_night/article/details/46348515#)



- /*  
- *实现一个自己的demuxer并加入到demuxer链中去 
- *作者：缪国凯（MK）  
- *821486004@qq.com  
- *2015-6-3  
- */   
- 
- #ifndef AVFORMAT_MKDEMUXER_H  
- #define AVFORMAT_MKDEMUXER_H  
- 
- #include "libavutil/opt.h"  
- #include "avformat.h"  
- 
- int mkvideo_read_header(AVFormatContext *ctx);  
- int mkvideo_read_packet(AVFormatContext *s, AVPacket *pkt);  
- 
- #endif //AVFORMAT_MKDEMUXER_H  










### 第二步：

在libavformat下的allformats.c的av_register_all里加入：





**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46348515#)[copy](http://blog.csdn.net/dancing_night/article/details/46348515#)



- REGISTER_DEMUXER   (MK,              mk);//add ByMK for mk demuxer    




### 第三步：

在libavformat目录下的makefile文件的# muxers/demuxers后加入：





**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46348515#)[copy](http://blog.csdn.net/dancing_night/article/details/46348515#)



- OBJS-$(CONFIG_MK_DEMUXER)                  += mkdemuxer.o   




### 第四步：

在ffmpeg根目录的config.h里加入：





**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46348515#)[copy](http://blog.csdn.net/dancing_night/article/details/46348515#)



- #define CONFIG_MK_DEMUXER 1    








### 第五步：

如果不想重新configure，则在ffmpeg根目录的config.mak文件中加入：




**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46348515#)[copy](http://blog.csdn.net/dancing_night/article/details/46348515#)



- CONFIG_MK_DEMUXER=yes  





OK，按照正常的编译去编译ffmpeg，然后运行ffmpeg.exe输入ffmpeg -formats 看到有mk这个muxer了，如下：





**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46348515#)[copy](http://blog.csdn.net/dancing_night/article/details/46348515#)



- DE mk              mk (MK Video Container)  
-  E mkvtimestamp_v2 extract pts as timecode v2 format, as defined by mkvtoolnix  
- DE mlp             raw MLP  
- D  mlv             Magic Lantern Video (MLV)  
- D  mm              American Laser Games MM  
- DE mmf             Yamaha SMAF  
-  E mov             QuickTime / MOV  






再试一下命令：ffmpeg -s 1280x720 -i test.mk -y test.avi。

注意：当.mk格式作为输入的时候，必须加输入参数-s 分辨率，就和yuv作为输入一样（因为从本质上讲就是yuv...）。



成功！OK，大功告成。



原理在上一篇的博文中已经讲到了，下一步，实现自己的encoder。

from:http://blog.csdn.net/dancing_night/article/details/46348515









