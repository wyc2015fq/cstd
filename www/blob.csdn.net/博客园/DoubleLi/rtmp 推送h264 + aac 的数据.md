# rtmp 推送h264 + aac 的数据 - DoubleLi - 博客园






相关源码下载：



http://download.csdn.net/detail/keepingstudying/8340431



需要libfaac，librtmp 的支持，

### 1、帧的划分

#### 1.1 H.264帧

对于H.264而言每帧的界定符为00 00 00 01或者00 00 01。

比如下面的h264文件片断这就包函三帧数据

00 00 00 01 67 42 C0 28 DA 01 E0 08 9F 96 10 00
00 03 00 10 00 00 03 01 48 F1 83 2A 00 00 00 01
68 CE 3C 80 00 00 01 06 05 FF FF 5D DC 45 E9 BD
E6 D9 48 B7 96 2C D8 20 D9 23 EE EF …

第一帧是00 00 00 01 67 42 C0 28 DA 01 E0 08 9F 96 10 00 00 03 00 10 00 00 03 01 48 F1 83 2A
第二帧是00 00 00 01 68 CE 3C 80
第三帧是00 00 01 06 05 FF FF 5D DC 45 E9 BD E6 D9 48 B7 96 2C D8 20 D9 23 EE EF ..

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

我们发送RTMP数据时只需要知道四种帧类型，其它类型我都把它规类成非关键帧。
分别是
NAL_SPS(7), sps帧
NAL_PPS(8), pps帧
NAL_SLICE_IDR(5), 关键帧
NAL_SLICE(1) 非关键帧

帧类型的方式判断为界面符后首字节的低四位。
第一帧的帧类型为: 0x67 & 0x1F = 7，这是一个SPS帧
第二帧的帧类型为: 0x68 & 0x1F = 8，这是一个PPS帧
第三帧的帧类型为: 0x06 & 0x1F = 6，这是一个SEI帧

以上是我们利用帧界定符划分帧，并可以判断每一个帧的类型。

注意：如果是压缩图像成H264帧，我们就可不必进行帧界定，因为每一次压缩的输出都明确了该帧的大小（包括界定符），每一次的压缩的结果可能包函多帧。一会具体讨论。

#### 1.2 AAC帧

对于AAC帧它的界定符是FF F1

这里我就不举例了，可通过查看AAC的二进制文件可以看到如下的帧结构。
FF F1 50 80 24 9F FD DE 04 00 00 6C 69 62 66 61 61 63 20 31 2E 32 38 00 00 42 15 95 ..

注意：那么对于AAC而言加上界定符每一帧的前7字节是帧的描述信息，也就是说AAC的祼数据是除去前面的7个字节的，在发送RTMP时，我们要去掉这7个字节。同样，如果我们是一边压缩一边发送RTMP，我们同样不需要界定帧，因为libfaac每次压缩完成的输出就是一个完整的帧数据，我们只需要将该帧打包发送即可。

综合上面的所述，如果我们只是一边压缩一边将压缩结果发送到RTMP服务器，那我们就可以不用对帧进行界定，如果我们是发送H264与AAC文件，那我们就要对帧进行界定。

### 2.视频与音频的编码信息

如果我们只是简答的将压缩数据打包发送给RTMP服务器，那么RTMP服务器是不可以对数据进行解码和播放的，在这之前我们要将音视频的视频的编码信息发送给RTMP服务器。很多人可能苦于寻找下面的三个编码参数而不得要领。其实要想得到也是很简单的。

#### 2.1 (H264)SPS

对于H264而言，SPS就是编码后的第一帧。如果是读取H264文件，就是第一个帧界定符与第二帧界定符中间的数据长度是4。

#### 2.2 (H264)PPS

对于H264而言，PPS就是编码后的第二帧。如果是读取H264文件，就是第二个帧界定符与第三帧界定符中间的数据，长度不固定。

#### 2.3 (AAC)AudioDecoderSpecificInfo

这个长度为2个字节，可以通过计算或者调用函数获取。
建议通过调用faacEncGetDecoderSpecificInfo(fh,&spec,&len);获取。
一般情况双声道44100采样下，该值是0x1210

### 3.librtmp的使用















C




|12345678910111213141516171819202122232425262728|/*分配与初始化*/rtmp=RTMP_Alloc();RTMP_Init(rtmp);/*设置URL*/if(RTMP_SetupURL(rtmp,rtmp_url)==FALSE){log(LOG_ERR,"RTMP_SetupURL() failed!");RTMP_Free(rtmp);return-1;}/*设置可写,即发布流,这个函数必须在连接前使用,否则无效*/RTMP_EnableWrite(rtmp);/*连接服务器*/if(RTMP_Connect(rtmp,NULL)==FALSE){log(LOG_ERR,"RTMP_Connect() failed!");RTMP_Free(rtmp);return-1;}/*连接流*/if(RTMP_ConnectStream(rtmp,0)==FALSE){log(LOG_ERR,"RTMP_ConnectStream() failed!");RTMP_Close(rtmp);RTMP_Free(rtmp);return-1;}|
|----|----|

















C




|123456789101112131415161718192021222324252627282930313233|/*定义包头长度,RTMP_MAX_HEADER_SIZE为rtmp.h中定义值为18*/#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)RTMPPacket*packet;unsignedchar*body;/*分配包内存和初始化,len为包体长度*/packet=(RTMPPacket*)malloc(RTMP_HEAD_SIZE+len);memset(packet,0,RTMP_HEAD_SIZE);/*包体内存*/packet->m_body=(char*)packet+RTMP_HEAD_SIZE;body=(unsignedchar*)packet->m_body;packet->m_nBodySize=len;/** 此处省略包体填充*/packet->m_hasAbsTimestamp=0;packet->m_packetType=RTMP_PACKET_TYPE_VIDEO;/*此处为类型有两种一种是音频,一种是视频*/packet->m_nInfoField2=rtmp->m_stream_id;packet->m_nChannel=0x04;packet->m_headerType=RTMP_PACKET_SIZE_LARGE;packet->m_nTimeStamp=timeoffset;/*发送*/if(RTMP_IsConnected(rtmp)){ret=RTMP_SendPacket(rtmp,packet,TRUE);/*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/}/*释放内存*/free(packet);|
|----|----|

















C




|123|/*关闭与释放*/RTMP_Close(rtmp);RTMP_Free(rtmp);|
|----|----|





### 4.包类型

#### 4.1 H.264编码信息帧

H.264的编码信息帧是发送给RTMP服务器称为AVC sequence header，RTMP服务器只有收到AVC sequence header中的sps,pps才能解析后续发送的H264帧。













C




|12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758|intsend_video_sps_pps(){RTMPPacket*packet;unsignedchar*body;inti;packet=(RTMPPacket*)malloc(RTMP_HEAD_SIZE+1024);memset(packet,0,RTMP_HEAD_SIZE);packet->m_body=(char*)packet+RTMP_HEAD_SIZE;body=(unsignedchar*)packet->m_body;memcpy(winsys->pps,buf,len);winsys->pps_len=len;i=0;body[i++]=0x17;body[i++]=0x00;body[i++]=0x00;body[i++]=0x00;body[i++]=0x00;/*AVCDecoderConfigurationRecord*/body[i++]=0x01;body[i++]=sps[1];body[i++]=sps[2];body[i++]=sps[3];body[i++]=0xff;/*sps*/body[i++]=0xe1;body[i++]=(sps_len>>8)&0xff;body[i++]=sps_len&0xff;memcpy(&body[i],sps,sps_len);i+=sps_len;/*pps*/body[i++]=0x01;body[i++]=(pps_len>>8)&0xff;body[i++]=(pps_len)&0xff;memcpy(&body[i],pps,pps_len);i+=pps_len;packet->m_packetType=RTMP_PACKET_TYPE_VIDEO;packet->m_nBodySize=i;packet->m_nChannel=0x04;packet->m_nTimeStamp=0;packet->m_hasAbsTimestamp=0;packet->m_headerType=RTMP_PACKET_SIZE_MEDIUM;packet->m_nInfoField2=rtmp->m_stream_id;/*调用发送接口*/RTMP_SendPacket(rtmp,packet,TRUE);free(packet);return0;}|
|----|----|



sps与pps怎么获取到呢？

在前面已经说过，H264的第1帧是sps帧,pps是第2帧。

我们在编码时会调用如下接口













C




|123456789101112131415161718192021222324|size=x264_encoder_encode(cx->hd,&nal,&n,pic,&pout);inti,last;for(i=0,last=0;i<n;i++){if(nal[i].i_type==NAL_SPS){sps_len=nal[i].i_payload-4;memcpy(sps,nal[i].p_payload+4,sps_len);}elseif(nal[i].i_type==NAL_PPS){pps_len=nal[i].i_payload-4;memcpy(pps,nal[i].p_payload+4,pps_len);/*发送sps pps*/send_video_sps_pps();}else{/*发送普通帧*/send_rtmp_video(nal[i].p_payload,nal[i].i_payload);}last+=nal[i].i_payload;}|
|----|----|



我完全可以不用知道sps,pps的具体意义:)

#### 4.2 H.264关键帧

#### 4.3 H.264非关键帧















C




|1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859|intsend_rtmp_video(unsignedchar*buf,intlen){inttype;longtimeoffset;RTMPPacket*packet;unsignedchar*body;timeoffset=GetTickCount()-start_time;/*start_time为开始直播时的时间戳*//*去掉帧界定符*/if(buf[2]==0x00){/*00 00 00 01*/buf+=4;len-=4;}elseif(buf[2]==0x01){/*00 00 01*/buf+=3;len-=3;}type=buf[0]&0x1f;packet=(RTMPPacket*)base_malloc(RTMP_HEAD_SIZE+len+9);memset(packet,0,RTMP_HEAD_SIZE);packet->m_body=(char*)packet+RTMP_HEAD_SIZE;packet->m_nBodySize=len+9;/*send video packet*/body=(unsignedchar*)packet->m_body;memset(body,0,len+9);/*key frame*/body[0]=0x27;if(type==NAL_SLICE_IDR){body[0]=0x17;}body[1]=0x01;/*nal unit*/body[2]=0x00;body[3]=0x00;body[4]=0x00;body[5]=(len>>24)&0xff;body[6]=(len>>16)&0xff;body[7]=(len>>8)&0xff;body[8]=(len)&0xff;/*copy data*/memcpy(&body[9],buf,len);packet->m_hasAbsTimestamp=0;packet->m_packetType=RTMP_PACKET_TYPE_VIDEO;packet->m_nInfoField2=winsys->rtmp->m_stream_id;packet->m_nChannel=0x04;packet->m_headerType=RTMP_PACKET_SIZE_LARGE;packet->m_nTimeStamp=timeoffset;/*调用发送接口*/RTMP_SendPacket(rtmp,packet,TRUE);free(packet);}|
|----|----|



这里要说明一下：

在libx264中每一次调用x264_encoder_encode输出了n个帧，我们要从这n个帧里找出sps和pps，剩下的分次全部发送nal，sps与pps的帧界定符都是00 00 00 01，而普通帧可能是00 00 00 01也有可能00 00 01。
比如说x264_encoder_encode输出了6帧。
分别是 sps帧,pps帧，关键帧，非关键帧，非关键帧，非关键帧。
发送结果应该是,sps,pps合成为一帧调用发送函数，剩下4帧，除去每个nal的界定符，分别发送每一个nal。

如果x264_encoder_encode里没有sps帧与pps帧，则结果除去第一帧的界定符所以帧做为一个整体调用发送函数，它们的类型是由第一帧类型决定。

另外，H264的流的第1帧一定是sps帧(包含帧界定符为8个字节),第2帧一定是pps帧。

#### 4.4 AAC编码信息















C




|1234567891011121314151617181920212223242526272829303132|intcap_rtmp_sendaac_spec(unsignedchar*spec_buf,intspec_len){RTMPPacket*packet;unsignedchar*body;intlen;len=spec_len;/*spec data长度,一般是2*/packet=(RTMPPacket*)base_malloc(RTMP_HEAD_SIZE+len+2);memset(packet,0,RTMP_HEAD_SIZE);packet->m_body=(char*)packet+RTMP_HEAD_SIZE;body=(unsignedchar*)packet->m_body;/*AF 00 + AAC RAW data*/body[0]=0xAF;body[1]=0x00;memcpy(&body[2],spec_buf,len);/*spec_buf是AAC sequence header数据*/packet->m_packetType=RTMP_PACKET_TYPE_AUDIO;packet->m_nBodySize=len+2;packet->m_nChannel=0x04;packet->m_nTimeStamp=0;packet->m_hasAbsTimestamp=0;packet->m_headerType=RTMP_PACKET_SIZE_LARGE;packet->m_nInfoField2=rtmp->m_stream_id;/*调用发送接口*/RTMP_SendPacket(rtmp,packet,TRUE);returnTRUE;}|
|----|----|



对于音频解码参数AAC sequence header是通过

下面是获得AAC sequence header的方法













C




|123456789|char*buf;intlen;faacEncGetDecoderSpecificInfo(fh,&buf,&len);memcpy(spec_buf,buf,len);spec_len=len;/*释放系统内存*/free(buf);|
|----|----|



另外如果你是打开aac文件进行发送，那么你可以尝试自己计算这个值，其实也很简单，打开faac源代码看一下faacEncGetDecoderSpecificInfo的实现，也就是几个移位的事:)。
对于一般情况44100Hz双声道，这个值是0x1210，偷懒就是直接用这个值吧。

#### 4.5 AAC普通数据

如前面所述，发送AAC的普通数据要改造一下，因为AAC的前7个字节（包括帧界定符）对于RTMP服务器来说是无用的。













C




|12345678910111213141516171819202122232425262728293031323334353637|void*cap_dialog_send_audio(unsignedchar*buf,intlen){longtimeoffset;timeoffset=GetTickCount()-start_time;buf+=7;len+=7;if(len>0){RTMPPacket*packet;unsignedchar*body;packet=(RTMPPacket*)malloc(RTMP_HEAD_SIZE+len+2);memset(packet,0,RTMP_HEAD_SIZE);packet->m_body=(char*)packet+RTMP_HEAD_SIZE;body=(unsignedchar*)packet->m_body;/*AF 01 + AAC RAW data*/body[0]=0xAF;body[1]=0x01;memcpy(&body[2],buf,len);packet->m_packetType=RTMP_PACKET_TYPE_AUDIO;packet->m_nBodySize=len+2;packet->m_nChannel=0x04;packet->m_nTimeStamp=timeoffset;packet->m_hasAbsTimestamp=0;packet->m_headerType=RTMP_PACKET_SIZE_MEDIUM;packet->m_nInfoField2=rtmp->m_stream_id;/*调用发送接口*/RTMP_SendPacket(rtmp,packet,TRUE);free(packet);}return0;}|
|----|----|



至此所有流程均结束了。
要注意的几件事：
libRTMP多线程发送有时候可能会出现问题，不知道是什么问题，最好改成队列发送。
将填充好的packet通过消息或者其它方式发送给其它线程，发送线程统一发送即可

以下信息作为aac 编码的参考内容；

AAC的帧头一般7个字节，或者包含CRC校验的话9个字节，这里面包括了声音的相关参数。

结构如下：

Structure

AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP (QQQQQQQQ QQQQQQQQ)

Header consists of 7 or 9 bytes (without or with CRC).

![RTMP直播到FMS中的AAC音频头 AAC Frame Header (转) - niulei20012001 - niulei20012001的博客](http://img4.ph.126.net/X3bFbVB4PQ6xJ5feZb1fQg==/6597300664098196674.jpg)






|Letter|Length (bits)|Description|
|----|----|----|
|A|12|syncword 0xFFF, all bits must be 1|
|B|1|MPEG Version: 0 for MPEG-4, 1 for MPEG-2|
|C|2|Layer: always 0|
|D|1|protection absent, Warning, set to 1 if there is no CRC and 0 if there is CRC|
|E|2|profile, the [MPEG-4 Audio Object Type](http://sun3eyes.blog.163.com/index.php?title=MPEG-4_Audio#Audio_Object_Types) minus 1|
|F|4|[MPEG-4 Sampling Frequency Index](http://sun3eyes.blog.163.com/index.php?title=MPEG-4_Audio#Sampling_Frequencies) (15 is forbidden)|
|G|1|private stream, set to 0 when encoding, ignore when decoding|
|H|3|[MPEG-4 Channel Configuration](http://sun3eyes.blog.163.com/index.php?title=MPEG-4_Audio#Channel_Configurations) (in the case of 0, the channel configuration is sent via an inband PCE)|
|I|1|originality, set to 0 when encoding, ignore when decoding|
|J|1|home, set to 0 when encoding, ignore when decoding|
|K|1|copyrighted stream, set to 0 when encoding, ignore when decoding|
|L|1|copyright start, set to 0 when encoding, ignore when decoding|
|M|13|frame length, this value must include 7 or 9 bytes of header length: FrameLength = (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)|
|O|11|Buffer fullness|
|P|2|Number of AAC frames (RDBs) in ADTS frame minus 1, for maximum compatibility always use 1 AAC frame per ADTS frame|
|Q|16|CRC if protection absent is 0|



[http://wiki.multimedia.cx/index.php?title=ADTS](http://wiki.multimedia.cx/index.php?title=ADTS)

其中最重要的就是E，F，H。

E就是类型了

1: [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) Main
2: [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) LC (Low Complexity)
3: [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) SSR (Scalable Sample Rate)
4: [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) LTP (Long Term Prediction)

F就是采样频率

0: 96000 Hz
- 1: 88200 Hz
- 2: 64000 Hz
- 3: 48000 Hz
- 4: 44100 Hz
- 5: 32000 Hz
- 6: 24000 Hz
- 7: 22050 Hz
- 8: 16000 Hz
- 9: 12000 Hz
- 10: 11025 Hz
- 11: 8000 Hz
- 12: 7350 Hz

H就是声道

1: 1 channel: front-center

- 2: 2 channels: front-left, front-right
- 3: 3 channels: front-center, front-left, front-right
- 4: 4 channels: front-center, front-left, front-right, back-center
- 5: 5 channels: front-center, front-left, front-right, back-left, back-right
- 6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
- 7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel

有了这三个参数，就可以发送音频的第一帧了，然后后面的帧，把包头的7个字节去掉？，打包到RTMP协议发送就可以了。

[http://wiki.multimedia.cx/index.php?title=MPEG-4_Audio#Audio_Object_Types](http://wiki.multimedia.cx/index.php?title=MPEG-4_Audio#Audio_Object_Types)

当然发送的时候要打上RTMP的包头，数据部分用 AF 00 代替AAC的包头。长度再计算一下。时间戳用采样的时间也可以，自己另算也可以。


//--------------------------------------------------------------------------------------------------------------//

第一个音频包那就是AAC header.

如：af 00 13 90。包长4个字节，解释如下，

1）第一个字节af，a就是10代表的意思是AAC，

Format of SoundData. The following values are defined:
0 = Linear PCM, platform endian
1 = ADPCM
2 = MP3
3 = Linear PCM, little endian
4 = Nellymoser 16 kHz mono
5 = Nellymoser 8 kHz mono
6 = Nellymoser
7 = G.711 A-law logarithmic PCM
8 = G.711 mu-law logarithmic PCM
9 = reserved
10 = AAC
11 = Speex
14 = MP3 8 kHz
15 = Device-specific sound
Formats 7, 8, 14, and 15 are reserved.
AAC is supported in Flash Player 9,0,115,0 and higher.
Speex is supported in Flash Player 10 and higher.

2）第一个字节中的后四位f代表如下

前2个bit的含义 抽样频率，这里是二进制11，代表44kHZ

Sampling rate. The following values are defined:
0 = 5.5 kHz
1 = 11 kHz
2 = 22 kHz
3 = 44 kHz

第3个bit，代表 音频用16位的

Size of each audio sample. This parameter only pertains to
uncompressed formats. Compressed formats always decode
to 16 bits internally.
0 = 8-bit samples
1 = 16-bit samples

第4个bit代表声道

Mono or stereo sound
0 = Mono sound
1 = Stereo sound

3）第2个字节

AACPacketType，这个字段来表示AACAUDIODATA的类型：0 = AAC sequence header，1 = AAC raw。第一个音频包用0，后面的都用1

4）第3，4个字节内容AudioSpecificConfig如下

AAC sequence header存放的是AudioSpecificConfig结构，该结构则在“ISO-14496-3 Audio”中描述。AudioSpecificConfig结构的描述非常复杂，这里我做一下简化，事先设定要将要编码的音频格式，其中，选择"AAC-LC"为音频编码，音频采样率为44100，于是AudioSpecificConfig简化为下表：

![](https://pic002.cnblogs.com/images/2011/254714/2011122913505721.jpg)

0x13 0x90（1001110010000） 表示 ObjectProfile=2， AAC-LC，SamplingFrequencyIndex=7，ChannelConfiguration=声道2

- 
![RTMP直播到FMS中的AAC音频头 AAC Frame Header (转) - niulei20012001 - niulei20012001的博客](http://img8.ph.126.net/iyw4tESg70eWhS4D57959g==/6597527163494022238.jpg)



AudioSpecificConfig，即为ObjectProfile，SamplingFrequencyIndex，ChannelConfiguration，TFSpecificConfig。

其中，ObjectProfile (AAC main ~1, AAC lc ~2, AAC ssr ~3)；

SamplingFrequencyIndex (0 ~ 96000， 1~88200， 2~64000， 3~48000， 4~44100， 5~32000， 6~24000， 7~ 22050， 8~16000...)，通常aac固定选中44100，即应该对应为4，但是试验结果表明，当音频采样率小于等于44100时，应该选择3，而当音频采样率为48000时，应该选择2；

ChannelConfiguration对应的是音频的频道数目。单声道对应1，双声道对应2，依次类推。

TFSpecificConfig的说明见标准14496-3中（1.2 T/F Audio Specific Configuration）的讲解，这里恒定的设置为0；

索引值如下含义：

There are 13 supported frequencies:
- 0: 96000 Hz
- 1: 88200 Hz
- 2: 64000 Hz
- 3: 48000 Hz
- 4: 44100 Hz
- 5: 32000 Hz
- 6: 24000 Hz
- 7: 22050 Hz
- 8: 16000 Hz
- 9: 12000 Hz
- 10: 11025 Hz
- 11: 8000 Hz
- 12: 7350 Hz
- 13: Reserved
- 14: Reserved
- 15: frequency is written explictly

channel_configuration: 表示声道数
- 0: Defined in AOT Specifc Config
- 1: 1 channel: front-center
- 2: 2 channels: front-left, front-right
- 3: 3 channels: front-center, front-left, front-right
- 4: 4 channels: front-center, front-left, front-right, back-center
- 5: 5 channels: front-center, front-left, front-right, back-left, back-right
- 6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
- 7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
- 8-15: Reserved



后面的视频包都是AF 01 + 去除7个字节头的音频AAC数据










