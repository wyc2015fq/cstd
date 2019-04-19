# [转载]EasyCamera中海康摄像头语音对讲和云台控制转发实现 - xcyl的口袋 - CSDN博客
2016年10月24日 16:56:25[xcyl](https://me.csdn.net/cai6811376)阅读数：1813
转自：[http://blog.csdn.net/yanzhuomin/article/details/52887311](http://blog.csdn.net/yanzhuomin/article/details/52887311)
EasyCamera中关于摄像头SDK的调用都集中在EasyCameraSource.cpp中，调用顺序：
## 一、开启流转发：
点击摄像头查看视频时调用：EasyCameraSource::StartStreaming具体调用流程为：
1.首先登陆摄像头：
```bash
if (!cameraLogin())
{
    theErr = QTSS_RequestFailed;
    break;
}
```
2.获取摄像头配置信息：（我的海康摄像头获取的音频采样率为32k，但是我实际测试 
8000hz才能正确播放）
```
if (!NET_DVR_GetDVRConfig(m_u32Handle, NET_DVR_GET_COMPRESSCFG_V30, 1, &struCompressionCfg, sizeof(NET_DVR_COMPRESSIONCFG_V30), &dwReturned))
{
    printf("can't get config！\n");
    mediainfo.u32VideoFps = 25;
    mediainfo.u32AudioCodec = EASY_SDK_AUDIO_CODEC_G711A;               
    mediainfo.u32AudioSamplerate = 8000;
}
else
{
    mediainfo.u32VideoFps = getFrameRateFromHKSDK(struCompressionCfg.struNetPara.dwVideoFrameRate);
    mediainfo.u32AudioCodec = getAudioCodecFromHKSDK(struCompressionCfg.struNetPara.byAudioEncType);
    mediainfo.u32AudioSamplerate = getAudioSimpleRateFromHKSDK(struCompressionCfg.struNetPara.byAudioSamplingRate);
    //mediainfo.u32AudioBitsPerSample = struCompressionCfg.struNetPara.byAudioBitRate;
}
```
3.开启EasyPusher推送开始
```bash
// 注册流推送事件回调
EasyPusher_SetEventCallback(fPusherHandle, __EasyPusher_Callback, 0, NULL);
// 根据接收到的命令生成流信息
char sdpName[128] = { 0 };
sprintf(sdpName, "%s/%s.sdp", /*inParams->inStreamID,*/ inParams->inSerial, inParams->inChannel);
// 开始推送流媒体数据
EasyPusher_StartStream(fPusherHandle, (char*)inParams->inIP, inParams->inPort, sdpName, "", "", &mediainfo, 1024/* 1M Buffer*/, 0);
saveStartStreamParams(inParams);
```
4.调用netDevStartStream();开启摄像头流转发
在该函数中调用海康摄像头的请求流转发API，并设置回调函数myStreamProc具体流转发 就在该回调函数中 ：streamHandle    =NET_DVR_RealPlay_V40(m_u32Handle,  &struPlayInfo, myStreamProc,this);
## 二、流转发过程
1.EasyCameraSource::PushFrame函数中实现转发： 
PushFrame函数为海康摄像头回调函数直接调用，首先调用：GetH246FromPS(frame, len,  
&h264Buf, h264Len, isVideo, isAudio); 从海康摄像头的h264编码的PS流中解析得到H264 
标准流，标准流中包含视频帧、语音帧(摄像头内设置为复合流才能获取到)和H264的其他帧。 
具体参见：[海康ps流转换h264流](http://blog.csdn.net/occupy8/article/details/39288035)该博客有详细说明 
2.发送解析得到的H264标准帧：
```
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
```
具体看获取到的帧为视频帧还是音频帧，进行不同的处理转发即可
## 三、语音转发过程：
当客户端调用语音对讲的时候则会调用到该类的EasyCameraSource::ControlTalkback函数： 
1.根据客户端的命令类型调用不同的海康SDK的API，如果命令为 
‘’EASY_TALKBACK_CMD_TYPE_START‘’则调用： 
NET_DVR_StartVoiceCom_MR_V30(m_u32Handle,1, myAudioStreamProc, (void*)this);//建立语音转发 
如果为”EASY_TALKBACK_CMD_TYPE_STOP”则调用： 
result =NET_DVR_StopVoiceCom(audioHandle);//停止语音转发 
如果为”EASY_TALKBACK_CMD_TYPE_SENDDATA”则调用： 
result =NET_DVR_VoiceComSendData(audioHandle, (params->inBuff)+sizeof(char)*len,160); 
转发客户端音频数据到摄像头。由于海康SDK每次发送数据有最大数量限制，所以while循环 
发送直到数据完。具体参见海康SDK说明文档。至此就实现海康摄像头的音频播放功能。 
2.语音流摄像头读取：在开始的摄像头开启语音转发函数中设置了回调函数myAudioStreamProc可以 
获取到摄像头的音频数据流，在回调函数中调用pThis->PushFrame((unsignedchar*)pBuffer,  
dwBufSize, NET_DVR_AUDIOSTREAMDATA);将获取到的音频数据进行转发，具体同上述的流转发过程类似。
## 四、云台控制：
云台控制相对比较简单，根据客户端调用的命令传入调用相应摄像头SDK的函数即可，只不过两边定义的 
上下左右运动的命令不太一样，做相应转换即可。
```
if (cameraLogin())
{
    if(NET_DVR_PTZPreset(m_u32Handle, getPTZCMDFromCMDType(params->inCommand), params->inPreset))
        return QTSS_NoErr;
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
