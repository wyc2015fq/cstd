# ffmpeg编程推流器，收流器实现直播功能 - 建建的博客 - CSDN博客
2018年09月17日 09:43:09[纪建](https://me.csdn.net/u013898698)阅读数：96
基于FFmpeg的推流器（simplest ffmpeg streamer）。推流器的作用就是将本地的视频数据推送至流媒体服务器。本文记录的推流器，可以将本地的 MOV / AVI / MKV / MP4 / FLV 等格式的媒体文件，通过流媒体协议（例如RTMP，HTTP，UDP，TCP，RTP等等）以直播流的形式推送出去。目前主要的推流器如下：TMP流媒体服务器（例如 Flash Media Server，Red5，Wowza等等），本文实现推流器后用相应的flash media server实现直播流播放
收流器实现将RTMP流媒体保存成为一个本地的FLV文件。本文实现保存为一个mp4文件，由于FFmpeg本身支持很多的流媒体协议和封装格式，所以也支持其它的封装格式和流媒体协议
推流器代码如下：
- 
- 
`#include <stdio.h>`
- 
- 
`#define __STDC_CONSTANT_MACROS`
- 
- 
`#ifdef _WIN32`
- 
`//Windows`
- 
`extern "C"`
- 
`{`
- 
`#include "libavformat/avformat.h"`
- 
`#include "libavutil/mathematics.h"`
- 
`#include "libavutil/time.h"`
- 
`};`
- 
`#else`
- 
`//Linux...`
- 
`#ifdef __cplusplus`
- 
`extern "C"`
- 
`{`
- 
`#endif`
- 
`#include <libavformat/avformat.h>`
- 
`#include <libavutil/mathematics.h>`
- 
`#include <libavutil/time.h>`
- 
`#ifdef __cplusplus`
- 
`};`
- 
`#endif`
- 
`#endif`
- 
- 
`int main(int argc, char* argv[])`
- 
`{`
- 
`AVOutputFormat *ofmt = NULL;`
- 
`//Input AVFormatContext and Output AVFormatContext`
- 
`AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;`
- 
`AVPacket pkt;`
- 
`const char *in_filename, *out_filename;`
- 
`int ret, i;`
- 
`int videoindex=-1;`
- 
`int frame_index=0;`
- 
`int64_t start_time=0;`
- 
- 
`in_filename = "dakongtou.mp4";//输入URL（Input file URL）`
- 
`//in_filename = "shanghai03_p.h264";`
- 
- 
`out_filename = "rtmp://localhost/test/livestream";//输出 URL（Output URL）[RTMP]`
- 
- 
`av_register_all();`
- 
`//Network`
- 
`avformat_network_init();`
- 
`//Input`
- 
`if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {`
- 
`printf( "Could not open input file.");`
- 
`goto end;`
- 
`}`
- 
`if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {`
- 
`printf( "Failed to retrieve input stream information");`
- 
`goto end;`
- 
`}`
- 
- 
`for(i=0; i<ifmt_ctx->nb_streams; i++) `
- 
`if(ifmt_ctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){`
- 
`videoindex=i;`
- 
`break;`
- 
`}`
- 
- 
`av_dump_format(ifmt_ctx, 0, in_filename, 0);`
- 
- 
`//Output`
- 
- 
`avformat_alloc_output_context2(&ofmt_ctx, NULL, "flv", out_filename); //RTMP`
- 
`//avformat_alloc_output_context2(&ofmt_ctx, NULL, "mpegts", out_filename);//UDP`
- 
- 
`if (!ofmt_ctx) {`
- 
`printf( "Could not create output context\n");`
- 
`ret = AVERROR_UNKNOWN;`
- 
`goto end;`
- 
`}`
- 
`ofmt = ofmt_ctx->oformat;`
- 
`for (i = 0; i < ifmt_ctx->nb_streams; i++) {`
- 
`//Create output AVStream according to input AVStream`
- 
`AVStream *in_stream = ifmt_ctx->streams[i];`
- 
`AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);`
- 
`if (!out_stream) {`
- 
`printf( "Failed allocating output stream\n");`
- 
`ret = AVERROR_UNKNOWN;`
- 
`goto end;`
- 
`}`
- 
`//Copy the settings of AVCodecContext`
- 
`ret = avcodec_copy_context(out_stream->codec, in_stream->codec);`
- 
`if (ret < 0) {`
- 
`printf( "Failed to copy context from input to output stream codec context\n");`
- 
`goto end;`
- 
`}`
- 
`out_stream->codec->codec_tag = 0;`
- 
`if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)`
- 
`out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;`
- 
`}`
- 
`//Dump Format------------------`
- 
`av_dump_format(ofmt_ctx, 0, out_filename, 1);`
- 
`//Open output URL`
- 
`if (!(ofmt->flags & AVFMT_NOFILE)) {`
- 
`ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);`
- 
`if (ret < 0) {`
- 
`printf( "Could not open output URL '%s'", out_filename);`
- 
`goto end;`
- 
`}`
- 
`}`
- 
`//Write file header`
- 
`ret = avformat_write_header(ofmt_ctx, NULL);`
- 
`if (ret < 0) {`
- 
`printf( "Error occurred when opening output URL\n");`
- 
`goto end;`
- 
`}`
- 
- 
`start_time=av_gettime();`
- 
`while (1) {`
- 
`AVStream *in_stream, *out_stream;`
- 
`//Get an AVPacket`
- 
`ret = av_read_frame(ifmt_ctx, &pkt);`
- 
`if (ret < 0)`
- 
`break;`
- 
`//FIX：No PTS (Example: Raw H.264)`
- 
`//Simple Write PTS`
- 
`if(pkt.pts==AV_NOPTS_VALUE){`
- 
`//Write PTS`
- 
`AVRational time_base1=ifmt_ctx->streams[videoindex]->time_base;`
- 
`//Duration between 2 frames (us)`
- 
`int64_t calc_duration=(double)AV_TIME_BASE/av_q2d(ifmt_ctx->streams[videoindex]->r_frame_rate);`
- 
`//Parameters`
- 
`pkt.pts=(double)(frame_index*calc_duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);`
- 
`pkt.dts=pkt.pts;`
- 
`pkt.duration=(double)calc_duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);`
- 
`}`
- 
`//Important:Delay`
- 
`if(pkt.stream_index==videoindex){`
- 
`AVRational time_base=ifmt_ctx->streams[videoindex]->time_base;`
- 
`AVRational time_base_q={1,AV_TIME_BASE};`
- 
`int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);`
- 
`int64_t now_time = av_gettime() - start_time;`
- 
`if (pts_time > now_time)`
- 
`av_usleep(pts_time - now_time);`
- 
- 
`}`
- 
- 
`in_stream = ifmt_ctx->streams[pkt.stream_index];`
- 
`out_stream = ofmt_ctx->streams[pkt.stream_index];`
- 
`/* copy packet */`
- 
`//Convert PTS/DTS`
- 
`pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));`
- 
`pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));`
- 
`pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);`
- 
`pkt.pos = -1;`
- 
`//Print to Screen`
- 
`if(pkt.stream_index==videoindex){`
- 
`printf("Send %8d video frames to output URL\n",frame_index);`
- 
`frame_index++;`
- 
`}`
- 
`//ret = av_write_frame(ofmt_ctx, &pkt);`
- 
`ret = av_interleaved_write_frame(ofmt_ctx, &pkt);`
- 
- 
`if (ret < 0) {`
- 
`printf( "Error muxing packet\n");`
- 
`break;`
- 
`}`
- 
- 
`av_free_packet(&pkt);`
- 
- 
`}`
- 
`//Write file trailer`
- 
`av_write_trailer(ofmt_ctx);`
- 
`end:`
- 
`avformat_close_input(&ifmt_ctx);`
- 
`/* close output */`
- 
`if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))`
- 
`avio_close(ofmt_ctx->pb);`
- 
`avformat_free_context(ofmt_ctx);`
- 
`if (ret < 0 && ret != AVERROR_EOF) {`
- 
`printf( "Error occurred.\n");`
- 
`return -1;`
- 
`}`
- 
`return 0;`
- 
`}`
由代码中可以看出，视频源为dakongtou.mp4,推送出的url地址为rtmp://localhost/test/livestream
收流器端的代码如下：
- 
`#include <stdio.h>`
- 
- 
`#define __STDC_CONSTANT_MACROS`
- 
- 
`#ifdef _WIN32`
- 
`//Windows`
- 
`extern "C"`
- 
`{`
- 
`#include "libavformat/avformat.h"`
- 
`#include "libavutil/mathematics.h"`
- 
`#include "libavutil/time.h"`
- 
`};`
- 
`#else`
- 
`//Linux...`
- 
`#ifdef __cplusplus`
- 
`extern "C"`
- 
`{`
- 
`#endif`
- 
`#include <libavformat/avformat.h>`
- 
`#include <libavutil/mathematics.h>`
- 
`#include <libavutil/time.h>`
- 
`#ifdef __cplusplus`
- 
`};`
- 
`#endif`
- 
`#endif`
- 
- 
`//'1': Use H.264 Bitstream Filter `
- 
`#define USE_H264BSF 0`
- 
- 
`int main(int argc, char* argv[])`
- 
`{`
- 
`AVOutputFormat *ofmt = NULL;`
- 
`//Input AVFormatContext and Output AVFormatContext`
- 
`AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;`
- 
`AVPacket pkt;`
- 
`const char *in_filename, *out_filename;`
- 
`int ret, i;`
- 
`int videoindex=-1;`
- 
`int frame_index=0;`
- 
`in_filename = "rtmp://localhost:80/test/livestream live=1";`
- 
`out_filename = "receive.mp4";`
- 
- 
`av_register_all();`
- 
`//Network`
- 
`avformat_network_init();`
- 
`//Input`
- 
`if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {`
- 
`printf( "Could not open input file.");`
- 
`goto end;`
- 
`}`
- 
`if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {`
- 
`printf( "Failed to retrieve input stream information");`
- 
`goto end;`
- 
`}`
- 
- 
`for(i=0; i<ifmt_ctx->nb_streams; i++) `
- 
`if(ifmt_ctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){`
- 
`videoindex=i;`
- 
`break;`
- 
`}`
- 
- 
`av_dump_format(ifmt_ctx, 0, in_filename, 0);`
- 
- 
`//Output`
- 
`avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename); //RTMP`
- 
- 
`if (!ofmt_ctx) {`
- 
`printf( "Could not create output context\n");`
- 
`ret = AVERROR_UNKNOWN;`
- 
`goto end;`
- 
`}`
- 
`ofmt = ofmt_ctx->oformat;`
- 
`for (i = 0; i < ifmt_ctx->nb_streams; i++) {`
- 
`//Create output AVStream according to input AVStream`
- 
`AVStream *in_stream = ifmt_ctx->streams[i];`
- 
`AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);`
- 
`if (!out_stream) {`
- 
`printf( "Failed allocating output stream\n");`
- 
`ret = AVERROR_UNKNOWN;`
- 
`goto end;`
- 
`}`
- 
`//Copy the settings of AVCodecContext`
- 
`ret = avcodec_copy_context(out_stream->codec, in_stream->codec);`
- 
`if (ret < 0) {`
- 
`printf( "Failed to copy context from input to output stream codec context\n");`
- 
`goto end;`
- 
`}`
- 
`out_stream->codec->codec_tag = 0;`
- 
`if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)`
- 
`out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;`
- 
`}`
- 
`//Dump Format------------------`
- 
`av_dump_format(ofmt_ctx, 0, out_filename, 1);`
- 
`//Open output URL`
- 
`if (!(ofmt->flags & AVFMT_NOFILE)) {`
- 
`ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);`
- 
`if (ret < 0) {`
- 
`printf( "Could not open output URL '%s'", out_filename);`
- 
`goto end;`
- 
`}`
- 
`}`
- 
`//Write file header`
- 
`ret = avformat_write_header(ofmt_ctx, NULL);`
- 
`if (ret < 0) {`
- 
`printf( "Error occurred when opening output URL\n");`
- 
`goto end;`
- 
`}`
- 
- 
`#if USE_H264BSF`
- 
`AVBitStreamFilterContext* h264bsfc = av_bitstream_filter_init("h264_mp4toannexb"); `
- 
`#endif`
- 
- 
`while (1) {`
- 
`AVStream *in_stream, *out_stream;`
- 
`//Get an AVPacket`
- 
`ret = av_read_frame(ifmt_ctx, &pkt);`
- 
`if (ret < 0)`
- 
`break;`
- 
- 
`in_stream = ifmt_ctx->streams[pkt.stream_index];`
- 
`out_stream = ofmt_ctx->streams[pkt.stream_index];`
- 
`/* copy packet */`
- 
`//Convert PTS/DTS`
- 
`pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));`
- 
`pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));`
- 
`pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);`
- 
`pkt.pos = -1;`
- 
`//Print to Screen`
- 
`if(pkt.stream_index==videoindex){`
- 
`printf("Receive %8d video frames from input URL\n",frame_index);`
- 
`frame_index++;`
- 
- 
`#if USE_H264BSF`
- 
`av_bitstream_filter_filter(h264bsfc, in_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);`
- 
`#endif`
- 
`}`
- 
`//ret = av_write_frame(ofmt_ctx, &pkt);`
- 
`ret = av_interleaved_write_frame(ofmt_ctx, &pkt);`
- 
- 
`if (ret < 0) {`
- 
`printf( "Error muxing packet\n");`
- 
`break;`
- 
`}`
- 
- 
`av_free_packet(&pkt);`
- 
- 
`}`
- 
- 
`#if USE_H264BSF`
- 
`av_bitstream_filter_close(h264bsfc); `
- 
`#endif`
- 
- 
`//Write file trailer`
- 
`av_write_trailer(ofmt_ctx);`
- 
`end:`
- 
`avformat_close_input(&ifmt_ctx);`
- 
`/* close output */`
- 
`if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))`
- 
`avio_close(ofmt_ctx->pb);`
- 
`avformat_free_context(ofmt_ctx);`
- 
`if (ret < 0 && ret != AVERROR_EOF) {`
- 
`printf( "Error occurred.\n");`
- 
`return -1;`
- 
`}`
- 
`return 0;`
- 
`}`
由代码可知设置相应的收流器端url地址如下：rtmp://localhost:80/test/livestream live=1，注意根据ffmpeg版本的代码的不同，需要设置相应的live=1参数，收流器保存的文件如下：receive.mp4
程序运行结果如下:推流器端
![](https://img-blog.csdn.net/20180401203642831)
将视频文件一帧帧推送至rmtp的url地址
收流器运行如下：
![](https://img-blog.csdn.net/20180401203748651)
打印出相应的接收视频数据帧
使用Flash Media Server 的Samples文件夹下的videoPlayer播放直播流的截图如下图所示。（直播地址：rtmp://localhost:80/test/livestream live=1![](https://img-blog.csdn.net/20180401203917288)
