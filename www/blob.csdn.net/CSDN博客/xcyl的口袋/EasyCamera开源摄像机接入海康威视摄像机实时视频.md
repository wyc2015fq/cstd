# EasyCamera开源摄像机接入海康威视摄像机实时视频 - xcyl的口袋 - CSDN博客
2016年10月08日 11:40:05[xcyl](https://me.csdn.net/cai6811376)阅读数：5544
EasyCamera接收云平台实时视频请求命令，会向EasyDarwin流媒体服务器进行推流操作。那么EasyCamera是如何通过海康摄像机SDK获取到媒体流的呢？
我们继续看海康摄像机SDK
![这里写图片描述](https://img-blog.csdn.net/20161008112605817)
这里使用NET_DVR_RealPlay_V40/NET_DVR_StopRealPlay接口控制实时视频。
在获取实时视频调用之前，我们需要知道摄像机目前的参数配置，比如FPS，码率等等。
![这里写图片描述](https://img-blog.csdn.net/20161008113102721)
```
//参数获取
NET_DVR_COMPRESSIONCFG_V30 struCompressionCfg;
memset(&struCompressionCfg, 0, sizeof(struCompressionCfg));
DWORD dwReturned = 0;
EASY_MEDIA_INFO_T mediainfo;
memset(&mediainfo, 0x00, sizeof(EASY_MEDIA_INFO_T));
mediainfo.u32VideoCodec = EASY_SDK_VIDEO_CODEC_H264;
mediainfo.u32AudioChannel = 1;
if (!NET_DVR_GetDVRConfig(m_u32Handle, NET_DVR_GET_COMPRESSCFG_V30, 1, &struCompressionCfg, sizeof(NET_DVR_COMPRESSIONCFG_V30), &dwReturned))
{
    printf("can't get config！\n");
    mediainfo.u32VideoFps = 25;
    mediainfo.u32AudioCodec = EASY_SDK_AUDIO_CODEC_G711A;               
    mediainfo.u32AudioSamplerate = 8000;
}
else
{
    mediainfo.u32VideoFps = getFrameRateFromHKSDK(struCompressionCfg.struNormHighRecordPara.dwVideoFrameRate);
    mediainfo.u32AudioCodec = getAudioCodecFromHKSDK(struCompressionCfg.struNormHighRecordPara.byAudioEncType);
    mediainfo.u32AudioSamplerate = getAudioSimpleRateFromHKSDK(struCompressionCfg.struNormHighRecordPara.byAudioSamplingRate);
}
```
```
//实时视频调用
NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
struPlayInfo.hPlayWnd = NULL; //需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
struPlayInfo.lChannel = 1; //预览通道号
struPlayInfo.dwStreamType = 0; //0-主码流， 1-子码流， 2-码流 3， 3-码流 4，以此类推
struPlayInfo.dwLinkMode = 0; //0- TCP 方式， 1- UDP 方式， 2- 多播方式， 3- RTP 方式， 4-RTP/RTSP， 5-RSTP/HTTP
struPlayInfo.bBlocked = 1; //0- 非阻塞取流， 1- 阻塞取流
streamHandle = NET_DVR_RealPlay_V40(m_u32Handle, &struPlayInfo, myStreamProc, this);
if (streamHandle < 0)
{
    qtss_printf("NET_DVR_RealPlay_V40 Fail\n");
    LONG error = NET_DVR_GetLastError();
    return QTSS_RequestFailed;
}
```
```cpp
//数据回调
QTSS_Error EasyCameraSource::PushFrame(unsigned char* frame, int len, DWORD dataType)
{
    OSMutexLocker locker(&fStreamingMutex);
    if (fPusherHandle == NULL) return QTSS_Unimplemented;
    if (dataType == NET_DVR_STREAMDATA)
    {       
        if (len > 0)
        {
            unsigned char *h264Buf = NULL;
            int h264Len = 0;
            BOOL isVideo;
            GetH246FromPS(frame, len, &h264Buf, h264Len, isVideo);
            if (isVideo)
            {
                if (h264Buf)
                {
                    /*printf("size:%04d\t[0]:0x%08X\t[1]:0x%08X\t[2]:0x%08X\t[3]:0x%08X\t[4]:0x%08X \n",
                        h264Len, h264Buf[0], h264Buf[1], h264Buf[2], h264Buf[3], h264Buf[4]);*/
                    if (h264Buf[0] == 0 && h264Buf[1] == 0 && h264Buf[2] == 0 && h264Buf[3] == 1)
                    {
                        if (fPusherBuffOffset > 0)
                        {
                            EASY_AV_Frame avFrameVideo;
                            memset(&avFrameVideo, 0x00, sizeof(EASY_AV_Frame));
                            avFrameVideo.u32AVFrameLen = fPusherBuffOffset;
                            avFrameVideo.pBuffer = (unsigned char*)fPusherBuff;
                            bool isKeyFrame = IsIFrame(fPusherBuff);
                            avFrameVideo.u32VFrameType = isKeyFrame ? EASY_SDK_VIDEO_FRAME_I : EASY_SDK_VIDEO_FRAME_P;
                            avFrameVideo.u32AVFrameFlag = EASY_SDK_VIDEO_FRAME_FLAG;
                            //avFrameVideo.u32TimestampSec = pstruAV->u32AVFramePTS / 1000;
                            //avFrameVideo.u32TimestampUsec = (pstruAV->u32AVFramePTS % 1000) * 1000;
                            Easy_U32 ret = EasyPusher_PushFrame(fPusherHandle, &avFrameVideo);
                            //printf("-- Pushing: Frame %s Result %d \n", isKeyFrame ? "I" : "P", ret);
                            fPusherBuffOffset = 0;                          
                        }
                        memcpy(fPusherBuff, h264Buf, h264Len);
                        fPusherBuffOffset += h264Len;
                    }
                    else
                    {
                        if (fPusherBuffOffset > 0)
                        {
                            memcpy(fPusherBuff + fPusherBuffOffset, h264Buf, h264Len);
                            fPusherBuffOffset += h264Len;
                        }
                    }
                }
                if (h264Buf)
                {
                    delete[] h264Buf;
                    h264Buf = NULL;
                }
            }
        }
    }
    else if (dataType == NET_DVR_AUDIOSTREAMDATA)
    {
        if (len > 0)
        {
            EASY_AV_Frame avFrameAudio;
            memset(&avFrameAudio, 0x00, sizeof(EASY_AV_Frame));
            avFrameAudio.u32AVFrameLen = len;
            avFrameAudio.pBuffer = (unsigned char*)frame;
            avFrameAudio.u32AVFrameFlag = EASY_SDK_AUDIO_FRAME_FLAG;
            //avFrameAudio.u32TimestampSec = pstruAV->u32AVFramePTS / 1000;
            //avFrameAudio.u32TimestampUsec = (pstruAV->u32AVFramePTS % 1000) * 1000;
            EasyPusher_PushFrame(fPusherHandle, &avFrameAudio);
        }
    }
    return Easy_NoErr;
}
```
## 源码及视频教程
源码地址：[https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK](https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK)
视频教程地址：[http://edu.csdn.net/course/detail/3029](http://edu.csdn.net/course/detail/3029)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
