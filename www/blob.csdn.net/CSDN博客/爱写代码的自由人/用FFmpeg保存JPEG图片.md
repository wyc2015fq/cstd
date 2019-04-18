# 用FFmpeg保存JPEG图片 - 爱写代码的自由人 - CSDN博客





2018年02月21日 10:47:19[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1928








```cpp
//函数作用：将解码出来的YUV数据保存成JPG图像
//out_file_name -- JPEG图像保存路径
//w, h -- 图像宽高
//linesize -- 图像的Y分量宽度（一般4字节对齐）
//Y, U, V -- 指向图像Y，U，V三个平面的指针
//
int EncodeAndSaveJPEG(const char* out_file_name, int w, int h, int linesize, uint8_t * Y, uint8_t * U, uint8_t * V)
{
    AVFormatContext* pFormatCtx;
    AVOutputFormat* fmt;
    AVStream* video_st;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;
    uint8_t* picture_buf;
    AVFrame* picture;
    AVPacket pkt;
    int y_size;
    int got_picture=0;
    int size;
    int ret=0;

    //av_register_all();

#if 0
    //Method 1
    pFormatCtx = avformat_alloc_context();
    //Guess format
    fmt = av_guess_format("mjpeg", NULL, NULL);
    pFormatCtx->oformat = fmt;
    //Output URL
    if (avio_open(&pFormatCtx->pb, out_file_name, AVIO_FLAG_READ_WRITE) < 0){
        fprintf(stderr, "Couldn't open output file.\n");
        return -1;
    }
#else
    //Method 2. More simple
    avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file_name);
    fmt = pFormatCtx->oformat;
#endif

    video_st = avformat_new_stream(pFormatCtx, 0);
    if (video_st==NULL){
        return -1;
    }
    pCodecCtx = video_st->codec;
    pCodecCtx->codec_id = fmt->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = PIX_FMT_YUVJ420P;

    pCodecCtx->width  = w;
    pCodecCtx->height = h;

    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    //Output some information
    av_dump_format(pFormatCtx, 0, out_file_name, 1);

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec){
        fprintf(stderr, "jpeg Codec not found. \n");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0){
        fprintf(stderr, "Could not open jpeg codec. \n");
        return -1;
    }
    //picture = av_frame_alloc();
    picture = avcodec_alloc_frame();
    size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    picture_buf = (uint8_t *)av_malloc(size);
    if (!picture_buf)
    {
        avcodec_close(video_st->codec);
        avio_close(pFormatCtx->pb);
        avformat_free_context(pFormatCtx);
        return -1;
    }
    avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

    if(m_jpegQuality > 0)
    {
        if(m_jpegQuality > 100)
            m_jpegQuality = 100;

       pCodecCtx->qcompress = (float)m_jpegQuality/100.f; // 0~1.0, default is 0.5
       pCodecCtx->qmin = 2;
       pCodecCtx->qmax = 31;
       pCodecCtx->max_qdiff = 3;

       fprintf(stderr, "JPEG quality is: %d\n", m_jpegQuality);
    }

    //Write Header
    avformat_write_header(pFormatCtx,NULL);

    y_size = pCodecCtx->width * pCodecCtx->height;
    av_new_packet(&pkt, y_size*3);

    picture->data[0] = Y;
    picture->data[1] = U;
    picture->data[2] = V;
    picture->linesize[0] = linesize;
    picture->linesize[1] = linesize/2;
    picture->linesize[2] = linesize/2;

    //Encode
    ret = avcodec_encode_video2(pCodecCtx, &pkt, picture, &got_picture);
    if(ret < 0){
        fprintf(stderr, "avcodec_encode_video2 Error.\n");
        return -1;
    }
    if (got_picture==1){
        pkt.stream_index = video_st->index;
        ret = av_write_frame(pFormatCtx, &pkt);
    }

    av_free_packet(&pkt);
    //Write Trailer
    av_write_trailer(pFormatCtx);

    if (video_st){
        avcodec_close(video_st->codec);
        av_free(picture);
        av_free(picture_buf);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    return 0;
}
```

在上面的函数中，m_jpegQuality 是 JPEG的编码质量，取值范围为：0-100,值越大质量越好，但文件体积越大，默认值是80。还要记得一点：上面输入的Y，U，V平面是假设为YUV420格式排列的，如果是其他YUV格式，要稍微改一下代码，主要是修改Y，U，V分量的Linesize大小。


调用方式：

```cpp
EncodeAndSaveJPEG(m_jpegPath,  m_pVideoCodecCtx->width, abs(m_pVideoCodecCtx->height), picture->linesize[0],
                                      picture->data[0], picture->data[1], picture->data[2]);
```




