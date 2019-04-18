# librtmp获取视频流和音频流2 - 深之JohnChen的专栏 - CSDN博客

2017年01月24日 21:55:07[byxdaz](https://me.csdn.net/byxdaz)阅读数：2621


        在上篇文章中，使用librtmp库中包流函数（比如：RTMP_ReadPacket，RTMP_ClientPacket等）读取视频数据包，音频数据包。但在实际过程中连接rtmp服务器请求视频流时只能获取到I帧，无法获取P帧，B帧，从而导致视频不流畅，关于无法获取到P帧、B帧数据包与rtmp服务器有关。连接crtmpserver服务器获取视频流时只能获取到I帧，无法获取P帧，B帧；而连接香港卫视的直播流对应的rtmp服务器时，就可以获取到P帧。无论那个rtmp服务器通过RTMP_Read可以获取I帧、P帧等。本文重点介绍使用RTMP_Read获取视频流和音频流。

 
   获取数据流流程：

1、RTMP_SetupURL，指定直播流地址。

2、设置参数，比如：RTMP_SetBufferMS，设置缓存大小

3、RTMP_Connect

4、RTMP_ConnectStream

5、循环使用RTMP_Read读取数据流，将数据流存放到一个内存区域中。

6、按照FLV格式，循环从内存区域中读取数据，解析文件头，meta数据，视频数据，音频数据等。

       主要流程代码：

```cpp
#define ONE_PACKET_PCM_DATA_LEN			320

//ACC音频ADTS
typedef struct tagAACDecoderSpecific
{
	unsigned char nAudioFortmatType;	//音频编码类型（0：Liner PCM platform endian，1：PCM，2：mp3，4：Nellymoser 16-kHz mono,5:Nellymoser 8-kHz mono,6:Nellymoser,7:G.711 A-law logarithmic PCM,8:G.711 mu-law logarithmic PCM,9:reserved,10:?AAC,14:MP3 8-Khz,15:Device-specific sound）
	unsigned char nAudioSampleType;	//音频采样率(0:5.5kHz,1:11KHz,2:22 kHz,3:44 kHz)
	unsigned char nAudioSizeType;	//音频采样精度(0:8bits,1:16bits)
	unsigned char nAudioStereo;//是否立体声(0:sndMono,1:sndStereo)
	unsigned char nAccPacketType;
}AACDecoderSpecific;

typedef struct tagAACAudioSpecificConfig
{
	unsigned char nAudioObjectType;
	unsigned char nSampleFrequencyIndex;
	unsigned char nChannels;
	unsigned char nFrameLengthFlag;
	unsigned char nDependOnCoreCoder;
	unsigned char nExtensionFlag;
}AACAudioSpecificConfig;

int			m_nImageWidth;
	int			m_nImageHeight;
	int			m_nFrameRate;
	char		m_szSPSData[512];
	int			m_nSPSDataLength;
	bool		m_bFindSPSData;
	char		m_szPPSData[512];
	int			m_nPPSDataLength;
	bool		m_bFindPPSData;
	bool		m_bSendSPSData;
	bool		m_bSendPPSData;
	bool		m_bRequireSendIFrame;
	AAC2PCM		m_aacOjbect;
	CammPoFrameDataProcess m_cfdCammPoFrameDataProcess;
	bool		m_bInitCammPoFrameDataProcess;
	FlvToStream m_ftsFlvToStream;//flv内存文件类

	char *m_pVideoTempBuffer;//视频流缓存区
	unsigned int   m_nVideoTempBufferSize;
	unsigned int   m_nVideoTempBufferActualSize;
	int			m_nDelayTime;
	AACAudioSpecificConfig m_ascAudioSpecificConfig;
	char m_szSPSDataTmp[512];
	SPS m_spsFlag;
	unsigned long long m_ullBaseTimeStamps;
	uint32_t m_ullStartTimeStamps;
	uint32_t m_ullCurrentTimeStamps;
	uint32_t m_ullDiffTimeStamps;
	uint32_t m_uPacketStartTimeStamps;
	uint32_t m_uPacketCurrentTimeStamps;
	uint32_t m_uPacketDiffTimeStamps;
	uint32_t m_uPacketLastTimeStamps;
	uint32_t m_uPacketTempTimeStamps;
	bool m_bFindNextIFrame;
	unsigned int m_nPackTiemStampSecondTmp;
	int m_nTempBufferSize;
	char *m_pTempBuffer;
	unsigned char start_code[4];
	bool m_bRawAcc; //ACC原始数据音频包
	bool m_bAccObjectInit;
	char *m_pAccTempBuffer;		//acc音频流缓冲区
	unsigned int   m_nAccTempBufferSize;
	unsigned int   m_nAccTempBufferActualSize;
	char *m_pPcmLineBuffer;		//pcm音频流缓存区
	unsigned int m_nPcmLineBufferSize;
	unsigned int m_nPcmLineBufferActualSize;
	unsigned int m_nPcmLineBufferReadPos;
	unsigned int m_nPcmLineBufferWritePos;
	unsigned char m_ucTempPcmData[ONE_PACKET_PCM_DATA_LEN+1];
	char *m_pG726TempBuffer;
	unsigned int m_nG726TempBufferSize;
	
	
RTMP *	m_rtmp;
m_rtmp = RTMP_Alloc();
	
RTMP_Init(m_rtmp);
//10秒
	RTMP_SetBufferMS(m_rtmp,10 * 1000);
	if (!RTMP_Connect(m_rtmp, NULL)){
		RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
		RTMP_Free(m_rtmp);
		m_rtmp = NULL;
		return -1;
	}
	if (!RTMP_ConnectStream(m_rtmp, 0)){
		RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
		RTMP_Close(m_rtmp);
		RTMP_Free(m_rtmp);
		m_rtmp = NULL;
		return -1;
	}
	
	int nRead = 0;
	int bufsize = 1024*1024;			
	char *buf = new char[bufsize];
	memset(buf,0,bufsize);
	long countbufsize = 0;
	m_ftsFlvToStream.Init(bufsize*10);
	bool bIsOnePackage = false;
	int  nTagType = 0;
	int nTimeStamp = 0;
	int nWriteRawData = 0;
	int nDiffTimeStamp = 0;
	bool bFirstTickCount = true;
	DWORD dwStart = 0;
	DWORD dwNow = 0;
	DWORD dwDiffTickCountTmp = 0;
	int   nTimeSpan = 0;
	bool bFirstStartTimeStamp = true;
	int  nStartTimeStampTmp = 0;
	int  nStopTimeStampTmp = 0;
	int  nDiffTimeStampTmp = 0;
	while (IsRunning() && !IsException())
	{
		nRead = RTMP_Read(m_rtmp,buf,bufsize);
		if(nRead == 0 || RTMP_IsTimedout(m_rtmp))
		{
			SetException(true);
			//回调设备状态
			if(m_pDeviceAnswerCallback != NULL)
			{
				m_pDeviceAnswerCallback(DVRNetSDK_ErrorCode_Exception,m_nDeviceAnswerUserData1,m_nDeviceAnswerUserData2);
			}
			break;
		}
		if(bFirstTickCount)
		{
			dwStart = GetTickCount();
			dwNow = dwStart;
			dwDiffTickCountTmp = 0;
			bFirstTickCount = false;
		}
		else
		{
			dwNow = GetTickCount();
			dwDiffTickCountTmp = dwNow - dwStart;
		}

		//获取到数据，写入到内存缓冲区
		nWriteRawData = m_ftsFlvToStream.WriteRawData(buf,nRead);
		if(nWriteRawData == 2)
		{
			//空间不够
			while(true)
			{
				//循环读数据，解析数据
				bIsOnePackage = m_ftsFlvToStream.IsOnePackageData();
				if(bIsOnePackage)
				{
					//读取数据
					m_ftsFlvToStream.ReadFrameData(nTagType,nTimeStamp,m_pTempBuffer,m_nTempBufferSize);
					//解析数据
					ParseFrameData(nTagType,nTimeStamp,m_pTempBuffer,m_nTempBufferSize);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			if(m_ftsFlvToStream.GetBufferEmptySize() >= bufsize)
			{
				//缓存区有太大多数据，丢弃
				int nStartTimeStampTmp2 = 0;
				int nStopTimeStampTmp2 = 0;
				nDiffTimeStamp = m_ftsFlvToStream.GetDiffTime(nStartTimeStampTmp2,nStopTimeStampTmp2);
				if(nDiffTimeStamp >= 3000)
				{
					m_ftsFlvToStream.SkipToLastFrame(0);
				}
				else
				{
					while(true)
					{
						//循环读数据，解析数据
						bIsOnePackage = m_ftsFlvToStream.IsOnePackageData();
						if(bIsOnePackage)
						{
							//读取数据
							m_ftsFlvToStream.ReadFrameData(nTagType,nTimeStamp,m_pTempBuffer,m_nTempBufferSize);
							//解析数据
							ParseFrameData(nTagType,nTimeStamp,m_pTempBuffer,m_nTempBufferSize);
						}
						else
						{
							break;
						}
					}
				}
			}
			else
			{
				//解析数据
				bIsOnePackage = m_ftsFlvToStream.IsOnePackageData();
				if(bIsOnePackage)
				{
					//读取数据
					m_ftsFlvToStream.ReadFrameData(nTagType,nTimeStamp,m_pTempBuffer,m_nTempBufferSize);
					//解析数据
					ParseFrameData(nTagType,nTimeStamp,m_pTempBuffer,m_nTempBufferSize);
				}
			}
		}
		if(bFirstStartTimeStamp)
		{
			nStartTimeStampTmp = 0;
			nStopTimeStampTmp = 0;
			nDiffTimeStampTmp = 0;
			bFirstStartTimeStamp = false;
		}
		else
		{
			int nStartTimeStampTmp1 = 0;
			int nStopTimeStampTmp1 = 0;
			m_ftsFlvToStream.GetDiffTime(nStartTimeStampTmp1,nStopTimeStampTmp1);
			nStopTimeStampTmp = nStopTimeStampTmp1;
			nDiffTimeStampTmp = nStopTimeStampTmp - nStartTimeStampTmp;
		}
		nTimeSpan = GetTickCount() - dwNow;
		if(dwDiffTickCountTmp >= (nDiffTimeStampTmp+nTimeSpan))
		{
			Sleep(10);
		}
		else
		{
			Sleep(25);
		}
	}

	//释放空间
	if(buf != NULL)
	{
		delete []buf;
	}
		if(m_rtmp != NULL)
	{
		RTMP_DeleteStream(m_rtmp);
	}
	
	
	
	
	//数据解析
int ParseFrameData(int nTagType,int nTimeStamp,char * pTempBuffer,int nTempBufferSize)
{
	int nFrameType = 0;
	unsigned char result = 0;
	unsigned char nAVCPacketType = 0;
	char *data = NULL;

	if(m_uPacketStartTimeStamps == 0)
	{
		m_uPacketStartTimeStamps = nTimeStamp;
		m_uPacketCurrentTimeStamps = m_uPacketStartTimeStamps;
	}
	else
	{
		m_uPacketStartTimeStamps = m_uPacketCurrentTimeStamps;
		m_uPacketCurrentTimeStamps = nTimeStamp;
	}
	m_uPacketDiffTimeStamps = m_uPacketCurrentTimeStamps - m_uPacketStartTimeStamps;
	m_ullStartTimeStamps = m_ullCurrentTimeStamps;
	m_ullCurrentTimeStamps = RTMP_GetTime();
	m_ullDiffTimeStamps = m_ullCurrentTimeStamps - m_ullStartTimeStamps;

	if(nTagType == RTMP_PACKET_TYPE_INFO)
	{
		int nVideoCodecId = 0;
		int nVideoWidth = 0;
		int nVideoHeight = 0;
		int nVideoFrameRate = 25;
		int nAudioCodecId = 0;
		int nAudioSampleRate = 0;
		int nAudioSampleSize = 0;
		bool bStereo = false;			//立体声
		int nFileSize = 0;
		ParseScriptTag(pTempBuffer, nTempBufferSize, nVideoCodecId, nVideoWidth, nVideoHeight, nVideoFrameRate, nAudioCodecId, nAudioSampleRate, nAudioSampleSize, bStereo, nFileSize);
		m_nImageWidth = nVideoWidth;
		m_nImageHeight = nVideoHeight;
		m_nFrameRate = nVideoFrameRate;
		if(m_nImageWidth != 0 && m_nImageHeight != 0)
		{
			m_nDelayTime = 15 < 1000.0/m_nFrameRate ? 1000.0/m_nFrameRate:15;
			//头帧码流回调
			HeaderFrameStreamCallback();
		}
	}
	else if(nTagType == RTMP_PACKET_TYPE_VIDEO)
	{
		data = pTempBuffer;
		result = pTempBuffer[0];
		nAVCPacketType = pTempBuffer[1];
		bool bIsKeyFrame = false;
		nFrameType = 11;
		if (result == 0x17)//I frame
		{
			bIsKeyFrame = true;
			nFrameType = 10;
		}
		else if (result == 0x27)
		{
			nFrameType = 11;
		}
		else
		{
			return 0;
		}			
		if (nAVCPacketType == 0)
		{
			//AVCsequence header
			//Access to SPS
			int spsnum = data[10] & 0x1f;
			int number_sps = 11;
			int count_sps = 1;
			while (count_sps <= spsnum){
				int spslen = (data[number_sps] & 0x000000FF) << 8 | (data[number_sps + 1] & 0x000000FF);
				number_sps += 2;
				memset(m_szSPSData, 0, sizeof(m_szSPSData));
				memcpy(m_szSPSData, start_code, 4);
				memcpy(m_szSPSData + 4, data + number_sps, spslen);
				m_nSPSDataLength = 4 + spslen;
				memcpy(m_szSPSDataTmp, m_szSPSData, sizeof(m_szSPSData));
				m_bFindSPSData = true;
				//如果没有获取图像宽高，帧率
				if (0 == m_nImageWidth)
				{
					//分析sps
					get_bit_context spsRawData;
					spsRawData.buf = (unsigned char *)m_szSPSDataTmp + 5;
					spsRawData.buf_size = m_nSPSDataLength - 5;
					spsRawData.bit_pos = 0;
					spsRawData.cur_bit_pos = 0;
					spsRawData.total_bit = 0;
					int nSpsParseRet = h264dec_seq_parameter_set(&spsRawData, &m_spsFlag);
					if (0 == nSpsParseRet)
					{
						//解析成功
						m_nImageWidth = (m_spsFlag.pic_width_in_mbs_minus1 + 1) * 16;
						m_nImageHeight = (m_spsFlag.pic_height_in_map_units_minus1 + 1) * 16;
						m_nFrameRate = m_spsFlag.vui_parameters.time_scale / m_spsFlag.vui_parameters.num_units_in_tick;
						m_nDelayTime = 15 < 1000.0/m_nFrameRate ? 1000.0/m_nFrameRate:15;
						//头帧码流回调
						HeaderFrameStreamCallback();
					}
				}

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
				memset(m_szPPSData, 0, sizeof(m_szPPSData));
				memcpy(m_szPPSData, start_code, 4);
				memcpy(m_szPPSData+4, data + number_pps, ppslen);
				m_nPPSDataLength = 4 + ppslen;
				m_bFindPPSData = true;

				number_pps += ppslen;
				count_pps++;
			}
		}
		else if (nAVCPacketType == 1)
		{
			//AVC NALU
			int len = 0;
			int num = 5;
			int nNALVNumbers = 0;
			while (num < nTempBufferSize)
			{
				len = (data[num] & 0x000000FF) << 24 | (data[num + 1] & 0x000000FF) << 16 | (data[num + 2] & 0x000000FF) << 8 | data[num + 3] & 0x000000FF;
				if(len > nTempBufferSize)
				{
					break;
				}
				num += 4;

				if((len+4+8) >m_nVideoTempBufferSize-1)
				{
					m_nVideoTempBufferSize = len + 1024*5;
					delete []m_pVideoTempBuffer;
					m_pVideoTempBuffer = new char[m_nVideoTempBufferSize];
				}
				if(m_pVideoTempBuffer != NULL)
				{	
					//8个字节时间戳
					unsigned long long ullTimeStamps = 0;
					if (!m_bSendSPSData && m_bFindSPSData)
					{
						ullTimeStamps = m_ullBaseTimeStamps + nTimeStamp;
						memcpy(m_pVideoTempBuffer,&ullTimeStamps,8);
						m_nVideoTempBufferActualSize = 8;
						memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,m_szSPSData,m_nSPSDataLength);
						m_nVideoTempBufferActualSize +=m_nSPSDataLength;
						if(m_pCallback != NULL)
						{
							//回调码流
							//m_pCallback(m_szDeviceID,m_uiChannel,0,0,e_DVRNET_FrameType_Video_IFrame,m_pVideoTempBuffer,m_nVideoTempBufferActualSize,m_uiCallbackUserData1,m_uiCallbackUserData2);
						}
						//m_bSendSPSData = true;
					}
					if (!m_bSendPPSData && m_bFindPPSData)
					{
						ullTimeStamps = m_ullBaseTimeStamps + nTimeStamp;
						memcpy(m_pVideoTempBuffer,&ullTimeStamps,8);
						m_nVideoTempBufferActualSize = 8;
						memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,m_szPPSData,m_nPPSDataLength);
						m_nVideoTempBufferActualSize +=m_nPPSDataLength;
						if(m_pCallback != NULL)
						{
							//回调码流
							//m_pCallback(m_szDeviceID,m_uiChannel,0,0,e_DVRNET_FrameType_Video_IFrame,m_pVideoTempBuffer,m_nVideoTempBufferActualSize,m_uiCallbackUserData1,m_uiCallbackUserData2);
						}
						//m_bSendPPSData = true;
					}
					//8个字节时间戳
					/*
					ullTimeStamps = m_ullBaseTimeStamps + nTimeStamp + nNALVNumbers;
					memcpy(m_pVideoTempBuffer,&ullTimeStamps,8);
					m_nVideoTempBufferActualSize = 8;
					memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,start_code,4);
					m_nVideoTempBufferActualSize +=4;
					memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,data + num,len);
					m_nVideoTempBufferActualSize +=len;
					*/
					ullTimeStamps = m_ullBaseTimeStamps + nTimeStamp + nNALVNumbers;
					memcpy(m_pVideoTempBuffer,&ullTimeStamps,8);
					m_nVideoTempBufferActualSize = 8;
					memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,start_code,4);
					m_nVideoTempBufferActualSize +=4;
					if (!m_bSendSPSData && m_bFindSPSData)
					{
						memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,m_szSPSData,m_nSPSDataLength);
						m_nVideoTempBufferActualSize +=m_nSPSDataLength;
						m_bSendSPSData = true;
					}
					if (!m_bSendPPSData && m_bFindPPSData)
					{
						memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,m_szPPSData,m_nPPSDataLength);
						m_nVideoTempBufferActualSize +=m_nPPSDataLength;
						m_bSendPPSData = true;
					}
					memcpy(m_pVideoTempBuffer+m_nVideoTempBufferActualSize,data + num,len);
					m_nVideoTempBufferActualSize +=len;

					if(m_pCallback != NULL)
					{
						//启动缓存区读写线程
						if(!m_bInitCammPoFrameDataProcess)
						{
							int nMaxFrameNum = 25;
							if(m_nFrameRate > 0 && m_nFrameRate < 80)
							{
								nMaxFrameNum = m_nFrameRate;
							}
							m_cfdCammPoFrameDataProcess.Init(Max_Video_Width*Max_Video_Width*5,1000,m_nFrameRate);
							m_cfdCammPoFrameDataProcess.SetDeviceID(m_szDeviceID);
							m_cfdCammPoFrameDataProcess.SetChannel(m_uiChannel);
							m_cfdCammPoFrameDataProcess.SetRealAVStreamDataCallback(m_pCallback,m_uiCallbackUserData1,m_uiCallbackUserData2);
							m_cfdCammPoFrameDataProcess.StartThread();
							m_bInitCammPoFrameDataProcess = true;
						}										
						//回调码流
						if(m_cfdCammPoFrameDataProcess.RemainWriteFrameNum() > 0)
						{
							TraceDebug("rtmp client llTimeStampsTmp=%lld,frameType=%d,framenum=%d,frametime=%d\n",ullTimeStamps,nFrameType,m_cfdCammPoFrameDataProcess.RemainWriteFrameNum(),m_cfdCammPoFrameDataProcess.m_fbVideoFrameBuffer.GetBufferFrameTime());
							if(m_bFindNextIFrame)
							{
								if(bIsKeyFrame)
								{
									m_bFindNextIFrame = false;
									m_cfdCammPoFrameDataProcess.WriteFrameData(nFrameType,(unsigned char *)m_pVideoTempBuffer,m_nVideoTempBufferActualSize);
								}
							}
							else
							{
								m_cfdCammPoFrameDataProcess.WriteFrameData(nFrameType,(unsigned char *)m_pVideoTempBuffer,m_nVideoTempBufferActualSize);
							}
						}
						else
						{
							m_cfdCammPoFrameDataProcess.m_fbVideoFrameBuffer.ResetToSaveVideoIFrame();
							m_bFindNextIFrame = true;
							//Sleep(15);
						}									
						//m_pCallback(m_szDeviceID,m_uiChannel,0,0,nFrameType,pVideoTempBuffer,nVideoTempBufferActualSize,m_uiCallbackUserData1,m_uiCallbackUserData2);
					}
				}
				num +=len;
				nNALVNumbers++;
			}
		}
		else if (nAVCPacketType == 2)
		{
			//AVC end of sequence (lower level NALU sequence ender is not required or supported)
		}

	}
	else if(nTagType == RTMP_PACKET_TYPE_AUDIO)
	{	
		nFrameType = 9;
		data = pTempBuffer;
		//判断是否ACC音频同步包,还是ACC原始数据音频包
		if (!m_bRawAcc && nTempBufferSize>=4)
		{
			//ACC音频同步包(1bytes+3bytes AccAudioData) (4bytes 包含了AACDecoderSpecific和AudioSpecificConfig)
			AACDecoderSpecific adsAACDecoderSpecific = { 0 };
			adsAACDecoderSpecific.nAudioFortmatType = (data[0] & 0xf0) >> 4;	//音频编码类型（0：Liner PCM platform endian，1：PCM，2：mp3，4：Nellymoser 16-kHz mono,5:Nellymoser 8-kHz mono,6:Nellymoser,7:G.711 A-law logarithmic PCM,8:G.711 mu-law logarithmic PCM,9:reserved,10:?AAC,14:MP3 8-Khz,15:Device-specific sound）
			adsAACDecoderSpecific.nAudioSampleType = (data[0] & 0x0c) >> 2;	//音频采样率(0:5.5kHz,1:11KHz,2:22 kHz,3:44 kHz)
			adsAACDecoderSpecific.nAudioSizeType = (data[0] & 0x02) >> 1;	//音频采样精度(0:8bits,1:16bits)
			adsAACDecoderSpecific.nAudioStereo = data[0] & 0x01;//是否立体声(0:sndMono,1:sndStereo)
			if (adsAACDecoderSpecific.nAudioFortmatType == 10)
			{
				//The following values are defined:
				//0 = AAC sequence header
				//1 = AAC raw
				adsAACDecoderSpecific.nAccPacketType = data[1];
				unsigned short audioSpecificConfig = 0;
				audioSpecificConfig = (data[2] & 0xff) << 8;
				audioSpecificConfig += 0x00ff & data[3];
				m_ascAudioSpecificConfig.nAudioObjectType = (audioSpecificConfig & 0xF800) >> 11;
				m_ascAudioSpecificConfig.nSampleFrequencyIndex = (audioSpecificConfig & 0x0780) >> 7;
				m_ascAudioSpecificConfig.nChannels = (audioSpecificConfig & 0x78) >> 3;
				m_ascAudioSpecificConfig.nFrameLengthFlag = (audioSpecificConfig & 0x04) >> 2;
				m_ascAudioSpecificConfig.nDependOnCoreCoder = (audioSpecificConfig & 0x02) >> 1;
				m_ascAudioSpecificConfig.nExtensionFlag = audioSpecificConfig & 0x01;
				m_bRawAcc = true;
			}
			else if (adsAACDecoderSpecific.nAudioFortmatType == 11)
			{
				//speex类型数据时，后面的4位数据不起作用，固定的是16KHZ，单声道，16bit/sample
				adsAACDecoderSpecific.nAudioStereo = 0;
				adsAACDecoderSpecific.nAudioSizeType = 1;
				adsAACDecoderSpecific.nAudioSampleType = 4;
				m_bRawAcc = true;
			}
		}
		else
		{
			//raw data 获取 audio payload
			if (nTempBufferSize > 2 && data[1] == 1)
			{
				//写ADTS数据到文件
				char szADTSTemp[8] = { 0 };
				CreateADTS(m_ascAudioSpecificConfig, nTempBufferSize - 2 + 7, szADTSTemp);
				//转换aac数据到g726
				memcpy(m_pAccTempBuffer, szADTSTemp, 7);
				memcpy(m_pAccTempBuffer + 7, pTempBuffer + 2, nTempBufferSize - 2);
				m_nAccTempBufferSize = 7 + nTempBufferSize - 2;
				if (!m_bAccObjectInit)
				{
					m_aacOjbect.init(m_ascAudioSpecificConfig.nAudioObjectType, GetSampleRate(m_ascAudioSpecificConfig.nSampleFrequencyIndex));
					m_bAccObjectInit = true;
				}
				if((m_nPcmLineBufferSize-m_nPcmLineBufferWritePos) <= 4096)
				{
					//移动数据，pcm缓冲区
					int kk = 0;
					for(int k=m_nPcmLineBufferReadPos; k<m_nPcmLineBufferWritePos; k++)
					{
						m_pPcmLineBuffer[kk] = m_pPcmLineBuffer[k];
						kk++;
					}
					m_nPcmLineBufferReadPos = 0;
					m_nPcmLineBufferWritePos = kk;
				}
				char *pTmp = m_pPcmLineBuffer + m_nPcmLineBufferWritePos;
				m_nPcmLineBufferActualSize = 0;
				int nAccConvertResult = m_aacOjbect.convert2PCM((unsigned char *)m_pAccTempBuffer, m_nAccTempBufferSize,(unsigned char *)pTmp, m_nPcmLineBufferActualSize);
				if(nAccConvertResult == 0)
				{
					m_nPcmLineBufferWritePos += m_nPcmLineBufferActualSize;
					//从pcm缓冲区读取数据
					while((m_nPcmLineBufferWritePos - m_nPcmLineBufferReadPos) >= ONE_PACKET_PCM_DATA_LEN)
					{
						memcpy(m_ucTempPcmData,m_pPcmLineBuffer+m_nPcmLineBufferReadPos,ONE_PACKET_PCM_DATA_LEN);
						m_nPcmLineBufferReadPos +=ONE_PACKET_PCM_DATA_LEN;
						//音频PCM转换G726
						size_t buf_sizeG726 = 0;
						nAccConvertResult = m_aacOjbect.Pcm2G726(m_ucTempPcmData,ONE_PACKET_PCM_DATA_LEN,(unsigned char *)m_pG726TempBuffer+8,buf_sizeG726);
						if(nAccConvertResult == 0)
						{
							if(m_pCallback != NULL)
							{
								//8个字节时间戳
								unsigned long long ullTimeStamps = 0;
								ullTimeStamps = m_ullBaseTimeStamps + nTimeStamp;
								memcpy(m_pG726TempBuffer,&ullTimeStamps,8);
								buf_sizeG726 +=8;
								//回调码流
								if(m_bInitCammPoFrameDataProcess)
								{
									if(m_cfdCammPoFrameDataProcess.RemainWriteFrameNum() > 0)
									{
										m_cfdCammPoFrameDataProcess.WriteFrameData(nFrameType,(unsigned char *)m_pG726TempBuffer,buf_sizeG726);
									}
								}

								//m_pCallback(m_szDeviceID,m_uiChannel,0,0,nFrameType,m_pG726TempBuffer,buf_sizeG726,m_uiCallbackUserData1,m_uiCallbackUserData2);
							}
						}
					}
				}
			}
			else if (nTempBufferSize > 4 && data[1] == 0)
			{
				int jjjjj = 0;
			}
		}
		//The actual audio content for the pkt-> m_body+1, size is pkt-> m_nBodySize-1. Here is the voice of Speex code.
	}

	return 0;
}
```

       FlvToStream类，FLV内存文件解析类。

      FlvToStream.h

```cpp
//帧缓冲区

#ifndef  __FlvToStream_H__
#define  __FlvToStream_H__

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//int h264space = 0x01000000;//H264内容间隔标识00000001

#define HTON16(x)  ((x>>8&0xff)|(x<<8&0xff00))
#define HTON24(x)  ((x>>16&0xff)|(x<<16&0xff0000)|x&0xff00)
#define HTON32(x)  ((x>>24&0xff)|(x>>8&0xff00)|\
	(x << 8 & 0xff0000) | (x << 24 & 0xff000000))

class FlvToStream
{
public:
	FlvToStream(void);
	virtual ~FlvToStream(void);

	//初始化
	int Init(int nAllocSize);
	
	//反初始化
	int UnInit();

	//写数据
	int WriteRawData(char *pBuffer,int nBufferSize);

	//是否有一包完整数据
	bool IsOnePackageData();

	//读取数据
	int ReadFrameData(int & nFrameType,int & nTimeStamp,char *pBuffer,int & nBufferSize);

	/*
	功能：跳转至最后一帧数据
	参数：nLastFrameDataType，数据帧类型，0：帧；1：视频主帧；2：视频虚帧；3：音频帧
	*/
	void SkipToLastFrame(int nLastFrameDataType);

	//获取帧数据时间差(总数据差)
	int  GetDiffTime(int & nStartTimeStamp,int & nStopTimeStamp);

	//获取剩余空间
	unsigned int  GetBufferEmptySize();
protected:
	//调整位置
	void Resize();

	//清除
	void Clear();

	//读字节数据	
	bool Read8(int &i8, char *pBuffer);
	bool Read16(int &i16,char *pBuffer);
	bool Read24(int &i24, char *pBuffer);
	bool Read32(int &i32, char *pBuffer);
	bool ReadTime(int &itime, char *pBuffer);

	bool Peek8(int &i8, char *pBuffer);

	bool ReadHead();

	void VideoParse(int datalength);
	void AudioParse(int datalength);
	void MediaInfoParse(int datalength);

	char *m_pAllocBuffer;
	unsigned int   m_nAllocBufferSize;
	unsigned int   m_nAllocBufferEmptySize;
	unsigned int   m_nReadPos;
	unsigned int   m_nWritePos;
	bool  m_bReadFileHeader;
	bool  m_bPreviousTagSize0Read;
	char *m_pTempBuffer;					//流缓存区
	unsigned int   m_nTempBufferSize;
	int	  m_nFrameType;
	int   m_nDateStamp;
	//FILE *h264file;
};

#endif
```
FlvToStream.cpp

```cpp
#include "stdafx.h"
#include "FlvToStream.h"

#define		Max_Video_Width				1024				//视频宽度
#define		Max_Video_Height			720					//视频高度

FlvToStream::FlvToStream(void)
{
	m_pAllocBuffer = NULL;
	m_nAllocBufferSize = 0;
	m_nAllocBufferEmptySize = 0;
	m_nReadPos = 0;
	m_nWritePos = 0;
	m_bReadFileHeader = false;
	m_bPreviousTagSize0Read = false;
	m_pTempBuffer = NULL;
	m_nTempBufferSize = 0;
}

FlvToStream::~FlvToStream(void)
{
	UnInit();
}

//初始化
int FlvToStream::Init(int nAllocSize)
{
	m_nAllocBufferSize = nAllocSize;
	m_nAllocBufferEmptySize = m_nAllocBufferSize;
	m_nReadPos = 0;
	m_nWritePos = 0;
	if(m_pAllocBuffer != NULL)
	{
		delete []m_pAllocBuffer;
	}
	m_pAllocBuffer = new char[m_nAllocBufferSize*1];
	memset(m_pAllocBuffer,0,sizeof(char)*(m_nAllocBufferSize*1));
	m_bReadFileHeader = false;
	m_bPreviousTagSize0Read = false;
	m_pTempBuffer = new  char[Max_Video_Width * Max_Video_Height+1];
	m_nTempBufferSize = Max_Video_Width * Max_Video_Height+1;
	//h264file = fopen("FlvToStream.h264", "wb");

	return 0;
}

//反初始化
int FlvToStream::UnInit()
{
	if(m_pAllocBuffer != NULL)
	{
		delete []m_pAllocBuffer;
		m_pAllocBuffer = NULL;
	}
	if(m_pTempBuffer != NULL)
	{
		delete []m_pTempBuffer;
		m_pTempBuffer = NULL;
	}

	return 0;
}

//调整位置
void FlvToStream::Resize()
{
	int n = 0;
	for(n=m_nReadPos;n<m_nWritePos;n++)
	{
		m_pAllocBuffer[n-m_nReadPos] = m_pAllocBuffer[n];
	}
	unsigned int nTmp = m_nWritePos-m_nReadPos;
	m_nReadPos = 0;
	m_nWritePos = nTmp;
	m_nAllocBufferEmptySize = m_nAllocBufferSize - nTmp;
}

//清除
void FlvToStream::Clear()
{
	m_nAllocBufferEmptySize = m_nAllocBufferSize;
	m_nReadPos = 0;
	m_nWritePos = 0;
}
//写数据
int FlvToStream::WriteRawData(char *pBuffer,int nBufferSize)
{
	unsigned int nTmp = m_nWritePos-m_nReadPos;
	if(nBufferSize > (m_nAllocBufferSize-m_nWritePos))
	{
		if(nBufferSize <= (m_nAllocBufferSize - nTmp))
		{
			//调整位置（线性缓冲区）
			Resize();
		}
		else
		{
			return 2;//空间不够
		}
	}
	if((m_nAllocBufferSize - m_nWritePos) < nBufferSize)
	{
		return 2;
	}

	memcpy(m_pAllocBuffer+m_nWritePos,pBuffer,nBufferSize);
	m_nWritePos +=nBufferSize;
	nTmp = m_nWritePos-m_nReadPos;
	m_nAllocBufferEmptySize = m_nAllocBufferSize - nTmp;

	return 0;
}

//读取数据
int FlvToStream::ReadFrameData(int & nFrameType,int & nTimeStamp,char *pBuffer,int & nBufferSize)
{
	nFrameType = m_nFrameType;
	nTimeStamp = m_nDateStamp;
	nBufferSize = m_nTempBufferSize;
	memcpy(pBuffer,m_pAllocBuffer+m_nReadPos,nBufferSize);
	//更新位置
	m_nReadPos = m_nReadPos+nBufferSize+4;//4个字节tag size
	unsigned int nTmp = m_nWritePos-m_nReadPos;
	m_nAllocBufferEmptySize = m_nAllocBufferSize - nTmp;

	return 0;
}

/*
功能：跳转至最后一帧数据
参数：nLastFrameDataType，数据帧类型，0：帧；1：视频主帧；2：视频虚帧；3：音频帧
*/
void FlvToStream::SkipToLastFrame(int nLastFrameDataType)
{
	int nFrameType = 0;
	int nTimeStamp = 0;
	int nBufferSize = 0;
	unsigned int nTmp = 0;
	while(true)
	{
		if(IsOnePackageData())
		{
			//读取数据
			nFrameType = m_nFrameType;
			nTimeStamp = m_nDateStamp;
			nBufferSize = m_nTempBufferSize;
			//更新位置
			m_nReadPos = m_nReadPos+nBufferSize+4;//4个字节tag size
			nTmp = m_nWritePos-m_nReadPos;
			m_nAllocBufferEmptySize = m_nAllocBufferSize - nTmp;
		}
		else
		{
			break;
		}
	}
}

//获取帧数据时间差
int  FlvToStream::GetDiffTime(int & nStartTimeStamp,int & nStopTimeStamp)
{
	unsigned int nReadPosOld = m_nReadPos;
	unsigned int nWritePosOld = m_nWritePos;
	bool	 bReadFileHeaderOld = m_bReadFileHeader;
	bool	 bPreviousTagSize0Read = m_bPreviousTagSize0Read;
	int nFrameType = 0;
	int nFirstTimeStamp = 0;
	int nTimeStamp = 0;
	int nBufferSize = 0;
	unsigned int nTmp = 0;
	bool bFirstFrame = false;
	while(true)
	{
		if(IsOnePackageData())
		{
			//读取数据
			nFrameType = m_nFrameType;
			nTimeStamp = m_nDateStamp;
			nBufferSize = m_nTempBufferSize;
			if(!bFirstFrame)
			{
				nFirstTimeStamp = nTimeStamp;
				nStartTimeStamp = nTimeStamp;
				nStopTimeStamp = nTimeStamp;
				bFirstFrame = true;
			}
			else
			{
				nStopTimeStamp = nTimeStamp;
			}
			//更新位置
			m_nReadPos = m_nReadPos+nBufferSize+4;//4个字节tag size
			nTmp = m_nWritePos-m_nReadPos;
			m_nAllocBufferEmptySize = m_nAllocBufferSize - nTmp;
		}
		else
		{
			break;
		}
	}
	//恢复位置
	m_bReadFileHeader = bReadFileHeaderOld;
	m_bPreviousTagSize0Read =  bPreviousTagSize0Read;
	m_nReadPos = nReadPosOld;
	m_nWritePos = nWritePosOld;
	nTmp = m_nWritePos-m_nReadPos;
	m_nAllocBufferEmptySize = m_nAllocBufferSize - nTmp;
	int nDiffTimeStamp = nTimeStamp - nFirstTimeStamp;
	return nDiffTimeStamp;
}

//获取剩余空间
unsigned int  FlvToStream::GetBufferEmptySize()
{
	return m_nAllocBufferEmptySize;
}
bool FlvToStream::Read8(int &i8,char *pBuffer)
{
	if(m_nReadPos > m_nWritePos || (m_nWritePos - m_nReadPos) < 1)
		return false;

	memcpy(&i8,m_pAllocBuffer+m_nReadPos,1);
	m_nReadPos +=1;
	m_nAllocBufferEmptySize +=1;
	return true;
}

bool FlvToStream::Read16(int &i16,char *pBuffer)
{
	if(m_nReadPos > m_nWritePos || (m_nWritePos - m_nReadPos) < 2)
		return false;

	memcpy(&i16,m_pAllocBuffer+m_nReadPos,2);
	m_nReadPos +=2;
	m_nAllocBufferEmptySize +=2;
	i16 = HTON16(i16);
	return true;
}

bool FlvToStream::Read24(int &i24,char *pBuffer)
{
	if(m_nReadPos > m_nWritePos || (m_nWritePos - m_nReadPos) < 3)
		return false;

	memcpy(&i24,m_pAllocBuffer+m_nReadPos,3);
	m_nReadPos +=3;
	m_nAllocBufferEmptySize +=3;
	i24 = HTON24(i24);
	return true;
}

bool FlvToStream::Read32(int &i32,char *pBuffer)
{
	if(m_nReadPos > m_nWritePos || (m_nWritePos - m_nReadPos) < 4)
		return false;

	memcpy(&i32,m_pAllocBuffer+m_nReadPos,4);
	m_nReadPos +=4;
	m_nAllocBufferEmptySize +=4;
	i32 = HTON32(i32);
	return true;
}

bool FlvToStream::Peek8(int &i8,char *pBuffer)
{
	if(m_nReadPos > m_nWritePos || (m_nWritePos - m_nReadPos) < 1)
		return false;

	memcpy(&i8,m_pAllocBuffer+m_nReadPos,1);
	return true;
}

bool FlvToStream::ReadTime(int &itime,char *pBuffer)
{
	int temp = 0;
	if(m_nReadPos > m_nWritePos || (m_nWritePos - m_nReadPos) < 4)
		return false;

	memcpy(&temp,m_pAllocBuffer+m_nReadPos,4);
	m_nReadPos +=4;
	m_nAllocBufferEmptySize +=4;
	itime = HTON24(temp);
	itime |= (temp & 0xff000000);
	return true;
}

bool FlvToStream::ReadHead()
{
	int headlength = 0;
	int filetype = 0;
	unsigned int nReadPosOld = m_nReadPos;
	unsigned int nAllocBufferEmptySizeOld = m_nAllocBufferEmptySize;

	if (!Read24(filetype, m_pAllocBuffer))
		return false;
	int typel = 'flv';
	int typeh = 'FLV';
	if (filetype != typeh && filetype != typel)
	{
		//printf("not flv file\n");
		return false;
	}

	m_nReadPos +=2;
	m_nAllocBufferEmptySize +=2;
	if (!Read32(headlength, m_pAllocBuffer))
		return false;
	//printf("headlength:%d\n", headlength);
	/////////跳过头部长度/////
	m_nReadPos = nReadPosOld;
	m_nAllocBufferEmptySize = nAllocBufferEmptySizeOld;
	m_nReadPos +=headlength;
	m_nAllocBufferEmptySize +=headlength;
	return true;
}

//是否有一包完整数据
bool FlvToStream::IsOnePackageData()
{
	//首先读取文件头
	if(!m_bReadFileHeader)
	{
		m_bReadFileHeader = ReadHead();
		if(!m_bReadFileHeader)
		{
			return false;
		}
	}
	//读取文件数据
	unsigned int nDataSize = m_nWritePos - m_nReadPos;
	if(!m_bPreviousTagSize0Read)
	{
		//读取PriviousTagSize 0    4字节
		if(nDataSize > 4)
		{
			m_nReadPos +=4;
			m_nAllocBufferEmptySize +=4;
			m_bPreviousTagSize0Read = true;
		}
		else
		{
			return false;
		}
	}

	//读取Tag1 ... N
	nDataSize = m_nWritePos - m_nReadPos;
	if(nDataSize > 11)
	{
		//读取tag类型，数据区长度，时间戳，时间戳扩展，StreamsID，数据区
		int type = 0;
		int time = 0;
		int htime = 0;
		int datalength = 0;
		int info = 0;
		unsigned int nReadPosOld = m_nReadPos;
		//char buff[256] = { 0 };
		if (!Read8(type, m_pAllocBuffer))
		{
			m_nReadPos = nReadPosOld;
			return false;
		}
		if (!Read24(datalength, m_pAllocBuffer))
		{
			m_nReadPos = nReadPosOld;
			return false;
		}
		if(nDataSize < (datalength+11+4))
		{
			m_nReadPos = nReadPosOld;
			return false;
		}

		if (!ReadTime(time, m_pAllocBuffer))
		{
			m_nReadPos = nReadPosOld;
			return false;
		}
		m_nFrameType = type;
		m_nDateStamp = time;
		m_nTempBufferSize = datalength;
		////////跳过StreamID////////////////
		m_nReadPos +=3;
		m_nAllocBufferEmptySize +=3;
		if (type == 9)
		{
			//视频
			VideoParse(datalength);
		}
		else if(type == 8)
		{
			//音频
			AudioParse(datalength);
		}
		else if(type == 18)
		{
			//脚本
			MediaInfoParse(datalength);
		}
		else
		{
			//保留
			int jjj = 0;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void FlvToStream::AudioParse(int datalength)
{

}

void FlvToStream::MediaInfoParse(int datalength)
{

}

void FlvToStream::VideoParse(int datelength)
{
	/*
	int h264space = 0x01000000;//H264内容间隔标识00000001
	unsigned int nReadPosOld = m_nReadPos;
	int info = 0;
	Read8(info, m_pAllocBuffer);
	int avctype = 0;
	Read8(avctype, m_pAllocBuffer);
	m_nReadPos +=3;
	int templength = 0;
	char*tempbuff = NULL;
	if (avctype == 0)
	{
		m_nReadPos +=6;
		Read16(templength, m_pAllocBuffer);
		printf("sssize:%d\n", templength);

		tempbuff = (char*)malloc(templength);
		//fread(tempbuff, 1, templength, m_pAllocBuffer);
		memcpy(tempbuff,m_pAllocBuffer+m_nReadPos,templength);
		m_nReadPos +=templength;
		fwrite(&h264space, 1, 4, h264file);
		fwrite(tempbuff, 1, templength, h264file);
		free(tempbuff);

		Read8(templength, m_pAllocBuffer);//ppsnum

		Read16(templength, m_pAllocBuffer);//ppssize
		printf("ppsize:%d\n", templength);

		tempbuff = (char*)malloc(templength);
		//fread(tempbuff, 1, templength, flvfile);
		memcpy(tempbuff,m_pAllocBuffer+m_nReadPos,templength);
		m_nReadPos +=templength;
		fwrite(&h264space, 1, 4, h264file);
		fwrite(tempbuff, 1, templength, h264file);
		free(tempbuff);
	}
	else
	{
		//	Read32(templength,flvfile);
		//	tempbuff=(char*)malloc(templength);
		//	fread(tempbuff,1,templength,flvfile);
		//	fwrite(&h264space,1,4,h264file);
		//	fwrite(tempbuff,1,templength,h264file);
		//	free(tempbuff);
		//可能存在多个nal，需全部读取
		int countsize = 2 + 3;
		while (countsize<datelength)
		{
			Read32(templength, m_pAllocBuffer);
			tempbuff = (char*)malloc(templength);
			//fread(tempbuff, 1, templength, flvfile);
			memcpy(tempbuff,m_pAllocBuffer+m_nReadPos,templength);
			m_nReadPos +=templength;
			fwrite(&h264space, 1, 4, h264file);
			fwrite(tempbuff, 1, templength, h264file);
			free(tempbuff);
			countsize += (templength + 4);
		}
	}

	m_nReadPos = nReadPosOld;
	*/
}
```

