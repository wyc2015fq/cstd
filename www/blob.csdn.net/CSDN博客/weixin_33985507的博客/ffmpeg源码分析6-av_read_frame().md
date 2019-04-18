# ffmpeg源码分析6-av_read_frame() - weixin_33985507的博客 - CSDN博客
2018年11月05日 11:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
本文参考雷神博文：[https://blog.csdn.net/leixiaohua1020](https://blog.csdn.net/leixiaohua1020)。
![14126810-5983af4f705629b1.jpg](https://upload-images.jianshu.io/upload_images/14126810-5983af4f705629b1.jpg)
20160523102949674.jpg
read_from_packet_buffer是旧版本函数，新版本是ff_packet_list_get，从AVPacketList里取出数据即可。若缓存中没有数据，av_read_frame函数则会调用read_frame_internal函数来获取数据。
ffmpeg中的av_read_frame()的作用是读取码流中的音频若干帧或者视频一帧。例如，解码视频的时候，每解码一个视频帧，需要先调用 av_read_frame()获得一帧视频的压缩数据，然后才能对该数据进行解码（例如H.264中一帧压缩数据通常对应一个NAL）。
上代码之前，先参考了其他人对av_read_frame()的解释，在此做一个参考：
通过av_read_packet(***)，读取一个包，需要说明的是此函数必须是包含整数帧的，不存在半帧的情况，以ts流为例，是读取一个完整的PES包（一个完整pes包包含若干视频或音频es包），读取完毕后，通过av_parser_parse2(***)分析出视频一帧（或音频若干帧），返回，下次进入循环的时候，如果上次的数据没有完全取完，则st = s->cur_st;不会是NULL，即再此进入av_parser_parse2(***)流程，而不是下面的av_read_packet（**）流程，这样就保证了，如果读取一次包含了N帧视频数据（以视频为例），则调用av_read_frame（***）N次都不会去读数据，而是返回第一次读取的数据，直到全部解析完毕。
# av_read_frame()
```
//获取一个AVPacket
/*
 * av_read_frame - 新版本的ffmpeg用的是av_read_frame，而老版本的是av_read_packet
 * 。区别是av_read_packet读出的是包，它可能是半帧或多帧，不保证帧的完整性。av_read_frame对
 * av_read_packet进行了封装，使读出的数据总是完整的帧
 */
int av_read_frame(AVFormatContext *s, AVPacket *pkt)
{
    const int genpts = s->flags & AVFMT_FLAG_GENPTS;
    int eof = 0;
    int ret;
    AVStream *st;
    if (!genpts) {
         /* 一般情况下会调用read_frame_internal(s, pkt)
         * 直接返回
         */
        ret = s->internal->packet_buffer
              ? ff_packet_list_get(&s->internal->packet_buffer,
                                        &s->internal->packet_buffer_end, pkt)
              : read_frame_internal(s, pkt);
        if (ret < 0)
            return ret;
        goto return_packet;
    }
    for (;;) {
        AVPacketList *pktl = s->internal->packet_buffer;
        if (pktl) {
            AVPacket *next_pkt = &pktl->pkt;
            if (next_pkt->dts != AV_NOPTS_VALUE) {
                int wrap_bits = s->streams[next_pkt->stream_index]->pts_wrap_bits;
                // last dts seen for this stream. if any of packets following
                // current one had no dts, we will set this to AV_NOPTS_VALUE.
                int64_t last_dts = next_pkt->dts;
                av_assert2(wrap_bits <= 64);
                while (pktl && next_pkt->pts == AV_NOPTS_VALUE) {
                    if (pktl->pkt.stream_index == next_pkt->stream_index &&
                        av_compare_mod(next_pkt->dts, pktl->pkt.dts, 2ULL << (wrap_bits - 1)) < 0) {
                        if (av_compare_mod(pktl->pkt.pts, pktl->pkt.dts, 2ULL << (wrap_bits - 1))) {
                            // not B-frame
                            next_pkt->pts = pktl->pkt.dts;
                        }
                        if (last_dts != AV_NOPTS_VALUE) {
                            // Once last dts was set to AV_NOPTS_VALUE, we don't change it.
                            last_dts = pktl->pkt.dts;
                        }
                    }
                    pktl = pktl->next;
                }
                if (eof && next_pkt->pts == AV_NOPTS_VALUE && last_dts != AV_NOPTS_VALUE) {
                    // Fixing the last reference frame had none pts issue (For MXF etc).
                    // We only do this when
                    // 1. eof.
                    // 2. we are not able to resolve a pts value for current packet.
                    // 3. the packets for this stream at the end of the files had valid dts.
                    next_pkt->pts = last_dts + next_pkt->duration;
                }
                pktl = s->internal->packet_buffer;
            }
            /* read packet from packet buffer, if there is data */
            st = s->streams[next_pkt->stream_index];
            if (!(next_pkt->pts == AV_NOPTS_VALUE && st->discard < AVDISCARD_ALL &&
                  next_pkt->dts != AV_NOPTS_VALUE && !eof)) {
                ret = ff_packet_list_get(&s->internal->packet_buffer,
                                               &s->internal->packet_buffer_end, pkt);
                goto return_packet;
            }
        }
        ret = read_frame_internal(s, pkt);
        if (ret < 0) {
            if (pktl && ret != AVERROR(EAGAIN)) {
                eof = 1;
                continue;
            } else
                return ret;
        }
        ret = ff_packet_list_put(&s->internal->packet_buffer,
                                 &s->internal->packet_buffer_end,
                                 pkt, FF_PACKETLIST_FLAG_REF_PACKET);
        av_packet_unref(pkt);
        if (ret < 0)
            return ret;
    }
return_packet:
    st = s->streams[pkt->stream_index];
    if ((s->iformat->flags & AVFMT_GENERIC_INDEX) && pkt->flags & AV_PKT_FLAG_KEY) {
        ff_reduce_index(s, st->index);
        av_add_index_entry(st, pkt->pos, pkt->dts, 0, 0, AVINDEX_KEYFRAME);
    }
    if (is_relative(pkt->dts))
        pkt->dts -= RELATIVE_TS_BASE;
    if (is_relative(pkt->pts))
        pkt->pts -= RELATIVE_TS_BASE;
    return ret;
}
```
可以从源代码中看出，av_read_frame()调用了read_frame_internal()。
## read_frame_internal()
```
static int read_frame_internal(AVFormatContext *s, AVPacket *pkt)
{
    int ret = 0, i, got_packet = 0;
    AVDictionary *metadata = NULL;
    av_init_packet(pkt);
    while (!got_packet && !s->internal->parse_queue) {
        AVStream *st;
        AVPacket cur_pkt;
        /* read next packet */
        ret = ff_read_packet(s, &cur_pkt);
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN))
                return ret;
            /* flush the parsers */
            for (i = 0; i < s->nb_streams; i++) {
                st = s->streams[i];
                /*需要解析*/
                if (st->parser && st->need_parsing)
                    parse_packet(s, NULL, st->index);
            }
            /* all remaining packets are now in parse_queue =>
             * really terminate parsing */
            break;
        }
        ret = 0;
        st  = s->streams[cur_pkt.stream_index];
        /* update context if required */
        if (st->internal->need_context_update) {
            if (avcodec_is_open(st->internal->avctx)) {
                av_log(s, AV_LOG_DEBUG, "Demuxer context update while decoder is open, closing and trying to re-open\n");
                avcodec_close(st->internal->avctx);
                st->info->found_decoder = 0;
            }
            /* close parser, because it depends on the codec */
            if (st->parser && st->internal->avctx->codec_id != st->codecpar->codec_id) {
                av_parser_close(st->parser);
                st->parser = NULL;
            }
            ret = avcodec_parameters_to_context(st->internal->avctx, st->codecpar);
            if (ret < 0)
                return ret;
#if FF_API_LAVF_AVCTX
FF_DISABLE_DEPRECATION_WARNINGS
            /* update deprecated public codec context */
            ret = avcodec_parameters_to_context(st->codec, st->codecpar);
            if (ret < 0)
                return ret;
FF_ENABLE_DEPRECATION_WARNINGS
#endif
            st->internal->need_context_update = 0;
        }
        if (cur_pkt.pts != AV_NOPTS_VALUE &&
            cur_pkt.dts != AV_NOPTS_VALUE &&
            cur_pkt.pts < cur_pkt.dts) {
            av_log(s, AV_LOG_WARNING,
                   "Invalid timestamps stream=%d, pts=%s, dts=%s, size=%d\n",
                   cur_pkt.stream_index,
                   av_ts2str(cur_pkt.pts),
                   av_ts2str(cur_pkt.dts),
                   cur_pkt.size);
        }
        if (s->debug & FF_FDEBUG_TS)
            av_log(s, AV_LOG_DEBUG,
                   "ff_read_packet stream=%d, pts=%s, dts=%s, size=%d, duration=%"PRId64", flags=%d\n",
                   cur_pkt.stream_index,
                   av_ts2str(cur_pkt.pts),
                   av_ts2str(cur_pkt.dts),
                   cur_pkt.size, cur_pkt.duration, cur_pkt.flags);
        if (st->need_parsing && !st->parser && !(s->flags & AVFMT_FLAG_NOPARSE)) {
            st->parser = av_parser_init(st->codecpar->codec_id);
            if (!st->parser) {
                av_log(s, AV_LOG_VERBOSE, "parser not found for codec "
                       "%s, packets or times may be invalid.\n",
                       avcodec_get_name(st->codecpar->codec_id));
                /* no parser available: just output the raw packets */
                st->need_parsing = AVSTREAM_PARSE_NONE;
            } else if (st->need_parsing == AVSTREAM_PARSE_HEADERS)
                st->parser->flags |= PARSER_FLAG_COMPLETE_FRAMES;
            else if (st->need_parsing == AVSTREAM_PARSE_FULL_ONCE)
                st->parser->flags |= PARSER_FLAG_ONCE;
            else if (st->need_parsing == AVSTREAM_PARSE_FULL_RAW)
                st->parser->flags |= PARSER_FLAG_USE_CODEC_TS;
        }
        if (!st->need_parsing || !st->parser) {
            /* no parsing needed: we just output the packet as is */
            *pkt = cur_pkt;
            compute_pkt_fields(s, st, NULL, pkt, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
            if ((s->iformat->flags & AVFMT_GENERIC_INDEX) &&
                (pkt->flags & AV_PKT_FLAG_KEY) && pkt->dts != AV_NOPTS_VALUE) {
                ff_reduce_index(s, st->index);
                av_add_index_entry(st, pkt->pos, pkt->dts,
                                   0, 0, AVINDEX_KEYFRAME);
            }
            got_packet = 1;
        } else if (st->discard < AVDISCARD_ALL) {
            if ((ret = parse_packet(s, &cur_pkt, cur_pkt.stream_index)) < 0)
                return ret;
            st->codecpar->sample_rate = st->internal->avctx->sample_rate;
            st->codecpar->bit_rate = st->internal->avctx->bit_rate;
            st->codecpar->channels = st->internal->avctx->channels;
            st->codecpar->channel_layout = st->internal->avctx->channel_layout;
            st->codecpar->codec_id = st->internal->avctx->codec_id;
        } else {
            /* free packet */
            av_packet_unref(&cur_pkt);
        }
        if (pkt->flags & AV_PKT_FLAG_KEY)
            st->skip_to_keyframe = 0;
        if (st->skip_to_keyframe) {
            av_packet_unref(&cur_pkt);
            if (got_packet) {
                *pkt = cur_pkt;
            }
            got_packet = 0;
        }
    }
//退出循环后，调用read_from_packet_buffer，从parse_queue中取出数据。
    if (!got_packet && s->internal->parse_queue)
        ret = ff_packet_list_get(&s->internal->parse_queue, &s->internal->parse_queue_end, pkt);
    if (ret >= 0) {
        AVStream *st = s->streams[pkt->stream_index];
        int discard_padding = 0;
        if (st->first_discard_sample && pkt->pts != AV_NOPTS_VALUE) {
            int64_t pts = pkt->pts - (is_relative(pkt->pts) ? RELATIVE_TS_BASE : 0);
            int64_t sample = ts_to_samples(st, pts);
            int duration = ts_to_samples(st, pkt->duration);
            int64_t end_sample = sample + duration;
            if (duration > 0 && end_sample >= st->first_discard_sample &&
                sample < st->last_discard_sample)
                discard_padding = FFMIN(end_sample - st->first_discard_sample, duration);
        }
        if (st->start_skip_samples && (pkt->pts == 0 || pkt->pts == RELATIVE_TS_BASE))
            st->skip_samples = st->start_skip_samples;
        if (st->skip_samples || discard_padding) {
            uint8_t *p = av_packet_new_side_data(pkt, AV_PKT_DATA_SKIP_SAMPLES, 10);
            if (p) {
                AV_WL32(p, st->skip_samples);
                AV_WL32(p + 4, discard_padding);
                av_log(s, AV_LOG_DEBUG, "demuxer injecting skip %d / discard %d\n", st->skip_samples, discard_padding);
            }
            st->skip_samples = 0;
        }
        if (st->inject_global_side_data) {
            for (i = 0; i < st->nb_side_data; i++) {
                AVPacketSideData *src_sd = &st->side_data[i];
                uint8_t *dst_data;
                if (av_packet_get_side_data(pkt, src_sd->type, NULL))
                    continue;
                dst_data = av_packet_new_side_data(pkt, src_sd->type, src_sd->size);
                if (!dst_data) {
                    av_log(s, AV_LOG_WARNING, "Could not inject global side data\n");
                    continue;
                }
                memcpy(dst_data, src_sd->data, src_sd->size);
            }
            st->inject_global_side_data = 0;
        }
    }
    av_opt_get_dict_val(s, "metadata", AV_OPT_SEARCH_CHILDREN, &metadata);
    if (metadata) {
        s->event_flags |= AVFMT_EVENT_FLAG_METADATA_UPDATED;
        av_dict_copy(&s->metadata, metadata, 0);
        av_dict_free(&metadata);
        av_opt_set_dict_val(s, "metadata", NULL, AV_OPT_SEARCH_CHILDREN);
    }
#if FF_API_LAVF_AVCTX
    update_stream_avctx(s);
#endif
    if (s->debug & FF_FDEBUG_TS)
        av_log(s, AV_LOG_DEBUG,
               "read_frame_internal stream=%d, pts=%s, dts=%s, "
               "size=%d, duration=%"PRId64", flags=%d\n",
               pkt->stream_index,
               av_ts2str(pkt->pts),
               av_ts2str(pkt->dts),
               pkt->size, pkt->duration, pkt->flags);
    return ret;
}
```
read_frame_internal()代码比较长，这里只简单看一下它前面的部分。它前面部分有2步是十分关键的：
（1）调用了ff_read_packet()从相应的AVInputFormat读取数据。
（2）如果媒体频流需要使用AVCodecParser，则调用parse_packet()解析相应的AVPacket。
### ff_read_packet()
```
int ff_read_packet(AVFormatContext *s, AVPacket *pkt)
{
    int ret, i, err;
    AVStream *st;
    for (;;) {
        AVPacketList *pktl = s->internal->raw_packet_buffer;
        if (pktl) {
            *pkt = pktl->pkt;
            st   = s->streams[pkt->stream_index];
            if (s->internal->raw_packet_buffer_remaining_size <= 0)
                if ((err = probe_codec(s, st, NULL)) < 0)
                    return err;
            if (st->request_probe <= 0) {//-1是probe完成，0是不要probe
                s->internal->raw_packet_buffer                 = pktl->next;
                s->internal->raw_packet_buffer_remaining_size += pkt->size;
                av_free(pktl);
                return 0;
            }
        }
        pkt->data = NULL;
        pkt->size = 0;
        av_init_packet(pkt);
      /*关键，从这里读包packet*/
        ret = s->iformat->read_packet(s, pkt);
        if (ret < 0) {
            /* Some demuxers return FFERROR_REDO when they consume
               data and discard it (ignored streams, junk, extradata).
               We must re-call the demuxer to get the real packet. */
            if (ret == FFERROR_REDO)
                continue;
            if (!pktl || ret == AVERROR(EAGAIN))
                return ret;
            for (i = 0; i < s->nb_streams; i++) {
                st = s->streams[i];
                if (st->probe_packets || st->request_probe > 0)
                    if ((err = probe_codec(s, st, NULL)) < 0)
                        return err;
                av_assert0(st->request_probe <= 0);
            }
            continue;
        }
        err = av_packet_make_refcounted(pkt);
        if (err < 0)
            return err;
//读取成功  ，判断是不是要丢掉  
        if ((s->flags & AVFMT_FLAG_DISCARD_CORRUPT) &&
            (pkt->flags & AV_PKT_FLAG_CORRUPT)) {
            av_log(s, AV_LOG_WARNING,
                   "Dropped corrupted packet (stream = %d)\n",
                   pkt->stream_index);
            av_packet_unref(pkt);
            continue;
        }
        if (pkt->stream_index >= (unsigned)s->nb_streams) {
            av_log(s, AV_LOG_ERROR, "Invalid stream index %d\n", pkt->stream_index);
            continue;
        }
        st = s->streams[pkt->stream_index];
        if (update_wrap_reference(s, st, pkt->stream_index, pkt) && st->pts_wrap_behavior == AV_PTS_WRAP_SUB_OFFSET) {
            // correct first time stamps to negative values
            if (!is_relative(st->first_dts))
                st->first_dts = wrap_timestamp(st, st->first_dts);
            if (!is_relative(st->start_time))
                st->start_time = wrap_timestamp(st, st->start_time);
            if (!is_relative(st->cur_dts))
                st->cur_dts = wrap_timestamp(st, st->cur_dts);
        }
        pkt->dts = wrap_timestamp(st, pkt->dts);
        pkt->pts = wrap_timestamp(st, pkt->pts);
        force_codec_ids(s, st);
        /* TODO: audio: time filter; video: frame reordering (pts != dts) */
//音频要做时间过滤; 视频帧的重排序(当视频帧的dts和pts不等的时候)
        if (s->use_wallclock_as_timestamps)
            pkt->dts = pkt->pts = av_rescale_q(av_gettime(), AV_TIME_BASE_Q, st->time_base);
        if (!pktl && st->request_probe <= 0)
            return ret;
 //最后调用add_to_pktbuf将packet加入s->raw_packet_buffer中。
        err = ff_packet_list_put(&s->internal->raw_packet_buffer,
                                 &s->internal->raw_packet_buffer_end,
                                 pkt, 0);
        if (err)
            return err;
//raw buffer也是有大小限制的，就是raw_packet_buffer_remaining_size 这么大。 
        s->internal->raw_packet_buffer_remaining_size -= pkt->size;
        if ((err = probe_codec(s, st, pkt)) < 0)
            return err;
    }
}
```
ff_read_packet的缓存和av_read_frame函数里的缓存是不一样的，ff_read_packet的缓存为AVFormatInternal::raw_packet_buffer
ff_read_packet()中最关键的地方就是调用了AVInputFormat的read_packet()方法。AVInputFormat的read_packet()是一个函数指针，指向当前的AVInputFormat的读取数据的函数。
它的主要功能就是根据文件封装格式的规范，逐层解析Tag以及TagData，获取Tag以及TagData中的信息。
### parse_packet()
```
/**
 * Parse a packet, add all split parts to parse_queue.
 *
 * @param pkt Packet to parse, NULL when flushing the parser at end of stream.
 */
static int parse_packet(AVFormatContext *s, AVPacket *pkt, int stream_index)
{
    AVPacket out_pkt = { 0 }, flush_pkt = { 0 };
    AVStream *st = s->streams[stream_index];
    uint8_t *data = pkt ? pkt->data : NULL;
    int size      = pkt ? pkt->size : 0;
    int ret = 0, got_output = 0;
    if (!pkt) {
        av_init_packet(&flush_pkt);
        pkt        = &flush_pkt;
        got_output = 1;
    } else if (!size && st->parser->flags & PARSER_FLAG_COMPLETE_FRAMES) {
        // preserve 0-size sync packets
        compute_pkt_fields(s, st, st->parser, pkt, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
    }
    while (size > 0 || (pkt == &flush_pkt && got_output)) {
        int len;
        int64_t next_pts = pkt->pts;
        int64_t next_dts = pkt->dts;
        av_init_packet(&out_pkt);
      /*解析*/
        len = av_parser_parse2(st->parser, st->internal->avctx,
                               &out_pkt.data, &out_pkt.size, data, size,
                               pkt->pts, pkt->dts, pkt->pos);
        pkt->pts = pkt->dts = AV_NOPTS_VALUE;
        pkt->pos = -1;
        /* increment read pointer */
        data += len;
        size -= len;
        got_output = !!out_pkt.size;
        if (!out_pkt.size)
            continue;
        if (pkt->buf && out_pkt.data == pkt->data) {
            /* reference pkt->buf only when out_pkt.data is guaranteed to point
             * to data in it and not in the parser's internal buffer. */
            /* XXX: Ensure this is the case with all parsers when st->parser->flags
             * is PARSER_FLAG_COMPLETE_FRAMES and check for that instead? */
            out_pkt.buf = av_buffer_ref(pkt->buf);
            if (!out_pkt.buf) {
                ret = AVERROR(ENOMEM);
                goto fail;
            }
        } else {
            ret = av_packet_make_refcounted(&out_pkt);
            if (ret < 0)
                goto fail;
        }
        if (pkt->side_data) {
            out_pkt.side_data       = pkt->side_data;
            out_pkt.side_data_elems = pkt->side_data_elems;
            pkt->side_data          = NULL;
            pkt->side_data_elems    = 0;
        }
        /* set the duration */
        out_pkt.duration = (st->parser->flags & PARSER_FLAG_COMPLETE_FRAMES) ? pkt->duration : 0;
        if (st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (st->internal->avctx->sample_rate > 0) {
                out_pkt.duration =
                    av_rescale_q_rnd(st->parser->duration,
                                     (AVRational) { 1, st->internal->avctx->sample_rate },
                                     st->time_base,
                                     AV_ROUND_DOWN);
            }
        }
        out_pkt.stream_index = st->index;
        out_pkt.pts          = st->parser->pts;
        out_pkt.dts          = st->parser->dts;
        out_pkt.pos          = st->parser->pos;
        out_pkt.flags       |= pkt->flags & AV_PKT_FLAG_DISCARD;
        if (st->need_parsing == AVSTREAM_PARSE_FULL_RAW)
            out_pkt.pos = st->parser->frame_offset;
        if (st->parser->key_frame == 1 ||
            (st->parser->key_frame == -1 &&
             st->parser->pict_type == AV_PICTURE_TYPE_I))
            out_pkt.flags |= AV_PKT_FLAG_KEY;
        if (st->parser->key_frame == -1 && st->parser->pict_type ==AV_PICTURE_TYPE_NONE && (pkt->flags&AV_PKT_FLAG_KEY))
            out_pkt.flags |= AV_PKT_FLAG_KEY;
        compute_pkt_fields(s, st, st->parser, &out_pkt, next_dts, next_pts);
        ret = ff_packet_list_put(&s->internal->parse_queue,
                                 &s->internal->parse_queue_end,
                                 &out_pkt, 0);
        if (ret < 0) {
            av_packet_unref(&out_pkt);
            goto fail;
        }
    }
    /* end of the stream => close and free the parser */
    if (pkt == &flush_pkt) {
        av_parser_close(st->parser);
        st->parser = NULL;
    }
fail:
    av_packet_unref(pkt);
    return ret;
}
```
parse_packet()的缓存为：：parse_queue
从代码中可以看出，最终调用了相应AVCodecParser的av_parser_parse2()函数，解析出来AVPacket。此后根据解析的信息还进行了一系列的赋值工作，不再详细叙述。
probe_codec
probe_codec是用来探测codec的，probe_codec先是把pkt的数据打包进AVProbeData，然后调用set_codec_from_probe_data来进行探测的，set_codec_from_probe_data的基本思想是根据av_probe_input_format3函数返回的一个AVInputFormat格式来和fmt_id_type匹配得出的codec_id和type的。
parse_packet
parse_packet()给需要AVCodecParser的媒体流提供解析AVPacket的功能，最终调用了相应AVCodecParser的av_parser_parse2()函数,代码为s->parser->parser_parse，接着会调用具体的解析函数，如h264_parse（parse_nal_units），最终解析出来AVPacket。
h264解析
ff_h264_decode_seq_parameter_set  解析SPS。
ff_h264_decode_picture_parameter_set   解析PPS。
ff_h264_decode_sei                   解析SEI。
以上代码在libavcdec/h264_parser.c。
av_parser_parse2()函数分析在[ffmpeg源码分析7-av_parser_parse2()](https://www.jianshu.com/p/22632167fd56)
