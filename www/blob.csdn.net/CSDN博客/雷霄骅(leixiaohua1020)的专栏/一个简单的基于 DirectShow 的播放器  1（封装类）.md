# 一个简单的基于 DirectShow 的播放器  1（封装类） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月26日 00:20:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：39









DirectShow最主要的功能就是播放视频，在这里介绍一个简单的基于DirectShow的播放器的例子，是用MFC做的，今后有机会可以基于该播放器开发更复杂的播放器软件。

注：该例子取自于《DirectShow开发指南》

首先看一眼最终结果，如图所示，播放器包含了：打开，播放，暂停，停止等功能。该图显示正在播放周杰伦的《听妈妈的话》。

![](https://img-blog.csdn.net/20131025133053484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

迅速进入主题，看一看工程是由哪些文件组成的，如下图所示

![](https://img-blog.csdn.net/20131025133059390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从上图可以看出，该工程最重要的cpp文件有两个：SimplePlayerDlg.cpp和CDXGraph.cpp。前者是视频播放器对话框对应的类，而后者是对DirectShow功能进行封装的类。尤其是后面那个类，写的很好，可以说做到了“可复用”，可以移植到其他DirectShow项目中。

**本文首先分析CDXGraph这个类，SimplePlayerDlg在下篇文章中再进行分析。**

首先看看它的头文件：

**CDXGraph.h**



```cpp
/* 雷霄骅
 * 中国传媒大学/数字电视技术
 * leixiaohua1020@126.com
 *
 */
// CDXGraph.h


#ifndef __H_CDXGraph__
#define __H_CDXGraph__

// Filter graph notification to the specified window
#define WM_GRAPHNOTIFY  (WM_USER+20)

class CDXGraph
{
private:
	//各种DirectShow接口
	IGraphBuilder *     mGraph;  
	IMediaControl *		mMediaControl;
	IMediaEventEx *		mEvent;
	IBasicVideo *		mBasicVideo;
	IBasicAudio *		mBasicAudio;
	IVideoWindow  *		mVideoWindow;
	IMediaSeeking *		mSeeking;

	DWORD				mObjectTableEntry; 

public:
	CDXGraph();
	virtual ~CDXGraph();

public:
	//创建IGraphBuilder，使用CoCreateInstance
	virtual bool Create(void);
	//释放
	virtual void Release(void);
	virtual bool Attach(IGraphBuilder * inGraphBuilder);

	IGraphBuilder * GetGraph(void); // Not outstanding reference count
	IMediaEventEx * GetEventHandle(void);

	bool ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	void DisconnectFilters(IPin * inOutputPin);

	bool SetDisplayWindow(HWND inWindow);
	bool SetNotifyWindow(HWND inWindow);
	bool ResizeVideoWindow(long inLeft, long inTop, long inWidth, long inHeight);
	void HandleEvent(WPARAM inWParam, LPARAM inLParam);
	//各种操作
	bool Run(void);        // Control filter graph
	bool Stop(void);
	bool Pause(void);
	bool IsRunning(void);  // Filter graph status
	bool IsStopped(void);
	bool IsPaused(void);

	bool SetFullScreen(BOOL inEnabled);
	bool GetFullScreen(void);

	// IMediaSeeking
	bool GetCurrentPosition(double * outPosition);
	bool GetStopPosition(double * outPosition);
	bool SetCurrentPosition(double inPosition);
	bool SetStartStopPosition(double inStart, double inStop);
	bool GetDuration(double * outDuration);
	bool SetPlaybackRate(double inRate);

	// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
	bool SetAudioVolume(long inVolume);
	long GetAudioVolume(void);
	// Attention: range from -10000(left) to 10000(right), and 0 is both.
	bool SetAudioBalance(long inBalance);
	long GetAudioBalance(void);

	bool RenderFile(const char * inFile);
	bool SnapshotBitmap(const char * outFile);

private:
	void AddToObjectTable(void) ;
	void RemoveFromObjectTable(void);
	//各种QueryInterface，初始各种接口
	bool QueryInterfaces(void);
};

#endif // __H_CDXGraph__
```


 该头文件定义了CDXGraph类封装的各种DirectShow接口，以及提供的各种方法。在这里因为方法种类特别多，所以只能选择最关键的方法进行分析。下面打开CDXGraph.cpp看看如下几个方法吧：



**Create()**：用于创建IGraphBuilder



```cpp
//创建IGraphBuilder，使用CoCreateInstance
bool CDXGraph::Create(void)
{
	if (!mGraph)
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder, (void **)&mGraph)))
		{
			AddToObjectTable();

			return QueryInterfaces();
		}
		mGraph = 0;
	}
	return false;
}
```



需要注意的是，Create()调用了QueryInterfaces()

**QueryInterfaces()：**用于初始化各种接口

```cpp
//各种QueryInterface，初始各种接口
bool CDXGraph::QueryInterfaces(void)
{
	if (mGraph)
	{
		HRESULT hr = NOERROR;
		hr |= mGraph->QueryInterface(IID_IMediaControl, (void **)&mMediaControl);
		hr |= mGraph->QueryInterface(IID_IMediaEventEx, (void **)&mEvent);
		hr |= mGraph->QueryInterface(IID_IBasicVideo, (void **)&mBasicVideo);
		hr |= mGraph->QueryInterface(IID_IBasicAudio, (void **)&mBasicAudio);
		hr |= mGraph->QueryInterface(IID_IVideoWindow, (void **)&mVideoWindow);
		hr |= mGraph->QueryInterface(IID_IMediaSeeking, (void **)&mSeeking);
		if (mSeeking)
		{
			mSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
		}
		return SUCCEEDED(hr);
	}
	return false;
}
```


**Release()**：释放各种接口



```cpp
//释放
void CDXGraph::Release(void)
{
	if (mSeeking)
	{
		mSeeking->Release();
		mSeeking = NULL;
	}
	if (mMediaControl)
	{
		mMediaControl->Release();
		mMediaControl = NULL;
	}
	if (mEvent)
	{
		mEvent->Release();
		mEvent = NULL;
	}
	if (mBasicVideo)
	{
		mBasicVideo->Release();
		mBasicVideo = NULL;
	}
	if (mBasicAudio)
	{
		mBasicAudio->Release();
		mBasicAudio = NULL;
	}
	if (mVideoWindow)
	{
		mVideoWindow->put_Visible(OAFALSE);
		mVideoWindow->put_MessageDrain((OAHWND)NULL);
		mVideoWindow->put_Owner(OAHWND(0));
		mVideoWindow->Release();
		mVideoWindow = NULL;
	}
	RemoveFromObjectTable();
	if (mGraph) 
	{
		mGraph->Release(); 
		mGraph = NULL;
	}
}
```


**Run()**：播放





```cpp
bool CDXGraph::Run(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsRunning())
		{
			if (SUCCEEDED(mMediaControl->Run()))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}
```


**Stop()**：停止





```cpp
bool CDXGraph::Stop(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsStopped())
		{	
			if (SUCCEEDED(mMediaControl->Stop()))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}
```


**Pause()**：暂停





```cpp
bool CDXGraph::Pause(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsPaused())
		{	
			if (SUCCEEDED(mMediaControl->Pause()))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}
```


**SetFullScreen()**：设置全屏





```cpp
bool CDXGraph::SetFullScreen(BOOL inEnabled)
{
	if (mVideoWindow)
	{
		HRESULT hr = mVideoWindow->put_FullScreenMode(inEnabled ? OATRUE : OAFALSE);
		return SUCCEEDED(hr);
	}
	return false;
}
```


**GetDuration()**：获得视频时长





```cpp
bool CDXGraph::GetDuration(double * outDuration)
{
	if (mSeeking)
	{
		__int64 length = 0;
		if (SUCCEEDED(mSeeking->GetDuration(&length)))
		{
			*outDuration = ((double)length) / 10000000.;
			return true;
		}
	}
	return false;
}
```


**SetAudioVolume()**：设置音量





```cpp
bool CDXGraph::SetAudioVolume(long inVolume)
{
	if (mBasicAudio)
	{
		HRESULT hr = mBasicAudio->put_Volume(inVolume);
		return SUCCEEDED(hr);
	}
	return false;
}
```


**RenderFile()**：关键！





```cpp
bool CDXGraph::RenderFile(const char * inFile)
{
	if (mGraph)
	{
		WCHAR    szFilePath[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, inFile, -1, szFilePath, MAX_PATH);
		if (SUCCEEDED(mGraph->RenderFile(szFilePath, NULL)))
		{
			return true;
		}
	}
	return false;
}
```


 播放器源代码下载：[http://download.csdn.net/detail/leixiaohua1020/6453467](http://download.csdn.net/detail/leixiaohua1020/6453467)
































