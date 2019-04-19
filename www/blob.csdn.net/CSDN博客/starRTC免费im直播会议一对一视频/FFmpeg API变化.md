# FFmpeg API变化 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月10日 11:12:35[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：668
可以查看doc目录下的APIchanges和根目录下的Changelog
去掉了ffserver程序
'avcodec_register_all' is deprecated
还有av_register_hwaccel   av_hwaccel_next也弃用了
Deprecate use of avcodec_register(), avcodec_register_all(),
av_codec_next(), av_register_codec_parser(), and av_parser_next().
Add av_codec_iterate() and av_parser_iterate().
Deprecate use of av_register_input_format(), av_register_output_format(),
av_register_all(), av_iformat_next(), av_oformat_next().
Add av_demuxer_iterate(), and av_muxer_iterate().
Deprecate use of avfilter_register(), avfilter_register_all(),
avfilter_next(). Add av_filter_iterate().
然后看下doc/examples目录下面的示例程序，可以发现不用调上面的注册函数了
![](https://images2018.cnblogs.com/blog/461810/201807/461810-20180710111120946-1024136066.png)
![](https://note.youdao.com/src/F2C83D2F28ED45A5BE4A25E172EB4A36)
avfiltergraph.h 也没有了，不用包含这个头文件了
