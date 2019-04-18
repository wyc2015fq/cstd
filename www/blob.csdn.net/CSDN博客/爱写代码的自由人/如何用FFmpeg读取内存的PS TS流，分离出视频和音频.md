# 如何用FFmpeg读取内存的PS/TS流，分离出视频和音频 - 爱写代码的自由人 - CSDN博客





2018年02月22日 21:14:20[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1833








    之前做一个项目遇到一个问题：从网络中收到PS/TS流，需要从中分离出视频和音频，但是FFmpeg只支持标准的几种输入流协议（RTP/HTTP/RTSP/MMS），而我们的协议它不支持，所以就不能够用它来直接做接收。那能否由自己来接收数据然后让FFmpeg读取内存中的数据呢？答案当然是可以的，这就需要用到FFmpeg的探测流格式的功能，主要是用到了两个API: avio_alloc_context， av_probe_input_buffer。前者向FFmpeg传递用户自定义的Read，write, Seek的回调函数，即文件IO操作由用户去管理。下面是函数的原型：

```cpp
AVIOContext *avio_alloc_context(  
                  unsigned char *buffer,  
                  int buffer_size,  
                  int write_flag,  
                  void *opaque,  
                  int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),//指定从内存中读取的方法，将buf_size字节大小的数据保存到buf  
                  int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),//对应的这是写内存的函数  
                  int64_t (*seek)(void *opaque, int64_t offset, int whence));
```

     后者则用于探测输入数据的格式，比如采用什么容器封装，这个是内部自动判断的，它会先缓冲一段数据，解析成功后，函数会返回0，如果返回值为负数则表示无法识别格式。

    下面我会实现一个例子，从内存中读取数据，传递给FFmpeg，然后后面的操作就跟读文件一样了。

    第一步，我们要做的工作是：打开一个文件-》创建输入流的上下文-》调用avio_alloc_context函数并传递read_packet函数的地址-》自动探测格式-》打开输入流。代码如下：

```cpp
AVIOContext* pb = NULL;  
AVInputFormat* piFmt = NULL;  

BOOL bRet = m_file.Open(m_filePath.c_str(), CFile::modeRead);
if(!bRet)
{
   TRACE("OpenFile %s Failed\n", m_filePath.c_str());
   return -3;
}

 uint8_t  * input_buf = (uint8_t*)av_mallocz(sizeof(uint8_t)* BUF_SIZE);  
 pb = avio_alloc_context(input_buf, BUF_SIZE, 0, this, read_data, NULL, NULL);

  //探测从内存中获取到的媒体流的格式  
 if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0)
 {  
     TRACE("probe format failed\n");  
     return -4;   
 }  
 else
 {  
     TRACE("format:%s[%s]\n", piFmt->name, piFmt->long_name);  
 } 
 m_inputAVFormatCxt = avformat_alloc_context();  
 m_inputAVFormatCxt->pb = pb;  

  if (avformat_open_input(&m_inputAVFormatCxt, "", piFmt, NULL) != 0)
  {
       TRACE("Couldn't open input stream.（无法打开输入流）\n");  
       return -1;  
  }
```

然后，重写read_packet函数：

```cpp
int read_data(void *opaque, uint8_t *buf, int buf_size)  
{  
    FileStreamReadTask * pThisTask  = (FileStreamReadTask*)opaque;

	int nReadLen = pThisTask->m_file.Read(buf,  buf_size);
	TRACE("read_data: %d \n", nReadLen);

	if(nReadLen <= 0) //读取出错或已经到了文件尾部
	{
		return -1;
	}

	return nReadLen;
}
```

**   注意：这个函数你不能随便返回一个值。返回值的意义：如果大于0表示读到了数据，如果为0则表示数据已经读完了，准备结束读取操作。**

**    第二步，获取输入流的视频和音频的相关信息，后面解码会用到。**

```cpp
if (avformat_find_stream_info(m_inputAVFormatCxt, 0) < 0)
{
	TRACE("can not find stream info \n");
	return -2;
}
av_dump_format(m_inputAVFormatCxt, 0, m_filePath.c_str(), 0);
for (int i = 0; i < m_inputAVFormatCxt->nb_streams; i++)
{
	AVStream *in_stream = m_inputAVFormatCxt->streams[i];

	TRACE("codec id: %d, filepath: %s \n", in_stream->codec->codec_id, m_filePath.c_str());

	if (in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		m_videoStreamIndex = i;

		m_coded_width = in_stream->codec->width;
		m_coded_height = in_stream->codec->height;

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

m_bInputInited = TRUE;
```

    第三步，调用FFmpeg的av_read_frame函数，分离出视频包和音频包。一旦调用av_read_frame，上面我们定义的read_data函数会一直被调用（事实上前面探测的时候也被调用到），这是FFmpeg自动完成的，我们只管输出的数据就行。得到视频包和音频包，我们只对视频包做处理，对它进行解码。代码如下：

```cpp
void FileStreamReadTask::readAndDemux()
{
	int nVideoFramesNum = 0;
	int64_t  first_pts_time = 0;

	DWORD start_time = GetTickCount(); 

	AVPacket pkt;
	av_init_packet(&pkt);

	unsigned int h264_startcode = 0x01000000;

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
			}
			else
			{
				TRACE("av_read_frame() got error: %d \n", res);
			}

			break;  
		}

		AVStream *in_stream = m_inputAVFormatCxt->streams[pkt.stream_index];

		if(in_stream->codec->codec_type != AVMEDIA_TYPE_VIDEO && in_stream->codec->codec_type != AVMEDIA_TYPE_AUDIO)
		{
			continue;
		}

		if(pkt.pts < 0)
			pkt.pts = 0;


		if(in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)  //视频
		{
			nVideoFramesNum++;

			DecodeVideo(in_stream, pkt);

			int nSecs = pkt.pts*in_stream->time_base.num/in_stream->time_base.den;
			TRACE("Frame time: %02d:%02d \n", nSecs/60, nSecs%60);

		}
		else if(in_stream->codec->codec_type == AVMEDIA_TYPE_AUDIO) //音频
		{

		}

		if((in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)	)
		{
			if(first_pts_time == 0)
				first_pts_time = pkt.pts;

			int64_t pts_time = (pkt.pts - first_pts_time)*1000*in_stream->time_base.num/in_stream->time_base.den; //转成毫秒
			int64_t now_time = GetTickCount() - start_time; 

			if(pts_time > now_time + 10 && pts_time < now_time + 3000)
			{
				Sleep(pts_time-now_time);
			}
			else if(pts_time == 0 && nVideoFramesNum > 1) //如果pts_time为0表示没有时间戳
			{
				Sleep(20);
			}

		}

		av_free_packet(&pkt);

	}//while

	TRACE("Reading ended, read %d video frames \n", nVideoFramesNum);

	CloseFileInputStream();

}
```

   DecodeVideo函数实现如下：

```cpp
int FileStreamReadTask::DecodeVideo(AVStream * st, AVPacket & dec_pkt)
{
	if(!m_bVideoDecoderInited)
	{
		if (avcodec_open2(st->codec, avcodec_find_decoder(st->codec->codec_id), NULL) < 0)
		{
			TRACE("Could not open video codec.（无法打开解码器）\n");
			return -1;
		}

		if(m_pframe == NULL)
		{
			m_pframe = av_frame_alloc();
		}

		if (!m_pframe) 
		{
			//ret = AVERROR(ENOMEM);
			return -3;
		}

		m_bVideoDecoderInited = TRUE;
	}

	int dec_got_frame = 0;
	int ret = avcodec_decode_video2(st->codec, m_pframe, &dec_got_frame, &dec_pkt);
	if (ret < 0) 
	{
		TRACE("Decoding failed------------\n");
		return -4;
	}
	if (dec_got_frame)
	{
           if(m_pVideoCBFunc)
	    {
		   m_pVideoCBFunc(st, st->codec->pix_fmt, m_pframe, /*av_gettime() - m_start_time*/ dec_pkt.pts);
	    }
	}

	return 0;
}
```

    下面贴出媒体流处理类的头文件（下面有代码链接）。

```cpp
//图像、音频包解码后调用的回调函数
typedef LRESULT (CALLBACK* VideoCaptureCB)(AVStream * input_st, enum PixelFormat pix_fmt, AVFrame *pframe, INT64 lTimeStamp);
typedef LRESULT (CALLBACK* AudioCaptureCB)(AVStream * input_st, AVFrame *pframe, INT64 lTimeStamp);

class FileStreamReadTask
{
public:
    FileStreamReadTask();
    virtual ~FileStreamReadTask();

    int  OpenMediaFile(const char* szFilePath); //打开媒体文件

    BOOL StartReadFile(); //开始播放

    void StopReadFile(); //停止播放，并关闭文件

    void GetVideoSize(long & width, long & height)  //获取视频分辨率
	{
		width  = m_coded_width;
		height = m_coded_height;
	}

	//设置播放完毕通知的窗口句柄
     void  SetNotifyWnd(HWND hNotify)
	{
		m_hNotifyWnd = hNotify;
	}

	//设置视频图像的回调函数
	void  SetVideoCaptureCB(VideoCaptureCB pFuncCB);

	//设置音频的回调函数
	void  SetAudioCaptureCB(AudioCaptureCB pFuncCB);

private:
    void run();

    void InitData(); //打开文件前初始化变量

    int  OpenFileInputStream(); //打开输入流
    void CloseFileInputStream(); //关闭输入流

    void readAndDemux(); //从文件中分离出音频包和视频包

     static DWORD WINAPI ReadingThrd(void * pParam); //读文件和解码线程

     void openOutputStream();  //打开输出容器
     void closeOutputStream(); //关闭输出容器

     int  DecodeVideo(AVStream * st, AVPacket & dec_pkt);
private:

    string m_filePath; //输入的文件路径
    string m_outputUrl; //输出流地址，可以为文件路径或URL

    AVFormatContext* m_inputAVFormatCxt;

    int m_videoStreamIndex;
    int m_audioStreamIndex;

    AVFormatContext* m_outputAVFormatCxt;

    char m_tmpErrString[64];
    bool m_stop_status; //退出标志，等于1时退出

    HANDLE m_hReadThread; //读文件的线程句柄

    BOOL   m_bInputInited;  //是否成功打开输入流（文件）
    BOOL   m_bOutputInited; //是否成功打开输出流

    AVFrame *    m_pframe;
    BOOL         m_bVideoDecoderInited; //是否已经初始化解码器

    VideoCaptureCB  m_pVideoCBFunc; //视频数据回调函数指针
    AudioCaptureCB  m_pAudioCBFunc; //音频数据回调函数指针

    int   m_coded_width, m_coded_height; //视频的宽高
    int   m_frame_rate; //帧率

    HWND    m_hNotifyWnd; //消息通知的窗口句柄

public:
  //uint8_t   *  m_input_buf;
   CFile        m_file;
  
};
```

代码资源：[https://download.csdn.net/download/zhoubotong2012/10836358](https://download.csdn.net/download/zhoubotong2012/10836358)











