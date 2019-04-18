# 用FFmpeg实现的RTMP接收类 - 爱写代码的自由人 - CSDN博客





2019年01月10日 10:36:23[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：362








      现在很多直播都用到RTMP来传输，而接收RTMP用FFmpeg比较常见（当然也有用其他库的）。FFmpeg对RTMP接收有比较完善实现了，API使用方法也很简单，大多数流程跟文件流处理一样，但是一些区别的地方。下面大概说一下用FFmpeg怎么实现RTMP接收功能，后面再附上封装类的代码。

1.  初始化和打开流。

 这是第一步要做的工作，打开了流后才能往下接收数据，打开流需要调用FFmpeg的API avformat_open_input函数，这个函数连接网络的时候会阻塞的，所以要设置超时值，否则有时候会阻塞很久时间。怎么设置超时时间？

   AVDictionary* options = nullptr;   

    av_dict_set(&options, "stimeout", "3000000", 0);  //设置超时断开连接时间  

上面两行代码设置了连接的超时时间为3秒，但是我试过了，到了超时时间函数还没有返回，好像是FFmpeg的问题。但没有关系，另外我们还有一种方法检测超时，就是通过异常回调函数，avformat_open_input函数可以传入一个回调函数地址作为参数，如果发生连接超时、接收超时，可以直接在回调函数里通知，这样可以使avformat_open_input函数马上返回，避免阻塞太久。

下面是打开流的代码：

```
BOOL RtmpStreamSession::openInputStream()
{
    int res = 0;

    bool bIsNetPath = false;

	if(_strnicmp(m_InputUrl.c_str(), "rtsp://", 7) == 0 || _strnicmp(m_InputUrl.c_str(), "RTSP://", 7) == 0)
	{
		bIsNetPath = true;
	}
	else if(_strnicmp(m_InputUrl.c_str(), "rtmp://", 7) == 0 || _strnicmp(m_InputUrl.c_str(), "RTMP://", 7) == 0)
	{
		bIsNetPath = true;
	}
	else
	{
		bIsNetPath = false;
	}

	if(bIsNetPath) //从网络接收
	{
		//Initialize format context
		m_inputAVFormatCxt = avformat_alloc_context();

		//Initialize intrrupt callback
		AVIOInterruptCB icb = {interruptCallBack,this};
		m_inputAVFormatCxt->interrupt_callback = icb;
	}


	m_dwLastRecvFrameTime = 0;
	m_dwStartConnectTime = GetTickCount();

	//m_inputAVFormatCxt->flags |= AVFMT_FLAG_NONBLOCK;

    AVDictionary* options = nullptr;   
    av_dict_set(&options, "stimeout", "3000000", 0);  //设置超时断开连接时间  

	//m_inputAVFormatCxt->max_analyze_duration = 2000000;
    // m_inputAVFormatCxt->fps_probe_size = 30;

    res = avformat_open_input(&m_inputAVFormatCxt, m_InputUrl.c_str(), 0, &options);

    if(res < 0)
    {
        string strError = "can not open file:" + m_InputUrl + ",errcode:" + to_string(res) + ",err msg:" + av_make_error_string(m_tmpErrString, AV_ERROR_MAX_STRING_SIZE, res);
		TRACE("%s \n", strError.c_str());
		return FALSE;
    }

	TRACE("%d, %d, %d \n", m_inputAVFormatCxt->max_analyze_duration, m_inputAVFormatCxt->probesize, m_inputAVFormatCxt->fps_probe_size);

  //获得各个流的信息

	return TRUE;
}
```

    OpenInputStream函数首先检测输入的URL是不是RTSP或RTMP地址，如果不是，则当作文件流。对网络流，需要传入一个回调函数地址，看下面代码：

```
if(bIsNetPath) //从网络接收
	{
		//Initialize format context
		m_inputAVFormatCxt = avformat_alloc_context();

		//Initialize intrrupt callback
		AVIOInterruptCB icb = {interruptCallBack,this};
		m_inputAVFormatCxt->interrupt_callback = icb;
	}
```

上面传入的异常回调函数是interruptCallback，下面代码是异常回调函数的实现：

```
static int interruptCallBack(void *ctx)
{
    RtmpStreamSession * pSession = (RtmpStreamSession*) ctx;

   //once your preferred time is out you can return 1 and exit from the loop
    if(pSession->CheckTimeOut(GetTickCount()))
    {
      return 1;
    }

   //continue 
   return 0;

}

BOOL   RtmpStreamSession::CheckTimeOut(DWORD dwCurrentTime)
{
	if(dwCurrentTime < m_dwLastRecvFrameTime) //CPU时间回滚
	{
		return FALSE;
	}

	if(m_stop_status)
		return TRUE;

	if(m_dwLastRecvFrameTime > 0)
	{
		if((dwCurrentTime - m_dwLastRecvFrameTime)/1000 > m_nMaxRecvTimeOut) //接收过程中超时
		{
		    return TRUE;
		}
	}
	else
	{
		if((dwCurrentTime - m_dwStartConnectTime)/1000 > m_nMaxConnectTimeOut) //连接超时
		{
			return TRUE;
		}
	}
	return FALSE;
}
```

如果interruptCallback返回1则退出连接或接收循环，返回0则继续接收。如果是前者，则avformat_open_input函数马上返回，这样就可以通过自己指定超时来限制连接的时间。

2. 获得视频流和音频流的信息。

```
if (avformat_find_stream_info(m_inputAVFormatCxt, 0) < 0)
    {
        TRACE("can not find stream info \n");
		return FALSE;
    }
    av_dump_format(m_inputAVFormatCxt, 0, m_InputUrl.c_str(), 0);
    for (int i = 0; i < m_inputAVFormatCxt->nb_streams; i++)
    {
        AVStream *in_stream = m_inputAVFormatCxt->streams[i];

		if (in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			m_videoStreamIndex = i;

			coded_width = in_stream->codec->width;
			coded_height = in_stream->codec->height;

			if(in_stream->avg_frame_rate.den != 0 && in_stream->avg_frame_rate.num != 0)
			{
			  m_frame_rate = in_stream->avg_frame_rate.num/in_stream->avg_frame_rate.den;//每秒多少帧 
			}

			TRACE("video stream index: %d, width: %d, height: %d, FrameRate: %d\n", m_videoStreamIndex, in_stream->codec->width, in_stream->codec->height, m_frame_rate);
		}
		else if (in_stream->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			m_audioStreamIndex = i;
		}
    }

	 //m_bsfcAAC = av_bitstream_filter_init("aac_adtstoasc");
  //  if(!m_bsfcAAC)
  //  {
  //      TRACE("can not create aac_adtstoasc filter \n");
  //  }

	m_bsfcH264 = av_bitstream_filter_init("h264_mp4toannexb");
    if(!m_bsfcH264)
    {
        TRACE("can not create h264_mp4toannexb filter \n");
    }
```

主要是获取视频流和音频流的索引，视频宽、高等信息。代码还调用av_bitstream_filter_init函数创建了一个转换Filter，这句代码有什么用呢？后面会讲到。

3.  接收和分离流。

调用av_read_frame函数解析网络接收到的数据并分离出视频包和音频包。

```
void RtmpStreamSession::readAndDemux()
{
	DWORD start_time = GetTickCount(); 

    AVPacket pkt;
	av_init_packet(&pkt);

    while(1)
    {
        if(m_stop_status == true)
        {
            break;
        }

        int res;
       
        res = av_read_frame(m_inputAVFormatCxt, &pkt);
        if (res < 0)  //读取错误或读到了文件尾
        {
			if(AVERROR_EOF == res)
			{
				TRACE("End of file \n");

				break;
			}
			else
			{
				TRACE("av_read_frame() got error: %d, pkt.size = %d \n", res, pkt.size);

			  if(res == -11)
			  {
				 Sleep(10);
			     continue;
			  }

			  break;
			}

			//break;  
        }

		AVStream *in_stream = m_inputAVFormatCxt->streams[pkt.stream_index];

        Demuxer(in_stream, pkt);
		av_free_packet(&pkt);

		m_dwLastRecvFrameTime = GetTickCount();

    }//while

	ReleaseCodecs();
}
```

Demuxer函数负责处理分离后的视频包和音频包，然后对数据进行解码或写到文件。

```
int RtmpStreamSession::Demuxer(AVStream *pStream, AVPacket & pkt)
{
	if(pStream->codec->codec_type != AVMEDIA_TYPE_VIDEO 
		&& pStream->codec->codec_type != AVMEDIA_TYPE_AUDIO)
	{
		return 0;
	}

	if(pkt.pts < 0)
		pkt.pts = 0;

	int64_t pts_time = (pkt.pts)*90000*pStream->time_base.num/pStream->time_base.den; //转成90KHz为单位

	if(pStream->codec->codec_type == AVMEDIA_TYPE_VIDEO)  //视频
	{
		if(pStream->codec->codec_id == AV_CODEC_ID_H264)
		{
			m_nVideoFramesNum++;

			if(!(pkt.data[0] == 0x0 && pkt.data[1] == 0x0 && pkt.data[2] == 0x0 && pkt.data[3] == 0x01))
			{
				//TRACE("Not H264 StartCode!\n");

				AVPacket tempPack; 
				av_init_packet(&tempPack);
				//av_copy_packet(&tempPack, &pkt); 

				int nRet = av_bitstream_filter_filter(m_bsfcH264, pStream->codec, NULL, &tempPack.data, &tempPack.size, pkt.data, pkt.size, 0);

				if(nRet >= 0)
				{
					if(m_pfd != NULL) //保存视频数据
					{
						fwrite( tempPack.data, tempPack.size, 1, m_pfd);		
					}


					DecodeVideo(pStream, tempPack);

					if(tempPack.data != NULL)
					{
						av_free(tempPack.data); //一定要加上这句，否则会有内存泄漏
						tempPack.data = NULL;
					}

				}
				else
				{
					TRACE("av_bitstream_filter_filter got error: %d \n", nRet);
				}

				//TRACE("FrameNo: %d,  size: %d \n", m_nVideoFramesNum,  pkt.size);
			}
			else
			{

				if(m_pfd != NULL) //保存视频数据
				{
					fwrite( pkt.data, pkt.size, 1, m_pfd);		
				}

				DecodeVideo(pStream, pkt);
			}


		}

		//int nSecs = pkt.pts*in_stream->time_base.num/in_stream->time_base.den;
		//TRACE("Frame time: %02d:%02d \n", nSecs/60, nSecs%60);


	}
	else if(pStream->codec->codec_type == AVMEDIA_TYPE_AUDIO) //音频
	{

		AVPacket tempPack; 
		av_init_packet(&tempPack);

		if(pStream->codec->codec_id == AV_CODEC_ID_AAC)
		{
			int nOutAACLen = 0;

			AAC_TO_ADTS(pkt.data, pkt.size, pStream->codec->sample_rate, m_aacADTSBuf, ONE_AUDIO_FRAME_SIZE, &nOutAACLen);

			tempPack.data = m_aacADTSBuf;
			tempPack.size = nOutAACLen;
			tempPack.pts = pkt.pts;
		}
		else
		{
			av_copy_packet(&tempPack, &pkt);
		}
		
		DecodeAudio(tempPack.data, tempPack.size, pStream->codec->codec_id, TRUE);
	}

	return 0;
}
```

**注意：分离出来后的H264视频数据并不能马上解码，因为从RTP包中提取出的视频H264数据并不是一个标准的NALU单元，它没有包含H264前缀码，需要使用FFMPEG中的名为＂h264_mp4toannexb＂的bitstream filter 进行处理。在上面代码中调用了h264_mp4toannexb_filter函数， 它对分离出的H264帧进行转换，处理后就得到标准的ES流。**

**另外，对分离出来的AAC音频帧需要插入ADTS头，否则也不能正确解码的。**

4.  解码音频、视频，或保存数据到文件。

解码实现详细请看这两个函数：

int RtmpStreamSession::DecodeVideo(AVStream * st, AVPacket & dec_pkt) 

bool RtmpStreamSession:: DecodeAudio(PBYTE pData, int nDataLen, AVCodecID audioID,  BOOL bPlayAudio)

5. 接收结束，释放相关资源。

```
void RtmpStreamSession::ReleaseCodecs()
{
	if (m_pfd != nullptr)
	{
		fclose(m_pfd);
		m_pfd = nullptr;
	}

	if(m_pframe)
	{
		av_frame_free(&m_pframe);
		m_pframe = NULL;
	}

	m_bVideoDecoderInited = FALSE;

	if(m_pAudioCodecCtx != NULL)
	{
		avcodec_close(m_pAudioCodecCtx);
		av_free(m_pAudioCodecCtx);

        m_pAudioCodecCtx = NULL;
		m_pAudioCodec = NULL;
	}

	if(m_pAudioFrame != NULL)
	{
		av_free(m_pAudioFrame);
		m_pAudioFrame = NULL;
	}

	if(m_pSamples != NULL)
	{
		av_free(m_pSamples);
		m_pSamples = NULL;
	}


	if(m_aacADTSBuf != NULL)
	{
		delete m_aacADTSBuf;
		m_aacADTSBuf = NULL;
	}

	 m_stop_status = true;
}
```

封装类的代码下载链接：[http://download.csdn.net/download/zhoubotong2012/10906954](https://download.csdn.net/download/zhoubotong2012/10906954)



