# 把自定义的decoder加入ffmpeg源码 - DoubleLi - 博客园






### 第一步：

在libavcodec目录下新建mkdecoder.c，并加入一下代码：




**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- /*  
- *实现一个自己的decoder,编码工作其实就是把pkt的数据拷贝到frame 
- *作者：缪国凯（MK）  
- *821486004@qq.com  
- *2015-6-5  
- */   
- #include "avcodec.h"  
- 
- static av_cold int mk_init_decoder(AVCodecContext *avctx)  
- {  
-     printf("init mk decoder\n");  
- return 0;  
- }  
- 
- static int mk_decode(AVCodecContext *avctx, void *data, int *got_frame,  
-     AVPacket *avpkt)  
- {  
-     AVFrame   *frame   = (AVFrame*)data;  
-     AVPicture *picture = (AVPicture*)data;  
- const uint8_t *buf             = avpkt->data;  
- int buf_size                   = avpkt->size;  
- 
- int size = avpicture_get_size(avctx->pix_fmt, avctx->width,  
-         avctx->height);  
- 
-     frame->pict_type        = AV_PICTURE_TYPE_I;  
-     frame->key_frame        = 1;  
- 
-     frame->buf[0] = av_buffer_alloc(size);  
- 
-     memcpy(frame->buf[0]->data, buf, buf_size);  
- 
- int res = 0;  
- if ((res = avpicture_fill(picture, frame->buf[0]->data, avctx->pix_fmt,  
-         avctx->width, avctx->height)) < 0)   
-     {  
-             av_buffer_unref(&frame->buf[0]);  
- return res;  
-     }  
- 
-     *got_frame = 1;  
- return 0;  
- }  
- 
- static av_cold int mk_close_decoder(AVCodecContext *avctx)  
- {  
-     printf("init mk decoder\n");  
- return 0;  
- }  
- 
- AVCodec ff_mkvideo_decoder = {  
-     .name           = "mkvideo",  
-     .long_name      = "mk video",  
-     .type           = AVMEDIA_TYPE_VIDEO,  
-     .id             = AV_CODEC_ID_MKVIDEO,  
-     .capabilities   = CODEC_CAP_PARAM_CHANGE,     
-     .init           = mk_init_decoder,    
-     .decode         = mk_decode,  
-     .close          = mk_close_decoder,  
- };  



### 第二步：

在avcodec.h里的 enum AVCodecID 最后加入：




**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- AV_CODEC_ID_MKVIDEO,  



### 第三步：

在allcodec.c的void avcodec_register_all(void)函数中加入



**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- REGISTER_DECODER(MKVIDEO,          mkvideo);    



### 第四步：

在libavcodec的makefile里加入：



**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- OBJS-$(CONFIG_MKVIDEO_DECODER)              += mkdecoder.o  



### 第五步：

在ffmpeg根目录config.h里加入：



**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- #define CONFIG_MKVIDEO_DECODER 1   



### 第六步：

在codec_desc.c的static const AVCodecDescriptor codec_descriptors[] 中加入：



**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- {  
-         .id        = AV_CODEC_ID_MKVIDEO,  
-         .type      = AVMEDIA_TYPE_VIDEO,  
-         .name      = "mkvideo",  
-         .long_name = NULL_IF_CONFIG_SMALL("mk video"),  
-         .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,  
- },  



### 第七步：

把以前实现的mkdemuxer的codeid改为



**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- AV_CODEC_ID_MKVIDEO  



### 第八步：

重新编译ffmpeg。

### 测试：

运行命令：ffmpeg -decoders：在显示中找到自己的decoder：




**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- VF...D mimic                Mimic  
- V....D mjpeg                MJPEG (Motion JPEG)  
- V....D mjpegb               Apple MJPEG-B  
- V..... mkvideo              mk video  


运行命令：



**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- ffmpeg -s 1280x720 -i test.mk -y test.avi  


生成test.avi，并在转码过程中打出在代码中添加的标记：



**[cpp]**[view plain](http://blog.csdn.net/dancing_night/article/details/46376511#)[copy](http://blog.csdn.net/dancing_night/article/details/46376511#)



- init mk decoder  



播放test.avi，能正确播放，ok，成功！









