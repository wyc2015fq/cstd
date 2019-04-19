# 如何强制ffmpeg编码时输出一个关键帧 - maopig的专栏 - CSDN博客
2011年09月10日 16:03:22[maopig](https://me.csdn.net/maopig)阅读数：2983
原文地址：http://www.ffmpeg.com.cn/index.php/%E5%A6%82%E4%BD%95%E5%BC%BA%E5%88%B6ffmpeg%E7%BC%96%E7%A0%81%E6%97%B6%E8%BE%93%E5%87%BA%E4%B8%80%E4%B8%AA%E5%85%B3%E9%94%AE%E5%B8%A7
如何强制ffmpeg编码时输出一个关键帧 
AVCodecContext *c //编码器环境句柄
AVFrame* f //需要编码的一帧视频
 在编码前设置
f->pict_type=FF_I_TYPE; 
f->key_frame=1; 
 然后编码
*outsize = avcodec_encode_video(c, temp, outbuf_size, f);
 则编码之后通过如下参数判断是否为关键帧：
key_frame=c->coded_frame->key_frame; 
pict_type=c->coded_frame->pict_type;
