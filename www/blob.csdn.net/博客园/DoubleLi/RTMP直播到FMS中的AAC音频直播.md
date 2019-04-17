# RTMP直播到FMS中的AAC音频直播 - DoubleLi - 博客园






本文引用了下面几个网友的文章：

[http://sun3eyes.blog.163.com/blog/#m=0&t=3&c=rtmp](http://sun3eyes.blog.163.com/blog/#m=0&t=3&c=rtmp)

[http://sun3eyes.blog.163.com/blog/static/1070797922012913337667/](http://sun3eyes.blog.163.com/blog/static/1070797922012913337667/)

[http://sun3eyes.blog.163.com/blog/static/107079792201291112451996/](http://sun3eyes.blog.163.com/blog/static/107079792201291112451996/)

[http://blog.csdn.net/helunlixing/article/details/7417778](http://blog.csdn.net/helunlixing/article/details/7417778)

直播的视频用H264，音频用AAC，从FAAC里面压缩出来的一帧音频数据，要经过简单处理才能打包用RTMP协议发送到FMS上，包括保存成FLV文件，都要稍微处理一下，主要是把AAC的帧头去掉，并提取出相应的信息。

1024字节的G.711A数据，AAC一般也就300多个字节。

可以把FAAC压缩出来的帧直接保存成AAC文件，用windows7自带的播放器可以播放的，方便测试。

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

有了这三个参数，就可以发送音频的第一帧了，然后后面的帧，**把包头的7个字节去掉？**，打包到RTMP协议发送就可以了。

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

**AACPacketType**，这个字段来表示**AACAUDIODATA**的类型：0 = AAC sequence header，1 = AAC raw。第一个音频包用0，后面的都用1

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

**channel_configuration:** 表示声道数
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










