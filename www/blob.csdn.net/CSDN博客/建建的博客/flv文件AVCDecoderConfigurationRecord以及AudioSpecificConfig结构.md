# flv文件AVCDecoderConfigurationRecord以及AudioSpecificConfig结构 - 建建的博客 - CSDN博客
2018年06月20日 10:56:06[纪建](https://me.csdn.net/u013898698)阅读数：33
## H264和AAC数据流
　　RTMP推送的音视频流的封装形式和FLV格式相似，由此可知，向FMS推送H264和AAC直播流，需要首先发送"AVC sequence header"和"AAC sequence header"，这两项数据包含的是重要的编码信息，没有它们，解码器将无法解码。
　　AVC sequence header就是AVCDecoderConfigurationRecord结构，该结构在标准文档“ISO-14496-15 AVC file format”中有详细说明。
![](http://pic002.cnblogs.com/images/2011/254714/2011122913292955.jpg)
　　AAC sequence header存放的是AudioSpecificConfig结构，该结构则在“ISO-14496-3 Audio”中描述。AudioSpecificConfig结构的描述非常复杂，这里我做一下简化，事先设定要将要编码的音频格式，其中，选择"AAC-LC"为音频编码，音频采样率为44100，于是AudioSpecificConfig简化为下表：
![](http://pic002.cnblogs.com/images/2011/254714/2011122913505721.jpg)
　　这样，AVC sequence header和AAC sequence header的内容可以基本确定了，更详细的信息，大家可以去翻阅相关文档。
## 举例
上图并非自己画的，因此遗漏了sequenceParameterSetLength，pictureParameterSetLength
下面蓝色的部分就是 FLV 文件中的 AVCDecoderConfigurationRecord 部分：
00000130h: 00 00 00 17 00 00 00 00 01 4D 40 15 FF E1 00 0A ; .........M@.?. 
00000140h: 67 4D 40 15 96 53 01 00 4A 20 01 00 05 68 E9 23 ; gM@.朣..J ...h? 
00000150h: 88 00 00 00 00 2A 08 00 00 52 00 00 00 00 00 00 ; ?...*...R......
根据 AVCDecoderConfigurationRecord 结构的定义：
- configurationVersion = 01
- AVCProfileIndication = 4D
- profile_compatibility = 40
- AVCLevelIndication = 15
- lengthSizeMinusOne = FF <- 非常重要，是 H.264 视频中 NALU 的长度，计算方法是 1 + (lengthSizeMinusOne & 3)，实际计算结果一直是4
- numOfSequenceParameterSets = E1 <- SPS 的个数，计算方法是 numOfSequenceParameterSets & 0x1F，实际计算结果一直为1
- sequenceParameterSetLength = 00 0A <- SPS 的长度
- sequenceParameterSetNALUnits = 67 4D 40 15 96 53 01 00 4A 20 <- SPS
- numOfPictureParameterSets = 01 <- PPS 的个数，一直为1
- pictureParameterSetLength = 00 05 <- PPS 的长度
- pictureParameterSetNALUnits = 68 E9 23 88 00 <- PPS
下面蓝色的为AudioSpecificConfig部分：
000001e0h: 00 00 00 00 00 AF 0011 90 00 00 00 0F 09 00 02; .....?.?......
根据AudioSpecificConfig结构的定义：
- audioObjectType = (0x11 & 0xF8) >> 3 <- 取前5bit，结果为2
- samplingFrequencyIndex = ((0x11 & 0x7) << 1) | (0x90 >> 7)<- 取后4bit，结果为3
- channelConfiguration = (0x90 >> 3) & 0x0F <- 取后4bit，结果为2
- frameLengthFlag = (0x90 >> 2) & 0x01<- 取后1bit，结果为0
- dependsOnCoreCoder = (0x90 >> 1) & 0x01 <- 取后1bit，结果为0
- extensionFlag = 0x90 & 0x01 <- 最后1bit，结果始终为0
- 
