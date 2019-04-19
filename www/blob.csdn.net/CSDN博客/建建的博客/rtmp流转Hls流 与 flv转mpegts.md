# rtmp流转Hls流 与 flv转mpegts - 建建的博客 - CSDN博客
2018年06月25日 15:59:35[纪建](https://me.csdn.net/u013898698)阅读数：165
rtmp协议是adobe出的一种流媒体格式协议。是目前较为流行的协议的一种。
HLS协议是苹果推出的一种流媒体协议，由于IOS系统不支持RTMP协议，如实现RTMP协议的流媒体需要自己实现RTMP的客户端，这使得移动手机端使用RTMP的流媒体增加了一些开发成本。
HLS协议的详情可以参考APPLE的网站，内容主要由M3U8和MPEGTS这两大文件组成，本文详解的是MPEGTS文件。
MPEG2-TS（Transport Stream“传输流”；又称 MTS、TS）是一种传输和存储包含音效、图像与通信协议各种数据的标准格式，用于数字电视广播系统，如 DVB、ATSC、IPTV 等等。
MPEG2-TS 定义于 MPEG-2 第一部份，系统（即原来之 ISO/IEC 标准 13818-1 或 ITU-T Rec. H.222.0）。
SMPLAYER、VLC多媒体播放器 等软件可以直接播放 MPEG-TS 文件。
MPEG2-TS传输流编码层允许一个或多个节目组合成单一流。来自每个基本流的数据同节目内允许该基本流同步显示的信息一起多路复用。
传输流由一个或多个节目组成。音频和视频基本流（ES）由存取单元（AU,一般常说的NALU）组成。
基本流数据（ES element stream ,  比如H264裸流）在PES 包中承载。PES 包由PES 包头及后随的包数据组成。PES 包插入到传输流包中。每个PES 包头的首字节定位于传输流包的第一个有效载荷位。
本文主要的参考为：ISO-13818-1，Adobe Flash Video File Format Specification  Version 10.1，ISO-14496-3，ISO-14486-15
本文所述内容与标准有一定的差异，这种差异并非指存在违背标准的内容，而是本文由于篇幅所限，对于某些约束，只取与符号标准的一项，而不是兼容标准所有的规范。
1.2结构
传输流类似OSI network 协议堆栈：
1.TS
2. PES
3.Elementarystream (ES) — audio or video (the below is for video only)
4.编码层..
1.3传输流数据包（transport stream packets）
TransportStream packets -> PES -> ES
TransportStream packets 是传输流（transport stream）内数据的最基本单位，包含了一个同步字节（sync byte）值为 0x47，它存在一个4bytes的固定包头，32bits 接着它的是可变包头区域。 Packets 有固定的188 bytes 的长度。
表1-1 Transport Stream packets结构
|字段|Value|bits||
|----|----|----|----|
|sync_byte|0x47|8|bslbf|
|transport_error_indicator|如果这个流中包含了一个无法修复的错误，由解调器设置，以告诉多路解调器，该包存在一个无法纠正的错误|1|bslbf|
|payload_unit_start_indicator|1 表示是 PES 数据或 PSI数据的开始部分，否则为零.|1|bslbf|
|transport_priority|1 意思是在相同 PID 的数据包中含有更高的优先权.|1|bslbf|
|PID|PID 为13 比特字段，指示包有效载荷中存储的数据类型，也就是包的标识号。|13|uimsbf|
|transport_scrambling_control|此2 比特字段指示传输流包有效载荷的加扰方式。|2|bslbf|
|adaptation_field_control|此2 比特字段指示此传输流包头是否后随自适应字段和/或有效载荷|2|bslbf|
|continuity_counter|continuity_counter 为4 比特字段，随着具有相同PID 的每个传输流包而增加|4|uimsbf|
|||||
这里最主要关系的是payload_unit_start_indicator，PID，adaptation_field_control，continuity_counter
payload_unit_start_indicator
这个位标志为1，指的是一个包的启示，因为ts包只有188个字节，对于一个PES包的话往往大于188字节，因此一个PES包往往要拆成多个TS包，为了识别收到的TS包属于另一个PES包，起始位表示新的一个PES包或者PSI包等到来了。
PID program id
节目标示符，一个13位的无符号整数。作用如下表描述。一般来说，参考FFMPEG，PMT表使用PID 4096，VIDEOSTREAM 采用256，AUDIOSTREAM采用257。
![](https://img-blog.csdn.net/20150619095926784)
adaptation_field_contro
当adaptation_field_control的值为10，接下来的是自适应字段adaptation_field，当adaptation_field_control的值如下表描述
![](https://img-blog.csdn.net/20150619100117208)
continuity_counter
这个是当前节目的一个计数器，独立于PID，也就是说各个PID分开计算。
接下来的就是有效的负载内容，具体的负载内容必须是来自PES 包（参阅2.4.3.6）、PSI 分段（参阅2.4.4）以及PSI 分段后的包填充字节数据的相连贯的数据字节，或者不在这些结构中的专用数据，如PID 所指示的。在具有PID 值0x1FFF 的空包情况中，data_bytes 可以指派为任何值。data_bytes 数N 通过184 减去 adaptation_field() 中的字节数来确定。
TS包结构图
![](https://img-blog.csdn.net/20150619100223052)
1.4 自适应字段 （adaptation_field）
自适应字段我们主要关注的是第一个adaptation  field length和PCR，这里重点讲解他们的主要用处：
    adaptationfield length指的是自适应字段的长度，也就是，从discontinuity indicator 到adaptation field最后的长度，也就是从第6字节（包含第6字节）开始算到最后。
PCR
这个值是系统的时间戳，在PES层时间戳是PTS与DTS，这里要注意与PCR，PTS,DTS的概念，可能会让人模糊。PCR是TS层的时间戳，PTS与DTS是PES的时间戳，PCR在PES层相当于DTS，TS不需要考虑PTS。为啥不需要，这里就要讲下，PTS的具体概念。详细的在ISO-13818-1上有，详细到可以看到你吐。其实实际中不需要考虑这么多。我简单的讲吧。在ES流中，依次组成图像帧序为I1P4B2B3P7B5B6I10B8B9的，这里,I、P、B分别指I帧，P帧，B帧。具体意义可以参考H264的相关基本概念，对于I、P帧而言，PES的图像帧序为I1P4B2B3P7B5B6I10B8B9，应该P4比B2、B3在先，但显示时P4一定 要比B2、B3在后，这就必须重新排序。在PTS/DTS时间标志指引下，将P4提前插入数据流，经过缓存器重新排序，重建视频帧序 I1B2B3P4B5B6P7B8B9I10。显然，PTS/DTS是表明确定事件或确定信息，并以专用时标形态确定事件或信息的开始时刻。说到这里，PTS,与DTS的概念应该明白了。但是为啥TS层不需要呢，因为TS层只是负责传输，你知道解码的时间在什么位置，确保传输的TS包不是延迟太久就可以了，具体的显示细节交给PES层去做。
2.1 PES包
PES将ES流打包，打包的依据我没有发现有硬性规定，但是一般会将ES流中的一个frame打包成一个PES,在H264的ES流中的NALU流如果分段的话，会由3个NALU组成一个frame，一般来说，都没有分段的选项。所以，我们可以简单的认为一个NALU就是一个frame。
   在ffmpeg中，H264的SPS与PPS一起打包在头一个NALU中，
PES包的字段没什么好讲的，
streamID , 这里视频H264填写0xe0 ,AAC音频填写OXCO
PES PACKET LENGTH 是从OPTIONAL FIELD 到包最后一个字节的长度，不算前面的4字节，和自身2字节，一般来说就是3+10+NALUSIZE，这里10是指VIDEOFRAME的，如果是AUDIOFRAME则是5.
PTS,和DTS的计算。如果是根据ES流计算怎么算，我还没想到容易的方法。但是如果根据FLV格式转换，PTS就是(flvTagHeader.timestamp +videoTagHeader.CompositionTime) * 90 , 为啥是90呢？flv里面的时间戳的单位都是毫秒的，1/1000秒。mpegts的系统时钟为27MHZ，这里要除以300(规定的除以300，参考ISO-13818-1)。也就是90000hz，一秒钟90000个周期，所以，PTS代表的周期flv的时间戳*90000/1000 ，90也就是这么来的。
TS层里的PCR可以直接采用DTS。
/
以下是截取的一个TS包，红色部分为SPS，PPS，蓝色部分为es 流中的startcode ，紫色部分为H.264 video access units must use Access Unit Delimiter NALs
这在[https://developer.apple.com/library/ios/documentation/networkinginternet/conceptual/streamingmediaguide/FrequentlyAskedQuestions/FrequentlyAskedQuestions.html](https://developer.apple.com/library/ios/documentation/networkinginternet/conceptual/streamingmediaguide/FrequentlyAskedQuestions/FrequentlyAskedQuestions.html)第10个问题那里说的很清楚。可以直接在每个NALU前面直接加上00 00 00 01 09 F0。
[cpp][view plain](https://blog.csdn.net/sara4321/article/details/46558481#)[copy](https://blog.csdn.net/sara4321/article/details/46558481#)
- 47 41 00 30 07 50 00 00 00 00 7E 00 00 00 01 E0  
- 50 A5 80 C0 0A 31 00 01 5D 85 11 00 01 00 01 00  
- 00 00 01 09 F0 00 00 00 01 67 64 00 15 AC C8 60  
- 20 09 6C 04 40 00 00 03 00 40 00 00 07 A3 C5 8B  
- 67 80 00 00 00 01 68 E9 BB 2C 8B 00 00 00 01 65  
- 88 84 00 7F 4D 32 19 FF 2D 02 C7 FE E6 51 22 40  
- 3D AA 71 9A 11 60 AE 24 4B BB 66 66 4E 71 C9 E0  
- E4 EB 4C D1 FE D4 B7 3B 8F 1D 36 2C 76 5B B9 12  
- 70 23 B6 F4 C0 EB 58 CD 66 AB 0D C5 2A 91 59 C6  
- 31 7F 7B A0 5A 76 AD 3F BD 01 88 6A 35 84 BE 58  
- 06 A0 1C 99 93 72 BC CE 61 6D 22 CB 89 E1 AD 56  
- 26 C0 48 66 78 2C 2C EF 38 F8 84 CF  
![](https://img-blog.csdn.net/20150619100432000)
2.2 PAT
节目相关表：
这个表主要是节目相关信息的表。这个表在转换的时候，可以采取一种暴力的方法，就是写死。
这里CRC校验的算法，最后会说，计算的字节是只计算PSI+PXT的内容，TS头部的内容不考虑。
Program map PID
13
The packet identifier that contains the associated PMT
[cpp][view plain](https://blog.csdn.net/sara4321/article/details/46558481#)[copy](https://blog.csdn.net/sara4321/article/details/46558481#)
- //TS
-    0x47,0x40,0x00,0x10,0x00,  
- //PSI,table id 00
-    0x00,0xb0,0x0d,0x00,0x01,0xc1,0x00,0x00,  
- //PAT
-    0x00,0x01,0xf0,0x01,  
- //crc
-    0x2e,0x70,0x19,0x05,  
- // stuffing 167 bytes
-    0xff,0xff,0xff,0xff,0xff,0xff,0xff,  
-    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  
-    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  
2.3 PMT
### PMT(Program map specific data)
Elementary stream info data
N*8
The streams used in this program map.
Elementary stream descriptors
N*8
When the ES info length is non-zero, this is the ES info length number of elementary stream descriptor bytes.
[cpp][view plain](https://blog.csdn.net/sara4321/article/details/46558481#)[copy](https://blog.csdn.net/sara4321/article/details/46558481#)
- //ts
-    0x47,0x50,0x01,0x10,0x00,  
- //PSI
-    0x02,0xb0,0x17,0x00,0x01,0xc1,0x00,0x00,  
- //PMT ,no pcr
-    0xe1,0x00,  
-    0xf0,0x00,  
-    0x1b,0xe1,0x00,0xf0,0x00,  
-    0x0f,0xe1,0x01,0xf0,0x00,  
- //crc
-    0x2f,0x44,0xb9,0x9b,  
- // stuffing 157 bytes
-    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  
-    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  
-    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  
3.FLV文件分析
FLV文件的结构，可以在网上找个flv parse的工具帮助分析。
可以看到，FLV的文件形式由FLV HEADER + pre Tag Size + Tag 组成
![](https://img-blog.csdn.net/20150619100739639)
首先每个TAG中的前面有个11字节的头部，这个描述了该tag中的内容的类型。后续接着的是相关类型的TAG内容。
![](https://img-blog.csdn.net/20150619100917554)
3.1 视频
对于VIDEO TAG来讲，首部会有个5字节的VIDEO TAG HEADER，
![](https://img-blog.csdn.net/20150619101009018)
![](https://img-blog.csdn.net/20150619101053825)
这里如果AVCPACKET TYPE为0，则表示AVC SEQUENCE HEADER，这里包含着很重要的信息，也就是H264 ES流中解码的关键信息--SPS ,PPS的内容
AVC SEQUENCE HEADER的具体内容可参考， ISO-14496-15AVC file format。这里简化如下表所示。 AVC SEQUENCEHEADER一般情况只出现一次，并出现在FLV文件的第一个VIDEO TAG。这里引用一张别人的图，出处我忘了不好意思，因为项目是一年前做的。
![](https://img-blog.csdn.net/20150619101156016)
从AVC PACKET TYPE 看出每个VIDEO TAG至少包含一个完整的FRAME。在FLV中的NALU，需要加上00 00 00 01这样的NALU头，并在前面再加上 00 00 00 01 09 f0，具体的原因应该是规范。视频的PTS的时间戳由TAG里的TIMESTAMP + COMPOSITIONTIME构成。DTS可直接由TIMESTAMP。这里FLV里的是毫秒，需要*90向MPEGTS的时钟周期进行转换
3.2音频
音频的TAG如下图所示，值得注意的是AAC SEQUENCE HEADER，这里和视频头部一样，其实质内容是AudioSpecificConfig ，定义在14496-3中。包含了AAC解码的关键内容，ADTS HEADER的内容。ADTS 的内容定义在13818-7中。这些定义在标准里面写的看起来很复杂，实际上内容倒也没什么。AAC ES流中要求每个音频frame前面要加上ADTS HEADER 的内容。我们根据首个音频TAG中提取2字节的AUDIO SPECIFIC CONFIG，转换成7字节的ADTS HEADER，代码可参考FFMPEG等，很多有提供。这里注意，很容易出错。
![](https://img-blog.csdn.net/20150619101423582)
![](https://img-blog.csdn.net/20150619101502568)
RTMP媒体流中，第一个音频包里保护的一定是AAC header.
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
![](https://img-blog.csdn.net/20150619101607670)
0x13 0x90（1001110010000） 表示 ObjectProfile=2， AAC-LC，SamplingFrequencyIndex=7，ChannelConfiguration=声道2
AudioSpecificConfig，即为ObjectProfile，SamplingFrequencyIndex，ChannelConfiguration，TFSpecificConfig。
其中，ObjectProfile (AAC main ~1, AAC lc ~2, AAC ssr ~3)；
SamplingFrequencyIndex (0 ~ 96000， 1~88200， 2~64000， 3~48000， 4~44100， 5~32000， 6~24000， 7~ 22050， 8~16000...)，通常aac固定选中44100，即应该对应为4，但是试验结果表明，当音频采样率小于等于44100时，应该选择3，而当音频采样率为48000时，应该选择2；
ChannelConfiguration对应的是音频的频道数目。单声道对应1，双声道对应2，依次类推。
TFSpecificConfig的说明见标准14496-3中（1.2 T/F Audio Specific Configuration）的讲解，这里恒定的设置为0；
索引值如下含义：
There are 13 supported frequencies:
0: 96000 Hz
1: 88200 Hz
2: 64000 Hz
3: 48000 Hz
4: 44100 Hz
5: 32000 Hz
6: 24000 Hz
7: 22050 Hz
8: 16000 Hz
9: 12000 Hz
10: 11025 Hz
11: 8000 Hz
12: 7350 Hz
13: Reserved
14: Reserved
15: frequency is written explictly
channel_configuration: 表示声道数
0: Defined in AOT Specifc Config
1: 1 channel: front-center
2: 2 channels: front-left, front-right
3: 3 channels: front-center, front-left, front-right
4: 4 channels: front-center, front-left, front-right, back-center
5: 5 channels: front-center, front-left, front-right, back-left,back-right
6: 6 channels: front-center, front-left, front-right, back-left,back-right, LFE-channel
7: 8 channels: front-center, front-left, front-right, side-left,side-right, back-left, back-right, LFE-channel
8-15: Reserved
