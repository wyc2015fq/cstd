# FFMPEG-AVFilter研究 - DoubleLi - 博客园






FFMPEG中的libswscale是做像素转换的，但是对于一些复杂的操作，比如添加水印等，这个库就不行了，这时候就要说一下另外一个AVFilter。AVFilter完全可以替代libswscale的所有功能，也许有一天，ffmpeg也会这样去做。AVFilter虽然学起来有点复杂，但是为了学好FFMPEG，为了涨工资，我忍了。(*^__^*) 嘻嘻……



## 概念:

做个directshow的同学，对于这一块应该很了解了。

1.AVFilterGraph:和dshow中的filtergraph功能基本上一样，管理filter的，可以看成filter的一个容器。

2.AVFilter:就是一个filter啦。但是为什么叫做filter,其实就是一个过滤器，目标资源通过该过滤器总该发生一点变化是吧。。。。。

3.AVFilterLink:代表两个filter的关联媒介。

4.AVFilterPad:代表一个filter的输入输出口，就跟dshow中的pin差不多。只有输入pad的叫做source，只有输出pad的叫做sink。



下面是官方提供的一个例子：





**[cpp]**[view plain](http://blog.csdn.net/rootusers/article/details/43305387#)[copy](http://blog.csdn.net/rootusers/article/details/43305387#)

[print](http://blog.csdn.net/rootusers/article/details/43305387#)[?](http://blog.csdn.net/rootusers/article/details/43305387#)

- /* 
- *modifier by tongli 
- */  
- #define _XOPEN_SOURCE 600   
- #include <stdio.h>  
- #include <windows.h>  
- #include "snprintf.h"  
- extern "C"  
- {  
- #include <libavcodec/avcodec.h>  
- #include <libavformat/avformat.h>  
- #include <libavfilter/avfiltergraph.h>  
- #include <libavfilter/avcodec.h>  
- #include <libavfilter/buffersink.h>  
- #include <libavfilter/buffersrc.h>  
- #include <libavutil/opt.h>  
- }  
- 
- const char *filter_descr = "scale=78:24";  
- 
- static AVFormatContext *fmt_ctx;  
- static AVCodecContext *dec_ctx;  
- AVFilterContext *buffersink_ctx;  
- AVFilterContext *buffersrc_ctx;  
- AVFilterGraph *filter_graph;  
- static int video_stream_index = -1;  
- static int64_t last_pts = AV_NOPTS_VALUE;  
- 
- static int open_input_file(const char *filename)  
- {  
- int ret;  
-     AVCodec *dec;  
- 
- if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {  
-         av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");  
- return ret;  
-     }  
- 
- if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {  
-         av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");  
- return ret;  
-     }  
- 
- /* select the video stream */  
-     ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);  
- if (ret < 0) {  
-         av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");  
- return ret;  
-     }  
-     video_stream_index = ret;  
-     dec_ctx = fmt_ctx->streams[video_stream_index]->codec;  
-     av_opt_set_int(dec_ctx, "refcounted_frames", 1, 0);  
- 
- /* init the video decoder */  
- if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {  
-         av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");  
- return ret;  
-     }  
- 
- return 0;  
- }  
- 
- static int init_filters(const char *filters_descr)  
- {  
- char args[512];  
- int ret = 0;  
-     AVFilter *buffersrc = avfilter_get_by_name("buffer");  
-     AVFilter *buffersink = avfilter_get_by_name("buffersink");  
-     AVFilterInOut *outputs = avfilter_inout_alloc();  
-     AVFilterInOut *inputs = avfilter_inout_alloc();  
-     AVRational time_base = fmt_ctx->streams[video_stream_index]->time_base;  
- enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE };  
- 
-     filter_graph = avfilter_graph_alloc();  
- if (!outputs || !inputs || !filter_graph) {  
-         ret = AVERROR(ENOMEM);  
- goto end;  
-     }  
- 
- /* buffer video source: the decoded frames from the decoder will be inserted here. */  
-     snprintf(args, sizeof(args),  
- "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",  
-         dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,  
-         time_base.num, time_base.den,  
-         dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);  
- 
-     ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",  
-         args, NULL, filter_graph);  
- if (ret < 0) {  
-         av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");  
- goto end;  
-     }  
- 
- /* buffer video sink: to terminate the filter chain. */  
-     ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",  
-         NULL, NULL, filter_graph);  
- if (ret < 0) {  
-         av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");  
- goto end;  
-     }  
- 
-     ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,  
-         AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);  
- if (ret < 0) {  
-         av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");  
- goto end;  
-     }  
- 
- /* Endpoints for the filter graph. */  
-     outputs->name = av_strdup("in");  
-     outputs->filter_ctx = buffersrc_ctx;  
-     outputs->pad_idx = 0;  
-     outputs->next = NULL;  
- 
-     inputs->name = av_strdup("out");  
-     inputs->filter_ctx = buffersink_ctx;  
-     inputs->pad_idx = 0;  
-     inputs->next = NULL;  
- 
- if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,  
-         &inputs, &outputs, NULL)) < 0)  
- goto end;  
- 
- if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)  
- goto end;  
- 
- end:  
-     avfilter_inout_free(&inputs);  
-     avfilter_inout_free(&outputs);  
- 
- return ret;  
- }  
- 
- static void display_frame(const AVFrame *frame, AVRational time_base)  
- {  
- int x, y;  
-     uint8_t *p0, *p;  
-     int64_t delay;  
-     AVRational ar_base_q = { 1, AV_TIME_BASE };  
- if (frame->pts != AV_NOPTS_VALUE) {  
- if (last_pts != AV_NOPTS_VALUE) {  
- /* sleep roughly the right amount of time; 
-             * usleep is in microseconds, just like AV_TIME_BASE. */  
-             delay = av_rescale_q(frame->pts - last_pts,  
-                 time_base, ar_base_q);  
- if (delay > 0 && delay < 1000000)  
- //usleep(delay);  
-                 Sleep(delay);  
-         }  
-         last_pts = frame->pts;  
-     }  
- 
- /* Trivial ASCII grayscale display. */  
-     p0 = frame->data[0];  
-     puts("\033c");  
- for (y = 0; y < frame->height; y++) {  
-         p = p0;  
- for (x = 0; x < frame->width; x++)  
-             putchar(" .-+#"[*(p++) / 52]);  
-         putchar('\n');  
-         p0 += frame->linesize[0];  
-     }  
-     fflush(stdout);  
- }  
- 
- int main(int argc, char **argv)  
- {  
- int ret;  
-     AVPacket packet;  
-     AVFrame *frame = av_frame_alloc();  
-     AVFrame *filt_frame = av_frame_alloc();  
- int got_frame;  
- 
- if (!frame || !filt_frame) {  
-         perror("Could not allocate frame");  
-         exit(1);  
-     }  
- 
-     av_register_all();  
-     avfilter_register_all();  
- 
- if ((ret = open_input_file("style.ts")) < 0)  
- goto end;  
- if ((ret = init_filters(filter_descr)) < 0)  
- goto end;  
- 
- /* read all packets */  
- while (1) {  
- if ((ret = av_read_frame(fmt_ctx, &packet)) < 0)  
- break;  
- 
- if (packet.stream_index == video_stream_index) {  
-             got_frame = 0;  
-             ret = avcodec_decode_video2(dec_ctx, frame, &got_frame, &packet);  
- if (ret < 0) {  
-                 av_log(NULL, AV_LOG_ERROR, "Error decoding video\n");  
- break;  
-             }  
- 
- if (got_frame) {  
-                 frame->pts = av_frame_get_best_effort_timestamp(frame);  
- 
- /* push the decoded frame into the filtergraph */  
- if (av_buffersrc_add_frame_flags(buffersrc_ctx, frame, AV_BUFFERSRC_FLAG_KEEP_REF) < 0) {  
-                     av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");  
- break;  
-                 }  
- 
- /* pull filtered frames from the filtergraph */  
- while (1) {  
-                     ret = av_buffersink_get_frame(buffersink_ctx, filt_frame);  
- if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)  
- break;  
- if (ret < 0)  
- goto end;  
-                     display_frame(filt_frame, buffersink_ctx->inputs[0]->time_base);  
-                     av_frame_unref(filt_frame);  
-                 }  
-                 av_frame_unref(frame);  
-             }  
-         }  
-         av_free_packet(&packet);  
-     }  
- end:  
-     avfilter_graph_free(&filter_graph);  
-     avcodec_close(dec_ctx);  
-     avformat_close_input(&fmt_ctx);  
-     av_frame_free(&frame);  
-     av_frame_free(&filt_frame);  
- 
- if (ret < 0 && ret != AVERROR_EOF) {  
-         fprintf(stderr, "Error occurred: %s\n"); //av_err2str(ret));  
-         exit(1);  
-     }  
- 
-     exit(0);  
- }  


未完待续。。。。









