# [SimplePlayer] 4. 从视频文件中提取音频 - TaigaComplex求职中 - 博客园







# [[SimplePlayer] 4. 从视频文件中提取音频](https://www.cnblogs.com/TaigaCon/p/9901527.html)





提取音频，具体点来说就是提取音频帧。提取方法与[从视频文件中提取图像](https://www.cnblogs.com/TaigaCon/p/9603854.html)的方法基本一样，这里仅列出其中的不同点：

1. 由于目的提取音频，因此在demux的时候需要指定的是提取audio stream
AudioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);


2. 在decode时，解码音频与解码视频分别采用的是不同的API
avcodec_decode_audio4(pCodecCtx, pFrame, &frameFinished, &packet);


3. 由于本播放器的音频输出实现是基于SDL，而SDL支持输出为S16的[音频样本格式](https://www.cnblogs.com/TaigaCon/p/9893847.html)（也许是系统原因？），因此我们这里在保存文件时，如果解码出来的音频样本格式不是S16，那么就需要对其进行格式转换，以方便后续读取文件进行播放。下面以最常见的FLTP格式为例，把该格式的音频转换为S16格式的音频。
    sample_count = pFrame->nb_samples;
    channel0 = (float *)pFrame->extended_data[0];
    channel1 = (float *)pFrame->extended_data[1];
    
    //normal PCM is mixed(interleave) track, but fltp "p" means planar
    if(pFrame->format == AV_SAMPLE_FMT_FLTP) 
    {
        for(i=0; i<sample_count; i++){ //stereo 
            sample0 = (short)(channel0[i]*32767.0f);
            sample1 = (short)(channel1[i]*32767.0f);
            fwrite(&sample0, 2, 1, pFile);
            fwrite(&sample1, 2, 1, pFile);
        }
    }











