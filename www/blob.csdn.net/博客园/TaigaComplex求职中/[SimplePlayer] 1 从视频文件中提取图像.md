# [SimplePlayer] 1. 从视频文件中提取图像 - TaigaComplex求职中 - 博客园







# [[SimplePlayer] 1. 从视频文件中提取图像](https://www.cnblogs.com/TaigaCon/p/9603854.html)





在开始之前，我们需要了解视频文件的格式。视频文件的格式众多，无法三言两语就能详细分析其结构，尽管如此，ffmpeg却很好地提取了各类视频文件的共同特性，并对其进行了抽象描述。

![image](https://img2018.cnblogs.com/blog/421096/201809/421096-20180907122020940-1083957608.png)

视频文件格式，统称为container。它包含一个描述视频信息的头部，以及内含实际的音视频编码数据的packets。当然，这里的头部以及packet部分只是个抽象描述，实际的视频格式的描述信息可能不是存放在视频文件的起始位置，可能是由分散于视频文件的各个位置的多个部分组成；数据包有可能是由头部以及尾部进行分割的传统数据包形式，也有可能是一大块数据区域，由索引进行各个数据包的分割。

视频文件中的packets最主要的就是视频以及音频packets，demux的过程就是解析container的header来获取视频信息，所得到的视频信息能帮助我们区分packet是音频或者视频。同样属性的packets会被称为stream。

packet中存储的数据就是音视频编码后的数据，通过解码器进行decode后就能得到视频图像或者音频帧。其中需要注意的一点是，一个packet不一定对应一帧，packet的顺序也不一定是实际的播放顺序，而通过ffmpeg解码出来的frame的顺序就是实际的播放顺序。



# Demux

首先需要一个用于存储视频文件信息的结构体。
pFormatCtx = avformat_alloc_context();


读取视频文件，并对该文件进行demux，所得到的视频信息存储于刚刚所构建的结构体当中
    if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0){
        fprintf(stderr, "open input failed\n");
        return -1;
    }
如果pFormatCtx=NULL，那么avformat_open_input也能自动为pFormatCtx分配存储空间。



对于有些视频格式，单单通过demux并不能获得所有的视频信息，为了获得这些信息，还需要读取并尝试解码该视频几个最前端packets（通常会解码每个stream第一个packet）。所读取的这几个packets会被缓存以供后续处理。
if(avformat_find_stream_info(pFormatCtx, NULL)<0){
        fprintf(stderr, "find stream info failed\n");
        return -1;
    }


从所获得的信息当中得到video stream序号，后续可以通过stream序号来对packet进行筛选。
videoStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);




# Decode

创建一个用于存储以及维护解码信息结构体。
pCodecCtx = avcodec_alloc_context3(NULL);


把demux时所获得的视频相关信息传递到解码结构体中。
if(avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar)<0){
        fprintf(stderr, "copy param from format context to codec context failed\n");
        return -1;
    }


根据解码器id来寻找对应的解码器
pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        fprintf(stderr, "Unsupported codec,codec id %d\n", pCodecCtx->codec_id);
        return -1;
    }else{
        fprintf(stdout, "codec id is %d\n", pCodecCtx->codec_id);
    }


打开该解码器，主要目的是对解码器进行初始化
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0){
        fprintf(stderr, "open codec failed\n");
        return -1;
    }


创建一个用于维护所读取的packet的结构体，一个用于维护解码所得的frame的结构体
    pPacket = av_packet_alloc();
    pFrame = av_frame_alloc();
    if(pFrame == NULL||pPacket == NULL){
        fprintf(stderr, "cannot get buffer of frame or packet\n");
        return -1;
    }


从视频文件中读取packet，如果所读取的packet是video，则进行解码，解码所得的帧由pFrame进行维护。当然，并不是每次调用avcodec_decode_video2都会返回一帧，因为也可能会有需要多个packet才能解码出一帧的情况，因此只有当指示一帧是否解码完成的frameFinished为1才能对这一帧进行后续处理。
    while(av_read_frame(pFormatCtx, pPacket)>=0){
        //Only deal with the video stream of the type "videoStream"
        if(pPacket->stream_index==videoStream){
            //Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, pPacket);
            //fprintf(stdout, "Frame : %d ,pts=%lld, timebase=%lf\n", i, pFrame->pts, av_q2d(pFormatCtx->streams[videoStream]->time_base));
            if(frameFinished){
                if(i>=START_FRAME && i<=END_FRAME){
                    SaveFrame2YUV(pFrame, pCodecCtx->width, pCodecCtx->height, i);
                    i++;
                }else{
                    i++;
                    continue;
                }
            }
        }
        av_packet_unref(pPacket);
    }
当一个packet被解码后就可以调用av_packet_unref来释放该packet所占用的空间了。





# Store

视频文件解码出来后通常都是YUV格式，Y、U、V三路分量分别存储在AVFrame的data[0]、data[1]、data[2]所指向的内存区域。linesize[0]、linesize[1]、linesize[2]分别指示了Y、U、V一行所占用的字节数。下面把解码所得的帧保存为YUV Planar格式。
void SaveFrame2YUV(AVFrame *pFrame, int width, int height, int iFrame){
    static FILE *pFile;
    char szFilename[32];
    int y;

    //Open file
    if(iFrame==START_FRAME){
         sprintf(szFilename, "Video.yuv");
        pFile = fopen(szFilename, "wb");
        if(pFile==NULL)
            return;
    }

    //Write YUV Data, Only support YUV420
    //Y
    for(y=0; y<height; y++){
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, pFrame->linesize[0], pFile);
    }
    //U
    for(y=0; y<(height+1)/2; y++){
        fwrite(pFrame->data[1]+y*pFrame->linesize[1], 1, pFrame->linesize[1], pFile);
    }
    //V
    for(y=0; y<(height+1)/2; y++){
        fwrite(pFrame->data[2]+y*pFrame->linesize[2], 1, pFrame->linesize[2], pFile);
    }

    //Close FIle
    if(iFrame==END_FRAME){
        fclose(pFile);
    }
}




最后就是释放内存，关闭decoder，关闭demuxer
    av_free(pPacket);
    av_free(pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);











