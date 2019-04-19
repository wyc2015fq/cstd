# [转载]EasyRTMP调用librtmp推送RTMP数据时CPU占用高的问题调优 - xcyl的口袋 - CSDN博客
2016年10月28日 22:04:58[xcyl](https://me.csdn.net/cai6811376)阅读数：598
本文转自EasyDarwin团队kim的博客：[http://blog.csdn.net/jinlong0603/article/details/52950948](http://blog.csdn.net/jinlong0603/article/details/52950948)
## EasyRTMP
> 
前面介绍过EasyRTMP是高效的跨平台的RTMP推送库，内部rtmp协议部分使用了librtmp（ RTMPDump Real-Time Messaging Protocol API）。
### libEasyRTMP的基本操作
> 
RTMP_Alloc() ：用于创建一个RTMP会话的句柄。 
  RTMP_Init()：初始化句柄。 
  RTMP_SetupURL()：设置会话的参数。 
  RTMP_Connect()：建立RTMP链接中的网络连接（NetConnection）。 
  RTMP_ConnectStream()：建立RTMP链接中的网络流（NetStream）。 
  RTMP_Read()：读取RTMP流的内容。 
  客户端可以在调用RTMP_Connect()之前调用RTMP_EnableWrite()，然后在会话开始之后调用 RTMP_Write()。 
  RTMP_Pause()：流播放的时候可以用于暂停和继续 
  RTMP_Seek()：改变流播放的位置 
  当RTMP_Read()返回0 字节的时候,代表流已经读取完毕，而后可以调用RTMP_Close() 
  RTMP_Free()：用于清理会话。 
  所有的数据都使用 FLV 格式进行传输。一个基本的会话需要一个RTMP URL。RTMP URL 格式如下所示： 
   rtmp[t][e|s]://hostname[:port][/app[/playpath]] 
  支持普通的，隧道的，以及加密的会话。 
  其他附加的选项可以在URL的后面添加以空格为间隔符的“key=value”形式的字符串。
### 推送视频流是CPU较高
- 
LibRTMP的结构体RTMP的成员是有m_outChunkSize，并且在RTMP_Init函数中被初始化了默认值128，然后整个LibRTMP代码没有改变m_outChunkSize的接口函数，内部也没有改变m_outChunkSize的实现逻辑，也没有发送改变块大小的消息给流媒体服务器的代码逻辑，关闭Nagle加如此小的块大小会导致很多小包，而以太网的MTU是1500，这样如果用在播放客户端由于主要是接收媒体流到也没有什么，但是如果用在发布媒体流的推流客户端网络效率就太低了，并且IP小包太多还会引起流媒体的服务器软中断升高，导致内核占用的CPU过高。
- 
调整输出块大小的函数
```
static int
ChangeChunkSize(RTMP *r,int outChunkSize)
{
RTMPPacket packet;
char pbuf[RTMP_MAX_HEADER_SIZE + 4];
packet.m_nBytesRead = 0;
packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;
packet.m_packetType = RTMP_PACKET_TYPE_CHUNK_SIZE;
packet.m_nChannel = 0x04; 
packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
packet.m_nTimeStamp = 0;
packet.m_nInfoField2 = 0;
packet.m_hasAbsTimestamp = 0;   
packet.m_nBodySize = 4;
r->m_outChunkSize = outChunkSize;
r->m_outChunkSize = htonl(r->m_outChunkSize);
memcpy(packet.m_body, &r->m_outChunkSize, 4);
r->m_outChunkSize = ntohl(r->m_outChunkSize);
return RTMP_SendPacket(r, &packet, TRUE);
}
```
- 调用调整输出块大小的函数的时机
```
if (r->Link.protocol & RTMP_FEATURE_WRITE)
{
ChangeChunkSize(r, 1360);//若不改拉流时的输出块大小在这里调用ChangeChunkSize
SendReleaseStream(r);
SendFCPublish(r);
}
else
{
RTMP_SendServerBW(r);
RTMP_SendCtrl(r, 3, 0, 300);
}
//ChangeChunkSize(r,1360);//若推、拉流时的输出块大小都改变在这里调用ChangeChunkSize
```
本文感谢：[http://www.cnblogs.com/oldmanlv/p/5487146.html](http://www.cnblogs.com/oldmanlv/p/5487146.html)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
