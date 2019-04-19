# [转载]使用EasyRTMP读取MP4文件进行RTMP推送 - xcyl的口袋 - CSDN博客
2016年10月29日 23:48:04[xcyl](https://me.csdn.net/cai6811376)阅读数：1782
本文转自EasyDarwin团队kim的博客：[http://blog.csdn.net/jinlong0603/article/details/52965101](http://blog.csdn.net/jinlong0603/article/details/52965101)
```
前面已经介绍过EasyRTMP，这里不再多说，可以网上搜索相关内容。
本文讲述下基于libEasyRTMP推送库实现的MP4文件的直播推送。Github中Demo的下载地址https://github.com/EasyDarwin/EasyRTMP。Demo中EasyRTMP_FILE的工程是将MP4文件进行解析推送。主要使用MP4Demux进行解析，分别获取到音频流和视频流，再利用libEasyRTMP进行RTMP推送，这样就可以达到直播或者点播的效果了。
```
首先获取音视频编码信息，判断MP4文件是否存储音频轨和视频轨
```cpp
//从MP4文件获取音视频编码信息，填入pusher媒体信息结构中
    CMp4_avcC_box videoInfo;
    memset(&videoInfo, 0x00, sizeof(CMp4_avcC_box));
    CMp4_mp4a_box audioInfo;
    memset(&audioInfo, 0x00, sizeof(CMp4_mp4a_box));
    int nVideoTrackId = get_video_info_in_moov(g_root,  videoInfo );
    int nAudioTrackId = get_audio_info_in_moov(g_root,  audioInfo );
```
如果存在音频轨、视频轨就起线程进行数据流获取并传给libEasyRTMP进行推送
```
//视频轨存在
    if (nVideoTrackId>-1)
    {
        mediainfo.u32VideoCodec =   EASY_SDK_VIDEO_CODEC_H264;
        mediainfo.u32VideoFps = 25;
        mediainfo.u32H264SpsLength = videoInfo.sps->sequenceParameterSetLength;
        mediainfo.u32H264PpsLength = videoInfo.pps->pictureParameterSetLength;
        if (videoInfo.sps->sequenceParameterSetNALUnit && videoInfo.sps->sequenceParameterSetLength>0 )
        {
            memcpy(mediainfo.u8H264Sps, videoInfo.sps->sequenceParameterSetNALUnit, mediainfo.u32H264SpsLength);
        }
        if (videoInfo.pps->pictureParameterSetNALUnit && videoInfo.pps->pictureParameterSetLength>0 )
        {
            memcpy(mediainfo.u8H264Pps, videoInfo.pps->pictureParameterSetNALUnit, mediainfo.u32H264PpsLength );
        }
        g_mp4TrackThread[nVideoTrackId] = (HANDLE)_beginthreadex(NULL, 0, VideoThread, (void*)nVideoTrackId,0,0);
        g_bThreadLiving[nVideoTrackId] = true;
    }
    //音频轨存在
    if (nAudioTrackId>-1)
    {
        mediainfo.u32AudioCodec =   EASY_SDK_AUDIO_CODEC_AAC;
        mediainfo.u32AudioSamplerate = audioInfo.samplerate;
        mediainfo.u32AudioChannel = audioInfo.channelcount;
        //Create thread to push mp4 demux data( aac)
        g_mp4TrackThread[nAudioTrackId] = (HANDLE)_beginthreadex(NULL, 0, AudioThread,  (void*)nAudioTrackId,0,0);
        g_bThreadLiving[nAudioTrackId] = true;
    }
```
主要的部分就是MP4Demux的使用，Demo中提供了代码，可自行学习。
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
