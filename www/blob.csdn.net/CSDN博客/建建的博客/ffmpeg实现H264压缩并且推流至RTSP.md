# ffmpeg实现H264压缩并且推流至RTSP - 建建的博客 - CSDN博客
2018年09月13日 11:28:16[纪建](https://me.csdn.net/u013898698)阅读数：1035
**一、整体思路**
    1 利用ffmpeg读取出视频的一帧图像   存储在pFrame中
    2 利用ffmpeg进行格式转换    sws_scale
    3 利用ffmpeg编码存在pFrame中的图像   H264压缩
    4 利用ffmpeg的 av_interleaved_write_frame( )  写入到rtsp端
    5 辅助部分：解码、编码初始化，QT线程创建
**二、编解码初始化**
    1 解码初始化
        参考[http://blog.csdn.net/yunge812/article/details/79342089](http://blog.csdn.net/yunge812/article/details/79342089)
    2 编码初始化           
- 
`    //创建AVFormatContext结构体`
- 
`//分配一个AVFormatContext，FFMPEG所有的操作都要通过这个AVFormatContext来进行`
- 
`fmtctx = avformat_alloc_context();`
- 
`//获得输出格式，这里是RTP网络流`
- 
`fmtctx->oformat = av_guess_format("rtp", NULL, NULL);`
- 
`snprintf(fmtctx->filename, sizeof(fmtctx->filename),"rtp://%s:%d",RDIP,RDPORT);`
- 
`//打开网络流 fmtctx->pb:创建的AVIOContext结构体 fmtctx->filename url协议地址 方式为只写`
- 
`avio_open(&fmtctx->pb,fmtctx->filename, AVIO_FLAG_WRITE);`
- 
`//打开输出流`
- 
`video_codec = avcodec_find_encoder(AV_CODEC_ID_H264);`
- 
`if(video_codec == NULL )`
- 
`{`
- 
`qDebug() << "video_codec == NULL"<< endl;`
- 
`}`
- 
`//初始化AVStream`
- 
`video_st = avformat_new_stream(fmtctx, video_codec);`
- 
`if(video_st == NULL )`
- 
`{`
- 
`qDebug() << "video_st == NULL"<< endl;`
- 
`}`
- 
`video_st->id = fmtctx->nb_streams-1; //add`
- 
`//设置AVCodecContext编码参数`
- 
`c = video_st->codec;`
- 
`avcodec_get_context_defaults3(c, video_codec);`
- 
`c->codec_id = AV_CODEC_ID_H264;`
- 
`c->width = OUTWIDTH;`
- 
`c->height = OUTHEIGHT;`
- 
`c->time_base.den = FPS; //分母`
- 
`c->time_base.num = 1; //分子`
- 
`c->pix_fmt = AV_PIX_FMT_YUV422P;`
- 
`c->codec_type = AVMEDIA_TYPE_VIDEO;`
- 
`if(fmtctx->oformat->flags & AVFMT_GLOBALHEADER)`
- 
`c->flags|= CODEC_FLAG_GLOBAL_HEADER;`
- 
`//AVOptions的参数`
- 
`// av_opt_set(c->priv_data, "preset", "ultrafast", 0);`
- 
`// av_opt_set(c->priv_data, "tune","stillimage,fastdecode,zerolatency",0);`
- 
`// av_opt_set(c->priv_data, "x264opts","crf=26:vbv-maxrate=728:vbv-bufsize=364:keyint=25",0);`
- 
`av_opt_set(c->priv_data, "preset", "slow", 0);`
- 
`av_opt_set(c->priv_data, "tune","zerolatency",0);`
- 
`av_opt_set(c->priv_data, "x264opts","crf=26:vbv-maxrate=728:vbv-bufsize=3640:keyint=25",0);`
- 
`//创建AVFormatContext结构体`
- 
`//分配一个AVFormatContext，FFMPEG所有的操作都要通过这个AVFormatContext来进行`
- 
`fmtctx = avformat_alloc_context();`
- 
`//获得输出格式，这里是RTP网络流`
- 
`fmtctx->oformat = av_guess_format("rtp", NULL, NULL);`
- 
`snprintf(fmtctx->filename, sizeof(fmtctx->filename),"rtp://%s:%d",RDIP,RDPORT);`
- 
`//打开网络流 fmtctx->pb:创建的AVIOContext结构体 fmtctx->filename url协议地址 方式为只写`
- 
`avio_open(&fmtctx->pb,fmtctx->filename, AVIO_FLAG_WRITE);`
- 
`//打开输出流`
- 
`video_codec = avcodec_find_encoder(AV_CODEC_ID_H264);`
- 
`if(video_codec == NULL )`
- 
`{`
- 
`qDebug() << "video_codec == NULL"<< endl;`
- 
`}`
- 
`//初始化AVStream`
- 
`video_st = avformat_new_stream(fmtctx, video_codec);`
- 
`if(video_st == NULL )`
- 
`{`
- 
`qDebug() << "video_st == NULL"<< endl;`
- 
`}`
- 
`video_st->id = fmtctx->nb_streams-1; //add`
- 
`//设置AVCodecContext编码参数`
- 
`c = video_st->codec;`
- 
`avcodec_get_context_defaults3(c, video_codec);`
- 
`c->codec_id = AV_CODEC_ID_H264;`
- 
`c->width = OUTWIDTH;`
- 
`c->height = OUTHEIGHT;`
- 
`c->time_base.den = FPS; //分母`
- 
`c->time_base.num = 1; //分子`
- 
`c->pix_fmt = AV_PIX_FMT_YUV422P;`
- 
`c->codec_type = AVMEDIA_TYPE_VIDEO;`
- 
`if(fmtctx->oformat->flags & AVFMT_GLOBALHEADER)`
- 
`c->flags|= CODEC_FLAG_GLOBAL_HEADER;`
- 
`//AVOptions的参数`
- 
`// av_opt_set(c->priv_data, "preset", "ultrafast", 0);`
- 
`// av_opt_set(c->priv_data, "tune","stillimage,fastdecode,zerolatency",0);`
- 
`// av_opt_set(c->priv_data, "x264opts","crf=26:vbv-maxrate=728:vbv-bufsize=364:keyint=25",0);`
- 
`av_opt_set(c->priv_data, "preset", "slow", 0);`
- 
`av_opt_set(c->priv_data, "tune","zerolatency",0);`
- 
`av_opt_set(c->priv_data, "x264opts","crf=26:vbv-maxrate=728:vbv-bufsize=3640:keyint=25",0);`
        这里要注意的是最后一句av_opt_set   不同设置可能导致不同的结果  有时候程序无法运行
        这里需要根据需要自行设置
    //打开编码器
    avcodec_open2(c, video_codec, NULL);
    /*写入数据流的头部*/
    avformat_write_header(fmtctx, NULL);
    //打印SDP信息, 该信息可用于Rtp流接收解码
    char sdp[2048];
    int tmp = av_sdp_create(&fmtctx,1, sdp, sizeof(sdp));
    qDebug() << sdp <<endl;
    if(tmp != 0)
    {
        qDebug() << "av_sdp_create error " <<endl;
    }
        打印信息可以打印出当前的RTSP流的信息   用于另一端捕获用。具体用法后文会有介绍
**三、读取视频**
     3.1 视频读取   
`  av_read_frame(pFormatCtx_decode, packet)`
    3.2  视频解码
` avcodec_decode_video2(pCodecCtx_decode, pFrame, &got_picture, packet)`
    3.3  格式转换
- 
`sws_scale(img_convert_ctx,pFrame->data, pFrame->linesize, 0, pCodecCtx_decode->height, //源`
- 
`pFrameYUV->data, pFrameYUV->linesize); //目的 `
    此时 经过解码的视频存储在pFrameYUV中
**四、编码视频**
` avcodec_encode_video2(c, &pkt, pFrameYUV, &got_output)`
**五 推流至RTSP**
`  av_interleaved_write_frame(fmtctx,&pkt);`
**六 利用VLC播放器接受压缩之后的视频**
    6.1 识别打印信息
        第二步留下打印信息很有用   例如我的打印信息是
- 
`v=0`
- 
`o=- 0 0 IN IP4 127.0.0.1`
- 
`s=No Name`
- 
`c=IN IP4 127.0.0.1`
- 
`t=0 0`
- 
`a=tool:libavformat 57.71.100`
- 
`m=video 5678 RTP/AVP 96`
- 
`a=rtpmap:96 H264/90000`
- 
`a=fmtp:96 packetization-mode=1`
        将上述打印信息直接copy到记事本，另存为 test.sdp的文件
    6.2 再次运行程序  同时将 test.sdp文件拖到VLC播放器中进行播放
    6.3 播放效果     
![](https://img-blog.csdn.net/20180222100705589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVuZ2U4MTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考：http://blog.csdn.net/czc1009/article/details/12913759
         http://blog.csdn.net/ytaosky/article/details/72820329
