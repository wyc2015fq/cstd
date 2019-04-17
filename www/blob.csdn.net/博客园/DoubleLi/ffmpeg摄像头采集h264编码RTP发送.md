# ffmpeg摄像头采集h264编码RTP发送 - DoubleLi - 博客园






一. 相关API说明

```
1. av_register_all 
2. avformat_network_init 
    不管是流媒体发送还是流媒体接收, 需要先执行该函数. 
3. avformat_alloc_context 
    初始化一个AVFormatContext. 
4. avcodec_alloc_frame
    初始化1个AVFrame. **需要注意的是，新版的FFmpeg使用av_frame_alloc来初始化一个AVFrame.**
5. av_guess_format
    返回一个已经注册的最合适的匹配输入参数地输出格式.
6. avio_open
    打开FFmpeg的输入输出文件. **需要注意的是, 新版的Ffmpeg使用avio_open2来打开输入输出文件.**
7. avcodec_find_encoder
    通过code ID查找一个已经注册的音视频编码器.
8. avformat_new_stream
    初始化一个AVStream. **销毁函数使用销毁AVFormatContext的avformat_free_context()就可以了**
9. avcodec_get_context_defaults3
    用于设置该AVCodecContext的默认值.
10. av_opt_set
    设置一个AVOption的值.
11. avcodec_open2
    打开编码器.
12. avformat_write_header
    写文件头.
13. av_sdp_create
    打印sdp信息.
14. av_init_packet
     使用默认值初始化AVPacket.
15. avcodec_encode_video2
    编码一帧视频数据.
16. av_rescale_q
    用于计算Packet的PTS.
17. av_interleaved_write_frame
    写入一个AVPacket到输出文件.
18. av_free_packet
    销毁一个AVPacket. 
19. av_frame_free
    销毁一个AVFrame. 
20. avcodec_close
    关闭编码器.
21. av_free
    av_free()简单的封装了free.
22. avformat_free_context
    销毁一个AVFormatContext.
```

二. 工作流程

```
1. 初始化, 依次完成以下工作
    av_register_all();

    //使用RTSP网络流时必须先执行
    avformat_network_init();                                                             
    pFrame = avcodec_alloc_frame();                                              
    fmtctx = avformat_alloc_context();         

     //设置流格式为RTP                                   
    fmtctx->oformat = av_guess_format("rtp", NULL, NULL);         
    //用指定IP和端口构造输出流地址
    sprintf(fmtctx->filename,"rtp://%s:%d",SendIp.toLatin1().data(),SendPort);

    //打开输出流
    avio_open(&fmtctx->pb,fmtctx->filename, AVIO_FLAG_WRITE);

    //查找编码器
    *codec = avcodec_find_encoder(codec_id);

    //初始化AVStream
    st = avformat_new_stream(oc, *codec);

    //设置AVCodecContext编码参数    
    avcodec_get_context_defaults3(c, *codec);
    c->codec_id = codec_id;
    c->codec_type = AVMEDIA_TYPE_VIDEO;
    c->width = capWidth;
    c->height = capHeight;
    c->time_base.den = frameRate;
    c->time_base.num = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;   //PIX_FMT_YUV420P;
    if(oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags|= CODEC_FLAG_GLOBAL_HEADER;

    av_opt_set(c->priv_data, "preset", "slow", 0);
    av_opt_set(c->priv_data, "tune","zerolatency",0);
    av_opt_set(c->priv_data, "x264opts","crf=26:vbv-maxrate=728:vbv-bufsize=3640:keyint=25",0);

    //打开编码器
    avcodec_open2(video_st->codec, video_codec, NULL);

    //写文件头
    avformat_write_header(fmtctx, NULL);

    //打印SDP信息, 该信息可用于Rtp流接收解码
    av_sdp_create(&fmtctx,1, sdp, sizeof(sdp));
2. 视频编码, rtp传输.  该处使用一个单独的线程完成.
    while (1) 
    {
        //退出线程, 略

        //获取RGB图像
        cap>>srcMat;

        //格式转换, BGR->YUV420P
        convertMatBGR2YUV420(dstMat ,yMat,uMat,vMat);

        //构造AVPacket.
        av_init_packet(&pkt);

        pFrame->pts = video_st->codec->frame_number;

         //视频编码
        avcodec_encode_video2(c, &pkt,pFrame, &got_output);
        if (got_output) {
            if (c->coded_frame->key_frame)
                pkt.flags |= AV_PKT_FLAG_KEY;
            pkt.stream_index = st->index;

            //计算PTS
            if (pkt.pts != AV_NOPTS_VALUE ) {
                pkt.pts = av_rescale_q(pkt.pts,video_st->codec->time_base, video_st->time_base);
            }
            if(pkt.dts !=AV_NOPTS_VALUE ) {
                pkt.dts = av_rescale_q(pkt.dts,video_st->codec->time_base, video_st->time_base);
            }

            //写入一个AVPacket到输出文件, 这里是一个输出流
            av_interleaved_write_frame(oc,&pkt);
        }

        //销毁AVPacket.
        av_free_packet(&pkt);
    }

    //销毁资源
    av_frame_free(&pFrame);
    avcodec_close(video_st->codec);
    av_free(video_st->codec);
    avformat_free_context(fmtctx);

3. 其他, 略.
```

三. 运行 
![程序运行后](https://img-blog.csdn.net/20170605161918641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveVRhb1NreQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击”打开”按纽, 启动RTP传输. 
![这里写图片描述](https://img-blog.csdn.net/20170605161012047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveVRhb1NreQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

RTP接收 
![这里写图片描述](https://img-blog.csdn.net/20170605163722308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveVRhb1NreQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









