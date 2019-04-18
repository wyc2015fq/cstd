# 如何用FFmpeg API采集摄像头视频和麦克风音频，并实现录制文件的功能 - 爱写代码的自由人 - CSDN博客





置顶2018年02月19日 22:08:02[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：10006








    之前一直用Directshow技术采集摄像头数据，但是觉得涉及的细节比较多，要开发者比较了解Directshow的框架知识，学习起来有一点点难度。最近发现很多人问怎么用FFmpeg采集摄像头图像，事实上FFmpeg很早就支持通过DShow获取采集设备（摄像头、麦克风）的数据了，只是网上提供的例子比较少。如果能用FFmpeg实现采集、编码和录制（或推流），那整个实现方案就简化很多，正因为这个原因，我想尝试做一个FFmpeg采集摄像头视频和麦克风音频的程序。经过一个星期的努力，终于做出来了。我打算把开发的心得和经验分享给大家。我分三部分来讲述：首先第一部分介绍如何用FFmpeg的官方工具（ffmpeg.exe）通过命令行来枚举DShow设备和采集摄像头图像，这部分是基础，能够快速让大家熟悉怎么用FFmpeg测试摄像头采集；第二部分介绍我写的采集程序的功能和用法；第三部分讲解各个模块包括采集、编码、封装和录制是如何实现的。

### 1.用命令行枚举采集设备和采集数据

  打开Cmd命令行控制台，进入FFmpeg的Bin目录，输入如下命令：

  ffmpeg -list_devices true -f dshow -i dummy  

  则在我的机器上显示如下结果：

![](https://img-blog.csdn.net/20180219211317255)

  在上面的命令行窗口中列出了两个设备，一个是视频采集设备，另外是一个音频采集设备。另外，我们发现：音频设备的名称有乱码，因为其中有中文名称，后面在讲到用API采集数据的时候会提到解决这个问题的方法。

  接着我们输入另外一个命令行：

  ffmpeg -list_options true -f dshow -i video="USB 2861 Device"

  这个命令行的作用是获取指定视频采集设备支持的分辨率、帧率和像素格式等属性，返回的是一个列表，结果如下：

![](https://img-blog.csdn.net/20180219211502942)

  这里我们看到采集设备支持的最大分辨率是720x576，输出像素格式是yuyv422，支持的帧率为29.97和25FPS。

  下面我们执行另外一条命令，将摄像头的图像和麦克风的音频录制保存成一个文件。命令如下：

  ffmpeg -f dshow -i video="USB 2861 Device" -f dshow -i audio="线路 (3- USB Audio Device)" -vcodec libx264 -acodec aac -strict -2 mycamera.mkv

  上面的命令行用video=指定视频设备，用audio=指定音频设备，后面的参数是定义编码器的格式和属性，输出为一个名为mycamera.mkv的文件。

  命令运行之后，控制台打印FFmpeg的运行日志，按“Q”键则中止命令。

  这里有些读者可能会问：采集设备不是支持多个分辨率吗？怎么设置采集时用哪一种分辨率输出？答案是用“-s”参数设置，若在上面的命令行加上“-s 720x576”，则FFmpeg就会以720x576的分辨率进行采集，如果不设置，则以默认的分辨率输出。

  注意：如果你运行上面命令ffmpeg报如下错误：Could not run filter

  Video=XXX:Input/output error

![](https://img-blog.csdn.net/20180219211656324)

  则说明该版本的ffmpeg不支持该采集设备**。这是由于旧版本的FFmpeg一个Bug引起的，不支持需要连接crossbar连接的视频采集设备（详情可参考这个帖子：https://ffmpeg.zeranoe.com/forum/viewtopic.php?t=722）。如果读者运行下面的Demo遇到打开视频采集设备失败,可能也是这个问题引起的. 新版本的FFmpeg（avdevice-58）修复了这个问题。所以如果你遇到这个问题，可以通过升级FFmpeg来解决。**

  好，关于命令行的内容就介绍完了。

### 2.采集程序的使用

  这个程序叫“AVCapture”，能从视频采集设备（摄像头，采集卡）获取图像，支持图像预览；还可以采集麦克风音频；支持对视频和音频编码，支持录制成文件。这是一个MFC开发的窗口程序，界面比较简洁，如下图：

![](https://img-blog.csdn.net/20180219211831164)

   开始采集前需要选择设备，点击文件菜单的“打开设备”，弹出一个设备选择对话框，如下图所示：

![](https://img-blog.csdn.net/20180219211959939)

  在对话框里选择任意一个视频设备和音频设备，如果想启用某种设备，必须勾选右边的“启用”选项，但如果只需要用其中一种采集设备，则可以把其中一个禁用掉。

  按“确定”则开始采集数据了。视频和音频会编码后保存到一个文件中，这个文件的路径是在配置文件中设置的，打开程序目录下的Config.ini文件，则显示如下字段：

[Client]

file_path = D:\camera.mkv

File_path就是录制文件的路径。

  采集的图像默认显示到中间的窗口中，如果不想预览，可以在主菜单栏的“编辑”菜单中取消勾选“预览视频”。

### 3.功能模块实现

    该采集程序实现了枚举采集设备，采集控制、显示图像、视频/音频编码和录制的功能，其中输入（Input）、输出（Output）和显示（Paint）这三个模块分别用一个单独的类进行封装：CAVInputStream，CAVOutputStream，CImagePainter。CAVInputStream负责从采集设备获取数据，提供接口获取采集设备的属性，以及提供回调函数把数据传给上层。CAVOutputStream负责对采集的视频和音频流进行编码、封装，保存成一个文件。而CImagePainter则用来显示图像，使用了GDI绘图，把图像显示到主界面的窗口。

3.1 枚举采集设备

  采集前我们需要先选择设备，把所有的设备名称列出来，其中一个方法可以用第一节介绍的运行ffmpeg命令行工具来列举，但是这样有两个问题：第一，假如设备名称带中文，则显示的名称有乱码，因此，我们不知道它真实的名称。第二，ffmpeg没有API返回系统中安装的采集设备列表，虽然FFmpeg提供了API把设备名称列举出来，但是是打印到控制台的，不是通过参数来返回，如下面这段代码只能打印输出结果到控制台。但是对于窗口界面程序，没有控制台，怎么获取命令行结果呢？

```cpp
AVFormatContext *pFmtCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options, "list_devices", "true", 0);
    AVInputFormat *iformat = av_find_input_format("dshow");
    //printf("Device Info=============\n");
    avformat_open_input(&pFmtCtx, "video=dummy", iformat, &options);
   //printf("========================\n");
```

  我用了一种最传统的做法来解决，就是通过Directshow的COM接口来枚举设备，工程里面的EnumDevice接口就实现了枚举设备的功能，函数原型如下：

```cpp
//枚举指定类型的所有采集设备的名称
ENUMDEVICE_API HRESULT EnumDevice(CAPTURE_DEVICE_TYPE type, char * deviceList[], int nListLen, int & iNumCapDevices);
```

   当然，如果读者用的采集设备是固定一种，那么可以固定采集设备的名称，这样做可以省点事。 

3.2 注册FFmpeg库

```cpp
av_register_all(); 
 avdevice_register_all();
```

  这两个API可以在程序的构造函数和窗口初始化里面调用。

3.3 打开输入设备

    首先需要指定采集设备的名称。如果是视频设备类型，则名称以“video=”开头；如果是音频设备类型，则名称以“audio=”开头。调用avformat_open_input接口打开设备，将设备名称作为参数传进去，**注意这个设备名称需要转成****UTF-8****编码**。然后调用avformat_find_stream_info获取流的信息，得到视频流或音频流的索引号，之后会频繁用到这个索引号来定位视频和音频的Stream信息。接着，调用avcodec_open2打开视频解码器或音频解码器，实际上，我们可以把设备也看成是一般的文件源，而文件一般采用某种封装格式，要播放出来需要进行解复用，分离成裸流，然后对单独的视频流、音频流进行解码。虽然采集出来的图像或音频都是未编码的，但是按照FFmpeg的常规处理流程，我们需要加上“解码”这个步骤。

```cpp
int i;
       m_pInputFormat = av_find_input_format("dshow");
       ASSERT(m_pInputFormat != NULL);

	if(!m_video_device.empty())
	{
		int res = 0;

		string device_name = "video=" + m_video_device;

		string device_name_utf8 = AnsiToUTF8(device_name.c_str(), device_name.length());  //转成UTF-8，解决设备名称包含中文字符出现乱码的问题

		 //Set own video device's name
		if ((res = avformat_open_input(&m_pVidFmtCtx, device_name_utf8.c_str(), m_pInputFormat, &device_param)) != 0)
		{
			ATLTRACE("Couldn't open input video stream.（无法打开输入流）\n");
			return false;
		}
		//input video initialize
		if (avformat_find_stream_info(m_pVidFmtCtx, NULL) < 0)
		{
			ATLTRACE("Couldn't find video stream information.（无法获取流信息）\n");
			return false;
		}
		m_videoindex = -1;
		for (i = 0; i < m_pVidFmtCtx->nb_streams; i++)
		{
			if (m_pVidFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				m_videoindex = i;
				break;
			}
		}

		if (m_videoindex == -1)
		{
			ATLTRACE("Couldn't find a video stream.（没有找到视频流）\n");
			return false;
		}
		if (avcodec_open2(m_pVidFmtCtx->streams[m_videoindex]->codec, avcodec_find_decoder(m_pVidFmtCtx->streams[m_videoindex]->codec->codec_id), NULL) < 0)
		{
			ATLTRACE("Could not open video codec.（无法打开解码器）\n");
			return false;
		}
	}

    //////////////////////////////////////////////////////////

	if(!m_audio_device.empty())
	{
		string device_name = "audio=" + m_audio_device;

		string device_name_utf8 = AnsiToUTF8(device_name.c_str(), device_name.length());  //转成UTF-8，解决设备名称包含中文字符出现乱码的问题

		//Set own audio device's name
		if (avformat_open_input(&m_pAudFmtCtx, device_name_utf8.c_str(), m_pInputFormat, &device_param) != 0){

			ATLTRACE("Couldn't open input audio stream.（无法打开输入流）\n");
			return false;
		}

		//input audio initialize
		if (avformat_find_stream_info(m_pAudFmtCtx, NULL) < 0)
		{
			ATLTRACE("Couldn't find audio stream information.（无法获取流信息）\n");
			return false;
		}
		m_audioindex = -1;
		for (i = 0; i < m_pAudFmtCtx->nb_streams; i++)
		{
			if (m_pAudFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				m_audioindex = i;
				break;
			}
		}
		if (m_audioindex == -1)
		{
			ATLTRACE("Couldn't find a audio stream.（没有找到音频流）\n");
			return false;
		}
		if (avcodec_open2(m_pAudFmtCtx->streams[m_audioindex]->codec, avcodec_find_decoder(m_pAudFmtCtx->streams[m_audioindex]->codec->codec_id), NULL) < 0)
		{
			ATLTRACE("Could not open audio codec.（无法打开解码器）\n");
			return false;
		}
	}
```

3.4 初始化输出流

  前面我们已经初始化了InputStream，现在需要对OutputStream进行初始化，而要初始化输出流需要知道视频采集的分辨率，帧率，输出像素格式等信息，还有音频采集设备的采样率，声道数，Sample格式，而这些信息可通过CAVInputStream类的接口来获取到。下面是初始化OutputStream的代码：

```cpp
m_InputStream.SetVideoCaptureCB(VideoCaptureCallback);
	m_InputStream.SetAudioCaptureCB(AudioCaptureCallback);

	bool bRet;
	bRet = m_InputStream.OpenInputStream(); //初始化采集设备
	if(!bRet)
	{
		MessageBox(_T("打开采集设备失败"), _T("提示"), MB_OK|MB_ICONWARNING);
		return 1;
	}

	int cx, cy, fps;
	AVPixelFormat pixel_fmt;
	if(m_InputStream.GetVideoInputInfo(cx, cy, fps, pixel_fmt)) //获取视频采集源的信息
	{
		m_OutputStream.SetVideoCodecProp(AV_CODEC_ID_H264, fps, 500000, 100, cx, cy); //设置视频编码器属性
	}

       int sample_rate = 0, channels = 0;
	AVSampleFormat  sample_fmt;
	if(m_InputStream.GetAudioInputInfo(sample_fmt, sample_rate, channels)) //获取音频采集源的信息
	{
		m_OutputStream.SetAudioCodecProp(AV_CODEC_ID_AAC, sample_rate, channels, 32000); //设置音频编码器属性
	}

	//从Config.INI文件中读取录制文件路径
	P_GetProfileString(_T("Client"), "file_path", m_szFilePath, sizeof(m_szFilePath));

	bRet  = m_OutputStream.OpenOutputStream(m_szFilePath); //设置输出路径
	if(!bRet)
	{
		MessageBox(_T("初始化输出失败"), _T("提示"), MB_OK|MB_ICONWARNING);
		return 1;
	}
```

  在上面的代码片段中，首先设置了视频和音频的数据回调函数。当采集开始时，视频和音频数据就会传递给相应的函数去处理，在该程序中，回调函数主要对图像或音频进行编码，然后封装成FFmpeg支持的容器（例如mkv/avi/mpg/ts/mp4）。另外，需要初始化OutputStream的VideoCodec和AudioCodec的属性，在我的程序中，视频编码器是H264，音频编码器用AAC，通过CAVInputStream对象获得输入流的信息之后再赋值给输出流相应的参数。最后调用m_OutputStream对象的OpenOutputStream成员函数打开编码器和录制的容器，其中我们需要传入一个输出文件路径作为参数，这个为录制的文件路径，路径是在Config.ini文件里配置的。如果OpenOutputStream函数返回true，则表示初始化输出流成功。

3.5 读取采集数据

 接着，我们就可以开始采集了。开始采集的函数实现如下：

```cpp
bool  CAVInputStream::StartCapture()
{
	if (m_videoindex == -1 && m_audioindex == -1)
	{
		ATLTRACE("错误：你没有打开设备\n");
		return false;
	}

    m_start_time = av_gettime();

	m_exit_thread = false;

	if(!m_video_device.empty())
	{
		m_hCapVideoThread = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        CaptureVideoThreadFunc,       // thread function name
        this,          // argument to thread function 
        0,                      // use default creation flags 
        NULL);   // returns the thread identifier 
	}

	if(!m_audio_device.empty())
	{
		m_hCapAudioThread = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        CaptureAudioThreadFunc,       // thread function name
        this,          // argument to thread function 
        0,                      // use default creation flags 
        NULL);   // returns the thread identifier 
	}

	return true;
}
```

   StartCapture函数分别建立了一个读取视频包和读取音频包的线程，两个线程各自独立工作，分别从视频采集设备，音频采集设备获取到数据，然后进行后续的处理。（**注意：两个线程同时向一个文件写数据可能会有同步的问题，FFmpeg内部可能没有做多线程安全访问的处理，所以最好在自己线程里加一个锁进行互斥，从而保护临界区的安全**）

  其中，读取摄像头数据的线程的处理代码如下：

```cpp
DWORD WINAPI CAVInputStream::CaptureVideoThreadFunc(LPVOID lParam)
{
	CAVInputStream * pThis = (CAVInputStream*)lParam;

	pThis->ReadVideoPackets();

	return 0;
}

int  CAVInputStream::ReadVideoPackets()
{
	if(dec_pkt == NULL)
	{
		////prepare before decode and encode
		dec_pkt = (AVPacket *)av_malloc(sizeof(AVPacket));
	}

	int encode_video = 1;
	int ret;

	//start decode and encode

	while (encode_video)
	{
		if (m_exit_thread)
			break;

		AVFrame * pframe = NULL;
		if ((ret = av_read_frame(m_pVidFmtCtx, dec_pkt)) >= 0)
		{
			pframe = av_frame_alloc();
			if (!pframe) 
			{
				ret = AVERROR(ENOMEM);
				return ret;
			}
			int dec_got_frame = 0;
			ret = avcodec_decode_video2(m_pVidFmtCtx->streams[dec_pkt->stream_index]->codec, pframe, &dec_got_frame, dec_pkt);
			if (ret < 0) 
			{
				av_frame_free(&pframe);
				av_log(NULL, AV_LOG_ERROR, "Decoding failed\n");
				break;
			}
			if (dec_got_frame)
			{
                if(m_pVideoCBFunc)
				{
					CAutoLock lock(&m_WriteLock);

					m_pVideoCBFunc(m_pVidFmtCtx->streams[dec_pkt->stream_index], m_pVidFmtCtx->streams[m_videoindex]->codec->pix_fmt, pframe, av_gettime() - m_start_time);
				}

				av_frame_free(&pframe);
			}
			else 
			{
				av_frame_free(&pframe);
			}

			av_free_packet(dec_pkt);
		}
		else
		{
			if (ret == AVERROR_EOF)
				encode_video = 0;
			else
			{
				ATLTRACE("Could not read video frame\n");
				break;
			}
		}
	}

	return 0;
}
```

  在CAVInputStream::ReadVideoPackets()函数中不停地调用 av_read_frame读取采集到的图像帧，接着调用avcodec_decode_video2进行“解码”，这样获得了原始的图像，图像可能是RGB或YUV格式。解码后的图像通过m_pVideoCBFunc指向的回调函数回调给上层处理，回调函数里可进行后续的一些操作，比如对视频帧编码或直接显示。

3.6 编码、封装成文件

  CAVInputStream的工作线程里面读取到的视频帧和音频包通过回调函数传给CAVOuputStream类去处理。下面是两个回调函数的实现：

```cpp
//采集到的视频图像回调
LRESULT CALLBACK VideoCaptureCallback(AVStream * input_st, enum PixelFormat pix_fmt, AVFrame *pframe, INT64 lTimeStamp)
{
	if(gpMainFrame->IsPreview())
	{
	   gpMainFrame->m_Painter.Play(input_st, pframe);
	}

	gpMainFrame->m_OutputStream.write_video_frame(input_st, pix_fmt, pframe, lTimeStamp);
	return 0;
}

//采集到的音频数据回调
LRESULT CALLBACK AudioCaptureCallback(AVStream * input_st, AVFrame *pframe, INT64 lTimeStamp)
{
	gpMainFrame->m_OutputStream.write_audio_frame(input_st, pframe, lTimeStamp);
	return 0;
}
```

  视频回调函数调用了CAVOutputStream的成员函数write_video_frame，这个函数对传入的图像帧进行编码（H264），并且写到指定的封装文件；而音频回调函数则调用了CAVOutputStream的另外一个成员函数write_audio_frame，这个函数负责对音频编码（AAC），然后输出到指定的封装文件。下面是Write_video_frame函数的实现代码：

```cpp
//input_st -- 输入流的信息
//input_frame -- 输入视频帧的信息
//lTimeStamp -- 时间戳，时间单位为/1000000
//
int CAVOutputStream::write_video_frame(AVStream * input_st, enum PixelFormat pix_fmt, AVFrame *pframe, INT64 lTimeStamp)
{
	if(video_st == NULL)
	   return -1;

	//ATLTRACE("Video timestamp: %ld \n", lTimeStamp);

   if(m_first_vid_time1 == -1)
   {
	   TRACE("First Video timestamp: %ld \n", lTimeStamp);
	   m_first_vid_time1 = lTimeStamp;
   }

	AVRational time_base_q = { 1, AV_TIME_BASE };

	if(img_convert_ctx == NULL)
	{
		//camera data may has a pix fmt of RGB or sth else,convert it to YUV420
		img_convert_ctx = sws_getContext(m_width, m_height,
			pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
	}

    sws_scale(img_convert_ctx, (const uint8_t* const*)pframe->data, pframe->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
    pFrameYUV->width = pframe->width;
    pFrameYUV->height = pframe->height;
    pFrameYUV->format = PIX_FMT_YUV420P;

    enc_pkt.data = NULL;
    enc_pkt.size = 0;
    av_init_packet(&enc_pkt);

    int ret;
    int enc_got_frame = 0;
    ret = avcodec_encode_video2(pCodecCtx, &enc_pkt, pFrameYUV, &enc_got_frame);

    if (enc_got_frame == 1)
	{
        //printf("Succeed to encode frame: %5d\tsize:%5d\n", framecnt, enc_pkt.size);
       
		if(m_first_vid_time2 == -1)
		{
			m_first_vid_time2 = lTimeStamp;
		}

        enc_pkt.stream_index = video_st->index;						
	
		//enc_pkt.pts= av_rescale_q(lTimeStamp, time_base_q, video_st->time_base);
		enc_pkt.pts = (INT64)video_st->time_base.den * lTimeStamp/AV_TIME_BASE;

        m_vid_framecnt++;
		
        ret = av_interleaved_write_frame(ofmt_ctx, &enc_pkt);
		if(ret < 0)	
		{
			char tmpErrString[128] = {0};
			ATLTRACE("Could not write video frame, error: %s\n", av_make_error_string(tmpErrString, AV_ERROR_MAX_STRING_SIZE, ret));
			av_packet_unref(&enc_pkt);
			return ret;
		}

        av_free_packet(&enc_pkt);
	}
	else if(ret == 0)
	{
		ATLTRACE("Buffer video frame, timestamp: %I64d.\n", lTimeStamp); //编码器缓冲帧
	}

	return 0;
}
```

   Write_video_frame和write_audio_frame是CAVOutputStream的两个很重要的函数，其中对音频包的处理略为复杂一些，主要是因为输入的音频和编码后的音频的frame_size不一样，中间需要一个Fifo作缓冲队列。另外时间戳PTS的计算也是很关键的，弄得不好保存的文件播放视音频就不同步了，具体怎么实现你们看代码吧。

  工程的代码下载地址：[点击打开链接](http://download.csdn.net/download/zhoubotong2012/10252187)





















