# ffmpeg 源代码简单分析 ： av_read_frame() - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月13日 15:58:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：41











此前写了好几篇ffmpeg源代码分析文章，列表如下：

[图解FFMPEG打开媒体的函数avformat_open_input](http://blog.csdn.net/leixiaohua1020/article/details/8661601)
[ffmpeg 源代码简单分析 ： av_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677129)
[ffmpeg 源代码简单分析 ： avcodec_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677265)
[ffmpeg 源代码简单分析 ： av_read_frame()](http://blog.csdn.net/leixiaohua1020/article/details/12678577)
[ffmpeg 源代码简单分析 ： avcodec_decode_video2()](http://blog.csdn.net/leixiaohua1020/article/details/12679719)

============================



ffmpeg中的av_read_frame()的作用是读取码流中的音频若干帧或者视频一帧。例如，解码视频的时候，每解码一个视频帧，需要先调用av_read_frame()获得一帧视频的压缩数据，然后才能对该数据进行解码（例如H.264中一帧压缩数据通常对应一个NAL）。

对该函数源代码的分析是很久之前做的了，现在翻出来，用博客记录一下。



上代码之前，先参考了其他人对av_read_frame()的解释，在此做一个参考：

> 
通过av_read_packet(***)，读取一个包，需要说明的是此函数必须是包含整数帧的，不存在半帧的情况，以ts流为例，是读取一个完整的PES包（一个完整pes包包含若干视频或音频es包），读取完毕后，通过av_parser_parse2(***)分析出视频一帧（或音频若干帧），返回，下次进入循环的时候，如果上次的数据没有完全取完，则st = s->cur_st;不会是NULL，即再此进入av_parser_parse2(***)流程，而不是下面的av_read_packet（**）流程，这样就保证了，如果读取一次包含了N帧视频数据（以视频为例），则调用av_read_frame（***）N次都不会去读数据，而是返回第一次读取的数据，直到全部解析完毕。


av_read_frame()的源代码如下：



```cpp
//获取一个AVPacket
/*
 * av_read_frame - 新版本的ffmpeg用的是av_read_frame，而老版本的是av_read_packet
 * 。区别是av_read_packet读出的是包，它可能是半帧或多帧，不保证帧的完整性。av_read_frame对
 * av_read_packet进行了封装，使读出的数据总是完整的帧
 */
int av_read_frame(AVFormatContext *s, AVPacket *pkt)
{
    const int genpts = s->flags & AVFMT_FLAG_GENPTS;
    int          eof = 0;

    if (!genpts)
    	/**
    	 * This buffer is only needed when packets were already buffered but
    	 * not decoded, for example to get the codec parameters in MPEG
    	 * streams.
    	 * 一般情况下会调用read_frame_internal(s, pkt)
    	 * 直接返回
    	 */
        return s->packet_buffer ? read_from_packet_buffer(s, pkt) :
                                  read_frame_internal(s, pkt);

    for (;;) {
        int ret;
        AVPacketList *pktl = s->packet_buffer;

        if (pktl) {
            AVPacket *next_pkt = &pktl->pkt;

            if (next_pkt->dts != AV_NOPTS_VALUE) {
                int wrap_bits = s->streams[next_pkt->stream_index]->pts_wrap_bits;
                while (pktl && next_pkt->pts == AV_NOPTS_VALUE) {
                    if (pktl->pkt.stream_index == next_pkt->stream_index &&
                        (av_compare_mod(next_pkt->dts, pktl->pkt.dts, 2LL << (wrap_bits - 1)) < 0) &&
                         av_compare_mod(pktl->pkt.pts, pktl->pkt.dts, 2LL << (wrap_bits - 1))) { //not b frame
                        next_pkt->pts = pktl->pkt.dts;
                    }
                    pktl = pktl->next;
                }
                pktl = s->packet_buffer;
            }

            /* read packet from packet buffer, if there is data */
            if (!(next_pkt->pts == AV_NOPTS_VALUE &&
                  next_pkt->dts != AV_NOPTS_VALUE && !eof))
                return read_from_packet_buffer(s, pkt);
        }

        ret = read_frame_internal(s, pkt);
        if (ret < 0) {
            if (pktl && ret != AVERROR(EAGAIN)) {
                eof = 1;
                continue;
            } else
                return ret;
        }

        if (av_dup_packet(add_to_pktbuf(&s->packet_buffer, pkt,
                          &s->packet_buffer_end)) < 0)
            return AVERROR(ENOMEM);
    }
}
```


 一般情况下，av_read_frame()会调用read_frame_internal()，其代码如下所示：





```cpp
//av_read_frame对他进行了封装
static int read_frame_internal(AVFormatContext *s, AVPacket *pkt)
{
    AVStream *st;
    int len, ret, i;
    //初始化
    av_init_packet(pkt);

    for(;;) {
        /* 选择当前的 input stream */
        st = s->cur_st;
        if (st) {
        	//不需要解析。不清楚哪些数据属于这类
            if (!st->need_parsing || !st->parser) {
                /* no parsing needed: we just output the packet as is */
                /* raw data support */
                *pkt = st->cur_pkt;
                st->cur_pkt.data= NULL;
                st->cur_pkt.side_data_elems = 0;
                st->cur_pkt.side_data = NULL;
                compute_pkt_fields(s, st, NULL, pkt);
                s->cur_st = NULL;
                if ((s->iformat->flags & AVFMT_GENERIC_INDEX) &&
                    (pkt->flags & AV_PKT_FLAG_KEY) && pkt->dts != AV_NOPTS_VALUE) {
                    ff_reduce_index(s, st->index);
                    av_add_index_entry(st, pkt->pos, pkt->dts, 0, 0, AVINDEX_KEYFRAME);
                }
                break;
            } //需要解析
            else if (st->cur_len > 0 && st->discard < AVDISCARD_ALL) {
            	//解析
                len = av_parser_parse2(st->parser, st->codec, &pkt->data, &pkt->size,
                                       st->cur_ptr, st->cur_len,
                                       st->cur_pkt.pts, st->cur_pkt.dts,
                                       st->cur_pkt.pos);
                st->cur_pkt.pts = AV_NOPTS_VALUE;
                st->cur_pkt.dts = AV_NOPTS_VALUE;
                /* increment read pointer */
                st->cur_ptr += len;
                st->cur_len -= len;

                /* return packet if any */
                if (pkt->size) {
                got_packet:
                    pkt->duration = 0;
                    pkt->stream_index = st->index;
                    pkt->pts = st->parser->pts;
                    pkt->dts = st->parser->dts;
                    pkt->pos = st->parser->pos;
                    if(pkt->data == st->cur_pkt.data && pkt->size == st->cur_pkt.size){
                        s->cur_st = NULL;
                        pkt->destruct= st->cur_pkt.destruct;
                        st->cur_pkt.destruct= NULL;
                        st->cur_pkt.data    = NULL;
                        assert(st->cur_len == 0);
                    }else{
                        pkt->destruct = NULL;
                    }
                    compute_pkt_fields(s, st, st->parser, pkt);

                    if((s->iformat->flags & AVFMT_GENERIC_INDEX) && pkt->flags & AV_PKT_FLAG_KEY){
                        int64_t pos= (st->parser->flags & PARSER_FLAG_COMPLETE_FRAMES) ? pkt->pos : st->parser->frame_offset;
                        ff_reduce_index(s, st->index);
                        av_add_index_entry(st, pos, pkt->dts,
                                           0, 0, AVINDEX_KEYFRAME);
                    }

                    break;
                }
            } else {
                /* free packet */
                av_free_packet(&st->cur_pkt);
                s->cur_st = NULL;
            }
        } else {
            AVPacket cur_pkt;
            /* read next packet */
            //读取AVPacket,老版本里只有av_read_packet，现在被封装了
            ret = av_read_packet(s, &cur_pkt);
            if (ret < 0) {
                if (ret == AVERROR(EAGAIN))
                    return ret;
                /* return the last frames, if any */
                for(i = 0; i < s->nb_streams; i++) {
                    st = s->streams[i];
                    if (st->parser && st->need_parsing) {
                        av_parser_parse2(st->parser, st->codec,
                                        &pkt->data, &pkt->size,
                                        NULL, 0,
                                        AV_NOPTS_VALUE, AV_NOPTS_VALUE,
                                        AV_NOPTS_VALUE);
                        if (pkt->size)
                            goto got_packet;
                    }
                }
                /* no more packets: really terminate parsing */
                return ret;
            }
            st = s->streams[cur_pkt.stream_index];
            st->cur_pkt= cur_pkt;

            if(st->cur_pkt.pts != AV_NOPTS_VALUE &&
               st->cur_pkt.dts != AV_NOPTS_VALUE &&
               st->cur_pkt.pts < st->cur_pkt.dts){
                av_log(s, AV_LOG_WARNING, "Invalid timestamps stream=%d, pts=%"PRId64", dts=%"PRId64", size=%d\n",
                    st->cur_pkt.stream_index,
                    st->cur_pkt.pts,
                    st->cur_pkt.dts,
                    st->cur_pkt.size);
//                av_free_packet(&st->cur_pkt);
//                return -1;
            }

            if(s->debug & FF_FDEBUG_TS)
                av_log(s, AV_LOG_DEBUG, "av_read_packet stream=%d, pts=%"PRId64", dts=%"PRId64", size=%d, duration=%d, flags=%d\n",
                    st->cur_pkt.stream_index,
                    st->cur_pkt.pts,
                    st->cur_pkt.dts,
                    st->cur_pkt.size,
                    st->cur_pkt.duration,
                    st->cur_pkt.flags);

            s->cur_st = st;
            st->cur_ptr = st->cur_pkt.data;
            st->cur_len = st->cur_pkt.size;
            if (st->need_parsing && !st->parser && !(s->flags & AVFMT_FLAG_NOPARSE)) {
                st->parser = av_parser_init(st->codec->codec_id);
                if (!st->parser) {
                    av_log(s, AV_LOG_VERBOSE, "parser not found for codec "
                           "%s, packets or times may be invalid.\n",
                           avcodec_get_name(st->codec->codec_id));
                    /* no parser available: just output the raw packets */
                    st->need_parsing = AVSTREAM_PARSE_NONE;
                }else if(st->need_parsing == AVSTREAM_PARSE_HEADERS){
                    st->parser->flags |= PARSER_FLAG_COMPLETE_FRAMES;
                }else if(st->need_parsing == AVSTREAM_PARSE_FULL_ONCE){
                    st->parser->flags |= PARSER_FLAG_ONCE;
                }
            }
        }
    }
    if(s->debug & FF_FDEBUG_TS)
        av_log(s, AV_LOG_DEBUG, "read_frame_internal stream=%d, pts=%"PRId64", dts=%"PRId64", size=%d, duration=%d, flags=%d\n",
            pkt->stream_index,
            pkt->pts,
            pkt->dts,
            pkt->size,
            pkt->duration,
            pkt->flags);

    return 0;
}
```


 一般的码流都需要解析，这是需要调用av_paser_parse2()，它的代码如下所示：





```cpp
//解析。例如解析264里的NAL等等
int av_parser_parse2(AVCodecParserContext *s,
                     AVCodecContext *avctx,
                     uint8_t **poutbuf, int *poutbuf_size,
                     const uint8_t *buf, int buf_size,
                     int64_t pts, int64_t dts,
                     int64_t pos)
{
    int index, i;
    uint8_t dummy_buf[FF_INPUT_BUFFER_PADDING_SIZE];

    if(!(s->flags & PARSER_FLAG_FETCHED_OFFSET)) {
        s->next_frame_offset =
        s->cur_offset        = pos;
        s->flags |= PARSER_FLAG_FETCHED_OFFSET;
    }

    if (buf_size == 0) {
        /* padding is always necessary even if EOF, so we add it here */
        memset(dummy_buf, 0, sizeof(dummy_buf));
        buf = dummy_buf;
    } else if (s->cur_offset + buf_size !=
               s->cur_frame_end[s->cur_frame_start_index]) { /* skip remainder packets */
        /* add a new packet descriptor */
            i = (s->cur_frame_start_index + 1) & (AV_PARSER_PTS_NB - 1);
            s->cur_frame_start_index = i;
            s->cur_frame_offset[i] = s->cur_offset;
            s->cur_frame_end[i] = s->cur_offset + buf_size;
            s->cur_frame_pts[i] = pts;
            s->cur_frame_dts[i] = dts;
            s->cur_frame_pos[i] = pos;
    }

    if (s->fetch_timestamp){
        s->fetch_timestamp=0;
        s->last_pts = s->pts;
        s->last_dts = s->dts;
        s->last_pos = s->pos;
        ff_fetch_timestamp(s, 0, 0);
    }

    /* WARNING: the returned index can be negative */
    //H264里对应的就是parser_parse=h264_parse,
    index = s->parser->parser_parse(s, avctx, (const uint8_t **)poutbuf, poutbuf_size, buf, buf_size);
//av_log(NULL, AV_LOG_DEBUG, "parser: in:%"PRId64", %"PRId64", out:%"PRId64", %"PRId64", in:%d out:%d id:%d\n", pts, dts, s->last_pts, s->last_dts, buf_size, *poutbuf_size, avctx->codec_id);
    /* update the file pointer */
    if (*poutbuf_size) {
        /* fill the data for the current frame */
        s->frame_offset = s->next_frame_offset;

        /* offset of the next frame */
        s->next_frame_offset = s->cur_offset + index;
        s->fetch_timestamp=1;
    }
    if (index < 0)
        index = 0;
    s->cur_offset += index;
    return index;
}
```


 从index = s->parser->parser_parse(s, avctx, (const uint8_t **)poutbuf, poutbuf_size, buf, buf_size);这句代码可以看出，最终调用了相应解码器的parser_parse()函数。



有点累了，先不做详细分析，以后有机会再补上。














