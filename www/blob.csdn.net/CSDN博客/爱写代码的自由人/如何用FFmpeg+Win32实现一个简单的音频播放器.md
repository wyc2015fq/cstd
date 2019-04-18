# 如何用FFmpeg+Win32实现一个简单的音频播放器 - 爱写代码的自由人 - CSDN博客





2018年02月25日 13:14:25[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：371








    这个音频播放器是别人写的，我看到代码结构比较清晰，代码量比较精简，所以就拿来作为一个例子给大家讲解一下。这个播放器的界面是用Win32开发的，界面很简单，上面是菜单栏，下面是一个播放进度条，界面截图如下：

![](https://img-blog.csdn.net/20180225112855915)


 这个播放器使用FFmpeg来解析音频文件格式和解码音频，播放音频用到了DirectSound。这里要说一下播放音频的方法，在Windows平台，播放音频的方法有很多，比较常用的是：DirectSound，WaveOut API，MCI函数，SDL，其中WaveOut API和MCI函数的用法可参考下面两篇博文：

https://www.cnblogs.com/margin1988/archive/2012/05/10/2493937.html


http://blog.csdn.net/dancewyr/article/details/7183572


下面我说一下这个代码的调用流程。

 首先，程序创建窗口的时候会建立一个线程：

```cpp
case WM_CREATE:
		{
			// start main play loop thread
			HANDLE hThreadReadyEvent = CreateEvent(NULL, TRUE, FALSE, _T("ThreadReadyEvent"));

			StartParameter startParam = {hWnd, hThreadReadyEvent};

			dwMainLoopThreadId = 0;
			hMainLoopThread = CreateThread(NULL, 0, MainLoopThreadProc, &startParam, 
				CREATE_SUSPENDED, &dwMainLoopThreadId);

			ResumeThread(hMainLoopThread);
			WaitForSingleObject(hThreadReadyEvent, INFINITE);

			CloseHandle(hThreadReadyEvent);
			hThreadReadyEvent = NULL;
		}
		break;
```

 线程函数：MainLoopThreadProc用来从文件中读取和解析音频，MainLoopThreadProc函数实现如下：

```cpp
DWORD WINAPI MainLoopThreadProc(VOID* pParam)
{
	StartParameter* pStartParam = (StartParameter*)pParam;
	BOOL bTerminateThread = FALSE;

	// register all codecs.
	av_register_all();
	SetEvent(pStartParam->hThreadReadyEvent);

	AudioState state = {0};
	int buffer_size = (AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2;
	memset(&state, 0, sizeof(AudioState));

	int err = 0;
	char filepath[MAX_PATH] = {0};

	__int64 duration = 0;
	int totalSeconds = 0;
	int minute = 0;
	int second = 0;
	int totalMinute = 0;
	int totalSecond = 0;

	int channels = 0;
	int samplerate = 0;
	int bitpersample = 16;

	DS_Info* info = NULL;
	int frameSize = 0;
	long bufferSize = 0;
	int bytesPerSec = 0;
	long waitTime = 0;
	int deviceCount = 0;

	DWORD readBytes = 0;
	char input_buffer[BLOCK_SIZE] = {0};
	TCHAR szTime[20] = {0};
	TCHAR szPosition[10] = {0};

	do 
	{
		MSG msg = {0};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch(msg.message)
			{
			case TM_PLAY:
				{

#ifdef _UNICODE
					WideCharToMultiByte(CP_ACP, 0, szFileName, -1, filepath, sizeof(filepath) / sizeof(char), NULL, NULL);
#else
					memcpy(filepath, szFileName, sizeof(filepath) / sizeof(char));
#endif

					err = avformat_open_input(&state.pFmtCtx, filepath, NULL, NULL);
					if(err < 0)
					{
						TCHAR buffer[2*MAX_PATH];
						_stprintf_s(buffer, sizeof(TCHAR) * (2 * MAX_PATH), _T("can not open file %s."), filepath);
						MessageBox(hMainWindow, buffer, _T("Error"), MB_OK | MB_ICONEXCLAMATION);

						Playing = FALSE;

						if(hMainLoopThread && dwMainLoopThreadId)
							PostThreadMessage(dwMainLoopThreadId, TM_EOF, 0, 0);

						break;
					}

					err = av_find_stream_info(state.pFmtCtx);
					if(err < 0)
					{
						TCHAR buffer[2*MAX_PATH];
						_stprintf_s(buffer, sizeof(TCHAR) * (2 * MAX_PATH), _T("can not find stream info of file %s."), filepath);
						MessageBox(hMainWindow, buffer, _T("Error"), MB_OK | MB_ICONEXCLAMATION);

						Playing = FALSE;

						if(hMainLoopThread && dwMainLoopThreadId)
							PostThreadMessage(dwMainLoopThreadId, TM_EOF, 0, 0);

						break;
					}

					int index = -1;
					for(unsigned int i = 0; i < state.pFmtCtx->nb_streams; i++)
					{
						if(state.pFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
						{
							state.pCodecCtx = state.pFmtCtx->streams[i]->codec;
							index = i;
							state.stream_index = i;
							break;
						}
					}

					if(!state.pCodecCtx)
					{
						MessageBox(hMainWindow, _T("can not get codec context."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
						av_close_input_file(state.pFmtCtx);

						Playing = FALSE;

						if(hMainLoopThread && dwMainLoopThreadId)
							PostThreadMessage(dwMainLoopThreadId, TM_EOF, 0, 0);

						break;
					}

					state.pCodec = avcodec_find_decoder(state.pCodecCtx->codec_id);
					if(!state.pCodec || avcodec_open(state.pCodecCtx, state.pCodec) < 0)
					{
						MessageBox(hMainWindow, _T("can not open codec."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
						av_close_input_file(state.pFmtCtx);

						Playing = FALSE;

						if(hMainLoopThread && dwMainLoopThreadId)
							PostThreadMessage(dwMainLoopThreadId, TM_EOF, 0, 0);

						break;
					}

					duration = state.pFmtCtx->duration;
					totalSeconds = (int)(duration / 1000000L);
					gTotalSeconds = totalSeconds;
					totalMinute = (int)(totalSeconds / 60);
					totalSecond = (int)(totalSeconds % 60);

					state.audio_buf1 = (uint8_t*)av_mallocz(buffer_size);
					state.buffer_size = buffer_size;

					channels = state.pCodecCtx->channels;
					samplerate = state.pCodecCtx->sample_rate;

					bitpersample = 16;
					switch(state.pCodecCtx->sample_fmt)
					{
					case AV_SAMPLE_FMT_U8:
						bitpersample = 8;
						break;
					case AV_SAMPLE_FMT_S16:
						bitpersample = 16;
						break;
					case AV_SAMPLE_FMT_S32:
						bitpersample = 32;
						break;
					case AV_SAMPLE_FMT_FLT:
						bitpersample = sizeof(double) * 8;
						break;
					default:
						bitpersample = 0;
						break;
					}

					frameSize = (channels == 1) ? 2 : 4;
					bufferSize = millis2bytes(samplerate, 500, frameSize);
					bytesPerSec = samplerate * frameSize;

					waitTime = bytes2millis(samplerate, bufferSize, frameSize) / 4;
					if(waitTime < 10) waitTime = 1;
					if(waitTime > 1000) waitTime = 1000;

					deviceCount = DAUDIO_GetDirectAudioDeviceCount();
					info = (DS_Info*)DAUDIO_Open(0, 0, 1, DAUDIO_PCM, (float)samplerate,
						bitpersample, frameSize, channels, TRUE, FALSE, bufferSize);

					if(info != NULL && DAUDIO_Start((void*)info, TRUE))
					{
						Playing = TRUE;
						PostMessage(hTrackBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
					}
				}
				break;
			case TM_STOP:
				{
					if(info)
					{
						DAUDIO_Stop((void*)info, TRUE);
						DAUDIO_Close((void*)info, TRUE);
						info = NULL;

						av_free(state.audio_buf1);
						state.audio_buf1 = NULL;

						avcodec_close(state.pCodecCtx);
						state.pCodecCtx = NULL;
						av_close_input_file(state.pFmtCtx);
						state.pFmtCtx = NULL;
					}

					Playing = FALSE;
					bTerminateThread = TRUE;
				}
				break;
			case TM_EOF:
				{
					if(info)
					{
						DAUDIO_Stop((void*)info, TRUE);
						DAUDIO_Close((void*)info, TRUE);
						info = NULL;

						av_free(state.audio_buf1);
						state.audio_buf1 = NULL;

						avcodec_close(state.pCodecCtx);
						state.pCodecCtx = NULL;
						av_close_input_file(state.pFmtCtx);
						state.pFmtCtx = NULL;
					}

					SendMessage(hTrackBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

					_stprintf_s(szTime, sizeof(szTime)/sizeof(szTime[0]), _T("%02d:%02d/%02d:%02d"), 
						0, 0, 0, 0);
					SendMessage(hStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)szTime);

					RECT r = {0};
					GetClientRect(hStatic, &r);
					InvalidateRect(hStatic, &r, FALSE);

					Playing = FALSE;
				}
				break;
			case TM_SEEK:
				{
					if(Playing == TRUE && info != NULL)
					{
						MSG msg2;
						if (PeekMessage(&msg2, NULL, TM_SEEK, TM_SEEK, PM_NOREMOVE) == FALSE)
						{
							int seekPosition = (int)msg.wParam;
							int timestamp = (int)(gTotalSeconds * (float)seekPosition/100);
							int ret = av_seek_frame(state.pFmtCtx, -1, timestamp * AV_TIME_BASE, 0);
							avcodec_flush_buffers(state.pCodecCtx);
							info->bytesPos = timestamp * bytesPerSec;
							DAUDIO_Flush((void*)info, TRUE);
						}
					}
				}
				break;
			default:
				break;
			}
		}

		if(bTerminateThread == TRUE)
			break;

		if(Playing == TRUE && info != NULL)
		{
			memset(input_buffer, 0, sizeof(input_buffer));
			readBytes = 0;

			readBytes = read_buffer(&state, input_buffer, BLOCK_SIZE);
			if(readBytes == -1)
			{
				Sleep(250);
				Playing = FALSE;

				if(hMainLoopThread && dwMainLoopThreadId)
					PostThreadMessage(dwMainLoopThreadId, TM_EOF, 0, 0);

				SendMessage(hTrackBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

				_stprintf_s(szTime, sizeof(szTime)/sizeof(szTime[0]), _T("%02d:%02d/%02d:%02d"), 
					0, 0, 0, 0);
				SendMessage(hStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)szTime);

				RECT r = {0};
				GetClientRect(hStatic, &r);
				InvalidateRect(hStatic, &r, FALSE);

				goto NextLoop;
			}

			DWORD len = readBytes;
			DWORD offset = 0;
			DWORD written = 0;

			for( ; ; )
			{
				int thisWritten = DAUDIO_Write((void*)info, input_buffer+offset, len);
				if(thisWritten < 0)
					break;

				len -= thisWritten;
				written += thisWritten;
				if(len > 0)
				{
					offset += thisWritten;
					Sleep(waitTime);
				}
				else break;
			}

			// update progress
			{
				__int64 wFp = DAUDIO_GetLongFramePosition((void*)info, TRUE);
				__int64 byteLen = wFp * frameSize;
				gCurPlaySeconds = (int)byteLen / bytesPerSec;

				int seconds = (int)byteLen / bytesPerSec;
				minute = (int)(seconds / 60);
				second = (int)(seconds % 60);

				_stprintf_s(szTime, sizeof(szTime)/sizeof(szTime[0]), _T("%02d:%02d/%02d:%02d"), 
					minute, second, totalMinute, totalSecond);
				SendMessage(hStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)szTime);

				RECT r = {0};
				GetClientRect(hStatic, &r);
				InvalidateRect(hStatic, &r, FALSE);

				float percent = (float)seconds / totalSeconds;
				int position = round(percent * 100);
				if(position >= 100)
					position = 100;

				gPosition = position;
				PostMessage(hTrackBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)position);

				// _stprintf_s(szPosition, 10, _T("%02d\n"), position);
				// OutputDebugString(szPosition);
			}
		}
		else
		{
			WaitMessage();
		}

NextLoop:
		;
	} while (bTerminateThread == FALSE);

	return 0;
}
```

MainLoopThreadProc函数代码有点长，为了方便大家了解它的处理逻辑，我将它简化一下：

```cpp
DWORD WINAPI MainLoopThreadProc(VOID* pParam)
{
	//初始化变量

        do 
	{
		MSG msg = {0};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch(msg.message)
			{
				case TM_PLAY: //播放开始
					break;
				case TM_STOP: //播放中止并且退出线程
					break;
				case TM_EOF: //播放中止或完成
					break;
				case TM_SEEK: //定位时间点
					break;
			} 
		}

		  if(bTerminateThread == TRUE) //退出线程
			break;


		   //如果是正在播放，则继续调用read_buffer读取音频数据，把解码后得到的PCM音频放到input_buffer里面

		   //调用DAUDIO_Write把input_buffer的PCM音频写到DirectSound的输出缓冲区，回放声音

		   //更新进度条进度
		
	}while (bTerminateThread == FALSE);

	return 0;
}
```

由上面的代码可以很容易看出，MainLoopThreadProc所做的工作是：它在一个While循环里通过PeekMessage不停地从消息队列里获取界面发过来的音频控制消息，消息类型有几种：TM_PLAY--播放开始  TM_STOP--播放中止和退出线程  TM_EOF--播放中止或完成  TM_SEEK--定位到时间点。当收到TM_PLAY命令，即开始播放时，程序会调用FFmpeg的API： avformat_open_input来打开一个音频文件，获取到音频格式，然后调用avcodec_open来打开指定格式的音频解码器。接着，初始化DirectSound的输出缓存，调用DAUDIO_Start开始回放音频。当收到TM_EOF命令，表示界面要停止播放，或者是播放顺利结束了需要销毁资源，这时候，调用FFmpeg API：av_close_input_file关闭文件，并且关闭音频解码器，以及停止音频回放、销毁DirectSound的缓冲区。当要退出程序时，向线程发出TM_STOP的命令，同时将bTerminateThread变量设为TRUE，当线程函数检查到这个变量的值为TRUE时就会退出线程。

   读取和解码音频的工作主要是在read_buffer函数完成：

```cpp
int read_buffer(AudioState* pState, void* buffer, int buf_size)
{
	int len = buf_size;
	uint8_t* pbuffer = (uint8_t*)buffer;
	int audio_size = 0;
	int len1 = 0;
	int size = 0;

	//pState->audio_buf  解码后音频的输出Buffer
	//pState->data   存放解码前数据的Buffer
	while(len > 0)
	{
		if(pState->audio_buf_index >= (int)pState->audio_buf_size)
		{
			audio_size = audio_decode_frame(pState); //audio_size为解码后的数据大小
			if(audio_size < 0)
				return (size > 0) ? size : -1;

			pState->audio_buf_size = audio_size;
			pState->audio_buf_index = 0;
		}

		if(pState->audio_buf_size - pState->audio_buf_index != audio_size)
		{
			//OutputDebugString(_T("Not equal\n"));
		}

		len1 = pState->audio_buf_size - pState->audio_buf_index;// (pState->audio_buf_size - pState->audio_buf_index == audio_size)
		if(len1 > len)
			len1 = len;

		memcpy(pbuffer, (uint8_t *)pState->audio_buf + pState->audio_buf_index, len1);

		len -= len1;
		pbuffer += len1;
		size += len1;
		pState->audio_buf_index += len1;
	}

	return size;
}
```

它里面又调用了一个audio_decode_frame函数，用于解码音频，解码后的音频帧放在AudioState类型指针指向的结构里面，其中pState->audio_buf是解码后音频的输出Buffer，pState->data是存放解码前数据的Buffer。audio_decode_frame函数的代码如下：

```cpp
int audio_decode_frame(AudioState* pState)
{
	AVPacket* pkt_temp = &pState->audio_pkt_temp;
	AVPacket* pkt = &pState->audio_pkt;
	AVCodecContext* dec= pState->pCodecCtx;
	int len = 0, data_size = sizeof(pState->audio_buf1);
	int err = 0;

	for( ; ; )
	{
		int i=0;
		while (pkt_temp->size > 0)
		{
			data_size = pState->buffer_size;
			len = avcodec_decode_audio3(dec, (int16_t*)pState->audio_buf1, &data_size, pkt_temp);
			if (len < 0)
			{
				/* if error, we skip the frame */
				pkt_temp->size = 0;
				break;
			}

			if(i>=1)
			{
				OutputDebugString(_T("avcodec_decode_audio3 \n"));
			}
			i++;

			pkt_temp->data += len;
			pkt_temp->size -= len;

			if (data_size <= 0)
				continue;

			pState->audio_buf = pState->audio_buf1;
			return data_size;
		}

		if (pkt->data)
			av_free_packet(pkt);

		if((err = av_read_frame(pState->pFmtCtx, pkt)) < 0)
			return -1;

		pkt_temp->data = pkt->data;
		pkt_temp->size = pkt->size;

		//ATLTRACE("av_read_frame size: %d \n", pkt->size);
	}

	return -1;
}
```

   工程的代码下载地址： http://download.csdn.net/download/zhoubotong2012/10258103

   另外还有一个用WaveOut API实现回放音频的例子：http://download.csdn.net/download/zhoubotong2012/10257451


 （注：这个程序的定位时间播放是有问题的，有兴趣的读者自己下代码后去改善！）





