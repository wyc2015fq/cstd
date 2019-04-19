# av_find_stream_info || av_estimate_timings - 三少GG - CSDN博客
2011年12月26日 16:35:47[三少GG](https://me.csdn.net/scut1135)阅读数：1920
[av_find_stream_info](http://hi.baidu.com/upmfd/blog/item/8ef4bc126d98970a962b4301.html)
int av_find_stream_info(AVFormatContext *ic)
{
view plaincopy to clipboardprint?
01.    for(i=0;i<ic->nb_streams;i++) {   
02.        分析AVCodecContext的每个AVStream   
03.        st = ic->streams[i];   
04.        if(st->codec->codec_type == CODEC_TYPE_VIDEO){   
05./*            if(!st->time_base.num)  
06.                st->time_base= */  
07.            if(!st->codec->time_base.num)   
08.                st->codec->time_base= st->time_base;   
09.        }   
10.        //only for the split stuff   
11.        if (!st->parser) {   
12.            寻找AVStream的parser，   
13.            st->parser = av_parser_init(st->codec->codec_id);   
14.            if(st->need_parsing == AVSTREAM_PARSE_HEADERS && st->parser){   
15.                st->parser->flags |= PARSER_FLAG_COMPLETE_FRAMES;   
16.            }   
17.        }   
18.    }  
    for(i=0;i<ic->nb_streams;i++) {
        分析AVCodecContext的每个AVStream
        st = ic->streams[i];
        if(st->codec->codec_type == CODEC_TYPE_VIDEO){
/*            if(!st->time_base.num)
                st->time_base= */
            if(!st->codec->time_base.num)
                st->codec->time_base= st->time_base;
        }
        //only for the split stuff
        if (!st->parser) {
            寻找AVStream的parser，
            st->parser = av_parser_init(st->codec->codec_id);
            if(st->need_parsing == AVSTREAM_PARSE_HEADERS && st->parser){
                st->parser->flags |= PARSER_FLAG_COMPLETE_FRAMES;
            }
        }
    }
av_parser_init根据AVStream的codec_id找到对应的parser,所有的parser在libavcodec/all_codec.c中注册，例如mpegvideo_parser在（libavcodec/mpegvideo_parser.c）：
view plaincopy to clipboardprint?
01.AVCodecParser mpegvideo_parser = {   
02.    { CODEC_ID_MPEG1VIDEO, CODEC_ID_MPEG2VIDEO },   
03.    sizeof(ParseContext1),   
04.    NULL,   
05.    mpegvideo_parse,   
06.    ff_parse1_close,   
07.};  
AVCodecParser mpegvideo_parser = {
    { CODEC_ID_MPEG1VIDEO, CODEC_ID_MPEG2VIDEO },
    sizeof(ParseContext1),
    NULL,
    mpegvideo_parse,
    ff_parse1_close,
}; 
 如果AVStream对应的codec根本没有媒体的信息，比如采样率等（has_codec_parameters(st->codec)返回0），那么需要读取AVStream的数据来获取媒体的信息。读取AVStream一个packet的函数是av_read_frame_internal
在av_read_frame_internal返回后，ffmpeg从媒体数据中获取到了一个packet的数据，如果这个时候还是不知道媒体的格式（codec已经知道），那么可能需要解码媒体数据来获取媒体格式，这个时候调用try_decode_frame来解码frame
}
## [ffplay中如何获取TS流文件的播放时长](http://blog.sina.com.cn/s/blog_6e65e8cc0100peel.html)
(2011-03-25 16:01:22)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载*▼*
|||
ffplay中获取ts文件时长的基本原理是首先获取文件大小，然后读取文件最后一个包，获取最后一个包的PTS，这样就可以通过PTS计算出最后一个包的播放时间，也就是文件的时长。
av_find_stream_info->av_estimate_timings
在av_estimate_timings中调用url_fsize获取文件的大小av_estimate_timings->url_fsize
同时在av_estimate_timings中调用av_estimate_timings_from_pts获取文件的时长av_estimate_timings->av_estimate_timings_from_pts
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
另外我在本地放了一个.h264的文件 通过打开这个文件 用ffmpeg 来解码播放 但是没有得到帧  
（就是
static void av_estimate_timings(AVFormatContext *ic, int64_t old_offset)
{
  int64_t file_size;
  /* get the file size, if possible */
  if (ic->iformat->flags & AVFMT_NOFILE) {
  file_size = 0;
  } else {
  file_size = url_fsize(ic->pb);
  if (file_size < 0)
  file_size = 0;
  }
  ic->file_size = file_size;
  if ((!strcmp(ic->iformat->name, "mpeg") ||
  !strcmp(ic->iformat->name, "mpegts")) &&
  file_size && !url_is_streamed(ic->pb)) {
  /* get accurate estimate from the PTSes */
  av_estimate_timings_from_pts(ic, old_offset);
  } else if (av_has_duration(ic)) {
  /* at least one component has timings - we use them for all
  the components */
  fill_all_stream_timings(ic);
  } else {
**//如果是mpg文件 或者mov 就行能得到帧 但 h264文件总是运行到这里  av_log(ic, AV_LOG_WARNING, "Estimating duration from bitrate, this may be inaccurate\n");**
  /* less precise: use bitrate info */
  av_estimate_timings_from_bit_rate(ic);
  }
  av_update_stream_timings(ic);
#if 0
  {
  int i;
  AVStream *st;
  for(i = 0;i < ic->nb_streams; i++) {
  st = ic->streams[i];
  printf("%d: start_time: %0.3f duration: %0.3f\n",
  i, (double)st->start_time / AV_TIME_BASE,
  (double)st->duration / AV_TIME_BASE);
  }
  printf("stream: start_time: %0.3f duration: %0.3f bitrate=%d kb/s\n",
  (double)ic->start_time / AV_TIME_BASE,
  (double)ic->duration / AV_TIME_BASE,
  ic->bit_rate / 1000);
  }
#endif
}
）
