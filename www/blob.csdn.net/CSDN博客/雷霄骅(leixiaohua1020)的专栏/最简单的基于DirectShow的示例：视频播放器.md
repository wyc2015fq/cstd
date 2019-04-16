# 最简单的基于DirectShow的示例：视频播放器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年01月10日 18:29:47[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：14764










=====================================================

最简单的基于DirectShow的示例文章列表：

[最简单的基于DirectShow的示例：视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/42372419)

[最简单的基于DirectShow的示例：视频播放器图形界面版](http://blog.csdn.net/leixiaohua1020/article/details/42372631)


[最简单的基于DirectShow的示例：视频播放器自定义版](http://blog.csdn.net/leixiaohua1020/article/details/42394509)


[最简单的基于DirectShow的示例：获取Filter信息](http://blog.csdn.net/leixiaohua1020/article/details/42649379)


=====================================================


本文记录一个最简单的基于DirectShow的视频播放器。DirectShow是一个庞大的框架，可以在Windows下实现多种多样的视频处理需求。但是它的“庞大”也使得新手不太容易学习它的使用。本文的例子正是为解决这一问题而做的，它只包含了使用DirectShow播放一个视频文件所需要的最重要的函数。

![](https://img-blog.csdn.net/20150103221023421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 流程图

最简单的使用DirectShow播放视频文件的流程如下图所示。

![](https://img-blog.csdn.net/20150103221043250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

流程图中涉及到几个接口如下所示。

> IGraphBuilder：继承自IFilterGraph，用于构建Filter Graph。相比于IFilterGraph来说IGraphBuilder提供了一些更加“智能”的方法，例如RenderFile()方法。
IMediaControl：提供和播放控制有关的一些接口。

IMediaEvent：用来处理Filter Graph发出的事件。


流程图中关键函数的作用如下所示。

> CoInitialize() ：初始化COM运行环境。
CoCreateInstance(…,pGraph) ：用指定的类标识符创建一个Com对象。在该播放器中类标识符为“CLSID_FilterGraph”，用于创建IGraphBuilder。
pGraph->QueryInterface(…,pControl) ：通过QueryInterface()查询某个组件是否支持某个特定的接口。在这里查询IMediaControl接口。
pGraph->QueryInterface(…,pEvent) ：同上。在这里查询IMediaEvent接口。
pGraph->RenderFile("xxx.mkv")：为指定的文件智能的构建一个Filter Graph。
pControl->Run() ：开始运行Filter Graph中的所有Filter。
pEvent->WaitForCompletion() ：等待Filter Graph处理完所有数据。
CoUninitialize()：释放CoInitialize()初始化的COM运行环境。


注意上述几个函数是构建一个基于DirectShow的视频播放器所必须的函数，除了上述几个接口之外还经常用到以下几个接口：

> IBasicVideo：提供和视频有关的一些接口。
IBasicAudio：提供和音频有关的一些接口。
IVideoWindow：提供和窗口有关的一些接口。
IMediaSeeking：提供和播放位置有关的一些接口。


## 源代码

```cpp
/**
 * 最简单的基于DirectShow的视频播放器
 * Simplest DirectShow Player
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序是一个最简单的基于DirectShow的播放器。
 * 适合初学者学习DirectShow。
 *
 * This example is the simplest Player based on DirectShow.
 * Suitable for the beginner of DirectShow.
 */

#include "stdafx.h"

#include <dshow.h>
#include <atlconv.h>


#define OUTPUT_INFO 1

//Show Filter in FilterGpragh
int show_filters_in_filtergraph(IGraphBuilder *pGraph){
	printf("Filters in FilterGpragh=======\n");
	USES_CONVERSION;
	IEnumFilters *pFilterEnum=NULL;
	if(FAILED(pGraph->EnumFilters(&pFilterEnum))){
		pFilterEnum->Release();
		return -1;
	}
	pFilterEnum->Reset();
	IBaseFilter * filter = NULL;
	ULONG fetchCount = 0;
	//Pin Info
	while (SUCCEEDED(pFilterEnum->Next(1, &filter, &fetchCount)) && fetchCount){
		if (!filter){
			continue;
		}
		FILTER_INFO FilterInfo;
		if (FAILED(filter->QueryFilterInfo(&FilterInfo))){
			continue;
		}
		printf("[%s]\n",W2A(FilterInfo.achName));
		filter->Release();
	}
	pFilterEnum->Release();
	printf("==============================\n");
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	IGraphBuilder *pGraph = NULL;
    IMediaControl *pControl = NULL;
    IMediaEvent   *pEvent = NULL; 
	//Get some param--------------
	HRESULT hr1;
	IBasicVideo *pVideo=NULL;
	IBasicAudio *pAudio=NULL;
	IVideoWindow *pWindow=NULL;
	IMediaSeeking *pSeeking=NULL;
	
	
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
   //  Query Interface
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	// RenderFile
    hr = pGraph->RenderFile(L"cuc_ieschool.mov", NULL);
	if (FAILED(hr)){
		printf("Error - Can't Render File.");
		return -1;
	}
#if OUTPUT_INFO
	//Get some information----------
	long video_w=0,video_h=0,video_bitrate=0,audio_volume=0;
	long long duration_1=0,position_1=0;
	REFTIME avgtimeperframe=0;
	float framerate=0,duration_sec=0,progress=0,position_sec=0;
	//Video
	hr1=pGraph->QueryInterface(IID_IBasicVideo, (void **)&pVideo);
	pVideo->get_VideoWidth(&video_w);
	pVideo->get_VideoHeight(&video_h);
	pVideo->get_AvgTimePerFrame(&avgtimeperframe);
	framerate=1/avgtimeperframe;
	//pVideo->get_BitRate(&video_bitrate);
	//Audio
	hr1=pGraph->QueryInterface(IID_IBasicAudio, (void **)&pAudio);
	//Mute
	//pAudio->put_Volume(-10000);
	printf("Some Information:\n");
	printf("Video Resolution:\t%dx%d\n",video_w,video_h);
	printf("Video Framerate:\t%.3f\n",framerate);
	//Window
	hr1=pGraph->QueryInterface(IID_IVideoWindow, (void **)&pWindow);
	pWindow->put_Caption(L"Simplest DirectShow Player");
	//pWindow->put_Width(480);
	//pWindow->put_Height(272);
	//Seek
	hr1=pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pSeeking);
	pSeeking->GetDuration(&duration_1);
	//time unit:100ns=0.0000001s
	duration_sec=(float)duration_1/10000000.0;
	printf("Duration:\t%.2f s\n",duration_sec);
	//pSeeking->SetPositions();
	//PlayBack Rate
	//pSeeking->SetRate(2.0);

	//Show Filter in FilterGpagh
	show_filters_in_filtergraph(pGraph);
	//----------------------
#endif

	printf("Progress Info\n");
	printf("Position\tProgress\n");
    if (SUCCEEDED(hr)){
        // Run
        hr = pControl->Run();
        if (SUCCEEDED(hr)){
			long evCode=0;
			//pEvent->WaitForCompletion(INFINITE, &evCode);
			while(evCode!=EC_COMPLETE){
				//Info
#if OUTPUT_INFO
				pSeeking->GetCurrentPosition(&position_1);
				position_sec=(float)position_1/10000000.0;
				progress=position_sec*100/duration_sec;
				printf("%7.2fs\t%5.2f%%\n",position_sec,progress);
#endif
				//1000ms
				pEvent->WaitForCompletion(1000, &evCode);
			}
        }
    }
	// Release resource
    pControl->Release();
    pEvent->Release();
    pGraph->Release();
    CoUninitialize();
	return 0;
}
```


## 运行结果
程序运行后即可开始播放一个“cuc_ieschool.mov”文件。程序运行时候的截图如下所示。由图可见运行的同时程序在控制台中打印出了两种信息：
（1）该视频的相关信息

（2）播放该视频的 Filter Graph中的Filter（该功能通过函数show_filters_in_filtergraph()完成）。

![](https://img-blog.csdn.net/20150103221227132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以通过定义在代码最前面宏OUTPUT_INFO控制是否输出视频的信息。定义成“0”的话则不会输出视频的信息。如下所示。

```cpp
#define OUTPUT_INFO 1
```


## 下载

****

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





