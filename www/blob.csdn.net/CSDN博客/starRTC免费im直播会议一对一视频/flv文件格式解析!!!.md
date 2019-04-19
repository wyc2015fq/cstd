# flv文件格式解析!!! - starRTC免费im直播会议一对一视频 - CSDN博客
2016年11月17日 13:29:53[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：599
flv头 FLV header
总体上看，FLV包括文件头（File Header）和文件体（File Body）两部分，其中文件体由一系列的Tag组成。
Signature: FLV 文件的前3个字节为固定的‘F’‘L’‘V’,用来标识这个文件是flv格式的.在做格式探测的时候，如果发现前3个字节为“FLV”，就认为它是flv文件.
Version:
第4个字节表示flv版本号.
Flags: 第5个字节中的第0位和第2位,分别表示 video 与 audio 存在的情况.(1表示存在,0表示不存在)
如果都存在，值为5
DataOffset : 最后4个字节表示FLV
 header 长度.
综上，一共9个字节
unsigned char flvHeaderTag[9] = {0x46, 0x4C, 0x56,
0x01, 0x05, 
0x00, 0x00, 0x00, 0x09}; 
FLV头后面第一个prevTagSize始终为0
===========File Body==========
FLV header之后,就是File Body.FLV File Body是由一连串的PreviousTagSize
 + tags构成. PreviousTagSize就是4个字节数据大小,表示前一个tag的size.
==========FLV Tag=====
FLV文件中的数据都是由一个个TAG组成,TAG里面的数据可能是video、audio、scripts.
TagType: TAG中第1个字节中的前5位表示这个TAG中包含数据的类型, 8
 = audio,9 = video,18 = script data.
DataSize（3个字节）:StreamID之后的数据长度.（length
 of the tag – 11），其中UI24表示3个字节，11为整个Tag除去Data之外的大小。
Timestamp和TimestampExtended组成了这个TAG包数据的PTS信息,记得刚开始做FLV demux的时候，并没有考虑TimestampExtended的值,直接就把Timestamp默认为是PTS，后来发生的现象就是画面有跳帧的现象,后来才仔细看了一下文档发现真正数据的PTS是PTS=
 Timestamp | TimestampExtended<<24.
StreamID之后就是数据。每种格式都不一样，下面进行详细介绍.
即一直到StreamId一共有11个字节
===========Audio Tag==============
如果TAG包中的TagType==8时，就表示这个TAG是audio。
StreamID之后的数据是AudioTagHeader，AudioTagHeader结构如下：
AudioTagHeader的第1个字节，也就是紧跟着StreamID的1个字节包含着音频类型、采样率等基本信息.上述表里列的十分清楚.AudioTagHeader之后跟着的就是AUDIODATA数据了，也就是audio
 payload 但是这里有个特例，如果音频格式（SoundFormat）是10 = AAC，AudioTagHeader中会多出1个字节的数据AACPacketType，这个字段来表示AACAUDIODATA的类型：0
 = AAC sequence header，1 = AAC raw。
图片双击可看大图：
- AAC sequence header也就是包含了AudioSpecificConfig，AudioSpecificConfig包含着一些更加详细的音频信息，AudioSpecificConfig的定义在ISO14496-3中1.6.2.1
 AudioSpecificConfig。
在ffmpeg中有对AudioSpecificConfig解析的函数，ff_mpeg4audio_get_config()(我的ffmpeg版本中是avpriv_mpeg4audio_get_config())。
- AAC raw 这种包含的就是音频ES流了，也就是audio payload.
在FLV文件中，一般情况下 AAC sequence header 这种包只出现1次，而且是第一个audio tag，为什么要提到这种tag，因为当时在做FLV demux的时候，如果是AAC的音频，需要在每帧AAC ES流前边添加7个字节ADST头,ADST在音频的格式中会详细解读，这是解码器通用的格式，就是AAC的纯ES流要打包成ADST格式的AAC文件，解码器才能正常播放.就是在打包ADST的时候，需要samplingFrequencyIndex这个信息，samplingFrequencyIndex最准确的信息是在AudioSpecificConfig中，所以就对AudioSpecificConfig进行解析并得到了samplingFrequencyIndex。
 到这步你就完全可以把FLV 文件中的音频信息及数据提取出来，送给音频解码器正常播放了。
==========Video Tag============
如果TAG包中的TagType==9时，就表示这个TAG是video. StreamID之后的数据就表示是VideoTagHeader，VideoTagHeader结构如下：
VideoTagHeader的头1个字节，也就是紧跟着StreamID的1个字节包含着视频帧类型(比如关键帧，p帧)及视频CodecID信息.
VideoTagHeader之后跟着的就是VIDEODATA数据了，也就是video payload.当然就像音频AAC一样，这里也有特例，即如果视频的格式是AVC（H.264）的话，VideoTagHeader会多出4个字节的信息：AVCPacketType
 和 CompositionTime。
AVCPacketType 表示接下来 VIDEODATA （AVCVIDEOPACKET）的内容：
IF AVCPacketType == 0
AVCDecoderConfigurationRecord（AVC sequence header）
IF AVCPacketType == 1 One or more NALUs (Full frames are required)
还有如果为2，表示是结束包，一般在flv文件结尾。
AVCDecoderConfigurationRecord包含着H.264解码比较重要的sps和pps信息，在给AVC解码器送数据流之前一定要把sps和pps信息送出，否则解码器不能正常解码。而且在解码器stop之后再次start之前，如seek、快进快退状态切换等，都需要重新送一遍sps和pps的信息。AVCDecoderConfigurationRecord在FLV文件中一般情况也是出现1次，也就是第一个video
 tag. AVCDecoderConfigurationRecord的定义在ISO 14496-15, 5.2.4.1中。
CompositionTime:3个字节
===========AVCDecoderConfigurationRecord=========
FFmpeg中extradata_size格式: 4B大小+SPS+4B大小+pps
AVCDecoderConfigurationRecord
结构的定义：
aligned(8) class AVCDecoderConfigurationRecord { 
unsigned int(8) configurationVersion = 1; 
unsigned int(8) AVCProfileIndication; 
unsigned int(8) profile_compatibility; 
unsigned int(8) AVCLevelIndication; 
bit(6) reserved = ‘111111’b; 
unsigned int(2) lengthSizeMinusOne; 
bit(3) reserved = ‘111’b; 
unsigned int(5) numOfSPS; 
for (i=0; i< numOfSPS; i++) { 
unsigned int(16) SPSLength ; 
bit(8*sequenceParameterSetLength) sequenceParameterSetNALUnit; 
} 
unsigned int(8) numOfPPS; 
for (i=0; i< numOfPPSet; i++) { 
unsigned int(16) PPSetLength; 
bit(8*pictureParameterSetLength) pictureParameterSetNALUnit; 
} 
}
下面蓝色的部分就是
 FLV 文件中的 AVCDecoderConfigurationRecord
部分。
17 00 表示是参数包，然后00 00 00 是:3个字节CompositionTime
00000130h: 00 00 00
17 00 00 00 0001 4D 40 15
FF E1 00 0A ; .........M@.?. 
00000140h: 67 4D 40 15 96 53 01 00 4A 20
 01 00 05 68 E9 23 ; gM@.朣..J
 ...h? 
00000150h: 88 00 00
 00 00 2A 08 00 00 52 00 00 00 00 00 00 ; ?...*...R......
根据定义：
·configurationVersion = 01
·AVCProfileIndication = 4D
·profile_compatibility = 40
·AVCLevelIndication = 15
·lengthSizeMinusOne = FF <-
非常重要，是 H.264
视频中 NALU
的长度，计算方法是
 1 + (lengthSizeMinusOne & 3) 计算结果为4
·numOfSequenceParameterSets = E1 <- SPS
的个数，计算方法是 numOfSequenceParameterSets
& 0x1F 计算结果为1
·sequenceParameterSetLength = 00 0A <-
SPS 的长度
·sequenceParameterSetNALUnits = 67 4D 40 15 96 53 01 00 4A 20 <- SPS数据
·numOfPictureParameterSets = 01 <- PPS
的个数
·pictureParameterSetLength = 00 05 <- PPS
的长度
·pictureParameterSetNALUnits = 68 E9 23 88 00 <- PPS数据
资料：
FLV文件格式规范.pdf
解析工具：
FlvParse
SpecialFFLV
