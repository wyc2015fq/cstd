# ffmpeg源码分析二：main函数和transcode函数 （转2） - 建建的博客 - CSDN博客
2017年03月11日 22:24:08[纪建](https://me.csdn.net/u013898698)阅读数：144
原帖地址：[http://blog.csdn.net/austinblog/article/details/24804455](http://blog.csdn.net/austinblog/article/details/24804455)
首先从main函数看起，关键解释部分已加注释，该函数在ffmpeg.c文件中。代码如下：
```
int main(int argc, char **argv)
{
    int ret;
    int64_t ti;
    // 注册清理回调函数
    register_exit(ffmpeg_cleanup);
    setvbuf(stderr,NULL,_IONBF,0); /* win32 runtime needs this */
    av_log_set_flags(AV_LOG_SKIP_REPEATED);
    parse_loglevel(argc, argv, options);
    if(argc>1 && !strcmp(argv[1], "-d")){
        run_as_daemon=1;
        av_log_set_callback(log_callback_null);
        argc--;
        argv++;
    }
    // 注册组件们
    avcodec_register_all();
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avfilter_register_all();
    av_register_all();
    avformat_network_init();
    show_banner(argc, argv, options);
    term_init();
    /* parse options and open all input/output files */
    // 解析参数，并且打开输入输出文件
    ret = ffmpeg_parse_options(argc, argv);
    if (ret < 0)
        exit_program(1);
    if (nb_output_files <= 0 && nb_input_files == 0) {
        show_usage();
        av_log(NULL, AV_LOG_WARNING, "Use -h to get full help or, even better, run 'man %s'\n", program_name);
        exit_program(1);
    }
    /* file converter / grab */
    if (nb_output_files <= 0) {
        av_log(NULL, AV_LOG_FATAL, "At least one output file must be specified\n");
        exit_program(1);
    }
//     if (nb_input_files == 0) {
//         av_log(NULL, AV_LOG_FATAL, "At least one input file must be specified\n");
//         exit_program(1);
//     }
    current_time = ti = getutime();
    // 音视频转换函数
    if (transcode() < 0)
        exit_program(1);
    ti = getutime() - ti;
    if (do_benchmark) {
        printf("bench: utime=%0.3fs\n", ti / 1000000.0);
    }
    av_log(NULL, AV_LOG_DEBUG, "%"PRIu64" frames successfully decoded, %"PRIu64" decoding errors\n",
           decode_error_stat[0], decode_error_stat[1]);
    if ((decode_error_stat[0] + decode_error_stat[1]) * max_error_rate < decode_error_stat[1])
        exit_program(69);
    exit_program(received_nb_signals ? 255 : main_return_code);
    return main_return_code;
}
```
下面阅读transcode函数：
```
static int transcode(void)
{
    int ret, i;
    AVFormatContext *os;
    OutputStream *ost;
    InputStream *ist;
    int64_t timer_start;
    // 初始化，打开所有输出流的编码器,打开所有输入流的解码器,写入所有输出文件的文件头。后面详细介绍。
    ret = transcode_init();
    if (ret < 0)
        goto fail;
    if (stdin_interaction) {
        av_log(NULL, AV_LOG_INFO, "Press [q] to stop, [?] for help\n");
    }
    timer_start = av_gettime();
#if HAVE_PTHREADS
    if ((ret = init_input_threads()) < 0)
        goto fail;
#endif
   
    // 循环,直到收到系统信号才退出 。
    while (!received_sigterm) {
        int64_t cur_time= av_gettime();
        /* if 'q' pressed, exits */
        if (stdin_interaction)
            if (check_keyboard_interaction(cur_time) < 0)
                break;
        /* check if there's any stream where output is still needed */
        if (!need_output()) {
            av_log(NULL, AV_LOG_VERBOSE, "No more output streams to write to, finishing.\n");
            break;
        }
        // 具体的转换工作，后面将详细介绍。
        ret = transcode_step();
        if (ret < 0) {
            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
                continue;
            av_log(NULL, AV_LOG_ERROR, "Error while filtering.\n");
            break;
        }
        /* dump report by using the output first video and audio streams */
        print_report(0, timer_start, cur_time);
    }
#if HAVE_PTHREADS
    free_input_threads();
#endif
    // 文件处理完了,把缓冲中剩余的数据写到输出文件中。
    /* at the end of stream, we must flush the decoder buffers */
    for (i = 0; i < nb_input_streams; i++) {
        ist = input_streams[i];
        if (!input_files[ist->file_index]->eof_reached && ist->decoding_needed) {
            output_packet(ist, NULL);
        }
    }
    flush_encoders();
    term_exit();
    /* write the trailer if needed and close file */
    // 为输出文件写文件尾(有的不需要)。  
    for (i = 0; i < nb_output_files; i++) {
        os = output_files[i]->ctx;
        av_write_trailer(os);
    }
    /* dump report by using the first video and audio streams */
    print_report(1, timer_start, av_gettime());
    // 关闭所有编码器。
    /* close each encoder */
    for (i = 0; i < nb_output_streams; i++) {
        ost = output_streams[i];
        if (ost->encoding_needed) {
            av_freep(&ost->st->codec->stats_in);
            avcodec_close(ost->st->codec);
        }
    }
    // 关闭所有解码器。
    /* close each decoder */
    for (i = 0; i < nb_input_streams; i++) {
        ist = input_streams[i];
        if (ist->decoding_needed) {
            avcodec_close(ist->st->codec);
            if (ist->hwaccel_uninit)
                ist->hwaccel_uninit(ist->st->codec);
        }
    }
    /* finished ! */
    ret = 0;
 fail:
#if HAVE_PTHREADS
    free_input_threads();
#endif
    if (output_streams) {
        for (i = 0; i < nb_output_streams; i++) {
            ost = output_streams[i];
            if (ost) {
                if (ost->stream_copy)
                    av_freep(&ost->st->codec->extradata);
                if (ost->logfile) {
                    fclose(ost->logfile);
                    ost->logfile = NULL;
                }
                av_freep(&ost->st->codec->subtitle_header);
                av_freep(&ost->forced_kf_pts);
                av_freep(&ost->apad);
                av_dict_free(&ost->opts);
                av_dict_free(&ost->swr_opts);
                av_dict_free(&ost->resample_opts);
            }
        }
    }
    return ret;
}
```
