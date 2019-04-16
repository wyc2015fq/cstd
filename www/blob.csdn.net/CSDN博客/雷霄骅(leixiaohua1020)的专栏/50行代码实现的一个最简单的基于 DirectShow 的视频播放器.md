# 50行代码实现的一个最简单的基于 DirectShow 的视频播放器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月26日 12:10:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：44
个人分类：[DirectShow](https://blog.csdn.net/leixiaohua1020/article/category/1645657)










本文介绍一个最简单的基于 DirectShow 的视频播放器。该播放器对于初学者来说是十分有用的，它包含了使用DirectShow 播放视频所有必备的函数。

直接贴上代码，具体代码的含义都写在注释中了：



```cpp
/* 雷霄骅
 * 中国传媒大学/数字电视技术
 * leixiaohua1020@126.com
 *
 */
// aviplayer.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <dshow.h>

// 用到的DirectShow SDK链接库
#pragma comment(lib,"strmiids.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	IGraphBuilder *pGraph = NULL;
    IMediaControl *pControl = NULL;
    IMediaEvent   *pEvent = NULL; 
    // 初始化COM库.
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        printf("错误 - 无法初始化 COM 组件");
        return -1;
    }
// 创建滤波器图表管理器
   hr=CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder, (void **)&pGraph);
    if (FAILED(hr))
    {
        printf("错误 - 无法创建 Filter Graph Manager.");
        return -1;
    }
   // 查询媒体控制和媒体事件接口
   hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
// 建立图表，在这里你可以更改待播放的文件名称
    hr = pGraph->RenderFile(L"E:\\movie\\外婆.VOB", NULL);
    if (SUCCEEDED(hr))
    {
        // 运行图表.
        hr = pControl->Run();
        if (SUCCEEDED(hr))
        {
            //等待回放结束事件.
            long evCode;
            pEvent->WaitForCompletion(INFINITE, &evCode);
           // 切记: 在实际应用当中,不能使用INFINITE标识, 因为它会不确定的阻塞程序
        }
    }
// 释放所有资源和关闭COM库
    pControl->Release();
    pEvent->Release();
    pGraph->Release();
    CoUninitialize();
	return 0;
}
```


 源代码下载（VC2010）：[http://download.csdn.net/detail/leixiaohua1020/6456581](http://download.csdn.net/detail/leixiaohua1020/6456581)








