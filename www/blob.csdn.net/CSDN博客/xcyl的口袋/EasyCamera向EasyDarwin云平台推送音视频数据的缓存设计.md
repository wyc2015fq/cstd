# EasyCamera向EasyDarwin云平台推送音视频数据的缓存设计 - xcyl的口袋 - CSDN博客
2016年10月08日 06:54:57[xcyl](https://me.csdn.net/cai6811376)阅读数：701
EasyCamera在向EasyDarwin云平台推送音视频数据时，有时一个I帧会很大，从摄像机SDK中获取的数据就会被拆分。我们使用EasyPusher向云平台推送时需要每次发送一整帧数据，这时我们就需要设计一个缓存机制将SDK拆分的帧组成一个完整的帧数据，再使用EasyPusher推送给EasyDarwin云平台。这里以海康威视摄像机SDK获取的数据为例。
## 数据分析
我们先看一下摄像机SDK获取的帧数据
![这里写图片描述](https://img-blog.csdn.net/20161008064254351)
这里我们可以看到图片的第三行到第十行是一帧数据。由于数据过大，SDK将其拆分为8个包。我们就需要将这8个数据包组合正一个完整的关键帧推送给云平台。
## 设计思路
- 我们建立一个固定大小的缓存区buffer，和一个用于标记偏移的pos并初始化为0；
- 当我们获取第一个数据包的时候我们判断数据包是否为0x00、0x00、0x00、0x01开头，则表示这是一个I帧或P帧，判断pos是否为0，若为0，则将数据放入buffer，若不是，说明buffer中已经存在一个完整的I帧或者P帧数据，则发送；
- 若获取到的数据开头不为0x00、0x00、0x00、0x01，则判断pos是否为0，若是，则丢弃，若不是，则说明这个数据为拆分的数据，需要加入buffer。
## 代码
```cpp
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
```
## 源码及视频教程
源码地址：[https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK](https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK)
视频教程地址：[http://edu.csdn.net/course/detail/3029](http://edu.csdn.net/course/detail/3029)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
