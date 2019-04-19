# [转载]使用EasyRTMP调用海康、大华等SDK进行RTMP推送 - xcyl的口袋 - CSDN博客
2016年10月30日 00:05:02[xcyl](https://me.csdn.net/cai6811376)阅读数：2828
本文转自EasyDarwin团队kim的博客：[http://blog.csdn.net/jinlong0603/article/details/52966320](http://blog.csdn.net/jinlong0603/article/details/52966320)
## Demo项目介绍
EasyRTMP Demo代码下载地址[https://github.com/EasyDarwin/EasyRTMP](https://github.com/EasyDarwin/EasyRTMP)
本文介绍Demo中EasyRTMP_SDK的项目。该项目是通过调用IP Camera自己的SDK获取到音视频流，然后通过libEasyRTMP推送到RTMP服务器进行直播。这样就可以将普通的不支持RTMP的IP Camera接入RTMP服务器进行直播推送。该Demo程序可以交叉编译内置到IP Camera内部，或者在Windows或者其他系统中执行此Demo拉取IP Camera的数据流再推送到RTMP服务器。 
Github中Demo里面用的IP Camera的SDK是EasyDarwin官网卖的海芯威视的IP Camera。如果想接入其他家的IP Camera（如海康、大华等）可自己将SDK库及调用方式换成相应的版本。
## 代码解析
下面是IP Camera SDK 数据回调函数的部分，解析回调出来的数据并使用libEasyRTMP库进行RTMP推送。关于回调出来的音频数据如果不是AAC格式，可以通过EasyDarwin团队提供的开源的EasyAACEncoder将其转成AAC格式再推送，当然使用其他的方式只要能转成正确的AAC格式就可以。
```cpp
HI_S32 NETSDK_APICALL OnStreamCallback(HI_U32 u32Handle,
                                HI_U32 u32DataType, 
                                HI_U8* pu8Buffer,
                                HI_U32 u32Length,
                                HI_VOID* pUserData
                                )
{
    HI_S_AVFrame* pstruAV = HI_NULL;
    HI_S_SysHeader* pstruSys = HI_NULL;
    if (u32DataType == HI_NET_DEV_AV_DATA)
    {
        pstruAV = (HI_S_AVFrame*)pu8Buffer;
        if (pstruAV->u32AVFrameFlag == HI_NET_DEV_VIDEO_FRAME_FLAG)
        {
            if(g_RtmpHandle == 0 ) return 0;
            if(pstruAV->u32AVFrameLen > 0)
            {
                unsigned char* pbuf = (unsigned char*)(pu8Buffer+sizeof(HI_S_AVFrame));
                EASY_AV_Frame  avFrame;
                memset(&avFrame, 0x00, sizeof(EASY_AV_Frame));
                avFrame.u32AVFrameLen = pstruAV->u32AVFrameLen;
                avFrame.pBuffer = (unsigned char*)pbuf;
                avFrame.u32VFrameType = (pstruAV->u32VFrameType==HI_NET_DEV_VIDEO_FRAME_I)?EASY_SDK_VIDEO_FRAME_I:EASY_SDK_VIDEO_FRAME_P;
                avFrame.u32AVFrameFlag = EASY_SDK_VIDEO_FRAME_FLAG;
                avFrame.u32TimestampSec = pstruAV->u32AVFramePTS/1000;
                avFrame.u32TimestampUsec = (pstruAV->u32AVFramePTS%1000)*1000;
                EasyRTMP_SendPacket(g_RtmpHandle, &avFrame);
            }   
        }
        else
        if (pstruAV->u32AVFrameFlag == HI_NET_DEV_AUDIO_FRAME_FLAG)
        {
            if(g_RtmpHandle == 0 ) return 0;
            EASY_AV_Frame   avFrame;
            memset(&avFrame, 0x00, sizeof(EASY_AV_Frame));
            avFrame.u32AVFrameFlag = EASY_SDK_AUDIO_FRAME_FLAG;
            if(pstruAV->u32AVFrameLen <= 4) return 0;
            if(EasyInitAACEncoder(EASY_SDK_AUDIO_CODEC_G711A) == 0)
            {
                memset(m_pAACEncBufer, 0, 64*1024);
                unsigned int iAACBufferLen = 0;
                unsigned char* pbuf = (unsigned char*)(pu8Buffer+sizeof(HI_S_AVFrame)) + 4;
                if(Easy_AACEncoder_Encode(g_pAACEncoderHandle, (unsigned char*)pbuf,  pstruAV->u32AVFrameLen-4, m_pAACEncBufer, &iAACBufferLen) > 0)
                {
                    avFrame.pBuffer = (Easy_U8*)(m_pAACEncBufer);
                    avFrame.u32AVFrameLen  = iAACBufferLen; 
                    avFrame.u32TimestampSec = pstruAV->u32AVFramePTS/1000;
                    avFrame.u32TimestampUsec = (pstruAV->u32AVFramePTS%1000)*1000;
                    EasyRTMP_SendPacket(g_RtmpHandle, &avFrame);
                }
            }       
        }
    }
    else
    if (u32DataType == HI_NET_DEV_SYS_DATA)
    {
        pstruSys = (HI_S_SysHeader*)pu8Buffer;
        printf("Video W:%u H:%u Audio: %u \n", pstruSys->struVHeader.u32Width, pstruSys->struVHeader.u32Height, pstruSys->struAHeader.u32Format);
    }
    return HI_SUCCESS;
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
