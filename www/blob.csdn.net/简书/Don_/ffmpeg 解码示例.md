# ffmpeg 解码示例 - 简书


# ffmpeg 解码示例



### 相关

[x264编码](https://www.jianshu.com/p/7e2e02500a3a)
[ffmpeg 编码示例](https://www.jianshu.com/p/33591c32a15b)
[码流控制](https://www.jianshu.com/p/f8c713f2e63d)
[h264编解码末尾丢帧](https://www.jianshu.com/p/41b313664f60)
### 示例
- 
编码前初始化，返回视频宽高

int* decode_init(char * in_filename)

{

avcodec_register_all();

av_register_all();

decode_frame = av_frame_alloc();

AVPacket pkt;

//获取各种上下文

if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {

retu = -1;

goto end;

}

if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {

retu = -2;

goto end;

}

//例遍流，找到流所对应得下标

for (i = 0; i < ifmt_ctx->nb_streams; i++) {

/ /Create output AVStream according to input AVStream

AVFormatContext *ofmt_ctx;

AVStream *in_stream = ifmt_ctx->streams[i];

AVStream *out_stream = NULL;
```
if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
             videoindex = i;

         }
     }
     //open decoder
     video_dec_ctx = ifmt_ctx->streams[videoindex]->codec;
     video_dec = avcodec_find_decoder(video_dec_ctx->codec_id);
     if(avcodec_open2(video_dec_ctx,video_dec,NULL)<0)
     {
         retu = -3;
         goto end;
     }
     double g_height = ifmt_ctx->streams[videoindex]->codec->height;
     double g_width = ifmt_ctx->streams[videoindex]->codec->width;

     int* wh = malloc(sizeof(int)*2);
     wh[0] = g_width;
     wh[1] = g_height;
 end: 
     if(retu != 0)
         return NULL;
     else
         return wh;
 }
```
- 
解码一帧

AVFrame* decode_next()

{

if(decode_frame!=NULL)

av_frame_unref(decode_frame);

int ret = 0;

int got_frame = 0;

int error_time = 0;

while(1)

{

//Get an AVPacket

if (av_read_frame(ifmt_ctx, &pkt) < 0)

{

av_free_packet(&pkt);

goto decode_free;

}

if (pkt.stream_index == videoindex)

{

ret = avcodec_decode_video2(video_dec_ctx,decode_frame, &got_frame, &pkt);

if(got_frame)

goto decode_success;

else

{

if(failed_times ++<30)

continue;

else

goto decode_free;

}

}

av_free_packet(&pkt);

}

decode_free:

av_free_packet(&pkt);

return NULL;
```
decode_success:
     return decode_frame;
 }
```

仅仅是在读取packet之后就停止解码会丢帧，参看

所以要在读取packet失败之后接着调用解码，获取剩余的帧。使用一个变量failed_times 记录编码器中存在的帧数。在读取结束之后接着调用failed_times 次avcodec_decode_video2

int failed_times  = 0；

int read_packet_failed = 0;

AVFrame* decode_next()

{

if(decode_frame!=NULL;

av_frame_unref(decode_frame);

int ret = 0;

int got_frame = 0;

int error_time = 0;

while(1)

{

if (av_read_frame(ifmt_ctx, &pkt) < 0)

{

read_packet_failed  = 1;

av_free_packet(&pkt);

if(failed_times > 0)

{

failed_times--;

goto decode;

}

else

goto decode_free;

}

if (pkt.stream_index == videoindex)

{

decode:

ret = avcodec_decode_video2(video_dec_ctx,decode_frame, &got_frame, &pkt);

if(got_frame)

goto decode_success;

else

{

if(read_packet_failed)

goto decode_free;

else

{

failed_times ++;

continue;

}

else

goto decode_free;

}

}

av_free_packet(&pkt);

}

decode_free:

av_free_packet(&pkt);

return NULL;

decode_success:

return decode_frame;

}- 
解码完成之后释放
``int decode_release() { avcodec_close(video_dec_ctx); avformat_close_input(&ifmt_ctx); if(decode_frame!=NULL) { av_frame_free(&decode_frame); decode_frame = NULL; } }








