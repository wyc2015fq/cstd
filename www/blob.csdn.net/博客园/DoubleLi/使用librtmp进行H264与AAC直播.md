# 使用librtmp进行H264与AAC直播 - DoubleLi - 博客园







# http://my.oschina.net/jerikc/blog/501948#OSC_h3_13

# 使用librtmp进行H264与AAC直播

发表于5个月前(2015-09-06 23:19)   阅读（1276） | 评论（[0](http://my.oschina.net/jerikc/blog/501948#comments)） **11**人收藏此文章, 我要收藏

赞0





目录[-]

- [1、帧的划分](http://my.oschina.net/jerikc/blog/501948#OSC_h2_1)
- [1.1 H.264 帧](http://my.oschina.net/jerikc/blog/501948#OSC_h2_2)
- [1.2 AAC 帧](http://my.oschina.net/jerikc/blog/501948#OSC_h2_3)
- [2.视频与音频的编码信息](http://my.oschina.net/jerikc/blog/501948#OSC_h2_4)
- [2.1 (H264)SPS](http://my.oschina.net/jerikc/blog/501948#OSC_h3_5)
- [2.2 (H264)PPS](http://my.oschina.net/jerikc/blog/501948#OSC_h3_6)
- [2.3 (AAC)AudioDecoderSpecificInfo](http://my.oschina.net/jerikc/blog/501948#OSC_h3_7)
- [3.librtmp 的使用](http://my.oschina.net/jerikc/blog/501948#OSC_h2_8)
- [4.包类型](http://my.oschina.net/jerikc/blog/501948#OSC_h2_9)
- [4.1 H.264编码信息帧](http://my.oschina.net/jerikc/blog/501948#OSC_h3_10)
- [4.2 H.264关键帧](http://my.oschina.net/jerikc/blog/501948#OSC_h3_11)
- [4.3 H.264非关键帧](http://my.oschina.net/jerikc/blog/501948#OSC_h3_12)
- [4.4 AAC编码信息](http://my.oschina.net/jerikc/blog/501948#OSC_h3_13)
- [4.5 AAC普通数据](http://my.oschina.net/jerikc/blog/501948#OSC_h3_14)
- [5.参考文档](http://my.oschina.net/jerikc/blog/501948#OSC_h2_15)




libx264 版本是 128
libfaac 版本是 1.28

## 1、帧的划分

## 1.1 H.264 帧

对于 H.264 而言每帧的界定符为 00 00 00 01 或者 00 00 01。

比如下面的 h264 文件片断这就包含三帧数据：

00 00 00 01 67 42 C0 28 DA 01 E0 08 9F 96 10 00
00 03 00 10 00 00 03 01 48 F1 83 2A 00 00 00 01
68 CE 3C 80 00 00 01 06 05 FF FF 5D DC 45 E9 BD
E6 D9 48 B7 96 2C D8 20 D9 23 EE EF …

第一帧是 00 00 00 01 67 42 C0 28 DA 01 E0 08 9F 96 10 00 00 03 00 10 00 00 03 01 48 F1 83 2A
第二帧是 00 00 00 01 68 CE 3C 80
第三帧是 00 00 01 06 05 FF FF 5D DC 45 E9 BD E6 D9 48 B7 96 2C D8 20 D9 23 EE EF ..

帧类型有：
NAL_SLICE = 1
NAL_SLICE_DPA = 2
NAL_SLICE_DPB = 3
NAL_SLICE_DPC = 4
NAL_SLICE_IDR = 5
NAL_SEI = 6
NAL_SPS = 7
NAL_PPS = 8
NAL_AUD = 9
NAL_FILLER = 12,

我们发送 RTMP 数据时只需要知道四种帧类型，其它类型我都把它规类成非关键帧。分别是
NAL_SPS(7), sps 帧
NAL_PPS(8), pps 帧
NAL_SLICE_IDR(5), 关键帧
NAL_SLICE(1) 非关键帧

帧类型的方式判断为界面符后首字节的低四位。
第一帧的帧类型为: 0x67 & 0x1F = 7，这是一个 SPS 帧
第二帧的帧类型为: 0x68 & 0x1F = 8，这是一个 PPS 帧
第三帧的帧类型为: 0x06 & 0x1F = 6，这是一个 SEI 帧

以上是我们利用帧界定符划分帧，并可以判断每一个帧的类型。

注意：如果是压缩图像成 H264 帧，我们就可不必进行帧界定，因为每一次压缩的输出都明确了该帧的大小（包括界定符），每一次的压缩的结果可能包函多帧。一会具体讨论。

## 1.2 AAC 帧

对于 AAC 帧它的界定符是 FF F1

这里我就不举例了，可通过查看 AAC 的二进制文件可以看到如下的帧结构：
FF F1 50 80 24 9F FD DE 04 00 00 6C 69 62 66 61 61 63 20 31 2E 32 38 00 00 42 15 95 ..

注意：那么对于 AAC 而言加上界定符每一帧的前 7 字节是帧的描述信息，也就是说 AAC 的祼数据是除去前面的 7 个字节的，在发送 RTMP 时，我们要去掉这 7 个字节。同样，如果我们是一边压缩一边发送 RTMP，我们同样不需要界定帧，因为 libfaac 每次压缩完成的输出就是一个完整的帧数据，我们只需要将该帧打包发送即可。

综合上面的所述，如果我们只是一边压缩一边将压缩结果发送到 RTMP 服务器，那我们就可以不用对帧进行界定，如果我们是发送 H264 与 AAC 文件，那我们就要对帧进行界定。

## 2.视频与音频的编码信息

如果我们只是简答的将压缩数据打包发送给 RTMP 服务器，那么 RTMP 服务器是不可以对数据进行解码和播放的，在这之前我们要将音视频的视频的编码信息发送给 RTMP 服务器。很多人可能苦于寻找下面的三个编码参数而不得要领。其实要想得到也是很简单的。

### 2.1 (H264)SPS

对于 H264 而言，SPS 就是编码后的第一帧。如果是读取 H264 文件，就是第一个帧界定符与第二帧界定符中间的数据长度是 4。

### 2.2 (H264)PPS

对于 H264 而言，PPS 就是编码后的第二帧。如果是读取 H264 文件，就是第二个帧界定符与第三帧界定符中间的数据，长度不固定。

### 2.3 (AAC)AudioDecoderSpecificInfo

这个长度为 2 个字节，可以通过计算或者调用函数获取。建议通过调用faacEncGetDecoderSpecificInfo(fh,&spec,&len); 获取。
一般情况双声道 44100 采样下，该值是 0x1210

## 3.librtmp 的使用



[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
/*分配与初始化*/
```



```cpp
rtmp
 = RTMP_Alloc();
```



```cpp
RTMP_Init(rtmp);
```





```cpp
/*设置URL*/
```



```cpp
if
```

```cpp
(RTMP_SetupURL(rtmp,rtmp_url) == FALSE) {
```



```cpp

```

```cpp
log
```

```cpp
(LOG_ERR,
```

```cpp
"RTMP_SetupURL()
 failed!"
```

```cpp
);
```



```cpp

```

```cpp
RTMP_Free(rtmp);
```



```cpp

```

```cpp
return
```

```cpp
-1;
```



```cpp
}
```





```cpp
/*设置可写,即发布流,这个函数必须在连接前使用,否则无效*/
```



```cpp
RTMP_EnableWrite(rtmp);
```





```cpp
/*连接服务器*/
```



```cpp
if
```

```cpp
(RTMP_Connect(rtmp, NULL) == FALSE) {
```



```cpp

```

```cpp
log
```

```cpp
(LOG_ERR,
```

```cpp
"RTMP_Connect()
 failed!"
```

```cpp
);
```



```cpp

```

```cpp
RTMP_Free(rtmp);
```



```cpp

```

```cpp
return
```

```cpp
-1;
```



```cpp
}
```





```cpp
/*连接流*/
```



```cpp
if
```

```cpp
(RTMP_ConnectStream(rtmp,0) == FALSE) {
```



```cpp

```

```cpp
log
```

```cpp
(LOG_ERR,
```

```cpp
"RTMP_ConnectStream()
 failed!"
```

```cpp
);
```



```cpp

```

```cpp
RTMP_Close(rtmp);
```



```cpp

```

```cpp
RTMP_Free(rtmp);
```



```cpp

```

```cpp
return
```

```cpp
-1;
```



```cpp
}
```
```







[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
/*定义包头长度,RTMP_MAX_HEADER_SIZE为rtmp.h中定义值为18*/
```





```cpp
#define
 RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)
```





```cpp
RTMPPacket
 * packet;
```



```cpp
unsigned
```

```cpp
char
```

```cpp
* body;
```





```cpp
/*分配包内存和初始化,len为包体长度*/
```



```cpp
packet
 = (RTMPPacket *)
```

```cpp
malloc
```

```cpp
(RTMP_HEAD_SIZE+len);
```



```cpp
memset
```

```cpp
(packet,0,RTMP_HEAD_SIZE);
```





```cpp
/*包体内存*/
```



```cpp
packet->m_body
 = (
```

```cpp
char
```

```cpp
*)packet + RTMP_HEAD_SIZE;
```



```cpp
body
 = (unsigned
```

```cpp
char
```

```cpp
*)packet->m_body;
```



```cpp
packet->m_nBodySize
 = len;
```





```cpp
/*
```



```cpp

```

```cpp
*
 此处省略包体填充
```



```cpp

```

```cpp
*/
```



```cpp
packet->m_hasAbsTimestamp
 = 0;
```



```cpp
packet->m_packetType
 = RTMP_PACKET_TYPE_VIDEO;
```

```cpp
/*此处为类型有两种一种是音频,一种是视频*/
```



```cpp
packet->m_nInfoField2
 = rtmp->m_stream_id;
```



```cpp
packet->m_nChannel
 = 0x04;
```



```cpp
packet->m_headerType
 = RTMP_PACKET_SIZE_LARGE;
```



```cpp
packet->m_nTimeStamp
 = timeoffset;
```





```cpp
/*发送*/
```



```cpp
if
```

```cpp
(RTMP_IsConnected(rtmp)) {
```



```cpp

```

```cpp
ret
 = RTMP_SendPacket(rtmp,packet,TRUE);
```

```cpp
/*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
```



```cpp
}
```





```cpp
/*释放内存*/
```



```cpp
free
```

```cpp
(packet);
```
```







[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
/*关闭与释放*/
```



```cpp
RTMP_Close(rtmp);
```



```cpp
RTMP_Free(rtmp);
```
```



## 4.包类型

### 4.1 H.264编码信息帧

H.264 的编码信息帧是发送给 RTMP 服务器称为 AVC sequence header，RTMP 服务器只有收到 AVC sequence header 中的 sps, pps 才能解析后续发送的 H264 帧。



[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
int
```

```cpp
send_video_sps_pps()
```



```cpp
{
```



```cpp

```

```cpp
RTMPPacket
 * packet;
```



```cpp

```

```cpp
unsigned
```

```cpp
char
```

```cpp
* body;
```



```cpp

```

```cpp
int
```

```cpp
i;
```





```cpp

```

```cpp
packet
 = (RTMPPacket *)
```

```cpp
malloc
```

```cpp
(RTMP_HEAD_SIZE+1024);
```



```cpp

```

```cpp
memset
```

```cpp
(packet,0,RTMP_HEAD_SIZE);
```





```cpp

```

```cpp
packet->m_body
 = (
```

```cpp
char
```

```cpp
*)packet + RTMP_HEAD_SIZE;
```



```cpp

```

```cpp
body
 = (unsigned
```

```cpp
char
```

```cpp
*)packet->m_body;
```





```cpp

```

```cpp
memcpy
```

```cpp
(winsys->pps,buf,len);
```



```cpp

```

```cpp
winsys->pps_len
 = len;
```





```cpp

```

```cpp
i
 = 0;
```



```cpp

```

```cpp
body[i++]
 = 0x17;
```



```cpp

```

```cpp
body[i++]
 = 0x00;
```





```cpp

```

```cpp
body[i++]
 = 0x00;
```



```cpp

```

```cpp
body[i++]
 = 0x00;
```



```cpp

```

```cpp
body[i++]
 = 0x00;
```





```cpp

```

```cpp
/*AVCDecoderConfigurationRecord*/
```



```cpp

```

```cpp
body[i++]
 = 0x01;
```



```cpp

```

```cpp
body[i++]
 = sps[1];
```



```cpp

```

```cpp
body[i++]
 = sps[2];
```



```cpp

```

```cpp
body[i++]
 = sps[3];
```



```cpp

```

```cpp
body[i++]
 = 0xff;
```





```cpp

```

```cpp
/*sps*/
```



```cpp

```

```cpp
body[i++]  
 = 0xe1;
```



```cpp

```

```cpp
body[i++]
 = (sps_len >> 8) & 0xff;
```



```cpp

```

```cpp
body[i++]
 = sps_len & 0xff;
```



```cpp

```

```cpp
memcpy
```

```cpp
(&body[i],sps,sps_len);
```



```cpp

```

```cpp
i
 +=  sps_len;
```





```cpp

```

```cpp
/*pps*/
```



```cpp

```

```cpp
body[i++]  
 = 0x01;
```



```cpp

```

```cpp
body[i++]
 = (pps_len >> 8) & 0xff;
```



```cpp

```

```cpp
body[i++]
 = (pps_len) & 0xff;
```



```cpp

```

```cpp
memcpy
```

```cpp
(&body[i],pps,pps_len);
```



```cpp

```

```cpp
i
 +=  pps_len;
```





```cpp

```

```cpp
packet->m_packetType
 = RTMP_PACKET_TYPE_VIDEO;
```



```cpp

```

```cpp
packet->m_nBodySize
 = i;
```



```cpp

```

```cpp
packet->m_nChannel
 = 0x04;
```



```cpp

```

```cpp
packet->m_nTimeStamp
 = 0;
```



```cpp

```

```cpp
packet->m_hasAbsTimestamp
 = 0;
```



```cpp

```

```cpp
packet->m_headerType
 = RTMP_PACKET_SIZE_MEDIUM;
```



```cpp

```

```cpp
packet->m_nInfoField2
 = rtmp->m_stream_id;
```





```cpp

```

```cpp
/*调用发送接口*/
```



```cpp

```

```cpp
RTMP_SendPacket(rtmp,packet,TRUE);
```



```cpp

```

```cpp
free
```

```cpp
(packet);
```





```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp
}
```
```



sps 与 pps 怎么获取到呢？

在前面已经说过，H264 的第 1 帧是 sps 帧, pps 是第 2 帧。

我们在编码时会调用如下接口





[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
size
 = x264_encoder_encode(cx->hd,&nal,&n,pic,&pout);
```





```cpp
int
```

```cpp
i,last;
```



```cpp
for
```

```cpp
(i = 0,last = 0;i < n;i++) {
```



```cpp

```

```cpp
if
```

```cpp
(nal[i].i_type == NAL_SPS) {
```



```cpp

```

```cpp
sps_len
 = nal[i].i_payload-4;
```



```cpp

```

```cpp
memcpy
```

```cpp
(sps,nal[i].p_payload+4,sps_len);
```



```cpp

```

```cpp
}
```

```cpp
else
```

```cpp
if
```

```cpp
(nal[i].i_type == NAL_PPS) {
```



```cpp

```

```cpp
pps_len
 = nal[i].i_payload-4;
```



```cpp

```

```cpp
memcpy
```

```cpp
(pps,nal[i].p_payload+4,pps_len);
```





```cpp

```

```cpp
/*发送sps
 pps*/
```



```cpp

```

```cpp
send_video_sps_pps();
```





```cpp

```

```cpp
}
```

```cpp
else
```

```cpp
{
```





```cpp

```

```cpp
/*发送普通帧*/
```



```cpp

```

```cpp
send_rtmp_video(nal[i].p_payload,nal[i].i_payload);
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
last
 += nal[i].i_payload;
```



```cpp
}
```
```



我完全可以不用知道 sps, pps 的具体意义:)

### 4.2 H.264关键帧



### 4.3 H.264非关键帧





[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
int
```

```cpp
send_rtmp_video(unsigned
```

```cpp
char
```

```cpp
* buf,
```

```cpp
int
```

```cpp
len)
```



```cpp
{
```



```cpp

```

```cpp
int
```

```cpp
type;
```



```cpp

```

```cpp
long
```

```cpp
timeoffset;
```



```cpp

```

```cpp
RTMPPacket
 * packet;
```



```cpp

```

```cpp
unsigned
```

```cpp
char
```

```cpp
* body;
```





```cpp

```

```cpp
timeoffset
 = GetTickCount() - start_time;
```

```cpp
/*start_time为开始直播时的时间戳*/
```





```cpp

```

```cpp
/*去掉帧界定符*/
```



```cpp

```

```cpp
if
```

```cpp
(buf[2] == 0x00) {
```

```cpp
/*00
 00 00 01*/
```



```cpp

```

```cpp
buf
 += 4;
```



```cpp

```

```cpp
len
 -= 4;
```



```cpp

```

```cpp
}
```

```cpp
else
```

```cpp
if
```

```cpp
(buf[2] == 0x01){
```

```cpp
/*00
 00 01*/
```



```cpp

```

```cpp
buf
 += 3;
```



```cpp

```

```cpp
len
 -= 3;
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
type
 = buf[0]&0x1f;
```





```cpp

```

```cpp
packet
 = (RTMPPacket *)base_malloc(RTMP_HEAD_SIZE+len+9);
```



```cpp

```

```cpp
memset
```

```cpp
(packet,0,RTMP_HEAD_SIZE);
```





```cpp

```

```cpp
packet->m_body
 = (
```

```cpp
char
```

```cpp
*)packet + RTMP_HEAD_SIZE;
```



```cpp

```

```cpp
packet->m_nBodySize
 = len + 9;
```





```cpp

```

```cpp
/*send
 video packet*/
```



```cpp

```

```cpp
body
 = (unsigned
```

```cpp
char
```

```cpp
*)packet->m_body;
```



```cpp

```

```cpp
memset
```

```cpp
(body,0,len+9);
```





```cpp

```

```cpp
/*key
 frame*/
```



```cpp

```

```cpp
body[0]
 = 0x27;
```



```cpp

```

```cpp
if
```

```cpp
(type == NAL_SLICE_IDR) {
```



```cpp

```

```cpp
body[0]
 = 0x17;
```



```cpp

```

```cpp
}
```





```cpp

```

```cpp
body[1]
 = 0x01;
```

```cpp
/*nal
 unit*/
```



```cpp

```

```cpp
body[2]
 = 0x00;
```



```cpp

```

```cpp
body[3]
 = 0x00;
```



```cpp

```

```cpp
body[4]
 = 0x00;
```





```cpp

```

```cpp
body[5]
 = (len >> 24) & 0xff;
```



```cpp

```

```cpp
body[6]
 = (len >> 16) & 0xff;
```



```cpp

```

```cpp
body[7]
 = (len >>  8) & 0xff;
```



```cpp

```

```cpp
body[8]
 = (len ) & 0xff;
```





```cpp

```

```cpp
/*copy
 data*/
```



```cpp

```

```cpp
memcpy
```

```cpp
(&body[9],buf,len);
```





```cpp

```

```cpp
packet->m_hasAbsTimestamp
 = 0;
```



```cpp

```

```cpp
packet->m_packetType
 = RTMP_PACKET_TYPE_VIDEO;
```



```cpp

```

```cpp
packet->m_nInfoField2
 = winsys->rtmp->m_stream_id;
```



```cpp

```

```cpp
packet->m_nChannel
 = 0x04;
```



```cpp

```

```cpp
packet->m_headerType
 = RTMP_PACKET_SIZE_LARGE;
```



```cpp

```

```cpp
packet->m_nTimeStamp
 = timeoffset;
```





```cpp

```

```cpp
/*调用发送接口*/
```



```cpp

```

```cpp
RTMP_SendPacket(rtmp,packet,TRUE);
```



```cpp

```

```cpp
free
```

```cpp
(packet);
```



```cpp
}
```
```



这里要说明一下：
比如说 x264_encoder_encode 输出了 6 帧。
分别是 sps 帧, pps 帧，关键帧，非关键帧，非关键帧，非关键帧。
发送结果应该是, sps,pps 合成为一帧调用发送函数，剩下 4 帧，除去每个 nal 的界定符，分别发送每一个 nal。

在 libx264 中每一次调用 x264_encoder_encode 输出了 n 个帧，我们要从这 n 个帧里找出 sps 和 pps，剩下的分次全部发送 nal，sps 与 pps 的帧界定符都是 00 00 00 01，而普通帧可能是 00 00 00 01 也有可能 00 00 01。

如果 x264_encoder_encode 里没有 sps 帧与 pps 帧，则结果除去第一帧的界定符所以帧做为一个整体调用发送函数，它们的类型是由第一帧类型决定。

另外，H264 的流的第 1 帧一定是 sps 帧(包含帧界定符为 8 个字节),第 2 帧一定是 pps帧。

### 4.4 AAC编码信息



[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
int
```

```cpp
cap_rtmp_sendaac_spec(unsigned
```

```cpp
char
```

```cpp
*spec_buf,
```

```cpp
int
```

```cpp
spec_len)
```



```cpp
{
```



```cpp

```

```cpp
RTMPPacket
 * packet;
```



```cpp

```

```cpp
unsigned
```

```cpp
char
```

```cpp
* body;
```



```cpp

```

```cpp
int
```

```cpp
len;
```





```cpp

```

```cpp
len
 = spec_len;
```

```cpp
/*spec
 data长度,一般是2*/
```





```cpp

```

```cpp
packet
 = (RTMPPacket *)base_malloc(RTMP_HEAD_SIZE+len+2);
```



```cpp

```

```cpp
memset
```

```cpp
(packet,0,RTMP_HEAD_SIZE);
```





```cpp

```

```cpp
packet->m_body
 = (
```

```cpp
char
```

```cpp
*)packet + RTMP_HEAD_SIZE;
```



```cpp

```

```cpp
body
 = (unsigned
```

```cpp
char
```

```cpp
*)packet->m_body;
```





```cpp

```

```cpp
/*AF
 00 + AAC RAW data*/
```



```cpp

```

```cpp
body[0]
 = 0xAF;
```



```cpp

```

```cpp
body[1]
 = 0x00;
```



```cpp

```

```cpp
memcpy
```

```cpp
(&body[2],spec_buf,len);
```

```cpp
/*spec_buf是AAC
 sequence header数据*/
```





```cpp

```

```cpp
packet->m_packetType
 = RTMP_PACKET_TYPE_AUDIO;
```



```cpp

```

```cpp
packet->m_nBodySize
 = len+2;
```



```cpp

```

```cpp
packet->m_nChannel
 = 0x04;
```



```cpp

```

```cpp
packet->m_nTimeStamp
 = 0;
```



```cpp

```

```cpp
packet->m_hasAbsTimestamp
 = 0;
```



```cpp

```

```cpp
packet->m_headerType
 = RTMP_PACKET_SIZE_LARGE;
```



```cpp

```

```cpp
packet->m_nInfoField2
 = rtmp->m_stream_id;
```





```cpp

```

```cpp
/*调用发送接口*/
```



```cpp

```

```cpp
RTMP_SendPacket(rtmp,packet,TRUE);
```





```cpp

```

```cpp
return
```

```cpp
TRUE;
```



```cpp
}
```
```



对于音频解码参数 AAC sequence header 是通过

下面是获得 AAC sequence header 的方法





[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
char
```

```cpp
*buf;
```



```cpp
int
```

```cpp
len;
```



```cpp
faacEncGetDecoderSpecificInfo(fh,&buf,&len);
```





```cpp
memcpy
```

```cpp
(spec_buf,buf,len);
```



```cpp
spec_len
 = len;
```





```cpp
/*释放系统内存*/
```



```cpp
free
```

```cpp
(buf);
```
```





另外如果你是打开 aac 文件进行发送，那么你可以尝试自己计算这个值，其实也很简单，打开faac 源代码看一下 faacEncGetDecoderSpecificInfo 的实现，也就是几个移位的事:)。

对于一般情况 44100Hz 双声道，这个值是 0x1210，偷懒就是直接用这个值吧。

### 4.5 AAC普通数据

如前面所述，发送 AAC 的普通数据要改造一下，因为 AAC 的前 7 个字节（包括帧界定符）对于 RTMP 服务器来说是无用的。





[?](http://my.oschina.net/jerikc/blog/501948#)

```
```cpp
void
```

```cpp
* cap_dialog_send_audio(unsigned
```

```cpp
char
```

```cpp
* buf,
```

```cpp
int
```

```cpp
len)
```



```cpp
{
```



```cpp

```

```cpp
long
```

```cpp
timeoffset;
```



```cpp

```

```cpp
timeoffset
 = GetTickCount() - start_time;
```





```cpp

```

```cpp
buf
 += 7;
```



```cpp

```

```cpp
len
 += 7;
```





```cpp

```

```cpp
if
```

```cpp
(len > 0) {
```



```cpp

```

```cpp
RTMPPacket
 * packet;
```



```cpp

```

```cpp
unsigned
```

```cpp
char
```

```cpp
* body;
```





```cpp

```

```cpp
packet
 = (RTMPPacket *)
```

```cpp
malloc
```

```cpp
(RTMP_HEAD_SIZE+len+2);
```



```cpp

```

```cpp
memset
```

```cpp
(packet,0,RTMP_HEAD_SIZE);
```





```cpp

```

```cpp
packet->m_body
 = (
```

```cpp
char
```

```cpp
*)packet + RTMP_HEAD_SIZE;
```



```cpp

```

```cpp
body
 = (unsigned
```

```cpp
char
```

```cpp
*)packet->m_body;
```





```cpp

```

```cpp
/*AF
 01 + AAC RAW data*/
```



```cpp

```

```cpp
body[0]
 = 0xAF;
```



```cpp

```

```cpp
body[1]
 = 0x01;
```



```cpp

```

```cpp
memcpy
```

```cpp
(&body[2],buf,len);
```





```cpp

```

```cpp
packet->m_packetType
 = RTMP_PACKET_TYPE_AUDIO;
```



```cpp

```

```cpp
packet->m_nBodySize
 = len+2;
```



```cpp

```

```cpp
packet->m_nChannel
 = 0x04;
```



```cpp

```

```cpp
packet->m_nTimeStamp
 = timeoffset;
```



```cpp

```

```cpp
packet->m_hasAbsTimestamp
 = 0;
```



```cpp

```

```cpp
packet->m_headerType
 = RTMP_PACKET_SIZE_MEDIUM;
```



```cpp

```

```cpp
packet->m_nInfoField2
 = rtmp->m_stream_id;
```





```cpp

```

```cpp
/*调用发送接口*/
```



```cpp

```

```cpp
RTMP_SendPacket(rtmp,packet,TRUE);
```



```cpp

```

```cpp
free
```

```cpp
(packet);
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp
}
```
```



至此所有流程均结束了。

要注意的几件事：
libRTMP 多线程发送有时候可能会出现问题，不知道是什么问题，最好改成队列发送。将填充好的 packet 通过消息或者其它方式发送给其它线程，发送线程统一发送即可。

## 5.参考文档
- 《Video File Format Specification Version 10》 http://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf
- 《H264视频通过RTMP直播》 http://blog.csdn.net/firehood_/article/details/8783589
- rtmpdump-2.3










