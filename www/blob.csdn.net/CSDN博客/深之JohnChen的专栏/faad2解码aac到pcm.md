# faad2解码aac到pcm - 深之JohnChen的专栏 - CSDN博客

2017年01月03日 16:51:26[byxdaz](https://me.csdn.net/byxdaz)阅读数：4411


```cpp
//解码AAC音频为PCM数据

#pragma once
#include "libfaad2/include/faad.h"

//aac数据状态
typedef enum AccDataStatus
{
	AccDataStatus_NotKnown			=		0x00,			//未知
	AccDataStatus_InValid			=		0x01,			//非法
	AccDataStatus_Valid				=		0x02,			//合法
};

class AAC2PCM
{
public:
	AAC2PCM(void);
	virtual ~AAC2PCM(void);

/**
 * fetch one ADTS frame
 */
int get_one_ADTS_frame(unsigned char* buffer, size_t buf_size, unsigned char* data ,size_t* data_size);

//初始化
int init(unsigned char defObjectType=2,unsigned long defSampleRate = 8000);

//反初始化
int uninit();

//检测数据是否合法
int detectFirstPackageData(unsigned char* bufferAAC, size_t buf_sizeAAC);

//获取第一数据包状态
int getFirstPackageAccDataStatus();
//重置第一数据包状态
void clearFirstPackageAccDataStatus(int nAccDataStatus);

//转换
int convert(unsigned char* bufferAAC, size_t buf_sizeAAC,unsigned char* bufferPCM, size_t & buf_sizePCM);
int convert2(unsigned char* bufferAAC, size_t buf_sizeAAC, unsigned char* bufferPCM, size_t & buf_sizePCM);

protected:
	unsigned long samplerate;
	unsigned char channels;
	NeAACDecHandle decoder;
	NeAACDecFrameInfo frame_info;
	int m_nFirstPackageAccDataStatus;		//第一数据包状态
	bool m_bNeAACDecInit;
};
```

```cpp
#include "AAC2PCM.h"
#include <stdio.h>
#include <memory.h>
#include "libsamplerate/samplerate.h"
#pragma comment(lib,"libfaad2/lib/libfaad2.lib")
#pragma comment(lib,"libsamplerate/libsamplerate.lib")
#define FRAME_MAX_LEN 1024*5 
#define BUFFER_MAX_LEN 1024*1024

static unsigned char frame[FRAME_MAX_LEN] = {0};
unsigned int framesize = FRAME_MAX_LEN;
//static unsigned char buffer[BUFFER_MAX_LEN] = {0};

AAC2PCM::AAC2PCM(void)
{
	samplerate = 1;
	channels = 1;
	decoder = 0;
	m_nFirstPackageAccDataStatus = AccDataStatus_NotKnown;
}

AAC2PCM::~AAC2PCM(void)
{
}

/**
 * fetch one ADTS frame
 */
int AAC2PCM::get_one_ADTS_frame(unsigned char* buffer, size_t buf_size, unsigned char* data ,size_t* data_size)
{
    size_t size = 0;

    if(!buffer || !data || !data_size )
    {
        return -1;
    }

    while(1)
    {
        if(buf_size  < 7 )
        {
            return -1;
        }

		if ((buffer[0] == 0xff) && ((buffer[1] & 0xf0) == 0xf0))
		{
			// profile; 2 uimsbf
			// sampling_frequency_index; 4 uimsbf
			// private_bit; 1 bslbf
			// channel_configuration; 3 uimsbf
			// original/copy; 1 bslbf
			// home; 1 bslbf
			// copyright_identification_bit; 1 bslbf
			// copyright_identification_start; 1 bslbf
			// frame_length; 13 bslbf

			size |= (((buffer[3] & 0x03)) << 11);//high 2 bit
			size |= (buffer[4] << 3);//middle 8 bit
			size |= ((buffer[5] & 0xe0) >> 5);//low 3bit

			printf("len1=%x\n", (buffer[3] & 0x03));
			printf("len2=%x\n", buffer[4]);
			printf("len3=%x\n", (buffer[5] & 0xe0) >> 5);
			printf("size=%d\r\n", (int)size);
			break;
		}
        --buf_size;
        ++buffer;
    }

    if(buf_size < size)
    {
        return -1;
    }

    memcpy(data, buffer, size);
    *data_size = size;
    
    return 0;
}

//初始化
int AAC2PCM::init(unsigned char defObjectType,unsigned long defSampleRate)
{
	m_nFirstPackageAccDataStatus = AccDataStatus_NotKnown;
	//open decoder
	decoder = NeAACDecOpen();
	NeAACDecConfigurationPtr conf = NeAACDecGetCurrentConfiguration(decoder);
	/*conf->defObjectType = defObjectType;
	conf->defSampleRate = defSampleRate;
	conf->outputFormat = FAAD_FMT_16BIT ; //
	*/
	conf->defObjectType = LC;
	conf->defSampleRate = 8000; //real samplerate/2
	conf->outputFormat = FAAD_FMT_16BIT; //
	conf->dontUpSampleImplicitSBR = 1;
	unsigned char nRet = NeAACDecSetConfiguration(decoder, conf);
	m_bNeAACDecInit = false;

	return 0;
}

//反初始化
int AAC2PCM::uninit()
{
	NeAACDecClose(decoder);
	return 0;
}

//检测数据是否合法
int AAC2PCM::detectFirstPackageData(unsigned char* bufferAAC, size_t buf_sizeAAC)
{
	size_t size = 0;
	if(get_one_ADTS_frame(bufferAAC, buf_sizeAAC, frame, &size) < 0)
	{
		m_nFirstPackageAccDataStatus = AccDataStatus_InValid;
		return -1;
	}
	m_nFirstPackageAccDataStatus  = AccDataStatus_Valid;
	return 0;
}

//获取第一数据包状态
int AAC2PCM::getFirstPackageAccDataStatus()
{
	return m_nFirstPackageAccDataStatus;
}
//重置第一数据包状态
void AAC2PCM::clearFirstPackageAccDataStatus(int nAccDataStatus)
{
	m_nFirstPackageAccDataStatus = nAccDataStatus;
}

//转换
int AAC2PCM::convert(unsigned char* bufferAAC, size_t buf_sizeAAC,unsigned char* bufferPCM, size_t & buf_sizePCM)
{
	if (m_nFirstPackageAccDataStatus != AccDataStatus_Valid)
		return -1;

	size_t size = 0;
	unsigned char* pcm_data = NULL;
	while(get_one_ADTS_frame(bufferAAC, buf_sizeAAC, frame, &size) == 0)
	{
		// printf("frame size %d\n", size);
		//decode ADTS frame
		pcm_data = (unsigned char*)NeAACDecDecode(decoder, &frame_info, frame, size);

		if(frame_info.error > 0)
		{
			printf("%s\n",NeAACDecGetErrorMessage(frame_info.error));            
			return -1;
		}
		else if(pcm_data && frame_info.samples > 0)
		{
			printf("frame info: bytesconsumed %d, channels %d, header_type %d\
				   object_type %d, samples %d, samplerate %d\n", 
				   frame_info.bytesconsumed, 
				   frame_info.channels, frame_info.header_type, 
				   frame_info.object_type, frame_info.samples, 
				   frame_info.samplerate);

			buf_sizePCM = frame_info.samples * frame_info.channels;
			/*
			//从双声道的数据中提取单通道  
			for (int i = 0, j = 0; i<4096 && j<2048; i += 4, j += 2)
			{
				bufferPCM[j] = pcm_data[i];
				bufferPCM[j + 1] = pcm_data[i + 1];
			}
			*/
			memcpy(bufferPCM,pcm_data,buf_sizePCM);
		}        
		bufferAAC -= size;
		buf_sizeAAC += size;
	}

	return 0;
}

int AAC2PCM::convert2(unsigned char* bufferAAC, size_t buf_sizeAAC, unsigned char* bufferPCM, size_t & buf_sizePCM)
{
	unsigned char* pcm_data = NULL;
	if (!m_bNeAACDecInit)
	{
		//initialize decoder
		NeAACDecInit(decoder, bufferAAC, buf_sizeAAC, &samplerate, &channels);
		printf("samplerate %d, channels %d\n", samplerate, channels);
		m_bNeAACDecInit = true;
	}
	//decode ADTS frame
	pcm_data = (unsigned char*)NeAACDecDecode(decoder, &frame_info, bufferAAC, buf_sizeAAC);

	if (frame_info.error > 0)
	{
		printf("%s\n", NeAACDecGetErrorMessage(frame_info.error));
		return -1;
	}
	else if (pcm_data && frame_info.samples > 0)
	{
		printf("frame info: bytesconsumed %d, channels %d, header_type %d\
			   				   object_type %d, samples %d, samplerate %d\n",
							   frame_info.bytesconsumed,
							   frame_info.channels, frame_info.header_type,
							   frame_info.object_type, frame_info.samples,
							   frame_info.samplerate);

		buf_sizePCM = frame_info.samples * frame_info.channels;
		/*
		//从双声道的数据中提取单通道  
		for (int i = 0, j = 0; i<4096 && i<buf_sizePCM && j<2048; i += 4, j += 2)
		{
			bufferPCM[j] = pcm_data[i];
			bufferPCM[j + 1] = pcm_data[i + 1];
		}
		*/
		memcpy(bufferPCM,pcm_data,buf_sizePCM);
		/*
		float in[4096] = { 0 };
		float out[4096] = { 0 };
		for (int j = 0; j < 4096 && j < buf_sizePCM; j++)
		{
			in[j] = pcm_data[j];
		}
		SRC_DATA dataResample;
		dataResample.data_in = in;
		dataResample.data_out = out;
		dataResample.input_frames = frame_info.samples;
		dataResample.output_frames = frame_info.samples;
		dataResample.src_ratio =  8000.0/frame_info.samplerate;
		int nRetResample = src_simple(&dataResample, SRC_SINC_FASTEST, 2);
		buf_sizePCM = dataResample.output_frames_gen * frame_info.channels;
		memcpy(bufferPCM, dataResample.data_out, buf_sizePCM);
		*/

		return 0;
	}

	return -1;
}
```

注意事项：

1，解码aac-解决采样频率和通道数不对的问题。

//防止采样频率加倍
NeAACDecConfigurationPtr conf = NeAACDecGetCurrentConfiguration(decoder);

conf->dontUpSampleImplicitSBR = 1;

NeAACDecSetConfiguration(decoder, conf);


//从双声道的数据中提取单通道  
for(i=0,j=0; i<4096 && j<2048; i+=4, j+=2)

{
frame_mono[j]=pcm_data[i];
frame_mono[j+1]=pcm_data[i+1];
}

２，采样率转换，使用libsamplerate

FAAC和FAAD的下载地址  [http://www.audiocoding.com/downloads.html](http://www.audiocoding.com/downloads.html)

