# 最简单的基于DirectShow的示例：视频播放器自定义版 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年01月11日 18:05:19[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：11710










=====================================================

最简单的基于DirectShow的示例文章列表：

[最简单的基于DirectShow的示例：视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/42372419)

[最简单的基于DirectShow的示例：视频播放器图形界面版](http://blog.csdn.net/leixiaohua1020/article/details/42372631)


[最简单的基于DirectShow的示例：视频播放器自定义版](http://blog.csdn.net/leixiaohua1020/article/details/42394509)


[最简单的基于DirectShow的示例：获取Filter信息](http://blog.csdn.net/leixiaohua1020/article/details/42649379)


=====================================================


本文记录一个简单的基于DirectShow的自定义的视频播放器。这里所说的“自定义播放器”，实际上指的是自己在Filter Graph中手动逐个添加Filter，并且连接这些Filter的后运行的播放器。这么做相对于使用RenderFile()这种“智能”创建Filter Graph的方法来说要复杂不少，但是可以让我们更加了解DirectShow的体系。

![](https://img-blog.csdn.net/20150104165914203)


## 流程图

最简单的基于DirectShow的自定义的视频播放器的流程如下图所示。

![](https://img-blog.csdn.net/20150104165956906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该流程图中包含如下变量：

> IGraphBuilder *pGraph：继承自IFilterGraph，用于构建Filter Graph。
IMediaControl *pControl：提供和播放控制有关的一些接口。
IMediaEvent   *pEvent：用来处理Filter Graph发出的事件。
IBaseFilter *pF_source：源Filter。
IFileSourceFilter* pFileSource：源Filter的暴露的接口，用于设置输入文件的路径。
IBaseFilter *pF_demuxer：解复用Filter。
IBaseFilter *pF_decoder：解码Filter。
IBaseFilter *pF_render：渲染Filter。
IPin *pOut：输出Pin。
IPin *pIn：输入Pin。
IPin **pPin：内部变量Pin。
该流程图大体上可以分成以下步骤：
**（1）       初始化DirectShow**
包括以下几个步骤：

> a)       CoInitialize()：初始化COM运行环境。
b)       CoCreateInstance(…,pGraph)：用指定的类标识符创建一个Com对象。在这里创建IGraphBuilder。
c)       pGraph->QueryInterface(…,pControl)：通过QueryInterface()查询某个组件是否支持某个特定的接口。在这里查询IMediaControl接口。
d)       pGraph->QueryInterface(…,pEvent)：同上。在这里查询IMediaEvent接口。
**（2）       添加Source Filter**
包括以下几个步骤：

> a)       CoCreateInstance(…,pF_source)：创建Source Filter。
b)       pGraph->AddFilter(pF_source,…)：将Source Filter加入Filter Graph。
c)       pF_source->QueryInterface(…,pFileSource)：查找Source Filter的IFileSourceFilter接口。
d)       pFileSource->Load(L"xxx.mpg",pF_source)：调用IFileSourceFilter的Load()方法加载视频文件。
**（3）       添加Demuxer Filter**
包括以下几个步骤：

> a)       CoCreateInstance(…,pF_demuxer)：创建Demuxer Filter。
b)       pGraph->AddFilter(pF_demuxer,…)：将Demuxer Filter加入Filter Graph。
**（4）       添加Decoder Filter**
包括以下几个步骤：

> a)       CoCreateInstance(…,pF_decoder)：创建Decoder Filter。
b)       pGraph->AddFilter(pF_decoder,…)：将Decoder Filter加入Filter Graph。
**（5）       添加Render Filter**
包括以下几个步骤：

> a)       CoCreateInstance(…,pF_render)：创建Render Filter。
b)       pGraph->AddFilter(pF_render,…)：将Render Filter加入Filter Graph。
**（6）       连接Source Filter和Demuxer Filter**
调用了一个函数connect_filters()用于连接2个Filter。
connect_filters()的执行步骤如下：

> a)       调用get_unconnected_pin()从源Filter中选择一个没有链接的输出Pin。
b)       调用get_unconnected_pin()从目的Filter中选择一个没有链接的输入Pin。
c)       连接这两个Pin
get_unconnected_pin()的执行步骤如下：

> a)       枚举Filter上的Pin。
b)       遍历这些Pin，查找符合输出方向（通过IPin的QueryDirection()方法），而且没有在使用的Pin（通过IPin的ConnectedTo()方法）。
**（7）       连接Demuxer Filter和Decoder Filter**
过程同上。
**（8）       连接Decoder Filter和Render Filter**
过程同上。
**（9）       开始播放**
包括以下步骤：

> pControl->Run()：开始运行Filter Graph中的所有Filter。
pEvent->WaitForCompletion()：等待Filter Graph处理完所有数据。


上述步骤可以理解为在GraphEdit软件中分别按照步骤添加以下控件。其中（1）、（2）、（3）、（4）为先添加的4个Filter，（5）、（6）、（7）为Filter之间的连接线。

![](https://img-blog.csdn.net/20150104170146022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## 源代码

```cpp
/**
 * 最简单的基于DirectShow的视频播放器（Custom）
 * Simplest DirectShow Player (Custom)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序是一个简单的基于DirectShow的视频播放器。该播放器通过逐个添加
 * 滤镜并连接这些滤镜实现了视频的播放。适合初学者学习DirectShow。
 * 
 * This software is a simple video player based on DirectShow.
 * It Add DirectShow Filter Manually and Link the Pins of these filters
 * to play videos.Suitable for the beginner of DirectShow.
 */

#include "stdafx.h"
#include <dshow.h>
//'1':Add filters manually
//'0':Add filters automatically
#define ADD_MANUAL 1

//Find unconnect pins
HRESULT get_unconnected_pin(
	IBaseFilter *pFilter, // Pointer to the filter.
	PIN_DIRECTION PinDir, // Direction of the pin to find.
	IPin **ppPin) // Receives a pointer to the pin.
{
	*ppPin = 0;
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return hr;
	}
	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr)) // Already connected, not the pin we want.
			{
				pTmp->Release();
			}
			else // Unconnected, the pin we want.
			{
				pEnum->Release();
				*ppPin = pPin;
				return S_OK;
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	// Did not find a matching pin.
	return E_FAIL;
}

//Connect 2 filters
HRESULT connect_filters(
	IGraphBuilder *pGraph, 
	IBaseFilter *pSrc, 
	IBaseFilter *pDest)
{
	if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}
	//Find Output pin in source filter
	IPin *pOut = 0;
	HRESULT hr = NULL;
	hr=get_unconnected_pin(pSrc, PINDIR_OUTPUT, &pOut);
	if (FAILED(hr)){
		return hr;
	}
	//Find Input pin in destination filter
	IPin *pIn = 0;
	hr = get_unconnected_pin(pDest, PINDIR_INPUT, &pIn);
	if (FAILED(hr)){
		return hr;
	}
	//Connnect them
	hr = pGraph->Connect(pOut, pIn);
	pIn->Release();
	pOut->Release();
	return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
	IGraphBuilder *pGraph = NULL;
    IMediaControl *pControl = NULL;
    IMediaEvent   *pEvent = NULL; 
    // Init COM
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)){
        printf("Error - Can't init COM.");
        return -1;
    }

	// Create FilterGraph
   hr=CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder, (void **)&pGraph);
    if (FAILED(hr)){
        printf("Error - Can't create Filter Graph.");
        return -1;
    }
   // Query Interface
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	//1. Add Filters=======================
	//Source
	IBaseFilter *pF_source = 0;
	hr = CoCreateInstance(CLSID_AsyncReader, 0, CLSCTX_INPROC_SERVER,IID_IBaseFilter, (void**)(&pF_source));
	if (FAILED(hr)){
		printf("Failed to create File Source.\n");
		return -1;
	}
	hr = pGraph->AddFilter(pF_source, L"Lei's Source");
	if (FAILED(hr)){
		printf("Failed to add File Source to Filter Graph.\n");
		return -1;
	}
	IFileSourceFilter* pFileSource;
	pF_source->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSource);
	pFileSource->Load(L"cuc_ieschool.mpg", NULL);
	pFileSource->Release();

#if ADD_MANUAL
	//Demuxer
	IBaseFilter *pF_demuxer = 0;
	hr = CoCreateInstance(CLSID_MPEG1Splitter, 0, CLSCTX_INPROC_SERVER,IID_IBaseFilter, (void**)(&pF_demuxer));
	if (FAILED(hr)){
		printf("Failed to create Demuxer.\n");
		return -1;
	}
	hr = pGraph->AddFilter(pF_demuxer, L"Lei's Demuxer");
	if (FAILED(hr)){
		printf("Failed to add Demuxer to Filter Graph.\n");
		return -1;
	}
	//Decoder
	IBaseFilter *pF_decoder = 0;
	hr = CoCreateInstance(CLSID_CMpegVideoCodec, 0, CLSCTX_INPROC_SERVER,IID_IBaseFilter, (void**)(&pF_decoder));
	if (FAILED(hr)){
		printf("Failed to create Decoder.\n");
		return -1;
	}
	hr = pGraph->AddFilter(pF_decoder, L"Lei's Decoder");
	if (FAILED(hr)){
		printf("Failed to add Decoder to Filter Graph.\n");
		return -1;
	}
	//Render
	IBaseFilter *pF_render = 0;
	hr = CoCreateInstance(CLSID_VideoRenderer, 0, CLSCTX_INPROC_SERVER,IID_IBaseFilter, (void**)(&pF_render));
	if (FAILED(hr)){
		printf("Failed to create Video Render.\n");
		return -1;
	}
	hr = pGraph->AddFilter(pF_render, L"Lei's Render");
	if (FAILED(hr)){
		printf("Failed to add Video Render to Filter Graph.\n");
		return -1;
	}
	//2. Connect Filters=======================
	hr = connect_filters(pGraph, pF_source, pF_demuxer);
	if (FAILED(hr)){
		printf("Failed to link Source and Demuxer.\n");
		return -1;
	}
	hr = connect_filters(pGraph, pF_demuxer, pF_decoder);
	if (FAILED(hr)){
		printf("Failed to link Demuxer and Decoder.\n");
		return -1;
	}
	hr = connect_filters(pGraph, pF_decoder, pF_render);
	if (FAILED(hr)){
		printf("Failed to link Decoder and Render.\n");
		return -1;
	}

	pF_source->Release();
	pF_demuxer->Release();
	pF_decoder->Release();
	pF_render->Release();
#else
	IPin*	 Pin;
	ULONG	 fetched;
	//	get output pin
	IEnumPins* pEnumPins;
	hr = pF_source->EnumPins(&pEnumPins);
	hr = pEnumPins->Reset();
	hr = pEnumPins->Next(1, &Pin, &fetched);
	pEnumPins->Release();
	//	render pin, graph builder automatically complete rest works
	hr = pGraph->Render(Pin);
#endif

    if (SUCCEEDED(hr)){
        // Run
        hr = pControl->Run();
        if (SUCCEEDED(hr)){
			long evCode=0;
			pEvent->WaitForCompletion(INFINITE, &evCode);
        }
    }
	//Release
    pControl->Release();
    pEvent->Release();
    pGraph->Release();
    CoUninitialize();
	return 0;
}
```


## 运行结果

程序的运行结果如下图所示。运行后会播放“cuc_ieschool.mpg”文件。需要注意的是，本程序并没有加入音频解码和播放的Filter，所以播放视频的时候是没有声音的。

![](https://img-blog.csdn.net/20150104170338686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

除了手动一个一个添加Filter之外，也可以在获得“源”Filter的Pin之后，直接调用IFilterGraph的Render()方法“智能”自动构建Filter Graph。注意Render()方法和RenderFile()方法是不一样的。RenderFile()是指定一个文件路径后，自动构建整个Filter Graph，相对来说更加简单些；而Render()方法则是首先要创建一个Source Filter之后，才可以自动构建整个Filter Graph。
可以通过修改源文件首部的宏定义ADD_MANUAL来设定是否手动添加Filter，如下所示。

```cpp
//'1':Add filters manually
//'0':Add filters automatically
#define ADD_MANUAL 1
```


## 下载

**Simplest DirectShow Example**

**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestdirectshowexample/](https://sourceforge.net/projects/simplestdirectshowexample/)

Github：[https://github.com/leixiaohua1020/simplest_directshow_example](https://github.com/leixiaohua1020/simplest_directshow_example)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_directshow_example](http://git.oschina.net/leixiaohua1020/simplest_directshow_example)

CDSN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8348163](http://download.csdn.net/detail/leixiaohua1020/8348163)

本程序包含了DirectShow开发的示例程序。适合DirectShow初学者进行学习。
它包含了以下几个子程序：
simplest_directshow_player: 最简单的基于DirectShow的视频播放器。
simplest_directshow_player_custom: 最简单的基于DirectShow的视频播放器（Custom）。
playerGUI: 最简单的基于DirectShow的播放器-图形界面版。
simplest_directshow_info: 最简单的Directshow信息显示例子。
simplest_directshow_filter: 目前还未完成。



