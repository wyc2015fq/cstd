# 如何实现基于RTP传输的流媒体播放器（1） - 爱写代码的自由人 - CSDN博客





2019年01月16日 12:07:38[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：249








 RTP是网络上进行流媒体传输的一种常用协议，现在有很多封装RTP协议的开源库，比如：ortp， jrtplib，而其中最有名的要数jrtplib，本文给大家演示怎么用jrtplib开发一个带RTP发送和接收功能的应用程序，但这篇文章不会讲述jrtplib的基本用法知识，如果你要了解更多关于这个库的用法，可以参考这篇文章：[http://www.cnblogs.com/yuweifeng/p/7550737.html](http://www.cnblogs.com/yuweifeng/p/7550737.html)。

本文给大家演示怎么开发一个基于RTP协议的流媒体播放器。播放器从网络上接收RTP包，解包后把视频分离出来，然后用FFmpeg解码，把图像显示出来。这个流媒体播放器实现的功能比较简单，但是实现了一个典型的网络播放器的框架。该文章涉及的开发知识和技巧包括：

1. 怎么用jrtplib发送数据；

2. 怎么使用jrtplib接收数据；

3. 怎么让ffmpeg从内存中读取流媒体数据，然后进行分离和解码；

4. 怎么用双线程技术同时接收和解码视频，提高播放的效率；

5. 怎么实现一个先入先出的缓冲队列存储收到的视频帧；

该播放器的代码下载地址：[https://download.csdn.net/download/zhoubotong2012/10918971](https://download.csdn.net/download/zhoubotong2012/10918971)

这个播放器的界面如下所示：

![](https://img-blog.csdnimg.cn/20190116100256716.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pob3Vib3RvbmcyMDEy,size_16,color_FFFFFF,t_70)

这个播放器主要演示播放网络流的功能，但为了方便测试，也集成了发送流媒体的功能，界面上提供一个按钮选择一个视频文件，文件可以是H264、PS、TS等容器格式；然后，需要指定发送的目标IP和目标端口号，这里的IP可以选本机IP。点击发送，则程序会以RTP方式打包，通过UDP将数据发送到目标地址。在本程序的接收端，只需要配置接收端口，点击“开始接收“按钮则开始接收数据。注意：目标端口要和接收端口一致，发送和接收是一对一的。程序可以分布在两台机器上运行，一个作发送端，一个作接收端；也可以在一台机上测试发送和接收。
- 如何实现发送

首先，讲一下怎么实现发送流媒体的功能。我们需要创建一个RTPSession的发送对象，然后初始化相关的参数：

```
RTPSession session;

	RTPSessionParams sessionparams;
	sessionparams.SetOwnTimestampUnit(1.0 / 90000.0);
	sessionparams.SetAcceptOwnPackets(true);

	RTPUDPv4TransmissionParams transparams;
	transparams.SetPortbase(8000); //这个端口必须未被占用

	int status = session.Create(sessionparams, &transparams);
	if (status < 0)
	{
		//std::cerr << RTPGetErrorString(status) << std::endl;
		return - 1;
	}

#if 1
	RTPIPv4Address addr(ntohl(inet_addr(m_szDestIP)), m_nDestPort);
	status = session.AddDestination(addr);
#else
	unsigned long addr = ntohl(inet_addr(m_szDestIP));
	status = session.AddDestination(addr, m_nDestPort);
#endif
	if (status < 0)
	{
		//std::cerr << RTPGetErrorString(status) << std::endl;
		return -2;
	}

	session.SetDefaultPayloadType(96);
	session.SetDefaultMark(false);
	session.SetDefaultTimestampIncrement(90000.0 / 25.0);
```

这里初始化的参数包括RTP头的Payload类型（赋值为96），时间单位（1.0/90000.0），时间戳增量（90000/25=3600），以及Rtp头的MarkerBit的默认值。

接着读取一个视频文件，每次读1K字节，然后调用jrtplib的RTPSession::SendPacket函数发送数据：

```
FILE *fp_open;
	uint8_t buff[1024 * 5] = { 0 };
	DWORD  bufsize = 1024; //每次读1024字节,不超过1400就行

	DWORD dwReadBytesPerSec = 2*1024*1024/8; //读取速度
	RTPTime delay(bufsize*1.0/ dwReadBytesPerSec);

	//读取文件
	fp_open = fopen(m_szFilePath, "rb");
	while (!feof(fp_open) && g_RTPSendThreadRun)
	{
		int true_size = fread(buff, 1, bufsize, fp_open);

		int status = session.SendPacket(buff, true_size);

		Sleep(1000* bufsize/dwReadBytesPerSec);
		//RTPTime::Wait(delay); //delay for a few milliseconds
	}
```

（注意：这里读文件数据只是简单地将文件数据块读出来然后直接发送，没有对视频帧做二次封装和处理，对于某些格式比如H264，一般要求要以NALU单元来传输，以FU-A分片方式打包，然后再封装到RTP包里面，而本文的方法没有采取这种方式，大家要注意区分。）
- 如何实现接收

接收的实现较为复杂一些，用到了多线程技术和缓冲队列。本文的实现中用到两条线程，一条用于接收RTP包，从中提取出视频数据；另一条线程用于解码视频，并把视频帧转成RGB格式后显示到窗口中。用到两条线程的好处是：可以并行接收和解码，两个工作相互独立，提高视频帧的处理效率，减少播放延时。而如果用一条线程来做，它既要接收又要解码，线程中处理一个帧的时间就长一些，而这时又不能接收数据，很可能造成后面的数据包丢掉。所以，用双线程的”分工合作“方式处理效率更高。两条线程之间需要维护一个队列，其中一条线程收到数据后放到队列里，然后另外一个线程从队列里读取数据，这是一个典型的”生产者-消费者“的模型，我们需要实现一个先入先出的队列来转运”视频帧“，这个队列的定义如下：
`std::list<PacketNode_t>  m_packetList; //包列表`
其中，PacketNode_t结构体的定义为：

```
typedef struct
{
	unsigned length;
	uint8_t *buf;
}PacketNode_t;
```

下面对接收线程和解码线程的工作流程作详细介绍。

首先，程序在接收前需要创建两个线程：

```
g_RTPRecvThreadRun = true;
	g_decoding_thread_run = true;

	DWORD threadID = 0;
	m_hRecvThread   = CreateThread(NULL, 0, RTPRecvThread, this, 0, &threadID);
	m_hDecodeThread = CreateThread(NULL, 0, decoding_thread, this, 0, &threadID);
```

RTPRecvThread是RTP数据的接收线程，实现方式如下：

```
DWORD WINAPI RTPRecvThread(void* param)
{
	TRACE("RTPRecvThread began! \n");

	CPlayStreamDlg * pThisDlg = (CPlayStreamDlg*)param;

	RTPSession session;
	//WSADATA dat;
	//WSAStartup(MAKEWORD(2, 2), &dat);

	RTPSessionParams sessionparams;
	sessionparams.SetOwnTimestampUnit(1.0 / 90000.0);
	//sessionparams.SetAcceptOwnPackets(true);

	RTPUDPv4TransmissionParams transparams;
	transparams.SetPortbase(m_nRecvPort); //接收端口

	int oldBufSize = transparams.GetRTPReceiveBuffer();
	transparams.SetRTPReceiveBuffer(oldBufSize * 2);
	int status = session.Create(sessionparams, &transparams);

	int newBufSize = transparams.GetRTPReceiveBuffer();
	int oldBufSizec = transparams.GetRTCPReceiveBuffer();
	transparams.SetRTCPReceiveBuffer(oldBufSizec * 2);
	int newBufSizec = transparams.GetRTCPReceiveBuffer();

	while (g_RTPRecvThreadRun)
	{
#ifndef RTP_SUPPORT_THREAD
		int error_status = session.Poll();
#endif // RTP_SUPPORT_THREAD

		session.BeginDataAccess();
		if (session.GotoFirstSourceWithData())
		{
			do
			{
				RTPPacket *pack;

				while ((pack = session.GetNextPacket()) != NULL)
				{
					int nPayType = pack->GetPayloadType();
					int nLen = pack->GetPayloadLength();
					unsigned char *pPayData = pack->GetPayloadData();
					int nPackLen = pack->GetPacketLength();
					unsigned char *pPackData = pack->GetPacketData();
					int csrc_cont = pack->GetCSRCCount();
					int ssrc = pack->GetSSRC();
					int nTimestamp = pack->GetTimestamp();
					int nSeqNum = pack->GetSequenceNumber();

#if 0
					Writebuf((char*)pPayData, nLen);
#else			
					pThisDlg->m_cs.Lock();
					//if (pThisDlg->m_packetList.size() < MAX_PACKET_COUNT)
					{
						PacketNode_t  temNode;
						temNode.length = nLen;
						temNode.buf = new uint8_t[nLen];
						memcpy(temNode.buf, pPayData, nLen);

						pThisDlg->m_packetList.push_back(temNode); //存包列表
					}
					pThisDlg->m_cs.Unlock();
#endif

					session.DeletePacket(pack);
				}
			} while (session.GotoNextSourceWithData());
		}
		else
		{
			//Sleep(10);
		}
		session.EndDataAccess();

		Sleep(1);
	}
	session.Destroy();

	TRACE("RTPRecvThread end! \n");
	return 0;
}
```

接收线程里创建了一个RTPSession对象，这个对象是用于接收RTP包，前面一部分代码用于初始化一些参数，包括：接收端口，时间戳单位，接收缓冲区大小。然后，进入一个循环，在里面不停地读取RTP数据包，如果session.GetNextPacket()返回的指针不为空，则表示读取到一个数据包，返回的指针变量是一个RTPPacket*类型，其指向的成员变量包括RTP头的各个字段的值，以及Payload数据的内存地址和大小。我们关键要提取出Payload的数据和大小，然后把它作为一个元素插入到缓冲队列中（如下面代码所示：）

```
pThisDlg->m_cs.Lock();

PacketNode_t  temNode;
temNode.length = nLen;
temNode.buf = new uint8_t[nLen];
memcpy(temNode.buf, pPayData, nLen);

pThisDlg->m_packetList.push_back(temNode); //存包列表

pThisDlg->m_cs.Unlock();
```

上面的接收线程实现了一个“生成者”，而“消费者”是实现在另外一个线程---decoding_thread，这个线程做的工作是解码。这个线程调用了很多FFmpeg的函数，但基本的流程是：打开一个文件源或URL地址-》从源中读取各个流的信息-》初始化解码器-》解码和显示。因为我们是从网络中收数据，所以是一个网络源，从网络源中读取数据有两种方式：一种是用FFmpeg内置的协议栈的支持，比如RTSP/RTMP/RTP，还有一种方式是我们传数据给FFmpeg，FFmpeg从内存中读取我们送的数据，然后用它的Demuxer和Parser来进行分析，分离出视频和音频。这里程序使用的是第二种方式，即从网络中探测数据，然后送数据给FFmpeg去解析。探测网络数据需要调用FFmpeg的av_probe_input_buffer函数，这个函数要传入一个内存缓冲区地址和一个回调函数指针，其中回调函数是用来从网络中读数据的（即我们放到缓冲队列里的数据包）。下面的fill_iobuffer就是读数据的回调函数，而pIOBuffer指向用于存放读取数据的缓冲区地址，FFmpeg就是从这里读取数据。

```
pIObuffer = (uint8_t*)av_malloc(4096);
	pb = avio_alloc_context(
		pIObuffer,
		4096,
		0,
		param,
		fill_iobuffer,
		NULL,
		NULL);

	if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0)//探测从内存中获取到的媒体流的格式
	{
		TRACE("Error: probe format failed\n");
		return -1;
	}
	else {
		TRACE("input format:%s[%s]\n", piFmt->name, piFmt->long_name);

	}
```

回调函数fill_iobuffer调用了一个ReadBuf的函数：

```
int fill_iobuffer(void* opaque, uint8_t* buf, int bufSize)
{
	ASSERT(opaque != NULL);
	CPlayStreamDlg* p_CPSDecoderDlg = (CPlayStreamDlg*)opaque;

	//TRACE("ReadBuf----- \n");
	int nBytes = ReadBuf((char*)buf, bufSize, (void*)p_CPSDecoderDlg);
	return (nBytes > 0) ? bufSize : -1;
}
```

```
static int ReadBuf(char* data, int len, void* pContext)
{
	CPlayStreamDlg * pThisDlg = (CPlayStreamDlg*)pContext;

	int data_to_read = len;
	char * pReadPtr = data;

	while (g_RTPRecvThreadRun)
	{
		int nRead = pThisDlg->ReadNetPacket((uint8_t*)pReadPtr, data_to_read);
		if (nRead < 0)
		{
			Sleep(10);
			continue;
		}
		pReadPtr += nRead;
		data_to_read -= nRead;
		if (data_to_read > 0)
		{
			Sleep(10);
			continue;
		}
		break;
	}

	return (data_to_read > 0) ? -1 : len;
}
```

ReadBuf函数的作用就不用解释了，大家一看就明白了。它实现了一个我们前面说的“消费者”，从前面实现的缓冲队列中读取数据包，读取之后就会从队列中删除相应的元素。如果队列不为空，则直接从前面的元素读取；如果无数据，则继续等待。

读了视频帧数据之后，就到了解码，解码的代码如下：

```
while (g_decoding_thread_run)
	{
		av_read_frame(pFormatContext, pAVPacket);
		if(pAVPacket->stream_index == video_stream_index)
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pAVPacket);
			if(got_picture)
			{
				p_uint8_t_temp = pFrame->data[1];
				pFrame->data[1] = pFrame->data[2];
				pFrame->data[2] = p_uint8_t_temp;
				pFrame->data[0] += pFrame->linesize[0] * (pCodecCtx->height - 1);
				pFrame->linesize[0] *= -1;
				pFrame->data[1] += pFrame->linesize[1] * (pCodecCtx->height / 2 - 1);
				pFrame->linesize[1] *= -1;
				pFrame->data[2] += pFrame->linesize[2] * (pCodecCtx->height / 2 - 1);
				pFrame->linesize[2] *= -1;
				got_picture = sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, RGB24Data, RGB24Linesize);
				got_picture = StretchDIBits(hDC, 0, 0, PlayingWidth, PlayingHeight, 0, 0, pCodecCtx->width, pCodecCtx->height, RGB24Data[0], (BITMAPINFO*)&bmpinfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}

		av_free_packet(pAVPacket);
	}
```

FFmpeg从解码器输出的格式是YUV的，我们要转成RGB图像格式显示，所以调用了sws_scale函数来转换，最后调用Windows GDI函数---StretchDiBits来把图像显示到指定的窗口区域。

如果要停止解码，则退出线程的时候记得要释放FFmpeg创建的资源：

```
if (pFormatContext)
	{
		avformat_close_input(&pFormatContext);
		pFormatContext = NULL;
	}

	sws_freeContext(img_convert_ctx);
	av_freep(&RGB24Data[0]);
	av_frame_free(&pFrame);
	//avcodec_close(pCodecCtx);
	//av_free(pIObuffer); //调用了avformat_close_input会自动释放pIObuffer
	ReleaseDC(hwnd, hDC);
```

到此为止，一个简单的流媒体播放器的实现过程就介绍完了。

这篇博文传输的视频是直接从文件读取到的，并且不分什么格式，而对于常见的H264，一般需要以FU-A方式打包，需要对数据进行重组后再打成RTP包，我的下一篇博文会向大家介绍怎么用这种方法打包和发送H264，并且RTP协议实现不依赖于rtplib，自己管理Socket实现RTP包收发。

------------------------------------------------------------------------------------

**后记：**

2018-01-29：

测试发现用jrtplib接收数据如果数据量很大会出现丢包，弄了半天才发现原来速度瓶颈是在session.Poll()函数，这个函数会等待很久去拿数据。jrtplib库里关于这个Poll函数的说明：

    /** If you're not using the poll thread, this function must be called regularly to process incoming data

     *  and to send RTCP data when necessary.

     */

 意思是用poll thread就不需要调用这个函数，那可能就没有这个问题了（我没有验证过）。但是，这样需要用到 jthread 库。关于这点，我引用网上一篇博文里的介绍：

** jrtp-3.x 中有两种数据接收方式：第一种是用 jthread 库提供的线程自动在后台执行对数据的接收。第二种是用户自己调用 RTPSession 中的 Poll 方法。**如果采取第一种方法则要安装 jthread 库。安装 jthread-1.2.1.tar.gz ，而且 jthread-1.2.1 必须先与jrtp-3.7.1 的安装。因为在 jrtp-3.7.1 的 configure 中，会查找系统是否有编译了 jthread 库，如果有，那么编译的 jrtp 库会开启对 jthread 的支持。因此如果先编译jrtp 再编译 jthread ，编译出来的 jrtp 是没有开启对 jthread 的支持的。如果采用第二种方法，那么可以不用编译 jthread 库，而直接编译 jrtp 库。

但是，我没有安装 jthread库，干脆不用jrtplib接收了，直接自己写Socket接收数据、对RTP解包。

新的例子代码：[https://download.csdn.net/download/zhoubotong2012/10943378](https://download.csdn.net/download/zhoubotong2012/10943378)



