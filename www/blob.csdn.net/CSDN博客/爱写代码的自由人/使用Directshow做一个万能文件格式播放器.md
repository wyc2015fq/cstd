# 使用Directshow做一个万能文件格式播放器 - 爱写代码的自由人 - CSDN博客





2018年02月23日 21:30:22[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：2312








在Windows平台上实现一个文件播放器有什么好的开发库和方案呢？方案有很多，比如基于FFmpeg，VLC的插件，mplayer，Directshow。用FFmpeg来实现文件格式解析、分离视频音频流、解码是很方便的，但是要实现一个播放器，还要实现视音频的显示和回放、视音频同步的处理，要做很多额外的开发工作，比较麻烦。而用VLC的插件不方便调试，扩充功能要改VLC的源代码，不灵活。而用Directshow来做播放器在Windows平台上是比较成熟和简单的方案，所以特别给大家介绍一下。

微软的Directshow是一个实现多媒体播放，视音频处理等功能的多媒体库，并且提供了Filter的机制使得多媒体任务的各个处理单元（读取数据、分离、解码、编码、回放）达到模块化，提高了复用性。开发者可以用别人提供的Filter，也可以实现自己的Filter来处理特定的任务。每个Filter至少有一个Pin，Pin分输入Pin和输出Pin，按照Filter的类型输入Pin和输出Pin的数目不等。每个Pin都有自己接受的媒体格式，比如视频渲染器Filter的输入Pin只接受RGB或YUV的格式。两个Filter的Pin连接前需要对Pin进行媒体类型协商（确定两方的Pin的媒体类型是否一致），如果协商成功，两个Pin就可以连接起来。一连串的Filter通过这种机制连接形成一个Graph，从而完成复杂的多媒体任务，比如播放文件，录制麦克风的音频，采集摄像头图像。下面演示了一个播放AVI文件的Graph的Filter连接图：

![](https://img-blog.csdn.net/2018022320411159)


在上面的Graph图中，包括几种Filter：Source Filter，Splitter，Video Decoder，Audio Decoder，Video Renderer，Audio Renderer。而这些Filter大部分系统已经提供了，还有少数功能需要第三方的Filter来完成。

 因为多媒体文件的格式众多，所以播放文件的Graph需要加入支持不同格式的Filter，其中最主要的Filter是：分离器，解码器。解码器Filter目前有很多了，免费的而又出名的解码器Filter有FFDshow。FFDshow底层用到了FFmpeg，基于Directshow框架来实现，解码效率很高，但是比较庞大和臃肿，现在已基本不维护了。并且FFDshow只提供了编码器和解码器，还没有实现分离器。以前微软平台上的Directshow开发员会了支持尽量多的媒体格式，想尽办法从各处收集到各种分离器Filter，结果程序打包之后体积很大，并且兼容各种格式要插入不同的的Filter使程序变得复杂。很多第三方Filter不开源，加上测试不够，稳定性不好，容易产生很多问题。幸好，现在有个老外开发了一套Directshow插件，叫LAV Filters。不错，是一套，包括分离器，视频解码器和音频解码器。只要安装这套插件，播放大多数的多媒体格式基本上没问题了，省了开发员很多功夫。还有，这个LAV Filters套件还是开源的，作者对工程的更新速度很快，已经修复了很多Bug，现在变得很稳定了，连大名鼎鼎的开源播放器MPC也使用了LAV Filters，将它列为优先选用的插件。

说了那么多，大家一定很想使用LAV Filters来做开发吧？不用急，我下面就介绍如何在Directshow Graph中使用它。

首先，我们要知道这几个Filter的CLSID，下面是这几个Filter的CLSID定义：

  DEFINE_GUID(CLSID_LAVSplitter,


                0x171252A0, 0x8820, 0x4AFE, 0x9D, 0xF8, 0x5C, 0x92, 0xB2, 0xD6, 0x6B, 0x04);
  DEFINE_GUID(CLSID_LAVVideoDecoder, 

                0xEE30215D, 0x164F, 0x4A92, 0xA4, 0xEB, 0x9D, 0x4C, 0x13, 0x39, 0x0F, 0x9F);

  DEFINE_GUID(CLSID_LAVAudioDecoder,

                0xE8E73B6B, 0x4CB3, 0x44A4, 0xBE, 0x99, 0x4F, 0x7B, 0xCB, 0x96, 0xE4, 0x91);

  另外还有一个**LAV Source Filter**，CLSID是：

  DEFINE_GUID(CLSID_LAVSource, 


                0xB98D13E7, 0x55DB, 0x4385, 0xA3, 0x3D, 0x09, 0xFD, 0x1B, 0xA2, 0x63, 0x38);

 这个LAV Source Filter的功能跟LAVSplitter差不多，都是分离器，不同的是它没有输入Pin，它集成了了Async Source Filter + LAVSplitter的功能。

下面我们就准备在Directshow的Graph中加入这些Filter。首先，我们需要实现一个类来封装播放文件的所有接口，下面是类的声明：

```cpp
class CDXGraph
{
private:
	IGraphBuilder *     mGraph;  
	IMediaControl *	    mMediaControl;
	IMediaEventEx *	    mEvent;
	IBasicVideo *	    mBasicVideo;
	IBasicAudio *	    mBasicAudio;
	IVideoWindow  *	    mVideoWindow;
	IMediaSeeking *	    mSeeking;
	IBaseFilter *       m_pSourceFilter;
	IBaseFilter *       m_pDumpFilter;

	DWORD		    mObjectTableEntry; 
	UINT                m_GraphMsg;

public:
	bool           m_bEnableSound;
	int            m_nJPEGQuality;
	SIZE           m_PictureSize;
	UINT           m_nFramerate;

public:
	CDXGraph();
	virtual ~CDXGraph();

public:
	virtual bool Create(void);
	virtual void Release(void);
	virtual bool Attach(IGraphBuilder * inGraphBuilder);

	IGraphBuilder * GetGraph(void); // Not outstanding reference count
	IMediaEventEx * GetEventHandle(void);

        UINT   GetGraphState();

	bool   SetClipSourceRect(RECT rcSource);
	bool   SetDisplayWindow(HWND inWindow, LPRECT  rcTarget);
	bool   SetNotifyWindow(HWND inWindow, long lMsg);
	bool   ResizeVideoWindow(long inLeft, long inTop, long inWidth, long inHeight);

	void   HandleEvent(LONG * eventCode);

	bool   Run(void);        // Control filter graph
	bool   Stop(void);
	bool   Pause(void);

	bool   IsRunning(void);  // Filter graph status
	bool   IsStopped(void);
	bool   IsPaused(void);

	//bool SetFullScreen(BOOL inEnabled);
	//bool GetFullScreen(void);

	// IMediaSeeking
	bool    GetCurrentPosition(LONGLONG * outPosition); //获得当前播放时间(单位为100纳秒, 等于10^(-4) ms )
	bool    GetStopPosition(LONGLONG * outPosition);
	bool    SetCurrentPosition(LONGLONG inPosition); //设置当前播放时间(单位为100纳秒, 等于10^(-4) ms )
	bool    SetStartStopPosition(LONGLONG inStart, LONGLONG inStop);
	bool    GetDuration(LONGLONG * outDuration); //获得文件时间长度(单位为100纳秒, 等于10^(-4) ms )
	bool    SetPlaybackRate(double inRate); //设置播放速度
	bool    GetPlaybackRate(double * outRate);

	// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
	bool    SetAudioVolume(long inVolume);//调节音量
	long    GetAudioVolume(void);
	// Attention: range from -10000(left) to 10000(right), and 0 is both.
	bool    SetAudioBalance(long inBalance);
	long    GetAudioBalance(void);

	bool    RenderFile(const TCHAR * inFile, DWORD & dwError);
	bool    SnapshotBitmap(const TCHAR * outFile);
	bool    GetTotalFrames(LONGLONG * outNum);

private:
	void    AddToObjectTable(void) ;
	void    RemoveFromObjectTable(void);
	
	bool    QueryInterfaces(void);

	HRESULT ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	void    DisconnectFilters(IPin * inOutputPin);

	HRESULT  RenderFilter(IBaseFilter * pFilter);
	HRESULT  GetVideoProps(IPin * pVideoOutputPin);
};
```

 接着列出最核心的一个函数CDXGraph::RenderFile，它负责创建FilterGraph，将各个Filter添加到Graph和连接起来。下面是函数的实现：

```cpp
bool CDXGraph::RenderFile(const TCHAR * inFile, DWORD & dwError)
{
	if (mGraph == NULL)
	{
	   dwError = ERROR_INVALID_POINTER;
           return false;
	}

	dwError = 0;

	HRESULT hr;
	
	bool bVideoPinConnected = false;
	bool bAudioPinConnected = false;
	bool bPrivateStreamPinConnected = false;

#ifndef UNICODE
	WCHAR   wszFilePath[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP, 0, inFile, -1, wszFilePath, MAX_PATH);
#else
	TCHAR   wszFilePath[MAX_PATH] = {0};
    lstrcpy(wszFilePath, inFile);
#endif
     
	CComPtr<IFileSourceFilter> pFileSource;
	CComPtr<IBaseFilter> pSplitter;
	CComPtr<IBaseFilter> pVideoDecoder;
	CComPtr<IBaseFilter> pVideoRenderer;

#if 0
	hr = AddFilterByCLSID(mGraph, CLSID_MpegSourceFilter, L"Mpeg Splitter ", &pSplitter );
	if(FAILED(hr))
	{
		OutputDebugString("Add Mpeg Splitter Filter Failed \n");
		return FALSE;
	}
#else
	hr = AddFilterByCLSID(mGraph, CLSID_LAVSource, L"LAV Source Splitter ", &pSplitter );
	if(FAILED(hr))
	{
		OutputDebugString("Add LAV Splitter Filter Failed \n");
		return FALSE;
	}
#endif
	hr  = pSplitter->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSource);
	if(FAILED(hr))
	{
		dwError = ERROR_GET_INTERFACE_FAIL;
		return false;
	}

	hr = pFileSource->Load(wszFilePath, NULL);
	if(FAILED(hr))
	{
		dwError = ERROR_LOADFILE_FAIL;
		return false;
	}

	hr = AddFilterByCLSID(mGraph, CLSID_LAVVideoDecoder, L"LAV Video Decoder", &pVideoDecoder);
	if(FAILED(hr))
	{
		OutputDebugString("Add LAV Video Filter Failed \n");
		//return FALSE;
	}
		  
	hr = AddFilterByCLSID(mGraph, CLSID_VideoMixingRenderer9, L"VMR9 Renderer ", &pVideoRenderer);
	if(FAILED(hr))
	{
		OutputDebugString("Add VMR9 Renderer Filter Failed \n");
		return FALSE;
	}

	hr = RenderFilter(pSplitter);
	if(SUCCEEDED(hr))
		bVideoPinConnected = true;
	else
		bVideoPinConnected = false;

	if(!bAudioPinConnected && !bVideoPinConnected)
	{
	    OutputDebugString("RenderFilter Failed!\n");

	    OutputDebugString("第二次尝试用RenderFile自动连接\n");

		if (FAILED(mGraph->RenderFile(wszFilePath, NULL)))
		{
			OutputDebugString("RenderFile Failed!\n\n");

			dwError = ERROR_AUTO_RENDERFILE_FAIL;
			return false;
		}
		
		IBaseFilter * pVideoRenderer = NULL;
		hr = FindVideoRenderer(mGraph, &pVideoRenderer);
		if(SUCCEEDED(hr))
		{
		  CComPtr<IPin> pVideoPin = GetInPin(pVideoRenderer, 0);
		  GetVideoProps(pVideoPin);
		}
	}

	if(!m_bEnableSound)
	{
	   SetAudioVolume(-10000);
	}

	OutputDebugString("RenderFile Succeeded!\n");

   return true;
}
```

CDXGraph::RenderFile函数显示加入了这几个Filter：LAV Source，LAV Video Decoder，VMR9。其中，VMR9是视频渲染器，负责渲染图像和根据时间戳控制视频帧何时显示；LAVSource负责读取文件和从文件容器里分离出视频流和音频流，当我们调用AddFilterByCLSID函数（实际上调用了COM接口CoCreateInstance函数）创建这个Filter实例时，它是没有加载文件的，我们必须查询它的IFileSourceFilter接口指针，通过这个接口指针设置文件路径，把文件加载进去，如果文件加载成功，LAV Source会根据文件容器里媒体流的数目生成对应的OutputPin。接着，加入LAV Video Decoder，然后调用RenderFilter函数把LAV Source的每个OutputPin自动与下游的Filter进行连接，因为连接Filter的时候Graph Manager会优先选用已经添加到Graph中的Filter，那么LAV Source的Video Output Pin就会尝试与Video Decoder进行连接，而Audio Output Pin也一样，但是由于我们没有显示加入任何的Audio Decoder，Graph Manager会从系统安装的Filter中找到一个合适的解码器Filter插入进去，然后自动连接两个Filter的OutputPin与InputPin。如果所有Filter连接成功，那么播放文件Graph的Filter链路图就像下面这样子：

![](https://img-blog.csdn.net/20180224104827515)


上面所讲的内容是Directshow开发很基本的操作，相信熟悉Directshow的读者看了觉得很熟悉和简单。

我的资源了提供了一个例子：基于Directshow框架的万能文件格式播放器，欢迎大家下载：http://download.csdn.net/download/zhoubotong2012/10255913。（注：资源包里只有一个Demo 执行程序，没有工程代码，大家可以参考Directshow的例子来搞![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)。）



