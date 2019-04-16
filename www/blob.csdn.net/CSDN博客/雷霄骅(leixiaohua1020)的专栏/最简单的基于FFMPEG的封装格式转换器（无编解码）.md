# 最简单的基于FFMPEG的封装格式转换器（无编解码） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年05月10日 00:25:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：150









本文介绍一个基于FFMPEG的封装格式转换器。所谓的封装格式转换，就是在AVI，FLV，MKV，MP4这些格式之间转换（对应.avi，.flv，.mkv，.mp4文件）。需要注意的是，本程序并不进行视音频的编码和解码工作。而是直接将视音频压缩码流从一种封装格式文件中获取出来然后打包成另外一种封装格式的文件。传统的转码程序工作原理如下图所示：

![](https://img-blog.csdn.net/20140509193232671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上图例举了一个举例：FLV（视频：H.264，音频：AAC）转码为AVI（视频：MPEG2，音频MP3）的例子。可见视频转码的过程通俗地讲相当于把视频和音频重新“录”了一遍。
本程序的工作原理如下图所示：

![](https://img-blog.csdn.net/20140509193252953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由图可见，本程序并不进行视频和音频的编解码工作，因此本程序和普通的转码软件相比，有以下两个特点：
处理速度极快。视音频编解码算法十分复杂，占据了转码的绝大部分时间。因为不需要进行视音频的编码和解码，所以节约了大量的时间。
视音频质量无损。因为不需要进行视音频的编码和解码，所以不会有视音频的压缩损伤。

下面贴上代码，代码是从FFmpeg的例子改编的，平台是VC2010。



```cpp
/*
*最简单的基于FFmpeg的封装格式转换器
*Simplest FFmpeg Remuxer
*
*雷霄骅 Lei Xiaohua
*leixiaohua1020@126.com
*中国传媒大学/数字电视技术
*Communication University of China / Digital TV Technology
*http://blog.csdn.net/leixiaohua1020
*
*本程序实现了视频封装格式之间的转换。
*需要注意的是本程序并不改变视音频的编码格式。
*
* This software converts a media file from one container format
* to another container format without encoding/decoding video files.
*/

#include "stdafx.h"

extern "C"
{
#include "libavformat/avformat.h"
};


int _tmain(int argc, _TCHAR* argv[])
{
 AVOutputFormat *ofmt = NULL;
 //输入对应一个AVFormatContext，输出对应一个AVFormatContext
 //（Input AVFormatContext and Output AVFormatContext）
 AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
 AVPacket pkt;
 const char *in_filename, *out_filename;
 int ret, i;
 if (argc < 3) {
 printf("usage: %s input output\n"
 "Remux a media file with libavformat and libavcodec.\n"
 "The output format is guessed according to the file extension.\n"
 "Modified by Lei Xiaohua, leixiaohua1020@126.com\n"
 "Communication University of China / Digital TV Technology\n"
 "http://blog.csdn.net/leixiaohua1020", argv[0]);
 return 1;
 }
 in_filename = argv[1];//输入文件名（Input file URL）
 out_filename = argv[2];//输出文件名（Output file URL）
 av_register_all();
 //输入（Input）
 if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
 printf( "Could not open input file.");
 goto end;
 }
 if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
 printf( "Failed to retrieve input stream information");
 goto end;
 }
 av_dump_format(ifmt_ctx, 0, in_filename, 0);
 //输出（Output）
 avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
 if (!ofmt_ctx) {
 printf( "Could not create output context\n");
 ret = AVERROR_UNKNOWN;
 goto end;
 }
 ofmt = ofmt_ctx->oformat;
 for (i = 0; i < ifmt_ctx->nb_streams; i++) {
 //根据输入流创建输出流（Create output AVStream according to input AVStream）
 AVStream *in_stream = ifmt_ctx->streams[i];
 AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
 if (!out_stream) {
 printf( "Failed allocating output stream\n");
 ret = AVERROR_UNKNOWN;
 goto end;
 }
 //复制AVCodecContext的设置（Copy the settings of AVCodecContext）
 ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
 if (ret < 0) {
 printf( "Failed to copy context from input to output stream codec context\n");
 goto end;
 }
 out_stream->codec->codec_tag = 0;
 if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
 out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
 }
 //输出一下格式------------------
 av_dump_format(ofmt_ctx, 0, out_filename, 1);
 //打开输出文件（Open output file）
 if (!(ofmt->flags & AVFMT_NOFILE)) {
 ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
 if (ret < 0) {
 printf( "Could not open output file '%s'", out_filename);
 goto end;
 }
 }
 //写文件头（Write file header）
 ret = avformat_write_header(ofmt_ctx, NULL);
 if (ret < 0) {
 printf( "Error occurred when opening output file\n");
 goto end;
 }
 int frame_index=0;
 while (1) {
 AVStream *in_stream, *out_stream;
 //获取一个AVPacket（Get an AVPacket）
 ret = av_read_frame(ifmt_ctx, &pkt);
 if (ret < 0)
 break;
 in_stream = ifmt_ctx->streams[pkt.stream_index];
 out_stream = ofmt_ctx->streams[pkt.stream_index];
 /* copy packet */
 //转换PTS/DTS（Convert PTS/DTS）
 pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
 pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
 pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
 pkt.pos = -1;
 //写入（Write）
 ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
 if (ret < 0) {
 printf( "Error muxing packet\n");
 break;
 }
 printf("Write %8d frames to output file\n",frame_index);
 av_free_packet(&pkt);
 frame_index++;
 }
 //写文件尾（Write file trailer）
 av_write_trailer(ofmt_ctx);
end:
 avformat_close_input(&ifmt_ctx);
 /* close output */
 if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
 avio_close(ofmt_ctx->pb);
 avformat_free_context(ofmt_ctx);
 if (ret < 0 && ret != AVERROR_EOF) {
 printf( "Error occurred.\n");
 return -1;
 }
 return 0;
}
```


调试的时候，只需要“右键工程->调试->命令行参数”里面设置输入的文件名和输出的文件名就可以了。



![](https://img-blog.csdn.net/20140509193453578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了一个测试的输入文件的视音频参数。

![](https://img-blog.csdn.net/20140509193500218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了输出文件的视音频参数。可以看出除了视频的封装格式从flv转换成了mp4，其他有关视音频编码的参数没有任何变化。



![](https://img-blog.csdn.net/20140509193506640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

完整工程下载地址：

[http://download.csdn.net/detail/leixiaohua1020/7323491](http://download.csdn.net/detail/leixiaohua1020/7323491)




