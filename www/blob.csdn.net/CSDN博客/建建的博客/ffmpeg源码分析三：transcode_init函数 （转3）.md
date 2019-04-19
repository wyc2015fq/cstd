# ffmpeg源码分析三：transcode_init函数 （转3） - 建建的博客 - CSDN博客
2017年03月11日 22:24:43[纪建](https://me.csdn.net/u013898698)阅读数：112
原帖地址：[http://blog.csdn.net/austinblog/article/details/25061945](http://blog.csdn.net/austinblog/article/details/25061945)
transcode_init()函数是在转换前做准备工作的。下面看看其源代码：
```
static int transcode_init(void)
{
    int ret = 0, i, j, k;
    AVFormatContext *oc;
    AVCodecContext *codec; //输出流的编解码器结构
    OutputStream *ost;     //输出流
    InputStream *ist;      //输入流
    char error[1024];
    int want_sdp = 1;
    for (i = 0; i < nb_filtergraphs; i++) {
        FilterGraph *fg = filtergraphs[i];
        for (j = 0; j < fg->nb_outputs; j++) {
            OutputFilter *ofilter = fg->outputs[j];
            if (!ofilter->ost || ofilter->ost->source_index >= 0)
                continue;
            if (fg->nb_inputs != 1)
                continue;
            for (k = nb_input_streams-1; k >= 0 ; k--)
                if (fg->inputs[0]->ist == input_streams[k])
                    break;
            ofilter->ost->source_index = k;
        }
    }
    /* init framerate emulation */
    //初始化帧率仿真（转换时是不按帧率来的,但如果要求帧率仿真,就可以做到）
    for (i = 0; i < nb_input_files; i++) {
        InputFile *ifile = input_files[i];
        //如果一个输入文件被要求帧率仿真(指的是即使是转换也像播放那样按照帧率来进行），则为这个文件中所有流记录下开始时间。
        if (ifile->rate_emu)
            for (j = 0; j < ifile->nb_streams; j++)
                input_streams[j + ifile->ist_index]->start = av_gettime();
    }
    /* output stream init */
    //什么也没做,只是做了个判断而已。
    for (i = 0; i < nb_output_files; i++) {
        oc = output_files[i]->ctx;
        if (!oc->nb_streams && !(oc->oformat->flags & AVFMT_NOSTREAMS)) {
            av_dump_format(oc, i, oc->filename, 1);
            av_log(NULL, AV_LOG_ERROR, "Output file #%d does not contain any stream\n", i);
            return AVERROR(EINVAL);
        }
    }
    /* init complex filtergraphs */
    //初始化滤波
    for (i = 0; i < nb_filtergraphs; i++)
        if ((ret = avfilter_graph_config(filtergraphs[i]->graph, NULL)) < 0)
            return ret;
    /* for each output stream, we compute the right encoding parameters */
    //轮循所有的输出流，跟据对应的输入流，设置其编解码器的参数。
    for (i = 0; i < nb_output_streams; i++) {
        AVCodecContext *icodec = NULL;            //输入流的编解码
        ost = output_streams[i];                  //轮循所有的输出流
        oc  = output_files[ost->file_index]->ctx; //输出流对应的FormatContext
        ist = get_input_stream(ost);              //取得输出流对应的输入流
        if (ost->attachment_filename)
            continue;
        //先把能赋值的赋值
        codec  = ost->st->codec;     //输出流的编解码器结构
        if (ist) {
            icodec = ist->st->codec; //输入流的编解码器结构
            ost->st->disposition          = ist->st->disposition;
            codec->bits_per_raw_sample    = icodec->bits_per_raw_sample;
            codec->chroma_sample_location = icodec->chroma_sample_location;
        } else {
            for (j=0; j<oc->nb_streams; j++) {
                AVStream *st = oc->streams[j];
                if (st != ost->st && st->codec->codec_type == codec->codec_type)
                    break;
            }
            if (j == oc->nb_streams)
                if (codec->codec_type == AVMEDIA_TYPE_AUDIO || codec->codec_type == AVMEDIA_TYPE_VIDEO)
                    ost->st->disposition = AV_DISPOSITION_DEFAULT;
        }
        //如果只是复制一个流(不用解码后再编码)，则把输入流的编码参数直接赋值给输出流  
        //此时是不需要解码也不需要编码，所以不需打开解码器和编码器  
        if (ost->stream_copy) {
            AVRational sar;
            uint64_t extra_size;
            av_assert0(ist && !ost->filter);
            //计算输出流的编解码器的extradata的大小，然后分配缓冲区  
            //然后把输入流的编解码器的extradata拷贝到输出流的编解码器中
            extra_size = (uint64_t)icodec->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE;
            if (extra_size > INT_MAX) {
                return AVERROR(EINVAL);
            }
            /* if stream_copy is selected, no need to decode or encode */
            codec->codec_id   = icodec->codec_id;
            codec->codec_type = icodec->codec_type;
            if (!codec->codec_tag) {
                unsigned int codec_tag;
                if (!oc->oformat->codec_tag ||
                     av_codec_get_id (oc->oformat->codec_tag, icodec->codec_tag) == codec->codec_id ||
                     !av_codec_get_tag2(oc->oformat->codec_tag, icodec->codec_id, &codec_tag))
                    codec->codec_tag = icodec->codec_tag;
            }
            codec->bit_rate       = icodec->bit_rate;
            codec->rc_max_rate    = icodec->rc_max_rate;
            codec->rc_buffer_size = icodec->rc_buffer_size;
            codec->field_order    = icodec->field_order;
            codec->extradata      = av_mallocz(extra_size);
            if (!codec->extradata) {
                return AVERROR(ENOMEM);
            }
            memcpy(codec->extradata, icodec->extradata, icodec->extradata_size);
            codec->extradata_size= icodec->extradata_size;
            codec->bits_per_coded_sample  = icodec->bits_per_coded_sample;
            //重新鼓捣一下time base(这家伙就是帧率) 
            codec->time_base = ist->st->time_base;
            /*
             * Avi is a special case here because it supports variable fps but
             * having the fps and timebase differe significantly adds quite some
             * overhead
             */
            //如果输出文件是avi,做一点特殊处理
            if(!strcmp(oc->oformat->name, "avi")) {
                if ( copy_tb<0 && av_q2d(ist->st->r_frame_rate) >= av_q2d(ist->st->avg_frame_rate)
                               && 0.5/av_q2d(ist->st->r_frame_rate) > av_q2d(ist->st->time_base)
                               && 0.5/av_q2d(ist->st->r_frame_rate) > av_q2d(icodec->time_base)
                               && av_q2d(ist->st->time_base) < 1.0/500 && av_q2d(icodec->time_base) < 1.0/500
                     || copy_tb==2){
                    codec->time_base.num = ist->st->r_frame_rate.den;
                    codec->time_base.den = 2*ist->st->r_frame_rate.num;
                    codec->ticks_per_frame = 2;
                } else if (   copy_tb<0 && av_q2d(icodec->time_base)*icodec->ticks_per_frame > 2*av_q2d(ist->st->time_base)
                                 && av_q2d(ist->st->time_base) < 1.0/500
                    || copy_tb==0){
                    codec->time_base = icodec->time_base;
                    codec->time_base.num *= icodec->ticks_per_frame;
                    codec->time_base.den *= 2;
                    codec->ticks_per_frame = 2;
                }
            } else if(!(oc->oformat->flags & AVFMT_VARIABLE_FPS)
                      && strcmp(oc->oformat->name, "mov") && strcmp(oc->oformat->name, "mp4") && strcmp(oc->oformat->name, "3gp")
                      && strcmp(oc->oformat->name, "3g2") && strcmp(oc->oformat->name, "psp") && strcmp(oc->oformat->name, "ipod")
                      && strcmp(oc->oformat->name, "f4v")
            ) {
                if(   copy_tb<0 && icodec->time_base.den
                                && av_q2d(icodec->time_base)*icodec->ticks_per_frame > av_q2d(ist->st->time_base)
                                && av_q2d(ist->st->time_base) < 1.0/500
                   || copy_tb==0){
                    codec->time_base = icodec->time_base;
                    codec->time_base.num *= icodec->ticks_per_frame;
                }
            }
            if (   codec->codec_tag == AV_RL32("tmcd")
                && icodec->time_base.num < icodec->time_base.den
                && icodec->time_base.num > 0
                && 121LL*icodec->time_base.num > icodec->time_base.den) {
                codec->time_base = icodec->time_base;
            }
            if (ist && !ost->frame_rate.num)
                ost->frame_rate = ist->framerate;
            if(ost->frame_rate.num)
                codec->time_base = av_inv_q(ost->frame_rate);
            //再修正一下帧率 
            av_reduce(&codec->time_base.num, &codec->time_base.den,
                        codec->time_base.num, codec->time_base.den, INT_MAX);
            ost->parser = av_parser_init(codec->codec_id);
            //根据不同媒体类型，赋值自己对应的编码参数
            switch (codec->codec_type) {
            case AVMEDIA_TYPE_AUDIO:  //音频的
                if (audio_volume != 256) {
                    av_log(NULL, AV_LOG_FATAL, "-acodec copy and -vol are incompatible (frames are not decoded)\n");
                    exit_program(1);
                }
                codec->channel_layout     = icodec->channel_layout;
                codec->sample_rate        = icodec->sample_rate;
                codec->channels           = icodec->channels;
                codec->frame_size         = icodec->frame_size;
                codec->audio_service_type = icodec->audio_service_type;
                codec->block_align        = icodec->block_align;
                codec->delay              = icodec->delay;
                if((codec->block_align == 1 || codec->block_align == 1152 || codec->block_align == 576) && codec->codec_id == AV_CODEC_ID_MP3)
                    codec->block_align= 0;
                if(codec->codec_id == AV_CODEC_ID_AC3)
                    codec->block_align= 0;
                break;
            case AVMEDIA_TYPE_VIDEO:  //视频的
                codec->pix_fmt            = icodec->pix_fmt;
                codec->width              = icodec->width;
                codec->height             = icodec->height;
                codec->has_b_frames       = icodec->has_b_frames;
                if (ost->frame_aspect_ratio.num) { // overridden by the -aspect cli option
                    sar =
                        av_mul_q(ost->frame_aspect_ratio,
                                 (AVRational){ codec->height, codec->width });
                    av_log(NULL, AV_LOG_WARNING, "Overriding aspect ratio "
                           "with stream copy may produce invalid files\n");
                }
                else if (ist->st->sample_aspect_ratio.num)
                    sar = ist->st->sample_aspect_ratio;
                else
                    sar = icodec->sample_aspect_ratio;
                ost->st->sample_aspect_ratio = codec->sample_aspect_ratio = sar;
                ost->st->avg_frame_rate = ist->st->avg_frame_rate;
                break;
            case AVMEDIA_TYPE_SUBTITLE:  //字幕的
                codec->width  = icodec->width;
                codec->height = icodec->height;
                break;
            case AVMEDIA_TYPE_DATA:
            case AVMEDIA_TYPE_ATTACHMENT: //其他
                break;
            default:
                abort();
            }
        } else {                         //如果不是复制,就麻烦多了
            //获取编码器
            if (!ost->enc)
                ost->enc = avcodec_find_encoder(codec->codec_id);
            if (!ost->enc) {
                /* should only happen when a default codec is not present. */
                snprintf(error, sizeof(error), "Encoder (codec %s) not found for output stream #%d:%d",
                         avcodec_get_name(ost->st->codec->codec_id), ost->file_index, ost->index);
                ret = AVERROR(EINVAL);
                goto dump_format;
            }
            //因为需要转换,所以既需解码又需编码 
            if (ist)
                ist->decoding_needed++;
            ost->encoding_needed = 1;
            if (!ost->filter &&
                (codec->codec_type == AVMEDIA_TYPE_VIDEO ||
                 codec->codec_type == AVMEDIA_TYPE_AUDIO)) {
                    FilterGraph *fg;
                    fg = init_simple_filtergraph(ist, ost);
                    if (configure_filtergraph(fg)) {
                        av_log(NULL, AV_LOG_FATAL, "Error opening filters!\n");
                        exit_program(1);
                    }
            }
            if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                if (ost->filter && !ost->frame_rate.num)
                    ost->frame_rate = av_buffersink_get_frame_rate(ost->filter->filter);
                if (ist && !ost->frame_rate.num)
                    ost->frame_rate = ist->framerate;
                if (ist && !ost->frame_rate.num)
                    ost->frame_rate = ist->st->r_frame_rate;
                if (ist && !ost->frame_rate.num) {
                    ost->frame_rate = (AVRational){25, 1};
                    av_log(NULL, AV_LOG_WARNING,
                           "No information "
                           "about the input framerate is available. Falling "
                           "back to a default value of 25fps for output stream #%d:%d. Use the -r option "
                           "if you want a different framerate.\n",
                           ost->file_index, ost->index);
                }
//                    ost->frame_rate = ist->st->avg_frame_rate.num ? ist->st->avg_frame_rate : (AVRational){25, 1};
                if (ost->enc && ost->enc->supported_framerates && !ost->force_fps) {
                    int idx = av_find_nearest_q_idx(ost->frame_rate, ost->enc->supported_framerates);
                    ost->frame_rate = ost->enc->supported_framerates[idx];
                }
                if (codec->codec_id == AV_CODEC_ID_MPEG4) {
                    av_reduce(&ost->frame_rate.num, &ost->frame_rate.den,
                              ost->frame_rate.num, ost->frame_rate.den, 65535);
                }
            }
            switch (codec->codec_type) {
            case AVMEDIA_TYPE_AUDIO:  //设置音频编码器的参数
                codec->sample_fmt     = ost->filter->filter->inputs[0]->format;                       //样点格式
                codec->sample_rate    = ost->filter->filter->inputs[0]->sample_rate;                  //采样率
                codec->channel_layout = ost->filter->filter->inputs[0]->channel_layout;
                codec->channels       = avfilter_link_get_channels(ost->filter->filter->inputs[0]);   //声道 
                codec->time_base      = (AVRational){ 1, codec->sample_rate };
                break;
            case AVMEDIA_TYPE_VIDEO:  //设置视频编码器的参数
                codec->time_base = av_inv_q(ost->frame_rate);
                if (ost->filter && !(codec->time_base.num && codec->time_base.den))
                    codec->time_base = ost->filter->filter->inputs[0]->time_base;
                if (   av_q2d(codec->time_base) < 0.001 && video_sync_method != VSYNC_PASSTHROUGH
                   && (video_sync_method == VSYNC_CFR || video_sync_method == VSYNC_VSCFR || (video_sync_method == VSYNC_AUTO && !(oc->oformat->flags & AVFMT_VARIABLE_FPS)))){
                    av_log(oc, AV_LOG_WARNING, "Frame rate very high for a muxer not efficiently supporting it.\n"
                                               "Please consider specifying a lower framerate, a different muxer or -vsync 2\n");
                }
                for (j = 0; j < ost->forced_kf_count; j++)
                    ost->forced_kf_pts[j] = av_rescale_q(ost->forced_kf_pts[j],
                                                         AV_TIME_BASE_Q,
                                                         codec->time_base);
                //宽高
                codec->width  = ost->filter->filter->inputs[0]->w;
                codec->height = ost->filter->filter->inputs[0]->h;
                codec->sample_aspect_ratio = ost->st->sample_aspect_ratio =
                    ost->frame_aspect_ratio.num ? // overridden by the -aspect cli option
                    av_mul_q(ost->frame_aspect_ratio, (AVRational){ codec->height, codec->width }) :
                    ost->filter->filter->inputs[0]->sample_aspect_ratio;
                if (!strncmp(ost->enc->name, "libx264", 7) &&
                    codec->pix_fmt == AV_PIX_FMT_NONE &&
                    ost->filter->filter->inputs[0]->format != AV_PIX_FMT_YUV420P)
                    av_log(NULL, AV_LOG_WARNING,
                           "No pixel format specified, %s for H.264 encoding chosen.\n"
                           "Use -pix_fmt yuv420p for compatibility with outdated media players.\n",
                           av_get_pix_fmt_name(ost->filter->filter->inputs[0]->format));
                if (!strncmp(ost->enc->name, "mpeg2video", 10) &&
                    codec->pix_fmt == AV_PIX_FMT_NONE &&
                    ost->filter->filter->inputs[0]->format != AV_PIX_FMT_YUV420P)
                    av_log(NULL, AV_LOG_WARNING,
                           "No pixel format specified, %s for MPEG-2 encoding chosen.\n"
                           "Use -pix_fmt yuv420p for compatibility with outdated media players.\n",
                           av_get_pix_fmt_name(ost->filter->filter->inputs[0]->format));
                codec->pix_fmt = ost->filter->filter->inputs[0]->format;
                if (!icodec ||
                    codec->width   != icodec->width  ||
                    codec->height  != icodec->height ||
                    codec->pix_fmt != icodec->pix_fmt) {
                    codec->bits_per_raw_sample = frame_bits_per_raw_sample;
                }
                if (ost->forced_keyframes) {
                    if (!strncmp(ost->forced_keyframes, "expr:", 5)) {
                        ret = av_expr_parse(&ost->forced_keyframes_pexpr, ost->forced_keyframes+5,
                                            forced_keyframes_const_names, NULL, NULL, NULL, NULL, 0, NULL);
                        if (ret < 0) {
                            av_log(NULL, AV_LOG_ERROR,
                                   "Invalid force_key_frames expression '%s'\n", ost->forced_keyframes+5);
                            return ret;
                        }
                        ost->forced_keyframes_expr_const_values[FKF_N] = 0;
                        ost->forced_keyframes_expr_const_values[FKF_N_FORCED] = 0;
                        ost->forced_keyframes_expr_const_values[FKF_PREV_FORCED_N] = NAN;
                        ost->forced_keyframes_expr_const_values[FKF_PREV_FORCED_T] = NAN;
                    } else {
                        parse_forced_key_frames(ost->forced_keyframes, ost, ost->st->codec);
                    }
                }
                break;
            case AVMEDIA_TYPE_SUBTITLE:  //设置字幕编码器的参数
                codec->time_base = (AVRational){1, 1000};
                if (!codec->width) {
                    codec->width     = input_streams[ost->source_index]->st->codec->width;
                    codec->height    = input_streams[ost->source_index]->st->codec->height;
                }
                break;
            default:
                abort();
                break;
            }
            /* two pass mode */
            if (codec->flags & (CODEC_FLAG_PASS1 | CODEC_FLAG_PASS2)) {
                char logfilename[1024];
                FILE *f;
                snprintf(logfilename, sizeof(logfilename), "%s-%d.log",
                         ost->logfile_prefix ? ost->logfile_prefix :
                                               DEFAULT_PASS_LOGFILENAME_PREFIX,
                         i);
                if (!strcmp(ost->enc->name, "libx264")) {
                    av_dict_set(&ost->opts, "stats", logfilename, AV_DICT_DONT_OVERWRITE);
                } else {
                    if (codec->flags & CODEC_FLAG_PASS2) {
                        char  *logbuffer;
                        size_t logbuffer_size;
                        if (cmdutils_read_file(logfilename, &logbuffer, &logbuffer_size) < 0) {
                            av_log(NULL, AV_LOG_FATAL, "Error reading log file '%s' for pass-2 encoding\n",
                                   logfilename);
                            exit_program(1);
                        }
                        codec->stats_in = logbuffer;
                    }
                    if (codec->flags & CODEC_FLAG_PASS1) {
                        f = av_fopen_utf8(logfilename, "wb");
                        if (!f) {
                            av_log(NULL, AV_LOG_FATAL, "Cannot write log file '%s' for pass-1 encoding: %s\n",
                                logfilename, strerror(errno));
                            exit_program(1);
                        }
                        ost->logfile = f;
                    }
                }
            }
        }
    }
    /* open each encoder */
    //轮循所有输出流,打开每个输出流的编码器
    for (i = 0; i < nb_output_streams; i++) {
        ost = output_streams[i];
        if (ost->encoding_needed) {
            AVCodec      *codec = ost->enc;
            AVCodecContext *dec = NULL;
            if ((ist = get_input_stream(ost)))
                dec = ist->st->codec;
            if (dec && dec->subtitle_header) {
                /* ASS code assumes this buffer is null terminated so add extra byte. */
                ost->st->codec->subtitle_header = av_mallocz(dec->subtitle_header_size + 1);
                if (!ost->st->codec->subtitle_header) {
                    ret = AVERROR(ENOMEM);
                    goto dump_format;
                }
                memcpy(ost->st->codec->subtitle_header, dec->subtitle_header, dec->subtitle_header_size);
                ost->st->codec->subtitle_header_size = dec->subtitle_header_size;
            }
            if (!av_dict_get(ost->opts, "threads", NULL, 0))
                av_dict_set(&ost->opts, "threads", "auto", 0);
            //打开
            if ((ret = avcodec_open2(ost->st->codec, codec, &ost->opts)) < 0) {
                if (ret == AVERROR_EXPERIMENTAL)
                    abort_codec_experimental(codec, 1);
                snprintf(error, sizeof(error), "Error while opening encoder for output stream #%d:%d - maybe incorrect parameters such as bit_rate, rate, width or height",
                        ost->file_index, ost->index);
                goto dump_format;
            }
            if (ost->enc->type == AVMEDIA_TYPE_AUDIO &&
                !(ost->enc->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE))
                av_buffersink_set_frame_size(ost->filter->filter,
                                             ost->st->codec->frame_size);
            assert_avoptions(ost->opts);
            if (ost->st->codec->bit_rate && ost->st->codec->bit_rate < 1000)
                av_log(NULL, AV_LOG_WARNING, "The bitrate parameter is set too low."
                                             " It takes bits/s as argument, not kbits/s\n");
            extra_size += ost->st->codec->extradata_size;
        } else {
            av_opt_set_dict(ost->st->codec, &ost->opts);
        }
    }
    /* init input streams */
    //初始化所有的输入流
    for (i = 0; i < nb_input_streams; i++)
        if ((ret = init_input_stream(i, error, sizeof(error))) < 0) {
            for (i = 0; i < nb_output_streams; i++) {
                ost = output_streams[i];
                avcodec_close(ost->st->codec);
            }
            goto dump_format;
        }
    /* discard unused programs */
    for (i = 0; i < nb_input_files; i++) {
        InputFile *ifile = input_files[i];
        for (j = 0; j < ifile->ctx->nb_programs; j++) {
            AVProgram *p = ifile->ctx->programs[j];
            int discard  = AVDISCARD_ALL;
            for (k = 0; k < p->nb_stream_indexes; k++)
                if (!input_streams[ifile->ist_index + p->stream_index[k]]->discard) {
                    discard = AVDISCARD_DEFAULT;
                    break;
                }
            p->discard = discard;
        }
    }
    /* open files and write file headers */
    //打开所有输出文件，写入媒体文件头
    for (i = 0; i < nb_output_files; i++) {
        oc = output_files[i]->ctx;
        oc->interrupt_callback = int_cb;
        if ((ret = avformat_write_header(oc, &output_files[i]->opts)) < 0) {
            snprintf(error, sizeof(error),
                     "Could not write header for output file #%d "
                     "(incorrect codec parameters ?): %s",
                     i, av_err2str(ret));
            ret = AVERROR(EINVAL);
            goto dump_format;
        }
//         assert_avoptions(output_files[i]->opts);
        if (strcmp(oc->oformat->name, "rtp")) {
            want_sdp = 0;
        }
    }
 dump_format:
    /* dump the file output parameters - cannot be done before in case
       of stream copy */
    for (i = 0; i < nb_output_files; i++) {
        av_dump_format(output_files[i]->ctx, i, output_files[i]->ctx->filename, 1);
    }
    /* dump the stream mapping */
    av_log(NULL, AV_LOG_INFO, "Stream mapping:\n");
    for (i = 0; i < nb_input_streams; i++) {
        ist = input_streams[i];
        for (j = 0; j < ist->nb_filters; j++) {
            if (ist->filters[j]->graph->graph_desc) {
                av_log(NULL, AV_LOG_INFO, "  Stream #%d:%d (%s) -> %s",
                       ist->file_index, ist->st->index, ist->dec ? ist->dec->name : "?",
                       ist->filters[j]->name);
                if (nb_filtergraphs > 1)
                    av_log(NULL, AV_LOG_INFO, " (graph %d)", ist->filters[j]->graph->index);
                av_log(NULL, AV_LOG_INFO, "\n");
            }
        }
    }
    for (i = 0; i < nb_output_streams; i++) {
        ost = output_streams[i];
        if (ost->attachment_filename) {
            /* an attached file */
            av_log(NULL, AV_LOG_INFO, "  File %s -> Stream #%d:%d\n",
                   ost->attachment_filename, ost->file_index, ost->index);
            continue;
        }
        if (ost->filter && ost->filter->graph->graph_desc) {
            /* output from a complex graph */
            av_log(NULL, AV_LOG_INFO, "  %s", ost->filter->name);
            if (nb_filtergraphs > 1)
                av_log(NULL, AV_LOG_INFO, " (graph %d)", ost->filter->graph->index);
            av_log(NULL, AV_LOG_INFO, " -> Stream #%d:%d (%s)\n", ost->file_index,
                   ost->index, ost->enc ? ost->enc->name : "?");
            continue;
        }
        av_log(NULL, AV_LOG_INFO, "  Stream #%d:%d -> #%d:%d",
               input_streams[ost->source_index]->file_index,
               input_streams[ost->source_index]->st->index,
               ost->file_index,
               ost->index);
        if (ost->sync_ist != input_streams[ost->source_index])
            av_log(NULL, AV_LOG_INFO, " [sync #%d:%d]",
                   ost->sync_ist->file_index,
                   ost->sync_ist->st->index);
        if (ost->stream_copy)
            av_log(NULL, AV_LOG_INFO, " (copy)");
        else
            av_log(NULL, AV_LOG_INFO, " (%s -> %s)", input_streams[ost->source_index]->dec ?
                   input_streams[ost->source_index]->dec->name : "?",
                   ost->enc ? ost->enc->name : "?");
        av_log(NULL, AV_LOG_INFO, "\n");
    }
    if (ret) {
        av_log(NULL, AV_LOG_ERROR, "%s\n", error);
        return ret;
    }
    if (want_sdp) {
        print_sdp();
    }
    transcode_init_done = 1; //初始化完成
    return 0;
}
```
