# librtmp获取视频流和音频流1 - 深之JohnChen的专栏 - CSDN博客

2017年01月03日 13:59:14[byxdaz](https://me.csdn.net/byxdaz)阅读数：3561


libRTMP 库从RTMP 直播服务器不断地获取RTMP 包，从RTMP包中获取音频和视频数据。RTMP只是一个应用层协议，传输的数据格式都是基于FLV格式的。

我们在推送音、视频包之前，会首先向服务器推送一个音、视频同步包，该包包含了AAC 音频帧以及H264 码流的解码信息。因此当我们首次向服务器请求RTMP 包之后，服务器会下发给我们对应的音、视频同步包，这将决定我们如何解析并重组音、视频。

一、首先要了解FLV格式。

flv文件主要由两部分组成：header和body。

1.header

header部分记录了flv的类型、版本等信息，是flv的开头，一般都差不多，占9bytes。具体格式如下：
|文件类型|3 bytes|“FLV”|
|----|----|----|
|版本|1 byte|一般为0x01|
|流信息|1 byte|倒数第一位是1表示有视频，倒数第三位是1表示有音频，倒数第二、四位必须为0|
|header长度|4 bytes|整个header的长度，一般为9；大于9表示下面还有扩展信息|

2.body

body部分由一个个Tag组成，每个Tag的下面有一块**4bytes**的空间，用来记录这个tag的长度，这个后置用于逆向读取处理，他们的关系如下图：

![](https://img-blog.csdn.net/20170103135031472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**2.1.Tag**

每个Tag由也是由两部分组成的：Tag Header和Tag Data。Tag Header里存放的是当前Tag的类型、数据区（Tag Data）长度等信息，具体如下：
|**名称**|**长度**|**介绍**|
|----|----|----|
|Tag类型|1 bytes|8：音频9：视频18：脚本其他：保留|
|数据区长度|3 bytes|在数据区的长度|
|时间戳|3 bytes|整数，单位是毫秒。对于脚本型的tag总是0|
|时间戳扩展|1 bytes|将时间戳扩展为4bytes，代表高8位。很少用到|
|StreamsID|3 bytes|总是0|
|数据区(data)|由数据区长度决定|数据实体|
**2.2.Tag Data**
数据区根据Tag类型的不同可分为三种，音频数据、视频数据和脚本数据。

**2.2.1.音频数据**

音频数据包括音频同步包和音频raw数据。

音频同步包格式
|**名称**|**二进制值**|Hex|介绍|
|----|----|----|----|
|音频格式|4 bits[AAC]1010|0xA|0 = Linear PCM, platform endian1 = ADPCM2 = MP33 = Linear PCM, little endian4 = Nellymoser 16-kHz mono5 = Nellymoser 8-kHz mono6 = Nellymoser7 = G.711 A-law logarithmic PCM8 = G.711 mu-law logarithmic PCM9 = reserved10 = AAC11 = Speex14 = MP3 8-Khz15 = Device-specific sound|
|采样率|2 bits[44kHZ]11||0 = 5.5-kHz1 = 11-kHz2 = 22-kHz3 = 44-kHz对于AAC总是3|
|采样的长度|1 bit||0 = snd8Bit1 = snd16Bit压缩过的音频都是16bit|
|音频类型|1 bit||0 = sndMono，单声道1 = sndStereo，立体声对于AAC总是1|
|ACCPacketType|8bit00000000只有SoundFormat == 10时才有此8bi的字段||0x00,表示音频同步包；0x01，表示音频raw数据。|
|AudioObjectType|5bits[AAC LC]00010|||
|SampleRateIndex|4bits[44100]0100|||
|ChannelConfig|4bits[Stereo]0010|||
|FrameLengthFlag|1bit0|||
|dependOnCoreCoder|1bit0|||
|extensionFlag|1bit0|||
共4个字节（2bytes AACDecoderSpecificInfo和2bytesAudioSpecificConfig）
注：speex数据

每个音频包（相当于FLV中的audiotag）中，第一个字节的前四位表示编码格式，等于11说明为speex编码。后4个字节分别表示编码采样率、单声道or立体声、每个sample大小8位or16位。但采用speex编码时，它们是固定不变的，协议中的为无效数据。编码采样率恒为16khz，单声道，16bit/sample。剩余的数据为音频帧数据。

AACsequence header存放的是**AudioSpecificConfig**结构，该结构则在“**ISO-14496-3Audio**”中描述。AudioSpecificConfig结构的描述非常复杂，这里我做一下简化，事先设定要将要编码的音频格式，其中，选择"AAC-LC"为音频编码，音频采样率为44100，于是AudioSpecificConfig简化为下表：

![](https://img-blog.csdn.net/20170103135213593)

音频raw数据格式
|**名称**|**二进制值**|Hex|介绍|
|----|----|----|----|
|音频格式|4 bits[AAC]1010|0xA|0 = Linear PCM, platform endian1 = ADPCM2 = MP33 = Linear PCM, little endian4 = Nellymoser 16-kHz mono5 = Nellymoser 8-kHz mono6 = Nellymoser7 = G.711 A-law logarithmic PCM8 = G.711 mu-law logarithmic PCM9 = reserved10 = AAC11 = Speex14 = MP3 8-Khz15 = Device-specific sound|
|采样率|2 bits[44kHZ]11||0 = 5.5-kHz1 = 11-kHz2 = 22-kHz3 = 44-kHz对于AAC总是3|
|采样的长度|1 bit||0 = snd8Bit1 = snd16Bit压缩过的音频都是16bit|
|音频类型|1 bit||0 = sndMono，单声道1 = sndStereo，立体声对于AAC总是1|
|ACCPacketType|8bit只有SoundFormat == 10时才有此8bi的字段||0x00,表示音频同步包；0x01，表示音频raw数据。|
|Raw data|||音频raw数据|
**AudioSpecificConfig部分参数含义见：**
[https://wiki.multimedia.cx/index.php?title=MPEG-4_Audio](https://wiki.multimedia.cx/index.php?title=MPEG-4_Audio)

**2.2.2.视频数据**

视频同步包
|**Field**|**Type**|**Comment**|
|----|----|----|
|Frame Type|UB [4]|Type of video frame. The following values are defined:1 = key frame (for AVC, a seekable frame)2 = inter frame (for AVC, a non-seekable frame)3 = disposable inter frame (H.263 only)4 = generated key frame (reserved for server use only)5 = video info/command frame|
|CodecID|UB [4]|Codec Identifier. The following values are defined:2 = Sorenson H.2633 = Screen video4 = On2 VP65 = On2 VP6 with alpha channel6 = Screen video version 27 = AVC|
|AVCPacketType|IF CodecID == 7UI8|The following values are defined:0 = AVC sequence header1 = AVC NALU2 = AVC end of sequence (lower level NALU sequence ender is not required or supported)|
|CompositionTime|IF CodecID == 7SI24|IF AVCPacketType == 1Composition time offsetELSE0See ISO 14496-12, 8.15.3 for an explanation of compositiontimes. The offset in an FLV file is always in milliseconds.|
**VideoTagHeader**的头1个字节，也就是接跟着**StreamID**的1个字节包含着视频帧类型及视频CodecID最基本信息.表里列的十分清楚.
**VideoTagHeader**之后跟着的就是**VIDEODATA**数据了，也就是video payload.当然就像音频AAC一样，这里也有特例就是如果视频的格式是AVC（H.264）的话，**VideoTagHeader**会多出4个字节的信息.

AVCPacketType和 CompositionTime。AVCPacketType表示接下来**VIDEODATA（AVCVIDEOPACKET）**的内容：

IF AVCPacketType == 0 **AVCDecoderConfigurationRecord**（AVC sequence header）
IF AVCPacketType == 1 **One or more NALUs (Full frames are required)**

AVCDecoderConfigurationRecord.包含着是H.264解码相关比较重要的sps和pps信息，再给AVC解码器送数据流之前一定要把sps和pps信息送出，否则的话解码器不能正常解码。而且在解码器stop之后再次start之前，如seek、快进快退状态切换等，都需要重新送一遍sps和pps的信息.AVCDecoderConfigurationRecord在FLV文件中一般情况也是出现1次，也就是第一个video tag.

AVC sequence header就是**AVCDecoderConfigurationRecord**结构，该结构在标准文档“**ISO-14496-15AVC file format**”中有详细说明。

![](https://img-blog.csdn.net/20170103135321485)

**2.2.3脚本数据**

脚本Tag一般只有一个，是flv的第一个Tag，用于存放flv的信息，比如duration、audiodatarate、creator、width等。

首先介绍下脚本的数据类型。所有数据都是以**数据类型+（数据长度）+数据**的格式出现的，数据类型占1byte，数据长度看数据类型是否存在，后面才是数据。
其中数据类型的种类有：
- 0 = Number type 
- 1 = Boolean type 
- 2 = String type 
- 3 = Object type 
- 4 = MovieClip type 
- 5 = Null type 
- 6 = Undefined type 
- 7 = Reference type 
- 8 = ECMA array type 
- 10 = Strict array type 
- 11 = Date type 
- 12 = Long string type

如果类型为String，后面的2bytes为字符串的长度（LongString是4bytes），再后面才是字符串数据；如果是Number类型，后面的8bytes为Double类型的数据；Boolean类型，后面1byte为Bool类型。

知道了这些后再来看看flv中的脚本，一般开头是0x02，表示String类型，后面的2bytes为字符串长度，一般是0x000a（“onMetaData”的长度），再后面就是字符串“onMetaData”。好像flv格式的文件都有onMetaData标记，在运行ActionScript的时候会用到它。后面跟的是0x08，表示ECMA Array类型，这个和Map比较相似，一个键跟着一个值。键都是String类型的，所以开头的0x02被省略了，直接跟着的是字符串的长度，然后是字符串，再是值的类型，也就是上面介绍的那些了。

第一个AMF包：第1个字节表示AMF包类型。

第二个AMF包：

第1个字节表示AMF包类型，一般总是0x08，表示数组。第2-5个字节为UI32类型值，表示数组元素的个数。后面即为各数组元素的封装，数组元素为元素名称和值组成的对。常见的数组元素如下表：

一、 通过RTMP_ReadPacket获取一包数据，在里面获取视频、音频、scripttag等。

视频H264获取过程:
1、获取第一视频同步包之后，解析到sps、pps数据。视频头增加H264边界分割符（*00 00 00* 01 或*0000*01分隔符）。
2、其他视频包，解析出nalu数据。视频头增加H264边界分割符。

音频AAC获取过程:
1、获取第一音频同步包之后，解析到AACDecoderSpecificInfo和AudioSpecificConfig。AudioSpecificConfig用于生成ADST。
2、其他音频包，解析出原始音频数据（即es）。
一般的AAC解码器都需要把AAC的ES流打包成ADTS的格式，一般是在AAC ES流前添加7个字节的ADTSheader。也就是说你可以吧ADTS这个头看作是AAC的frameheader。

ADTS_header

AAC ES

ADTS_header

AAC ES

...

ADTS_header

AAC ES

## ADTS内容及结构

ADTS 头中相对有用的信息**采样率、声道数、帧长度**。想想也是，我要是解码器的话，你给我一堆得AAC音频ES流我也解不出来。每一个带ADTS头信息的AAC流会清晰的告送解码器他需要的这些信息。

一般情况下ADTS的头信息都是7个字节，分为2部分：

**adts_fixed_header();**

**adts_variable_header();**

![](https://img-blog.csdn.net/20170103135445086)

**syncword **：同步头总是0xFFF,all bits must be 1，代表着一个ADTS帧的开始

**ID**：MPEGVersion: 0 for MPEG-4, 1 for MPEG-2

**Layer**：always: '00'

**profile**：表示使用哪个级别的AAC，有些芯片只支持AAC LC 。在MPEG-2AAC中定义了3种：

![](https://img-blog.csdn.net/20170103135543954)

**sampling_frequency_index**：表示使用的采样率下标，通过这个下标在**Sampling Frequencies[ ]**数组中查找得知采样率的值。

Thereare 13 supported frequencies:
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

![](https://img-blog.csdn.net/20170103135700886)

**frame_length** :一个ADTS帧的长度包括ADTS头和AAC原始流。

**adts_buffer_fullness：**0x7FF说明是码率可变的码流。

代码

```cpp
/**
 * 本程序使用RTMP_ReadPacket与RTMP_ClientPacket方式读取流数据，接收RTMP流媒体并存储视频h264和音频aac文件。
*/
#include <stdio.h>
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

//ACC音频ADTS
typedef struct tagAACDecoderSpecific
{
	unsigned char nAudioFortmatType;	//音频编码类型（0：Liner PCM platform endian，1：PCM，2：mp3，4：Nellymoser 16-kHz mono,5:Nellymoser 8-kHz mono,6:Nellymoser,7:G.711 A-law logarithmic PCM,8:G.711 mu-law logarithmic PCM,9:reserved,10: AAC,14:MP3 8-Khz,15:Device-specific sound）
	unsigned char nAudioSampleType;	//音频采样率(0:5.5kHz,1:11KHz,2:22 kHz,3:44 kHz)
	unsigned char nAudioSizeType;	//音频采样精度(0:8bits,1:16bits)
	unsigned char nAudioStereo;//是否立体声(0:sndMono,1:sndStereo)
	unsigned char nAccPacketType;
}AACDecoderSpecific;

typedef struct tagAudioSpecificConfig
{
	unsigned char nAudioObjectType;
	unsigned char nSampleFrequencyIndex;
	unsigned char nChannels;
	unsigned char nFrameLengthFlag;
	unsigned char nDependOnCoreCoder;
	unsigned char nExtensionFlag;
}AudioSpecificConfig;

int InitSockets()
{
#ifdef WIN32
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(1, 1);
	return (WSAStartup(version, &wsaData) == 0);
#endif
}

void CleanupSockets()
{
#ifdef WIN32
	WSACleanup();
#endif
}

//解析ScriptTag
void ParseScriptTag(char *pBuffer, int nBufferLength, int &nVideoCodecId, int &nVideoWidth, int &nVideoHeight, int &nFrameRate, int &nAudioCodecId, int &nAudioSampleRate, int &nAudioSampleSize, bool &bStereo, int &nFileSize)
{
	AMFObject obj;
	AVal val;
	AMFObjectProperty * property;
	AMFObject subObject;
	int nRes = AMF_Decode(&obj, pBuffer, nBufferLength, FALSE);
	if (nRes < 0)
	{
		//RTMP_Log(RTMP_LOGERROR, "%s, error decoding invoke packet", __FUNCTION__);
		return;
	}

	AMF_Dump(&obj);
	for (int n = 0; n < obj.o_num; n++)
	{
		property = AMF_GetProp(&obj, NULL, n);
		if (property != NULL)
		{
			if (property->p_type == AMF_OBJECT)
			{
				AMFProp_GetObject(property, &subObject);
				for (int m = 0; m < subObject.o_num; m++)
				{
					property = AMF_GetProp(&subObject, NULL, m);
					if (property != NULL)
					{
						if (property->p_type == AMF_OBJECT)
						{

						}
						else if (property->p_type == AMF_BOOLEAN)
						{
							int bVal = AMFProp_GetBoolean(property);
							if (strncmp("stereo", property->p_name.av_val, property->p_name.av_len) == 0)
							{
								bStereo = bVal > 0 ? true : false;
							}
						}
						else if (property->p_type == AMF_NUMBER)
						{
							double dVal = AMFProp_GetNumber(property);
							if (strncmp("width", property->p_name.av_val, property->p_name.av_len) == 0)
							{
								nVideoWidth = (int)dVal;
							}
							else if (stricmp("height", property->p_name.av_val) == 0)
							{
								nVideoHeight = (int)dVal;
							}
							else if (stricmp("framerate", property->p_name.av_val) == 0)
							{
								nFrameRate = (int)dVal;
							}
							else if (stricmp("videocodecid", property->p_name.av_val) == 0)
							{
								nVideoCodecId = (int)dVal;
							}
							else if (stricmp("audiosamplerate", property->p_name.av_val) == 0)
							{
								nAudioSampleRate = (int)dVal;
							}
							else if (stricmp("audiosamplesize", property->p_name.av_val) == 0)
							{
								nAudioSampleSize = (int)dVal;
							}
							else if (stricmp("audiocodecid", property->p_name.av_val) == 0)
							{
								nAudioCodecId = (int)dVal;
							}
							else if (stricmp("filesize", property->p_name.av_val) == 0)
							{
								nFileSize = (int)dVal;
							}
						}
						else if (property->p_type == AMF_STRING)
						{
							AMFProp_GetString(property, &val);
						}

					}
				}
			}
			else
			{
				AMFProp_GetString(property, &val);
			}
		}
	}
}

//生成ADTS
int CreateADTS(AudioSpecificConfig ascAudioSpecificConfig, unsigned short nAudioFrameLength, char *pADTS)
{
		if (pADTS == NULL)
		return -1;

	unsigned char adts[7] = { 0 };
	int chanCfg = ascAudioSpecificConfig.nChannels;
	// fill in ADTS data
	adts[0] = (byte)0xFF;
	adts[1] = (byte)0xF9;
	adts[2] = (byte)(((ascAudioSpecificConfig.nAudioObjectType - 1) << 6) + (ascAudioSpecificConfig.nSampleFrequencyIndex << 2) + (chanCfg >> 2));
	adts[3] = (byte)(((chanCfg & 3) << 6) + (nAudioFrameLength >> 11));
	adts[4] = (byte)((nAudioFrameLength & 0x7FF) >> 3);
	adts[5] = (byte)(((nAudioFrameLength & 7) << 5) + 0x1F);
	adts[6] = (byte)0xFC;

	/*
	int i = 0;
	adts[i++] = 0xFF;	//AAAAAAAA
	adts[i++] = 0xF9;	//AAAABCCD
	
	//calc EEFFFFGH 8 bits
	adts[i] = ((ascAudioSpecificConfig.nAudioObjectType - 1) << 6) & 0xC0;
	adts[i++] = (ascAudioSpecificConfig.nSampleFrequencyIndex << 2) & 0x3C;
	
	adts[i] = (ascAudioSpecificConfig.nChannels << 6) & 0xFF;
	adts[i++] &= 0xC0;

	unsigned short frameLength = nAudioFrameLength;			//一个ADTS帧的长度包括ADTS头和AAC原始流
	adts[i++] = ((frameLength << 5) & 0xFF00) >> 8;
	adts[i] = (frameLength & 0x07) << 5;
	adts[i++] = 0x1F;
	adts[6] = 0xFC;
	*/
	if (pADTS != NULL)
	{
		memcpy(pADTS, adts, sizeof(adts));
	}
	return 0;
}

int main(int argc, char* argv[])
{
	InitSockets();
	
	double duration=-1;
	int nRead;
	//is live stream ?
	bool bLiveStream=true;				
	
	
	int bufsize=1024*1024*10;			
	char *buf=(char*)malloc(bufsize);
	memset(buf,0,bufsize);
	long countbufsize=0;
	
	//点播
	if (0 == 1)
	{
		FILE *fp = fopen("receive.flv", "wb");
		if (!fp){
			RTMP_LogPrintf("Open File Error.\n");
			CleanupSockets();
			return -1;
		}

		/* set log level */
		//RTMP_LogLevel loglvl=RTMP_LOGDEBUG;
		//RTMP_LogSetLevel(loglvl);

		RTMP *rtmp = RTMP_Alloc();
		RTMP_Init(rtmp);
		//set connection timeout,default 30s
		rtmp->Link.timeout = 30;
		// HKS's live URL
		if(!RTMP_SetupURL(rtmp,"rtmp://live.hkstv.hk.lxdns.com/live/hks"))
		//if(!RTMP_SetupURL(rtmp,"rtmp://localhost/live/hks"))
		//if (!RTMP_SetupURL(rtmp, "rtmp://localhost:1935/flvplayback/flv:1.flv"))//rtmp://localhost:1935/flvplayback/mp4:2.mp4
		//if (!RTMP_SetupURL(rtmp, "rtmp://localhost:1935/flvplayback/mp4:2.mp4"))
		{
			RTMP_Log(RTMP_LOGERROR, "SetupURL Err\n");
			RTMP_Free(rtmp);
			CleanupSockets();
			return -1;
		}

		//1hour
		RTMP_SetBufferMS(rtmp, 3600 * 1000);

		if (!RTMP_Connect(rtmp, NULL)){
			RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
			RTMP_Free(rtmp);
			CleanupSockets();
			return -1;
		}

		if (!RTMP_ConnectStream(rtmp, 0)){
			RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
			RTMP_Close(rtmp);
			RTMP_Free(rtmp);
			CleanupSockets();
			return -1;
		}

		//它直接输出的就是FLV文件,包括FLV头,可对流按照flv格式解析就可提取音频,视频数据
		while(nRead=RTMP_Read(rtmp,buf,bufsize)){
			fwrite(buf,1,nRead,fp);
			countbufsize+=nRead;
			RTMP_LogPrintf("Receive: %5dByte, Total: %5.2fkB\n",nRead,countbufsize*1.0/1024);
		}
		if (fp)
			fclose(fp);

		if (buf){
			free(buf);
		}

		if (rtmp){
			RTMP_Close(rtmp);
			RTMP_Free(rtmp);
			CleanupSockets();
			rtmp = NULL;
		}
		return 0;
	}

	//直播
	if (0 == 0)
	{
		FILE *fp = fopen("receive.h264", "wb");
		if (!fp){
			RTMP_LogPrintf("Open File Error.\n");
			CleanupSockets();
			return -1;
		}
		FILE *fpAAC = fopen("receive.m4a", "wb");
		if(!fpAAC){
			RTMP_LogPrintf("Open File Error.\n");
			CleanupSockets();
			return -1;
		}

		/* set log level */
		//RTMP_LogLevel loglvl=RTMP_LOGDEBUG;
		//RTMP_LogSetLevel(loglvl);

		RTMP *rtmp = RTMP_Alloc();
		RTMP_Init(rtmp);
		//set connection timeout,default 30s
		rtmp->Link.timeout = 30;
		// HKS's live URL
		if(!RTMP_SetupURL(rtmp,"rtmp://live.hkstv.hk.lxdns.com/live/hks"))
		//if(!RTMP_SetupURL(rtmp,"rtmp://localhost/live/hks"))
		//if (!RTMP_SetupURL(rtmp, "rtmp://localhost:1935/flvplayback/mp4:2.mp4"))//rtmp://localhost:1935/flvplayback/mp4:2.mp4
		{
			RTMP_Log(RTMP_LOGERROR, "SetupURL Err\n");
			RTMP_Free(rtmp);
			CleanupSockets();
			return -1;
		}
		if (bLiveStream){
			rtmp->Link.lFlags |= RTMP_LF_LIVE;
		}

		//1hour
		RTMP_SetBufferMS(rtmp, 3600 * 1000);

		if (!RTMP_Connect(rtmp, NULL)){
			RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
			RTMP_Free(rtmp);
			CleanupSockets();
			return -1;
		}

		if (!RTMP_ConnectStream(rtmp, 0)){
			RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
			RTMP_Close(rtmp);
			RTMP_Free(rtmp);
			CleanupSockets();
			return -1;
		}

		RTMPPacket pc = { 0 };// ps = { 0 };
		bool bVideoFirst = true;
		bool bAudioFirst = true;
		unsigned int nFrameType = 0;
		unsigned char result = 0;
		char *data = NULL;
		//FLV文件头
		static const char flvHeader[] = { 'F', 'L', 'V', 0x01,
			0x00,				/* 0x04代表有音频, 0x01代表有视频 */
			0x00, 0x00, 0x00, 0x09,
			0x00, 0x00, 0x00, 0x00
		};
		//fwrite(flvHeader, sizeof(flvHeader), 1, fp);
		int jjj = 0;
		AudioSpecificConfig ascAudioSpecificConfig = { 0 };
		while (RTMP_ReadPacket(rtmp, &pc))
		{
			if (RTMPPacket_IsReady(&pc))	//是否读取完毕。((a)->m_nBytesRead == (a)->m_nBodySize)  
			{
				if (!pc.m_nBodySize)
					continue;
				if (pc.m_packetType == RTMP_PACKET_TYPE_VIDEO && RTMP_ClientPacket(rtmp, &pc))
				{
					jjj++;
					if (jjj > 1000)
						break;
					result = pc.m_body[0];
					data = pc.m_body;
					bool bIsKeyFrame = false;
					if (result == 0x17)//I frame
					{
						bIsKeyFrame = true;
					}
					else if (result == 0x27)
					{
						bIsKeyFrame = false;
					}
					static unsigned char const start_code[4] = { 0x00, 0x00, 0x00, 0x01 };
					//fwrite(start_code, 1, 4, fp);
					//int ret = fwrite(pc.m_body + 9, 1, pc.m_nBodySize-9, pf);
					if (bVideoFirst)
					{
						fwrite(start_code, 1, 4, fp);
						//AVCsequence header
						//ioBuffer.put(foredata);
						//Access to SPS
						int spsnum = data[10] & 0x1f;
						int number_sps = 11;
						int count_sps = 1;
						while (count_sps <= spsnum){
							int spslen = (data[number_sps] & 0x000000FF) << 8 | (data[number_sps + 1] & 0x000000FF);
							number_sps += 2;
							fwrite(data + number_sps, 1, spslen, fp);
							fwrite(start_code, 1, 4, fp);
							number_sps += spslen;
							count_sps++;
						}
						//Get PPS
						int ppsnum = data[number_sps] & 0x1f;
						int number_pps = number_sps + 1;
						int count_pps = 1;
						while (count_pps <= ppsnum){
							int ppslen = (data[number_pps] & 0x000000FF) << 8 | data[number_pps + 1] & 0x000000FF;
							number_pps += 2;
							fwrite(data + number_pps, 1, ppslen, fp);
							fwrite(start_code, 1, 4, fp);
							number_pps += ppslen;
							count_pps++;
						}
						bVideoFirst = false;
					}
					else
					{
						//AVCNALU
						int len = 0;
						int num = 5;
						while (num<pc.m_nBodySize)
						{
							len = (data[num] & 0x000000FF) << 24 | (data[num + 1] & 0x000000FF) << 16 | (data[num + 2] & 0x000000FF) << 8 | data[num + 3] & 0x000000FF;
							num = num + 4;
							fwrite(data + num, 1, len, fp);
							fwrite(start_code, 1, 4, fp);
							num = num + len;
						}
					}
				}
				else if (pc.m_packetType == RTMP_PACKET_TYPE_AUDIO && RTMP_ClientPacket(rtmp, &pc))
				{
					data = pc.m_body;
					if (bAudioFirst)
					{
						//ACC音频同步包(1bytes+3bytes AccAudioData) (4bytes 包含了AACDecoderSpecific和AudioSpecificConfig)
						AACDecoderSpecific adsAACDecoderSpecific = { 0 };
						adsAACDecoderSpecific.nAudioFortmatType = (data[0] & 0xf0) >> 4;	//音频编码类型（0：Liner PCM platform endian，1：PCM，2：mp3，4：Nellymoser 16-kHz mono,5:Nellymoser 8-kHz mono,6:Nellymoser,7:G.711 A-law logarithmic PCM,8:G.711 mu-law logarithmic PCM,9:reserved,10: AAC,14:MP3 8-Khz,15:Device-specific sound）
						adsAACDecoderSpecific.nAudioSampleType = (data[0] & 0x0c) >> 2;	//音频采样率(0:5.5kHz,1:11KHz,2:22 kHz,3:44 kHz)
						adsAACDecoderSpecific.nAudioSizeType = (data[0] & 0x02) >> 1;	//音频采样精度(0:8bits,1:16bits)
						adsAACDecoderSpecific.nAudioStereo = data[0] & 0x01;//是否立体声(0:sndMono,1:sndStereo)
						if(adsAACDecoderSpecific.nAudioFortmatType == 10)
						{
							//The following values are defined:
							//0 = AAC sequence header
							//1 = AAC raw
							adsAACDecoderSpecific.nAccPacketType = data[1];
							unsigned short audioSpecificConfig = 0;
							audioSpecificConfig = (data[2] & 0xff) << 8;
							audioSpecificConfig += 0x00ff & data[3];
							ascAudioSpecificConfig.nAudioObjectType = (audioSpecificConfig & 0xF800) >> 11;
							ascAudioSpecificConfig.nSampleFrequencyIndex = (audioSpecificConfig & 0x0780) >> 7;
							ascAudioSpecificConfig.nChannels = (audioSpecificConfig & 0x78) >> 3;
							ascAudioSpecificConfig.nFrameLengthFlag = (audioSpecificConfig & 0x04) >> 2;
							ascAudioSpecificConfig.nDependOnCoreCoder = (audioSpecificConfig & 0x02) >> 1;
							ascAudioSpecificConfig.nExtensionFlag = audioSpecificConfig & 0x01;
						}
						else if(adsAACDecoderSpecific.nAudioFortmatType == 11)
						{
							//speex类型数据时，后面的4位数据不起作用，固定的是16KHZ，单声道，16bit/sample
							adsAACDecoderSpecific.nAudioStereo = 0;
							adsAACDecoderSpecific.nAudioSizeType = 1;
							adsAACDecoderSpecific.nAudioSampleType = 4;
						}

						bAudioFirst = false;
					}
					else
					{
						if(pc.m_nBodySize > 2)
						{
							if(data[1] == 1)
							{
								//raw data 获取 audio payload
								//写ADTS数据到文件
								char szADTSTemp[8] = { 0 };
								CreateADTS(ascAudioSpecificConfig, pc.m_nBodySize - 2 + 7, szADTSTemp);
								fwrite(szADTSTemp, 7, 1, fpAAC);
								//写raw数据到文件
								fwrite(pc.m_body + 2, pc.m_nBodySize - 2, 1, fpAAC);
							}
							else
							{
								int kkkkk = 0;
							}
						}
						else
						{
							int kkkk = 0;
						}
					}
						
					//The actual audio content for the pkt-> m_body+1, size is pkt-> m_nBodySize-1. Here is the voice of Speex code.
				}
				else if (pc.m_packetType == RTMP_PACKET_TYPE_INFO && RTMP_ClientPacket(rtmp, &pc))
				{
					int nVideoCodecId = 0;
					int nVideoWidth = 255;
					int nVideoHeight = 188;
					int nVideoFrameRate = 25;
					int nAudioCodecId = 0;
					int nAudioSampleRate = 0;
					int nAudioSampleSize = 0;
					bool bStereo = false;			//立体声
					int nFileSize = 0;
					ParseScriptTag(pc.m_body, pc.m_nBodySize, nVideoCodecId, nVideoWidth, nVideoHeight, nVideoFrameRate, nAudioCodecId, nAudioSampleRate, nAudioSampleSize, bStereo, nFileSize);
					int k = 0;
				}
				RTMPPacket_Free(&pc);
			}
		}

		if (fp)
			fclose(fp);

		if (fpAAC)
			fclose(fpAAC);

		if (buf){
			free(buf);
		}

		if (rtmp){
			RTMP_Close(rtmp);
			RTMP_Free(rtmp);
			CleanupSockets();
			rtmp = NULL;
		}
	}

	return 0;
}
```

注：H.264视频分析软件(Elecard.Streameye.Tools)分析H264数据，使用暴风影音播放m4a文件中的aac音频。

代码下载：[点击链接](http://download.csdn.net/detail/byxdaz/9727585)

参考资料：
[http://blog.csdn.net/leixiaohua1020/article/details/42104893](http://blog.csdn.net/leixiaohua1020/article/details/42104893)
[http://blog.chinaunix.net/uid-15063109-id-4273162.html](http://blog.chinaunix.net/uid-15063109-id-4273162.html)
[http://blog.csdn.net/bsplover/article/details/7426511](http://blog.csdn.net/bsplover/article/details/7426511)
[http://blog.csdn.net/zqf_office/article/details/50868520](http://blog.csdn.net/zqf_office/article/details/50868520)

