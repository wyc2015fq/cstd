# [转载]使用EasyRTSPClient与EasyRTMP推送RTSP视频源进行RTMP直播 - xcyl的口袋 - CSDN博客
2016年10月28日 22:02:59[xcyl](https://me.csdn.net/cai6811376)阅读数：1232
本文转自EasyDarwin团队kim的博客：[http://blog.csdn.net/jinlong0603/article/details/52951311](http://blog.csdn.net/jinlong0603/article/details/52951311)
## EasyRTMP
> 
EasyRTMP封装了RTMP协议，提供了一套非常简单易用的接口调用SDK，Github上有多个基于EasyRTMP SDK的Demo。Git地址：[https://github.com/EasyDarwin/EasyRTMP](https://github.com/EasyDarwin/EasyRTMP)， Demo中EasyRTMP_RTSP项目是将RTSP流获取到本地进行RTMP推送，可进行RTMP直播。
## RTSP视频源进行RTMP直播
> 
目前安防项目上，基本都是RTSP流，无法接入RTMP服务器。 
  EasyRTMP_RTSP Demo中通过libEasyRTSPClient库将RTSP数据流获取到本地，再讲获取来的音视频数据送给libEasyRTMP进行RTMP推送。如果获取来的数据不是AAC格式，而是G711、G726、PCM等格式，可以使用EasyDarwin团队提供的开源的EasyAACEncoder将音频数据转换成AAC格式再推送。这样可以实现将RTSP视频源实时的进行RTMP协议直播。
```cpp
/* NVSource从RTSPClient获取数据后回调给上层 */
int Easy_APICALL __RTSPSourceCallBack( int _chid, void *_chPtr, int _mediatype, char *pbuf, RTSP_FRAME_INFO *frameinfo)
{
    if (NULL != frameinfo)
    {
        if (frameinfo->height==1088)        frameinfo->height=1080;
        else if (frameinfo->height==544)    frameinfo->height=540;
    }
    Easy_Bool bRet = 0;
    int iRet = 0;
    //目前只处理视频
    if (_mediatype == EASY_SDK_VIDEO_FRAME_FLAG)
    {
        if(frameinfo && frameinfo->length)
        {
            if( frameinfo->type == EASY_SDK_VIDEO_FRAME_I)
            {
                if(g_rtmpPusher.rtmpHandle == 0)
                {
                    g_rtmpPusher.rtmpHandle = EasyRTMP_Create();
                    bRet = EasyRTMP_Connect(g_rtmpPusher.rtmpHandle, SRTMP);
                    if (!bRet)
                    {
                        printf("Fail to EasyRTMP_Connect ...\n");
                    }
                    EASY_MEDIA_INFO_T mediaInfo;
                    memset(&mediaInfo, 0, sizeof(EASY_MEDIA_INFO_T));
                    mediaInfo.u32VideoFps = 25;
                    mediaInfo.u32AudioSamplerate = 8000;
                    iRet = EasyRTMP_InitMetadata(g_rtmpPusher.rtmpHandle, &mediaInfo, 1024);
                    if (iRet < 0)
                    {
                        printf("Fail to InitMetadata ...\n");
                    }
                }
                EASY_AV_Frame avFrame;
                memset(&avFrame, 0, sizeof(EASY_AV_Frame));
                avFrame.u32AVFrameFlag = EASY_SDK_VIDEO_FRAME_FLAG;
                avFrame.u32AVFrameLen = frameinfo->length;
                avFrame.pBuffer = (unsigned char*)pbuf;
                avFrame.u32VFrameType = EASY_SDK_VIDEO_FRAME_I;
                avFrame.u32TimestampSec = frameinfo->timestamp_sec;
                avFrame.u32TimestampUsec = frameinfo->timestamp_usec;
                iRet = EasyRTMP_SendPacket(g_rtmpPusher.rtmpHandle, &avFrame);
                if (iRet < 0)
                {
                    printf("Fail to EasyRTMP_SendH264Packet(I-frame) ...\n");
                }
                else
                {
                    printf("I");
                }
            }
            else
            {
                if(g_rtmpPusher.rtmpHandle)
                {
                    EASY_AV_Frame avFrame;
                    memset(&avFrame, 0, sizeof(EASY_AV_Frame));
                    avFrame.u32AVFrameFlag = EASY_SDK_VIDEO_FRAME_FLAG;
                    avFrame.u32AVFrameLen = frameinfo->length-4;
                    avFrame.pBuffer = (unsigned char*)pbuf+4;
                    avFrame.u32VFrameType = EASY_SDK_VIDEO_FRAME_P;
                    avFrame.u32TimestampSec = frameinfo->timestamp_sec;
                    avFrame.u32TimestampUsec = frameinfo->timestamp_usec;
                    iRet = EasyRTMP_SendPacket(g_rtmpPusher.rtmpHandle, &avFrame);
                    if (iRet < 0)
                    {
                        printf("Fail to EasyRTMP_SendH264Packet(P-frame) ...\n");
                    }
                    else
                    {
                        printf("P");
                    }
                }
            }               
        }   
    }
    else if (_mediatype == EASY_SDK_AUDIO_FRAME_FLAG)
    {
        EASY_AV_Frame   avFrame;
        memset(&avFrame, 0x00, sizeof(EASY_AV_Frame));
        avFrame.u32AVFrameFlag = EASY_SDK_AUDIO_FRAME_FLAG;
        avFrame.u32TimestampSec = frameinfo->timestamp_sec;
        avFrame.u32TimestampUsec = frameinfo->timestamp_usec;
        if(frameinfo->codec == EASY_SDK_AUDIO_CODEC_AAC)
        {
            avFrame.pBuffer = (Easy_U8*)(pbuf);
            avFrame.u32AVFrameLen  = frameinfo->length; 
            printf("*");
            iRet = EasyRTMP_SendPacket(g_rtmpPusher.rtmpHandle, &avFrame);
        }
        else if ((frameinfo->codec == EASY_SDK_AUDIO_CODEC_G711A) || (frameinfo->codec == EASY_SDK_AUDIO_CODEC_G711U) || (frameinfo->codec == EASY_SDK_AUDIO_CODEC_G726))
        {
            if(EasyInitAACEncoder(frameinfo) == 0)
            {
                memset(g_rtmpPusher.m_pAACEncBufer, 0, 64*1024);
                unsigned int iAACBufferLen = 0;
                if(Easy_AACEncoder_Encode(g_rtmpPusher.m_pAACEncoderHandle, (unsigned char*)pbuf,  frameinfo->length, g_rtmpPusher.m_pAACEncBufer, &iAACBufferLen) > 0)
                {
                    printf("*");
                    avFrame.pBuffer = (Easy_U8*)(g_rtmpPusher.m_pAACEncBufer);
                    avFrame.u32AVFrameLen  = iAACBufferLen; 
                    iRet = EasyRTMP_SendPacket(g_rtmpPusher.rtmpHandle, &avFrame);
                }
            }
        }
    }
    return 0;
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
