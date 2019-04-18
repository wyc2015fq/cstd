# asf文件转成mp4 - 深之JohnChen的专栏 - CSDN博客

2018年12月07日 17:04:20[byxdaz](https://me.csdn.net/byxdaz)阅读数：215


//asfToMp4.h

```
#pragma once

#include <windows.h>
#include "Audio/AudioFrameBuffer.h"
#include "AsfFile.h"								
#include "audio/G726EnDecoder.h"
#include "mp4v2/mp4v2.h"
#include "libfaac/include/faac.h"

#ifdef _DEBUG
#pragma  comment(lib,"libmp4v2/lib/Debug/libmp4v2.lib")
#pragma  comment(lib,"libfaac/lib/libfaacD.lib")
#else
#pragma  comment(lib,"libmp4v2/lib/Release/libmp4v2.lib")
#pragma  comment(lib,"libfaac/lib/libfaac.lib")
#endif

//文件转换进度类型
enum Mp4FileConvertProcessType
{
	Mp4FileConvertProcessType_None		=	0,			//none
	Mp4FileConvertProcessType_Running	=	1,			//转换中
	Mp4FileConvertProcessType_Finish	=	2,			//转换完成
	Mp4FileConvertProcessType_Exception	=	3,			//转换异常
	Mp4FileConvertProcessType_Cancel	=	4,			//取消
};
//文件转换进度回调函数
typedef void (CALLBACK* Mp4FileConvertProcessCallback)(int nType, unsigned int nCurrentSeconds, unsigned int nTotalSenconds, void* pUsr);

// NALU单元    
struct AsfToMp4_NaluUnit
{
	short alltype;//0x67,0x68,0x65 分别表示sps,pps,i
	short type;
	int size;
	unsigned char *data;
};

class AsfToMp4
{
public:
	AsfToMp4();
	virtual ~AsfToMp4();

	//设置文件路径
	void SetFilePathName(char *pSrc, char *pDest);

	void SetFrameRate(int nFrameRate);

	BOOL StartConvert(BOOL bIsAudio, BOOL bIsBlock, Mp4FileConvertProcessCallback funFileConvert,void *pUserData);
	BOOL StopConvert();

	static unsigned __stdcall ConvertThreadFun(LPVOID pParam);
	unsigned int ConvertThreadFun_Inner();

	//---------------------------------------------------------------------
	//说明：从文件中读取数据
	//参数：无
	//返回：成功返回TRUE，失败返回FALSE
	//---------------------------------------------------------------------
	BOOL	ReadFileData(char *pOutData,int & nOutDataSize, int & FrameType);

	//---------------------------------------------------------------------
	//说明：取得I帧的偏移长度，I帧中可能存在一定的无效数据，因此需要删除
	//参数：[in]stream:帧数据
	//		[in]iLen:数据长度
	//返回：返回I帧的偏移长度
	//调用：ChangeFile()中调用
	//---------------------------------------------------------------------
	int		GetPFrameOffset(unsigned char *stream, int iLen);

	//---------------------------------------------------------------------
	//说明：取得I帧的偏移长度，I帧中可能存在一定的无效数据，因此需要删除
	//参数：[in]stream:帧数据
	//		[in]iLen:数据长度
	//返回：返回I帧的偏移长度
	//调用：ChangeFile()中调用
	//---------------------------------------------------------------------
	int		GetAFrameOffset(unsigned char *stream, int iLen);

private:
	// 获取Asf文件头信息结构体。
	BOOL		GetAsfFileInfo(ASF_INFO_S *struAsfInfo);

	// read one nalu from H264 data buffer        
	int ReadOneNaluFromBuf(const unsigned char *buffer, unsigned int nBufferSize, unsigned int offSet, AsfToMp4_NaluUnit &nalu);

private:
	char m_szSrcFilePathName[MAX_PATH];
	char m_szDestFilePathName[MAX_PATH];
	CG726EnDecoder		m_G726EnDecoder;
	CAsfFile		m_AsfFile;					//asf文件读取类
	BOOL				m_bFileOver;				//是否文件结束
	ASF_INFO_S			m_AsfFileHead;	// asf文件头信息
	int					m_width;		// 画面宽度
	int					m_height;		// 画面高度
	int					m_nFrameRate;
	int					m_nTimeScale;

	BOOL				m_bIsAudio;						// 是否存在音频
	char *				m_pPcmDataBuffer;
	int					m_nPcmDataBufferSize;
	char *				m_pAacDataBuffer;
	int					m_nAacDataBufferSize;
	int					m_nAacMaxInputBytes;			//aac编码输入字节数（一帧数据）
	int					m_nAacMaxOutputBytes;			//aac编码输出字节数（一帧数据）
	char *				m_pTempDataBuffer;
	int					m_nTempDataBufferSize;
	int					m_nTempDataStartPos;
	faacEncHandle		m_faacEncHandle;
	BOOL				m_bInitFaac;

	BOOL				m_bIsAddH264VideoTrack;
	BOOL				m_bIsAddH264PictureParameterSet;
	BOOL				m_bIsAddAudioTrack;
	MP4FileHandle m_fileMp4;
	MP4TrackId m_videoTrack;
	MP4TrackId m_audioTrack;

protected:
	Mp4FileConvertProcessCallback m_funFileConvert;
	void *m_pUserData;
	BOOL				m_bIsStopConvert;
	HANDLE				m_hConvertThread;
};
```

//asfToMp4.cpp

```
#include "stdafx.h"
#include "AsfToMp4.h"
#include <process.h>

AsfToMp4::AsfToMp4()
{
	m_bIsAudio = FALSE;
	m_nFrameRate = 25;
	m_nTimeScale = 90000;
	memset(m_szSrcFilePathName, 0, sizeof(m_szSrcFilePathName));
	memset(m_szDestFilePathName, 0, sizeof(m_szDestFilePathName));
	m_bIsAddH264VideoTrack = FALSE;
	m_bIsAddH264PictureParameterSet = FALSE;
	m_bIsAddAudioTrack = FALSE;
	m_fileMp4 = NULL;
	m_videoTrack = 0;
	m_audioTrack = 0;
	m_nPcmDataBufferSize = 0;
	m_pPcmDataBuffer = NULL;
	m_nAacDataBufferSize = 0;
	m_pAacDataBuffer = NULL;
	m_nAacMaxInputBytes = 0;
	m_nAacMaxOutputBytes = 0;
	m_nTempDataBufferSize = 0;
	m_pTempDataBuffer = NULL;
	m_nTempDataStartPos = 0;
	m_faacEncHandle = NULL;
	m_bInitFaac = FALSE;

	m_funFileConvert = NULL;
	m_pUserData = NULL;
	m_bIsStopConvert = TRUE;
	m_hConvertThread = NULL;
}

AsfToMp4::~AsfToMp4()
{
	if (m_pPcmDataBuffer != NULL)
	{
		delete[]m_pPcmDataBuffer;
		m_pPcmDataBuffer = NULL;
	}
	if (m_pAacDataBuffer != NULL)
	{
		delete[]m_pAacDataBuffer;
		m_pAacDataBuffer = NULL;
	}
	if (m_pTempDataBuffer != NULL)
	{
		delete[]m_pTempDataBuffer;
		m_pTempDataBuffer = NULL;
	}
}

//设置文件路径
void AsfToMp4::SetFilePathName(char *pSrc, char *pDest)
{
	strcpy_s(m_szSrcFilePathName, pSrc);
	strcpy_s(m_szDestFilePathName, pDest);
}

void AsfToMp4::SetFrameRate(int nFrameRate)
{
	if (nFrameRate > 0)
	{
		m_nFrameRate = nFrameRate;
	}
}

unsigned __stdcall AsfToMp4::ConvertThreadFun(LPVOID pParam)
{
	AsfToMp4 *pAsfToMp4 = (AsfToMp4 *)pParam;
	return pAsfToMp4->ConvertThreadFun_Inner();
}
unsigned int AsfToMp4::ConvertThreadFun_Inner()
{
	BOOL bRet = TRUE;
	std::string strUtf8FileName = AsciiToUtf8(m_szDestFilePathName);
	m_fileMp4 = MP4CreateEx(strUtf8FileName.c_str(), 0, 1, 1, 0, 0, 0, 0);
	if (m_fileMp4 == MP4_INVALID_FILE_HANDLE)
	{
		printf("open file fialed.\n");
		if (m_funFileConvert != NULL)
		{
			m_funFileConvert(Mp4FileConvertProcessType_Exception, 0, 0, m_pUserData);
		}
		return 0;
	}
	unsigned int uiTotalSeconds = 0;
	unsigned int uiCurrentSeconds = 0;
	unsigned long long ullBeginTimestamp = 0;
	unsigned long inputSamples = 0;
	int nAudioChannelNum = 1;
	int nMP4AudioDuration = 1024;			//音频时间刻度
	if (m_bIsAudio)
	{
		if (!m_bInitFaac)
		{
			//init faac
			int nBitsPerSample = 16;
			inputSamples = 0;
			m_faacEncHandle = faacEncOpen(8000, nAudioChannelNum, &inputSamples, (unsigned long *)&m_nAacMaxOutputBytes);
			m_nAacMaxInputBytes = inputSamples*nBitsPerSample / 8;
			m_nPcmDataBufferSize = m_nAacMaxInputBytes;
			nMP4AudioDuration = inputSamples / nAudioChannelNum;

			m_pPcmDataBuffer = new char[m_nPcmDataBufferSize + 1];
			m_nAacDataBufferSize = m_nAacMaxOutputBytes;
			m_pAacDataBuffer = new char[m_nAacMaxOutputBytes + 1];
			m_nTempDataBufferSize = 1024 * 8;
			m_pTempDataBuffer = new char[m_nTempDataBufferSize + 1];
			m_nTempDataStartPos = 0;
			// Get current encoding configuration
			faacEncConfigurationPtr pConfiguration = faacEncGetCurrentConfiguration(m_faacEncHandle);
			if (!pConfiguration)
			{
				printf("GetCurrentConfiguration error!\n");
				if (m_funFileConvert != NULL)
				{
					m_funFileConvert(Mp4FileConvertProcessType_Exception, 0, 0, m_pUserData);
				}
				return 0;
			}
			//设置版本,录制MP4文件时要用MPEG4
			pConfiguration->version = MPEG4;
			pConfiguration->aacObjectType = LOW; //LC编码
												 //输入数据类型
			pConfiguration->inputFormat = FAAC_INPUT_16BIT;
			// outputFormat (0 = Raw; 1 = ADTS)
			// 录制MP4文件时，要用raw流。检验编码是否正确时可设置为 adts传输流，
			pConfiguration->outputFormat = 1;
			//瞬时噪声定形(temporal noise shaping，TNS)滤波器
			pConfiguration->shortctl = SHORTCTL_NORMAL;
			pConfiguration->useTns = true;
			//pConfiguration->useLfe=false;
			pConfiguration->quantqual = 100;
			pConfiguration->bandWidth = 0;
			pConfiguration->bitRate = 0;
			//  Set encoding configuration
			faacEncSetConfiguration(m_faacEncHandle, pConfiguration);

			m_bInitFaac = TRUE;
		}
	}
	MP4SetTimeScale(m_fileMp4, m_nTimeScale);
	int nDataBufferSize = 1920 * 1080 * 3;
	int nWriteSampleDataSize = 1920 * 1080 * 2;
	char * pWriteSampleData = new char[nWriteSampleDataSize + 1];
	int nFrameType = 0;
	int nTempDataBufferSize = nDataBufferSize;
	char *pDataBuffer = new char[nDataBufferSize + 1];
	unsigned long long ullTimeStamp = 0;
	unsigned int uiProcessIndex = 0;
	while (true)
	{
		nTempDataBufferSize = nDataBufferSize;
		bRet = ReadFileData(pDataBuffer, nTempDataBufferSize, nFrameType);
		if (!bRet)
		{
			break;
		}
		REPAIRFILEFRAMEINFO *pFrameInfo = (REPAIRFILEFRAMEINFO *)pDataBuffer;
		if (pFrameInfo->FrameType == 0x63643030 || pFrameInfo->FrameType == 0x63643130)
		{
			//I或P帧
			AsfToMp4_NaluUnit nalu;
			int pos = 0, len = 0;
			pos = 0;
			nalu.data = NULL;
			nalu.size = 0;
			nalu.type = 0;
			while (len = ReadOneNaluFromBuf((unsigned char *)pDataBuffer + sizeof(REPAIRFILEFRAMEINFO), nTempDataBufferSize - sizeof(REPAIRFILEFRAMEINFO), pos, nalu))
			{
				if (nalu.type == 0x07) // sps          
				{
					if (!m_bIsAddH264VideoTrack)
					{
						//获取视频分辨率
						ASF_INFO_S aiAsfInfo = m_AsfFile.GetAsfFileInfo();
						uiTotalSeconds = aiAsfInfo.struDuration.s32Hours * 3600 + aiAsfInfo.struDuration.s32Mins * 60 + aiAsfInfo.struDuration.s32Secs;
						ullBeginTimestamp = pFrameInfo->FrameStamp;
						if (aiAsfInfo.struVStream.u32samplePerSec == 0)
						{
							m_nFrameRate = m_AsfFile.GetFrameRateInVBR();
						}
						else
						{
							m_nFrameRate = aiAsfInfo.struAStream.u32SamplesPerSec;
						}
						//添加h264 track
						m_videoTrack = MP4AddH264VideoTrack(m_fileMp4,
							m_nTimeScale,
							m_nTimeScale / m_nFrameRate,
							aiAsfInfo.struVStream.u32BiWidth,
							aiAsfInfo.struVStream.u32BiHeight,
							nalu.data[1], // sps[1] AVCProfileIndication                   
							nalu.data[2], // sps[2] profile_compat                    
							nalu.data[3], // sps[3] AVCLevelIndication                    
							3);
						// 4 bytes length before each NAL unit                
						if (m_videoTrack == MP4_INVALID_TRACK_ID)
						{
							printf("add video track failed.\n");
							MP4Close(m_fileMp4);
							m_fileMp4 = NULL;
							if (pWriteSampleData != NULL)
							{
								delete[]pWriteSampleData;
								pWriteSampleData = NULL;
							}
							if (pDataBuffer != NULL)
							{
								delete[]pDataBuffer;
								pDataBuffer = NULL;
							}
							return FALSE;
						}
						MP4SetVideoProfileLevel(m_fileMp4, 1);
						//  Simple Profile @ Level 3                    
						MP4AddH264SequenceParameterSet(m_fileMp4, m_videoTrack, nalu.data, nalu.size);

						m_bIsAddH264VideoTrack = TRUE;
					}
					else
					{
						int jjj = 0;
					}
				}
				else if (nalu.type == 0x08) // pps            
				{
					if (!m_bIsAddH264PictureParameterSet)
					{
						if (m_bIsAddH264VideoTrack)
						{
							MP4AddH264PictureParameterSet(m_fileMp4, m_videoTrack, nalu.data, nalu.size);
							m_bIsAddH264PictureParameterSet = TRUE;
						}
					}
				}
				else
				{
					int datalen = nalu.size + 4;
					if (datalen > nWriteSampleDataSize)
					{
						delete[]pWriteSampleData;
						nWriteSampleDataSize = datalen;
						pWriteSampleData = new char[nWriteSampleDataSize + 1];
					}
					if (pWriteSampleData != NULL)
					{
						unsigned char *data = (unsigned char *)pWriteSampleData;
						// MP4 Nalu前四个字节表示Nalu长度                
						data[0] = nalu.size >> 24;
						data[1] = nalu.size >> 16;
						data[2] = nalu.size >> 8;
						data[3] = nalu.size & 0xff;
						memcpy(data + 4, nalu.data, nalu.size);
						if (!MP4WriteSample(m_fileMp4, m_videoTrack, data, datalen, MP4_INVALID_DURATION, 0, 1))
						{
							MP4Close(m_fileMp4);
							m_fileMp4 = NULL;
							if (pWriteSampleData != NULL)
							{
								delete[]pWriteSampleData;
								pWriteSampleData = NULL;
							}
							if (pDataBuffer != NULL)
							{
								delete[]pDataBuffer;
								pDataBuffer = NULL;
							}
							if (m_funFileConvert != NULL)
							{
								m_funFileConvert(Mp4FileConvertProcessType_Exception, 0, 0, m_pUserData);
							}
							return 0;
						}
					}
				}
				pos += len;
			}
		}
		else if (pFrameInfo->FrameType == 0x62773130)
		{
			//音频
			if (m_bIsAudio)
			{
				if (!m_bIsAddAudioTrack)
				{
					//添加aac音频
					m_audioTrack = MP4AddAudioTrack(m_fileMp4, 8000, nMP4AudioDuration, MP4_MPEG4_AUDIO_TYPE);
					if (m_audioTrack == MP4_INVALID_TRACK_ID)
					{
						printf("add audio track failed.\n");
						MP4Close(m_fileMp4);
						m_fileMp4 = NULL;
						if (m_funFileConvert != NULL)
						{
							m_funFileConvert(Mp4FileConvertProcessType_Exception, 0, 0, m_pUserData);
						}
						return 0;
					}
					MP4SetAudioProfileLevel(m_fileMp4, 0x2);
					unsigned long uLen = 0;
					unsigned char *apInfo = NULL;
					faacEncGetDecoderSpecificInfo(m_faacEncHandle, &apInfo, &uLen);
					MP4SetTrackESConfiguration(m_fileMp4, m_audioTrack, (unsigned char*)apInfo, uLen);
					if (apInfo != NULL)
					{
						delete apInfo;
					}
					m_bIsAddAudioTrack = TRUE;
				}
				if (m_bIsAddAudioTrack)
				{
					if (m_bInitFaac)
					{
						//转换g726转换aac
						char szPcmBuffer[2048] = { 0 };
						int nPcmBufferSize = 2048;
						BOOL bAudioFrameDecodeRet = m_G726EnDecoder.Decoder_DecodeFrame(pDataBuffer + sizeof(REPAIRFILEFRAMEINFO), nTempDataBufferSize - sizeof(REPAIRFILEFRAMEINFO), szPcmBuffer, &nPcmBufferSize);
						if (bAudioFrameDecodeRet)
						{
							memcpy(m_pTempDataBuffer + m_nTempDataStartPos, szPcmBuffer, nPcmBufferSize);
							m_nTempDataStartPos += nPcmBufferSize;
							if (m_nTempDataStartPos >= m_nAacMaxInputBytes)
							{
								memcpy(m_pPcmDataBuffer, m_pTempDataBuffer, m_nAacMaxInputBytes);
								//move data
								int nRemainSize = m_nTempDataStartPos - m_nAacMaxInputBytes;
								int j = 0;
								for (j = 0; j < nRemainSize; j++)
								{
									m_pTempDataBuffer[j] = m_pTempDataBuffer[m_nAacMaxInputBytes + j];
								}
								m_nTempDataStartPos -= m_nAacMaxInputBytes;
								int nRetFaacEncode = faacEncEncode(m_faacEncHandle, (int *)m_pPcmDataBuffer, inputSamples, (unsigned char *)m_pAacDataBuffer, m_nAacMaxOutputBytes);
								if (nRetFaacEncode > 0)
								{
									bool bMP4WriteSample = MP4WriteSample(m_fileMp4, m_audioTrack, (BYTE*)&m_pAacDataBuffer[7], nRetFaacEncode - 7, MP4_INVALID_DURATION, 0, true);
								}
								else if (nRetFaacEncode == 0)
								{
									//缓存处理，非错误
								}
								else
								{
									//编码失败，错误
								}
							}
						}
					}
				}
			}
		}
		else if (pFrameInfo->FrameType == 0x63643939)
		{
			//Binary
		}
		else
		{
			continue;
		}
		uiProcessIndex++;
		if (m_bIsStopConvert)
		{
			break;
		}
		else
		{
			if (m_funFileConvert != NULL)
			{
				if (ullBeginTimestamp != 0)
				{
					uiCurrentSeconds = ((pFrameInfo->FrameStamp - ullBeginTimestamp) / 1000000);
				}
				if (uiProcessIndex == 1 || uiProcessIndex % 25 == 0)
				{
					m_funFileConvert(Mp4FileConvertProcessType_Running, uiCurrentSeconds, uiTotalSeconds, m_pUserData);
				}
			}
		}
	}
	if (m_faacEncHandle != NULL)
	{
		faacEncClose(m_faacEncHandle);
		m_faacEncHandle = NULL;
	}
	MP4Close(m_fileMp4);
	m_fileMp4 = NULL;
	if (pWriteSampleData != NULL)
	{
		delete[]pWriteSampleData;
		pWriteSampleData = NULL;
	}
	if (pDataBuffer != NULL)
	{
		delete[]pDataBuffer;
		pDataBuffer = NULL;
	}
	if (!m_bIsStopConvert)
	{
		if (m_funFileConvert != NULL)
		{
			m_funFileConvert(Mp4FileConvertProcessType_Finish, uiTotalSeconds, uiTotalSeconds, m_pUserData);
		}
	}

	return 0;
}

BOOL AsfToMp4::StartConvert(BOOL bIsAudio, BOOL bIsBlock, Mp4FileConvertProcessCallback funFileConvert, void *pUserData)
{
	m_funFileConvert = funFileConvert;
	m_pUserData = pUserData;
	m_bIsAudio = bIsAudio;

	if (bIsBlock)
	{
		BOOL bRet = TRUE;
		std::string strUtf8FileName = AsciiToUtf8(m_szDestFilePathName);
		m_fileMp4 = MP4CreateEx(strUtf8FileName.c_str(), 0, 1, 1, 0, 0, 0, 0);
		if (m_fileMp4 == MP4_INVALID_FILE_HANDLE)
		{
			printf("open file fialed.\n");
			return FALSE;
		}
		unsigned long inputSamples = 0;
		int nAudioChannelNum = 1;
		int nMP4AudioDuration = 1024;			//音频时间刻度
		if (bIsAudio)
		{
			if (!m_bInitFaac)
			{
				//init faac
				int nBitsPerSample = 16;
				inputSamples = 0;
				m_faacEncHandle = faacEncOpen(8000, nAudioChannelNum, &inputSamples, (unsigned long *)&m_nAacMaxOutputBytes);
				m_nAacMaxInputBytes = inputSamples*nBitsPerSample / 8;
				m_nPcmDataBufferSize = m_nAacMaxInputBytes;
				nMP4AudioDuration = inputSamples / nAudioChannelNum;

				m_pPcmDataBuffer = new char[m_nPcmDataBufferSize + 1];
				m_nAacDataBufferSize = m_nAacMaxOutputBytes;
				m_pAacDataBuffer = new char[m_nAacMaxOutputBytes + 1];
				m_nTempDataBufferSize = 1024 * 8;
				m_pTempDataBuffer = new char[m_nTempDataBufferSize + 1];
				m_nTempDataStartPos = 0;
				// Get current encoding configuration
				faacEncConfigurationPtr pConfiguration = faacEncGetCurrentConfiguration(m_faacEncHandle);
				if (!pConfiguration)
				{
					printf("GetCurrentConfiguration error!\n");
					return FALSE;
				}
				//设置版本,录制MP4文件时要用MPEG4
				pConfiguration->version = MPEG4;
				pConfiguration->aacObjectType = LOW; //LC编码
													 //输入数据类型
				pConfiguration->inputFormat = FAAC_INPUT_16BIT;
				// outputFormat (0 = Raw; 1 = ADTS)
				// 录制MP4文件时，要用raw流。检验编码是否正确时可设置为 adts传输流，
				pConfiguration->outputFormat = 1;
				//瞬时噪声定形(temporal noise shaping，TNS)滤波器
				pConfiguration->shortctl = SHORTCTL_NORMAL;
				pConfiguration->useTns = true;
				//pConfiguration->useLfe=false;
				pConfiguration->quantqual = 100;
				pConfiguration->bandWidth = 0;
				pConfiguration->bitRate = 0;
				//  Set encoding configuration
				faacEncSetConfiguration(m_faacEncHandle, pConfiguration);

				m_bInitFaac = TRUE;
			}
		}
		MP4SetTimeScale(m_fileMp4, m_nTimeScale);
		int nDataBufferSize = 1920 * 1080 * 3;
		int nWriteSampleDataSize = 1920 * 1080 * 2;
		char * pWriteSampleData = new char[nWriteSampleDataSize + 1];
		int nFrameType = 0;
		int nTempDataBufferSize = nDataBufferSize;
		char *pDataBuffer = new char[nDataBufferSize + 1];
		unsigned long long ullTimeStamp = 0;
		while (true)
		{
			nTempDataBufferSize = nDataBufferSize;
			bRet = ReadFileData(pDataBuffer, nTempDataBufferSize, nFrameType);
			if (!bRet)
			{
				break;
			}
			REPAIRFILEFRAMEINFO *pFrameInfo = (REPAIRFILEFRAMEINFO *)pDataBuffer;
			if (pFrameInfo->FrameType == 0x63643030 || pFrameInfo->FrameType == 0x63643130)
			{
				//I或P帧
				AsfToMp4_NaluUnit nalu;
				int pos = 0, len = 0;
				pos = 0;
				nalu.data = NULL;
				nalu.size = 0;
				nalu.type = 0;
				while (len = ReadOneNaluFromBuf((unsigned char *)pDataBuffer + sizeof(REPAIRFILEFRAMEINFO), nTempDataBufferSize - sizeof(REPAIRFILEFRAMEINFO), pos, nalu))
				{
					if (nalu.type == 0x07) // sps          
					{
						if (!m_bIsAddH264VideoTrack)
						{
							//获取视频分辨率
							ASF_INFO_S aiAsfInfo = m_AsfFile.GetAsfFileInfo();
							if (aiAsfInfo.struVStream.u32samplePerSec == 0)
							{
								m_nFrameRate = m_AsfFile.GetFrameRateInVBR();
							}
							else
							{
								m_nFrameRate = aiAsfInfo.struAStream.u32SamplesPerSec;
							}
							//添加h264 track
							m_videoTrack = MP4AddH264VideoTrack(m_fileMp4,
								m_nTimeScale,
								m_nTimeScale / m_nFrameRate,
								aiAsfInfo.struVStream.u32BiWidth,
								aiAsfInfo.struVStream.u32BiHeight,
								nalu.data[1], // sps[1] AVCProfileIndication                   
								nalu.data[2], // sps[2] profile_compat                    
								nalu.data[3], // sps[3] AVCLevelIndication                    
								3);
							// 4 bytes length before each NAL unit                
							if (m_videoTrack == MP4_INVALID_TRACK_ID)
							{
								printf("add video track failed.\n");
								MP4Close(m_fileMp4);
								m_fileMp4 = NULL;
								if (pWriteSampleData != NULL)
								{
									delete[]pWriteSampleData;
									pWriteSampleData = NULL;
								}
								if (pDataBuffer != NULL)
								{
									delete[]pDataBuffer;
									pDataBuffer = NULL;
								}
								return FALSE;
							}
							MP4SetVideoProfileLevel(m_fileMp4, 1);
							//  Simple Profile @ Level 3                    
							MP4AddH264SequenceParameterSet(m_fileMp4, m_videoTrack, nalu.data, nalu.size);

							m_bIsAddH264VideoTrack = TRUE;
						}
						else
						{
							int jjj = 0;
						}
					}
					else if (nalu.type == 0x08) // pps            
					{
						if (!m_bIsAddH264PictureParameterSet)
						{
							if (m_bIsAddH264VideoTrack)
							{
								MP4AddH264PictureParameterSet(m_fileMp4, m_videoTrack, nalu.data, nalu.size);
								m_bIsAddH264PictureParameterSet = TRUE;
							}
						}
					}
					else
					{
						int datalen = nalu.size + 4;
						if (datalen > nWriteSampleDataSize)
						{
							delete[]pWriteSampleData;
							nWriteSampleDataSize = datalen;
							pWriteSampleData = new char[nWriteSampleDataSize + 1];
						}
						if (pWriteSampleData != NULL)
						{
							unsigned char *data = (unsigned char *)pWriteSampleData;
							// MP4 Nalu前四个字节表示Nalu长度                
							data[0] = nalu.size >> 24;
							data[1] = nalu.size >> 16;
							data[2] = nalu.size >> 8;
							data[3] = nalu.size & 0xff;
							memcpy(data + 4, nalu.data, nalu.size);
							if (!MP4WriteSample(m_fileMp4, m_videoTrack, data, datalen, MP4_INVALID_DURATION, 0, 1))
							{
								MP4Close(m_fileMp4);
								m_fileMp4 = NULL;
								if (pWriteSampleData != NULL)
								{
									delete[]pWriteSampleData;
									pWriteSampleData = NULL;
								}
								if (pDataBuffer != NULL)
								{
									delete[]pDataBuffer;
									pDataBuffer = NULL;
								}
								return FALSE;
							}
						}
					}
					pos += len;
				}
			}
			else if (pFrameInfo->FrameType == 0x62773130)
			{
				//音频
				if (bIsAudio)
				{
					if (!m_bIsAddAudioTrack)
					{
						//添加aac音频
						m_audioTrack = MP4AddAudioTrack(m_fileMp4, 8000, nMP4AudioDuration, MP4_MPEG4_AUDIO_TYPE);
						if (m_audioTrack == MP4_INVALID_TRACK_ID)
						{
							printf("add audio track failed.\n");
							MP4Close(m_fileMp4);
							m_fileMp4 = NULL;
							return FALSE;
						}
						MP4SetAudioProfileLevel(m_fileMp4, 0x2);
						unsigned long uLen = 0;
						unsigned char *apInfo = NULL;
						faacEncGetDecoderSpecificInfo(m_faacEncHandle, &apInfo, &uLen);
						MP4SetTrackESConfiguration(m_fileMp4, m_audioTrack, (unsigned char*)apInfo, uLen);
						if (apInfo != NULL)
						{
							delete apInfo;
						}
						m_bIsAddAudioTrack = TRUE;
					}
					if (m_bIsAddAudioTrack)
					{
						if (m_bInitFaac)
						{
							//转换g726转换aac
							char szPcmBuffer[2048] = { 0 };
							int nPcmBufferSize = 2048;
							BOOL bAudioFrameDecodeRet = m_G726EnDecoder.Decoder_DecodeFrame(pDataBuffer + sizeof(REPAIRFILEFRAMEINFO), nTempDataBufferSize - sizeof(REPAIRFILEFRAMEINFO), szPcmBuffer, &nPcmBufferSize);
							if (bAudioFrameDecodeRet)
							{
								memcpy(m_pTempDataBuffer + m_nTempDataStartPos, szPcmBuffer, nPcmBufferSize);
								m_nTempDataStartPos += nPcmBufferSize;
								if (m_nTempDataStartPos >= m_nAacMaxInputBytes)
								{
									memcpy(m_pPcmDataBuffer, m_pTempDataBuffer, m_nAacMaxInputBytes);
									//move data
									int nRemainSize = m_nTempDataStartPos - m_nAacMaxInputBytes;
									int j = 0;
									for (j = 0; j < nRemainSize; j++)
									{
										m_pTempDataBuffer[j] = m_pTempDataBuffer[m_nAacMaxInputBytes + j];
									}
									m_nTempDataStartPos -= m_nAacMaxInputBytes;
									int nRetFaacEncode = faacEncEncode(m_faacEncHandle, (int *)m_pPcmDataBuffer, inputSamples, (unsigned char *)m_pAacDataBuffer, m_nAacMaxOutputBytes);
									if (nRetFaacEncode > 0)
									{
										bool bMP4WriteSample = MP4WriteSample(m_fileMp4, m_audioTrack, (BYTE*)&m_pAacDataBuffer[7], nRetFaacEncode - 7, MP4_INVALID_DURATION, 0, true);
									}
									else if (nRetFaacEncode == 0)
									{
										//缓存处理，非错误
									}
									else
									{
										//编码失败，错误
									}
								}
							}
						}
					}
				}
			}
			else if (pFrameInfo->FrameType == 0x63643939)
			{
				//Binary
			}
			else
			{
				continue;
			}
		}
		if (m_faacEncHandle != NULL)
		{
			faacEncClose(m_faacEncHandle);
			m_faacEncHandle = NULL;
		}
		MP4Close(m_fileMp4);
		m_fileMp4 = NULL;
		if (pWriteSampleData != NULL)
		{
			delete[]pWriteSampleData;
			pWriteSampleData = NULL;
		}
		if (pDataBuffer != NULL)
		{
			delete[]pDataBuffer;
			pDataBuffer = NULL;
		}
		return bRet;
	}
	else
	{
		m_bIsStopConvert = FALSE;
		m_hConvertThread = (HANDLE)_beginthreadex(NULL, 0, ConvertThreadFun, this, 0, NULL);
		return TRUE;
	}
}
BOOL AsfToMp4::StopConvert()
{
	m_bIsStopConvert = TRUE;
	if (m_hConvertThread != NULL)
	{
		WaitForSingleObject(m_hConvertThread, INFINITE);
		CloseHandle(m_hConvertThread);
		m_hConvertThread = NULL;
	}
	if (m_fileMp4 != NULL)
	{
		MP4Close(m_fileMp4);
		m_fileMp4 = NULL;
	}
	if (m_faacEncHandle != NULL)
	{
		faacEncClose(m_faacEncHandle);
		m_faacEncHandle = NULL;
	}
	return TRUE;
}

//---------------------------------------------------------------------
//说明：从文件中读取数据
//参数：无
//返回：成功返回TRUE，失败返回FALSE
//---------------------------------------------------------------------
BOOL AsfToMp4::ReadFileData(char *pOutData, int & nOutDataSize, int & FrameType)
{
	if (m_AsfFile.IsOpened() == FALSE)
	{
		int iCntWnd = 0, iCh_id = 0;
		if (!m_AsfFile.OpenFileEx(m_szSrcFilePathName, iCntWnd, &iCh_id))
			return FALSE;
	}

	int BufSize = 0;
	FrameType = 0;
	LONGLONG Framestamp = 0;
	BOOL bRet = m_AsfFile.ReadFileData(pOutData, &BufSize);
	if (bRet == TRUE)
	{
		nOutDataSize = BufSize;
		FrameType = *(int *)pOutData;
		if (m_bFileOver == FALSE && m_AsfFile.m_TotalSecond == m_AsfFile.m_StartTime.Pts)
		{
			m_bFileOver = TRUE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------
//说明：取得P帧的偏移长度，P帧中可能存在一定的无效数据，因此需要删除
//参数：[in]stream:帧数据
//		[in]iLen:数据长度
//返回：返回P帧的偏移长度
//调用：ChangeFile()中调用
//---------------------------------------------------------------------
int	AsfToMp4::GetPFrameOffset(unsigned char *stream, int iLen)
{
	return 8;
}

//---------------------------------------------------------------------
//说明：取得P帧的偏移长度，P帧中可能存在一定的无效数据，因此需要删除
//参数：[in]stream:帧数据
//		[in]iLen:数据长度
//返回：返回P帧的偏移长度
//调用：ChangeFile()中调用
//---------------------------------------------------------------------
int	AsfToMp4::GetAFrameOffset(unsigned char *stream, int iLen)
{
	return 8;
}

// 获取Asf文件头信息结构体。
BOOL AsfToMp4::GetAsfFileInfo(ASF_INFO_S *struAsfInfo)
{
	CXMVSAsfFile TmpAsf;
	int iCntWnd = 0, iCh_id = 0;
	if (!TmpAsf.OpenFileEx(m_szSrcFilePathName, iCntWnd, &iCh_id))
		return FALSE;
	*struAsfInfo = TmpAsf.GetAsfFileInfo();
	if (TmpAsf.IsOpened() == TRUE)
		TmpAsf.CloseFile();

	return TRUE;
}

// read one nalu from H264 data buffer        
int AsfToMp4::ReadOneNaluFromBuf(const unsigned char *buffer, unsigned int nBufferSize, unsigned int offSet, AsfToMp4_NaluUnit &nalu)
{
	nalu.size = 0;
	nalu.type = 0;
	int i = offSet;
	while (i < nBufferSize)
	{
		if (buffer[i++] == 0x00 && buffer[i++] == 0x00 && buffer[i++] == 0x00 && buffer[i++] == 0x01)
		{
			int pos = i;
			while (pos < nBufferSize)
			{
				if (buffer[pos++] == 0x00 && buffer[pos++] == 0x00 && buffer[pos++] == 0x00 && buffer[pos++] == 0x01)
				{
					break;
				}
			}
			if (pos == nBufferSize)
			{
				nalu.size = pos - i;
			}
			else
			{
				nalu.size = (pos - 4) - i;
			}
			nalu.alltype = buffer[i];
			nalu.type = buffer[i] & 0x1f;
			nalu.data = (unsigned char*)&buffer[i];
			return (nalu.size + i - offSet);
		}
	}
	return 0;
}
```

