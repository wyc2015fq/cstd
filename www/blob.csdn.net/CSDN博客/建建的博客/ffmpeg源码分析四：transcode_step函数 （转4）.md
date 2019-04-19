# ffmpeg源码分析四：transcode_step函数 （转4） - 建建的博客 - CSDN博客
2017年03月11日 22:25:17[纪建](https://me.csdn.net/u013898698)阅读数：160
原帖地址：[http://blog.csdn.net/austinblog/article/details/25099979](http://blog.csdn.net/austinblog/article/details/25099979)
该函数的主要功能是一步完整的转换工作，下面看看源代码：
```
static int transcode_step(void)
{
    OutputStream *ost;
    InputStream  *ist;
    int ret;
    //选择一个有效的输出流进行处理
    ost = choose_output(); 
    if (!ost) {
        if (got_eagain()) {
            reset_eagain();
            av_usleep(10000);
            return 0;
        }
        av_log(NULL, AV_LOG_VERBOSE, "No more inputs to read from, finishing.\n");
        return AVERROR_EOF;
    }
    //选择一个输入流
    if (ost->filter) {
        if ((ret = transcode_from_filter(ost->filter->graph, &ist)) < 0)
            return ret;
        if (!ist)
            return 0;
    } else {
        av_assert0(ost->source_index >= 0);
        ist = input_streams[ost->source_index];
    }
    //读取并处理每一个包
    ret = process_input(ist->file_index);
    if (ret == AVERROR(EAGAIN)) {
        if (input_files[ist->file_index]->eagain)
            ost->unavailable = 1;
        return 0;
    }
    if (ret < 0)
        return ret == AVERROR_EOF ? 0 : ret;
    return reap_filters(); //根据滤波器做滤波处理，并把处理完的音视频输出到输出文件中
}
```
解码流程是：
process_input() -> output_packet() -> decode_audio()/decode_video()/transcode_subtitles()
而decode_audio() 是调用 avcodec_decode_audio4() 来完成工作的。decode_video() 则是通过调用 avcodec_decode_video2()
 来完成的。
编码流程是：
reap_filters() -> do_video_out() 或 do_audio_out() -> avcodec_encode_video2() 或 avcodec_encode_audio2()。
