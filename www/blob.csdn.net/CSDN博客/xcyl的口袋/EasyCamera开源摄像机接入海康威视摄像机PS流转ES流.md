# EasyCamera开源摄像机接入海康威视摄像机PS流转ES流 - xcyl的口袋 - CSDN博客
2016年10月08日 06:30:24[xcyl](https://me.csdn.net/cai6811376)阅读数：2095
海康威视使用PS流封装H.264流，EasyDarwin云平台支持ES流。当我们需要接入海康的视频时，我们就需要将海康的PS流转换为ES流。 
首先，我们要知道什么PS流，ES流。
> 
ES流(Elementary Stream): 也叫基本码流,包含视频、音频或数据的连续码流。
PS流(Packet Elementary Stream): 也叫打包的基本码流, 是将基本的码流ES流根据需要分成长度不等的数据包, 并加上包头就形成了打包的基本码流PS流。
TS流(Transport Stream): 也叫传输流, 是由固定长度为188字节的包组成, 含有独立时基的一个或多个program, 一个program又可以包含多个视频、音频、和文字信息的ES流; 每个ES流会有不同的PID标示. 而又为了可以分析这些ES流, TS有一些固定的PID用来间隔发送program和ES流信息的表格: PAT和PMT表。
关于PS流的格式解析，大家可以参照 [PS流的格式和解析总结](http://www.cnblogs.com/lihaiping/p/4181607.html)
那我们如何将PS流转换为ES流呢？（这里要感谢EasyDarwin团队的Sword）
源码地址：[https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK](https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK)
视频教程地址：[http://edu.csdn.net/course/detail/3029](http://edu.csdn.net/course/detail/3029)
```
bool GetH246FromPS(IN BYTE* pBuffer, IN int nBufLenth, BYTE** pH264, int& nH264Lenth, BOOL& bVideo)
{
    if (!pBuffer || nBufLenth <= 0)
    {
        return FALSE;
    }
    BYTE* pH264Buffer = NULL;
    int nHerderLen = 0;
    if (pBuffer
        && pBuffer[0] == 0x00
        && pBuffer[1] == 0x00
        && pBuffer[2] == 0x01
        && pBuffer[3] == 0xE0)//E==视频数据(此处E0标识为视频)
    {
        bVideo = TRUE;
        nHerderLen = 9 + (int)pBuffer[8];//9个为固定的数据包头长度，pBuffer[8]为填充头部分的长度
        pH264Buffer = pBuffer + nHerderLen;
        if (*pH264 == NULL)
        {
            *pH264 = new BYTE[nBufLenth];
        }
        if (*pH264&&pH264Buffer && (nBufLenth - nHerderLen)>0)
        {
            memcpy(*pH264, pH264Buffer, (nBufLenth - nHerderLen));
        }
        nH264Lenth = nBufLenth - nHerderLen;
        return TRUE;
    }
    else if (pBuffer
        && pBuffer[0] == 0x00
        && pBuffer[1] == 0x00
        && pBuffer[2] == 0x01
        && pBuffer[3] == 0xC0) //C==音频数据？
    {
        *pH264 = NULL;
        nH264Lenth = 0;
        bVideo = FALSE;
    }
    else if (pBuffer
        && pBuffer[0] == 0x00
        && pBuffer[1] == 0x00
        && pBuffer[2] == 0x01
        && pBuffer[3] == 0xBA)//视频流数据包 包头
    {
        bVideo = TRUE;
        *pH264 = NULL;
        nH264Lenth = 0;
        return FALSE;
    }
    return FALSE;
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
